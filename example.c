#include <stdio.h>

#define SIZE 5

int main(void) {
	int a[SIZE] = { 1,3,5,7,0 };
	printf("%p", a);
	printf("%p", a[0]);

}