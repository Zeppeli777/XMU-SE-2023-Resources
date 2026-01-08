#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int display1(char *string);
int display2(char *string);

int main (int argc,char **argv)
{
	char string[] = "Embedded Linux";

	display1 (string);
	display2 (string);

	return 0;
}

int display1 (char *string)
{
	printf ("The original string is %s \n", string);
}

int display2 (char *string1)
{
	char *string2;
	int size,i;

	size = strlen (string1);
	string2 = (char *) malloc (size+1);
	for (i = 0; i < size; i++)
		string2[size - i - 1] = string1[i];
	string2[size] = '\0';
	printf("The string afterward is %s\n",string2);
	free(string2);
}

