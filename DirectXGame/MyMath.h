#pragma once
#include <cmath>
#include <Matrix4x4.h>
#include <Vector3.h>

Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);

Matrix4x4 MakeRotateMatrix(const Vector3& rotate);

Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);

Vector3& operator+=(Vector3& v1,Vector3& v2);

const Vector3 operator+(const Vector3& v1, const Vector3& v2);