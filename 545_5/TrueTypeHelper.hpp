#ifndef TRUETYPEHELPER_HPP
#define TRUETYPEHELPER_HPP

#include <glad/glad.h>
#include <ft2build.h>
#include <freetype/freetype.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
//#include FT_FREETYPE_H

#include <string>
#include <map>
#include <iostream>

#include "Shader.hpp"

using namespace std;

struct Character {
    GLuint     TextureID;  // ID handle of the glyph texture
    glm::ivec2 Size;       // Size of glyph
    glm::ivec2 Bearing;    // Offset from baseline to left/top of glyph
    GLuint     Advance;    // Offset to advance to next glyph
};

class TrueTypeHelper
{
public:
	map<GLchar, Character> Characters;
	GLuint VAO, VBO;
	
	TrueTypeHelper();
	~TrueTypeHelper();
	void initalizeFace();
	void RenderText(Shader &s, string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);
};

#endif // TRUETYPEHELPER_HPP
