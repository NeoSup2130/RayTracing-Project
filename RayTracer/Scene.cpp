#include "precomp.h"
#include "Primitives.h"
#include "Material.h"
#include "Scene.h"

using namespace SlavMath;

SceneData::SceneData(unsigned int max) : m_MaxElements(max)
{
	m_PrimList = new Primitives *[m_MaxElements];
	m_LigthList = new LightObject *[m_MaxElements];

	for (unsigned int i = 0; i < m_MaxElements; i++)
	{
		m_PrimList[i] = nullptr;
		m_LigthList[i] = nullptr;
	}
}

SceneData::~SceneData()
{
	delete[] m_PrimList;
	delete[] m_LigthList;
}

bool SceneData::SpaceAvailable(Elements element) const
{
	switch (element)
	{
	case Elements::Primitives:
		return m_PrimIt < m_MaxElements;
		break;
	case Elements::Ligths:
		return m_LightIt < m_MaxElements;
		break;
	default:
		return false;
	}
}

void SceneData::AddElement(Elements type, void* element)
{
	if (!SpaceAvailable(type)) return;

	switch (type)
	{
	case Elements::Primitives:
		m_PrimList[m_PrimIt] = static_cast<Primitives*>(element);
		m_PrimIt++;
		break;
	case Elements::Ligths:
		m_LigthList[m_LightIt] = static_cast<LightObject*>(element);
		m_LightIt++;
		break;
	}
}

Scene::Scene(const char* file) : m_MaterialSystem(new MaterialSystem()), data(100)
{
	if (!LoadScene(file))
	{
		CreateBasicScene();
	} 
}

Scene::~Scene()
{
}

void Scene::CreateBasicScene()
{
	AddSphere(Vector3(0, 0, 0), 50.0f, Materials::CYAN);
	AddRectLight(Point3D(0, 25.0f, 0), Vector2(10, 10), Vector3{ 0,1,0 }, Color{ 1200 });

	AddPlane(Vector3(0, 1.0f, 0), 1.0f, Materials::WHITE, true);
	AddPlane(Vector3(0, 0, 1), 3.0f, Materials::MIRROR, false);
	AddPlane(Vector3(1, 0, 0), 4.0f, Materials::MIRROR, false);

	AddCube(Vector3(-2, 0, 2), Vector3(0.5f, 1, 0.5f), Matrix3().getIdentity(), Materials::FALSEMIRROR);

	Matrix3 angle; angle.setToRotationY(30);
	AddCube(Vector3(0, 0, 4), Vector3(0.8f, 2, 0.8f), angle, Materials::GLASS);
	AddCube(Vector3(0, 0.705f, 4), Vector3(0.65f, 1.3f, 0.65f), angle, Materials::WATER);
	angle.setToIdentity();
	angle.setToRotationZ(20);
	AddCube(Vector3(0, 1.2f, 4), Vector3(0.1f, 1.7f, 0.1f), angle, Materials::YELLOW);
	AddSphere(Vector3(-0.6f, 3.0f, 4), 0.5f, Materials::COBALT);
}

bool Scene::LoadScene(const char* file)
{
	(void*)file;
	return false;
}

void Scene::AddSphere(Point3D position, float radius, int&& a_Material_Index)
{
	data.AddElement(Elements::Primitives, new Sphere(position, radius, &m_MaterialSystem->AssignMaterial(a_Material_Index)));
}
//Adds an axis aligned bounding box to the scene with a position and a float size.
void Scene::AddCube(SlavMath::Vector3 position, float size, int&& a_Material_Index)
{
	data.AddElement(Elements::Primitives, new AxisAlignedBox(position, size, &m_MaterialSystem->AssignMaterial(a_Material_Index)));
}
//Adds an axis aligned bounding box to the scene with a Vmin and a Vmax.
void Scene::AddCube(SlavMath::Vector3 Vmin, SlavMath::Vector3 Vmax, int&& a_Material_Index)
{
	data.AddElement(Elements::Primitives, new AxisAlignedBox(Vmin, Vmax, &m_MaterialSystem->AssignMaterial(a_Material_Index)));
}
//Adds an orientated bounding box to the scene with a position, a vector size and a model matrix.
void Scene::AddCube(SlavMath::Vector3&& position, SlavMath::Vector3&& size, SlavMath::Matrix3&& modelmatrix, int&& a_Material_Index)
{
	data.AddElement(Elements::Primitives, new OrientatedBox(position, size, modelmatrix, &m_MaterialSystem->AssignMaterial(a_Material_Index)));
}
//Adds an orientated bounding box to the scene with a position, a size and a model matrix.
void Scene::AddCube(SlavMath::Vector3&& position, SlavMath::Vector3&& size, const SlavMath::Matrix3& modelmatrix, int&& a_Material_Index)
{
	data.AddElement(Elements::Primitives, new OrientatedBox(position, size, modelmatrix, &m_MaterialSystem->AssignMaterial(a_Material_Index)));
}
void Scene::AddPlane(Vector3 normal, float d, int&& a_Material_Index, const bool& is_Checker)
{
	data.AddElement(Elements::Primitives, new Plane(normal, d, &m_MaterialSystem->AssignMaterial(a_Material_Index), is_Checker));
}
void Scene::AddPointLight(Point3D position, SlavMath::Color&& color)
{
	data.AddElement(Elements::Ligths, new PointLight(position, color));
}
void Scene::AddRectLight(SlavMath::Point3D position, SlavMath::Vector2 dimensions, SlavMath::Vector3 normal, SlavMath::Color&& color)
{
	data.AddElement(Elements::Ligths, new RectangleLight(position, dimensions, normal, color));
}
