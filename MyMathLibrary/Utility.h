#include <String.h>
#include <math.h>
#include <assert.h>

#define MyMathLibrary_Latest_Revision "22/01/2021"

#define FLOAT_SWAP(x, y, temp) temp = x; x = y; y = temp;
#define PI 3.141592653f
#define radian (PI / 180.0f)
#define ANGLE_TO_RADIAN(angle) angle * radian

typedef unsigned int Pixel;

#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))

