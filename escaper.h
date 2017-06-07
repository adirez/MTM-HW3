//
// Created by Shahak on 06/06/2017.
//

#ifndef HW3_ESCAPER_H
#define HW3_ESCAPER_H

#include "mtm_ex3.h"
#include "set.h"

/**
 * Escaper will be implemented in this file as a set
 */
typedef struct Escaper_t *Escaper;

/**
 * a type used to return error codes related to Escaper
 */
typedef enum {
    ESCAPER_SUCCESS,
    ESCAPER_INVALID_PARAMETER,
    ESCAPER_EMAIL_DOES_NOT_EXIST,
    ESCAPER_OUT_OF_MEMORY
} EscaperErrorCode;

/**
 * creates a new escaper client according to the relevant input parameters and
 * returns the relevant result code.
 * @param escaper_email - the email of the escaper to be created
 * @param Faculty - the Faculty the escaper is listed in
 * @param skill_level - the expertise of the escaper
 * @param EscaperError - the result of the function
 * @return Escaper - a pointer to the successfully created escaper if everything
 *         went ok
           NULL - if the allocation was not successful
 */
Escaper escaperCreate(char *escaper_email, TechnionFaculty faculty,
                      int skill_level, EscaperErrorCode *EscaperError);

/**
 * destroys an escaper and and releases all relevant allocated memory.
 * @param escaper - a pointer to the relevant escaper to be destroyed
 * @return ESCAPER_SUCCESS - the escaper was removed successfuly
 *         ESCAPER_INVALID_PARAMETER - one of the parameters is invalid
 *         ESCAPER_EMAIL_DOES_NOT_EXIST - the given email wasn't recognized
 */
EscaperErrorCode escaperDestroy(Escaper escaper);

/**
 * a comparison of two escapers by email
 * @param escaper_1 - the first escaper
 * @param escaper_2 - the second escaper
 * @return 0 - if both of the escapers' emails are identical
 * @return NEGATIVE NUMBER - if the first escaper's email is lexicographically
 *         smaller than the second escaper's email
 * @return POSITIVE NUMBER - if the first escaper's email is lexicographically
 *          bigger than the second escaper's email
 */
int escaperCompareElements(SetElement escaper_1, SetElement escaper_2);

/**
 * frees all relevant allocated memory of a specific escaper element
 * @param escaper - the escaper to be freed
 * @return COMPANY_SUCCESS - the function went with no errors
 *         ESCAPER_INVALID_PARAMETER - one of the parameters was invalid
 *         ESCAPER_EMAIL_DOES_NOT_EXIST - the given email wasn't recognized
 */
void escaperFreeElement(SetElement escaper);

/**
 * receives a source escaper element and copies it's data into a newly created
 * escaper element
 * @param src_escaper - the source escaper that needs to be copied
 * @return pointer to the new allocated escaper or NULL if the allocation failed
 */
SetElement escaperCopyElement(SetElement src_escaper);


#endif //HW3_ESCAPER_H
