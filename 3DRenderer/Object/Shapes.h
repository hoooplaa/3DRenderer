#pragma once

#include <iostream>
#include <fstream>
#include <strstream>
#include <vector>
#include <list>

#include <SFML/Graphics.hpp>

#include "Object.h"
#include "ObjectGuard.h"
#include "../Math/Math.h"

class Scene;

struct Shape : public Object {
public:
	using tVector2 = Math::Vector2;
	using tVector3 = Math::Vector3;
	using tTriangle = Math::Triangle;
	using tMesh = Math::Mesh;
	using tTexture = std::shared_ptr<sf::Texture>;

	Shape();
	~Shape();
	void Intialize();
	void Destroy();

	virtual void Update(const double in_dt);
	virtual void Draw(std::shared_ptr<Scene> in_scene);

	void Move(const tVector3& in_movement);

	tVector3 GetPosition() const { return m_position; }
	void SetPosition(const tVector3& in_position);
	tVector3 GetDimensions() const { return m_dimensions; }
	void SetDimensions(const tVector3& in_dim);

	tMesh mesh;

protected:
	std::shared_ptr<sf::Texture> m_pTexture = std::make_shared<sf::Texture>();

	tVector3 m_dimensions = {1, 1, 1 };
	tVector3 m_position = { 0, 0, 0 };
	tVector3 m_ticMovement; //tracks movement and rotation since last update so we only have to update the tris once per frame
};

struct Cuboid : public Shape {
public:
	Cuboid();
	Cuboid(const tVector3& in_position, const tVector3& in_dimensions);
	Cuboid(const tVector3& in_position, const tVector3& in_dimensions, std::shared_ptr<tTexture> in_pTexture);
	~Cuboid();

	void Initialize();
	void Destroy();
};