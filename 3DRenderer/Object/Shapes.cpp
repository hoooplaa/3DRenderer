#include "Shapes.h"

#include "../Renderer/Scene.h"

//////////////////////////////////// SHAPE ///////////////////////////////////////////////////

Shape::Shape() {
	mesh.tris = {
		//SOUTH
		{ tVector3(0.0f, 0.0f, 0.0f),    tVector3(0.0f, 1.0f, 0.0f),    tVector3(1.0f, 1.0f, 0.0f) },
		{ tVector3(0.0f, 0.0f, 0.0f),    tVector3(1.0f, 1.0f, 0.0f),    tVector3(1.0f, 0.0f, 0.0f) },

		//EAST
		{ tVector3(1.0f, 0.0f, 0.0f),    tVector3(1.0f, 1.0f, 0.0f),    tVector3(1.0f, 1.0f, 1.0f) },
		{ tVector3(1.0f, 0.0f, 0.0f),    tVector3(1.0f, 1.0f, 1.0f),    tVector3(1.0f, 0.0f, 1.0f) },

		//North
		{ tVector3(1.0f, 0.0f, 1.0f),    tVector3(1.0f, 1.0f, 1.0f),    tVector3(0.0f, 1.0f, 1.0f) },
		{ tVector3(1.0f, 0.0f, 1.0f),    tVector3(0.0f, 1.0f, 1.0f),    tVector3(0.0f, 0.0f, 1.0f) },

		//WEST
		{ tVector3(0.0f, 0.0f, 1.0f),    tVector3(0.0f, 1.0f, 1.0f),    tVector3(0.0f, 1.0f, 0.0f) },
		{ tVector3(0.0f, 0.0f, 1.0f),    tVector3(0.0f, 1.0f, 0.0f),    tVector3(0.0f, 0.0f, 0.0f) },

		//TOP
		{ tVector3(0.0f, 1.0f, 0.0f),    tVector3(0.0f, 1.0f, 1.0f),    tVector3(1.0f, 1.0f, 1.0f) },
		{ tVector3(0.0f, 1.0f, 0.0f),    tVector3(1.0f, 1.0f, 1.0f),    tVector3(1.0f, 1.0f, 0.0f) },

		//BOTTOM
		{ tVector3(1.0f, 0.0f, 1.0f),    tVector3(0.0f, 0.0f, 1.0f),    tVector3(0.0f, 0.0f, 0.0f) },
		{ tVector3(1.0f, 0.0f, 1.0f),    tVector3(0.0f, 0.0f, 0.0f),    tVector3(1.0f, 0.0f, 0.0f) },
	};
};

Shape::~Shape() {

};

void Shape::Intialize() {
	Object::Initialize();

}

void Shape::Destroy() {

	Object::Destroy();
}

void Shape::Update(const double in_dt) {

};

void Shape::Draw(std::shared_ptr<Scene> in_scene) {
	in_scene->Render(*this);
}

void Shape::Move(const tVector3& in_mov) {
	tVector3 newPos(m_position.x + in_mov.x, m_position.y + in_mov.y, m_position.z + in_mov.z);

	SetPosition(newPos);
}

void Shape::SetPosition(const tVector3& in_position) {
	tVector3 dif(in_position.x - m_position.x, in_position.y - m_position.y, in_position.z - m_position.z);

	for (auto& t : mesh.tris) {
		t.points[0].x += dif.x;
		t.points[0].y += dif.y;
		t.points[0].z += dif.z;
		
		t.points[1].x += dif.x;
		t.points[1].y += dif.y;
		t.points[1].z += dif.z;
		
		t.points[2].x += dif.x;
		t.points[2].y += dif.y;
		t.points[2].z += dif.z;
	}
};

void Shape::SetRotation(const tVector3& in_rotation) {

};

////////////////////////////////////////////////// CUBE ///////////////////////////////////////////////////////

Cube::Cube() {

};

Cube::Cube(const tVector3& in_position, float in_sideLength) {
	auto pos = in_position;
	float s = in_sideLength;

	// Set vertices for triangles
	mesh.tris = {
		//SOUTH
		{ pos,								     tVector3(pos.x, pos.y + s, pos.z),         tVector3(pos.x + s, pos.y + s, pos.z) },
		{ tVector3(pos.x, pos.y, pos.z),         tVector3(pos.x + s, pos.y + s, pos.z),     tVector3(pos.x + s, pos.y, pos.z) },

		//EAST								    										  
		{ tVector3(pos.x + s, pos.y, pos.z),     tVector3(pos.x + s, pos.y + s, pos.z),     tVector3(pos.x + s, pos.y + s, pos.z + s) },
		{ tVector3(pos.x + s, pos.y, pos.z),     tVector3(pos.x + s, pos.y + s, pos.z + s), tVector3(pos.x + s, pos.y, pos.z + s) },

		//North																			  
		{ tVector3(pos.x + s, pos.y, pos.z + s), tVector3(pos.x + s, pos.y + s, pos.z + s), tVector3(pos.x, pos.y + s, pos.z + s) },
		{ tVector3(pos.x + s, pos.y, pos.z + s), tVector3(pos.x, pos.y + s, pos.z + s),     tVector3(pos.x, pos.y, pos.z + s) },

		//WEST																			  
		{ tVector3(pos.x, pos.y, pos.z + s),     tVector3(pos.x, pos.y + s, pos.z + s),     tVector3(pos.x, pos.y + s, pos.z) },
		{ tVector3(pos.x, pos.y, pos.z + s),     tVector3(pos.x, pos.y + s, pos.z),         tVector3(pos.x, pos.y, pos.z) },

		//TOP																			  
		{ tVector3(pos.x, pos.y + s, pos.z),     tVector3(pos.x, pos.y + s, pos.z + s),     tVector3(pos.x + s, pos.y + s, pos.z + s) },
		{ tVector3(pos.x, pos.y + s, pos.z),     tVector3(pos.x + s, pos.y + s, pos.z + s), tVector3(pos.x + s, pos.y + s, pos.z) },

		//BOTTOM																		  
		{ tVector3(pos.x + s, pos.y, pos.z + s), tVector3(pos.x, pos.y, pos.z + s),         tVector3(pos.x, pos.y, pos.z) },
		{ tVector3(pos.x + s, pos.y, pos.z + s), tVector3(pos.x, pos.y, pos.z),             tVector3(pos.x + s, pos.y, pos.z) },
	};

	// Set vertices for texture (assuming a "cubed" texture was passed in)
	for (auto& t : mesh.tris) {
		t.coords[0] = { t.points[0].x - s, t.points[0].y - s };
		t.coords[1] = { t.points[1].x - s, t.points[1].y - s };
		t.coords[2] = { t.points[2].x - s, t.points[2].y - s };
	};
}

Cube::Cube(const tVector3& in_position, float in_sideLength, tTexture in_cubedTexture) {
	auto pos = in_position;
	float s = in_sideLength;

	// Set vertices for triangles
	mesh.tris = {
		//SOUTH
		{ pos,								     tVector3(pos.x, pos.y + s, pos.z),         tVector3(pos.x + s, pos.y + s, pos.z) },
		{ tVector3(pos.x, pos.y, pos.z),         tVector3(pos.x + s, pos.y + s, pos.z),     tVector3(pos.x + s, pos.y, pos.z) },

		//EAST								    										  
		{ tVector3(pos.x + s, pos.y, pos.z),     tVector3(pos.x + s, pos.y + s, pos.z),     tVector3(pos.x + s, pos.y + s, pos.z + s) },
		{ tVector3(pos.x + s, pos.y, pos.z),     tVector3(pos.x + s, pos.y + s, pos.z + s), tVector3(pos.x + s, pos.y, pos.z + s) },

		//North																			  
		{ tVector3(pos.x + s, pos.y, pos.z + s), tVector3(pos.x + s, pos.y + s, pos.z + s), tVector3(pos.x, pos.y + s, pos.z + s) },
		{ tVector3(pos.x + s, pos.y, pos.z + s), tVector3(pos.x, pos.y + s, pos.z + s),     tVector3(pos.x, pos.y, pos.z + s) },

		//WEST																			  
		{ tVector3(pos.x, pos.y, pos.z + s),     tVector3(pos.x, pos.y + s, pos.z + s),     tVector3(pos.x, pos.y + s, pos.z) },
		{ tVector3(pos.x, pos.y, pos.z + s),     tVector3(pos.x, pos.y + s, pos.z),         tVector3(pos.x, pos.y, pos.z) },

		//TOP																			  
		{ tVector3(pos.x, pos.y + s, pos.z),     tVector3(pos.x, pos.y + s, pos.z + s),     tVector3(pos.x + s, pos.y + s, pos.z + s) },
		{ tVector3(pos.x, pos.y + s, pos.z),     tVector3(pos.x + s, pos.y + s, pos.z + s), tVector3(pos.x + s, pos.y + s, pos.z) },

		//BOTTOM																		  
		{ tVector3(pos.x + s, pos.y, pos.z + s), tVector3(pos.x, pos.y, pos.z + s),         tVector3(pos.x, pos.y, pos.z) },
		{ tVector3(pos.x + s, pos.y, pos.z + s), tVector3(pos.x, pos.y, pos.z),             tVector3(pos.x + s, pos.y, pos.z) },
	};

	// Set vertices for texture (assuming a "cubed" texture was passed in)
	for (auto& t : mesh.tris) {
		t.coords[0] = { t.points[0].x - s, t.points[0].y - s };
		t.coords[1] = { t.points[1].x - s, t.points[1].y - s };
		t.coords[2] = { t.points[2].x - s, t.points[2].y - s };
	}
}

Cube::~Cube() {

};

void Cube::Initialize() {
	Shape::Initialize();

}

void Cube::Destroy() {

	Shape::Destroy();
}

////////////////////////////////////////////////// CUBOID ///////////////////////////////////////////////////////

Cuboid::Cuboid() {

};

Cuboid::Cuboid(const tVector3& in_position, float in_sideWidth, float in_sideHeight, float in_sideDepth) {
	auto pos = in_position;
	float w = in_sideWidth;
	float h = in_sideHeight;
	float d = in_sideDepth;

	// Set vertices for triangles
	mesh.tris = {
		//SOUTH
		{ pos,								     tVector3(pos.x, pos.y + h, pos.z),         tVector3(pos.x + w, pos.y + h, pos.z) },
		{ tVector3(pos.x, pos.y, pos.z),         tVector3(pos.x + w, pos.y + h, pos.z),     tVector3(pos.x + w, pos.y, pos.z) },

		//EAST								    										  
		{ tVector3(pos.x + w, pos.y, pos.z),     tVector3(pos.x + w, pos.y + h, pos.z),     tVector3(pos.x + w, pos.y + h, pos.z + d) },
		{ tVector3(pos.x + w, pos.y, pos.z),     tVector3(pos.x + w, pos.y + h, pos.z + d), tVector3(pos.x + w, pos.y, pos.z + d) },

		//North																			  
		{ tVector3(pos.x + w, pos.y, pos.z + d), tVector3(pos.x + w, pos.y + h, pos.z + d), tVector3(pos.x, pos.y + h, pos.z + d) },
		{ tVector3(pos.x + w, pos.y, pos.z + d), tVector3(pos.x, pos.y + h, pos.z + d),     tVector3(pos.x, pos.y, pos.z + d) },

		//WEST																			  
		{ tVector3(pos.x, pos.y, pos.z + d),     tVector3(pos.x, pos.y + h, pos.z + d),     tVector3(pos.x, pos.y + h, pos.z) },
		{ tVector3(pos.x, pos.y, pos.z + d),     tVector3(pos.x, pos.y + h, pos.z),         tVector3(pos.x, pos.y, pos.z) },

		//TOP																			  
		{ tVector3(pos.x, pos.y + h, pos.z),     tVector3(pos.x, pos.y + h, pos.z + d),     tVector3(pos.x + w, pos.y + h, pos.z + d) },
		{ tVector3(pos.x, pos.y + h, pos.z),     tVector3(pos.x + w, pos.y + h, pos.z + d), tVector3(pos.x + w, pos.y + h, pos.z) },

		//BOTTOM																		  
		{ tVector3(pos.x + w, pos.y, pos.z + d), tVector3(pos.x, pos.y, pos.z + d),         tVector3(pos.x, pos.y, pos.z) },
		{ tVector3(pos.x + w, pos.y, pos.z + d), tVector3(pos.x, pos.y, pos.z),             tVector3(pos.x + w, pos.y, pos.z) },
	};
}

Cuboid::~Cuboid() {

};

void Cuboid::Initialize() {
	Shape::Initialize();

}

void Cuboid::Destroy() {

	Shape::Destroy();
}