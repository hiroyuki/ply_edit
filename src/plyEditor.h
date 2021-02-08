#pragma once

#include "example-utils.hpp"
#include "ofMain.h"

using namespace tinyply;

class plyEditor
{
public:
	void loadPLY(string filepath)
	{
		std::cout << "........................................................................\n";
		std::cout << "Now Reading: " << filepath << std::endl;
		std::unique_ptr<std::istream> file_stream;
		std::vector<uint8_t> byte_buffer;
		bool preload_into_memory = true;
		try
		{
			// For most files < 1gb, pre-loading the entire file upfront and wrapping it into a 
			// stream is a net win for parsing speed, about 40% faster. 
			if (preload_into_memory)
			{
				byte_buffer = read_file_binary(filepath);
				file_stream.reset(new memory_stream((char*)byte_buffer.data(), byte_buffer.size()));
			}
			else
			{
				file_stream.reset(new std::ifstream(filepath, std::ios::binary));
			}

			if (!file_stream || file_stream->fail()) throw std::runtime_error("file_stream failed to open " + filepath);

			file_stream->seekg(0, std::ios::end);
			const float size_mb = file_stream->tellg() * float(1e-6);
			file_stream->seekg(0, std::ios::beg);

			PlyFile file;
			file.parse_header(*file_stream);

			std::cout << "\t[ply_header] Type: " << (file.is_binary_file() ? "binary" : "ascii") << std::endl;
			for (const auto & c : file.get_comments()) std::cout << "\t[ply_header] Comment: " << c << std::endl;
			for (const auto & c : file.get_info()) std::cout << "\t[ply_header] Info: " << c << std::endl;

			for (const auto & e : file.get_elements())
			{
				std::cout << "\t[ply_header] element: " << e.name << " (" << e.size << ")" << std::endl;
				for (const auto & p : e.properties)
				{
					std::cout << "\t[ply_header] \tproperty: " << p.name << " (type=" << tinyply::PropertyTable[p.propertyType].str << ")";
					if (p.isList) std::cout << " (list_type=" << tinyply::PropertyTable[p.listType].str << ")";
					std::cout << std::endl;
				}
			}


			// The header information can be used to programmatically extract properties on elements
			// known to exist in the header prior to reading the data. For brevity of this sample, properties 
			// like vertex position are hard-coded: 
			try { vertices = file.request_properties_from_element("vertex", { "x", "y", "z" }); }
			catch (const std::exception & e) { std::cerr << "tinyply exception: " << e.what() << std::endl; }

			try { normals = file.request_properties_from_element("vertex", { "nx", "ny", "nz" }); }
			catch (const std::exception & e) { std::cerr << "tinyply exception: " << e.what() << std::endl; }

			try { colors = file.request_properties_from_element("vertex", { "red", "green", "blue" }); }
			catch (const std::exception & e) { std::cerr << "tinyply exception: " << e.what() << std::endl; }

			try { texcoords = file.request_properties_from_element("vertex", { "u", "v" }); }
			catch (const std::exception & e) { std::cerr << "tinyply exception: " << e.what() << std::endl; }

			// Providing a list size hint (the last argument) is a 2x performance improvement. If you have 
			// arbitrary ply files, it is best to leave this 0. 
			try { faces = file.request_properties_from_element("face", { "vertex_indices" }, 3); }
			catch (const std::exception & e) { std::cerr << "tinyply exception: " << e.what() << std::endl; }

			// Tristrips must always be read with a 0 list size hint (unless you know exactly how many elements
			// are specifically in the file, which is unlikely); 
			try { tripstrip = file.request_properties_from_element("tristrips", { "vertex_indices" }, 0); }
			catch (const std::exception & e) { std::cerr << "tinyply exception: " << e.what() << std::endl; }

			manual_timer read_timer;

			read_timer.start();
			file.read(*file_stream);
			read_timer.stop();

			const float parsing_time = static_cast<float>(read_timer.get()) / 1000.f;
			std::cout << "\tparsing " << size_mb << "mb in " << parsing_time << " seconds [" << (size_mb / parsing_time) << " MBps]" << std::endl;

			if (vertices)   std::cout << "\tRead " << vertices->count << " total vertices " << std::endl;
			if (normals)    std::cout << "\tRead " << normals->count << " total vertex normals " << std::endl;
			if (colors)     std::cout << "\tRead " << colors->count << " total vertex colors " << std::endl;
			if (texcoords)  std::cout << "\tRead " << texcoords->count << " total vertex texcoords " << std::endl;
			if (faces)      std::cout << "\tRead " << faces->count << " total faces (triangles) " << std::endl;
			if (tripstrip)  std::cout << "\tRead " << (tripstrip->buffer.size_bytes() / tinyply::PropertyTable[tripstrip->t].stride) << " total indicies (tristrip) " << std::endl;

			// Example One: converting to your own application types
			{
				const size_t numVerticesBytes = vertices->buffer.size_bytes();
				std::vector<float3> verts(vertices->count);
				std::memcpy(verts.data(), vertices->buffer.get(), numVerticesBytes);
			}

			// Example Two: converting to your own application type
			{
				std::vector<float3> verts_floats;
				std::vector<double3> verts_doubles;
				if (vertices->t == tinyply::Type::FLOAT32) { /* as floats ... */ }
				if (vertices->t == tinyply::Type::FLOAT64) { /* as doubles ... */ }
			}
			m.setMode(OF_PRIMITIVE_POINTS);
			m.addVertices(reinterpret_cast<glm::vec3 *>(vertices->buffer.get()), vertices->count);
			
			std::vector<unsigned char> cols(colors->count*3);
			std::vector<ofColor> colorVecs;
			memcpy(cols.data(), reinterpret_cast<unsigned char *>(colors->buffer.get()), colors->count*3);
			for (int i = 0; i < cols.size(); i += 3)
			{
				m.addColor(ofColor(cols[i], cols[i + 1], cols[i + 2]));
			}

		}
		catch (const std::exception & e)
		{
			std::cerr << "Caught tinyply exception: " << e.what() << std::endl;
		}
	}

	void save(string filename, float hue, float saturation, float brightness)
	{
		std::vector<ofFloatColor> _colors = m.getColors();
		unsigned char * colorsv = new unsigned char[_colors.size()*3];
		int index = 0;
		for (auto& c : _colors)
		{
			c.setHue(c.getHue() * hue);
			c.setSaturation(c.getSaturation() * saturation);
			c.setBrightness(c.getBrightness() * brightness);
			colorsv[index] = c.r * 255;
			colorsv[index + 1] = c.g * 255;
			colorsv[index + 2] = c.b * 255;
			index += 3;
		}

		std::filebuf fb_binary;
		fb_binary.open("data/"+filename, std::ios::out | std::ios::binary);
		std::ostream outstream_binary(&fb_binary);
		if (outstream_binary.fail()) throw std::runtime_error("failed to open " + filename);

		PlyFile exportFile;

		exportFile.add_properties_to_element("vertex", { "x", "y", "z" },
			Type::FLOAT32, vertices->count, reinterpret_cast<uint8_t*>(vertices->buffer.get()), Type::INVALID, 0);

		exportFile.add_properties_to_element("vertex", { "nx", "ny", "nz" },
			Type::FLOAT32, normals->count, reinterpret_cast<uint8_t*>(normals->buffer.get()), Type::INVALID, 0);

		exportFile.add_properties_to_element("vertex", { "red", "green", "blue" },
			Type::UINT8, colors->count, colorsv, Type::INVALID, 0);

		exportFile.get_comments().push_back("generated by tinyply 2.3");

		//// Write an ASCII file
		//cube_file.write(outstream_ascii, false);

		// Write a binary file
		exportFile.write(outstream_binary, true);
		delete[] colorsv;
	}

	void changeColorAll(ofColor color)
	{

	}

	void debugDraw()
	{
		ofPushStyle();
		m.drawVertices();
		ofPopStyle();
	}

	// Because most people have their own mesh types, tinyply treats parsed data as structured/typed byte buffers. 
	// See examples below on how to marry your own application-specific data structures with this one. 
	std::shared_ptr<PlyData> vertices, normals, colors, texcoords, faces, tripstrip;
	ofMesh m;
};
