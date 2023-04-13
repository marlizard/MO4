#include <iostream>
#include <iomanip>
#include <vector>
#include <math.h>
#include <random>

using namespace std;

typedef double real;

//Номер используемого метода
#define method_number 4

//Массивы коэффициентов
vector <real> C = { 2, 4, 2, 6, 2, 3 };
vector <real> a = { -3, -6, 2, 6, -3, 8};
vector <real> b = { 6, -8, -8, 8, -4, -1 };

//Целевая функция
real f(vector<real> x) {
	real sum = 0;

	for (int i = 0; i < 6; i++)
		sum += C[i] / (1 + pow(x.front() - a[i], 2) + pow(x.back() - b[i], 2));

	return sum;
}

//Получение случайного числа из области
real random_number(real min, real max, int precision)
{
	real value;

	// получить случайное число как целое число с порядком precision
	value = rand() % (int)pow(10, precision);

	// получить вещественное число
	value = min + (value / pow(10, precision)) * (max - min);

	return value;
}

//Простой случайный ненаправленный поиск
void simple_random_search(real e, real P) {
	real x0 = -10, xe = 10;
	real y0 = -10, ye = 10;

	int used_f = 0;

	real Pe = (e * e) / ((xe - x0) * (ye - y0));

	real n = ceil(log(1 - P) / log(1 - Pe));

	vector<real> x1(2, 0), x2(2, 0);
	real f1, f2;

	x1.front() = random_number(x0, xe, 4);
	x1.back() = random_number(y0, ye, 4);
	x2.front() = random_number(x0, xe, 4);
	x2.back() = random_number(y0, ye, 4);
	f1 = f(x1);
	f2 = f(x2);
	used_f += 2;

	if (f1 < f2) {
		f1 = f2;
		x1.front() = x2.front();
		x1.back() = x2.back();
	}

	for (int i = 0; i < n; i++) {
		x2.front() = random_number(x0, xe, 4);
		x2.back() = random_number(y0, ye, 4);
		f2 = f(x2);
		used_f++;
		if (f1 < f2) {
			f1 = f2;
			x1.front() = x2.front();
			x1.back() = x2.back();
		}
	}
	cout << n << " " << used_f << " " << fixed << setprecision(4) << x1.front() << " " << x1.back() << " " << f1 << endl;
}

//Алгоритм наилучшей пробы с направляющим гиперквадратом
void hypersquare_search_method(vector<real>& x, real& fmax, int& used_f) {
	real x0 = -10, xe = 10;
	real y0 = -10, ye = 10;

	vector<real> y(2, 0);
	int m = 1000, maxiter = 1000, n = 0;
	real e = 10e-6, alpha = 2;
	real a1, a2, b1, b2, prev_fmax = 0, tmp;

	fmax = f(x);
	used_f++;

	if (x.front() - 5 < -10) a1 = -10;
	else a1 = x.front() - 5;

	if (x.back() - 5 < -10) a2 = -10;
	else a2 = x.back() - 5;

	if (x.front() + 5 > 10) b1 = 10;
	else b1 = x.front() + 5;

	if (x.back() + 5 > 10) b2 = 10;
	else b2 = x.back() + 5;

	do {
		n++;

		for (int i = 0; i < m; i++) {
			y.front() = random_number(a1, b1, 4);
			y.back() = random_number(a2, b2, 4);

			tmp = f(y);
			used_f++;

			if (fmax < tmp) {
				prev_fmax = fmax;
				fmax = tmp;
				x.front() = y.front();
				x.back() = y.back();
			}
		}

		real a1new, b1new, a2new, b2new;

		if (x.front() - (b1 - a1) / (2 * alpha) < -10) a1new = -10;
		else a1new = x.front() - (b1 - a1) / (2 * alpha);

		if (x.back() - (b2 - a2) / (2 * alpha) < -10) a2new = -10;
		else a2new = x.back() - (b2 - a2) / (2 * alpha);

		if (x.front() + (b1 - a1) / (2 * alpha) > 10) b1new = 10;
		else b1new = x.front() + (b1 - a1) / (2 * alpha);

		if (x.back() + (b2 - a2) / (2 * alpha) > 10) b2new = 10;
		else b2new = x.back() + (b2 - a2) / (2 * alpha);

		a1 = a1new;
		b1 = b1new;
		a2 = a2new;
		b2 = b2new;

	} while (abs(fmax - prev_fmax) > e && n < maxiter);
}

//Первый алгоритм
void first_algorithm(int m) {
	real x0 = -10, xe = 10;
	real y0 = -10, ye = 10;
	vector<real> x(2, 0), y(2, 0);
	real fmax, ftmp;
	int counter = 0, used_f = 0;

	x.front() = random_number(x0, xe, 4);
	x.back() = random_number(y0, ye, 4);

	hypersquare_search_method(x, fmax, used_f);

	while (counter < m) {
		y.front() = random_number(x0, xe, 4);
		y.back() = random_number(y0, ye, 4);

		hypersquare_search_method(y, ftmp, used_f);

		if (ftmp > fmax) {
			fmax = ftmp;
			x.front() = y.front();
			x.back() = y.back();
		}
		else counter++;
	}

	cout << used_f << " " << fixed << setprecision(4) << x.front() << " " << x.back() << " " << fmax << endl;
}

//Второй алгоритм
void second_algorithm(int m) {
	real x0 = -10, xe = 10;
	real y0 = -10, ye = 10;
	vector<real> x(2, 0), y(2, 0);
	real fmax, ftmp;
	int counter = 0, used_f = 0;

	bool loop = true;

	x.front() = random_number(x0, xe, 4);
	x.back() = random_number(y0, ye, 4);

	hypersquare_search_method(x, fmax, used_f);

	while (loop) {

		do {
			y.front() = random_number(x0, xe, 4);
			y.back() = random_number(y0, ye, 4);
			counter++;

			ftmp = f(y);
			used_f++;
		} while (fmax > ftmp && counter < m);

		if (counter == m) {
			loop = false;
		}
		else {
			fmax = ftmp;
			x.front() = y.front();
			x.back() = y.back();
			hypersquare_search_method(x, fmax, used_f);
			counter = 0;
		}
	}

	cout << used_f << " " << fixed << setprecision(4) << x.front() << " " << x.back() << " " << fmax << endl;
}

//Третий алгоритм
void third_algorithm(int direction) {
	real x0 = -10, xe = 10;
	real y0 = -10, ye = 10;
	vector<real> x(2, 0), x1(2, 0), y(2, 0), dx(2, 0), xsh(2, 0), xsh1(2, 0);
	real fx, fx1, fy, fdx, fxsh, fxsh1;
	real e = 10e-5;
	int used_f = 0;
	bool loop = true;

	x.front() = random_number(x0, xe, 4);
	x.back() = random_number(y0, ye, 4);

	y.front() = x.front();
	y.back() = x.back();

	hypersquare_search_method(x, fx, used_f);

	while (loop) {
		dx.front() = x.front() - y.front();
		dx.back() = x.back() - y.back();

		real h = 0;

		xsh.front() = x.front();
		xsh.back() = x.back();
		fxsh = f(xsh);
		used_f++;
		do {
			xsh1.front() = xsh.front() + 2 * dx.front();
			xsh1.back() = xsh.back() + 2 * dx.back();
			fxsh1 = f(xsh1);
			used_f++;

			h = fxsh1 - fxsh;

			xsh.front() = xsh1.front();
			xsh.back() = xsh1.back();
			fxsh = fxsh1;
		} while (h <= 0 && xsh.front() < 10 && xsh.front() > -10 && xsh.back() < 10 && xsh.back() > -10);

		if (h > 0) {
			x1.front() = xsh.front();
			x1.back() = xsh.back();

			hypersquare_search_method(x1, fx1, used_f);
		}

		if (h > 0 && fx1 - fx > e) {
			x.front() = x1.front();
			x.back() = x1.back();
			fx = fx1;
		}

		else {
			int direction_count = 0;

			do {
				dx.front() = random_number(x0, xe, 4);
				dx.back() = random_number(y0, ye, 4);

				xsh.front() = x.front();
				xsh.back() = x.back();

				direction_count++;

				do {
					xsh1.front() = xsh.front() + 2 * dx.front();
					xsh1.back() = xsh.back() + 2 * dx.back();
					fxsh1 = f(xsh1);
					used_f++;

					h = fxsh1 - fxsh;

					xsh.front() = xsh1.front();
					xsh.back() = xsh1.back();
					fxsh = fxsh1;
				} while (h <= 0 && xsh.front() < 10 && xsh.front() > -10 && xsh.back() < 10 && xsh.back() > -10);

				if (h > 0) {
					x1.front() = xsh.front();
					x1.back() = xsh.back();

					hypersquare_search_method(x1, fx1, used_f);
				}

			} while ((h <= 0 || fx1 - fx < e) && direction_count < direction);

			if (h > 0 && fx1 - fx > e) {
				x.front() = x1.front();
				x.back() = x1.back();
				fx = fx1;
			}
			else loop = false;
		}
	}
	cout << used_f << " " << fixed << setprecision(4) << x.front() << " " << x.back() << " " << fx << endl;
}


int main() {
	switch (method_number) {
	case 1: {
		simple_random_search(0.1, 0.8);
		break;
	}
	case 2: {
		first_algorithm(100);
		break;
	}
	case 3: {
		second_algorithm(100000);
		break;
	}
	case 4:{
		third_algorithm(1000);
		break;
	}
	}
	return 0;
}