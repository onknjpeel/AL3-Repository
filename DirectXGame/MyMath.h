#pragma once
#include <cmath>
#include <Matrix4x4.h>
#include <Vector3.h>

struct AABB {
	Vector3 min;
	Vector3 max;
};

Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);

Matrix4x4 MakeRotateXMatrix(const Vector3& rotate);

Matrix4x4 MakeRotateYMatrix(const Vector3& rotate);

Matrix4x4 MakeRotateZMatrix(const Vector3& rotate);

Matrix4x4 MakeRotateZMatrix(const float& rotate);

Matrix4x4 MakeRotateMatrix(const Vector3& rotate);

Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);

Vector3 Transform(Vector3 vector, Matrix4x4 matrix);

Vector3& operator+=(Vector3& v1,const Vector3& v2);

const Vector3 operator+(const Vector3& v1, const Vector3& v2);

Vector3& operator-=(Vector3& v1,const Vector3& v2);

const Vector3 operator-(const Vector3& v1, const Vector3& v2);

AABB operator+(const AABB& aabb);

float EaseInSine(float num);

float EaseOutSine(float num);

bool AABBCollision(const AABB& aabb1, const AABB& aabb2);