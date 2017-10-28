#include "TrueTypeHelper.hpp"

TrueTypeHelper::TrueTypeHelper()
{
}

TrueTypeHelper::~TrueTypeHelper()
{
}

void TrueTypeHelper::initalizeFace()
{
	FT_Library ft;
	
	if(FT_Init_FreeType(&ft))
		cout << "error: counld not initalize FreeType" << endl;
	FT_Face face;
	if(FT_New_Face(ft,"/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", 0, &face))
		cout << "error: could not load font" << endl;
	FT_Set_Pixel_Sizes(face, 0, 48);
		
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	
	for(GLubyte c = 0; c < 128; c++)
	{
		if(FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			cout << "error: failed to load glyth" << endl;
		}
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		
		Character character = {
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::vec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			(GLuint)face->glyph->advance.x
		};
		Characters.insert(std::pair<GLchar, Character>(c, character));
	}
	
	FT_Done_Face(face);
	FT_Done_FreeType(ft);
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void TrueTypeHelper::RenderText(Shader &s, string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color)
{
	s.use();
	glUniform3f(glGetUniformLocation(s.Id, "textColor"), color.x, color.y, color.z);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(VAO);
	
	string::const_iterator c;
	for(c = text.begin(); c != text.end(); c++)
	{
		Character ch = Characters[*c];
		
		GLfloat xpos = x + ch.Bearing.x * scale;
		GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;
		
		GLfloat w = ch.Size.x * scale;
		GLfloat h = ch.Size.y * scale;
		
		GLfloat vertices[6][4] = {
			{ xpos,		ypos + h,	0.0, 0.0 },
			{ xpos,		ypos,		0.0, 1.0 },
			{ xpos + w,	ypos,		1.0, 1.0 },
			
			{ xpos,		ypos + h,	0.0, 0.0 },
			{ xpos + w,	ypos,		1.0, 1.0 },
			{ xpos + w,	ypos + h,	1.0, 0.0 }
		};
		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		
		x += (ch.Advance >> 6) * scale;
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}