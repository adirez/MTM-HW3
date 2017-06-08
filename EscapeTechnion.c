//
// Created by Shahak on 07/06/2017.
//

#include <stdlib.h>

#include "EscapeTechnion.h"
#include "utility.h"

struct EscapeTechnion_t {
    int *faculties_arr;
    Set companies;
    Set escapers;
    List reservations;
};

EscapeTechnion escapeTechnionCreate(MtmErrorCode *EscapeTechnionError) {
    EscapeTechnion escapeTechnion = malloc((size_t) sizeof(*escapeTechnion));
    if (NULL == escapeTechnion) {
        *EscapeTechnionError = MTM_OUT_OF_MEMORY;
        return NULL;
    }

    escapeTechnion->faculties_arr = malloc((size_t) sizeof(int) * UNKNOWN);
    if (NULL == escapeTechnion->faculties_arr) {
        *EscapeTechnionError = MTM_OUT_OF_MEMORY;
        free(escapeTechnion);
        return NULL;
    }
    initializeArr(escapeTechnion->faculties_arr, UNKNOWN);

    escapeTechnion->companies = setCreate(companyCopyElement, companyFreeElement,
                                        companyCompareElements);
    if (NULL == escapeTechnion->companies) {
        *EscapeTechnionError = MTM_OUT_OF_MEMORY;
        free(escapeTechnion->faculties_arr);
        free(escapeTechnion);
        return NULL;
    }

    escapeTechnion->escapers = setCreate(escaperCopyElement, escaperFreeElement,
                                       escaperCompareElements);
    if (NULL == escapeTechnion->escapers) {
        *EscapeTechnionError = MTM_OUT_OF_MEMORY;
        setDestroy(escapeTechnion->companies);
        free(escapeTechnion->faculties_arr);
        free(escapeTechnion);
        return NULL;
    }

    escapeTechnion->reservations = listCreate(reservationCopyElement,
                                            reservationFreeElement);
    if (NULL == escapeTechnion->reservations) {
        *EscapeTechnionError = MTM_OUT_OF_MEMORY;
        setDestroy(escapeTechnion->escapers);
        setDestroy(escapeTechnion->companies);
        free(escapeTechnion->faculties_arr);
        free(escapeTechnion);
        return NULL;
    }

    *EscapeTechnionError = MTM_SUCCESS;
    return escapeTechnion;
}