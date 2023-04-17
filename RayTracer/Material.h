#ifndef __OBJECT_MATERIAL__
#define __OBJECT_MATERIAL__
#pragma once

/* Material that a primitives will carry or reference
Material contains:
Singular Color --> SlavMath Color / Texture --> Image loaded and stored within 
Reflectivity --> float 0..1.0f

Functionality:
Adding materials through function
--> AddMaterial ( path directory to texture , reflectivity )
? Mip mapping later on ?
128 - 64 - 32 - 16 - 8 - 4 - 2 - 1 MIP map pattern

For now
--> AddMaterial (SlavMath Color, reflectivity);

*/ 
namespace Materials
{
	enum Index_Materials
	{
		DEBUG_PINK,
		BLACK,
		BLUE,
		GREEN,
		YELLOW,
		RED,
		CYAN,
		WHITE,
		MIRROR,
		SHINE,
		METAL,
		GOLD,
		COBALT,
		OBSIDIAN,
		WATER,
		GLASS,
		LOWINDEX,
		AMBER,
		AIR,
		FALSEMIRROR,
		FUZZYGLASS,
		REDGLASS,
		SIZE_LIST
	};
}

constexpr int MAX_MATERIAL_POOL = Materials::Index_Materials::SIZE_LIST;

class Material
{
public:
	Material();
	void Init(SlavMath::Color* a_Texture, const float& a_Reflectivity = 0, const bool& a_IsDieletric = false);
	const SlavMath::Color* const GetTexture() const;
	const float& GetReflectivity() const;
	const float& GetIndex() const;
	bool IsReflective() const;
	bool IsDieletric() const;
private:
	float m_Reflectivity;
	bool m_Dieletric;
	SlavMath::Color* m_Texture;
};

class MaterialSystem
{
public:
	MaterialSystem();
	void AddMaterial(SlavMath::Color* a_Texture, float&& a_Reflectivity = 0, const float& a_IsDieletric = false);
	Material& AssignMaterial(const int& a_Index);
private:
	Material m_MaterialPool[MAX_MATERIAL_POOL];
	int m_PoolIt;
};

#endif
