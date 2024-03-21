#include <stdio.h>
#define ROWS 7
#define CATEGORIES 3

void printCategories(const char* categories[], size_t totalCategories);
void getRatings(int rideShareSurvey[][CATEGORIES], const char* surveyCategories[], int* surveyCount, size_t categories);
void printSurveyResults(const int rideShareSurvey[][CATEGORIES], int surveyCount);
void calculateCategoryAverages(const int rideShareSurvey[][CATEGORIES], double categoryAvg[CATEGORIES], int surveyCount, size_t totalCategories);
void printCategoryData(const double categoryAvg[CATEGORIES], const char* surveyCategories[], int surveyCount);

int main(void) {
    int rideShareSurvey[ROWS][CATEGORIES] = { {0} };
	double categoryAverage[CATEGORIES] = { 0 };
	const char* surveyCategories[CATEGORIES] = { "Safety", "Cleanliness", "Comfort" };
    int surveyCount = 0;


    getRatings(rideShareSurvey, surveyCategories, &surveyCount, CATEGORIES);
    getRatings(rideShareSurvey, surveyCategories, &surveyCount, CATEGORIES);
    getRatings(rideShareSurvey, surveyCategories, &surveyCount, CATEGORIES);
    printSurveyResults(rideShareSurvey, surveyCount);
    calculateCategoryAverages(rideShareSurvey, categoryAverage, surveyCount, CATEGORIES);
    printCategoryData(categoryAverage, surveyCategories, surveyCount);
   

	return 0;
}

void printCategories(const char* categories[], size_t totalCategories)
{
    //loop to display each category horizontally
    printf("%s", "Rating Categories:\t");
    for (size_t surveyCategory = 0; surveyCategory < totalCategories; ++surveyCategory)
    {
        printf("\t%zu.%s\t", surveyCategory + 1, categories[surveyCategory]);
    }
    puts(""); // start new line of output
}

// function to fill rideShareSurvey in main

void getRatings(int rideShareSurvey[][CATEGORIES],const char* surveyCategories[], int *surveyCount, size_t totalCategories) {
    printf("Enter rating for each category (1-5): \n");

    // size_t iterates through an array in bit sized chunks
    for (size_t categorie = 0; categorie < totalCategories; categorie++) {
        printf("%s: ", surveyCategories[categorie]);
        scanf("%d", &rideShareSurvey[*surveyCount][categorie]);
    }
    *surveyCount += 1;
}

// function to print rideShareSurvey 2d array in main
void printSurveyResults(const int rideShareSurvey[][CATEGORIES], int surveyCount) {
    printf("Survey Results:\n");
    printf("\tSafety\tCleanliness\tComfort\n");
    for (int i = 0; i < surveyCount; i++) {
        printf("Survey #%d", i);
        for (int e = 0; e < CATEGORIES; e++) {
            printf("\t%d\t", rideShareSurvey[i][e]);
        }
        printf("\n");
    }
}

// function to calculate averages of the 3 categories and storing in a array of doubles
void calculateCategoryAverages(const int rideShareSurvey[][CATEGORIES], double categoryAvg[CATEGORIES], int surveyCount, size_t totalCategories) {
    // size_t iterates through an array in bit sized chunks
    for (size_t categories = 0; categories < totalCategories; categories++) {
        double average = 0;
        for (int i = 0; i < surveyCount; i++) {
            average += rideShareSurvey[i][categories];
        }
        categoryAvg[categories] = average / surveyCount;
    }
}

// function to print the array of category averages
void printCategoryData(const double categoryAvg[CATEGORIES], const char* surveyCategories[], int surveyCount) {
    printf("%s\n", "Averages: ");
    for (int i = 0; i < surveyCount; i++) {
        printf("%s: %.3lf\n", surveyCategories[i], categoryAvg[i]);
    }
}