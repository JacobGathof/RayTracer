#include "glew.h"
#include <iostream>
#include <fstream>

class ShaderProgram
{
public:
	/*Unique Shader ID assign by GL*/
	int shader_program;

	void ShaderProgram::loadFloat(char * location, float value) {
		glUniform1f(glGetUniformLocation(this->shader_program, location), value);
	}

	void compileShader(char * file_vertex, char * file_geo, char * file_fragment)
	{

		shader_program = glCreateProgram();

		int shader_vertex = glCreateShader(GL_VERTEX_SHADER);
		const char* source_vertex = getShaderSource(file_vertex);
		glShaderSource(shader_vertex, 1, &source_vertex, 0);
		glCompileShader(shader_vertex);
		glAttachShader(shader_program, shader_vertex);
		delete[] source_vertex;

		if (file_geo != NULL) {
			int shader_geometry = glCreateShader(GL_GEOMETRY_SHADER);
			const char* source_geometry = getShaderSource(file_geo);
			glShaderSource(shader_geometry, 1, &source_geometry, 0);
			glCompileShader(shader_geometry);
			glAttachShader(shader_program, shader_geometry);

			checkCompileErrors(shader_geometry, 'g');
			delete[] source_geometry;
		}


		int shader_fragment = glCreateShader(GL_FRAGMENT_SHADER);
		const char* source_fragment = getShaderSource(file_fragment);
		glShaderSource(shader_fragment, 1, &source_fragment, 0);
		glCompileShader(shader_fragment);
		glAttachShader(shader_program, shader_fragment);
		delete[] source_fragment;


		checkCompileErrors(shader_vertex, 'v');
		checkCompileErrors(shader_fragment, 'f');


		glLinkProgram(shader_program);
		checkLinkErrors(shader_program);
		glUseProgram(shader_program);

	}

	void ShaderProgram::use()
	{
		glUseProgram(this->shader_program);
	}

	void ShaderProgram::checkCompileErrors(int shader, char type)
	{
		int success;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

		if (!success) {
			char infolog[2048];
			glGetShaderInfoLog(shader, 2048, 0, infolog);
			std::cout << "Shader : " << ((type == 'v') ? "Vertex Shader" : (type == 'f') ? "Fragment Shader" : "Geometry Shader") << "\n";
			std::cout << infolog << std::endl;
			std::cout << "--------------------\n";
		}

	}

	void ShaderProgram::checkLinkErrors(int program)
	{
		int success;
		glGetShaderiv(program, GL_LINK_STATUS, &success);

		if (!success) {
			char infolog[2048];
			glGetShaderInfoLog(program, 2048, 0, infolog);
			std::cout << "Program Link Errors\n";
			std::cout << infolog << std::endl;
		}

	}

	const char* ShaderProgram::getShaderSource(char * filename)
	{

		std::ifstream file;
		file.open(filename, file.binary | file.in);

		char *buffer;
		int size;

		file.seekg(0, file.end);
		size = (int)file.tellg();
		file.seekg(0, file.beg);

		buffer = new char[size + 1];
		file.read(buffer, size);

		buffer[size] = '\0';

		return buffer;

	}

};