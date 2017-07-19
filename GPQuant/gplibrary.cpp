#include "gplibrary.h"
#include <iostream>
#include <math.h>
using namespace std;

namespace GPQuant
{

	
	double target_func(double* row) {
		return 2*sin(row[0]) - row[1] + 3 * row[2];
	}

	double fitness_func(double y, double y_pred) {
		return abs(y - y_pred);
	}

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

	double* BackTesting::cheating(double* origin_x_data, int n_dim, int x_len = -1) {

		if (x_len < 0) {
			x_len = sizeof(origin_x_data) / sizeof(origin_x_data[0]);
		}

		if (x_len % n_dim != 0) {
			cout << "lengh of data and n_dim does not match!" << endl;
			exit(0);
		}

		double ** x_data = BackTesting::convert_1d_array_to_2d_array(origin_x_data, n_dim, x_len);

		int n_data = x_len / n_dim;
		double* res = new double[n_data];


		for (int i = 0; i < n_data; i++)
		{
			double* row = x_data[i];
			res[i] = target_func(row);
		}
		return res;
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

	double BackTesting::get_reward(int* indices, double* y_pred, int n_data, double* price_table, int n_dim, int x_len) {

		double * op = new double[n_data];
		op[0] = 0;
		int ctr = 1;
		while (ctr < n_data)
		{
			if (y_pred[ctr] * y_pred[ctr-1] < 0) {
				op[ctr] = y_pred[ctr] > 0 ? 1 : -1;
			}
			else {
				op[ctr] = 0;
			}
			ctr++;
		}
		
		double fund = 0;
		int sum = 0; // should be zero after all transcation otherwise we may substract the last invest
		double invest = 0;
		for (int i = 0; i < n_data; i++)
		{
			int idx = indices[i];
			sum += op[idx];
			
			invest = -1 * op[idx] * price_table[idx];
			fund += invest;
			if (x_len < 0) {
				cout << "idx: " << idx << ", op: " << op[idx] << ", y_pred: " << y_pred[idx] << ", price: " << price_table[idx] << ", invest: " << invest << ", fund: " << fund << endl;;
			}
		}

		if (sum != 0) {
			fund -= invest;
		}

		return 1 / (atan(fund) + 3.14);
	}

	double BackTesting::get_reward_with_x_old(int* indices, double* y_pred, int n_data, double* origin_x_data, int n_dim, int x_len = -1) {


		if (x_len < 0) {
			x_len = sizeof(origin_x_data) / sizeof(origin_x_data[0]);
		}

		if (x_len % n_dim != 0) {
			cout << "lengh of data and n_dim does not match!" << endl;
			exit(0);
		}


		double ** x_data = BackTesting::convert_1d_array_to_2d_array(origin_x_data, n_dim, x_len);
		double score = 0;
		double* row;



		for (int i = 0; i < n_data; i++)
		{

			int index = indices[i];
			row = x_data[index];
			double y_true = target_func(row);
			score += fitness_func(y_true, y_pred[i]);
		}


		for (int i = 0; i < x_len / n_dim; i++)
		{
			row = x_data[i];
			delete[] row;
		}

		delete[] x_data;
		

		return score / n_data;
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

