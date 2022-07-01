#pragma once
class Renderer
{
public:
	Renderer();
	~Renderer();
	void Draw();
	void Init();

private:
	
private:
	class VertexArray* _va;
	class Texture* _texture;
	class Shader* _shader;

};

