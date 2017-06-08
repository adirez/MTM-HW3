//
// Created by Shahak on 06/06/2017.
//

/**
 * receives an email and checks if it's valid
 * @param email - the email address to be checked
 * @return true - contains only one '@'
 *         false - containes more / less than one '@' or NULL
 */
bool isEmailValid(char *email);

/**
 * receives a faculty name and checks if it's valid
 * @param Faculty - the faculty name to be checked
 * @return true - if it's mentioned in the listed faculties
 *         false - it is not mentioned in the listed faculties
 */
bool isFacultyValid (TechnionFaculty Faculty);

/**
 * receives a string representing operation hours in the format "hh-hh" and
 * returns the opening time and the closing time as integers
 * @param hours_str - the original string
 * @param opening_time - the output of the calculated opening time
 * @param closing_time - the output of the calculated closing time
 * @return true - if the string is valid
 *         false - if the string contains illegal chars, too short / long or if
 *         it is NULL
 */
bool getHoursFromStr (char *hours_str, int *opening_time, int* closing_time);

/**
 * receives a string representing a day and an hour in the format "dd-hh" and
 * returns the day and the hour as integers
 * @param src_str - the source string
 * @param day - the output of the calculated day
 * @param hour - the output of the calculated hour
 * @return true - if the string is valid
 *         false - if the string contains illegal chars, too short / long or if
 *         it is NULL
 */
bool GetDayAndHourFromStr (char *src_str, int *day, int *hour);
