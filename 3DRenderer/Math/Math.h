#pragma once

#include <iostream>
#include <cmath>
#include <vector>

#include <SFML/System/Vector2.hpp>

const float MATH_PI = 3.1415926;

//////////////////////////////////// Types ////////////////////////////////
namespace Math {

	struct Matrix4x4 {
		Matrix4x4();
		~Matrix4x4();

		float mat[4][4] = { 0 };
	};

	struct Vector2 {
		Vector2();
		Vector2(float in_x, float in_y);
		Vector2(const sf::Vector2f& in_v);
		~Vector2();

		float x, y;
		float w = 1.0f;

		bool operator== (const Vector2& v) { // equal sign operators do not check for w
			return (this->x == v.x && this->y == v.y);
		}

		friend std::ostream& operator<< (std::ostream& stream, const Vector2& v) {
			stream << v.x << ", " << v.y;
			return stream;
		}
	};

	struct Vector3 {
		Vector3();
		Vector3(float in_x, float in_y, float in_z);
		~Vector3();

		float x, y, z;
		float w = 1.0f;

		bool operator== (const Vector3& v) {
			return (this->x == v.x && this->y == v.y && this->z == v.z);
		}

		friend std::ostream& operator<< (std::ostream& stream, const Vector3& v) {
			stream << v.x << ", " << v.y << ", " << v.z;
			return stream;
		}
	};

	struct Triangle {
		Triangle(const Vector3& p1, const Vector3& p2, const Vector3& p3);
		Triangle();
		~Triangle();

		Triangle* Project(const Matrix4x4& in_projMat);
		Vector3 GetNormal();

		Vector3 points[3];
		Vector2 coords[3];
		float color = 0;
	};

	struct Mesh {
		std::vector<Triangle> tris;
	};

	////////////////////////////////////////// General Math /////////////////////////////////////
	float Radians(float in_degrees);
	float Degrees(float in_radians);

	///////////////////////////////////// Vector Math //////////////////////////
	Vector3 Vector_Add(const Vector3& v1, const Vector3& v2);
	Vector3 Vector_Sub(const Vector3& v1, const Vector3& v2);
	Vector3 Vector_Mul(const Vector3& v1, float m);
	Vector3 Vector_Div(const Vector3& v1, float d);

	float DotProduct(const Vector3& v1, const Vector3& v2);
	float Magnitude(const Vector3& v);
	Vector3 Normalize(const Vector3& v);
	Vector3 CrossProduct(const Vector3& v1, const Vector3& v2);

	////////////////////////////// Matrix Math //////////////////////////////////////////
	Vector3 MultiplyMatrixVector(const Matrix4x4& m, const Vector3 v);
	Matrix4x4 MultiplyMatrixMatrix(const Matrix4x4& m1, const Matrix4x4& m2);

	Matrix4x4 Matrix_PointAt(const Vector3& pos, const Vector3& target, const Vector3& up);
	Matrix4x4 Matrix_QuickInverse(const Matrix4x4& m);

	Matrix4x4 Matrix_MakeIdentity();
	Matrix4x4 Matrix_MakeTranslation(float x, float y, float z);
	Matrix4x4 MakeProjectionMatrix(float zNear, float zFar, float FOVDegrees, float AspectRatio);

	Matrix4x4 MakeRotationMatrixX(float in_thetaRad);
	Matrix4x4 MakeRotationMatrixY(float in_thetaRad);
	Matrix4x4 MakeRotationMatrixZ(float in_thetaRad);

	//////////////////////////////////// Triangle Math /////////////////////////////////////////

	Triangle MultiplyMatrixTriangle(const Matrix4x4& in_mat, const Triangle& in_tri);

	int Triangle_ClipAgainstPlane(Vector3 plane_p, Vector3 plane_n, Triangle& in_tri, Triangle& out_tri1, Triangle& out_tri2);
	Vector3 Vector_IntersectPlane(Vector3& plane_p, Vector3& plane_n, Vector3& lineStart, Vector3& lineEnd, float& t);
}