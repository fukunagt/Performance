#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mdperf.h"

int main
(
	int argc,
	char **argv
)
{
	char label[LABEL_LEN];
	char method[LABEL_LEN];
	char path[PATH_LEN];
	int times;
	int interval;
	long threshold;
	
	/* initialize */
	memset(label, 0, sizeof(label));
	memset(path, 0, sizeof(label));
	threshold = 0;
	interval = 60;

	if (argv[1], "alert")
	{
		/* TODO: NULL check */
		strcpy(label, argv[2]);
		threshold = atol(argv[3]);
		times = atoi(argv[4]);
		interval = atoi(argv[5]);
		strcpy(method, argv[6]);
		strcpy(path, argv[7]);

		/* DEBUG */
		printf("label    : %s\n", label);
		printf("threshold: %d\n", threshold);
		printf("times    : %d\n", times);
		printf("interval : %d\n", interval);
		printf("path     : %s\n", path);
		sendalert(label, threshold, times, interval, method, path);	
	}
	else
	{
		printf("%d: Invalid parameter.\n", __LINE__);
		return 1;
	}
	
	return 0;
}

int
sendalert
(
	char *label,
	int threshold,
	int times,
	int interval,
	int method,
	char *path
)
{
	char *token = NULL;
	char tmp[ROW_LEN];
	int column;
	int row;
	int count;
	int send;
	int i;
	long cur;
	FILE *fp;

	/* initialize */
	memset(tmp, 0, sizeof(tmp));
	column = 0;
	row = 0;
	count = 0;
	fp = NULL;

	/* find the column */
	fp = fopen(path, "r");	
	if (fp == NULL)
	{
		printf("%d: Failed to open file.\n", __LINE__);
		return 1;
	}
	fgets(tmp, sizeof(tmp), fp);
	token = strtok(tmp, "\"");
	i = 0;
	while (token != NULL)
	{
		if (!strcmp(token, label))
		{
			column = i;
			printf("column: %d\n", column);
			break;
		}
		token = strtok(NULL, "\"");
		i++;
	}
	fclose(fp);

	while (1)
	{
		fp = fopen(path, "r");	
		/* find the last row */
		i = 0;
		fseek(fp, 0, SEEK_SET);
		while (fgets(tmp, sizeof(tmp), fp) != NULL)
		{
			i++;
		}
		row = i;
		printf("row: %d\n", row);

		/* find the column on the last row */
		fseek(fp, 0, SEEK_SET);
		for (i = 0; i < row - 1; i++)
		{
			fgets(tmp, sizeof(tmp), fp);
		}
		token = strtok(tmp, "\"");
		if (!strcmp(token, "Date"))
		{
			/* there are no data yet */
			fclose(fp);
			sleep(interval);
			continue;	
		}
		for (i = 0; i < column; i++)
		{
			token = strtok(NULL, "\"");
		}
		printf("token: %s\n", token);
		cur = atol(token);
		if (count == 0 && cur < threshold)
		{
			count = 0;
			send = 0;
		}
		if (cur >= threshold)
		{
			count++;	
		}
		if (count >= times)
		{
			if (send == 0)
			{
				send = 1;
				printf("ERROR!!\n");
			}
		}
		fclose(fp);
		sleep(interval);
	}
	return 0;
}
