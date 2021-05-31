#include "Math.h"

namespace Math {
	///////////////////////////// Matrix4x4 Type //////////////////////
	Matrix4x4::Matrix4x4() {
		float mat[4][4] = { float(0) };
	};

	Matrix4x4::~Matrix4x4() {

	};

	//////////////////// Vector Type //////////////////////////////
	Vector2::Vector2() : x(0), y(0) {
		x, y = 0.0f;
	};

	Vector2::Vector2(float in_x, float in_y) : x(in_x), y(in_y) {

	};

	Vector2::Vector2(const sf::Vector2f& in_v) {
		this->x = in_v.x;
		this->y = in_v.y;
	}

	Vector2::~Vector2() {

	};

	Vector3::Vector3() : x(0), y(0), z(0) {
		x, y, z = 0.0f;
	};

	Vector3::Vector3(float in_x, float in_y, float in_z) : x(in_x), y(in_y), z(in_z) {

	};

	Vector3::~Vector3() {

	};

	////////////////////////// Triangle Type //////////////////////////
	Triangle::Triangle(const Vector3& p1, const Vector3& p2, const Vector3& p3) {
		points[0] = p1;
		points[1] = p2;
		points[2] = p3;
	}

	Triangle::Triangle() {

	};

	Triangle::~Triangle() {

	};

	Triangle* Triangle::Project(const Matrix4x4& in_projMat) {
		points[0] = MultiplyMatrixVector(in_projMat, points[0]);
		points[1] = MultiplyMatrixVector(in_projMat, points[1]);
		points[2] = MultiplyMatrixVector(in_projMat, points[2]);

		return this;
	}

	Vector3 Triangle::GetNormal() {
		Vector3 out_normal, line1, line2;

		line1 = Vector_Sub(points[1], points[0]);
		line2 = Vector_Sub(points[2], points[0]);

		out_normal = CrossProduct(line1, line2);
		out_normal = Normalize(out_normal);

		return out_normal;
	}

	///////////////////////////// General Math ////////////////////////////////
	float Radians(float in_degrees) {
		return in_degrees * (MATH_PI / 180);
	}
	float Degrees(float in_radians) {
		return in_radians * (180 / MATH_PI);
	}

	///////////////////////// Vector Math /////////////////////////////
	Vector3 Vector_Add(const Vector3& v1, const Vector3& v2) {
		return { v1.x + v2.x, v1.y + v2.y, v1.z + v2.z };
	}

	Vector3 Vector_Sub(const Vector3& v1, const Vector3& v2) {
		return { v1.x - v2.x, v1.y - v2.y, v1.z - v2.z };
	}

	Vector3 Vector_Mul(const Vector3& v1, float m) {
		return { v1.x * m, v1.y * m, v1.z * m };
	}

	Vector3 Vector_Div(const Vector3& v1, float d) {
		return { v1.x / d, v1.y / d, v1.z / d };
	}

	float DotProduct(const Vector3& v1, const Vector3& v2) {
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	}

	float Magnitude(const Vector3& v) {
		return sqrtf(DotProduct(v, v));
	}

	Vector3 Normalize(const Vector3& v) {
		float mag = Magnitude(v);
		return { v.x / mag, v.y / mag, v.z / mag };
	}

	Vector3 CrossProduct(const Vector3& v1, const Vector3& v2) {
		Vector3 out_vec;

		out_vec.x = v1.y * v2.z - v1.z * v2.y;
		out_vec.y = v1.z * v2.x - v1.x * v2.z;
		out_vec.z = v1.x * v2.y - v1.y * v2.x;

		return out_vec;
	}

	//////////////////////////// Matrix Math //////////////////////////////

	Vector3 MultiplyMatrixVector(const Matrix4x4& m, const Vector3 v) {
		Vector3 out_vec;
		out_vec.x = v.x * m.mat[0][0] + v.y * m.mat[1][0] + v.z * m.mat[2][0] + v.w * m.mat[3][0];
		out_vec.y = v.x * m.mat[0][1] + v.y * m.mat[1][1] + v.z * m.mat[2][1] + v.w * m.mat[3][1];
		out_vec.z = v.x * m.mat[0][2] + v.y * m.mat[1][2] + v.z * m.mat[2][2] + v.w * m.mat[3][2];
		out_vec.w = v.x * m.mat[0][3] + v.y * m.mat[1][3] + v.z * m.mat[2][3] + v.w * m.mat[3][3];

		return out_vec;
	}

	Matrix4x4 MultiplyMatrixMatrix(const Matrix4x4& m1, const Matrix4x4& m2) {
		Matrix4x4 out_mat;
		for (int c = 0; c < 4; c++)
			for (int r = 0; r < 4; r++)
				out_mat.mat[r][c] = m1.mat[r][0] * m2.mat[0][c] + m1.mat[r][1] * m2.mat[1][c] + m1.mat[r][2] * m2.mat[2][c] + m1.mat[r][3] * m2.mat[3][c];
		return out_mat;
	}

	Triangle MultiplyMatrixTriangle(const Matrix4x4& in_mat, const Triangle& in_tri) {
		Triangle out_tri;

		out_tri.points[0] = MultiplyMatrixVector(in_mat, in_tri.points[0]);
		out_tri.points[1] = MultiplyMatrixVector(in_mat, in_tri.points[1]);
		out_tri.points[2] = MultiplyMatrixVector(in_mat, in_tri.points[2]);
		out_tri.coords[0] = in_tri.coords[0];
		out_tri.coords[1] = in_tri.coords[1];
		out_tri.coords[2] = in_tri.coords[2];
		out_tri.color = in_tri.color;

		return out_tri;
	}

	Matrix4x4 Matrix_PointAt(const Vector3& pos, const Vector3& target, const Vector3& up) {
		// Calculate new forward direction
		Vector3 newForward, newUp;
		newForward.x = target.x - pos.x;
		newForward.y = target.y - pos.y;
		newForward.z = target.z - pos.z;

		newForward = Normalize(newForward);

		// Calculate new Up direction
		Vector3 a = Vector_Mul(newForward, DotProduct(up, newForward));
		newUp.x = up.x - a.x;
		newUp.y = up.y - a.y;
		newUp.z = up.z - a.z;

		newUp = Normalize(newUp);

		// New Right direction is easy, its just cross product
		Vector3 newRight = CrossProduct(newUp, newForward);

		// Construct Dimensioning and Translation Matrix	
		Matrix4x4 out_mat;
		out_mat.mat[0][0] = newRight.x;		out_mat.mat[0][1] = newRight.y;		out_mat.mat[0][2] = newRight.z;		out_mat.mat[0][3] = 0.0f;
		out_mat.mat[1][0] = newUp.x;		out_mat.mat[1][1] = newUp.y;		out_mat.mat[1][2] = newUp.z;		out_mat.mat[1][3] = 0.0f;
		out_mat.mat[2][0] = newForward.x;	out_mat.mat[2][1] = newForward.y;	out_mat.mat[2][2] = newForward.z;	out_mat.mat[2][3] = 0.0f;
		out_mat.mat[3][0] = pos.x;			out_mat.mat[3][1] = pos.y;			out_mat.mat[3][2] = pos.z;			out_mat.mat[3][3] = 1.0f;

		return out_mat;
	}

	Matrix4x4 Matrix_QuickInverse(const Matrix4x4& m) // Only for Rotation/Translation Matrices
	{
		Matrix4x4 out_mat;
		out_mat.mat[0][0] = m.mat[0][0]; out_mat.mat[0][1] = m.mat[1][0]; out_mat.mat[0][2] = m.mat[2][0]; out_mat.mat[0][3] = 0.0f;
		out_mat.mat[1][0] = m.mat[0][1]; out_mat.mat[1][1] = m.mat[1][1]; out_mat.mat[1][2] = m.mat[2][1]; out_mat.mat[1][3] = 0.0f;
		out_mat.mat[2][0] = m.mat[0][2]; out_mat.mat[2][1] = m.mat[1][2]; out_mat.mat[2][2] = m.mat[2][2]; out_mat.mat[2][3] = 0.0f;
		out_mat.mat[3][0] = -(m.mat[3][0] * out_mat.mat[0][0] + m.mat[3][1] * out_mat.mat[1][0] + m.mat[3][2] * out_mat.mat[2][0]);
		out_mat.mat[3][1] = -(m.mat[3][0] * out_mat.mat[0][1] + m.mat[3][1] * out_mat.mat[1][1] + m.mat[3][2] * out_mat.mat[2][1]);
		out_mat.mat[3][2] = -(m.mat[3][0] * out_mat.mat[0][2] + m.mat[3][1] * out_mat.mat[1][2] + m.mat[3][2] * out_mat.mat[2][2]);
		out_mat.mat[3][3] = 1.0f;

		return out_mat;
	}

	Matrix4x4 Matrix_MakeIdentity()
	{
		Matrix4x4 out_mat;
		out_mat.mat[0][0] = 1.0f;
		out_mat.mat[1][1] = 1.0f;
		out_mat.mat[2][2] = 1.0f;
		out_mat.mat[3][3] = 1.0f;

		return out_mat;
	}

	Matrix4x4 Matrix_MakeTranslation(float x, float y, float z)
	{
		Matrix4x4 out_mat;
		out_mat.mat[0][0] = 1.0f;
		out_mat.mat[1][1] = 1.0f;
		out_mat.mat[2][2] = 1.0f;
		out_mat.mat[3][3] = 1.0f;
		out_mat.mat[3][0] = x;
		out_mat.mat[3][1] = y;
		out_mat.mat[3][2] = z;

		return out_mat;
	}

	Matrix4x4 MakeProjectionMatrix(float zNear, float zFar, float FOVDegrees, float AspectRatio) {
		Matrix4x4 out_mat;

		float FOVRadians = 1.0f / tanf(FOVDegrees * 0.5f / 180.0f * 3.14159f);
		out_mat.mat[0][0] = AspectRatio * FOVRadians;
		out_mat.mat[1][1] = FOVRadians;
		out_mat.mat[2][2] = zFar / (zFar - zNear);
		out_mat.mat[3][2] = (-zFar * zNear) / (zFar - zNear);
		out_mat.mat[2][3] = 1.0f;
		out_mat.mat[3][3] = 0.0f;

		return out_mat;
	}

	Matrix4x4 MakeRotationMatrixX(float in_thetaRad) {
		Matrix4x4 out_mat;
		out_mat.mat[0][0] = 1.0f;
		out_mat.mat[1][1] = cosf(in_thetaRad);
		out_mat.mat[1][2] = sinf(in_thetaRad);
		out_mat.mat[2][1] = -sinf(in_thetaRad);
		out_mat.mat[2][2] = cosf(in_thetaRad);
		out_mat.mat[3][3] = 1.0f;
		return out_mat;
	}

	Matrix4x4 MakeRotationMatrixY(float in_thetaRad) {
		Matrix4x4 out_mat;
		out_mat.mat[0][0] = cosf(in_thetaRad);
		out_mat.mat[0][2] = sinf(in_thetaRad);
		out_mat.mat[2][0] = -sinf(in_thetaRad);
		out_mat.mat[1][1] = 1.0f;
		out_mat.mat[2][2] = cosf(in_thetaRad);
		out_mat.mat[3][3] = 1.0f;
		return out_mat;
	}

	Matrix4x4 MakeRotationMatrixZ(float in_thetaRad) {
		Matrix4x4 out_mat;
		out_mat.mat[0][0] = cos(in_thetaRad);
		out_mat.mat[0][1] = sinf(in_thetaRad);
		out_mat.mat[1][0] = -sinf(in_thetaRad);
		out_mat.mat[1][1] = cosf(in_thetaRad);
		out_mat.mat[2][2] = 1.0f;
		out_mat.mat[3][3] = 1.0f;
		return out_mat;
	}

	////////////////////////////// Other ////////////////////////////////////

	int Triangle_ClipAgainstPlane(Vector3 plane_p, Vector3 plane_n, Triangle& in_tri, Triangle& out_tri1, Triangle& out_tri2) {
		//Keep shading
		out_tri1.color = in_tri.color;
		out_tri2.color = in_tri.color;
		// Make sure plane normal is indeed normal
		plane_n = Normalize(plane_n);

		// Return signed shortest distance from point to plane, plane normal must be normalised
		auto dist = [&](Vector3& p)
		{
			Vector3 n = Normalize(p);
			return (plane_n.x * p.x + plane_n.y * p.y + plane_n.z * p.z - DotProduct(plane_n, plane_p));
		};

		// Create two temporary storage arrays to classify points either side of plane
		// If distance sign is positive, point lies on "inside" of plane
		Vector3* inside_points[3];  int nInsidePointCount = 0;
		Vector3* outside_points[3]; int nOutsidePointCount = 0;
		Vector2* inside_tex[3]; int nInsideTexCount = 0;
		Vector2* outside_tex[3]; int nOutsideTexCount = 0;
		
		// Get signed distance of each point in Triangle to plane
		float d0 = dist(in_tri.points[0]);
		float d1 = dist(in_tri.points[1]);
		float d2 = dist(in_tri.points[2]);

		if (d0 >= 0) { inside_points[nInsidePointCount++] = &in_tri.points[0]; inside_tex[nInsideTexCount++] = &in_tri.coords[0]; }
		else {
			outside_points[nOutsidePointCount++] = &in_tri.points[0]; outside_tex[nOutsideTexCount++] = &in_tri.coords[0];
		}
		if (d1 >= 0) {
			inside_points[nInsidePointCount++] = &in_tri.points[1]; inside_tex[nInsideTexCount++] = &in_tri.coords[1];
		}
		else {
			outside_points[nOutsidePointCount++] = &in_tri.points[1];  outside_tex[nOutsideTexCount++] = &in_tri.coords[1];
		}
		if (d2 >= 0) {
			inside_points[nInsidePointCount++] = &in_tri.points[2]; inside_tex[nInsideTexCount++] = &in_tri.coords[2];
		}
		else {
			outside_points[nOutsidePointCount++] = &in_tri.points[2];  outside_tex[nOutsideTexCount++] = &in_tri.coords[2];
		}

		// Now classify Triangle points, and break the input Triangle into 
		// smaller output Triangles if required. There are four possible
		// outcomes...

		if (nInsidePointCount == 0)
		{
			// All points lie on the outside of plane, so clip whole Triangle
			// It ceases to exist

			return 0; // No returned Triangles are valid
		}

		if (nInsidePointCount == 3)
		{
			// All points lie on the inside of plane, so do nothing
			// and allow the Triangle to simply pass through
			out_tri1 = in_tri;

			return 1; // Just the one returned original Triangle is valid
		}

		if (nInsidePointCount == 1 && nOutsidePointCount == 2)
		{
			// Triangle should be clipped. As two points lie outside
			// the plane, the triangle simply becomes a smaller triangle

			// Copy appearance info to new triangle
			out_tri1.color = in_tri.color;

			// The inside point is valid, so keep that...
			out_tri1.points[0] = *inside_points[0];
			out_tri1.coords[0] = *inside_tex[0];

			// but the two new points are at the locations where the 
			// original sides of the triangle (lines) intersect with the plane
			float t;
			out_tri1.points[1] = Vector_IntersectPlane(plane_p, plane_n, *inside_points[0], *outside_points[0], t);
			out_tri1.coords[1].x = t * (outside_tex[0]->x - inside_tex[0]->x) + inside_tex[0]->x;
			out_tri1.coords[1].y = t * (outside_tex[0]->y - inside_tex[0]->y) + inside_tex[0]->y;
			out_tri1.coords[1].w = t * (outside_tex[0]->w - inside_tex[0]->w) + inside_tex[0]->w;

			out_tri1.points[2] = Vector_IntersectPlane(plane_p, plane_n, *inside_points[0], *outside_points[1], t);
			out_tri1.coords[2].x = t * (outside_tex[1]->x - inside_tex[0]->x) + inside_tex[0]->x;
			out_tri1.coords[2].y = t * (outside_tex[1]->y - inside_tex[0]->y) + inside_tex[0]->y;
			out_tri1.coords[2].w = t * (outside_tex[1]->w - inside_tex[0]->w) + inside_tex[0]->w;

			return 1; // Return the newly formed single triangle
		}

		if (nInsidePointCount == 2 && nOutsidePointCount == 1)
		{
			// Triangle should be clipped. As two points lie inside the plane,
			// the clipped triangle becomes a "quad". Fortunately, we can
			// represent a quad with two new triangles

			// Copy appearance info to new triangles
			out_tri1.color = in_tri.color;

			out_tri2.color = in_tri.color;

			// The first triangle consists of the two inside points and a new
			// point determined by the location where one side of the triangle
			// intersects with the plane
			out_tri1.points[0] = *inside_points[0];
			out_tri1.points[1] = *inside_points[1];
			out_tri1.coords[0] = *inside_tex[0];
			out_tri1.coords[1] = *inside_tex[1];

			float t;
			out_tri1.points[2] = Vector_IntersectPlane(plane_p, plane_n, *inside_points[0], *outside_points[0], t);
			out_tri1.coords[2].x = t * (outside_tex[0]->x - inside_tex[0]->x) + inside_tex[0]->x;
			out_tri1.coords[2].y = t * (outside_tex[0]->y - inside_tex[0]->y) + inside_tex[0]->y;
			out_tri1.coords[2].w = t * (outside_tex[0]->w - inside_tex[0]->w) + inside_tex[0]->w;

			// The second triangle is composed of one of he inside points, a
			// new point determined by the intersection of the other side of the 
			// triangle and the plane, and the newly created point above
			out_tri2.points[0] = *inside_points[1];
			out_tri2.coords[0] = *inside_tex[1];
			out_tri2.points[1] = out_tri1.points[2];
			out_tri2.coords[1] = out_tri1.coords[2];
			out_tri2.points[2] = Vector_IntersectPlane(plane_p, plane_n, *inside_points[1], *outside_points[0], t);
			out_tri2.coords[2].x = t * (outside_tex[0]->x - inside_tex[1]->x) + inside_tex[1]->x;
			out_tri2.coords[2].y = t * (outside_tex[0]->y - inside_tex[1]->y) + inside_tex[1]->y;
			out_tri2.coords[2].w = t * (outside_tex[0]->w - inside_tex[1]->w) + inside_tex[1]->w;
			return 2; // Return two newly formed triangles which form a quad
		}
	}

	Vector3 Vector_IntersectPlane(Vector3& plane_p, Vector3& plane_n, Vector3& lineStart, Vector3& lineEnd, float& t) {
		plane_n = Normalize(plane_n);
		float plane_d = -DotProduct(plane_n, plane_p);
		float ad = DotProduct(lineStart, plane_n);
		float bd = DotProduct(lineEnd, plane_n);
		t = (-plane_d - ad) / (bd - ad);
		Vector3 lineStartToEnd = Vector_Sub(lineEnd, lineStart);
		Vector3 lineToIntersect = Vector_Mul(lineStartToEnd, t);
		return Vector_Add(lineStart, lineToIntersect);
	}
}