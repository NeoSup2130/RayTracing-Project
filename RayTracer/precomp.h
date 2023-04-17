#ifndef __PRECOMPILED_HEADER__
#define __PRECOMPILED_HEADER__

#include <cinttypes>
#include <cstdlib>
#include <fstream>
#include <cstdio>

//typedef unsigned int Pixel;
#ifdef _MSC_VER
//Nonstandard extension: nameless struct/union
//Disabled as this is only a compiler dependent warning --> my target platform is only MSVC.
#pragma warning (disable : 4201) 
#include "MyMathLibrary.h"
#endif
//#define SDL_MAIN_HANDLED --> fix for SDL macro on main function
#include <SDL.h>
#include "Framework.h"
#include "Camera.h"
#include "Screen.h"

constexpr int SCREENWIDTH = 576;
constexpr float ASPECT_RATIO = 3.0f / 4.0f; //4:3
constexpr float EPSILON = 0.00001f;
constexpr int SCREENHEIGHT = static_cast<int>(SCREENWIDTH * ASPECT_RATIO);
constexpr float SEC = 1000.0f;

#endif