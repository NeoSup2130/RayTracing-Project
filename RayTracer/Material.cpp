#include "precomp.h"
#include "Material.h"

Material::Material() :
	m_Texture(nullptr), m_Reflectivity(0), m_Dieletric(false)
{
}

void Material::Init(SlavMath::Color* a_Texture, const float& a_Reflectivity, const bool& a_IsDieletric)
{
	m_Texture = a_Texture;
	m_Reflectivity = a_Reflectivity;
	m_Dieletric = a_IsDieletric;
}

const SlavMath::Color* const Material::GetTexture() const
{
	return m_Texture;
}

const float& Material::GetReflectivity() const
{
	return m_Reflectivity;
}

const float& Material::GetIndex() const
{
	return m_Reflectivity;
}

bool Material::IsReflective() const
{
	return m_Reflectivity > 0;
}

bool Material::IsDieletric() const
{
	return m_Dieletric;
}

//bool Material::IsSet()
//{
//	return m_Texture != nullptr;
//}

MaterialSystem::MaterialSystem() :
	m_PoolIt(0)
{
	//Debug pink
	AddMaterial(new SlavMath::Color(1.0f, 0.05f, 0.88f));
	//Black
	AddMaterial(new SlavMath::Color(0, 0, 0));
	//Blueish
	AddMaterial(new SlavMath::Color(0.06f, 0.11f, 1.0f));
	//Greenish
	AddMaterial(new SlavMath::Color(0.14f, 1.0f, 0.48f));
	//Yellowish
	AddMaterial(new SlavMath::Color(1.0f, 0.94f, 0.16f));
	//Redish
	AddMaterial(new SlavMath::Color(1.0f, 0.16f, 0.28f));
	//Cyanish
	AddMaterial(new SlavMath::Color(0.21f, 1.0f, 0.95f));
	//White
	AddMaterial(new SlavMath::Color(1.0f, 1.0f, 1.0f));
	//Mirror
	AddMaterial(new SlavMath::Color(1.0f, 1.0f, 1.0f), 1.0f);
	//Plane shine
	AddMaterial(new SlavMath::Color(1.0f, 1.0f, 1.0f), 0.7f);
	//Metal
	AddMaterial(new SlavMath::Color(0.8f, 0.8f, 0.8f), 0.3f);
	//Gold
	AddMaterial(new SlavMath::Color(1.0f, 0.85f, 0.3f), 0.1f);
	//Cobalt
	AddMaterial(new SlavMath::Color(0.5f, 0.5f, 1.0f), 0.55f);
	//Obsidian
	AddMaterial(new SlavMath::Color(0.44f, 0.27f, 1.0f), 0.2f);
	//Water
	AddMaterial(new SlavMath::Color(0.8f, 0.8f, 1.0f), 1.333f, true);
	//Glass
	AddMaterial(new SlavMath::Color(1.0f, 1.0f, 1.0f), 2.53f, true);
	//Small Index
	AddMaterial(new SlavMath::Color(1.0f, 1.0f, 1.0f), 0.01f, true);
	//Amber
	AddMaterial(new SlavMath::Color(0.8f, 0.59f, 0.42f), 1.55f, true);
	//Air
	AddMaterial(new SlavMath::Color(1.0f, 1.0f, 1.0f), 1.00f, true);
	//False mirror
	AddMaterial(new SlavMath::Color(1.0f, 1.0f, 1.0f), 0.90f, false);
	//Fuzzy glass
	AddMaterial(new SlavMath::Color(0.9f, 0.9f, 0.9f), 2.53f, true);
	//Red glass
	AddMaterial(new SlavMath::Color(1.0f, 0.9f, 0.9f), 1.52f, true);
}

void MaterialSystem::AddMaterial(SlavMath::Color* a_Texture, float&& a_Reflectivity, const float& a_IsDieletric)
{
	m_MaterialPool[m_PoolIt].Init(a_Texture, a_Reflectivity, a_IsDieletric);
	m_PoolIt++;
}

Material& MaterialSystem::AssignMaterial(const int& a_Index) 
{
	if (a_Index > m_PoolIt) return m_MaterialPool[0];
	return m_MaterialPool[a_Index];
}
