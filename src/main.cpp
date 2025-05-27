#include <iostream>
#include <cmath>

#define STOP_SPEED  0.0
#define LAMBAT_MIN  40.0
#define LAMBAT_MAX  60.0
#define SEDANG_MIN  40.0
#define SEDANG_PEAK  60.0
#define SEDANG_MAX  80.0
#define CEPAT_MIN 80.0
#define CEPAT_MAX 120.0

double tnorm(double a, double b);
double snorm(double a, double b);
double complement(double a);
double implication(double a, double b);
double zadeh(double a, double b);
double dienes_rescher(double a, double b);
double lukasiewicz(double a, double b);
double godel(double a, double b);
double mamdani_min(double a, double b);
double mamdani_product(double a, double b);

double tnorm(double a, double b) {
	return fmin(a, b);
}

double snorm(double a, double b) {
	return fmax(a, b);
}

double complement(double a) {
	return 1 - a;
}

double implication(double a, double b)
{
	return 0.0;
}

double zadeh(double a, double b)
{
	return fmax(fmin(a, b), complement(b));
}

double dienes_rescher(double a, double b)
{
	return fmax(complement(a), b);
}

double lukasiewicz(double a, double b)
{
	return fmin(1, 1-(a+b));
}

double godel(double a, double b)
{
	if (a < b) return 1;
	else return b;
}

double mamdani_min(double a, double b)
{
	return fmin(a, b);
}

double mamdani_product(double a, double b)
{
	return a*b;
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
	return (x == STOP_SPEED) ? 1.0 : 0.0;
}

double membership_kecepatan_lambat(double x) {
	if (x <= LAMBAT_MIN) return 1.0;
	if (x >= LAMBAT_MAX) return 0.0;
	return (LAMBAT_MAX - x) / (LAMBAT_MAX - LAMBAT_MIN);
}

double membership_kecepatan_sedang(double x) {
	if (x <= SEDANG_MIN || x>=SEDANG_MAX) return 0.0;
	if (x <= SEDANG_PEAK) return (x - SEDANG_MIN) / (SEDANG_PEAK - SEDANG_MIN);
	return (SEDANG_MAX - x) / (SEDANG_MAX - SEDANG_PEAK);
}

double membership_kecepatan_cepat(double x) {
	if (x <= CEPAT_MIN) return 0.0;
	if (x >= CEPAT_MAX) return 1.0;
	return (x - CEPAT_MIN) / (CEPAT_MAX - CEPAT_MIN);
}

int main()
{
	std::cout << "mobil";
}