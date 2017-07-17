// Test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "../GPQuant/gplibrary.h"
#pragma comment(lib, "..\\x64\\Release\\GPQuant.lib")
#include <iostream>
#include <string>
using namespace std;

void test_convert_1d_array_to_2d_array() {
	double data[] = { 1.,2.,3.,4.,5.,6. };
	int len = 6;
	int n_dim = 2;
	int n_data = 6 / 2;
	double** res = GPQuant::BackTesting::convert_1d_array_to_2d_array(data, n_dim, len);
	GPQuant::BackTesting::display_data(res, n_dim, n_data);
}

void test_get_reward_with_x() {
	int n_data = 10000;
	int n_dim = 3;
	int x_len = n_data * n_dim;

	int* indices = new int[n_data];
	double* y_pred = new double[n_data];
	double* x_data = new double[x_len];

	for (int i = 0; i < n_data; i++)
	{
		indices[i] = i;
		y_pred[i] = 0.5;
	}

	for (int i = 0; i < x_len; i++)
	{
		x_data[i] = 0.5;
	}
	GPQuant::BackTesting::get_reward_with_x(indices, y_pred, n_data, x_data, n_dim, x_len);
	delete indices;
	delete y_pred;
	delete x_data;
}
int main()
{
	string s;
	while (true) {
		test_get_reward_with_x();
	}
	return 0;
}

