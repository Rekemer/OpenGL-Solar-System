#include "TextureDefault.h"

#include <glew.h>
#include <vector>

#include "SOIL.h"
#include "Rendering/Debuger.h"

std::vector<std::string> faces
{
		"right.jpg",
		"left.jpg",
		"top.jpg",
		"bottom.jpg",
		"front.jpg",
		"back.jpg"
};

TextureDefault::~TextureDefault()
{
	Unload();
}


void TextureDefault::LoadCubeMap(const char* mapDir)
{
	;
	// assumes that the six texture image files are named xp, xn, yp, yn, zp, zn and are JPG
	std::string xp = mapDir; xp = xp + "/right.jpg";
	std::string xn = mapDir; xn = xn + "/left.jpg";
	std::string yp = mapDir; yp = yp + "/top.jpg";
	std::string yn = mapDir; yn = yn + "/bottom.jpg";
	std::string zp = mapDir; zp = zp + "/center.jpg";
	std::string zn = mapDir; zn = zn + "/back.jpg";
	_textureID = SOIL_load_OGL_cubemap(xp.c_str(), xn.c_str(), yp.c_str(), yn.c_str(),
		zp.c_str(), zn.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	if (_textureID == 0) std::cout << "didnt find cube map image file" << std::endl;
	glBindTexture(GL_TEXTURE_CUBE_MAP, _textureID);
	// reduce seams
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void TextureDefault::Load(std::string fileName)
{
	int channels = 0;

	unsigned char* image = SOIL_load_image(
		fileName.c_str(),
		&_width,
		&_height,
		&channels,
		SOIL_LOAD_AUTO
	);
	if (image == nullptr)
	{
		std::cout<<("[SOIL] Failed to load image %s", fileName.c_str(), SOIL_last_result()) << "\n";
		return;
	}
	int format = GL_RGB;
	if (channels == 4)
	{
		format = GL_RGBA;
	}
	else if (channels == 1)
	{
		format = GL_RED;
	}
	GLCall(glGenTextures(1, &_textureID));
	GLCall(glBindTexture(GL_TEXTURE_2D, _textureID));

	GLCall(glTexImage2D(
		GL_TEXTURE_2D, // Texture target
		0, // Level of detail (for now, assume 0)
		format, // Color format OpenGL should use
		_width, // Width of texture
		_height, // Height of texture
		0, // Border – "this value must be 0"
		format, // Color format of input data
		GL_UNSIGNED_BYTE,// Bit depth of input data
		// Unsigned byte specifies 8-bit channels
		image // Pointer to image data
	));

	SOIL_free_image_data(image);

	// bilinear  filtering
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
//	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR));
	GLCall(glGenerateMipmap(GL_TEXTURE_2D));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

void TextureDefault::Unload()
{
	GLCall(glDeleteTextures(1, &_textureID));
}

void TextureDefault::Bind()
{
	GLCall(glBindTexture(GL_TEXTURE_2D, _textureID));
}
