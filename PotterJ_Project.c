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
#define MIN_RATING 1
#define MAX_RATING 5
#define CATEGORIES 3
#define CORRECT_ID "id1"
#define CORRECT_PASSCODE "ABCD"
#define SIZE_STRING 80
#define MAX_ATTEMPT 4
#define SENTINEL_VALUE -1

typedef struct rideshare {
	double baseFare;
	double costPerMin;
	double costPerMile;
	double minFlatRate;
	unsigned int surveyCount;
	unsigned int totalMinutes;
	double totalMiles;
	double totalFare;
	double totalRideCount;
	char categoryNames[CATEGORIES][SIZE_STRING];
	int rating[MAX_SURVEY][CATEGORIES];
	double surveyAvg[CATEGORIES];
	char organizationName[SIZE_STRING];
} RideShare;

bool LoginAdmin(const char* correctLogin, const char* correctPass, int size, int maxAttempts);
double getValidDoubleSentinel(int min, int max, int sentinel);
double getValidDouble(int min, int max);
int calculateRandomNumber(int min, int max);
double calculateFare(double base, double minuteCost, double mileCost, double minRate, double miles, int minutes);
void printFare(int count, double miles, int minutes, double fare);
bool scanDouble(const char* buff, double *validNumber);
void FgetsRemoveNewLine(char* string);
void RidersMode(double baseFare, double costPerMin, double costPerMile, double minFlatRate, const char* name, int totalMinutes, double totalMiles
	, double totalFare, int totalRideCount, int surveyRatings[][CATEGORIES], int surveyCount, const char* username, const char* password,
	double surveyAvg[CATEGORIES]);
void getSurveyRatings(int surveyRatings[][CATEGORIES], const char *categoryNames[CATEGORIES], int* surveyCount, size_t totalCategories, int min, int max);
void calculateSurveyAvg(const int surveyRatings[][CATEGORIES], double surveyAvg[CATEGORIES], int surveyCount, size_t totalCategories);
void displaySurveyAvg(const double surveyAvg[CATEGORIES], const char* categoryNames[CATEGORIES]);
void displaySurveyRatings(const int surveyRatings[][CATEGORIES], const char *categoryNames[CATEGORIES], int surveyCount);
void setUp(RideShare* rideShare);
char getYorN();

const char* surveyCategories[CATEGORIES] = { "Safety", "Cleanliness", "Comfort" };

int main(void) {
	//Generates random seed to calculate a random number
	srand((unsigned int)time(NULL));

	if (LoginAdmin(CORRECT_ID, CORRECT_PASSCODE, SIZE_STRING, MAX_ATTEMPT) == true) {
		RideShare rideShare;
		RideShare* rideSharePtr = &rideShare;
		setUp(rideSharePtr);
		RidersMode(rideShare.baseFare, rideShare.costPerMin, rideShare.costPerMile, rideShare.minFlatRate, rideShare.organizationName,
			rideShare.totalMinutes, rideShare.totalMiles, rideShare.totalFare, rideShare.totalRideCount, rideShare.rating, rideShare.surveyCount
		, CORRECT_ID, CORRECT_PASSCODE, rideShare.surveyAvg);

	}
	printf("%s", "You reached the max amount of login attempts");
	return 0;
}

char getYorN() {
	char input;
	bool end = false;

	while (!end) {
		int YorN = getchar();
		input = (char)YorN;

		while (getchar() != '\n');

		if (input == 'y' || input == 'Y' || input == 'n' || input == 'N') {
			end = true;
		}
		else {
			printf("%s", "Enter Y or N: ");
		}
	}
	return input;
}

//Function for getting a valid double between a set range
double getValidDouble(int min, int max) {
	double number = 0;
	bool validInput = false;
	char input[SIZE_STRING] = "";
	while (!validInput) {
		//printf("%s%d%s%d", "Enter a number between ", min, " & ", max);
		FgetsRemoveNewLine(&input);
		if (scanDouble(&input, &number) == false) {
			printf("%s", "Enter a number.");
		}
		else if (number > max || number < min) {
			printf("%s", "Number is out of range");
		}
		else {
			validInput = true;
			return number;
		}
	}
}

// This function is for validating the user input for miles.
// it ensurse the value is indeed a number and is between 1-100
double getValidDoubleSentinel(int min, int max, int sentinel) {
	double miles = 0;
	bool validInput = false;
	char input[SIZE_STRING] = "";
	// iterates until validInput is changed to true
	while (!validInput) {
		FgetsRemoveNewLine(&input);
		if (scanDouble(&input, &miles) == false) {
			printf("%s", "Error: Invalid input. Please enter a number.\n");

			while (getchar() != '\n');
		}
		else if (miles == sentinel) {
			validInput = true;
		}
		else if (miles < min || miles > max) {
			printf("%s", "Error: Entry out of range. Enter value between 0-100: \n");
		}
		else {
			validInput = true;
		}
	}

	return miles;
}

//Acts like scanf for doubles
bool scanDouble(const char* buffer, double *validNumber) {
	char* end;
	*validNumber = 0;
	double intTest = strtod(buffer, &end);

		if (end == buffer) {
			return false;
		}
		else if ('\0' != *end) {
			return false;
		}
		else if ((DBL_MIN == intTest || DBL_MAX == intTest) && ERANGE == errno) {
			return false;
		}
		else if (intTest > INT_MAX) {
			return false;
		}
		else if (intTest < INT_MIN) {
			return false;
		}
		else {
			*validNumber = (double)intTest;
			return true;
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

//Function to prompt admin to login
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
			if (strcmp(username, correctUsername) != 0) {
				printf("%s\n", "Invalid login.");
				attempts++;
				if (attempts >= maxAttempts) {
					return false;
				}
			}
		}

		while (strcmp(pass, correctPass) != 0) {
			printf("%s", "Enter your password: ");
			FgetsRemoveNewLine(passPtr);
			if (strcmp(pass, correctPass) != 0) {
				printf("%s\n", "Incorrect password.");
				attempts++;
				if (attempts >= maxAttempts) {
					return false;
				}
			}
		}
		correctLogin = true;
	}
	return true;
}

//Removes new line character
void FgetsRemoveNewLine(char* stringPtr) {
	fgets(stringPtr, SIZE_STRING, stdin);
	int strLength = strlen(stringPtr);
	if (stringPtr[strLength - 1] == '\n') {
		stringPtr[strLength - 1] = '\0';
	}
}

//Function to set up the ride share variables
void setUp(RideShare* rideShare) {
	double min = 0.1;
	double max = 50;
	rideShare->totalFare = 0;
	rideShare->totalMiles = 0;
	rideShare->totalMinutes = 0;
	rideShare->totalRideCount = 0;
	rideShare->surveyCount = 0;

	char input[SIZE_STRING];
	double number = 0;

	//Promting user for baseFare
	printf("%s", "Enter the amount base fare: ");
	FgetsRemoveNewLine(input);
	while (!scanDouble(input, &number) || number > max || number < min) {
		printf("%s", "\nInvalid input. Enter base fare: ");
		FgetsRemoveNewLine(input);
	}
	rideShare->baseFare = number;

	//Promting user for costPerMin
	printf("%s\n", "Enter the amount cost per minute: ");
	FgetsRemoveNewLine(input);
	while (!scanDouble(input, &number) || number > max || number < min) {
		printf("%s", "\nInvalid input. Enter cost per min: ");
		FgetsRemoveNewLine(input);
	}
	rideShare->costPerMin = number;

	//Promting user for costPerMile
	printf("%s\n", "Enter the amount cost per mile: ");
	FgetsRemoveNewLine(input);
	while (!scanDouble(input, &number) || number > max || number < min) {
		printf("%s", "\nInvalid input. Enter cost per mile: ");
		FgetsRemoveNewLine(input);
	}
	rideShare->costPerMile = number;

	//Promting user for minFlatRate
	printf("%s\n", "Enter the amount min flat rate: ");
	FgetsRemoveNewLine(input);
	while (!scanDouble(input, &number) || number > max || number < min) {
		printf("%s", "\nInvalid input. Enter min flat rate: ");
		FgetsRemoveNewLine(input);
	}
	rideShare->minFlatRate = number;

	printf("%s\n", "Enter orgnization name: ");
	FgetsRemoveNewLine(rideShare->organizationName);

	printf("%s%.2f\n","Base fare: ", rideShare->baseFare);
	printf("%s%.2f\n","Cost per minute: ", rideShare->costPerMin);
	printf("%s%.2f\n","Cost per mile: ", rideShare->costPerMile);
	printf("%s%.2f\n","Min flat rate: ", rideShare->minFlatRate);
	printf("%s%s\n","Organization Name: ", rideShare->organizationName);
}

//Function to store survey ratings in surveyRatings array
void getSurveyRatings(int surveyRatings[][CATEGORIES], const char *categoryNames[CATEGORIES], int* surveyCount, size_t totalCategories, int min, int max) {
	int name = 0;
	for (size_t categories = 0; categories < totalCategories; categories++) {
		printf("%s%s%s%d%s%d%s", "Enter ", categoryNames[name], " rating(" , min , "-" , max , ")");
		surveyRatings[*surveyCount][categories] = getValidDouble(min, max);
		name++;
	}
	surveyCount++;
}

//Function to display current ratings
void displaySurveyRatings(const int surveyRatings[][CATEGORIES], const char *categoryNames[CATEGORIES], int surveyCount) {
	printf("Survey Results:\n");
	if (surveyCount > 0) {
		for (int i = 0; i < CATEGORIES; i++) {
			printf("\t\t%s", categoryNames[i]);
		}
		puts("\n");
		for (int i = 0; i < surveyCount; i++) {
			printf("Survey #%d", i);
			for (int e = 0; e < CATEGORIES; e++) {
				printf("\t%d\t", surveyRatings[i][e]);
			}
			printf("\n");
		}
	}
	else {
		printf("%s", "No ratings currently\n");
	}
}

void calculateSurveyAvg(const int surveyRatings[][CATEGORIES], double surveyAvg[CATEGORIES], int surveyCount, size_t totalCategories) {
	// size_t iterates through an array in bit sized chunks
	for (size_t categories = 0; categories < totalCategories; categories++) {
		double average = 0;
		for (int i = 0; i < surveyCount; i++) {
			average += surveyRatings[i][categories];
		}
		surveyAvg[categories] = average / surveyCount;
	}
}

void displaySurveyAvg(const double surveyAvg[CATEGORIES], const char *categoryNames[CATEGORIES]) {
	printf("%s", "Survey Averages:\n");
	for (int i = 0; i < CATEGORIES; i++) {
		printf("\t\t%s", categoryNames[i]);
	}
	puts("\n");
	for (int i = 0; i < CATEGORIES; i++) {
		printf("\t\t%.2f", surveyAvg[i]);
	}
}

void RidersMode(double baseFare, double costPerMin, double costPerMile, double minFlatRate, const char *name, int totalMinutes, double totalMiles
, double totalFare, int totalRideCount , int surveyRatings[][CATEGORIES], int surveyCount, const char *username, const char *password,
double surveyAvg[CATEGORIES]) {
	int minMiles = 1;
	int maxMiles = 100;
	totalRideCount = 0;
	totalFare = 0;
	totalMinutes = 0;
	totalMiles = 0;

	bool endRider = false;

	while (!endRider) {
		printf("%s%s%s", "\nWelcome to ", name, " Ride Share. We can only "
			"provide services for rides from 1 to 100 miles.\n");
		displaySurveyRatings(surveyRatings, surveyCategories,surveyCount);

		printf("%s", "Do you want to take a ride? Y or N?");
		char YorN = getYorN();
		if (YorN == 'Y' || YorN == 'y') {
			printf("%s%d%s%d%s", "Enter the number of miles your travelling? ", minMiles, " - ", maxMiles, ":");
			double miles = getValidDoubleSentinel(minMiles, maxMiles, SENTINEL_VALUE);
			if (miles == SENTINEL_VALUE) {
				if (LoginAdmin(username, password, SIZE_STRING, MAX_ATTEMPT) == true) {
					endRider = true;
				}
			}
			else {
				totalRideCount += 1;
				int minRandomMinutes = MIN_RAND_MINUTES_FACTOR * miles;
				int maxRandomMinutes = MAX_RAND_MINUTES_FACTOR * miles;

				//calculating ride time and price total for customer
				int rideTime = calculateRandomNumber(minRandomMinutes, maxRandomMinutes);
				double rideTotal = calculateFare(baseFare, costPerMin, costPerMile, minFlatRate, miles, rideTime);

				//incrementing total values for business owner
				totalMiles += miles;
				totalMinutes += rideTime;
				totalFare += rideTotal;

				printf("\nCustomer Summary\n");
				printFare(totalRideCount, miles, rideTime, rideTotal);

				printf("%s", "Do you want to take survey? Y or N:");
				YorN = getYorN();
				if (YorN == 'Y' || YorN == 'y') {
					getSurveyRatings(surveyRatings, surveyCategories, &surveyCount, CATEGORIES, MIN_RATING, MAX_RATING);
					surveyCount++;
					printf("%s", "Thanks for your feedback. Have a great day! ");
				}
				else {
					printf("%s", "Thanks for riding! Have a splendid day!");
				}
			}
		}
	}
	// This part is for the business owner and executes once the sentinel value is input
	printf("%s", "UCCS Ride Share Business Summary\n\n");
	printFare(totalRideCount, totalMiles, totalMinutes, totalFare);
	calculateSurveyAvg(surveyRatings, surveyAvg, surveyCount, CATEGORIES);
	displaySurveyAvg(surveyAvg, surveyCategories);
}