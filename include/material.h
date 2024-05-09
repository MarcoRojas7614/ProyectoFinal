#ifndef MATERIAL_H
#define MATERIAL_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

class Material 
{
public:
	// Material Attributes
	glm::vec4 ambient;
	glm::vec4 diffuse;
	glm::vec4 specular;
	float     transparency;

	Material() {
		ambient = glm::vec4(0.329412f, 0.223529f, 0.027451f, 1.0f);
		diffuse = glm::vec4(0.780392f, 0.568627f, 0.113725f, 1.0f);
		specular = glm::vec4(0.992157f, 0.941176f, 0.807843f, 1.0f);
		transparency = 1.0f;
	}
	~Material() {}

private:

};

#endif
