#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/file.h>

int main(void)
{
	pid_t child; char buffer[6]; // For file input 

	int firstNum = 0; // Last number the file should be at (based on what is in the file)
	FILE *fp = fopen("input.txt", "r");
	if(fp) // Check and instantiated variables related to file
	{
		fscanf(fp, "%s", buffer);
		firstNum = atoi(buffer) + 99;
		fclose(fp);
	}
	else
	{
		fp = fopen("input.txt", "w");
		fprintf(fp, "%d", 1);
		firstNum = 100;
		fclose(fp);
	}

	printf("**********PLEASE ALLOW PROGRAM TO LOAD**********\n"); // flag for the user

	if((child = fork()) == -1) // fork
	{
		perror("fork");
		exit(1);
	}

	while(firstNum) // Keep running program until flagged with a 0 (number will never instantiate at 0)
	{
		if (child == 0) // Child
		{
			FILE *fp = fopen("input.txt", "r");

			fscanf(fp, "%s", buffer);

			int seqN = atoi(buffer); // Read number from file

			if(seqN > firstNum) // Flag if the files number is greater than the limit
				firstNum = 0;

			fclose(fp);

			if (seqN % 2 == 0) // Only edit or manipulate if even
			{
				seqN++; // Increment files value

				printf("\tN Value %d - Child PID %d\n", seqN, getpid());

				fp=fopen("input.txt", "w+");

				fprintf(fp, "%d", seqN); // Save new value to file

				fflush(fp);  // Flush File Buffer

				fclose(fp);

			}

			sleep(1); // Wait a few seconds to allow other process to run
		}
		else
		{
		  FILE *fp = fopen("input.txt", "r");

		  fscanf(fp, "%s", buffer);

		  int seqN = atoi(buffer);

			fclose(fp);

			if(seqN > firstNum)
						firstNum = 0;

			if (seqN % 2 != 0) // Only edit or manipulate if even
			{
				seqN++;

				printf("N Value %d - Parent PID %d\n", seqN, getpid());

				fp=fopen("input.txt", "w+");

				fprintf(fp, "%d", seqN);

				fflush(fp);

				fclose(fp);

			}
			sleep(1); // Wait a few seconds to allow other process to run
		}

	}
}
