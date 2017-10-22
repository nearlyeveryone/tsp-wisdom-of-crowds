#ifndef SHADER_HPP
#define SHADER_HPP

#include <glad/glad.h>
  
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
	unsigned int Id;
	
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
	void use();
	void setBool(const std::string &name, bool value) const;
	void setInt(const std::string &name, int value) const;
	void setFloat(const std::string &name, float value) const;
	void set4f(const std::string &name, float valueX, float valueY, float valueZ, float valueT) const;

};

#endif // SHADER_HPP
