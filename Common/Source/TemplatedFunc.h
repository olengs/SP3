/***
@file: TemplatedFunc.h
@author Lim Jun Chen
@par email: Limjunchen37@gmail.com
@brief condensed helper functions
*/
#ifndef MY_ALGORITHM_H
#define MY_ALGORITHM_H
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <exception>
#include <ctime>
#include <cstdlib>

using namespace std;
namespace Algorithm {
	/***
	@brief SearchnSort namespace for general search and sort functions
	*/
	namespace SearchnSort {
		/***
		@brief find a T type obj in a static array
		@param in - obj to find
		@param foo[N} - static array of size N to find obj in
		@exception None
		@return bool whether obj in inside array
		*/
		template <typename T, size_t N>
		bool in(T& in, T(&foo)[N]) {
			for (int i = 0; i < N; ++i) {
				try {
					if (in == foo[i]) {
						return true;
					}
				}
				catch (const char* s) {
					cout << s;
					return false;
				}
			}
			return false;
		}

		/***
		@brief overloaded operator == for in function in case == is undefined
		@param None
		@exception None
		@return throws and error and returns false
		*/
		template <typename T>
		bool operator==(T LHS, T RHS) {

			throw("operator == does not exist for T == T");
			return false;
		}

		/***
		@brief find a T type obj in a std::vector object array
		@param in - obj to find
		@param list - std::vector typed array of size N to find obj in
		@exception None
		@return bool whether obj in inside array
		*/
		template <typename T>
		bool in(T in, vector<T>& list) {
			typename std::vector<T>::iterator it;
			for (it = list.begin(); it != list.end(); ++it) {
				T temp = (T)*it;
				try {
					if (in == temp) {
						return true;
					}
				}
				catch (const char* s) {
					cout << s;
					return false;
				}
			}
			return false;
		}

		/***
		@brief Swap the first and second value
		@param first - value 0 to be swapped
		@param second - value 1 to be swapped
		@exception None
		@return None
		*/
		template<class T>
		void Swap(T& first, T& second) {
			T temp = first;
			first = second;
			second = temp;
		}

		/***
		@brief find partition position for static array quicksort
		@param arr - static array to sort
		@param start - first index to sort
		@param end - last index to sort
		@exceptions None
		@return int partition index
		*/
		template <class T>
		int partitions(T arr[], int start, int end)
		{
			T pivotValue = arr[start];
			int pivotPosition = start;
			for (int pos = start + 1; pos <= end; pos++)
			{
				if (arr[pos] < pivotValue)
				{
					swap(arr[pivotPosition + 1], arr[pos]);
					swap(arr[pivotPosition], arr[pivotPosition + 1]);
					pivotPosition++;
				}
			}
			return pivotPosition;
		}

		/***
		@brief quicksort a static array, ( class T must have < overloaded )
		@param arr - static array to sort
		@param start - first index to sort
		@param end - last index to sort
		@exceptions None
		@return sorted static array
		*/
		template<class T>
		T* quickSort(T arr[], int start, int end)
		{
			if (start < end)        //test for base case start == end
			{
				// Partition the array and get the pivot point.
				int p = partitions(arr, start, end);

				// Sort the portion before the pivot point.
				quickSort(arr, start, p - 1);

				// Sort the portion after the pivot point.
				quickSort(arr, p + 1, end);
			}
			return arr;                 //base case
		}

		/***
		@brief find partition position for std::vector array quicksort
		@param arr - std::vector array to sort
		@param start - first index to sort
		@param end - last index to sort
		@exceptions None
		@return int partition index
		*/
		template <class T>
		int partitions(vector<T>& arr, int start, int end)
		{
			T pivotValue = arr[start];
			int pivotPosition = start;
			for (int pos = start + 1; pos <= end; pos++)
			{
				if (arr[pos] < pivotValue)
				{
					swap(arr[pivotPosition + 1], arr[pos]);
					swap(arr[pivotPosition], arr[pivotPosition + 1]);
					pivotPosition++;
				}
			}
			return pivotPosition;
		}

		/***
		@brief quicksort a std::vector array, ( class T must have < overloaded )
		@param arr - std::vector array to sort
		@param start - first index to sort
		@param end - last index to sort
		@exceptions None
		@return sorted std::vector array
		*/
		template<class T>
		vector<T>& quickSort(vector<T>& arr, int start, int end)
		{
			if (start < end)
			{
				int p = partitions(arr, start, end);
				quickSort(arr, start, p - 1);
				quickSort(arr, p + 1, end);
			}
			return arr;
		}

		/***
		@brief find partition position for std::vector array quicksort with functor(function pointer)
		@param arr - std::vector array to sort
		@param start - first index to sort
		@param end - last index to sort
		@param functor - class with overloaded () function that returns a bool - acts as a comparitor
		@exceptions None
		@return int partition index
		*/
		template <class T, class S>
		int partitions(vector<T>& arr, int start, int end, S functor)
		{
			T pivotValue = arr[start];
			int pivotPosition = start;
			for (int pos = start + 1; pos <= end; pos++)
			{
				if (functor(arr[pos], pivotValue))
				{
					Swap(arr[pivotPosition + 1], arr[pos]);
					Swap(arr[pivotPosition], arr[pivotPosition + 1]);
					pivotPosition++;
				}
			}
			return pivotPosition;
		}

		/***
		@brief quicksort a std::vector array using a functor(function pointer class) compartior
		@param arr - std::vector array to sort
		@param start - first index to sort
		@param end - last index to sort
		@param functor - class with overloaded () function that returns a bool - acts as a comparitor
		@exceptions None
		@return sorted std::vector array
		*/
		template<class T, class S>
		vector<T>& quickSort(vector<T>& arr, int start, int end, S functor)
		{
			if (start < end)
			{
				int p = partitions(arr, start, end, functor);
				quickSort(arr, start, p - 1);
				quickSort(arr, p + 1, end);
			}
			return arr;
		}

	}//end of namespace SearchnSort
	/***
	@brief Math namespace for general math functions
	*/
	namespace Math {
		const float	EPSILON    = 0.00001f;  ///Used for error checking
		const float	PI         = 3.1415926535897932384626433832795f;
		const float TWO_PI     = PI * 2.0f;
		const float HALF_PI    = PI * 0.5f;
		const float QUARTER_PI = PI * 0.25f;
		const float natural_e  = 2.7182818284590452353602874713527f;

		/***
		@brief negate value if negative
		@param val - val to be absolute(d)
		@exception None
		@returns absolute of value as float
		*/
		inline float Fabs(float val) {
			return val < 0 ? -val : val;
		}//end of Fabs function

		/***
		@brief negate value if negative
		@param val - val to be absolute(d)
		@exception None
		@returns absolute of value as double
		*/
		inline double Abs(double val) {
			return val < 0 ? -val : val;
		}//end of Abs function
		
		/***
		@brief converts degree to radians
		@param deg - angle in degree
		@exception None
		@returns angle in radians
		*/
		inline float DegreetoRadians(float deg) {
			return deg * PI / 180.0f;
		}//end of DegreetoRadians function
		
		/***
		@brief converts radians to degree
		@param rad - angle in radians
		@exception None
		@returns angle in degrees
		*/
		inline float RadianstoDegree(float rad) {
			return rad * 180.0f / PI;
		}//end of RadianstoDegree function
		
		/***
		@brief 	Keeps a value between a min and max point.  If it goes to low, it will become the max.
		If the value gets too high, it will become the min.
		@param val - the value to Wrap.
		@param minVal - the minimum value allowed.
		@param maxVal - the maximum value allowed.
		@exception None
		@return The wrapped value.
		*/
		template<typename T>
		inline T Warp(T val, T min, T max) {
			if (val < min) {
				return max;
			}
			if (val > max) {
				return min;
			}
			return val;
		} // end of Warp function
		
		/***
		@brief loops around a value between a min and max point.  
		If it goes to low, it will become a loop and become val + max.
		If the value gets too high, it will become val - max.

		@param val - the value to Wrap.
		@param minVal - the minimum value allowed.
		@param maxVal - the maximum value allowed.
		@exception None
		@return The wrapped value.
		*/
		template<typename T>
		inline T PWrap(T val, T min, T max) {
			if (val < min) {
				return PWrap(val + max, min, max);
			}
			if (val > max) {
				return PWrap(val - max, min, max);
			}
			return val;
		}//end of PWrap function

		/***
		@brief Keeps a value between a min and max point.  If it goes to low, it will become the min.
		If the value gets too high, it will become the max.
		@param val - the value to clamp.
		@param minVal - the minimum value allowed.
		@param maxVal - the maximum value allowed.
		@exception None
		@return The clampped value.
		*/
		template<typename T>
		inline T Clamp(T val, T minVal, T maxVal)
		{
			if (val < minVal)
			{
				return minVal;
			}
			else if (val > maxVal)
			{
				return maxVal;
			}

			return val;
		}//end of Clamp function

		/***
		@brief Returns the smaller of the two values
		@param x the first value to check
		@param y the second value to check
		@exception None
		@return The smaller of the two values
		*/
		template<typename T>
		inline T Min(T x, T y)
		{
			return (x < y) ? x : y;
		}//end of Min function

		/***
		@brief Returns the larger of the two values
		@param x the first value to check
		@param y the second value to check
		@exception None
		@return The larger of the two values
		*/
		template<typename T>
		inline T Max(T x, T y)
		{
			return (x > y) ? x : y;
		}//end of Max function

		/***
		@brief Returns the square of a value
		@param x the value
		@exception None
		@return The square of the value
		*/
		template<typename T>
		inline T Square(T x)
		{
			return x * x;
		}//end of Square function

		/***
		@brief Initialize Random Number Generator
		@param None
		@exception None
		@return None
		*/
		inline void InitRNG(void)
		{
			srand(static_cast<unsigned> (time(0)));
		}//end of InitRNG function

		/***
		@brief Return a random integer
		@param None
		@exception None
		@return Random integer
		*/
		inline unsigned RandInt(void)
		{
			return rand();
		}//end of RandInt function

		/***
		@brief Return a random integer between min and max
		@param min - minimum range
		@param max - maximum range
		@exception None
		@return Random integer
		*/
		inline int RandIntMinMax(int min, int max)
		{
			int num = rand() % (max - min + 1);
			return (num + min);
		}//end of RandIntMinMax function

		/***
		@brief Return a random float
		@param None
		@exception None
		@return Random float
		*/
		inline float RandFloat(void)
		{
			return rand() / static_cast <float> (RAND_MAX);
		}//end of RandFloat function

		/***
		@brief Return a random float between min and max
		@param min - minimum range
		@param max - maximum range
		@exception None
		@return Random float
		*/
		inline float RandFloatMinMax(float min, float max)
		{
			float num = (max - min) * RandFloat();
			return (num + min);
		}//end of RandFloatMinMax function

		/***
		@brief compresses a number in domain (-inf,inf) to range [0,1]
		@param val - value to be compressed
		@exception None
		@return sigmoid(val)
		*/
		inline float Sigmoid(float val) {
			return 1 / (1 + powf(natural_e, -val));
		}//end of sigmoid function
		/***
		@brief brief compresses a number in domain (-inf,inf) to range [0,1] and then compressed again to [0,0.25]
		@param val - value to be compressed
		@exception None
		@return d/dx(sigmoid(val))
		*/
		inline float DerivedSigmoid(float val) {
			return Sigmoid(val) * (1 - Sigmoid(val));
		}//end of Derivedsigmoid function
		/***
		@brief extracts a number from [0,1] to (-inf,inf) that has been compressed by sigmoid
		@param val - value to be extracted
		@exception None
		@return extracted val value
		*/
		inline float logits(float val) {
			return log(val / (1 - val));
		}//end of logits function
		
		/***
		@brief gets result of n! or n-factorial
		@param n - number to be factorial-ed
		@exception factorial is undefined. Throw when n is negative
		@return factorial-ed value
		*/
		inline int factorial(int n) {
			if (n < 0) {
				throw "factorial is undefined";
			}
			if (n == 0) {
				return 1;
			}
			return n * factorial(n - 1);
		}
		/***
		@brief the number of ways n objects can be arranged in r ways.
		@param n - number of objects
		@param r - number of types of arrangements
		@exception None
		@return number of permutations
		*/
		inline int nPr(int n, int r) {
			return factorial(n) / factorial(n - r);
		}
		/***
		@brief the number of ways n objects can be selected in r ways 
		@param n - number of objects
		@param r - number of types of arrangements
		@exception None
		@return number of combinations
		*/
		inline int nCr(int n, int r) {
			return nPr(n, r) / factorial(r);
		}
		inline int nTermAP(int a, int d, int n) {
			return a + (n - 1) * d;
		}
		inline int summationAP(int a, int d, int n) {
			return (n / 2) * (2 * a + (n - 1) * d);
		}
		inline int nTermGP(int a, int r, int n) {

		}
		inline int summationGP(int a, int r, int n) {

		}


	}//end of namespace Algorithm::Math
}

#endif