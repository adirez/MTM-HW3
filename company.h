//
// Created by Shahak on 06/06/2017.
//

#ifndef ESCAPETECHNION_COMPANY_H
#define ESCAPETECHNION_COMPANY_H

#include "mtm_ex3.h"
#include "set.h"
#include "room.h"

/**
 * Company will be implemented in this file as a set
 */
typedef struct Company_t *Company;

/**
 * a type used to return error codes related to Company
 */
typedef enum {
    COMPANY_SUCCESS,
    COMPANY_ROOM_ALREADY_EXISTS,
    COMPANY_ROOM_DOES_NOT_EXIST,
    COMPANY_INVALID_PARAMETER,
    COMPANY_RESERVATION_EXISTS,
    COMPANY_OUT_OF_MEMORY
} CompanyErrorCode;

/**
 * creates a new company according to the relevant input parameters and returns
 * the relevant result code.
 * @param company_email - the email address of the company which will be used
 *        to identify the company.
 * @param Faculty - the name of the faculty that the company is listed under
 * @param CompanyError - the result of the creation
 * @return Company - a pointer to the successfully created company if everything
 *         went ok
 *         NULL - if the allocation was not successful
 */
Company companyCreate(char *company_email, TechnionFaculty Faculty,
                      CompanyErrorCode *CompanyError);

/**
 * destroys a company and all of it's components. releases all relevant
 * allocated memory.
 * @param company - a pointer to the relevant company to be destroyed
 * @return COMPANY_SUCCESS - the room was removed successfuly
 *         COMPANY_INVALID_PARAMETER - one of the parameters was invalid
 *         COMPANY_ROOM_DOES_NOT_EXIST - the asked room wasn't found
 *         COMPANY_RESERVATION_EXISTS - one of the rooms has an existing
 *         reservation
 */
CompanyErrorCode companyDestroy(Company company);

/**
 * creates a new room according to the relevant input parameters and returns
 * the relevant result code. calculates the working hours of the room and if
 * found valid, calls roomCreate to add the room to the set.
 * @param company - the company to which the room needs to be added
 * @param company_email - the email of the company the rooms will be listed
 *        under
 * @param room_id - the id of the room
 * @param price - entry price per person (must be a multiple of 4)
 * @param num_ppl - recommended number of people for this specific room
 * @param working_hrs - start time and close time of the room
 * @param difficulty - the difficulty level of the room
 * @return COMPANY_SUCCESS - the function finished with no errors
 *         COMPANY_INVALID_PARAMETER - the function received an invalid
 *         parameter as an input
 *         COMPANY_MEMORY_PROBLEM - the room allocation was not successful
 *         COMPANY_RESERVATION_EXISTS - one of the rooms has an existing
 *         reservation
 */
CompanyErrorCode companyAddRoom(Company company, char *company_email,
                                int room_id, int price, int num_ppl,
                                int opening_time, int closing_time,
                                int difficulty);

/**
 * removes a room from the company set
 * @param company - the company from which the room needs to be removed
 * @param room - the room which needs to be removed from the company
 * @return COMPANY_SUCCESS - the room was removed successfuly
 *         COMPANY_ROOM_DOES_NOT_EXIST - the room does not exist in the company
 *         COMPANY_INVALID_PARAMETER - one of the parameters was invalid
 *         COMPANY_RESERVATION_EXISTS - one of the rooms has an existing
 *         reservation
 */
CompanyErrorCode companyRemoveRoom(Company company, Room room);

/**
 * a comparison of two companies by email
 * @param company_1 - the first company
 * @param company_2 - the second company
 * @return 0 - if both of the companies emails are identical
 * @return NEGATIVE NUMBER - if the first company's email is lexicographically
 *         smaller than the second company's email
 * @return POSITIVE NUMBER - if the first company's email is lexicographically
 *          bigger than the second company's email
 */
int companyCompareElements(SetElement company_1, SetElement company_2);

/**
 * frees all relevant allocated memory of a specific company element
 * @param company - the company  to be freed
 */
void companyFreeElement(SetElement company);

/**
 * receives a source company element and copies it's data into a newly created
 * company element
 * @param src_company - the source company that needs to be copied
 * @return pointer to the new allocated company or NULL if the allocation failed
 */
SetElement companyCopyElement(SetElement src_company);



#endif //ESCAPETECHNION_COMPANY_H
