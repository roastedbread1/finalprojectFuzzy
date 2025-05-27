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

template <typename T>
T max_variadic(T value) {
	return value;
}

template <typename T, typename... Args>
T max_variadic(T first, Args... rest) {
	T max_rest = max_variadic(rest...);
	return (first > max_rest) ? first : max_rest;
}

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

double mid_membershp_akselerasi_gas = (LAMBAT_MIN + LAMBAT_MAX) / 2;
double membership_akselerasi_gas(double x)
{
	if (x <= LAMBAT_MIN) return 1.0;
	if (x >= LAMBAT_MAX) return 0.0;
	return (LAMBAT_MAX - x) / (LAMBAT_MAX - LAMBAT_MIN);
}

double mid_membershp_akselerasi_noop = (SEDANG_MIN + SEDANG_MAX) / 2;
double membership_akselerasi_noop(double x)
{
	if (x <= SEDANG_MIN || x >= SEDANG_MAX) return 0.0;
	if (x <= SEDANG_PEAK) return (x - SEDANG_MIN) / (SEDANG_PEAK - SEDANG_MIN);
	return (SEDANG_MAX - x) / (SEDANG_MAX - SEDANG_PEAK);
}

double mid_membershp_akselerasi_brake = (CEPAT_MIN + CEPAT_MAX) / 2;
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
double evaluate(double jarak, double kecepatan) {

	// Fuzzify
	double m_jarak_dekat = membership_jarak_dekat(jarak);
	double m_jarak_sedang = membership_jarak_sedang(jarak);
	double m_jarak_jauh = membership_jarak_jauh(jarak);

	double m_kecepatan_lambat = membership_kecepatan_lambat(kecepatan);
	double m_kecepatan_sedang = membership_kecepatan_sedang(kecepatan);
	double m_kecepatan_cepat = membership_kecepatan_cepat(kecepatan);

	// RULES:
	//(lambat & dekat) -> noop
	double c1 = tnorm(m_kecepatan_lambat, m_jarak_dekat);

	//(lambat & sedang) -> gas
	double c2 = tnorm(m_kecepatan_lambat, m_jarak_sedang);

	//(lambat & jauh) -> gas
	double c3 = tnorm(m_kecepatan_lambat, m_jarak_jauh);

	//(sedang & dekat) -> rem
	double c4 = tnorm(m_kecepatan_sedang, m_jarak_dekat);

	//(sedang & sedang) -> noop
	double c5 = tnorm(m_kecepatan_sedang, m_jarak_sedang);

	//(sedang & jauh) -> gas
	double c6 = tnorm(m_kecepatan_sedang, m_jarak_jauh);

	//(cepat & dekat) -> rem
	double c7 = tnorm(m_kecepatan_cepat, m_jarak_dekat);

	//(cepat & sedang) -> rem
	double c8 = tnorm(m_kecepatan_cepat, m_jarak_sedang);

	//(cepat & jauh) -> noop
	double c9 = tnorm(m_kecepatan_cepat, m_jarak_jauh);

	std::cout
		<< c1 << ", "
		<< c2 << ", "
		<< c3 << ", "
		<< c4 << ", "
		<< c5 << ", "
		<< c6 << ", "
		<< c7 << ", "
		<< c8 << ", "
		<< c9 << "\n";

	// defuzzification: center of average
	double num =
		(c1 + c5 + c9) * mid_membershp_akselerasi_noop +
		(c2 + c3 + c6) * mid_membershp_akselerasi_gas +
		(c4 + c7 + c8) * mid_membershp_akselerasi_brake;

	// karena setiap membership function selalu ada yang nilainya 1, maka jika dikali Q hasilnya adalah Q
	double den = c1 + c2 + c3 + c4 + c5 + c6 + c7 + c8 + c9;

	return den != 0 ? num / den : 0;
}


int main()
{
	double jarak = 42;
	double kecepatan = 63;

	double akselerasi = evaluate(jarak, kecepatan);


	std::cout << "mobil gas " << akselerasi;
}