#include "precomp.h"
#include "Screen.h"

Screen::Screen(int width, int height)
	: m_Width(width), m_Height(height), m_Size(height * width)
{
	m_Buffer = new unsigned int[m_Size];
}

void Screen::Clear() const
{
	memset(m_Buffer, 0, m_Size * sizeof(int));
}

unsigned int* Screen::GetBuffer()
{
	return m_Buffer;
}

const int Screen::GetSize() const
{
	return m_Size;
}
