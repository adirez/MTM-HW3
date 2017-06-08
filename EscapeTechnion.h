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













#endif //HW3_ESCAPETECHNION_H
