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

// Jarak terhadap sesuatu di depan mobil.
// Input: x dalam meter.
double membership_jarak_dekat(double x) {
	return 0;
}

double membership_jarak_sedang(double x) {
	return 0;
}

double membership_jarak_jauh(double x) {
	return 0;
}

double membership_jarak_jauuuuuh(double x) {
	return 0;
}

// Kecepatan mobil saat ini.
// Input: x dalam meter / detik.

double membership_kecepatan_berhenti(double x) {
	return 0;
}

double membership_kecepatan_lambat(double x) {
	return 0;
}

double membership_kecepatan_sedang(double x) {
	return 0;
}

double membership_kecepatan_cepat(double x) {
	return 0;
}







int main()
{
	std::cout << "mobil";
}