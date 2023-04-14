#include "Sphere.h"

Sphere::Sphere(glm::vec3 position, glm::vec4 color, float radius, float roughness, bool isEmitter, float intensity) 
{
	m_Position = position;
	m_Color = color;
	m_Radius = radius;
	m_Roughness = roughness;
	m_IsEmitter = isEmitter;
	m_Intensity = intensity;
}

float Sphere::Intersect(Ray &ray) 
{
	glm::vec3 v = ray.Origin - m_Position;
	float a = glm::dot(ray.Direction, ray.Direction);
	float b = 2 * glm::dot(ray.Direction, v);
	float c = glm::dot(v, v) - m_Radius * m_Radius;

	float discriminant = (b * b) - (4.0f * a * c);

	if (discriminant > 0)
	{
		float x1 = (-b - glm::sqrt(discriminant)) / (2.0f + a);
		float x2 = (-b + glm::sqrt(discriminant)) / (2.0f + a);

		if (x1 >= 0 && x2 >= 0)
		{
			return x1;
		}
		if (x1 < 0 && x1 >= 0)
		{
			return x2;
		}
	}

	return -1.0f;
}

glm::vec3 Sphere::Normal(glm::vec3& point)
{
	return glm::normalize(point - m_Position);
}
