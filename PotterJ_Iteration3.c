#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <float.h>
#include <string.h>
#include <ctype.h>

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
#define FILE_PATH "C:/rideshare/"

typedef struct rideshare {
	double baseFare;
	double costPerMin;
	double costPerMile;
	double minFlatRate;
	bool found;
	unsigned int surveyCount;
	unsigned int totalMinutes;
	double totalMiles;
	double totalFare;
	double totalRideCount;
	int rating[MAX_SURVEY][CATEGORIES];
	double surveyAvg[CATEGORIES];
	char organizationName[SIZE_STRING];
	struct rideshare *nextRideSharePtr;
} RideShare;

bool LoginAdmin(const char* correctLogin, const char* correctPass, int size, int maxAttempts);
double getValidDoubleSentinel(int min, int max, int sentinel);
double getValidDouble(int min, int max);
int calculateRandomNumber(int min, int max);
double calculateFare(double base, double minuteCost, double mileCost, double minRate, double miles, int minutes);
void printFare(int count, double miles, int minutes, double fare);
bool scanDouble(const char* buff, double* validNumber);
void FgetsRemoveNewLine(char* string);
RideShare* findRideShare(RideShare* headRideSharePtr, char* stringPtr);
void RidersMode(RideShare* HeadRideSharePtr, const char* username, const char* password);
void getSurveyRatings(int surveyRatings[][CATEGORIES], const char* categoryNames[CATEGORIES], int surveyCount, size_t totalCategories, int min, int max);
void calculateSurveyAvg(const int surveyRatings[][CATEGORIES], double surveyAvg[CATEGORIES], int surveyCount, size_t totalCategories);
void displaySurveyAvg(const double surveyAvg[CATEGORIES], const char* categoryNames[CATEGORIES]);
void displaySurveyRatings(RideShare* headRideSharePtr, const char* categoryNames[CATEGORIES]);
void addRideShare(RideShare** headRideSharePtr);
void setUp(RideShare* rideShare);
void printBusinessSummary(RideShare* headRideSharePtr, const char* categoryNames[CATEGORIES]);
char getYorN();
void writeToFile(RideShare* headRideSharePtr);
void freeMemory(RideShare** HeadRideSharePtr);
int strcmpInsensitive(const char* str1, const char* str2);
void displayRideShareRates(RideShare* HeadRideSharePtr);

const char* surveyCategories[CATEGORIES] = { "Safety", "Cleanliness", "Comfort" };
RideShare rideShare = { 0 };

int main(void) {
	//Generates random seed to calculate a random number
	srand((unsigned int)time(NULL));

	if (LoginAdmin(CORRECT_ID, CORRECT_PASSCODE, SIZE_STRING, MAX_ATTEMPT) == true) {
		RideShare* headRideSharePtr = NULL;
		addRideShare(&headRideSharePtr);
		RidersMode(headRideSharePtr, CORRECT_ID, CORRECT_PASSCODE);
		printBusinessSummary(headRideSharePtr, surveyCategories);
		writeToFile(headRideSharePtr);
		freeMemory(&headRideSharePtr);
	}
	else {
		printf("%s", "You reached the max amount of login attempts");
	}
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
		FgetsRemoveNewLine(input);
		if (scanDouble(input, &number) == false) {
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

// This function is for validating the user input for miles
// it ensurse the value is indeed a number and is between 1-100
double getValidDoubleSentinel(int min, int max, int sentinel) {
	double miles = 0;
	bool validInput = false;
	char input[SIZE_STRING] = "";
	// iterates until validInput is changed to true
	while (!validInput) {
		FgetsRemoveNewLine(input);
		if (scanDouble(input, &miles) == false) {
			printf("%s", "Error: Invalid input. Please enter a number.\n");
			//while (getchar() != '\n');
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
bool scanDouble(const char* buffer, double* validNumber) {
	char* end;
	*validNumber = 0;
	double intTest = strtod(buffer, &end);
	bool returnedBool = false;

	if (end == buffer) {

	}
	else if ('\0' != *end) {

	}
	else if ((DBL_MIN == intTest || DBL_MAX == intTest) && ERANGE == errno) {

	}
	else if (intTest > INT_MAX) {

	}
	else if (intTest < INT_MIN) {

	}
	else {
		*validNumber = (double)intTest;
		returnedBool = true;
	}
	return returnedBool;
}

//Code for getting random time
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
	bool correctLogin = false;

	while (attempts < maxAttempts && !correctLogin) {
		// Get username from user
		printf("Enter your username: ");
		FgetsRemoveNewLine(username);

		if (strcmp(username, correctUsername) == 0) {
			// Username is correct, now ask for password
			printf("Enter your password: ");
			FgetsRemoveNewLine(pass);

			if (strcmp(pass, correctPass) == 0) {
				correctLogin = true; // Correct password
			}
			else {
				printf("Incorrect password.\n");
				attempts++;
			}
		}
		else {
			printf("Invalid username.\n");
			attempts++;
		}
	}

	return correctLogin; // Return the result of login attempt
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

	//Promting admin for baseFare
	printf("%s", "Enter the amount base fare: ");
	FgetsRemoveNewLine(input);
	while (!scanDouble(input, &number) || number > max || number < min) {
		printf("%s", "\nInvalid input. Enter base fare: ");
		FgetsRemoveNewLine(input);
	}
	rideShare->baseFare = number;

	//Promting admin for costPerMin
	printf("%s\n", "Enter the amount cost per minute: ");
	FgetsRemoveNewLine(input);
	while (!scanDouble(input, &number) || number > max || number < min) {
		printf("%s", "\nInvalid input. Enter cost per min: ");
		FgetsRemoveNewLine(input);
	}
	rideShare->costPerMin = number;

	//Promting admin for costPerMile
	printf("%s\n", "Enter the amount cost per mile: ");
	FgetsRemoveNewLine(input);
	while (!scanDouble(input, &number) || number > max || number < min) {
		printf("%s", "\nInvalid input. Enter cost per mile: ");
		FgetsRemoveNewLine(input);
	}
	rideShare->costPerMile = number;

	//Promting admin for minFlatRate
	printf("%s\n", "Enter the amount min flat rate: ");
	FgetsRemoveNewLine(input);
	while (!scanDouble(input, &number) || number > max || number < min) {
		printf("%s", "\nInvalid input. Enter min flat rate: ");
		FgetsRemoveNewLine(input);
	}
	rideShare->minFlatRate = number;


	//Prompting admin for org name
	printf("%s\n", "Enter orgnization name: ");
	FgetsRemoveNewLine(rideShare->organizationName);

	printf("%s%.2f\n", "Base fare: ", rideShare->baseFare);
	printf("%s%.2f\n", "Cost per minute: ", rideShare->costPerMin);
	printf("%s%.2f\n", "Cost per mile: ", rideShare->costPerMile);
	printf("%s%.2f\n", "Min flat rate: ", rideShare->minFlatRate);
	printf("%s%s\n", "Organization Name: ", rideShare->organizationName);
}

//Function to add rideshares to the linked list
void addRideShare(RideShare** headRideSharePtr) {
	bool another = true;
	do {
		RideShare* newRideShare = malloc(sizeof(RideShare));
		if (newRideShare != NULL) {
			setUp(newRideShare);
			newRideShare->nextRideSharePtr = NULL;
			RideShare* previousPtr = NULL;
			RideShare* currentPtr = *headRideSharePtr;

			while (currentPtr != NULL && currentPtr->nextRideSharePtr != NULL && strcmp(currentPtr->organizationName, currentPtr->nextRideSharePtr->organizationName) < 0) {
				previousPtr = currentPtr;
				currentPtr = currentPtr->nextRideSharePtr;
			}

			if (previousPtr == NULL) {
				*headRideSharePtr = newRideShare;
			}
			else {
				previousPtr->nextRideSharePtr = newRideShare;
			}
			newRideShare->nextRideSharePtr = currentPtr;
		}

		else {
			printf("No room in memory for new ride share.");
		}
		printf("%s", "Would you like to create another ride share? (y/n)");
		char YorN = getYorN();
		if (YorN == 'n' || YorN == 'N') {
			another = false;
		}
	} while (another != false);
}

//Function to store survey ratings in surveyRatings array
void getSurveyRatings(int surveyRatings[][CATEGORIES], const char* categoryNames[CATEGORIES], int surveyCount, size_t totalCategories, int min, int max) {
	int name = 0;
	for (size_t categories = 0; categories < totalCategories; categories++) {
		printf("%s%s%s%d%s%d%s", "Enter ", categoryNames[name], " rating(", min, "-", max, ")");
		surveyRatings[surveyCount][categories] = getValidDouble(min, max);
		name++;
	}
}

//Function to display current ratings
void displaySurveyRatings(RideShare* headRideSharePtr, const char* categoryNames[CATEGORIES]) {
	RideShare* currentPtr = headRideSharePtr;
	while (currentPtr != NULL) {
		printf("%s%s%s\n", "RideShare Organization ", currentPtr->organizationName, " Ratings");
		printf("%s\n", "Survey Ratings");
		if (currentPtr->surveyCount > 0) {
			for (int i = 0; i < CATEGORIES; i++) {
				printf("\t\t%s", categoryNames[i]);
			}
			puts("\n");
			for (int i = 0; i < currentPtr->surveyCount; i++) {
				printf("Survey #%d", i);
				for (int e = 0; e < CATEGORIES; e++) {
					printf("\t%d\t", currentPtr->rating[i][e]);
				}
				puts("\n\n");
			}
		}
		else {
			printf("%s", "No ratings currently\n\n");
		}
		currentPtr = currentPtr->nextRideSharePtr;
	}
}

//Function to calculate survey averages
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

//Function to match user input to a Rideshare
RideShare* findRideShare(RideShare* headRideSharePtr, char* stringPtr) {
	RideShare *currentPtr = headRideSharePtr;
	RideShare* correctPtr = NULL;
	while (currentPtr != NULL) {

		if (strcmpInsensitive(stringPtr, currentPtr->organizationName) == 0) {
			correctPtr = currentPtr;
			correctPtr->found = true;
			return correctPtr;
		}
		else {
			currentPtr = currentPtr->nextRideSharePtr;
		}

	}
}

//Function to display the surveyAvg
void displaySurveyAvg(const double surveyAvg[CATEGORIES], const char* categoryNames[CATEGORIES]) {
	printf("%s", "Survey Averages:\n");
	for (int i = 0; i < CATEGORIES; i++) {
		printf("\t\t%s", categoryNames[i]);
	}
	puts("\n");
	for (int i = 0; i < CATEGORIES; i++) {
		printf("\t\t%.2f", surveyAvg[i]);
	}
}

//Iterates through all rideshares and print their information 
void printBusinessSummary(RideShare* headRideSharePtr, const char* categoryNames[CATEGORIES]) {
	RideShare* currentPtr = headRideSharePtr;
	while (currentPtr != NULL) {
		calculateSurveyAvg(currentPtr->rating, currentPtr->surveyAvg, currentPtr->surveyCount, CATEGORIES);
		printf("%s : Business Summary\n", currentPtr->organizationName);
		if (currentPtr->totalRideCount != 0) {
			printf("%s\t%s\t%s\t%s\n", "Rider", "Number of Miles", "Number of Minutes", "Ride Fare Amount");
			printf("%.1f\t%.1f\t\t%d\t\t$%.2f\n", currentPtr->totalRideCount, currentPtr->totalMiles,
				currentPtr->totalMinutes, currentPtr->totalFare);
			if (currentPtr->surveyCount != 0) {
				printf("Survey Averages:\n");
				for (int i = 0; i < CATEGORIES; i++) {
					printf("\t%s", categoryNames[i]);
				}
				puts("\n");
				for (int i = 0; i < CATEGORIES; i++) {
					printf("\t%.2f", currentPtr->surveyAvg[i]);
				}
				puts("\n");
				puts("\n");
			}
		}
		else {
			printf("%s : There were no rides.\n", currentPtr->organizationName);
		}
		//iterates to next node
		currentPtr = currentPtr->nextRideSharePtr;  
	}
}

//Function to write all rideshare info to file located in C:/RideShare/
void writeToFile(RideShare* headRideSharePtr) {
	RideShare* current = headRideSharePtr;
	while (current != NULL) {
		FILE* filePtr;
		char path[SIZE_STRING] = FILE_PATH;
		strcat(path, current->organizationName);
		filePtr = fopen(path, "w");
		if (filePtr  == NULL) {
			printf("%s", "Failed to open file.");
		}
		else {
			fprintf(filePtr, "%s Business Summary: \n", current->organizationName);
			fprintf(filePtr, "%s\t%s\t%s\t%s\n", "Rider", "Number of Miles", "Number of Minutes", "Ride Fare Amount");
			fprintf(filePtr, "%.1f\t%.1f\t\t%d\t\t$%.2f\n", current->totalRideCount, current->totalMiles,
				current->totalMinutes, current->totalFare);
			for (int i = 0; i < CATEGORIES; i++) {
				fprintf(filePtr, "%s\t", surveyCategories[i]);
			}
			for (int i = 0; i < CATEGORIES; i++) {
				fprintf(filePtr, "%.2f\t", current->surveyAvg[i]);
			}
		}
		current = current->nextRideSharePtr;
	}
}

//Function for ride share
void RidersMode(RideShare* HeadRideSharePtr, const char* username, const char* password) {
	int minMiles = 1;
	int maxMiles = 100;

	bool endRider = false;

	while (!endRider) {
		displaySurveyRatings(HeadRideSharePtr, surveyCategories);
		displayRideShareRates(HeadRideSharePtr);
		puts("\n");
		printf("\n%s", "Type in ride share name: ");
		char input[SIZE_STRING] = { 0 };
		FgetsRemoveNewLine(input);
		RideShare* current = findRideShare(HeadRideSharePtr, input);
		if (current != NULL && current->found == true) {
			double baseFare = current->baseFare;
			double costPerMin = current->costPerMin;
			double costPerMile = current->costPerMile;
			double minFlatRate = current->minFlatRate;

			printf("%s%s%s", "\nWelcome to ", current->organizationName, " Ride Share. We can only "
				"provide services for rides from 1 to 100 miles.\n");

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
					current->totalRideCount += 1;
					int minRandomMinutes = MIN_RAND_MINUTES_FACTOR * miles;
					int maxRandomMinutes = MAX_RAND_MINUTES_FACTOR * miles;

					//calculating ride time and price total for customer
					int rideTime = calculateRandomNumber(minRandomMinutes, maxRandomMinutes);
					double rideTotal = calculateFare(baseFare, costPerMin, costPerMile, minFlatRate, miles, rideTime);

					//incrementing total values for business owner
					current->totalMiles += miles;
					current->totalMinutes += rideTime;
					current->totalFare += rideTotal;

					printf("\nCustomer Summary\n");
					printFare(current->totalRideCount, miles, rideTime, rideTotal);

					printf("%s", "Do you want to take survey? Y or N:");
					YorN = getYorN();
					if (YorN == 'Y' || YorN == 'y') {
						getSurveyRatings(current->rating, surveyCategories, current->surveyCount, CATEGORIES, MIN_RATING, MAX_RATING);
						current->surveyCount++;
						printf("%s\n", "Thanks for your feedback. Have a great day! ");
					}
					else {
						printf("%s\n", "Thanks for riding! Have a splendid day!");
					}
				}
			}
		}
		else {
			printf("%s\n", "No ride share match. Please re-enter.");
		}
	}
}

//Function that is very similar to strcmp just make it not case sensitive
int strcmpInsensitive(const char* str1, const char* str2) {
	char c1 = '0';
	char c2 = '0';
	do {

		//Iterates through the string and converts each character to lower case
		//using the tolower function inside of the ctype.h library
		c1 = tolower(*str1++);
		c2 = tolower(*str2++);
	
		//while the character stored in c1 and c2 are equal to each other and the first character is not null
		//
	} while (c1 == c2 && c1 != '\0');

	//Return the difference between the two chars
	return (unsigned char)c1 - (unsigned char)c2;
}

//Function to freeMemory we allocated in the heapp for ride shares
void freeMemory(RideShare** HeadRideSharePtr) {


	RideShare* currentPtr = *HeadRideSharePtr;
	RideShare* nextRideSharePtr = NULL;

	while (currentPtr != NULL)
	{
		nextRideSharePtr = currentPtr->nextRideSharePtr;
		free(currentPtr);
		currentPtr = nextRideSharePtr;
	}

	*HeadRideSharePtr = NULL;
}

void displayRideShareRates(RideShare* HeadRideSharePtr) {
	RideShare* currentPtr = HeadRideSharePtr;
	while (currentPtr != NULL) {
		printf("%s%s", currentPtr->organizationName, " Organization Rates");
		printf("Base Fare: %.2f", currentPtr->baseFare);
		printf("Cost Per Minute: %.2f", currentPtr->costPerMin);
		printf("Cost Per Mile: %.2f", currentPtr->costPerMile);
		printf("Minimum Flat Rate: %.2f", currentPtr->minFlatRate);
		currentPtr = currentPtr->nextRideSharePtr;
	}
}