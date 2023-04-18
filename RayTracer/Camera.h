#ifndef __CameraRayTracer__
#define __CameraRayTracer__
#pragma once
//#include "Vector3.h"

class Camera
{
public:
	Camera();
	//U and V between [0,1]. U ^= x / image width and V ^= y / image height.
	SlavMath::Vector3 GetRayDirection(const float& u, const float& v) const;
	const SlavMath::Vector3& GetPosition() const;
	void MoveSideWays(const float& direction);
	void MoveBackForth(const float& direction);
	void MoveUpDown(const float& direction);
	void RotateAroundY(const float& angle);
private:
	SlavMath::Point3D m_Eye{0}; 
	SlavMath::Vector3 m_ViewDirection{ 0,0,1 }; 
	//The view angle is 30 degrees
	float FOV = 0.87f;

	SlavMath::Point3D m_Screen = m_Eye + m_ViewDirection * FOV;
	SlavMath::Point3D m_Corners[4]{};
};
#endif 

