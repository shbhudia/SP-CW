// @author Shayam N. Bhudia
// @guid 2394138b
// Systems Programming - Coursework 1

#include "date.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// define the date structure
struct date {
    int day;  // day (2 digits)
    int month;  // month (2 digits)
    int year;  // year (4 digits)
};

/*
 * date_create creates a Date structure from `datestr`
 * `datestr' is expected to be of the form "dd/mm/yyyy"
 * returns pointer to Date structure if successful,
 *         NULL if not (syntax error)
 */
Date *date_create(char *datestr) {

	int valid_date_format = 0;

	if (datestr == NULL) {
		valid_date_format = 1;
	}

	if (valid_date_format == 0) {
		Date *date = (Date *)malloc(sizeof(Date));

		// convert the date string into integer values
		char dd_str[3];
		char mm_str[3];
		char yyyy_str[5];

		// slice the string date into day, month and year
		memcpy(dd_str, &datestr[0], 2);
		memcpy(mm_str, &datestr[3], 2);
		memcpy(yyyy_str, &datestr[6], 4);

		// convert string into integer
		int dd, mm, yyyy;
		sscanf(dd_str, "%d", &dd);
		sscanf(mm_str, "%d", &mm);
		sscanf(yyyy_str, "%d", &yyyy);

		// date validation
		if (dd < 1 || dd > 31) {
			return NULL;
		}
		if (mm < 1 || mm > 12) {
			return NULL;
		}
		if (yyyy < 1983 || yyyy > 2020) { // birth of the internet and the current year (hardcoded for this assignment)
			return NULL;
		}

		// assign values to the structure
		date->day = dd;
		date->month = mm;
		date->year = yyyy;

		return date;

	} else {
		return NULL;
	}

	
}

/*
 * date_duplicate creates a duplicate of `d'
 * returns pointer to new Date structure if successful,
 *         NULL if not (memory allocation failure)
 */
Date *date_duplicate(Date *d) {
	Date *date_cpy = (Date *)malloc(sizeof(Date));
	date_cpy->day = d->day;
	date_cpy->month = d->month;
	date_cpy->year = d->year;
	return date_cpy;
}

/*
 * date_compare compares two dates, returning <0, 0, >0 if
 * date1<date2, date1==date2, date1>date2, respectively
 */
int date_compare(Date *date1, Date *date2) {
	if (date1->year > date2->year) {
		return 1;
	} else if (date1->year < date2->year) {
		return -1;
	} else {
		if (date1->month > date2->month) {
			return 1;
		} else if (date1->month < date2->month) {
			return -1;
		} else {
			if (date1->day > date2->day) {
				return 1;
			} else if (date1->day < date2->day) {
				return -1;
			} else {
				return 0;
			}
		}
	}
}

/*
 * date_destroy returns any storage associated with `d' to the system
 */
void date_destroy(Date *d) {
	free(d);
}