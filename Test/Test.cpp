// Test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "../GPQuant/gplibrary.h"
#pragma comment(lib, "..\\x64\\Release\\GPQuant.lib")
#include <iostream>
using namespace std;

void test_convert_1d_array_to_2d_array() {
	double data[] = { 1.,2.,3.,4.,5.,6. };
	int len = 6;
	int n_dim = 2;
	int n_data = 6 / 2;
	double** res = GPQuant::BackTesting::convert_1d_array_to_2d_array(data, n_dim, len);
	GPQuant::BackTesting::display_data(res, n_dim, n_data);
}
int main()
{
	test_convert_1d_array_to_2d_array();
	cout << "Hello world" << endl;
    return 0;
}

