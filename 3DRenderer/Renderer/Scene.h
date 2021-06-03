#pragma once

#include <iostream>

#include <SFML/Graphics.hpp>

#include "../Math/Math.h"
#include "../Object/Shapes.h"
#include "Renderer.h"

class Scene : public Object {
public:
	using tVector2 = sf::Vector2f;
	using tVector3 = Math::Vector3;
	using tMatrix4x4 = Math::Matrix4x4;

	Scene(const tVector2& in_dimensions, const std::string& in_name);
	~Scene();

	void Initialize();
	void Destroy();

	void Update(double in_dt);
	void Draw();

	void Clear(const sf::Color& in_color);
	void Display();
	void Render(Shape& in_shape);

	void RotateCameraYaw(float in_yaw);
	void RotateCameraPitch(float in_pitch);
	void MoveCamera(const tVector3& in_movement);

	std::shared_ptr<sf::RenderWindow> GetWindow() const { return m_window; }

public:
	std::shared_ptr<sf::RenderWindow> m_window;

	tVector3 m_cameraPos = { 0.0f, 0.0f, 0.0f };
	float m_cameraYaw = 0;
	float m_cameraPitch = 0;
	tVector3 m_cameraDir; // Direction vector along the direction camera points

	const tVector3 m_constLight = { 1.0f, -1.0f, 0.0f };
	tVector3 m_light = { 1.0f, -1.0f, 0.0f };
	tVector2 m_dimensions;
	float m_theta = 0.0f;

	std::string m_name;

	std::shared_ptr<Renderer> m_renderer = std::make_shared<Renderer>();
};