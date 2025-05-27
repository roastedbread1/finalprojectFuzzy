#include <iostream>
#include <cmath>

#define DELTA 0.01

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

static double gas = 0.0;
static double noop = 0.0;
static double rem = 0.0;

double integral(double xFrom, double xTo, double(*fn)(double)) {
	double sum = 0;
	for (double x = xFrom; x < xTo; x += DELTA) {
		sum += fn(x);
	}
	return sum;
}


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
	return fmin(a, b);
}

// Jarak terhadap sesuatu di depan mobil.
// Input: x dalam meter.
double membership_jarak_dekat(double x) {
	if (x <= LAMBAT_MIN) return 1.0;
	if (x >= LAMBAT_MAX) return 0.0;
	return (LAMBAT_MAX - x) / (LAMBAT_MAX - LAMBAT_MIN);
}

double membership_jarak_sedang(double x) {
	if (x <= SEDANG_MIN || x >= SEDANG_MAX) return 0.0;
	if (x <= SEDANG_PEAK) return (x - SEDANG_MIN) / (SEDANG_PEAK - SEDANG_MIN);
	return (SEDANG_MAX - x) / (SEDANG_MAX - SEDANG_PEAK);
}

double membership_jarak_jauh(double x) {
	if (x <= CEPAT_MIN) return 0.0;
	if (x >= CEPAT_MAX) return 1.0;
	return (x - CEPAT_MIN) / (CEPAT_MAX - CEPAT_MIN);
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


double membership_akselerasi_gas(double x)
{
	if (x <= LAMBAT_MIN) return 1.0;
	if (x >= LAMBAT_MAX) return 0.0;
	return (LAMBAT_MAX - x) / (LAMBAT_MAX - LAMBAT_MIN);
}
double membership_akselerasi_do_nothing(double x)
{
	if (x <= SEDANG_MIN || x >= SEDANG_MAX) return 0.0;
	if (x <= SEDANG_PEAK) return (x - SEDANG_MIN) / (SEDANG_PEAK - SEDANG_MIN);
	return (SEDANG_MAX - x) / (SEDANG_MAX - SEDANG_PEAK);
}

double membership_akselerasi_brake(double x)
{
	if (x <= CEPAT_MIN) return 0.0;
	if (x >= CEPAT_MAX) return 1.0;
	return (x - CEPAT_MIN) / (CEPAT_MAX - CEPAT_MIN);
}


// RULES:
/*
			DEKAT	SEDANG	JAUH
	LAMBAT	NOOP	GAS		GAS
	SEDANG	REM		NOOP	GAS
	CEPAT	REM		REM		NOOP
*/
double clipped_gas(double x)
{
	return fmin(gas, membership_akselerasi_gas(x));
}

double clipped_noop(double x)
{
	return fmin(noop, membership_akselerasi_do_nothing(x));
}
double clipped_rem(double x)
{
	return fmin(rem, membership_akselerasi_brake(x));
}

double combined_output(double x)
{
	return (fmax(fmax(clipped_gas(x), clipped_noop(x)), clipped_rem(x)));
}

double numerator_func(double x)
{
	return x * combined_output(x);
}
double evaluate(double jarak, double kecepatan) {

	double m_jarak_dekat = membership_jarak_dekat(jarak);
	double m_jarak_sedang = membership_jarak_sedang(jarak);
	double m_jarak_jauh = membership_jarak_jauh(jarak);

	double m_kecepatan_lambat = membership_kecepatan_lambat(kecepatan);
	double m_kecepatan_sedang = membership_kecepatan_sedang(kecepatan);
	double m_kecepatan_cepat = membership_kecepatan_cepat(kecepatan);

	// RULES:
	//IF lambat AND dekat THEN NOOP
	double rule1 = tnorm(m_kecepatan_lambat, m_jarak_dekat);

	//IF lambat AND sedang THEN gas
	double rule2 = tnorm(m_kecepatan_lambat, m_jarak_sedang);

	//IF lambat AND jauh THEN gas
	double rule3 = tnorm(m_kecepatan_lambat, m_jarak_jauh);

	//IF sedang AND dekat THEN rem
	double rule4 = tnorm(m_kecepatan_sedang, m_jarak_dekat);

	//IF sedang AND sedang THEN noop
	double rule5 = tnorm(m_kecepatan_sedang, m_jarak_sedang);

	//IF sedang AND jauh THEN gas
	double rule6 = tnorm(m_kecepatan_sedang, m_jarak_jauh);

	//IF cepat AND dekat THEN rem
	double rule7 = tnorm(m_kecepatan_cepat, m_jarak_dekat);

	//IF cepat AND sedang THEN rem
	double rule8 = tnorm(m_kecepatan_cepat, m_jarak_sedang);

	//IF cepat AND jauh THEN noop
	double rule9 = tnorm(m_kecepatan_cepat, m_jarak_jauh);

	//aggregate
	gas = snorm(snorm(rule2, rule3), rule6);
	noop = snorm(snorm(rule1, rule5), snorm(rule8, rule9));
	rem = snorm(rule4, rule7);

	double numerator = integral(0, 120, numerator_func);
	double denominator = integral(0, 120, combined_output);

	if (denominator == 0) return 60;
	// To do: defuzzifier centroid.
	

	return numerator/denominator;
}


int main()
{
	double jarak = 0;
	double kecepatan = 0;




	std::cout << "mobil";
}