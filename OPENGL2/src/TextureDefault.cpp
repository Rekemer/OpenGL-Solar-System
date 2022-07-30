#include "TextureDefault.h"

#include <glew.h>

#include "SOIL.h"
#include "Rendering/Debuger.h"



TextureDefault::~TextureDefault()
{
	Unload();
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
		0, // Border � "this value must be 0"
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