#include "Shapes.h"
#include "../Renderer/Renderer.h"

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

void Shape::Update(const double in_dt) {

};

void Shape::Draw(std::shared_ptr<Scene> in_scene) {
	
}

void Shape::SetPosition(const tVector3& in_position) {

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

////////////////////////////////////////////////// CUBE ///////////////////////////////////////////////////////

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