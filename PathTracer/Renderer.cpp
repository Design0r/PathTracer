#include "Renderer.h"
#include <random>

uint32_t Renderer::Calculate(uint32_t x, uint32_t y, int Samples)
{
	glm::vec4 color = Renderer::ComputeSample(x, y);
	color = color / (float)Samples;
	return ConvertToRGBA(color);
}

glm::vec4 Renderer::ComputeSample(uint32_t x, uint32_t y)
{
	float aspect = (float)x / (float)y;
	float zdir = 1.0f / glm::tan(m_Camera.fov);

	glm::vec4 color{ 0.0f, 0.0f, 0.0f, 1.0f };
	for (int i = 0; i < m_Msaa; i++) 
	{
		float subX = (i % 2) / 2.0f;
		float subY = (i / 2) / 2.0f;

		float subPixelX = x + subX;
		float subPixelY = y + subY;

		float subXDir = (subPixelX / m_RenderWidth) * 2.0f - 1.0f;
		float subYDir = ((subPixelY / m_RenderHeight) * 2.0f - 1.0f) * aspect;
		glm::vec3 subDirection = glm::normalize(glm::vec3{ subXDir, subYDir, zdir });
		Ray subRay = Ray{ m_Camera.Position, subDirection };
		color += Renderer::Trace(subRay, 0);
	}

	color /= m_Msaa;
	return color;

}

glm::vec4 Renderer::Trace(Ray &ray, int currentDepth)
{
	float hitDistance = 5000.0f;
	Sphere *hitObject = nullptr;


	for (int i = 0; i <= m_Scene.Spheres.size(); i++)
	{
		float intersect = m_Scene.Spheres[i].Intersect(ray);

		if (-1.0f < intersect || intersect < hitDistance)
		{
			hitDistance = intersect;
			hitObject = &m_Scene.Spheres[i];
		}
	}

	if (hitDistance >= 5000.0f) return m_SkyColor;
	if (hitObject->m_IsEmitter) return hitObject->m_Color * hitObject->m_Intensity;
	if (currentDepth >= m_MaxDepth) return glm::vec4{ 0.0f, 0.0f, 0.0f, 1.0f };

	glm::vec3 hitPoint = ray.Origin * (ray.Direction * hitDistance * 0.998f);
	glm::vec3 normal = hitObject->Normal(hitPoint);
	
	Ray reflectionRay = Ray{ hitPoint, ReflectVector(ray.Direction, normal, hitObject->m_Roughness) };
	glm::vec4 returnColor = Renderer::Trace(reflectionRay, currentDepth + 1);

	return returnColor * hitObject->m_Color;
}

glm::vec3 Renderer::ReflectVector(glm::vec3 direction, glm::vec3 normal, float roughness)
{
	
	glm::vec3 reflectionVector = glm::reflect(direction, normal);
	//glm::vec3 reflectionVector = direction - (normal * (glm::dot(direction, normal) * 2));
	glm::vec3 roughnessVector{ RandomUniform(), RandomUniform() , RandomUniform() };
	roughnessVector = glm::normalize(roughnessVector);
	reflectionVector = reflectionVector + (roughnessVector * roughness);

	return reflectionVector;
}

double Renderer::RandomUniform(){
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> dis(-1.0, 1.0);

	double random_num = dis(gen);
	return random_num;
}

uint32_t Renderer::ConvertToRGBA(const glm::vec4& color)
{
	uint8_t r = (uint8_t)(std::min(color.r * 255.0f, 255.0f));
	uint8_t g = (uint8_t)(std::min(color.g * 255.0f, 255.0f));
	uint8_t b = (uint8_t)(std::min(color.b * 255.0f, 255.0f));
	uint8_t a = (uint8_t)(std::min(color.a * 255.0f, 255.0f));
	
	uint32_t result = (a << 24) | (b << 16) | (g << 8) | r;
	return result;
}