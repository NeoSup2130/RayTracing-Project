#include "precomp.h"
#include "Camera.h"

using namespace SlavMath;

Camera::Camera()
{
	m_Corners[0] = m_Screen - Vector3(-1,  1 * ASPECT_RATIO, 0);
	m_Corners[1] = m_Screen - Vector3( 1,  1 * ASPECT_RATIO, 0);
	m_Corners[2] = m_Screen - Vector3(-1, -1 * ASPECT_RATIO, 0);
	m_Corners[3] = m_Corners[1] - m_Corners[0] + m_Corners[2];
}

SlavMath::Vector3 Camera::GetRayDirection(const float&  u, const float&  v) const
{
	Vector3 point = m_Corners[0] + u * (m_Corners[1] - m_Corners[0]) + v * (m_Corners[2] - m_Corners[0]) - m_Eye;
	point.normalize();
	return point;
}
const SlavMath::Vector3& Camera::GetPosition() const
{
	return m_Eye;
}
void Camera::MoveSideWays(const float& direction)
{
	SlavMath::Vector3 perpendicular(m_ViewDirection.z * direction, 0, -m_ViewDirection.x * direction);
	m_Eye += perpendicular * 0.1f;
	for (int i = 0; i < sizeof(m_Corners) / sizeof(SlavMath::Point3D); i++)
		m_Corners[i] += perpendicular * 0.1f;
	m_Screen = m_Eye + m_ViewDirection * FOV;
}
void Camera::MoveBackForth(const float& direction)
{
	m_Eye += m_ViewDirection * direction * 0.1f;
	for (int i = 0; i < sizeof(m_Corners) / sizeof(SlavMath::Point3D); i++)
		m_Corners[i] += m_ViewDirection * direction * 0.1f;
	m_Screen = m_Eye + m_ViewDirection * FOV;
}

void Camera::MoveUpDown(const float& direction)
{
	SlavMath::Vector3 perpendicular(m_ViewDirection.z, 0, -m_ViewDirection.x);
	perpendicular = crossProduct(perpendicular, m_ViewDirection);
	perpendicular *= direction;
	m_Eye += perpendicular * 0.1f;
	for (int i = 0; i < sizeof(m_Corners) / sizeof(SlavMath::Point3D); i++)
		m_Corners[i] += perpendicular * 0.1f;
	m_Screen = m_Eye + m_ViewDirection * FOV;
}

void Camera::RotateAroundY(const float& angle)
{
	SlavMath::Matrix3 mat;
	mat.setToRotationY(angle);
	m_ViewDirection.Transform(mat);
	
	SlavMath::Vector3 first{ m_ViewDirection.z, 0, -m_ViewDirection.x };
	SlavMath::Vector3 second{ crossProduct(first, m_ViewDirection) };
	m_Screen = m_Eye + m_ViewDirection * FOV;
	
	m_Corners[0] = m_Screen + first + second * ASPECT_RATIO;
	m_Corners[1] = m_Screen - first + second * ASPECT_RATIO;
	m_Corners[2] = m_Screen + first - second * ASPECT_RATIO;
	m_Corners[3] = m_Corners[1] - m_Corners[0] + m_Corners[2];
}
