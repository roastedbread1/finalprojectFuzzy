#include <iostream>
#include <cmath>

double tnorm(double a, double b);
double snorm(double a, double b);
double complement(double a);
double implication(double a, double b);

double tnorm(double a, double b) {
	return fmin(a, b);
}

double snorm(double a, double b) {
	return fmax(a, b);
}

double complement(double a) {
	return 1 - a;
}



int main()
{
	std::cout << "mobil";
}