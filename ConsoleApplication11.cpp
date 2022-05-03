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
float res[30];
const int numberOfBits = 600, numberSize = 20;
int duration;
const double pi = 3.14159265358979323846;
struct tupl {
	float pr;
	int po;
};
thread threads[30];
random_device generator;
uniform_int_distribution<int> distribution01i(0, 1);
uniform_real_distribution<float> distribution01f(0, 1);

// rastrigin function
double rastrigin(int n, bool b[numberOfBits]) {
	float ra[30];
	float x;
	for (int i = 0; i < n; i++) {
		x = 0;
		for (int j = 0; j <= numberSize; j++)
		{
			x = x * 2;
			x += b[i * numberSize + j];
		}
		ra[i] = x / (pow(2, 17)) - 8;
	}
	double res = 10 * n;
	for (int i = 0; i < n; i++) {
		res += pow(ra[i], 2) - 10 * cos(2 * pi * ra[i]);
	}
	return res;
}

// mutation wita a 1% chance
void mutation(bool(&a)[100][numberOfBits], int n) {
	int i, j;
	for (i = 3; i < 100; i++)
		for (j = 0; j < n * numberSize; j++)
			if (distribution01f(generator) < 0.01)
				a[i][j] = !a[i][j];
}

// assigns a probability and sorts the population in ascending order
void assignAndSortProbabilities(tupl(&prob)[100])
{	
	int k = 0, poz;
	tupl t;
	float min, pr = 0.2;
	for (int i = 0; i < 100; i++)  // assign values between 0 and 1 to the population
	{
		prob[i].pr = distribution01f(generator);
		prob[i].po = i;
	}
	prob[0].pr = 0; prob[1].pr = 0; prob[2].pr = 0;
	for (int i = 0; i < 100; i++) {  // sorting the population
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
		if (min > pr)  // stop sorting if all the unsorted value are over 0.2
			i = 101;
	}
}
// crossover with a 20% chance
void crossover(bool(&a)[100][numberOfBits], int n) {
	uniform_int_distribution<int> distribution0n(1, n * numberSize - 1);
	int i, j, poz, ok = 1;
	tupl prob[100], t;
	float pr = 0.2;
	bool x[numberOfBits], y[numberOfBits];
	assignAndSortProbabilities(prob);
	i = 0;
	while (ok) {  // swap bits from the chosen members
		if (prob[i].pr < pr && prob[i + 1].pr < pr)  
		{
			for (j = 0; j < numberSize * n; j++)
			{
				x[j] = a[prob[i].po][j];
				y[j] = a[prob[i + 1].po][j];
			}
			poz = distribution0n(generator);
			for (j = 0; j < numberSize * n; j++) {
				if (j < poz) {
					a[prob[i].po][j] = y[j];
					a[prob[i + 1].po][j] = x[j];
				}
			}
		}
		if (prob[i].pr >= pr)
			ok = 0;
		i += 2;
	}
}
// computes the result of the rastrigin function and saves the positions of the best members of the population
void computeResult(bool(&a)[100][numberOfBits], int n, double (&wheel)[101],double &max, double (&min)[3], int (&minPos)[3])
{
	double res;
	int i;
	max = rastrigin(n, a[0]);
	for (i = 1; i < 101; i++)
	{
		res = rastrigin(n, a[i - 1]);
		wheel[i] = res;
		if (res < min[0])
		{
			min[2] = min[1];
			min[1] = min[0];
			min[0] = res;
			minPos[2] = minPos[1];
			minPos[1] = minPos[0];
			minPos[0] = i - 1;

		}
		if (max < res)
		{
			max = res;
		}
	}
}
// compare the best members of the population tot the 3 best members from the previous generation
void elitism(bool(&a)[100][numberOfBits],int n, double (&min)[3], bool(&elit)[3][numberOfBits], int(&minPos)[3])
{
	if (min[0] < rastrigin(n, elit[0])) {
		for (int i = 0; i < n * numberSize; i++)
			elit[0][i] = a[minPos[0]][i];
	}
	if (min[1] < rastrigin(n, elit[1])) {
		for (int i = 0; i < n * numberSize; i++)
			elit[1][i] = a[minPos[1]][i];
	}
	if (min[2] < rastrigin(n, elit[2])) {
		for (int i = 0; i < n * numberSize; i++)
			elit[2][i] = a[minPos[2]][i];
	}
	// add the best memebers to the current population
	for (int i = 0; i < n * numberSize; i++) {
		a[0][i] = elit[0][i];
		a[1][i] = elit[1][i];
		a[2][i] = elit[2][i];
	}
}
// picks the new negeration using the roulette wheel method
void pickNewGeneration(bool (&a)[100][numberOfBits], bool(&b)[100][numberOfBits], double(&wheel)[101], double max, int n)
{
	double x;
	for (int i = 1; i < 101; i++)
	{
		wheel[i] = 1.1 * max - wheel[i] + wheel[i - 1];
	}
	for (int i = 3; i < 100; i++)
	{
		x = wheel[100] * distribution01f(generator);
		for (int j = 3; j < 100; j++) {
			if (x > wheel[j] && x < wheel[j + 1])
				for (int k = 0; k < n * numberSize; k++) {
					a[i][k] = b[i][k];
				}
		}
	}
}
// roulette wheel selection
void wheel(bool(&a)[100][numberOfBits], int n, int generation, bool(&elit)[3][numberOfBits])
{
	double wheel[101] = {}, min[3], max;
	int minPos[3];
	minPos[0] = 0; minPos[1] = 1; minPos[2] = 2;
	bool b[100][numberOfBits];
	int i, j, pmin;
	min[0] = DBL_MAX; min[1] = DBL_MAX; min[2] = DBL_MAX;
	for (i = 0; i < 100; i++)
		for (j = 0; j < n * numberSize; j++)
			b[i][j] = a[i][j];
	computeResult(a, n, wheel, max, min, minPos);
	elitism(a,n,min,elit,minPos);
	pickNewGeneration(a,b,wheel,max,n);
	if (generation % 100 == 0)
		cout << "generation " << generation << " " << rastrigin(n, elit[0]) << endl;
}
// the genetic algorithm function
void genetic_1(int n, int id) {
	int i, j, generation = 0;
	bool elit[3][numberOfBits];
	bool a[100][numberOfBits];
	double gmin = DBL_MAX;
	for (int i = 0; i < 100; i++)
		for (int j = 0; j < numberSize * n; j++)
			a[i][j] = distribution01i(generator);
	while (generation < 10000) {
		mutation(a, n);
		crossover(a, n);
		wheel(a, n, generation, elit);
		generation++;
	}
	res[id] = rastrigin(n, a[0]);
}
// runs the genetic algorithm on 30 threads and saves the results into a text file
void genetic(int n) {
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
	t2 = time(0);
	float min, med = 0;
	min = res[1];
	for (int i = 0; i < 30; i++)
	{
		p1 << res[i] << endl;
		if (res[i] < min)
			min = res[i];
		med += res[i];
	}
	p1 << "med=" << med / 30 << endl << "min=" << min << endl << t2 - t1 << "seconds";
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
