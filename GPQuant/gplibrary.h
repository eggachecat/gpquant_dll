#pragma once
#ifdef GPQUANTDLL_EXPORTS
#define GPQUANTDLL_API __declspec(dllexport) 
#else
#define GPQUANTDLL_API __declspec(dllimport) 
#endif
#include <string>
#include <iostream>
using namespace std;

namespace GPQuant
{

	
	class BackTesting
	{
	public:


		// For testing
		static GPQUANTDLL_API void display_data(double** x_data, int n_dim, int n_data);
		static GPQUANTDLL_API double** convert_1d_array_to_2d_array(double* origin, int n_dim, int n_data);
		static GPQUANTDLL_API double* cheating(double* origin_x_data, int n_dim, int x_len);

		// Memory
		static GPQUANTDLL_API void delete_double_pointer(double* d_pointer);
		static GPQUANTDLL_API void delete_int_pointer(int* i_pointer);

		// Returns reward given y_pred

		static GPQUANTDLL_API double get_reward(int* indices, double* y_pred, int n_data);
		static GPQUANTDLL_API double get_reward_with_x(int* indices, double* y_pred, int n_data, double* x_data, int n_dim, int x_len);
		static GPQUANTDLL_API double get_reward_with_x_old(int* indices, double* y_pred, int n_data, double* origin_x_data, int n_dim, int x_len);

	};
}