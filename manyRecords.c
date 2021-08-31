/*
 * File: manyRecords.c
 * Author:Qiwei Li
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_CHAR	15
#define SCANF_FMT	"%d %d %15s"

struct record {
	int	foo;
	int	bar;
	char	baz[MAX_CHAR + 1];
};

void printRecord(struct record *r)
{
	printf("%d %d %s\n", r->foo, r->bar, r->baz);
}

int main(int argc, char *argv[])
{
	struct record *records;
	int capacity = 1, nr = 0;
	char endstr[4];
	int idx;
	int ret;

	/* Initial the records with size as 1 */
	records = (struct record *)malloc(sizeof(struct record) * capacity);
	if (!records) {
		printf("Initial malloc() failed\n");
		return 1;
	}

	while ((ret = scanf(SCANF_FMT, &records[nr].foo,
					&records[nr].bar,
					records[nr].baz)) == 3) {
		struct record *tmp;

		/* Successfully scan a new record */
		nr++;

		/* Check whether need to extend the array */
		if (nr < capacity)
			continue;

		/* Print before extending */
		printf("Need to extend the array; the buffer is full (with %d records)\n", nr);
		printf("  Before extending the array, element 0 is: ");
		printRecord(records);

		/* Extend the array */
		tmp = (struct record *)malloc(sizeof(struct record) * (capacity * 2));
		if (!tmp) {
			printf("Could not extend the array\n");
			break;
		}
		memcpy(tmp, records, sizeof(struct record) * capacity);
		free(records);
		records = tmp;
		capacity *= 2;

		/* Print after extending */
		printf("  After  extending the array, element 0 is: ");
		printRecord(records);
	}

	/* Check scanf return value */
	if (ret != 0) {
		fprintf(stderr, "ERROR: scanf() rc=%d\n", ret);
		ret = 1;
		goto out;
	}

	printf("scanf() returned 0, so now we'll check to see if the string is \"END\"...\n");
	/* Need not to check scanf here since last scanf returns 0 */
	scanf("%3s", endstr);
	if (strcmp(endstr, "END")) {
		fprintf(stderr, "ERROR: the string was not \"END\".  Ending the program!\n");
		ret = 1;
		goto out;
	} else
		printf("...\"END\" found.  Will now read the indices...\n");

	while ((ret = scanf("%d", &idx)) == 1) {
		printf("[%d] ", idx);
		if (idx < 0 || idx >= nr)
			printf("<invalid index>\n");
		else
			printRecord(&records[idx]);
	}

	if (ret != EOF) {
		fprintf(stderr, "ERROR: scanf() rc=%d\n", ret);
		ret = 1;
		goto out;
	} else
		ret = 0;

out:
	/* Always free records when return */
	if (records)
		free(records);
	return ret;
}
