#pragma once

#include <iostream>
#include <time.h>
#include <chrono>
#include <thread>
#include <memory>

#include <SFML/Graphics.hpp>

#include "../Object/Shapes.h"

class Scene;

class Renderer {
public:
	using tVector2 = Math::Vector2;
	using tVector3 = Math::Vector3;
	using tMatrix4x4 = Math::Matrix4x4;
	using tTriangle = Math::Triangle;
	using tMesh = Math::Mesh;
	using tTexture = std::shared_ptr<sf::Texture>;

	Renderer();
	~Renderer();

	void Initialize(std::shared_ptr<Scene> in_scene);

	void UpdateScene(std::shared_ptr<Scene> in_scene);
	void RenderShape(Shape& in_shape, std::shared_ptr<Scene> in_scene);

	tMesh LoadFromObj(const std::string& in_fileLocation, bool hasTexture = false);

private:
	float* ptrDepthBuffer = nullptr;
	tMatrix4x4 m_mProjectionMat;
	tMatrix4x4 m_mWorld;
	tMatrix4x4 m_mView;
	tMatrix4x4 m_mTranslation;

	tTexture m_texture = std::make_shared<sf::Texture>();
};

// singleton renderer
//class Renderer {
//public:
//	using tVector3 = Math::Vector3;
//	using tMatrix4x4 = Math::Matrix4x4;
//	using tTriangle = Math::Triangle;
//	using tMesh = Math::Mesh;
//
//	Renderer(const Renderer&) = delete; // delete copy constructor
//	static Renderer* Get() { // function to get instance of renderer
//		static Renderer* s_instance; // make a static instance of Renderer during get
//		return s_instance;
//	}
//	
//	// static versions of actual functions so we dont have to use "Get()->" when calling renderer's functions
//	static void Initialize() { Get()->InitializeImpl(); }
//	static void UpdateScene(std::shared_ptr<Scene> in_scene) { Get()->UpdateSceneImpl(in_scene); }
//	static void RenderShape(Shape& in_shape, std::shared_ptr<Scene> in_scene) { Get()->RenderShapeImpl(in_shape, in_scene); }
//	static tMesh LoadFromObj(const std::string& in_fileLocation) { return Get()->LoadFromObjImpl(in_fileLocation); }
//
//	static void SetProjectionMatrix(const tMatrix4x4& in_projMat) { Get()->m_mProjectionMat = in_projMat; }
//
//private:
//	Renderer(); // private constructor so another instance cant be made
//	~Renderer();
//
//	// Private implemented versions of static functions that are able to access member vairables
//	void InitializeImpl();
//	void UpdateSceneImpl(std::shared_ptr<Scene> in_scene);
//	void RenderShapeImpl(Shape& in_shape, std::shared_ptr<Scene> in_scene);
//	tMesh LoadFromObjImpl(const std::string& in_fileLocation);
//
//	tMatrix4x4 m_mProjectionMat;
//	tMatrix4x4 m_mWorld;
//	tMatrix4x4 m_mView;
//	tMatrix4x4 m_mTranslation;
//
//	sf::Texture m_texture;
//};