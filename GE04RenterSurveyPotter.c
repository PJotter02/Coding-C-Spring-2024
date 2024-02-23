#include <stdio.h>
#define ROWS 7
#define CATEGORIES 3

void printCategories(const char* categories[], size_t totalCategories);
void getRatings(int resultSurvey[][CATEGORIES], const char* surveyCategories[], int* surveyCount);
void printSurveyResults(int rideShareSurvey[][CATEGORIES], int surveyCount);


int main(void) {
    int rideShareSurvey[ROWS][CATEGORIES] = { {0} };
	int categoryAverage[CATEGORIES] = { 0 };
	const char* surveyCategories[CATEGORIES] = { "Safety", "Cleanliness", "Comfort" };
    int surveyCount = 0;

    getRatings(rideShareSurvey, surveyCategories, &surveyCount);
    printSurveyResults(rideShareSurvey, surveyCount);

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
void getRatings(int resultSurvey[][CATEGORIES],const char* surveyCategories[], int *surveyCount) {
    printf("Enter ratings for each category (1-5) for the renter:\n");

    for (int i = 0; i < CATEGORIES; i++) {
        printf("%s: ", surveyCategories[i]);
        scanf("%d", &resultSurvey[*surveyCount][i]);
    }

    printf("Ratings recorded for renter %d\n", *surveyCount + 1);
}

void printSurveyResults(int rideShareSurvey[][CATEGORIES], int surveyCount) {
    printf("Survey Results:\n");
    printf("\tSafety\tCleanliness\tComfort\n");

    for (int i = 0; i < surveyCount; i++) {
        printf("%d\t", i + 1);
        for (int j = 0; j < CATEGORIES; j++) {
            printf("survey #%d\t%d\t" ,i, rideShareSurvey[i][j]);
        }
        printf("\n");
    }
}