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

int DirNearAroundRad(float from, float to)
{
    float ret = 1.0f;
    float diff = to - from;
    if (diff >= 0.0f)
    {
        // 比較元よりも時計回りに位置する
        if (diff > DX_PI_F)
        {
            // でも、180度以上離れているので、反時計回りの方が近い
            ret = -1.0f;
        }
        else
        {
            // 時計回り
            ret = 1.0f;
        }
    }
    else
    {
        // 比較元よりも反時計回りに位置する
        if (diff < -DX_PI_F)
        {
            // でも、180度以上離れているので、時計回りの方が近い
            ret = 1.0f;
        }
        else
        {
            // 反時計回り
            ret = -1.0f;
        }
    }
    return ret;
}

float RadIn2PI(double rad)
{
    rad = fmodf(rad, DX_TWO_PI);
    if (rad < 0.0f)
    {
        rad += DX_TWO_PI;
    }
    return rad;
}

void Vector3ToDxVector(const Vector3& vec3, VECTOR& dxVec)
{
	dxVec = VGet(vec3.x, vec3.y, vec3.z);
}

