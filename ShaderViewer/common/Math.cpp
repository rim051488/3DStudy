#include <DxLib.h>
#include <cmath>
#include "Math.h"

float GetNormalizedAngle(float angle, float valMin, float valMax)
{
	float cycle = valMax - valMin;
	float result = std::fmod((angle - valMin), cycle + valMin);
	if (result < valMin)
	{
		result += cycle;
	}
	return result;
}

double GetNormalizedAngle(double angle, double valMin, double valMax)
{
	double cycle = valMax - valMin;
	double result = std::fmod((angle - valMin), cycle + valMin);
	if (result < valMin)
	{
		result += cycle;
	}
	return result;
}

void Vector3ToDxVector(const Vector3& vec3, VECTOR& dxVec)
{
	dxVec = VGet(vec3.x, vec3.y, vec3.z);
}

