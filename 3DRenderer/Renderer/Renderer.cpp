#include "Renderer.h"
#include "Scene.h"

Renderer* Renderer::s_instance = new Renderer();

Renderer::Renderer() {

};

void Renderer::InitializeImpl(std::shared_ptr<Scene> in_scene) {

	//m_texture->loadFromFile("C://Users//Alex Haurin//source//repos//3DRenderer//3DRenderer//Assets//Images//RedMushroomBlock.png");

	// Create projection matrix when Renderer is intialized using current scene
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

void Renderer::UpdateSceneImpl(std::shared_ptr<Scene> in_scene) {
	// Update current scene data (camera pos/dir) to create matrices for main render shape function
	tVector3& cameraPos = in_scene->m_cameraPos;
	tVector3& cameraDir = in_scene->m_cameraDir;
	
	// Create simple translation matrix
	m_mTranslation = Math::MakeTranslationMatrix(0.0f, 0.0f, 5.0f);

	// Create World translation matrix (not necessarily used, just for simplicity)
	m_mWorld = Math::MakeIdentityMatrix();
	m_mWorld = Math::MultiplyMatrixMatrix(m_mWorld, m_mTranslation);
	
	// Create "Point At" Matrix for camera (Matrix used to convert world coords to current camera's 'view')
	tVector3 up = { 0, 1, 0 };
	tVector3 target = { 0, 0, 1 };
	target = VectorAdd(cameraPos, cameraDir);
	
	tMatrix4x4 mCamera = MakePointAtMatrix(cameraPos, target, up);
	m_mView = MatrixQuickInverse(mCamera);
}

void Renderer::RenderShapeImpl(Shape& in_shape, std::shared_ptr<Scene> in_scene) {
	tVector3& light = in_scene->m_light;
	tVector3& cameraPos = in_scene->m_cameraPos;
	auto& window = in_scene->m_window;

	// Store triagles for rastering later
	std::vector<tTriangle> trisToRaster;

	// Draw Triangles
	for (auto tri : in_shape.mesh.tris) {
		tTriangle triTransformed, triViewed, triProjected;

		// World Matrix Transform
		//triTransformed = Math::MultiplyMatrixTriangle(m_mWorld, tri);

		tVector3 cameraRay = VectorSub(tri.points[0], cameraPos); // Get Ray from Triangle to camera
		tVector3 normal = tri.GetNormal();
		tri.color = 100 * std::acos(Math::DotProduct(light, normal) / (Math::Magnitude(light) * Math::Magnitude(normal)));


		// If cameraRay is alligned with triangle normal, then triangle is visible
		if (DotProduct(normal, cameraRay) < 0.0f) {

			// Convert World Space --> View Space (account for camera's pos)
			triViewed = Math::MultiplyMatrixTriangle(m_mView, tri);

			// Clip Viewed Triangle against near plane to prevent numerical errors/INSANELY big triangles
			int numclippedTris = 0;
			tTriangle clippedTris[2];
			numclippedTris = TriangleClipAgainstPlane({ 0.0f, 0.0f, 0.1f }, { 0.0f, 0.0f, 1.0f }, triViewed, clippedTris[0], clippedTris[1]);

			// Clipping could create more triangles, so accunt for those triangles
			for (int n = 0; n < numclippedTris; n++) {
				// Project triangles from 3D --> 2D
				triProjected = Math::MultiplyMatrixTriangle(m_mProjectionMat, clippedTris[n]);

				// Scale tris into view
				triProjected.points[0] = VectorDiv(triProjected.points[0], triProjected.points[0].w);
				triProjected.points[1] = VectorDiv(triProjected.points[1], triProjected.points[1].w);
				triProjected.points[2] = VectorDiv(triProjected.points[2], triProjected.points[2].w);

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
				tVector3 vOffsetView = { 1, 1, 0 };
				triProjected.points[0] = VectorAdd(triProjected.points[0], vOffsetView);
				triProjected.points[1] = VectorAdd(triProjected.points[1], vOffsetView);
				triProjected.points[2] = VectorAdd(triProjected.points[2], vOffsetView);
				triProjected.points[0].x *= 0.5f * (float)window->getSize().x;
				triProjected.points[0].y *= 0.5f * (float)window->getSize().y;
				triProjected.points[1].x *= 0.5f * (float)window->getSize().x;
				triProjected.points[1].y *= 0.5f * (float)window->getSize().y;
				triProjected.points[2].x *= 0.5f * (float)window->getSize().x;
				triProjected.points[2].y *= 0.5f * (float)window->getSize().y;

				// Store Triangle for sorting
				trisToRaster.push_back(triProjected);
			}
		}
	}

	// Sort triangles from back to front (makes sure that we draw the triangles in the correct order)
	// A better solution to this would be z-buffering, but without going realllly deep into SFML's code or
	// creating my own pixel shaders dont think it is really practical to do, so i'll leave figuring
	// that out for later
	sort(trisToRaster.begin(), trisToRaster.end(), [](tTriangle& t1, tTriangle& t2) {
		float z1 = (t1.points[0].z + t1.points[1].z + t1.points[2].z) / 3.0f;
		float z2 = (t2.points[0].z + t2.points[1].z + t2.points[2].z) / 3.0f;
		return z1 > z2;
	});

	// Loop through all transformed, viewed, projected, and sorted triangles
	for (auto& tri : trisToRaster) {
		// Clip triangles against all four screen edges, this could create
		// a bunch of triangles, so create a queue that we traverse to 
		// ensure we only test new triangles generated against planes
		tTriangle clippedTris[2];
		std::list<tTriangle> listTriangles;

		// Add initial triangle
		listTriangles.push_back(tri);
		int numNewTriangles = 1;

		for (int p = 0; p < 4; p++) {
			int numTrisToAdd = 0;
			while (numNewTriangles > 0) {
				// Take Triangle from front of queue
				tTriangle test = listTriangles.front();
				listTriangles.pop_front();
				numNewTriangles--;

				switch (p) {

				case 0:	numTrisToAdd = TriangleClipAgainstPlane({ 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, test, clippedTris[0], clippedTris[1]); break;
				case 1:	numTrisToAdd = TriangleClipAgainstPlane({ 0.0f, (float)window->getSize().y - 1, 0.0f }, { 0.0f, -1.0f, 0.0f }, test, clippedTris[0], clippedTris[1]); break;
				case 2:	numTrisToAdd = TriangleClipAgainstPlane({ 0.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, test, clippedTris[0], clippedTris[1]); break;
				case 3:	numTrisToAdd = TriangleClipAgainstPlane({ (float)window->getSize().x - 1, 0.0f, 0.0f }, { -1.0f, 0.0f, 0.0f }, test, clippedTris[0], clippedTris[1]); break;
				}

				for (int w = 0; w < numTrisToAdd; w++) {
					listTriangles.push_back(clippedTris[w]);
				}
			}
			numNewTriangles = listTriangles.size();
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
			triangle[0].color = sf::Color(t.color, t.color, t.color);
			triangle[1].color = sf::Color(t.color, t.color, t.color);
			triangle[2].color = sf::Color(t.color, t.color, t.color);
			
			//if (in_shape.m_texture) { window->draw(triangle, &*m_texture); }
			//else { window->draw(triangle); }

			window->draw(triangle);
		}
	}
};

////////////////////////////////// Loading Shapes ////////////////////////////////////

Math::Mesh Renderer::LoadFromObjImpl(const std::string& in_fileLocation, bool hasTexture) {
	std::ifstream file(in_fileLocation);
	assert(file.is_open() && "Could not open file");

	tMesh out_mesh;

	// Local cache of verts
	std::vector<tVector3> verts;
	std::vector<tVector2> texs;

	while (!file.eof())
	{
		char line[128];
		file.getline(line, 128);

		std::strstream stream;
		stream << line;

		char junk;

		if (line[0] == 'v')
		{
			if (line[1] == 't')
			{
				tVector2 v;
				stream >> junk >> junk >> v.x >> v.y;
				// A little hack for the spyro texture
				//v.u = 1.0f - v.u;
				//v.v = 1.0f - v.v;
				texs.push_back(v);
			}
			else
			{
				tVector3 v;
				stream >> junk >> v.x >> v.y >> v.z;
				verts.push_back(v);
			}
		}

		if (!hasTexture)
		{
			if (line[0] == 'f')
			{
				int f[3];
				stream >> junk >> f[0] >> f[1] >> f[2];
				out_mesh.tris.push_back({ verts[f[0] - 1], verts[f[1] - 1], verts[f[2] - 1] });
			}
		}
		else
		{
			if (line[0] == 'f')
			{
				stream >> junk;

				std::string tokens[32];
				int nTokenCount = -1;


				while (!stream.eof())
				{
					char c = stream.get();
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

				out_mesh.tris.push_back(buffer);
			}
		}
	}

	return out_mesh;
}