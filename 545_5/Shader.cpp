#include "Shader.hpp"

using namespace std;

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
	//read in files
	string vertexCode;
	string fragmentCode;
	ifstream vShaderFile;
	ifstream fShaderFile;
	
	vShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
	fShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
	
	try
	{
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		stringstream vShaderStream, fShaderStream;
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		
		vShaderFile.close();
		fShaderFile.close();
		
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch(ifstream::failure e)
	{
		cout << "Error: Shader file not successfully read" << endl;
	}
	
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();
	
	unsigned int vertex, fragment;
	int success;
	char infoLog[512];
	//vertex shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		cout << "Error: Vertex Shader compilation failed \n" << infoLog << endl;
	}
	//fragment shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		cout << "Error: Fragment Shader compilation failed \n" << infoLog << endl;
	}
	
	Id = glCreateProgram();
	glAttachShader(Id, vertex);
	glAttachShader(Id, fragment);
	glLinkProgram(Id);
	
	glGetProgramiv(Id, GL_LINK_STATUS, &success);
	if(!success)
	{
		glGetProgramInfoLog(Id, 512, NULL, infoLog);
		cout << "Error: Shader Program linking failed\n" << infoLog << endl;
	}
	
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

void Shader::use()
{
	glUseProgram(Id);
}

void Shader::setBool(const std::string &name, bool value) const
{
	glUniform1i(glGetUniformLocation(Id, name.c_str()), (int)value); 
}

void Shader::setInt(const std::string &name, int value) const
{
	glUniform1i(glGetUniformLocation(Id, name.c_str()), value); 
}

void Shader::setFloat(const std::string &name, float value) const
{
	glUniform1f(glGetUniformLocation(Id, name.c_str()), value); 
}
void Shader::set4f(const std::string &name, float valueX, float valueY, float valueZ, float valueT) const
{
	glUniform4f(glGetUniformLocation(Id, name.c_str()), valueX, valueY, valueZ, valueT); 
} 