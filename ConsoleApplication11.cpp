// ConsoleApplication11.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


#include <iostream>
#include <thread>
#include <ctime>
#include <math.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <fstream>
#include <iomanip>
#include <random>
bool ok;
using namespace std;
float rez[30];
const int numarbit = 600, numarsize = 20;
int durata;
const double pi = 3.14159265358979323846;
thread threads[30];
random_device generator;
uniform_int_distribution<int> distribution01i(0, 1);
uniform_real_distribution<float> distribution01f(0, 1);
double rastrigin(int n, bool b[numarbit]) { // rastrigin fucntion
	float ra[30];
	float x;
	for (int i = 0; i < n; i++) {
		x = 0;
		for (int j = 0; j <= numarsize; j++)
		{
			x = x * 2;
			x += b[i * numarsize + j];
		}
		ra[i] = x / (pow(2, 17)) - 8;
        
	}
	double rez = 10 * n;
	for (int i = 0; i < n; i++) {
		rez += pow(ra[i], 2) - 10 * cos(2 * pi * ra[i]);
	} 
	return rez;
}
void mutatie(bool(&a)[100][numarbit], int n) { //mutation with 1% chance for each chromosone
	int i, j;
	for (i = 3; i < 100; i++)
		for (j = 0; j < n * numarsize; j++)
			if (distribution01f(generator) < 0.01)
				a[i][j] = !a[i][j];
}
void crossover(bool(&a)[100][numarbit], int n) { //crossover, 20% chance to be picked with 50% change of chormosone exchange
	uniform_int_distribution<int> distribution0n(1, n * numarsize - 1);
	int i, j, poz, k = 0, ok = 1;
	struct {
		float pr;
		int po;
	}prob[100], t;
	float min, pr = 0.2;
	bool x[numarbit], y[numarbit];

	for (i = 0; i < 100; i++)
	{
		prob[i].pr = distribution01f(generator);
		prob[i].po = i;
	}
	prob[0].pr = 0; prob[1].pr = 0; prob[2].pr = 0;
	for (i = 0; i < 100; i++) {
		min = prob[i].pr;
		poz = i;
		for (int j = i; j < 100; j++) {
			if (prob[j].pr < min)
			{
				min = prob[j].pr; poz = j;
			}
			k++;
		}
		t = prob[i];
		prob[i] = prob[poz];
		prob[poz] = t;
		if (min > pr)
			i = 101;
	}
	i = 0;
	while (ok) {
		if (prob[i].pr < pr && prob[i + 1].pr < pr)
		{
			for (j = 0; j < numarsize * n; j++)
			{
				x[j] = a[prob[i].po][j];
				y[j] = a[prob[i + 1].po][j];
			}
			poz = distribution0n(generator);
			for (j = 0; j < numarsize * n; j++) {
				if (j < poz) {
					a[prob[i].po][j] = y[j];
					a[prob[i + 1].po][j] = x[j];
				}
			}
		}
		if (prob[i].pr < pr && prob[i + 1].pr >= pr)
		{
			if (distribution01f(generator) < 0.5) {
				for (j = 0; j < numarsize * n; j++)
				{
					x[j] = a[prob[i].po][j];
					y[j] = a[prob[i + 1].po][j];
				}
				poz = distribution0n(generator);
				for (j = 0; j < numarsize * n; j++) {
					if (j < poz) {
						a[prob[i].po][j] = y[j];
						a[prob[i + 1].po][j] = x[j];
					}
				}
			}
			ok = 0;
		}
		if (prob[i].pr >= pr)
			ok = 0;
		i += 2;
	}
}
void roata(bool(&a)[100][numarbit], int n, int generatie, bool(&elit)[3][numarbit]) { //roulette wheel selection
	double roata[101] = {}, x, rez, min[3], max;
	int minpoz[3];
	minpoz[0] = 0; minpoz[1] = 1; minpoz[2] = 2;
	bool b[100][numarbit];
	int i, j, pmin;
	min[0] = DBL_MAX; min[1] = DBL_MAX; min[2] = DBL_MAX;
	//cout << "stop 1" << endl;
	for (i = 0; i < 100; i++)
		for (j = 0; j < n * numarsize; j++)
			b[i][j] = a[i][j];
	//cout << "stop 2" << endl;
	max = rastrigin(n, a[0]);
	//cout << "stop 3" << endl;
	for (i = 1; i < 101; i++)
	{
		rez = rastrigin(n, a[i - 1]);
		roata[i] = rez;
		if (rez < min[0])
		{
			min[2] = min[1];
			min[1] = min[0];
			min[0] = rez;
			minpoz[2] = minpoz[1];
			minpoz[1] = minpoz[0];
			minpoz[0] = i - 1;

		}
		if (max < rez)
		{
			max = rez;
		}
	}
	//cout << "stop 4" << endl;
	if (min[0] < rastrigin(n, elit[0])) {
		for (int i = 0; i < n * numarsize; i++)
			elit[0][i] = a[minpoz[0]][i];
	}
	if (min[1] < rastrigin(n, elit[1])) {
		for (int i = 0; i < n * numarsize; i++)
			elit[1][i] = a[minpoz[1]][i];
	}
	if (min[2] < rastrigin(n, elit[2])) {
		for (int i = 0; i < n * numarsize; i++)
			elit[2][i] = a[minpoz[2]][i];
	}
	//cout << "stop 5" << endl;
	for (i = 0; i < n * numarsize; i++) {
		a[0][i] = elit[0][i];
		a[1][i] = elit[1][i];
		a[2][i] = elit[2][i];
	}
	//cout << "stop 6" << endl;
	for (i = 1; i < 101; i++)
	{
		roata[i] = 1.1 * max - roata[i] + roata[i - 1];
	}
	//cout << "stop 7" << endl;
	for (i = 3; i < 100; i++)
	{
		x = roata[100] * distribution01f(generator);
		for (j = 3; j < 100; j++) {
			if (x > roata[j] && x < roata[j + 1])
				for (int k = 0; k < n * numarsize; k++) {
					a[i][k] = b[i][k];
				}
		}
	}
	//cout << "stop 8" << endl;
	//if (generatie % 100 == 0)
		//cout << "generatia " << generatie << " " << rastrigin(n, elit[0]) << endl;
}
void genetic_1(int n, int id) {//genetic algorithm, 10000 iterations
	int i, j, generatie = 0;
	bool elit[3][numarbit];
	bool a[100][numarbit];
	double gmin = DBL_MAX;
	for (int i = 0; i < 100; i++)
		for (int j = 0; j < numarsize * n; j++)
			a[i][j] = distribution01i(generator);
	while (generatie < 10000) {
		mutatie(a, n);
		crossover(a, n);
		roata(a, n, generatie, elit);
		generatie++;
	}
	rez[id] = rastrigin(n, a[0]);
}
void timer() {
	Sleep(durata);
	ok = 0;
}
void genetic(int n) { //multithreading
	ok = 1;
	ofstream p1;
	time_t t1, t2;
	t1 = time(0);
	char fisier[100];
	sprintf_s(fisier, "rastrigin_genetic_elitism_%d.txt", n);
	p1.open(fisier);
	p1 << fixed << setprecision(5);
	for (int i = 0; i < 30; i++) {
		threads[i] = thread(genetic_1, n, i);
	}
	for (int i = 0; i < 30; i++)
		threads[i].join();
	//abc.join();
	t2 = time(0);
	float min, med = 0;
	min = rez[1];
	for (int i = 0; i < 30; i++)
	{
		p1 << rez[i] << endl;
		if (rez[i] < min)
			min = rez[i];
		med += rez[i];
	}
	p1 << "med=" << med / 30 << endl << "min=" << min << endl << t2 - t1 << "secunde";
	//cout << t2 - t1 << "secunde" << endl;
	p1.close();
}
int main() {
	genetic(2);
	genetic(15);
	genetic(30);
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
