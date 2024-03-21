#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <float.h>
#include <string.h>

// used to generate random number of minutes based on number of miles
#define MIN_RAND_MINUTES_FACTOR 1.2
#define MAX_RAND_MINUTES_FACTOR 1.5
#define MAX_SURVEY 7
#define CATEGORIES 3
#define SIZE_STRING 80
#define MAX_ATTEMPT 4
//sentinel value to end rider mode.
#define SENTINEL_VALUE -1

typedef struct rideshare {
	double baseFare;
	double costPerMin;
	double costPerMile;
	double totalCostPerMile;
	double minFlatRate;
	unsigned int surveyCount;
	unsigned int totalMinutes;
	double totalMiles;
	double totalFare;
	int rating[MAX_SURVEY][CATEGORIES];
	char organizationName[SIZE_STRING];
} RideShare;

bool LoginAdmin(const char* correctLogin, const char* correctPass, int size, int maxAttempts);
double getValidDouble(int min, int max, int sentinel);
int calculateRandomNumber(int min, int max);
double calculateFare(double base, double minuteCost, double mileCost, double minRate, double miles, int minutes);
void printFare(int count, double miles, int minutes, double fare);
double scanDouble(const char* buff);
void FgetsRemoveNewLine(char* string);
void RidersMode(double baseFare, double costPerMin, double costPerMile, double minFlatRate, const char name, int totalMinutes, double totalMiles
, double totalFare, int sentinel);
void getSurveyRatings(int surveyRatings[][CATEGORIES], int* surveyCount, size_t totalCategories, int min, int max);
void setUp(RideShare* rideShare);
char getYorN();


int main(void) {
	const char correctLogin[] = "Admin";
	const char correctPass[] = "Admin";
	char number[SIZE_STRING];
	/*fgets(number, SIZE_STRING, stdin);
	FgetsRemoveNewLine(number);
	scanDouble(number);
	*/
	if (LoginAdmin(correctLogin, correctPass, SIZE_STRING, MAX_ATTEMPT) == true) {
		RideShare rideShare;
		RideShare* rideSharePtr = &rideShare;
		setUp(rideSharePtr);

		double totalMiles = 0;
		double totalFare = 0;
		int totalMinutes = 0;
		int totalRideCount = 0;
		//Boolean flag
		bool continueRider = false;
		while (!continueRider) {
			int totalRideCount = 0;
			int baseFare = 1;
			int costPerMinute = 1;
			int costPerMile = 1;
			int minFlatRate = 1;
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

double scanDouble(const char* buff) {
	char* end;
	int validDouble = 0;
	double intTest = strtod(buff, &end);

	if (end == buff) {
		printf("%s is not a decimal number", buff);
	}
	else if ('\0' != *end) {
		printf("%s: extra characters at end of input: %s\n", buff, end);
	}
	else if ((DBL_MIN == intTest || DBL_MAX == intTest) && ERANGE == errno) {
		printf("%s out of range of type double\n", buff);
	}
	else if (intTest > INT_MAX) {
		printf("%ld greater than INT_MAX\n", intTest);
	}
	else if (intTest < INT_MIN) {
		printf("%ld less than INT_MIN\n", intTest);
	}
	else {
		validDouble = (double)intTest;
		printf("%d is integer value ", validDouble);
	}
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

bool LoginAdmin(const char* correctUsername, const char* correctPass, int size, int maxAttempts) {
	int attempts = 0;
	char username[SIZE_STRING];
	char pass[SIZE_STRING];
	char* usernamePtr = username;
	char* passPtr = pass;

	//Why does it not jum out of while loop even if attempts exceeds maxAttempts aka 4
	bool correctLogin = false;
	while (!correctLogin) {

		//While loop to compare logins
		while (strcmp(username, correctUsername) != 0) {
			printf("%s", "Enter your username: ");
			FgetsRemoveNewLine(usernamePtr);
			attempts++;
			if (attempts > maxAttempts) {
				return false;
			}
		}

		while (strcmp(pass, correctPass) != 0) {
			printf("%s", "Enter your password: ");
			FgetsRemoveNewLine(passPtr);
			attempts++;
			if (attempts > maxAttempts) {
				return false;
			}
		}
		correctLogin = true;
	}
	return true;
}

void FgetsRemoveNewLine(char* stringPtr) {
	fgets(stringPtr, SIZE_STRING, stdin);
	int strLength = strlen(stringPtr);
	if (stringPtr[strLength - 1] == '\n') {
		stringPtr[strLength - 1] = '\0';
	}
}

void setUp(RideShare* rideShare) {
	char input[SIZE_STRING];
	printf("%s\n", "Enter the amount base fare: ");
	FgetsRemoveNewLine(input);
	rideShare->baseFare = scanDouble(input);

	printf("%s\n", "Enter the amount cost per minute: ");
	FgetsRemoveNewLine(input);
	rideShare->costPerMin = scanDouble(input);

	printf("%s\n", "Enter the amount cost per mile: ");
	FgetsRemoveNewLine(input);
	rideShare->costPerMile = scanDouble(input);

	printf("%s\n", "Enter the amount min flat rate: ");
	FgetsRemoveNewLine(input);
	rideShare->minFlatRate = scanDouble(input);

	printf("%s\n", "Enter orgnization name: ");
	fgets(rideShare->organizationName, SIZE_STRING, stdin);

	printf("%s%.2f","Base fare: ", rideShare->baseFare);
	printf("%.2f","Cost per minute: ", rideShare->costPerMin);
	printf("%s%.2f","Cost per mile: ", rideShare->costPerMile);
	printf("%s%.2f","Min flat rate: ", rideShare->minFlatRate);
	printf("%s%s","Organization Name: ", rideShare->organizationName);
}

void getSurveyRatings(int surveyRatings[][CATEGORIES], int* surveyCount, size_t totalCategories, int min, int max) {

	for (size_t categories; categories < totalCategories; categories++) {
		printf("%s", "Enter categories rating: ");
		//surveyRatings[*surveyCount][categories] = getValid(min, max);
	}
	*surveyCount++;
}