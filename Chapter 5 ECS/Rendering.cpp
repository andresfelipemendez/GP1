#include "Rendering.h"
#include "Math.h"
#include "SDL.h"
#include "GL/glew.h"
#include <fstream>
#include <sstream>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

void LoadMesh(entt::registry *registry, std::vector<float> vertices,
              std::vector<unsigned int> indices) {

  unsigned int vertexArray;
  unsigned int vertexBuffer;
  unsigned int indexBuffer;
  glGenVertexArrays(1, &vertexArray);
  glBindVertexArray(vertexArray);

  glGenBuffers(1, &vertexBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float),
               vertices.data(), GL_STATIC_DRAW);

  glGenBuffers(1, &indexBuffer);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
               indices.data(), GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5,
                        reinterpret_cast<void *>(sizeof(float) * 3));
  auto vertexArrayEntity = registry->create();
  registry->emplace<VertexArray>(vertexArrayEntity, vertexArray);
}

Texture LoadTexture(const std::string& fileName) {
	Texture t;
    int channels = 0;
    unsigned char* image = stbi_load(fileName.c_str(), &t.texWidth, &t.texHeight, &channels, 0);
    if (!image) {
        SDL_Log("STB failed to load image %s: %s", fileName.c_str(), stbi_failure_reason());
    }

    int format = channels == 4? GL_RGBA:GL_RGB;
    
    glGenTextures(1, &t.textureID);
    glBindTexture(GL_TEXTURE_2D, t.textureID);
	
    glTexImage2D(GL_TEXTURE_2D, 0, format, t.texWidth, t.texWidth, 0, format, GL_UNSIGNED_BYTE, image);
    stbi_image_free(image);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    return t;
}

void SetTextureActive(unsigned int textureID) {
	glBindTexture(GL_TEXTURE_2D, textureID);
}

void SetShaderActive(unsigned int shaderProgram) {
	glUseProgram(shaderProgram);
}

void SetVerticesActive(unsigned int vertexID)
{
	glBindVertexArray(vertexID);
}

unsigned int GetVertexArray()
{
	return 0;
}


void SetMatrixUniform(Shader shader, const char* uniformName, const Matrix4* matrix) {
    GLuint loc = glGetUniformLocation(shader.shaderProgram, uniformName);
    glUniformMatrix4fv(
        loc, 1, GL_TRUE, matrix->GetAsFloatPtr()
    );
}

bool IsCompiled(GLuint shader)
{
	GLint status;
	// Query the compile status
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

	if (status != GL_TRUE)
	{
		char buffer[512];
		memset(buffer, 0, 512);
		glGetShaderInfoLog(shader, 511, nullptr, buffer);
		SDL_Log("GLSL Compile Failed:\n%s", buffer);
		return false;
	}

	return true;
}

bool CompileShader(const std::string& fileName,
	GLenum shaderType,
	GLuint& outShader)
{
	// Open file
	std::ifstream shaderFile(fileName);
	if (shaderFile.is_open())
	{
		// Read all the text into a string
		std::stringstream sstream;
		sstream << shaderFile.rdbuf();
		std::string contents = sstream.str();
		const char* contentsChar = contents.c_str();

		// Create a shader of the specified type
		outShader = glCreateShader(shaderType);
		// Set the source characters and try to compile
		glShaderSource(outShader, 1, &(contentsChar), nullptr);
		glCompileShader(outShader);

		if (!IsCompiled(outShader))
		{
			SDL_Log("Failed to compile shader %s", fileName.c_str());
			return false;
		}
	}
	else
	{
		SDL_Log("Shader file not found: %s", fileName.c_str());
		return false;
	}

	return true;
}


bool IsValidProgram(GLuint mShaderProgram)
{
	GLint status;
	// Query the link status
	glGetProgramiv(mShaderProgram, GL_LINK_STATUS, &status);
	if (status != GL_TRUE)
	{
		char buffer[512];
		memset(buffer, 0, 512);
		glGetProgramInfoLog(mShaderProgram, 511, nullptr, buffer);
		SDL_Log("GLSL Link Status:\n%s", buffer);
		return false;
	}

	return true;
}

Shader LoadShader(const std::string& vertexShader, const std::string& fragmentShader) {
	GLuint mVertexShader;
	GLuint mFragShader;
	if (!CompileShader(vertexShader,
		GL_VERTEX_SHADER,
		mVertexShader) ||
		!CompileShader(fragmentShader,
			GL_FRAGMENT_SHADER,
			mFragShader))
	{
		SDL_Log("Shader program failed to compile");
		return Shader{ 0 };
	}

	// Now create a shader program that
	// links together the vertex/frag shaders
	GLuint mShaderProgram = glCreateProgram();
	glAttachShader(mShaderProgram, mVertexShader);
	glAttachShader(mShaderProgram, mFragShader);
	glLinkProgram(mShaderProgram);

	// Verify that the program linked successfully
	if (!IsValidProgram(mShaderProgram))
	{
		SDL_Log("Shader program failed to link");
	}

	return Shader{mShaderProgram};
}
