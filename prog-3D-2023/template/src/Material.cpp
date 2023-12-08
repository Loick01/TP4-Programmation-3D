// Local includes
#include "Material.h"
#include "Shader.h"
// GLM includes
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
// OPENGL includes
#include <GL/glew.h>
#include <GL/glut.h>

#include "Context.h"

#include "Texture.h"

#include <iostream>
#include "stb_image.h"

Material::~Material() {
	if (m_program != 0) {
		glDeleteProgram(m_program);
	}
}

unsigned int Material::loadCubemap(std::vector<std::string> liste_textures){
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < liste_textures.size(); i++)
	{
		unsigned char *data = stbi_load(liste_textures[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
						 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
			);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap tex failed to load at path: " << liste_textures[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;	
}


void Material::init() {
	// TODO : Change shader by your
	
	/*
	// Pour l'exercice 2 (normal map) (à commenter/décommenter)
	m_program = load_shaders("shaders/vertex.glsl", "shaders/fragment.glsl");
	
	check();
	// TODO : set initial parameters
	m_color = {1.0, 1.0, 1.0, 1.0};
	
	m_texture = loadTexture2DFromFilePath("data/TwoSidedPlane_BaseColor.png");
	setDefaultTexture2DParameters(m_texture);
	m_texture_normal = loadTexture2DFromFilePath("data/TwoSidedPlane_Normal.png");
	// (à commenter/décommenter jusqu'ici)
	*/
	
	// ---------------------------------------------------------------------------------------
	
	
	// Pour l'exercice 3 (skybox) (à commenter/décommenter)
	Context::skyboxProgram = load_shaders("shaders/vertexSkybox.glsl", "shaders/fragmentSkybox.glsl");
	m_program = Context::skyboxProgram;
	
	check();
	
	std::vector<std::string> liste_textures {
    	"./data/skybox/right.jpg",
    	"./data/skybox/left.jpg",
    	"./data/skybox/top.jpg",
    	"./data/skybox/bottom.jpg",
    	"./data/skybox/front.jpg",
    	"./data/skybox/back.jpg"
	};
	
	Context::skyboxTexture = loadCubemap(liste_textures);
	
	float skyboxVertices[] = {
		// positions
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f
	};
	

	glGenVertexArrays(1, &Context::skyboxVAO);
	glGenBuffers(1, &Context::skyboxVBO);
	glBindVertexArray(Context::skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, Context::skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glBindVertexArray(0);
	
	// (à commenter/décommenter jusqu'ici)
	
}

void Material::clear() {
	// nothing to clear
	// TODO: Add the texture or buffer you want to destroy for your material
}

void Material::bind() {
	check();
	glUseProgram(m_program);
	internalBind();
}

void Material::internalBind() {
	// bind parameters
	GLint color = getUniform("color");
	glUniform4fv(color, 1, glm::value_ptr(m_color));
	if (m_texture != -1) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_texture);
		glUniform1i(getUniform("colorTexture"), 0);
	}
	if (m_texture_normal != -1) {
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, m_texture_normal);
		glUniform1i(getUniform("colorTextureNormal"), 1);
	}
	
	glUniform3f(getUniform("positionLight"),0.,0.4,0.);
	glUniform3f(getUniform("couleurLight"),1.,1.,1.);

	// TODO : Add your custom parameters here
}

void Material::setMatrices(glm::mat4& projectionMatrix, glm::mat4& viewMatrix, glm::mat4& modelMatrix)
{
	check();
	glUniformMatrix4fv(getUniform("projection"), 1, false, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(getUniform("view"), 1, false, glm::value_ptr(viewMatrix));
	glUniformMatrix4fv(getUniform("model"), 1, false, glm::value_ptr(modelMatrix));
	
	glm::vec3 CamPos = Context::camera.getPosition();
	glUniform3f(getUniform("positionCamera"),CamPos[0],CamPos[1],CamPos[2]);
}

GLint Material::getAttribute(const std::string& in_attributeName) {
	check();
	return glGetAttribLocation(m_program, in_attributeName.c_str());
}

GLint Material::getUniform(const std::string& in_uniformName) {
	check();
	return glGetUniformLocation(m_program, in_uniformName.c_str());
}
