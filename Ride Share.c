#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

// used to generate random number of minutes based on number of miles
#define MIN_RAND_MINUTES_FACTOR 1.2
#define MAX_RAND_MINUTES_FACTOR 1.5
//sentinel value to end rider mode
#define SENTINEL_VALUE -1

int getValidInt(int min, int max, int sentinel);
double calculateFare(double base, double minuteCost, double mileCost, double minRate, double miles, int minutes);
void printFare(int count, double miles, int minutes, double fare);

int main(void) {
	//min and max miles range
	int minMiles = 1;
	int maxMiles = 100;

	// prices for ride
	double baseFare = 1.8;
	double costPerMinute = 0.25;
	double costPerMile = 1.2;
	double minFlatRate = 20.00;

	// vars for keeping track of each ride
	int totalRideCount = 0;
	double totalMiles = 0;
	int totalMinutes = 0;
	double totalFare = 0;

	// generating random seed based of systems time
	srand(time(NULL));

	printf("%s", "Welcome to UCCS Ride Share. We can only\n"
		"provide services for rides from 1 to 100 miles.\n");
	
	int miles = getValidInt(minMiles, maxMiles , SENTINEL_VALUE);

	while (miles != SENTINEL_VALUE) {
		totalRideCount += 1;
		int minRandomMinutes = MIN_RAND_MINUTES_FACTOR * miles;
		int maxRandomMinutes = MAX_RAND_MINUTES_FACTOR * miles;
		int rideTime = rand() % (maxRandomMinutes - minRandomMinutes + 1) + minRandomMinutes;
		double rideTotal = calculateFare(baseFare, costPerMinute, costPerMile, minFlatRate, miles, rideTime);
		totalMiles += miles;
		totalMinutes += rideTime;
		totalFare += rideTotal;
		printFare(totalRideCount, miles, rideTime, rideTotal);
		miles = getValidInt(minMiles, maxMiles, SENTINEL_VALUE);
	}

	printf("%s", "UCCS Ride Share Business Summary\n\n");
	printFare(totalRideCount, totalMiles, totalMinutes, totalFare);

	return 0;
}

int getValidInt(int min, int max, int sentinel) {
	int miles = 0;
	bool validInput = false;

	while (!validInput) {
		printf("%s", "Enter the number of miles to your desination: ");
		if (scanf("%d", &miles) != 1) {
			printf("%s", "Error: Invalid input. Please enter a number.\n");
			while (getchar() != '\n');
		}
		else if (miles == sentinel) {
			validInput = true;
		}
		else if (miles < min || miles > max) {
			printf("%s","Error: Miles must be between 1 and 100.\n");
		}
		else {
			validInput = true;
		}
	}

	return miles;
}

double calculateFare(double base, double minuteCost, double mileCost, double minRate, double miles, int minutes) {
	double total = base + (minuteCost * minutes) + (mileCost * miles);
	if (total < minRate) {
		total = minRate;
	}

	return total;
}

void printFare(int count, double miles, int minutes, double fare) {
	printf("%s", "\nCurrent Ride Information\n\n");
	printf("%s", "Rider  Number of Miles  Number of Minutes  Ride Fare Amount\n");
	printf("%d      %.1f             %d                 $%.2f\n", count, miles, minutes, fare);
}