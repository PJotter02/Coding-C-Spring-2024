#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// used to generate random number of minutes based on number of miles
#define MIN_RAND_MINUTES_FACTOR 1.2
#define MAX_RAND_MINUTES_FACTOR 1.5
#define CATEGORIES 3
#define SIZE_STRING 80
#define MAX_ATTEMPT 4
//sentinel value to end rider mode.
#define SENTINEL_VALUE -1

typedef struct rideshare {
	double baseFare;
	double totalBaseFare;
	double costPerMin;
	double totalCostPerMin;
	double costPerMile;
	double totalCostPerMile;
	double minFlatRate;
	int rating[][CATEGORIES];
	char organizationName[SIZE_STRING];
} RideShare;

int validLogin(const char* correctLogin, const char* correctPass, int size, int maxAttempts);
double getValidDouble(int min, int max, int sentinel);
int calculateRandomNumber(int min, int max);
double calculateFare(double base, double minuteCost, double mileCost, double minRate, double miles, int minutes);
void printFare(int count, double miles, int minutes, double fare);
void FgetsRemoveNewLine(char* string);
void setUp(RideShare* rideShare);


int main(void) {
	const char correctLogin[] = "Admin";
	const char correctPass[] = "Admin";
	if (validLogin(correctLogin, correctPass, SIZE_STRING, MAX_ATTEMPT == 1)) {
		double baseFare = 1.8;
		double costPerMinute = 0.25;
		double costPerMile = 1.2;
		double minFlatRate = 20.00;
		// Vars for tracking total
		unsigned int totalRideCount = 0;
		double totalMiles = 0;
		unsigned int totalMinutes = 0;
		double totalFare = 0;

		//Boolean flag
		bool continueRider = false;
		while (!continueRider) {
			//min and max miles range
			int minMiles = 1;
			int maxMiles = 100;

			// generating random seed based of systems time for generating a random number
			// between minRandomMinutes and maxRandomMinutes
			srand(time(NULL));

			printf("%s", "Welcome to UCCS Ride Share. We can only\n"
				"provide services for rides from 1 to 100 miles.\n");

			double miles = getValidDouble(minMiles, maxMiles, SENTINEL_VALUE);

			// While loop that iterates until the sentinel value -1 
			while (miles != SENTINEL_VALUE) {
				totalRideCount += 1;
				int minRandomMinutes = MIN_RAND_MINUTES_FACTOR * miles;
				int maxRandomMinutes = MAX_RAND_MINUTES_FACTOR * miles;

				//calculating ride time and price total for customer
				int rideTime = calculateRandomNumber(minRandomMinutes, maxRandomMinutes);
				double rideTotal = calculateFare(baseFare, costPerMinute, costPerMile, minFlatRate, miles, rideTime);

				//incrementing total values for business owner
				totalMiles += miles;
				totalMinutes += rideTime;
				totalFare += rideTotal;

				printf("\nCustomer Summary\n");
				printFare(totalRideCount, miles, rideTime, rideTotal);
				miles = getValidDouble(minMiles, maxMiles, SENTINEL_VALUE);
			}
		}

		// This part is for the business owner and executes once the sentinel value is input
		printf("%s", "UCCS Ride Share Business Summary\n\n");
		printFare(totalRideCount, totalMiles, totalMinutes, totalFare);
	}
	printf("%s", "You reached the max amount of login attempts");
	return 0;
}

// This function is for validating the user input for miles.
// it ensurse the value is indeed a number and is between 1-100
double getValidDouble(int min, int max, int sentinel) {
	double miles = 0;
	bool validInput = false;

	// iterates until validInput is changed to true
	while (!validInput) {
		printf("%s", "\nEnter the number of miles to your desination: ");
		if (scanf("%lf", &miles) != 1) {
			printf("%s", "Error: Invalid input. Please enter a number.\n");

			//code clears buffer
			while (getchar() != '\n');
		}
		else if (miles == sentinel) {
			validInput = true;
		}
		else if (miles < min || miles > max) {
			printf("%s", "Error: Miles must be between 1 and 100.\n");
		}
		else {
			validInput = true;
		}
	}

	return miles;
}

//Code for getting random time.
int calculateRandomNumber(int min, int max) {
	int time = rand() % (max - min + 1) + min;
	return time;
}

// Code for calculating the price of the trip
double calculateFare(double base, double minuteCost, double mileCost, double minRate, double miles, int minutes) {
	double total = base + (minuteCost * minutes) + (mileCost * miles);
	if (total < minRate) {
		total = minRate;
	}

	return total;
}

// Code for printing customer overview and business overview
void printFare(int count, double miles, int minutes, double fare) {
	if (count != 0) {
		printf("%s", "\nCurrent Ride Information\n\n");
		printf("%s", "Rider  Number of Miles  Number of Minutes  Ride Fare Amount\n");
		printf("%d      %.1f             %d                 $%.2f\n", count, miles, minutes, fare);
	}
	else {
		printf("There were no rides.");
	}
}

int validLogin(const char* correctLogin, const char* correctPass, int size, int maxAttempts) {
	int attempts = 0;
	char login[SIZE_STRING];
	char pass[SIZE_STRING];
	char* loginPtr = login;
	char* passPtr = pass;

	//Why does it not jum out of while loop even if attempts exceeds maxAttempts aka 4
	while (attempts < maxAttempts) {
		printf("%s\n", "Enter your username: ");
		//investigate &login
		fgets(loginPtr, size, stdin);
		FgetsRemoveNewLine(loginPtr);


		//While loop to compare logins
		while (strcmp(login, correctLogin) != 0) {
			printf("%s\n", "Incorrect username. Retry.");
			printf("%s", "Enter your username: ");
			fgets(login, size, stdin);
			attempts++;
		}
		printf("%s", "Enter your password: ");
		fgets(passPtr, size, stdin);

		while (strcmp(pass, correctPass) != 0) {
			printf("%s\n", "Incorrect password. Retry.");
			printf("%s", "Enter your password: ");
			fgets(pass, size, stdin);
			attempts++;
		}
		return 1;
	}
	return 0;
}

void FgetsRemoveNewLine(char* string) {
	fgets(string, SIZE_STRING, stdin);
	int strLength = strlen(string);
	if (string[strLength - 1] == '\n') {
		string[strLength - 1] = '\0';
	}
}

void setUp(RideShare* rideShare) {

}