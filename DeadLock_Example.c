#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

#include <sys/file.h>

int main(void)
{
	pid_t child1;
	pid_t child2;

	child1 = fork();

	int lock; // Flag for file lock

	/*
		Process 1 will first attempt to access file A, then file B.
		If file is bieng used by another process then wait.

		Continue on..
		Process 1 -> File A, File B
		Process 2 -> File B, File C
		Process 3 -> File C, File A
	*/

	if(child1 == 0) // Process 2
	{
		printf("The Process 1(%d)\n", getpid()); // Inform user
			printf("\tProcess 1 requesting access to File A...\n");

			FILE *fp = fopen("File A.txt", "a"); // Attempt to open file A
			lock = flock(fileno(fp), LOCK_EX);	// If file is successfully open, lock it

			if(fp) // If successful, inform user
			{
				printf("\tProcess 1 has access to File A\n");
				fprintf(fp, "Property of Process 1");

			}
			else //If uncessful in trying to open, wait for file to be freed - inform user
			{
				printf("\tProcess 1 unable to access File A, it in use by other processes!\n");
				printf("\tProcess 1 will wait until the resource is free to use\n");
			}

			sleep(2); // Wait for other processes

			// CODING IS ALL SIMILAR AFTERWORDS

			printf("\tProcess 1 requesting access to File B...\n");

			fp = fopen("File B.txt", "a");
			lock = flock(fileno(fp), LOCK_EX);

			if(fp)
			{
				printf("\tProcess 1 has access to File B\n");
				fprintf(fp, "Property of Process 1");

			}
			else
			{
				printf("\tProcess 1 unable to access File B, it in use by other processes!\n");
				printf("\tProcess 1 will wait until the resource is free to use\n");
			}
	}
	else
	{
		child2 = fork();

		if(child2 == 0)
		{
			printf("The Process 2(%d)\n", getpid());
				printf("\tProcess 2 requesting access to File B...\n");

				FILE *fp = fopen("File B.txt", "a");
				lock = flock(fileno(fp), LOCK_EX);

				if(fp)
				{
					printf("\tProcess 2 has access to File B\n");
					fprintf(fp, "Property of Process 2");

				}
				else
				{
					printf("\tProcess 2 unable to access File B, it in use by other processes!\n");
					printf("\tProcess 2 will wait until the resource is free to use\n");
				}

				sleep(1);

				printf("\tProcess 2 requesting access to File C...\n");

				fp = fopen("File C.txt", "a");
				lock = flock(fileno(fp), LOCK_EX);

				if(fp)
				{
					printf("\tProcess 2 has access to File C\n");
					fprintf(fp, "Property of Process 2");

				}
				else
				{
					printf("\tProcess 2 unable to access File C, it in use by other processes!\n");
					printf("\tProcess 2 will wait until the resource is free to use\n");
				}
		}
		else
		{
			printf("The Process 3(%d)\n", getpid());
				printf("\tProcess 3 requesting access to File C...\n");

				FILE *fp = fopen("File C.txt", "a");
				lock = flock(fileno(fp), LOCK_EX);

				if(fp)
				{
					printf("\tProcess 3 has access to File C\n");
					fprintf(fp, "Property of Process 3");
				}
				else
				{
					printf("\tProcess 3 unable to access File C, it in use by other processes!\n");
					printf("\tProcess 3 will wait until the resource is free to use\n");
				}

				sleep(3);

				printf("\tProcess 3 requesting access to File A...\n");

				fp = fopen("File A.txt", "a");
				lock = flock(fileno(fp), LOCK_EX);

				if(fp)
				{
					printf("\tProcess 3 has access to File A\n");
					fprintf(fp, "Property of Process 3");
				}
				else
				{
					printf("\tProcess 3 unable to access File A, it in use by other processes!\n");
					printf("\tProcess 3 will wait until the resource is free to use\n");
				}

		}
	}

	return 0;
}
