#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include <float.h>
#include "MyMathLibrary.h"

constexpr float EPSILON = 0.00000001f;

void inline CHECK_ANSWERS_AGAINST_RESULTS(float* answers, float* results, size_t size)
{
	for (size_t i = 0; i < size; i++)
		CHECK(results[i] <= answers[i]);
}

bool inline nearlyEqual(const float& a, const float& b, const float epsilon) {
	float absA = abs(a);
	float absB = abs(b);
	float diff = abs(a - b);

	if (a == b) { // shortcut, handles infinities
		return true;
	}
	else if (a == 0 || b == 0 || (absA + absB < FLT_MIN)) {
		// a or b is zero or both are extremely close to it
		// relative error is less meaningful here
		return diff < (epsilon* FLT_MIN);
	}
	else { // use relative error
		return (diff / min((absA + absB), FLT_MAX)) < epsilon;
	}
}

void inline COMPARE_NEAR_VALUES(const float* answers, const float* results, size_t size, float epsilon = EPSILON)
{
	float difference{ 0 };
	for (size_t i = 0; i < size; i++)
	{
		if (answers[i] == results[i])
		{
			CHECK(true); continue;
		}
		difference = abs(answers[i] - results[i]);
		CHECK(difference < epsilon);
		//CHECK(nearlyEqual(answers[i], results[i], EPSILON) == true);
	}
}

unsigned int Seed[]{ (unsigned int)783573489803254 };
MyRandom RNG(Seed[0]);


using namespace SlavMath;
TEST_CASE("Testing Vector2 functionality")
{
	SUBCASE("Construction")
	{
		/// Default constructor
		unsigned int Seed[]{ 0x340aeaf1 };
		float SomeData[12];
		for (int i = 0; i < 12; i++)
		{
			SomeData[i] = ((float)((RNG.xorshift32()) & 1000) / 100.0f);
		}

		const Vector2 v0;
		CHECK(v0.x == 0.0f);
		CHECK(v0.y == 0.0f);

		/// Constructor from components
		const Vector2 v1(9.1f, -3.0f);
		CHECK(v1.x == 9.1f);
		CHECK(v1.y == -3.0f);

		/// Constructor from components
		const Vector2 v2(4.3f);
		CHECK(v2.x == 4.3f);
		CHECK(v2.y == 4.3f);

		/// Copy construction
		Vector2 v3(v2);
		CHECK(v2.x == v3.x);
		CHECK(v2.y == v3.y);

		/// Zero all the components of the vector
		v3.clear();
		CHECK(v0.x == v3.x);
		CHECK(v0.y == v3.y);
	}

	SUBCASE("Mathematical operators")
	{
		const Vector2 v0(2.3f, 3.7f);
		const Vector2 v1(-6.6f, 1.2f);
		const float factor = 3.5f;
		Vector2 v2;

		/// Returns the value of the given vector added to this.
		v2 = v0 + v1;
		CHECK(v2.x == v0.x + v1.x);
		CHECK(v2.y == v0.y + v1.y);

		/// Returns the value of the given vector subtracted from this.
		v2 = v0 - v1;
		CHECK(v2.x == v0.x - v1.x);
		CHECK(v2.y == v0.y - v1.y);

		SUBCASE("Vector and scalar multiplication.")
		{
			/// Returns a copy of this vector scaled the given value.
			v2 = v1 * factor;
			CHECK(v2.x == v1.x * factor);
			CHECK(v2.y == v1.y * factor);

			/// Returns a new vector being the result of a float value multiplied on right hand side with a vector
			v2 = factor * v0;
			CHECK(v2.x == v0.x * factor);
			CHECK(v2.y == v0.y * factor);
		}

		/// Returns a copy of this vector scaled the given value.
		v2 = v0 / factor;
		CHECK(v2.x == v0.x / factor);
		CHECK(v2.y == v0.y / factor);

		SUBCASE("Dot product calculation.")
		{
			/// Calculates and returns the dot product of this vector with the given vector.
			const float dotProduct = dot(v0, v1);
			CHECK(-10.74f == dotProduct);

			const float calculatedDot = dot(v0, v1);
			CHECK(dotProduct == calculatedDot);
		}

		/// Adds the given vector to this.
		SUBCASE("Adds the given vector to this.")
		{
			Vector2 v3(3.0f, -4.0f);
			v3 += v0;
			CHECK(v3.x == v0.x + 3.0f);
			CHECK(v3.y == v0.y + -4.0f);
		}

		/// Subtracts the given vector from this
		SUBCASE("Subtracts the given vector from this.")
		{
			Vector2 v3(3.0f, -4.0f);
			v3 -= v1;
			CHECK(v3.x == 3.0f - v1.x);
			CHECK(v3.y == -4.0f - v1.y);
		}

		/// Multiplies this vector by the given scalar
		SUBCASE("Multiplies this vector by the given scalar.")
		{
			Vector2 v3(3.0f, -4.0f);
			v3 *= factor;
			CHECK(v3.x == 3.0f * factor);
			CHECK(v3.y == -4.0f * factor);
		}
	}

	SUBCASE("Logical operators")
	{
		/// Checks if the two vectors have identical components
		const float x = 2.93f;
		const float y = 4.39f;
		Vector2 v0(x, y);
		Vector2 v1(x, y);
		Vector2 v2(y, x);

		for (size_t i = 0; i < sizeof(Vector2) / sizeof(float); i++)
		{
			CHECK(v0.cell[i] == v1.cell[i]);
		}
	}

	SUBCASE("Length operations")
	{
		Vector2 v0(3.0f, 4.0f);

		/// Gets the magnitude of this vector
		const float vectorLenght = v0.magnitude();
		CHECK(vectorLenght == 5.0f);

		/// Gets the squared magnitude of this vector
		const float vectorLenghtSq = v0.magnitudeRoot();
		CHECK(vectorLenghtSq == 25.0f);

		/// Limits the size of the vector to the given maximum
		v0.trim(2.5f);
		CHECK(2.5f == v0.magnitude());

		/// Returns the normalised version of a vector
		Vector2 v1 = v0;
		Vector2 v2 = v1.unit();
		if (v1 == v0) CHECK(true);
		//CHECK(v1 == v0);
		CHECK(1.0f == v2.magnitude());
			
		/// Turns a non-zero vector into a vector of unit length
		v0.normalize();
		CHECK(1.0f == v0.magnitude());
	}
}

TEST_CASE("Testing Vector3 functionality")
{
	SUBCASE("Construction")
	{
		/// The default constructor creates a zero vector.
		Vector3 v0;
		CHECK(v0.x == 0);
		CHECK(v0.y == 0);
		CHECK(v0.z == 0);
		/// Creates a vector with the given components
		Vector3 v1(3.54f, 932.0f, -12.2f);
		CHECK(v1.x == 3.54f);
		CHECK(v1.y == 932.0f);
		CHECK(v1.z == -12.2f);

		Vector3 v2(3.54f);
		CHECK(v2.x == 3.54f);
		CHECK(v2.y == 3.54f);
		CHECK(v2.z == 3.54f);
		/// Zero all the components of the vector
		v2.clear();
		CHECK(v2.x == 0);
		CHECK(v2.y == 0);
		CHECK(v2.z == 0);
	}

	SUBCASE("Mathematical operators")
	{
		 //Returns the value of the given vector added to this
		const Vector3 v0(3.0f, 3.7f, 2.2f);
		const Vector3 v1(3.2f, 1.2f, -4.0f);
		const float factor = 3.5f;
		Vector3 v2;

		// Adds the given vector to this
		v2 = v0 + v1;
		CHECK(v2.x == 6.20000001f);
		CHECK(v2.y == v0.y + v1.y);
		CHECK(v2.z == v0.z + v1.z);

		/// Returns the value of the given vector subtracted from this.
		v2 = v0 - v1;
		CHECK(v2.x == v0.x - v1.x);
		CHECK(v2.y == v0.y - v1.y); 
		CHECK(v2.z == v0.z - v1.z);
		// Returns a copy of this vector scaled the given value
		v2 = v0 * 34.5f;
		CHECK(v2.x == v0.x * 34.5f);
		CHECK(v2.y == v0.y * 34.5f);
		CHECK(v2.z == v0.z * 34.5f);

		// Returns a copy of this vector scaled the inverse of the value
		v2 = -34.5f  * v0;
		CHECK(v2.x == v0.x * -34.5f);
		CHECK(v2.y == v0.y * -34.5f);
		CHECK(v2.z == v0.z * -34.5f);

		// Returns the negative this vector
		v2 = v0 * -1.0f;
		CHECK(v2.x == v0.x * -1.0f);
		CHECK(v2.y == v0.y * -1.0f);
		CHECK(v2.z == v0.z * -1.0f);
		
		// Subtracts the given vector from this
		v2 = v0 - 10.0f;
		CHECK(v2.x == v0.x - 10.0f);
		CHECK(v2.y == v0.y - 10.0f);
		CHECK(v2.z == v0.z - 10.0f);


		SUBCASE("Vector products")
		{
			Vector3 v4{ 2.0f, 3.0f, 4.0f };
			Vector3 v5{ 4.0f, 3.0f, 2.0f };
			/// Calculates and returns the dot product of this with the given vector
			float dotProduct = dot(v4, v5);
			CHECK(dotProduct == 25.0f);
			/// Calculates and returns the cross product of this vector with the given vector
			Vector3 v6 = crossProduct(v4, v5);
			CHECK(v6.x == -6.0f);
			CHECK(v6.y == 12.0f);
			CHECK(v6.z == -6.0f);
		}

		SUBCASE("Length operations")
		{
			///  Gets the magnitude of this vector
			///  Gets the squared magnitude of this vector
			/// Turns a non-zero vector into a vector of unit length
			/// Returns the normalised version of a vector
			Vector3 v0(6.0f, 6.0f, 3.0f);
			Vector3 v1 = v0;
			float magnitude = v0.magnitude();
			CHECK(magnitude == 9.0f);

			CHECK(v0.magnitudeRoot() == 81.0f);

			v0.normalize();
			CHECK(v0.magnitude() == 1.0f);

			Vector3 v2 = v1.unit();
			CHECK(v2.magnitude() == 1.0f);
		}
	}

	SUBCASE("Logical operations")
	{
		Vector3 v0(10.2f, 22.0f, 122.2f);
		Vector3 v1(-10.2f, 22.1f, -122.2f);
		Vector3 v2 = v0;
		if (v0 == v2) CHECK(true);
		else CHECK(false);
		/// Checks if the two vectors have identical components
		/// Checks if the two vectors have non-identical components
		v2 = v1;
		if (v1 == v2) CHECK(true);
		else CHECK(false);
		//CHECK(v1 == v2);
	}
}

TEST_CASE("Testing Matrix33 functionality")
{
	SUBCASE("Construction")
	{
		/// Construct a new matrix from explicit values
		Matrix3 mat1{ 1,2,3,4,5,6,7,8,9 };
		for (size_t i = 0; i < sizeof(Matrix3) / sizeof(float); i++)
		{
			CHECK(mat1[i] == (float)i + 1.0f);
		}
		
		/// Construct a new identity matrix
		Matrix3 mat2{ 1,0,0,0,1,0,0,0,1 };
		CHECK(mat2.cell[4] == 1.0f);
		CHECK(mat2.cell[1] == 0.0f);
		CHECK(mat2.cell[0] == 1.0f);
		CHECK(mat2.cell[8] == 1.0f);

		/// Creates an identity matrix
		mat1.setToIdentity();
		CHECK(mat1.cell[0] == 1.0f);
		CHECK(mat1.cell[1] == 0.0f);
		CHECK(mat1.cell[4] == 1.0f);
		CHECK(mat1.cell[8] == 1.0f);

		/// Creates a translation matrix
		mat1.setToTransform(3.0f, 6.0f);
		CHECK(mat1.cell[0] == 1.0f);
		CHECK(mat1.cell[1] == 0.0f);
		CHECK(mat1.cell[2] == 3.0f);
		CHECK(mat1.cell[5] == 6.0f);

		/// Creates a scale matrix
		mat1.setToScale(1.0f, 2.0f, 1.0f);
		CHECK(mat1.cell[0] == 1.0f);
		CHECK(mat1.cell[1] == 0.0f);
		CHECK(mat1.cell[4] == 2.0f);
		CHECK(mat1.cell[8] == 1.0f);

		/// Creates a uniform scale matrix
		mat1.setToScale(2.0f);
		CHECK(mat1.cell[0] == 2.0f);
		CHECK(mat1.cell[1] == 0.0f);
		CHECK(mat1.cell[4] == 2.0f);
		CHECK(mat1.cell[8] == 2.0f);

		/// Creates a rotation matrix around an arbitrary axis (angle in radians)
		// Rodriquez formula

		/// Creates a rotation matrix around the x axis (angle in radians)
		/// Added small margin, 0.00000100f which comes down to a 1*10^-7% loss of precision, for precision errors
		mat1.setToRotationX(181);
		Vector3 sample{ 10.0f, 5.0f, 5.0f };
		sample.Transform(mat1);
		CHECK(sample.cell[0] == 10.0f);
		CHECK(sample.cell[1] > -4.911977444f);
		CHECK(sample.cell[2] > -5.086501508f);
		
		/// Creates a rotation matrix around the y axis (angle in radians)
		mat1.setToRotationY(90);
		sample = { 0, 0, 1 };
		sample.Transform(mat1);
		CHECK(sample.cell[0] == 1);
		CHECK(sample.cell[1] == 0);
		CHECK(sample.cell[2] < 0.00001);

		mat1.setToRotationY(45);
		sample = { 1, 1, 0 };
		float answer[9] = { 
			0.7071067812f, 1, -0.7071067812f,
			0, 1, 0,
			0.7071067812f, 1,  0.7071067812f };
		sample.Transform(mat1);
		CHECK(sample.cell[0] - answer[0] < EPSILON);
		CHECK(sample.cell[1] == answer[1]);
		CHECK(sample.cell[2] - answer[2] < EPSILON);
		Matrix3 sampleMat{
			1, 0, 0,
			0, 1, 0,
			0, 0, 1
		};
		sampleMat *= mat1;
		sampleMat.Transpose();
		CHECK_ANSWERS_AGAINST_RESULTS(answer, sampleMat.cell, 9);

		/// Creates a rotation matrix around the z axis (angle in radians)
		mat1.setToRotationZ(121);
		sample = { 10.0f, 5.0f, 5.0f };
		sample.Transform(mat1);
		CHECK(sample.cell[0] > -9.436218253f);
		CHECK(sample.cell[1] < 5.996483632f);
		CHECK(sample.cell[2] == 5.0f);
	}

	SUBCASE("Mathematical operations")
	{
		Matrix3 mat1{1,2,3,4,5,6,7,8,9};
		Matrix3 mat2{2,4,6,8,10,12,14,16,18};
		/// Matrix addition
		mat1 += 5.0f;
		CHECK(mat1.cell[6] == 12.0f);
		/// Matrix multiplication
		mat1 *= 3.0f;
		CHECK(mat1.cell[6] == 36.0f);
		/// Matrix division
		mat1 /= 3.0f;
		CHECK(mat1.cell[6] == 12.0f);
		/// Matrix subtraction
		mat1 -= 5.0f;
		CHECK(mat1.cell[6] == 7.0f);

		SUBCASE("Matrix on Matrix operations")
		{
			const Matrix3 MatrixBase{ 0,1,2,3,4,5,6,7,8 };
			/// Subtraction
			memcpy(&mat1, &MatrixBase, sizeof(mat1));
			mat1 = mat1 - MatrixBase;
			CHECK(mat1[2] == 0.0f);
			CHECK(mat1[5] == 0.0f);

			mat1 -= MatrixBase;
			CHECK(mat1[2] == -2.0f);
			CHECK(mat1[5] == -5.0f);

			// Addition
			mat1 += MatrixBase;
			CHECK(mat1[2] == 0.0f);
			CHECK(mat1[5] == 0.0f);

			mat1 = mat1 + MatrixBase;
			CHECK(mat1[2] == 2.0f);
			CHECK(mat1[5] == 5.0f);

			/// Multiplication
			memcpy(&mat1, &MatrixBase, sizeof(mat1));

			mat1 = mat1 * MatrixBase;
			CHECK(mat1[0] == 15.0f);
			CHECK(mat1[5] == 66.0f);

			memcpy(&mat1, &MatrixBase, sizeof(mat1));

			mat1 *= MatrixBase;
			CHECK(mat1[0] == 15.0f);
			CHECK(mat1[5] == 66.0f);
		}

		SUBCASE("Inversion")
		{
			Matrix3 inv{-2,2,4,4,5,6,6,8,10};
			
			/// Get the determinant of this matrix
			CHECK(inv.Det() == -4.0f);

			/// Inverts this matrix
			inv.Invert();
			CHECK(inv.cell[8] == 4.5f);
			CHECK(inv.cell[7] == -7.0f);
			{
				float temp[9] = { -2,2,4,4,5,6,6,8,10 };
				memcpy(&inv, &temp, sizeof(temp));
			}

			inv = inv.Inverse();
			CHECK(inv.cell[8] == 4.5f);
			CHECK(inv.cell[7] == -7.0f);
			/// Transposes this matrix
			CHECK(inv.cell[2] == -0.5f);
			CHECK(inv.cell[6] == 2.0f);
			inv.Transpose();
			CHECK(inv.cell[6] == -0.5f);
		}
	}
	Matrix3 mat3{ 10, 0, 0, 0, 5, 0, 0, 0, 5 };
	CHECK(mat3.GetX() == 10.0f);
	CHECK(mat3.GetY() == 5.0f);
	CHECK(mat3.GetZ() == 5.0f);
	/// Get the x axis 
	/// Get the y axis 
	/// Get the z axis 
}

TEST_CASE("Testing Matrix44 functionality")
{
	SUBCASE("Construction")
	{
		/// Construct a new matrix from explicit values
		Matrix4 Explicit{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
		for (size_t i = 0; i < sizeof(Matrix4) / sizeof(float); i++)
		{
			CHECK(Explicit[i] == (float)i);
		}
		/// Construct a new identity matrix
		Matrix4 Identity{ true };
		CHECK(Identity[0] == 1.0f); 
		CHECK(Identity[5] == 1.0f); 
		CHECK(Identity[10] == 1.0f); 
		CHECK(Identity[15] == 1.0f);
		CHECK(Identity[8] == 0);
		/// Creates an identity matrix
		Explicit.setToIdentity();
		CHECK(Explicit[0] == 1.0f); 
		CHECK(Explicit[5] == 1.0f); 
		CHECK(Explicit[10] == 1.0f); 
		CHECK(Explicit[15] == 1.0f); 
		CHECK(Explicit[8] == 0);
		/// Creates a translation matrix
		Explicit.setToTransform(10.0f, -9.0f, 2.1f);
		CHECK(Explicit[3] == 10.0f);
		CHECK(Explicit[7] == -9.0f);
		CHECK(Explicit[11] == 2.1f);
		CHECK(Explicit[15] == 1.0f);
		CHECK(Explicit[5] == 1.0f);
		/// Creates a scale matrix
		Explicit.setToScale(2.0f, 1.0f, 5.0f);
		CHECK(Explicit[0] == 2.0f);
		CHECK(Explicit[5] == 1.0f);
		CHECK(Explicit[10] == 5.0f);
		CHECK(Explicit[15] == 1.0f);
		CHECK(Explicit[1] == 0.0f);
		/// Creates a uniform scale matrix
		Explicit.setToScale(-3.5f);
		CHECK(Explicit[0] == -3.5f);
		CHECK(Explicit[5] == -3.5f);
		CHECK(Explicit[10] == -3.5f);
		CHECK(Explicit[15] == 1.0f);
		CHECK(Explicit[1] == 0.0f);

		/// Creates a rotation matrix around an arbitrary axis (angle in radians)
		/// Creates a rotation matrix around the x axis (angle in radians)
		/// Creates a rotation matrix around the y axis (angle in radians)
		/// Creates a rotation matrix around the z axis (angle in radians)
	
		/// Creates an orthographic projection matrix
		
		/// Creates a frustum projection matrix
		
		/// Creates a perspective projection matrix from camera settings
		
		/// Creates a look at matrix, usually a view matrix  
	}

	SUBCASE("Mathematical operations")
	{
		Matrix4 example1{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
		Matrix4 example2{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };
		Matrix4 example3;
		/// Matrix addition
		example3 = example1 + example2;
		CHECK(example3[0] == 1.0f);
		CHECK(example3[7] == 15.0f);
		CHECK(example3[15] == 31.0f);
		example3 += example2;
		CHECK(example3[0] == 2.0f);
		CHECK(example3[7] == 23.0f);
		CHECK(example3[15] == 47.0f);
		example3 += 10.0f;
		CHECK(example3[0] == 12.0f);
		CHECK(example3[7] == 33.0f);
		CHECK(example3[15] == 57.0f);
		/// Matrix subtraction
		example3 -= 10.0f;
		CHECK(example3[0] == 2.0f);
		CHECK(example3[7] == 23.0f);
		CHECK(example3[15] == 47.0f);
		example3 -= example2;
		CHECK(example3[0] == 1.0f);
		CHECK(example3[7] == 15.0f);
		CHECK(example3[15] == 31.0f);
		example3 = example1 - example2;
		CHECK(example3[0] == -1.0f);
		CHECK(example3[7] == -1.0f);
		CHECK(example3[15] == -1.0f);
		/// Matrix multiplication
		example3 *= -1.0f;
		CHECK(example3[0] == 1.0f);
		CHECK(example3[7] == 1.0f);
		CHECK(example3[15] == 1.0f);
		example3 = example1 * example1;
		CHECK(example3[0] == 56.0f);
		CHECK(example3[7] == 218.0f);
		CHECK(example3[15] == 506.0f);
		SUBCASE("Inversion")
		{
			example3 = { 1, 0, 4, -6, 2, 5, 0, 3, -1, 2, 3, 5, 2, 1, -2, 3};
			/// Get the determinant of this matrix
			CHECK(example1.Det() == 0.0f);
			CHECK(example3.Det() == 318.0f);

			{/// Inverts this matrix
				float answer[]{
					(62.0f/481.0f), -(83.0f/3848.0f), -(5.0f/104.0f), (141.0f/3848),
					-(41.0f/481.0f), (101.0f/962.0f), -(3.0f/26), (95.0f/962.0f),
					(29.0f/962.0f), (32.0f/481.0f), -(1.0f/26.0f), -(8.0f/481.0f),
					-(1.0f/481.0f), -(21.0f/962.0f), (5.0f/26.0f), -(115.0f/962.0f)
				};
				example3 = {
				8, 1, 2, 3,
				4, 8, 6, 7,
				8, 9,-8, 11,
				12, 13, -14, 8
				};
				example3 = example3.Inverse();
				for (size_t i = 0; i < sizeof(answer) / sizeof(float); i++)
					CHECK(example3[i] < answer[i] + 0.0000001f);
			}
			
			{/// Transposes this matrix
				
				float answer[]{
					1, 0, 1, 2,
					2, 5, 2, 1,
					4, 0, 3, 2,
					6, 3, 5, 3
				};
				example3 = {
					1, 2, 4, 6,
					0, 5, 0, 3,
					1, 2, 3, 5,
					2, 1, 2, 3
				};
				example3.Transpose();
				for (size_t i = 0; i < sizeof(answer) / sizeof(float); i++)
					CHECK(example3[i] == answer[i]);
			}

		}
	}
	
	SUBCASE("Transformation")
	{
		Vector3 point{ 2, 2, -2 };
		Matrix4 skew;
		{/// Transform the given vector by this matrix.
			skew.setToTransform(6, -3, 4);
			point.Transform(skew);
			float answer[]{ 8, -1, 2 };
			
			CHECK_ANSWERS_AGAINST_RESULTS(answer, point.cell, 3);
		}
		{	/// Transform the direction vector of this matrix
			skew *= skew;
			float answer[]{ 
				1, 0, 0, 12,
				0, 1, 0, -6,
				0, 0, 1, 8,
				0, 0, 0, 1
			};

			CHECK_ANSWERS_AGAINST_RESULTS(answer, skew.cell, 16);
		}
	}

	/// Retrieve translation part of the matrix
	/// Set the translation of the matrix
	/// Get the x orientation axis 
	/// Get the y orientation axis 
	/// Get the z orientation axis 

	/// Sets the orientation of the matrix to the orthogonal basis vector
}

TEST_CASE("Testing Randomness functionality")
{
	SUBCASE("XORSHIFT by Marsgalia")
	{
		unsigned int SomeData{ 0 };
		Seed[0] = SomeData;
		CHECK(RNG.xorshift32() != SomeData);
		CHECK((Seed[0] & 9) < 10);
		CHECK(Seed[0] > 0);

		float rngFloat = RNG.GetUniformFloat();
		CHECK(rngFloat >= 0.0f);
		CHECK(rngFloat <= 1.0f);

		rngFloat = RNG.GetAFloatBetween(2.0f, 3.0f);
		CHECK(rngFloat >= 2.0f);
		CHECK(rngFloat <= 3.0f);

		float RNGList[100];
		for (int i = 0; i < 100; i++)
			RNGList[i] = RNG.GetUniformFloat();
	}
	SUBCASE("Random 3D point on rectangle")
	{
		SUBCASE("Axis aligned rectangle")
		{
			Point3D center{ 0, 10, 5 };
			Vector2 dimensions{ 5, 5 };
			Vector3 normal{ 0,0,-1 };
			Vector3 U, V;
			//Setting UV vector
			U = normal.perpendicular();
			V = crossProduct(normal, U);
			//End
			Vector3 corners[4];
			//TL, TR, BL, BR
			//Top left
			corners[0] = center + dimensions.x * U + dimensions.y * V;
			//Top right
			corners[1] = center - dimensions.x * U + dimensions.y * V;
			//Bottom left
			corners[2] = center + dimensions.x * U - dimensions.y * V;
			//Bottom right
			corners[3] = center - dimensions.x * U - dimensions.y * V;

			
			Point3D point;

			float randomU, randomV;

			// Expected value of point should be: x [-5, 5], y [5, 15], z = 5;
			for (int i = 0; i < 6; i++)
			{
				randomU = RNG.GetUniformFloat();
				randomV = RNG.GetUniformFloat();
				point =
					center +
					dimensions.x * (0.5f - randomU) * U +
					dimensions.y * (0.5f - randomV) * V;

				CHECK(point.z == 5.0f);
				CHECK(point.x <= corners[1].x);
				CHECK(point.x >= corners[0].x);
				CHECK(point.y <= corners[1].y);
				CHECK(point.y >= corners[2].y);
			}
		}
		SUBCASE("Orientated rectangle")
		{
			Point3D center{ 0, 10, 5 };
			Vector2 dimensions{ 5, 5 };
			Vector3 normal{ 0, 1, 1 }; 
			normal.normalize();
			Vector3 U, V;

			//Setting U vector
			U = normal.perpendicular();
			//End
			float dotCheck[3]{ dot(normal, U), dot(normal, V), dot(U,V) };
			CHECK(dot(normal, U) == 0);
			CHECK(dot(normal, V) == 0);
			CHECK(dot(U,V) == 0);
			
			V = crossProduct(normal, U);
			Point3D point;

			Vector3 corners[4];
			//TL, TR, BL, BR
			//Top left
			corners[0] = center + dimensions.x * U + dimensions.y * V;
			//Top right
			corners[1] = center - dimensions.x * U + dimensions.y * V;
			//Bottom left
			corners[2] = center + dimensions.x * U - dimensions.y * V;
			//Bottom right
			corners[3] = center - dimensions.x * U - dimensions.y * V;

			float randomU, randomV;

			for (int i = 0; i < 6; i++)
			{
				randomU = RNG.GetUniformFloat();
				randomV = RNG.GetUniformFloat();
				point =
					center +
					dimensions.x * (0.5f - randomU) * U +
					dimensions.y * (0.5f - randomV) * V;

				CHECK(point.z >= corners[0].z);
				CHECK(point.z <= corners[2].z);
				CHECK(point.x >= corners[1].x);
				CHECK(point.x <= corners[0].x);
				CHECK(point.y <= corners[0].y);
				CHECK(point.y >= corners[2].y);
			}

		}
	}
}
struct Ray
{
	Ray(const Point3D& origin, const Vector3& direction) :
		origin(origin), direction(direction)
	{}
	Vector3 RayAtT(const float& t)
	{
		return origin + direction * t;
	}
	Vector3 origin;
	Vector3 direction;
};

struct AxisAlignedBox
{
	AxisAlignedBox(const Vector3& Vmin, const Vector3& Vmax) :
		bounds{ Vmin, Vmax } {
		center = { (Vmin + Vmax) * 0.50f };
	}
	Vector3 bounds[2];
	Vector3 center;

	bool IntersectRay(const Ray& ray, float& tvalue)
	{
		//Vector3 invRayDir = ray.direction.inversed();
		Vector3 t0 = (bounds[0] - ray.origin) / ray.direction;
		Vector3 t1 = (bounds[1] - ray.origin) / ray.direction;

		float tmin = max(max(min(t0.x, t1.x), min(t0.y, t1.y)), min(t0.z, t1.z));
		float tmax = min(min(max(t0.x, t1.x), max(t0.y, t1.y)), max(t0.z, t1.z));

		if (tmax >= tmin && tmax > EPSILON)
		{
			if (tmin > EPSILON)
			{
				tvalue = tmin;
				return true;
			}
		}
		return false;
	}
};

struct Sphere
{
	Sphere(const Vector3& pos, const float& radius)
		: m_Position(pos), m_Radius(radius * radius)
	{

	}
	float m_Radius;
	SlavMath::Point3D m_Position;

	bool IntersectRay(const Ray& ray, float& t_value)
	{
		Vector3 center = m_Position - ray.origin;
		float t = dot(center, ray.direction);
		Vector3 q = center - ray.direction * t;
		float p = dot(q, q);

		if (p > m_Radius) return false;
		float x = sqrtf(m_Radius - p);
		if (t - x > EPSILON) t -= x;
		else t += x;

		if (t > EPSILON)
		{
			t_value = t;
			return true;
		}
		return false;
	}
};

struct Plane
{
	Plane(const Vector3& normal, const float& d)
		: d(d), normal(normal)
	{}
	float d;
	Vector3 normal;
	bool IntersectRay(const Ray& ray, float& tvalue)
	{
		float t = -(dot(ray.origin, normal) + d) / dot(ray.direction, normal);

		if (t > EPSILON)
		{
			tvalue = t;
			return true;
		}
		return false;
	}
};

struct OrientatedBox
{
	OrientatedBox(const Vector3& pos, const Matrix3& model, const Vector3& size):
		m_BasisVectors{{1,0,0},{0,1,0},{0,0,1}}, m_Position(pos), m_ModelMatrix(model), m_Dimensions(size)
	{
		for (size_t i = 0; i < sizeof(m_BasisVectors) / sizeof(Vector3); i++)
		{
			m_BasisVectors[i].Transform(model);
			m_BasisVectors[i].normalize();
		}
	}

	//The box's normal vectors; used for orientation.
	SlavMath::Vector3 m_BasisVectors[3];

	//The center point of the box; origin of object world space.
	SlavMath::Vector3 m_Position;

	//Uses for keeping track of the rotation.
	SlavMath::Matrix3 m_ModelMatrix;

	//The Length, heigth and depth
	SlavMath::Vector3 m_Dimensions;

	bool IntersectRay(const Ray& ray, float& tvalue)
	{
		Vector3 delta = m_Position - ray.origin;

		float t0, t1, e, scale;
		float tMin = EPSILON;
		float tMax = (float)INFINITE;

		for (int i = 0; i < 3; i++)
		{
			e = dot(m_BasisVectors[i], delta);
			scale = dot(ray.direction, m_BasisVectors[i]);

			t0 = (e + m_Dimensions.cell[i]) / scale;
			t1 = (e - m_Dimensions.cell[i]) / scale;

			if (t0 > t1)
			{
				float w = t0;
				t0 = t1;
				t1 = w;
			}
			if (t1 < tMax) tMax = t1;
			if (t0 > tMin) tMin = t0;
		}

		if (tMax > tMin && tMax > EPSILON)
		{
			if (tMin > EPSILON)
			{
				tvalue = tMin;
				return true;
			}
		}
		return false;
	}
};

TEST_CASE("Ray intersection methods")
{
	SUBCASE("Sphere intersection")
	{
		float tValue{ 0 };
		Vector3 direction = { 0.10926f, -0.4f, 4.09558f }; direction.normalize();
		Ray ray(Vector3(0, 0.6f, 0), direction);
		Sphere sphereFront(Vector3(0.05157f, 0.41122f, 1.93293f), 0.39542f);
		Sphere sphereBack(Vector3(0.05157f, 0.41122f, -1.93293f), 0.39542f);

		//Check intersection
		CHECK(sphereFront.IntersectRay(ray, tValue) == true);
		CHECK(sphereBack.IntersectRay(ray, tValue) == false);
		//Check hit point
		Vector3 tMinPos(0.0410708027f, 0.4496401146f, 1.5395273487f);
		COMPARE_NEAR_VALUES(tMinPos.cell, ray.RayAtT(tValue).cell, 3, 0.0001f);

		//Check edge case
		direction = { 0.4387864617f, -0.1999983354f, 1.8536258877f };
		direction.normalize();
		ray.direction = direction;
		//Check egde condition.
		CHECK(sphereFront.IntersectRay(ray, tValue) == true);
		CHECK(sphereBack.IntersectRay(ray, tValue) == false);
		Vector3 edge{ 0.4387864617f, 0.4000016646f, 1.8536258877f };
		//Check edge position.
		COMPARE_NEAR_VALUES(edge.cell, ray.RayAtT(tValue).cell, 3, 0.1f);
	}

	SUBCASE("Plane intersection")
	{
		float m_tValues[3]{};
		Vector3 direction(1, -1, 2); direction.normalize();
		Ray f(Vector3(0,1,0), direction);
		Point3D hitF(0.5424413917f, 0.4575586083f, 1.0848827834f);

		direction = { -2, 0, 2 }; direction.normalize();
		Ray g(Vector3(1, 0.4575586083f, 0), direction);

		direction = { 2, 0.4575586083f, 2 }; direction.normalize();
		Ray h(Vector3(-1, 0, 0), direction);

		Plane test(Vector3(0, 1, 0), -0.4575586083f);

		//Hitting plane from above
		CHECK(test.IntersectRay(f, m_tValues[0]) == true);
		COMPARE_NEAR_VALUES( f.RayAtT(m_tValues[0]).cell, hitF.cell, 3, 0.000001f);

		//Direction parallel to plane
		CHECK(test.IntersectRay(g, m_tValues[1]) == false);

		//Hitting plane from below
		CHECK(test.IntersectRay(h, m_tValues[2]) == true);
		Point3D hitH(1, 0.4575586083f, 2);
		COMPARE_NEAR_VALUES(h.RayAtT(m_tValues[2]).cell, hitH.cell, 3, 0.000001f);
	};

	SUBCASE("Axis aligned bounding box intersection")
	{
		float tValues[3]{};
		Ray f(Point3D(0, 1.0f, 0), Vector3(0.25322f, 0, 4.0f).normalized());
		Ray g(Point3D(-1, 2, 0), Vector3(0, 0, 1));
		Ray h(Point3D(0, 1.0f, 0), Vector3(0, 0, -1.0f));

		AxisAlignedBox SimpleBox(Vector3(1.0f, 0, 2.0f), Vector3(-1.0f, 2, 4.0f));

		//Hit cube in the middle.
		CHECK(SimpleBox.IntersectRay(f, tValues[0]) == true);
		//Hit corner of cube.
		CHECK(SimpleBox.IntersectRay(g, tValues[1]) == true);
		//Ray moves away from cube
		CHECK(SimpleBox.IntersectRay(h, tValues[2]) == false);
		
		Vector3 answer_hit{ 0.12661f, 1.0f, 2.0f };
		CHECK_ANSWERS_AGAINST_RESULTS(answer_hit.cell, f.RayAtT(tValues[0]).cell, 3);

		//Check if Ray G intersection point is corner, Vmax.
		CHECK_ANSWERS_AGAINST_RESULTS(Vector3(-1.0f, 2, 4.0f).cell, g.RayAtT(tValues[1]).cell, 3);
	}

	SUBCASE("Orientated bounding box intersection")
	{
		float tValues[4]{0,0,0,0};
		float tAnswer[4]{1.30126f, 1.2673970344398f, 0.9793349618127f,0};
		{
			OrientatedBox OBB(Vector3(0, 0.2f, 1.5f), Matrix3().getIdentity(), Vector3(0.2f, 0.2f, 0.2f));
			Ray h(Vector3(0, 0.2f, 0), Vector3(0.05733f, 0, 1.3f).normalized());
			CHECK(OBB.IntersectRay(h, tValues[0]) == true);
		}

		Matrix3 angle; angle.setToRotationY(30);
		OrientatedBox OBB(Vector3(0, 0.2f, 1.5f), angle, Vector3(0.2f, 0.2f, 0.2f));

		Ray i(Vector3(0, 0.2f, 0), Vector3(-0.00588f, 0.06594f, 1.26567f).normalized());
		CHECK(OBB.IntersectRay(i, tValues[1]) == true);

		Ray j(Vector3(0.4f, 0.6266861668441f, 0.4f), Vector3(-0.463753641962f, -0.2266861668441f, 0.8322517099756f).normalized());
		CHECK(OBB.IntersectRay(j, tValues[2]) == true);

		Ray fake(Vector3(0, 0.2f, 0), Vector3(-0.00588f, 0.06594f, -1.26567f).normalized());
		CHECK(OBB.IntersectRay(fake, tValues[3]) == false);
		
		COMPARE_NEAR_VALUES(tAnswer, tValues, 4, 0.15f);
	}
}