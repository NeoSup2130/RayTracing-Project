#include "precomp.h"
#include "RayTracer.h"
#include "Primitives.h"
#include "Material.h"
#include "Scene.h"

//Preproccer
#define SHOW_NORMAL_COLORS false
#define SHOW_SHADOWS true

#define TEMP_SIZE 100

#define USE_ACCUMULATOR true
#define TEST_RANDOMNESS false
#define USE_SOFT_SHADOWS true

constexpr float HeightInverse = 1.0f / (SCREENHEIGHT - 1);
constexpr float WidthInverse = 1.0f / (SCREENWIDTH - 1);
constexpr char MaxRecursion = 6;

using namespace SlavMath;

void RayTracer::Init()
{
	//float u, v;
	//size_t it = 0;
	//m_TablePixels = new SlavMath::Vector2[(size_t)m_Screen->GetSize()];

	//for (int y = (SCREENHEIGHT - 1); y > -1; y--)
	//{
	//	v = (float)y * HeightInverse;

	//	for (int x = 0; x < SCREENWIDTH; x++)
	//	{
	//		u = (float)x * WidthInverse;
	//		m_TablePixels[it++] = SlavMath::Vector2(u, v);
	//	}
	//}
}

void RayTracer::SetScene(const SceneData* scene)
{
	m_Scene = scene;
}

void RayTracer::SetRNG(MyRandom* rng)
{
	m_Generator = rng;
}

void RayTracer::CastRay(Ray& a_Ray)
{
	for (int i = 0; i < TEMP_SIZE; i++)
	{
		if (m_Scene->m_PrimList[i] == nullptr) break;
		m_Scene->m_PrimList[i]->IntersectionRay(a_Ray);
	}
}

void RayTracer::Trace(Ray& a_Ray, SlavMath::Color& a_Color, SlavMath::Color& a_Emittance, char recur)
{
	a_Color.clear();
	if (!a_Ray.hasHitObject())
	{
		//return background color | skydome
		a_Color = {0.1f, 0.3f, 0.9f};
		return;
	}
#if SHOW_NORMAL_COLORS
	a_Color = a_Ray.GetObject()->GetNormal(a_Ray);
#else
	a_Ray.GetObject()->GetColorAtPoint(a_Ray.At(a_Ray.GetRecordT()), a_Color);
#endif
	if (a_Ray.GetObject()->IsDieletric() && recur < MaxRecursion) // Object is a dieletric
	{
		// Set a ray refracted --> Refracted ray
		// Set a ray reflected --> Primary ray
		// Set indices
		// Calculate eta
		// Calculate Fresnel
		// Cos transmission and Cos reflecive
		// Do reflections first then refractions
		Vector3 direction{ 0 }; 
		Point3D hit = a_Ray.IntersectPoint();
		
		float RayDistance = hit.magnitudeRoot();

		Color cReflection{ 0 };
		Color objectColor = a_Color;
		
		float Fr = Fresnel(a_Ray, direction);
		if (m_RayDistance > 0)
		{
			RayDistance = sqrtf(abs(RayDistance - m_RayDistance));
		}
		float medium = m_RefractedRay.GetRecordIndex();

		if (Fr > 0)
		{
			SetRayToReflected(a_Ray, a_Ray.GetObject()->GetNormal(a_Ray));
			CastRay(a_Ray);
			Trace(a_Ray, a_Color, a_Emittance, ++recur);
			cReflection = a_Color * objectColor;
		}

		if ((1.0f - Fr) > 0)
		{
			m_RefractedRay.Init(hit + direction * EPSILON, direction);
			m_RefractedRay.SetRecordIndex(medium);
			CastRay(m_RefractedRay);
			memcpy(&a_Ray, &m_RefractedRay, sizeof(a_Ray));
			Trace(a_Ray, a_Color, a_Emittance, ++recur);
		}
		a_Color = a_Color * (1.0f - Fr) * objectColor + cReflection * Fr;
		if (RayDistance > 0)
		{
			Vector3 absorb{ 1.0f - objectColor.R, 1.0f - objectColor.G, 1.0f - objectColor.B };
			absorb.R = expf(-absorb.R * RayDistance);
			absorb.G = expf(-absorb.G * RayDistance);
			absorb.B = expf(-absorb.B * RayDistance);
			a_Color *= absorb;
		}

	}
	else if (a_Ray.GetObject()->IsReflective() && recur < MaxRecursion) // Object is reflective
	{
		Color objectColor = a_Color;
		Color objectEmitt = 0;
		const float reflect = a_Ray.GetObject()->GetReflectivity();

		if (1.0f - reflect > 0)
			Illumination(a_Ray, objectEmitt);

		SetRayToReflected(a_Ray, a_Ray.GetObject()->GetNormal(a_Ray));
		CastRay(a_Ray); 
		Trace(a_Ray, a_Color, a_Emittance, ++recur);

		if (1.0f - reflect > 0)
			a_Color = objectColor * objectEmitt * (1.0f - reflect) + objectColor * a_Color * reflect;
		else a_Color *= objectColor;
	}
	else // Object is a diffuse
	{
#if SHOW_SHADOWS
		Illumination(a_Ray, a_Emittance);
		a_Color *= a_Emittance;
#else
		a_Emittance = 1.0f;
		a_Color *= a_Emittance;
#endif
		//If Light are true; cast a shadow ray
		
	}
}

void RayTracer::Illumination(Ray& a_Ray, SlavMath::Color& a_Emittance)
{
	a_Emittance.clear();
	//const Vector3 normal = a_Ray.m_Record.hitObject->getNormal(a_Ray);
	m_HitPoint = a_Ray.IntersectPoint();

	for (int i = 0; i < TEMP_SIZE; i++)
	{
		if (m_Scene->m_LigthList[i] == nullptr) break;
		//If normal dot light dir < 0, then shadowed
#if USE_SOFT_SHADOWS
		m_Length = m_Scene->m_LigthList[i]->GetPointOnArea(m_Generator->GetUniformFloat(), m_Generator->GetUniformFloat()) - m_HitPoint;
#else
		m_Length = m_Scene->m_LigthList[i]->GetPosition() - m_HitPoint;
#endif
		m_Direction = m_Length.normalized();
		m_NdotL = dot(a_Ray.GetObject()->GetNormal(a_Ray), m_Direction);
		if (m_NdotL > 0)
		{
			if (!Occluded(m_HitPoint, m_Direction, m_Length.magnitude() - 2 * EPSILON))
			{
				a_Emittance += m_Scene->m_LigthList[i]->GetColor() * m_NdotL * (1.0f / dot(m_Length, m_Length));
			}
			else if (m_ShadowRay.GetObject()->IsDieletric())
			{
				a_Emittance += 0.5f * (m_Scene->m_LigthList[i]->GetColor() * m_NdotL * (1.0f / dot(m_Length, m_Length)));
			}

		} 
	}
}

bool RayTracer::Occluded(const Point3D& a_Origin, const Vector3& a_Direction, const float& a_Max_T)
{
	m_ShadowRay.Init(a_Origin + a_Direction * EPSILON, a_Direction);
	m_ShadowRay.SetRecord(a_Max_T, nullptr);
	
	for (int i = 0; i < TEMP_SIZE; i++)
	{
		if (m_Scene->m_PrimList[i] == nullptr) break;
		if (m_PrimaryRay.GetObject()->IsDieletric()) continue;
		//if (m_PrimList[i]->IsDieletric()) continue;
		m_Scene->m_PrimList[i]->IntersectionRay(m_ShadowRay);
		if (m_ShadowRay.hasHitObject())
			return true;
	}
	return false;
}

float RayTracer::Fresnel(Ray& a_Ray, SlavMath::Vector3& a_Direction)
{
	memcpy(&m_RefractedRay, &a_Ray, sizeof(a_Ray));

	if (!SetRayToRefracted(m_RefractedRay, a_Ray.GetObject()->GetNormal(a_Ray)))
		return 1.0f;
	a_Direction = m_RefractedRay.GetDirection();

	m_RayDistance = 0;
	float n1{ a_Ray.GetRecordIndex()}, n2{ m_RefractedRay.GetRecordIndex() };
	if (n1 == n2)
	{
		n2 = 1.0f;
		m_RefractedRay.SetRecordIndex(1.0f);
		m_RayDistance = a_Ray.IntersectPoint().magnitudeRoot();
	}
	Vector3 normal = a_Ray.GetObject()->GetNormal(a_Ray);
	float cosIn{ dot(normal, -a_Ray.GetDirection()) };
	float cosTr{ dot(-normal, a_Direction) };

	float s = ((n1 * cosIn - n2 * cosTr) / (n1 * cosIn + n2 * cosTr));
	float p = ((n1 * cosTr - n2 * cosIn) / (n1 * cosTr + n2 * cosIn));

	return 0.5f * (s * s + p * p);
}

void RayTracer::SetRayToReflected(Ray& a_Ray, const SlavMath::Vector3& a_Normal)
{
	a_Ray.SetOrigin(a_Ray.IntersectPoint());
	a_Ray.SetDirection(a_Ray.GetDirection() - 2 * (dot(a_Ray.GetDirection(), a_Normal)) * a_Normal);
	a_Ray.AddOffset(EPSILON);
	a_Ray.ClearRecord();
}

bool RayTracer::SetRayToRefracted(Ray& a_Ray, const SlavMath::Vector3& a_Normal)
{
	float cosi{ dot(a_Normal, -a_Ray.GetDirection()) };
	float eta = a_Ray.GetRecordIndex();

	// Example: If we are leaving water, 1.333f, to air then eta will be 1.333f due to a division with 1.0f
	if (a_Ray.GetRecordIndex() != a_Ray.GetObject()->GetIndex())
	{
		eta /= a_Ray.GetObject()->GetIndex();
		a_Ray.SetRecordIndex(a_Ray.GetObject()->GetIndex());
	}

	float k = 1 - (eta * eta) * (1 - cosi * cosi);

	if (k >= 0)
	{
		a_Ray.SetDirection(eta * a_Ray.GetDirection() + a_Normal * (eta * cosi - sqrtf(k)));
		return true;
	}
	a_Ray.ClearRecord();
	return false;
}

//void RayTracer::SceneCreation()
//{
//	AddSphere(Vector3(0, 0, 0), 50.0f, Materials::CYAN);
//	AddRectLight(Point3D(0, 25.0f, 0), Vector2(10, 10), Vector3{ 0,1,0 }, Color{ 1500 });
//	AddRectLight(Point3D(0, 20.0f, -20), Vector2(10, 10), Vector3{ 0,0,1 }, Color{ 1000 });
//
//	AddPlane(Vector3(0, 1.0f, 0), 1.0f, Materials::SHINE, true);
//	AddPlane(Vector3(0, 0, 1), 3.0f, Materials::MIRROR, false);
//	AddPlane(Vector3(1, 0, 0), 4.0f, Materials::MIRROR, false);
//
//	AddCube(Vector3(-2, 0, 2), Vector3(0.5f, 1, 0.5f), Matrix3().getIdentity(), Materials::FALSEMIRROR);
//
//	Matrix3 angle; angle.setToRotationY(30);
//	AddCube(Vector3(0, 0, 4), Vector3(0.8f, 2, 0.8f), angle, Materials::GLASS);
//	AddCube(Vector3(0, 0.705f, 4), Vector3(0.65f, 1.3f, 0.65f), angle, Materials::WATER);
//	angle.setToIdentity();
//	angle.setToRotationZ(20);
//	AddCube(Vector3(0, 1.2f, 4), Vector3(0.1f, 1.7f, 0.1f), angle, Materials::YELLOW);
//	AddSphere(Vector3(-0.6f, 3.0f, 4), 0.5f, Materials::COBALT);
//
//#if SHOW_SHADOWS 
//	//AddRectLight(Point3D(2, 6, 2), Vector2(5.0f, 5.0f), Vector3{ 2,1,0 }.normalized(), Color{ 60 });
//#endif
//	//CornellBox();
//}

//void RayTracer::CornellBox()
//{
//	AddPlane(Vector3(0, 1.0f, 0), 1.0f, Materials::WHITE, false);
//	AddPlane(Vector3(0, 1.0f, 0), -2.0f, Materials::WHITE, false);
//	AddPlane(Vector3(1, 0, 0), -1.5f, Materials::RED, false);
//	AddPlane(Vector3(0, 0, -1), 3.0f, Materials::WHITE, false);
//	AddPlane(Vector3(0, 0, 1), 0.5f, Materials::BLACK, false);
//	AddPlane(Vector3(-1, 0, 0), -1.5f, Materials::GREEN, false);
//
//	AddSphere(Vector3(-0.7f, -0.65f, 1.3f), 0.35f, Materials::COBALT);
//
//	AddSphere(Vector3(0.5f, -0.65f, 0.9f), 0.35f, Materials::FUZZYGLASS);
//
//	Matrix3 angle; angle.setToRotationY(30);
//	AddCube(Vector3(0.5f, -1, 2.0f), Vector3(0.4f, 1.8f, 0.4f), angle, Materials::COBALT);
//
//	AddRectLight(Point3D(0, 1.7f, 1.3f), Vector2(1.5f, 1.5f), Vector3{ 0,1,0 }, Color{ 3.8f });
//}

/*
* !The right hand rule is used!
*/
void RayTracer::Update(const float& deltaTime)
{
	(void)deltaTime;
#if TEST_RANDOMNESS
	float u, v;
	m_Screen->Clear();
	for (int i = 0; i < m_Screen->GetSize(); i++)
	{
		u = m_Generator->GetAFloatBetween(0.25f, 0.75f);
		v = m_Generator->GetAFloatBetween(0.25f, 0.75f);
		u *= SCREENWIDTH; v *= SCREENHEIGHT;
		m_Screen->GetBuffer()[(size_t)u + (size_t)v * (size_t)SCREENWIDTH] = 0x00FFF0;
	}
#else
	//Color* buffer = m_Accumulator;

	//for (size_t MAX = m_Screen->GetSize(), i = 0; i < MAX; i++)
	//{
	//	buffer[i] += GetPixelColor(m_Camera->GetPosition(), m_Camera->GetRayDirection(
	//		m_TablePixels[i].x + m_Generator->GetAFloatBetween(0.01f, 0.015f),
	//		m_TablePixels[i].y + m_Generator->GetAFloatBetween(0.01f, 0.015f)
	//	));
	//}
	//HandleBufferFilter();
	//printf("Update deltaTime: %fs\n", deltaTime);
#endif
}

SlavMath::Color RayTracer::GetPixelColor(SlavMath::Vector3 pos, SlavMath::Vector3 rayDir)
{
	Color color, emittance;

	m_PrimaryRay.Init(pos, rayDir);
	CastRay(m_PrimaryRay);
	Trace(m_PrimaryRay, color, emittance);

	return color;
}