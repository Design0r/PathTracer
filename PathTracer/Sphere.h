#pragma once

#include <glm/glm.hpp>
#include "Ray.h"

class Sphere
{


public:
	Sphere(glm::vec3 position, glm::vec4 color, float radius, float roughness, bool isEmitter, float intensity);

	float Intersect(Ray &ray);
	glm::vec3 Normal(glm::vec3& point);

	glm::vec3 m_Position{ 0.0f,0.0f,0.0f };
	glm::vec4 m_Color{ 0.0f,0.0f,0.0f,  1.0f };
	float m_Radius = 0.0f;
	float m_Roughness = 0.0f;
	bool m_IsEmitter = false;
	float m_Intensity = 0.0f;

};