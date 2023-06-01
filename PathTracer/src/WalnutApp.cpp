#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"

#include "Walnut/Image.h"
#include "Walnut/Random.h"
#include "Walnut/Timer.h"
#include "../Renderer.h"
#include "../Scene.h"

using namespace Walnut;

class ExampleLayer : public Walnut::Layer
{
public:
	virtual void OnUIRender() override
	{
		ImGui::Begin("Settings");
		ImGui::Text("Last render: %.3fms", m_LastRenderTime);
		if (ImGui::Button("Button")) {
			Render();
		}
		ImGui::End();


		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("Viewport");
		
		m_ViewportWidth = ImGui::GetContentRegionAvail().x;
		m_ViewportHeight = ImGui::GetContentRegionAvail().y;

		if (m_Image)
		{
			ImGui::Image(m_Image->GetDescriptorSet(), { (float)m_Image->GetWidth(), (float)m_Image->GetHeight() });
		}

		ImGui::End();
		ImGui::PopStyleVar();

		//ImGui::ShowDemoWindow();
	}

	void Render() {

		Timer timer;
		m_Renderer.m_Camera = cam;
		m_Renderer.m_RenderWidth = m_ViewportWidth;
		m_Renderer.m_RenderHeight = m_ViewportHeight;
		scene.addSphere(sphere);
		m_Renderer.m_Scene = scene;

		if (!m_Image || m_ViewportWidth != m_Image->GetWidth() || m_ViewportHeight != m_Image->GetHeight()) 
		{
			m_Image = std::make_shared<Image>(m_ViewportWidth, m_ViewportHeight, ImageFormat::RGBA);
			delete[] m_ImageData;
			m_ImageData = new uint32_t[m_ViewportWidth * m_ViewportHeight];
		}

		int i = 0;
		for (int w = 0; w < m_ViewportWidth; w++)
		{
			for (int h = 0; h < m_ViewportHeight; h++)
			{
				m_ImageData[i] = m_Renderer.Calculate(w, h, m_Samples);
				i++;
			}
		}

		/*for (uint32_t i = 0; i < m_ViewportWidth * m_ViewportHeight; i++) 
		{
			m_ImageData[i] = Random::UInt();
			m_ImageData[i] |= 0xff000000;
		}*/
		
		m_Image->SetData(m_ImageData);
		m_LastRenderTime = timer.ElapsedMillis();
	}

private:
	std::shared_ptr<Image> m_Image;
	uint32_t* m_ImageData = nullptr;
	uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;
	float m_LastRenderTime = 0.0f;
	Camera cam = Camera{ glm::vec3{0.0f, -2.0f, 12.5f}, 160 };
	Scene scene = Scene{};
	Sphere sphere = Sphere{ glm::vec3{-5.339, -1.948, -16.014}, glm::vec4{0.5f, 0.5f, 0.5f, 1.0f}, 6, 0.0f, false, 0.0f };
	Renderer m_Renderer = Renderer();
	int m_Samples = 0;
};

Walnut::Application* Walnut::CreateApplication(int argc, char** argv)
{
	Walnut::ApplicationSpecification spec;
	spec.Name = "Walnut Example";

	Walnut::Application* app = new Walnut::Application(spec);
	app->PushLayer<ExampleLayer>();
	app->SetMenubarCallback([app]()
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Exit"))
			{
				app->Close();
			}
			ImGui::EndMenu();
		}
	});
	return app;
}