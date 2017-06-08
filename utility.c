//
// Created by Shahak on 06/06/2017.
//

#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#include "mtm_ex3.h"
#include "utility.h"

#define INVALID_PARAMETER -1
#define AT_SYMBOL '@'
#define MIN_NUMBER 48 //represents '0' in ASCII
#define MAX_NUMBER 57 //represents '9' in ASCII
#define MIN_HOUR 0
#define MAX_HOUR 24
#define HOURS_STR_LEN 5 //according to the format "HH-HH"


bool isEmailValid(char *email) {
    if (NULL == email) {
        return false;
    }

    int counter = 0;
    char *tmp_ptr = email;

    while (tmp_ptr) {
        if (*tmp_ptr == AT_SYMBOL) {
            counter++;
        }
        tmp_ptr++;
    }
    if (counter == 1) {
        return true;
    }
    return false;
}

bool isFacultyValid (TechnionFaculty Faculty) {
    if (Faculty < CIVIL_ENGINEERING || Faculty > UNKNOWN) {
        return false;
    }

    return true;
}

bool getHoursFromStr (char* hours_str, int *opening_time, int* closing_time) {
    int tmp_opening = 0, tmp_closing = 0;
    if (NULL == hours_str || strlen(hours_str) != HOURS_STR_LEN) {
        return false;
    }
    char tmp_char = hours_str[0];
    if (tmp_char < MIN_NUMBER || tmp_char > MAX_NUMBER) {
        return false;
    }
    tmp_opening = 10 * atoi(&tmp_char);
    tmp_char = hours_str[1];
    if (tmp_char < MIN_NUMBER || tmp_char > MAX_NUMBER) {
        return false;
    }
    tmp_opening += atoi(&tmp_char);

    tmp_char = hours_str[3];
    if (tmp_char < MIN_NUMBER || tmp_char > MAX_NUMBER) {
        return false;
    }
    tmp_closing = 10 * atoi(&tmp_char);
    tmp_char = hours_str[4];
    if (tmp_char < MIN_NUMBER || tmp_char > MAX_NUMBER) {
        return false;
    }
    tmp_closing += atoi(&tmp_char);

    if (tmp_opening < MIN_HOUR || tmp_opening > MAX_HOUR ||
        tmp_closing < MIN_HOUR || tmp_closing > MAX_HOUR ||
        tmp_opening > tmp_closing) {
        return false;
    }

    *opening_time = tmp_opening;
    *closing_time = tmp_closing;
    return true;
}
