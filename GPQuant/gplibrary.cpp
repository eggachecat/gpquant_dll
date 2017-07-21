#include "gplibrary.h"
#include <iostream>
#include <math.h>
using namespace std;

namespace GPQuant
{

	void BackTesting::display_data(double ** x_data, int n_dim, int n_data = -1) {
		if (n_data < 0) {
			n_data = sizeof(x_data) / sizeof(x_data[0]);
		}

		for (int i = 0; i < n_data; i++)
		{
			double* row = x_data[i];
			cout << "#" << i << ": ";
			for (int j = 0; j < n_dim; j++)
			{
				cout << x_data[i][j] << ", ";
			}
			cout << endl;
		}
	}

	double** BackTesting::convert_1d_array_to_2d_array(double* origin, int n_dim, int len = -1) {
		if (len < 0) {
			len = sizeof(origin) / sizeof(origin[0]);
		}

		if (len % n_dim != 0) {
			cout << "lengh of data and n_dim does not match!" << endl;
			exit(0);
		}

		int n_data = len / n_dim;
		double** result = new double*[n_data];

		for (int i = 0; i < n_data; i++)
		{
			result[i] = new double[n_dim];
			for (int j = 0; j < n_dim; j++)
			{
				result[i][j] = origin[i * n_dim + j];
			}

		}

		return result;
	}

	void BackTesting::delete_double_pointer(double* d_pointer) {
		cout << "dll start deleting double pointer" << endl;
		cout << d_pointer;
		delete[] d_pointer;
		cout << "dll deleted double pointer" << endl;
	}

	void BackTesting::delete_int_pointer(int* i_pointer) {
		cout << "dll start deleting int pointer" << endl;
		delete[] i_pointer;
		cout << "dll deleted double i_pointer" << endl;

	}

	double* BackTesting::BarStrategy::get_op_arr(double * y_pred, int n_data) {

		double * op = new double[n_data];

		op[0] = 0;
		int ctr = 1;
		while (ctr < n_data)
		{
			if (y_pred[ctr] * y_pred[ctr - 1] < 0) {
				op[ctr] = y_pred[ctr] > 0 ? 1 : -1;
			}
			else {
				op[ctr] = 0;
			}
			ctr++;
		}

		return op;
	}

	double BackTesting::BarStrategy::get_reward(int* indices, double* y_pred, int n_data, double* price_table, int n_dim, int x_len) {
		// should not call get_info for performance issue

		double * op = get_op_arr(y_pred, n_data);

		double fund = 0;
		double invest = 0;
		double pre_invest = 0;
		double profit = 0;

		int transcation_ctr = 0; // the first proft doesnt count


		for (int i = 0; i < n_data; i++)
		{
			int idx = indices[i];

			if (op[idx] != 0) {
				invest = price_table[idx];

				profit = -1 * op[idx] * (invest - pre_invest) * int(transcation_ctr > 0);
				pre_invest = invest;
				fund += profit;

				transcation_ctr += 1;
			}

			if (x_len < 0) {
				cout << "idx: " << idx << ", op: " << op[idx] << ", y_pred: " << y_pred[idx] << ", price: " << price_table[idx] << ", invest: " << invest << ", fund: " << fund << endl;;
			}
		}

		delete[] op;

		return fund;
	}

	strategy_info* BackTesting::BarStrategy::get_info(int* indices, double* y_pred, int n_data, double* price_table, int n_dim, int x_len) {
		double * op_arr = get_op_arr(y_pred, n_data);
		return get_info_by_op(indices, op_arr, n_data, price_table, n_data, x_len);
	}

	strategy_info* BackTesting::BarStrategy::get_info_by_op(int* indices, double* op_arr, int n_data, double* price_table, int n_dim, int x_len) {

		double* profit_arr = new double[n_data];
		double invest = 0;
		double pre_invest = 0;
		double profit = 0;

		int transcation_ctr = 0; // the first proft doesnt count


		for (int i = 0; i < n_data; i++)
		{
			int idx = indices[i];
			profit = 0;

			if (op_arr[idx] != 0) {

				invest = price_table[idx];
				profit = -1 * op_arr[idx] * (invest - pre_invest) * int(transcation_ctr > 0);

				pre_invest = invest;
				transcation_ctr += 1;
			}
			profit_arr[i] = profit;
		}

		int ctr = 1;

		double previous_op = 0;
		while (ctr < n_data)
		{
			if (op_arr[ctr] != 0) {
				previous_op = op_arr[ctr];
			}
			else {
				op_arr[ctr] = previous_op;
			}
			ctr++;
		}


		strategy_info* info = new strategy_info;
		info->profit_arr = profit_arr;
		info->op_arr = op_arr;

		return info;
	}


	/*
		Functions below are used to test communciation between dll and python.
	*/
	
	double target_func(double* row) {
		return 2*sin(row[0]) - row[1] + 3 * row[2];
	}

	double fitness_func(double y, double y_pred) {
		return abs(y - y_pred);
	}

	
	double BackTesting::get_reward_with_x(int* indices, double* y_pred, int n_data, double* x_data, int n_dim, int x_len) {


		if (x_len % n_dim != 0) {
			cout << "lengh of data and n_dim does not match!" << endl;
			exit(0);
		}

		double score = 0;
		double* row;
	
		for (int i = 0; i < n_data; i++)
		{
			int index = n_dim * indices[i];
			row = &x_data[index];
			double y_true = target_func(row);
			score += fitness_func(y_true, y_pred[i]);
		}
		return score / n_data;
	}

}

//double BackTesting::BarStrategy::get_reward(int* indices, double* y_pred, int n_data, double* price_table, int n_dim, int x_len) {
//
//
//	double * op = new double[n_data];
//	op[0] = 0;
//	int ctr = 1;
//	while (ctr < n_data)
//	{
//		if (y_pred[ctr] * y_pred[ctr - 1] < 0) {
//			op[ctr] = y_pred[ctr] > 0 ? 1 : -1;
//		}
//		else {
//			op[ctr] = 0;
//		}
//		ctr++;
//	}
//
//	double fund = 0;
//	double invest = 0;
//	double pre_invest = 0;
//	double profit = 0;
//
//	int transcation_ctr = 0; // the first proft doesnt count
//
//
//	for (int i = 0; i < n_data; i++)
//	{
//		int idx = indices[i];
//
//		if (op[idx] != 0) {
//			invest = price_table[idx];
//
//			profit = -1 * op[idx] * (invest - pre_invest) * int(transcation_ctr > 0);
//			pre_invest = invest;
//			fund += profit;
//
//			transcation_ctr += 1;
//		}
//
//		if (x_len < 0) {
//			cout << "idx: " << idx << ", op: " << op[idx] << ", y_pred: " << y_pred[idx] << ", price: " << price_table[idx] << ", invest: " << invest << ", fund: " << fund << endl;;
//		}
//	}
//	if (x_len < 0) {
//		cout << "Profit in reality: " << fund * 200 << endl;
//		cout << "Num of transcation: " << transcation_ctr << endl;
//	}
//
//	delete[] op;
//
//
//
//	return fund;
//}
//
//strategy_info* BackTesting::BarStrategy::get_info(int* indices, double* y_pred, int n_data, double* price_table, int n_dim, int x_len) {
//
//	double * op_arr = new double[n_data];
//	op_arr[0] = 0;
//	int ctr = 1;
//	while (ctr < n_data)
//	{
//
//		if (y_pred[ctr] * y_pred[ctr - 1] < 0) {
//			op_arr[ctr] = y_pred[ctr] > 0 ? 1 : -1;
//		}
//		else {
//			op_arr[ctr] = 0;
//		}
//		cout << op_arr[ctr] << endl;
//		ctr++;
//
//	}
//
//	double* profit_arr = new double[n_data];
//	double invest = 0;
//	double pre_invest = 0;
//	double profit = 0;
//
//	int transcation_ctr = 0; // the first proft doesnt count
//
//
//	for (int i = 0; i < n_data; i++)
//	{
//		int idx = indices[i];
//		profit = 0;
//
//		if (op_arr[idx] != 0) {
//
//			invest = price_table[idx];
//
//			profit = -1 * op_arr[idx] * (invest - pre_invest) * int(transcation_ctr > 0);
//
//
//			if (x_len < 0) {
//				cout << "idx: " << idx << ", op: " << op_arr[idx] << ", y_pred: " << y_pred[idx] << ", pre_invest: " << pre_invest << ", invest: " << invest << ", fund: " << profit << endl;;
//			}
//
//			pre_invest = invest;
//			transcation_ctr += 1;
//		}
//		profit_arr[i] = profit;
//	}
//
//	ctr = 1;
//
//	double previous_op = 0;
//	while (ctr < n_data)
//	{
//		if (op_arr[ctr] != 0) {
//			previous_op = op_arr[ctr];
//		}
//		else {
//			op_arr[ctr] = previous_op;
//		}
//		ctr++;
//	}
//
//
//	strategy_info* info = new strategy_info;
//	info->profit_arr = profit_arr;
//	info->op_arr = op_arr;
//
//	return info;
//}
