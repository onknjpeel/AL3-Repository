#include "MyMath.h"
#define _USE_MATH_DEFINES
#include "math.h"
#include <assert.h>

Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 result = {};
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			for (int k = 0; k < 4; k++) {
				result.m[i][j] += m1.m[i][k] * m2.m[k][j];
			}
		}
	}
	return result;
}

Matrix4x4 MakeRotateXMatrix(const Vector3& rotate) {
	Matrix4x4 rotateX;
	rotateX = {
		1,0,0,0,
		0,std::cos(rotate.x),std::sin(rotate.x),0,
		0,-std::sin(rotate.x),std::cos(rotate.x),0,
		0,0,0,1
	};
	return rotateX;
}

Matrix4x4 MakeRotateYMatrix(const Vector3& rotate) {
	Matrix4x4 rotateY;
	rotateY = {
		std::cos(rotate.y),0,-std::sin(rotate.y),0,
		0,1,0,0,
		std::sin(rotate.y),0,std::cos(rotate.y),
		0,0,0,0,1
	};
	return rotateY;
}

Matrix4x4 MakeRotateZMatrix(const Vector3& rotate) {
	Matrix4x4 rotateZ;
	rotateZ = {
		std::cos(rotate.z),std::sin(rotate.z),0,0,
		-std::sin(rotate.z),std::cos(rotate.z),0,0,
		0,0,1,0,
		0,0,0,1
	};
	return rotateZ;
}

Matrix4x4 MakeRotateZMatrix(const float& rotate) {
	Matrix4x4 rotateZ;
	rotateZ = {
		std::cos(rotate),std::sin(rotate),0,0,
		-std::sin(rotate),std::cos(rotate),0,0,
		0,0,1,0,
		0,0,0,1
	};
	return rotateZ;
}

Matrix4x4 MakeRotateMatrix(const Vector3& rotate) {
	Matrix4x4 rotateX;
	rotateX = MakeRotateXMatrix(rotate);

	Matrix4x4 rotateY;
	rotateY = MakeRotateYMatrix(rotate);

	Matrix4x4 rotateZ;
	rotateZ = MakeRotateZMatrix(rotate);

	Matrix4x4 result = Multiply(rotateX, Multiply(rotateY, rotateZ));
	return result;
}

Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate) {
	Matrix4x4 result;
	Matrix4x4 rotateM = MakeRotateMatrix(rotate);
	result = {
		scale.x * rotateM.m[0][0],scale.x * rotateM.m[0][1],scale.x * rotateM.m[0][2],0,
		scale.y * rotateM.m[1][0],scale.y * rotateM.m[1][1],scale.y * rotateM.m[1][2],0,
		scale.z * rotateM.m[2][0],scale.z * rotateM.m[2][1],scale.z * rotateM.m[2][2],0,
		translate.x,translate.y,translate.z,1
	};
	return result;
}

Vector3 Transform(Vector3 vector, Matrix4x4 matrix) {
	Vector3 result;
	result = {
		vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0] + matrix.m[3][0],
		vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1] + matrix.m[3][1],
		vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2] + matrix.m[3][2]
	};
	float w = vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] + vector.z * matrix.m[2][3] + matrix.m[3][3];

	assert(w != 0.0f);
	result.x /= w;
	result.y /= w;
	result.z /= w;

	return result;
}

Vector3& operator+=(Vector3& v1, const Vector3& v2) {
	v1.x += v2.x;
	v1.y += v2.y;
	v1.z += v2.z;
	Vector3 &result = v1;
	return result;
}

const Vector3 operator+(const Vector3& v1, const Vector3& v2) {
	Vector3 temp(v1);
	temp = {
		temp.x + v2.x,
		temp.y + v2.y,
		temp.z + v2.z
	};
	return temp;
}

Vector3& operator-=(Vector3& v1, const Vector3& v2) {
	v1.x -= v2.x;
	v1.y -= v2.y;
	v1.z -= v2.z;
	Vector3 &result = v1;
	return result;
}

const Vector3 operator-(const Vector3& v1, const Vector3& v2) {
	Vector3 temp(v1);
	temp = {
		temp.x - v2.x,
		temp.y - v2.y,
		temp.z - v2.z
	};
	return temp;
}

AABB operator+(const AABB& aabb) {
	return aabb;
}

float EaseInSine(float num) {
	return 1.0f - cosf((num * float(M_PI)) / 2.0f);
}

float EaseOutSine(float num) {
	return sinf((num * float(M_PI)) / 2.0f);
}

bool AABBCollision(const AABB& aabb1, const AABB& aabb2) {
	if ((aabb1.min.x <= aabb2.max.x && aabb1.max.x >= aabb2.min.x) &&
		(aabb1.min.y <= aabb2.max.y && aabb1.max.y >= aabb2.min.y) &&
		(aabb1.min.z <= aabb2.max.z && aabb1.max.z >= aabb2.min.z)) {
		return true;
	}
	else {
		return false;
	}
}