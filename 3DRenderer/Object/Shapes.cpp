#include "Shapes.h"

#include "../Renderer/Scene.h"

//////////////////////////////////// SHAPE ///////////////////////////////////////////////////

Shape::Shape() {

};

Shape::~Shape() {

};

void Shape::Intialize() {
	Object::Initialize();

	tVector3 pos = m_position;
	tVector3 dim = m_dimensions;

	mesh.tris = {
		//SOUTH																						 
		{ pos,								        { pos.x, pos.y + dim.y, pos.z },                  { pos.x + dim.x, pos.y + dim.y, pos.z } },
		{ { pos.x, pos.y, pos.z },                  { pos.x + dim.x, pos.y + dim.y, pos.z },          { pos.x + dim.x, pos.y, pos.z } },

		//EAST								    	 									  			 
		{ { pos.x + dim.x, pos.y, pos.z },          { pos.x + dim.x, pos.y + dim.y, pos.z },          { pos.x + dim.x, pos.y + dim.y, pos.z + dim.z } },
		{ { pos.x + dim.x, pos.y, pos.z },          { pos.x + dim.x, pos.y + dim.y, pos.z + dim.z },  { pos.x + dim.x, pos.y, pos.z + dim.z } },

		//North										 									  			 
		{ { pos.x + dim.x, pos.y, pos.z + dim.z },  { pos.x + dim.x, pos.y + dim.y, pos.z + dim.z },  { pos.x, pos.y + dim.y, pos.z + dim.z } },
		{ { pos.x + dim.x, pos.y, pos.z + dim.z },  { pos.x, pos.y + dim.y, pos.z + dim.z },          { pos.x, pos.y, pos.z + dim.z } },

		//WEST										 									  			 
		{ { pos.x, pos.y, pos.z + dim.z },		    { pos.x, pos.y + dim.y, pos.z + dim.z },          { pos.x, pos.y + dim.y, pos.z } },
		{ { pos.x, pos.y, pos.z + dim.z },		    { pos.x, pos.y + dim.y, pos.z },                  { pos.x, pos.y, pos.z } },

		//TOP										 									  			 
		{ { pos.x, pos.y + dim.y, pos.z },          { pos.x, pos.y + dim.y, pos.z + dim.z },          { pos.x + dim.x, pos.y + dim.y, pos.z + dim.z } },
		{ { pos.x, pos.y + dim.y, pos.z },          { pos.x + dim.x, pos.y + dim.y, pos.z + dim.z },  { pos.x + dim.x, pos.y + dim.y, pos.z } },

		//BOTTOM									 									  			 
		{ { pos.x + dim.x, pos.y, pos.z + dim.z },  { pos.x, pos.y, pos.z + dim.z },                  { pos.x, pos.y, pos.z } },
		{ { pos.x + dim.x, pos.y, pos.z + dim.z },  { pos.x, pos.y, pos.z },                          { pos.x + dim.x, pos.y, pos.z } },
	};
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

void Shape::SetDimensions(const tVector3& in_dim) {
	tVector3 dif(std::max(-m_dimensions.x, in_dim.x - m_dimensions.x),
				 std::max(-m_dimensions.y, in_dim.y - m_dimensions.y),
				 std::max(-m_dimensions.z, in_dim.z - m_dimensions.z));

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
}

////////////////////////////////////////////////// CUBOID ///////////////////////////////////////////////////////

Cuboid::Cuboid() {

};

Cuboid::Cuboid(const tVector3& in_position, const tVector3& in_dimensions) {
	m_position = in_position;
	m_dimensions = in_dimensions;
}

Cuboid::Cuboid(const tVector3& in_position, const tVector3& in_dimensions, std::shared_ptr<tTexture> in_pTexture) {
	m_position = in_position;
	m_dimensions = in_dimensions;
	m_pTexture = in_pTexture;
}
																			    
Cuboid::~Cuboid() {

};

void Cuboid::Initialize() {
	Shape::Initialize();

}

void Cuboid::Destroy() {

	Shape::Destroy();
}