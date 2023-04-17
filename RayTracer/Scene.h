#pragma once

class MaterialSystem;
class Primitives;
class LightObject;
class SlavMath::Vector3;
class SlavMath::Vector2;

enum class Elements
{
	Primitives,
	Ligths
};

struct SceneData
{
	SceneData(unsigned int max);
	~SceneData();
	SceneData(SceneData& other) = delete;
	SceneData& operator=(SceneData& other) = delete;

	unsigned int m_PrimIt = 0, m_LightIt = 0;
	const unsigned int m_MaxElements = 100;

	bool SpaceAvailable(Elements element) const;
	void AddElement(Elements type, void* element);

	Primitives** m_PrimList = nullptr;
	LightObject** m_LigthList = nullptr;
};

class Scene
{
public:
	Scene(const char* file = "");
	~Scene();
	Scene(Scene& other) = delete;
	Scene& operator=(Scene& other) = delete;

	const SceneData& GetScene() const { return data; };

	void CreateBasicScene();
	bool LoadScene(const char* file);

public:
	void AddSphere(SlavMath::Point3D position, float radius, int&& a_Material_Index);
	void AddCube(SlavMath::Vector3 position, float size, int&& a_Material_Index);
	void AddCube(SlavMath::Vector3 Vmin, SlavMath::Vector3 Vmax, int&& a_Material_Index);

	void AddCube(SlavMath::Vector3&& position, SlavMath::Vector3&& size, SlavMath::Matrix3&& modelmatrix, int&& a_Material_Index);
	void AddCube(SlavMath::Vector3&& position, SlavMath::Vector3&& size, const SlavMath::Matrix3& modelmatrix, int&& a_Material_Index);

	void AddPlane(SlavMath::Vector3 normal, float d, int&& a_Material_Index, const bool& is_Checker);
	void AddPointLight(SlavMath::Point3D position, SlavMath::Color&& color);
	void AddRectLight(SlavMath::Point3D position, SlavMath::Vector2 dimensions, SlavMath::Vector3 normal, SlavMath::Color&& color);

private:
	MaterialSystem* m_MaterialSystem = nullptr;
	SceneData data;
};

