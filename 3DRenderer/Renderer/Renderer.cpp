#include "Renderer.h"
#include "Scene.h"

Renderer::Renderer() {

};

Renderer::~Renderer() {

};

void Renderer::Initialize(std::shared_ptr<Scene> in_scene) {

	m_texture->loadFromFile("C://Users//Alex Haurin//source//repos//3DRenderer//3DRenderer//Assets//Images//RedMushroomBlock.png");

	float zNear = 0.1f;
	float zFar = 1000.0f;
	float FOV = 90.0f;
	float AspectRatio = 800.0f / 1200.0f;
	float FOVRadians = 1.0f / tanf(FOV * 0.5f / 180.0f * MATH_PI);

	m_mProjectionMat.mat[0][0] = AspectRatio * FOVRadians;
	m_mProjectionMat.mat[1][1] = FOVRadians;
	m_mProjectionMat.mat[2][2] = zFar / (zFar - zNear);
	m_mProjectionMat.mat[3][2] = (-zFar * zNear) / (zFar - zNear);
	m_mProjectionMat.mat[2][3] = 1.0f;
	m_mProjectionMat.mat[3][3] = 0.0f;
};

void Renderer::UpdateScene(std::shared_ptr<Scene> in_scene) {
	//////// Bugged version using math functions ///////
	tVector3& light = in_scene->m_light;
	tVector3& cameraPos = in_scene->m_cameraPos;
	tVector3& cameraDir = in_scene->m_cameraDir;
	auto& window = in_scene->m_window;
	
	m_mTranslation = Math::Matrix_MakeTranslation(0.0f, 0.0f, 5.0f);
	m_mWorld = Math::Matrix_MakeIdentity();	// Form World Matrix
	m_mWorld = Math::MultiplyMatrixMatrix(m_mWorld, m_mTranslation); // Transform by translation
	
	// Create "Point At" Matrix for camera, or "Look At" matrix
	tVector3 up = { 0,1,0 };
	tVector3 target = { 0,0,1 };
	target = Vector_Add(cameraPos, cameraDir);
	
	// Make view matrix from camera
	tMatrix4x4 mCamera = Matrix_PointAt(cameraPos, target, up);
	m_mView = Matrix_QuickInverse(mCamera);
}

void Renderer::RenderShape(Shape& in_shape, std::shared_ptr<Scene> in_scene) {
	tVector3& light = in_scene->m_light;
	tVector3& cameraPos = in_scene->m_cameraPos;
	auto& window = in_scene->m_window;

	// Store triagles for rastering later
	std::vector<tTriangle> vecTrianglesToRaster;

	// Draw Triangles
	for (auto tri : in_shape.mesh.tris) {
		tTriangle triTransformed, triProjected, triViewed;

		// World Matrix Transform
		// triTransformed = Math::MultiplyMatrixTriangle(m_mWorld, tri);

		// Get Ray from Triangle to camera
		tVector3 cameraRay = Vector_Sub(tri.points[0], cameraPos);
		auto normal = tri.GetNormal();
		tri.color = std::max(100.0f, DotProduct(Normalize(light), tVector3(1, 1, 1)) * 100);


		// If ray is aligned with normal, then Triangle is visible
		if (DotProduct(normal, cameraRay) < 0.0f) {

			// Convert World Space --> View Space
			triViewed = Math::MultiplyMatrixTriangle(m_mView, tri);

			// Clip Viewed Triangle against near plane, this could form two additional
			// additional triangles. 
			int clippedTriangles = 0;
			tTriangle clipped[2];
			clippedTriangles = Triangle_ClipAgainstPlane({ 0.0f, 0.0f, 0.1f }, { 0.0f, 0.0f, 1.0f }, triViewed, clipped[0], clipped[1]);

			// We may end up with multiple triangles form the clip, so project as
			// required
			for (int n = 0; n < clippedTriangles; n++) {
				// Project triangles from 3D --> 2D
				triProjected = Math::MultiplyMatrixTriangle(m_mProjectionMat, clipped[n]);

				std::cout << m_mProjectionMat.mat[0][0] << std::endl;

				// Scale into view, we moved the normalising into cartesian space
				// out of the matrix.vector function from the previous videos, so
				// do this manually
				triProjected.points[0] = Vector_Div(triProjected.points[0], triProjected.points[0].w);
				triProjected.points[1] = Vector_Div(triProjected.points[1], triProjected.points[1].w);
				triProjected.points[2] = Vector_Div(triProjected.points[2], triProjected.points[2].w);

				//Divide tex coords by w to add perspective and prevent messing up when we get to close (dividing by z value to 'change' size of the pixel for how far away it is)
				triProjected.coords[0].x = triProjected.coords[0].x / triProjected.points[0].w;
				triProjected.coords[1].x = triProjected.coords[1].x / triProjected.points[1].w;
				triProjected.coords[2].x = triProjected.coords[2].x / triProjected.points[2].w;

				triProjected.coords[0].y = triProjected.coords[0].y / triProjected.points[0].w;
				triProjected.coords[2].y = triProjected.coords[1].y / triProjected.points[1].w;
				triProjected.coords[2].y = triProjected.coords[2].y / triProjected.points[2].w;

				triProjected.coords[0].w = 1.0f / triProjected.points[0].w;
				triProjected.coords[1].w = 1.0f / triProjected.points[1].w;
				triProjected.coords[2].w = 1.0f / triProjected.points[2].w;

				// X/Y are inverted so put them back
				triProjected.points[0].x *= -1.0f;
				triProjected.points[1].x *= -1.0f;
				triProjected.points[2].x *= -1.0f;
				triProjected.points[0].y *= -1.0f;
				triProjected.points[1].y *= -1.0f;
				triProjected.points[2].y *= -1.0f;

				// Offset verts into visible normalised space
				tVector3 vOffsetView = { 1,1,0 };
				triProjected.points[0] = Vector_Add(triProjected.points[0], vOffsetView);
				triProjected.points[1] = Vector_Add(triProjected.points[1], vOffsetView);
				triProjected.points[2] = Vector_Add(triProjected.points[2], vOffsetView);
				triProjected.points[0].x *= 0.5f * (float)window->getSize().x;
				triProjected.points[0].y *= 0.5f * (float)window->getSize().y;
				triProjected.points[1].x *= 0.5f * (float)window->getSize().x;
				triProjected.points[1].y *= 0.5f * (float)window->getSize().y;
				triProjected.points[2].x *= 0.5f * (float)window->getSize().x;
				triProjected.points[2].y *= 0.5f * (float)window->getSize().y;

				// Store Triangle for sorting
				vecTrianglesToRaster.push_back(triProjected);
			}
		}
	}

	// Sort triangles from back to front
	sort(vecTrianglesToRaster.begin(), vecTrianglesToRaster.end(), [](tTriangle& t1, tTriangle& t2) {
		float z1 = (t1.points[0].z + t1.points[1].z + t1.points[2].z) / 3.0f;
		float z2 = (t2.points[0].z + t2.points[1].z + t2.points[2].z) / 3.0f;
		return z1 > z2;
	});

	// Loop through all transformed, viewed, projected, and sorted triangles
	for (auto& triToRaster : vecTrianglesToRaster) {
		// Clip triangles against all four screen edges, this could yield
		// a bunch of triangles, so create a queue that we traverse to 
		//  ensure we only test new triangles generated against planes
		tTriangle clipped[2];
		std::list<tTriangle> listTriangles;

		// Add initial triangle
		listTriangles.push_back(triToRaster);
		int newTriangles = 1;

		for (int p = 0; p < 4; p++) {
			int trisToAdd = 0;
			while (newTriangles > 0) {
				// Take Triangle from front of queue
				tTriangle test = listTriangles.front();
				listTriangles.pop_front();
				newTriangles--;

				// Clip it against a plane. We only need to test each 
				// subsequent plane, against subsequent new triangles
				// as all triangles after a plane clip are guaranteed
				// to lie on the inside of the plane. I like how this
				// comment is almost completely and utterly justified
				switch (p)
				{
				case 0:	trisToAdd = Triangle_ClipAgainstPlane({ 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, test, clipped[0], clipped[1]); break;
				case 1:	trisToAdd = Triangle_ClipAgainstPlane({ 0.0f, (float)window->getSize().y - 1, 0.0f }, { 0.0f, -1.0f, 0.0f }, test, clipped[0], clipped[1]); break;
				case 2:	trisToAdd = Triangle_ClipAgainstPlane({ 0.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, test, clipped[0], clipped[1]); break;
				case 3:	trisToAdd = Triangle_ClipAgainstPlane({ (float)window->getSize().x - 1, 0.0f, 0.0f }, { -1.0f, 0.0f, 0.0f }, test, clipped[0], clipped[1]); break;
				}

				// Clipping may yield a variable number of triangles, so
				// add these new ones to the back of the queue for subsequent
				// clipping against next planes
				for (int w = 0; w < trisToAdd; w++) {
					listTriangles.push_back(clipped[w]);
				}
			}
			newTriangles = listTriangles.size();
		}

		// Draw the transformed, viewed, clipped, projected, sorted, clipped triangles
		for (auto& t : listTriangles) {
			//Convert into vertex array
			sf::VertexArray triangle(sf::Triangles, 3);
			triangle[0].position = sf::Vector2f(t.points[0].x, t.points[0].y);
			triangle[1].position = sf::Vector2f(t.points[1].x, t.points[1].y);
			triangle[2].position = sf::Vector2f(t.points[2].x, t.points[2].y);

			//Add texturing to vertex array
			triangle[0].texCoords = sf::Vector2f(t.coords[0].x / t.coords[0].w, t.coords[0].y / t.coords[0].w);
			triangle[1].texCoords = sf::Vector2f(t.coords[1].x / t.coords[1].w, t.coords[1].y / t.coords[1].w);
			triangle[2].texCoords = sf::Vector2f(t.coords[2].x / t.coords[2].w, t.coords[2].y / t.coords[2].w);

			//Set color
			//triangle[0].color = sf::Color(0, 100, 0);
			//triangle[1].color = sf::Color(0, 100, 0);
			//triangle[2].color = sf::Color(0, 100, 0);

			window->draw(triangle, &*m_texture);
		}
	}
};

////////////////////////////////// Loading Shapes ////////////////////////////////////

Math::Mesh Renderer::LoadFromObj(const std::string& in_fileLocation, bool hasTexture) {
	tMesh out_mesh;
	std::vector<tVector3> verts;
	std::vector<tVector2> texs;
	std::vector<tTriangle> tris;
	std::ifstream file(in_fileLocation);

	if (!file.is_open()) {
		std::cout << "File at: " << in_fileLocation << ", not found" << std::endl;
		return out_mesh;
	}

	//looping over each line
	while (!file.eof()) {
		char line[128];
		file.getline(line, 128);

		std::strstream string;
		string << line;

		char junk;

		//collecting verts
		if (line[0] == 'v') {
			if (line[1] == 't') {
				tVector2 vert;
				string >> junk >> vert.x >> vert.y;
				texs.push_back(vert);
			}
			else {
				tVector3 vert;
				string >> junk >> vert.x >> vert.y >> vert.z;
				verts.push_back(vert);
			}
		}

		//collecting triangles
		if (!hasTexture) {
			if (line[0] == 'f') {
				int f[3];
				string >> junk >> f[0] >> f[1] >> f[2];
				out_mesh.tris.push_back(tTriangle(verts[f[0] - 1], verts[f[1] - 1], verts[f[2] - 1]));
			}
		}
		else {
			if (line[0] == 'f')
			{
				string >> junk;

				std::string tokens[6];
				int nTokenCount = -1;


				while (!string.eof())
				{
					char c = string.get();
					if (c == ' ' || c == '/')
						nTokenCount++;
					else
						tokens[nTokenCount].append(1, c);
				}

				tokens[nTokenCount].pop_back();


				tTriangle buffer(verts[stoi(tokens[0]) - 1], verts[stoi(tokens[2]) - 1], verts[stoi(tokens[4]) - 1]);
				buffer.coords[0] = { texs[stoi(tokens[1]) - 1] };
				buffer.coords[1] = { texs[stoi(tokens[3]) - 1] };
				buffer.coords[2] = { texs[stoi(tokens[5]) - 1] };

				tris.push_back(buffer);
			};
		}
	}

	return out_mesh;
}