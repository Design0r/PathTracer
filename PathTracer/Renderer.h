#pragma once
#include <cstdint>
#include "Scene.h"
#include "Camera.h"
#include <glm/glm.hpp>
#include <algorithm>

class Renderer
{
public:
	uint32_t Calculate(uint32_t x, uint32_t y, int Samples);
	uint32_t m_RenderWidth;
	uint32_t m_RenderHeight;
	Scene m_Scene;
	Camera m_Camera;
private:
	glm::vec4 ComputeSample(uint32_t x, uint32_t y);
	glm::vec4 Trace(Ray ray, int currentDepth);

	static double RandomUniform();
	static glm::vec3 Renderer::ReflectVector(glm::vec3 direction, glm::vec3 normal, float roughness);
	static uint32_t ConvertToRGBA(const glm::vec4& color);

private:
	
	uint8_t m_Msaa = 2;
	uint8_t m_MaxDepth = 3;
	glm::vec4 m_SkyColor{1.0f, 1.0f , 1.0f , 1.0f};
};