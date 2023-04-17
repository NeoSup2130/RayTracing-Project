#ifndef __MY_SCREEN_RAY__
#pragma once

class Screen
{
public:
	Screen(int width, int height);
	void Clear() const;
	unsigned int* GetBuffer();
	const int GetSize() const;
private:
	unsigned int* m_Buffer;
	int m_Width, m_Height;
	const int m_Size;
};

#define __MY_SCREEN_RAY__
#endif // !__MY_SCREEN_RAY__

