#include "precomp.h"
#include "SlavFrame.h"
#include "Primitives.h"
using namespace SlavMath;

Screen::Screen(int m_Width, int m_Height) :
	m_Size(m_Width* m_Height),
	m_Width(m_Width),
	m_Height(m_Height)
{
	m_Buffer = new unsigned int[m_Size];
}

void Screen::Clear() const
{
	memset(m_Buffer, 0, m_Size * sizeof(int));
}


SlavFrame::SlavFrame() :
	m_Camera(nullptr),
	m_Screen(nullptr)
{
#if !USE_TEMPLATE_STRUCTURE
	m_ObjectsList = new Primitives* [m_POOLSIZE];
	m_LightList = new PointLight* [m_POOLSIZE];
	int i;

	for (i = 0; i < m_POOLSIZE; i++)
	{
		m_ObjectsList[i] = nullptr;
	}
	for (i = 0; i < m_POOLSIZE; i++)
	{
		m_LightList[i] = nullptr;
	}

#endif
}

void SlavFrame::Update()
{
	m_Screen->Clear();
#if USE_TEMPLATE_STRUCTURE
	while (m_LightList.Iterate())
	{
		m_LightList.Pointer();
	}
#endif



}

void SlavFrame::Init()
{
	AddPlane(Vector3(0, 1.0f, 0), 0.8f, BLUE);
	AddPlane(Vector3(0, 1.0f, 0), -10.0f, BLUE);
	AddSphere(Point3D(0, 0, 2.0f), 1.0f, Color(0.5f, 0.6f, 0.2f));
	AddLightPoint(Point3D(-1.2f, 3.2f, 0), Color(30.0f));
}

void SlavFrame::SetScreen(Screen* screen)
{
	m_Screen = screen;
}

void SlavFrame::SetCamera(Camera* camera)
{
	m_Camera = camera;
}

#if USE_TEMPLATE_STRUCTURE
void SlavFrame::AddSphere(SlavMath::Point3D position, float radius, SlavMath::Color color)
{
	m_ObjectsList.Add(new Sphere(position, radius, color));
}

void SlavFrame::AddPlane(SlavMath::Vector3 normal, float d, SlavMath::Color color)
{
	m_ObjectsList.Add(new Plane(normal, d, color));
}

void SlavFrame::AddLightPoint(SlavMath::Point3D position, SlavMath::Color color)
{
	m_LightList.Add(new PointLight(position, color));
}
#else
void SlavFrame::AddSphere(SlavMath::Point3D position, float radius, SlavMath::Color color)
{
	m_ObjectsList[m_ObjectIt] = new Sphere(position, radius, color);
	m_ObjectIt++;
}

void SlavFrame::AddPlane(SlavMath::Vector3 normal, float d, SlavMath::Color color)
{
	m_ObjectsList[m_ObjectIt] = new Plane(normal, d, color);
	m_ObjectIt++;
}

void SlavFrame::AddLightPoint(SlavMath::Point3D position, SlavMath::Color color)
{
	m_LightList[m_LightIt] = new PointLight(position, color);
	m_LightIt++;
}
#endif

