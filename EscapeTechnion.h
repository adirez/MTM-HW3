//
// Created by Shahak on 07/06/2017.
//

#ifndef HW3_ESCAPETECHNION_H
#define HW3_ESCAPETECHNION_H

#include "company.h"
#include "room.h"
#include "escaper.h"
#include "reservation.h"

/**
 * EscapeTechnion will be implemented in this flie.
 */
typedef struct EscapeTechnion_t *EscapeTechnion;

/**
 * creates a member of EscapeTechnion type, including all internal fields of
 * companies, rooms, faculties and reservations with all relevant allocations.
 * @param EscapeTechnionError - a type to hold the result of the function
 * @return a pointer to the allocated system or NULL if an error was found
 */
EscapeTechnion escapeTechnionCreate(MtmErrorCode *EscapeTechnionError);

/**
 * receives an escapeTechnion type and destroys it, including releasing all
 * allocated memory from internal fields
 * @param escapeTechnion - the type to be destroyed
 * @return MTM_SUCCESS - if the function worked ok
 *         MTM_NULL_PARAMETER - if the argument was NULL
 */
MtmErrorCode escapeTechnionDestroy(EscapeTechnion escapeTechnion);

/**
 * receives an escapeTechnion type system and details of a company and creates
 * a new company with suitable details within the system
 * @param escapeTechnion - the system to create the company in
 * @param email - the email of the company to be created
 * @param Faculty - the faculty of the company to be created
 * @return MTM_SUCCESS - if the function succeeded
 *         MTM_INVALID_PARAMETER - if one of the parameters was incompatible
 *         MTM_MEMORY_PROBLEM - if the allocations failed
 *         MTM_EMAIL_ALREADY_EXISTS - if the email is already in use
 */
MtmErrorCode escapeTechnionAddCompany(EscapeTechnion escapeTechnion,
                                      char *email, TechnionFaculty Faculty);







#endif //HW3_ESCAPETECHNION_H
