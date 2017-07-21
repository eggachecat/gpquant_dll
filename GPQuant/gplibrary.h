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


	struct strategy_info {
		double* profit_arr;
		double* op_arr;
	} ;


	class BackTesting
	{
	public:

		// For debug
		static GPQUANTDLL_API void display_data(double** x_data, int n_dim, int n_data);
		static GPQUANTDLL_API double** convert_1d_array_to_2d_array(double* origin, int n_dim, int n_data);

		// For memory manipulation
		static GPQUANTDLL_API void delete_double_pointer(double* d_pointer);
		static GPQUANTDLL_API void delete_int_pointer(int* i_pointer);


		// Service functions

		class BarStrategy 
		{
		public:
			static GPQUANTDLL_API double* get_op_arr(double * y_pred, int n_data);

			static GPQUANTDLL_API strategy_info* get_info_by_op(int* indices, double* op_arr, int n_data, double* price_table, int n_dim, int x_len);

			static GPQUANTDLL_API strategy_info* get_info(int* indices, double* y_pred, int n_data, double* price_table, int n_dim, int x_len);
			static GPQUANTDLL_API double get_reward(int* indices, double* y_pred, int n_data, double* price_table, int n_dim, int x_len);
		};
	

		// Dll and python communciation test
		static GPQUANTDLL_API double get_reward_with_x(int* indices, double* y_pred, int n_data, double* x_data, int n_dim, int x_len);
	};
}