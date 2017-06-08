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
#define HYPHEN '-'
#define MIN_NUMBER 48 //represents '0' in ASCII
#define MAX_NUMBER 57 //represents '9' in ASCII
#define MIN_HOUR 0
#define MAX_HOUR 24
#define HOURS_STR_LEN 5 //according to the format "HH-HH"
#define DAY_HOUR_MIN_LEN 3 //according to the format "D-H"

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
    if (Faculty < CIVIL_ENGINEERING || Faculty >= UNKNOWN) {
        return false;
    }

    return true;
}

bool getHoursFromStr (char *hours_str, int *opening_time, int* closing_time) {
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

bool GetDayAndHourFromStr (char *src_str, int *day, int *hour) {
    int tmp_day = 0, tmp_hour = 0, idx = 0, len = 0;
    if (NULL == src_str || strlen(src_str) < DAY_HOUR_MIN_LEN) {
        return false;
    }
    len = (int)strlen(src_str);
    char tmp_char = src_str[0];
    //TODO check if we should return false if its a number starting with 0
    if (tmp_char < MIN_NUMBER || tmp_char > MAX_NUMBER) {
        return false;
    }
    while (1) {
        if (idx >= len) return false;
        tmp_char = src_str[idx];
        if (tmp_char == HYPHEN) break;
        if (tmp_char < MIN_NUMBER || tmp_char > MAX_NUMBER) {
            return false;
        }
        int tmp_calc = atoi(&tmp_char);
        tmp_day *= 10;
        tmp_day += tmp_calc;

        idx++;
    }

    //if we got here, curr char is '-'
    if (++idx >= len) return false;

    tmp_char = src_str[idx];
    if (tmp_char < MIN_NUMBER || tmp_char > MAX_NUMBER) {
        return false;
    }
    int tmp_calc = atoi(&tmp_char);
    tmp_hour = tmp_calc;
    idx++;
    if (idx < len) {
        tmp_char = src_str[idx];
        if (tmp_char < MIN_NUMBER || tmp_char > MAX_NUMBER) {
            return false;
        }
        tmp_calc = atoi(&tmp_char);
        tmp_hour *= 10;
        tmp_hour += tmp_calc;

        idx++;
        if (idx < len) {
            //too many characters for an hour
            return false;
        }
    }
    if (tmp_hour < MIN_HOUR || tmp_hour >= MAX_HOUR) {
        return false;
    }

    *day = tmp_day;
    *hour = tmp_hour;
    return true;
}
