#include "gplibrary.h"
#include <iostream>
using namespace std;
//inline double abs(double x) { return x > 0 ? x : -1 * x; }
namespace GPQuant
{

	double target_func(double* row) {
		return row[0] * row[1] + row[2] * row[2] + row[2];
	}

	double fitness_func(double y, double y_pred) {
		return abs(y - y_pred);
	}

	void BackTesting::display_data(double ** x_data, int n_data, int n_dim) {
		for (int i = 0; i < n_data; i++)
		{
			double * row = x_dara[];
		}
	}

	double BackTesting::get_reward(int* indices, double* y_pred, int n_data) {
		return 0.0;
	}

	double BackTesting::get_reward_with_x(int* indices, double* y_pred, int n_data, double** x_data, int n_dim) {

		double score = 0;
		for (int i = 0; i < n_data; i++)
		{
			double* row = x_data[i];
			double y = target_func(row);
			score += fitness_func(y, y_pred[i]);
		}

		return score;
	}
	
}

