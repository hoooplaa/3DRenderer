#pragma once

#include <iostream>
#include <fstream>
#include <strstream>
#include <vector>
#include <list>

#include <SFML/Graphics.hpp>

#include "../Math/Math.h"

class Scene;

struct Shape {
public:
	using tVector2 = Math::Vector2;
	using tVector3 = Math::Vector3;
	using tMesh = Math::Mesh;
	using tTexture = std::shared_ptr<sf::Texture>;

	Shape();
	~Shape();

	virtual void Update(const double in_dt);
	virtual void Draw(std::shared_ptr<Scene> in_scene);

	void SetPosition(const tVector3& in_position);
	void SetRotation(const tVector3& in_rotation);

public:
	tMesh mesh;
	tTexture m_texture;

	tVector3 m_position;
	tVector3 m_ticMovement; //tracks movement and rotation since last update so we only have to update the tris once per frame
};

struct Cube : public Shape {
public:
	Cube();
	Cube(const tVector3& in_position, float in_sideLength);
	Cube(const tVector3& in_position, float in_sideLength, tTexture in_cubedTexture);
	~Cube();
};

struct Cuboid : public Shape {
public:
	Cuboid();
	Cuboid(const tVector3& in_position, float in_sideWidth, float in_sideHeight, float in_sideDepth);
	~Cuboid();
};