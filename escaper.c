//
// Created by Shahak on 06/06/2017.
//

#include "escaper.h"
#include "utility.h"

#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define INVALID_PARAMETER -1

struct Escaper_t {
    char *escaper_email;
    TechnionFaculty FacultyOfEscaper;
    int skill_level;
};

/**
 * receives all the arguments to create an escaper and checks if they're all
 * valid
 * @param escaper_email - the email of the escaper to be added to his profile
 * @param FacultyOfEscaper - the faculty the escaper is listed in
 * @param skill_level - the escaper's expertise in escape rooms
 * @return true - all of the arguments are valid
 *         false - not all of the arguments are valid
 */
static bool checkEscaperArgs (char *escaper_email, int skill_level,
                              TechnionFaculty FacultyOfEscaper);

Escaper escaperCreate(char *escaper_email, TechnionFaculty FacultyOfEscaper,
                      int skill_level, EscaperErrorCode *EscaperError) {
    assert(NULL != escaper_email);
    if (NULL == escaper_email || !checkEscaperArgs(escaper_email, skill_level,
                                                   FacultyOfEscaper)) {
        *EscaperError = ESCAPER_INVALID_PARAMETER;
        return NULL;
    }

    Escaper escaper = malloc((size_t)sizeof(*escaper));
    if (NULL == escaper) {
        *EscaperError = ESCAPER_OUT_OF_MEMORY;
        return NULL;
    }

    escaper->escaper_email = malloc((size_t)sizeof(char)*strlen(escaper_email));
    if (NULL == escaper->escaper_email) {
        free(escaper);
        *EscaperError = ESCAPER_OUT_OF_MEMORY;
        return NULL;
    }
    strcpy(escaper->escaper_email, escaper_email);
    escaper->FacultyOfEscaper = FacultyOfEscaper;
    escaper->skill_level = skill_level;
    *EscaperError = ESCAPER_SUCCESS;
    return escaper;
}


EscaperErrorCode escaperDestroy(Escaper escaper) {
    if (NULL == escaper) {
        return ESCAPER_INVALID_PARAMETER;
    }
    free(escaper->escaper_email);
    free(escaper);
    return ESCAPER_SUCCESS;
}


int escaperCompareElements(SetElement escaper_1, SetElement escaper_2) {
    if (NULL == escaper_1 || NULL == escaper_2) {
        return INVALID_PARAMETER;
        //TODO gotta make sure that -1 won't be a problem
    }
    Escaper ptr1 = escaper_1, ptr2 = escaper_2;
    return (strcmp(ptr1->escaper_email, ptr2->escaper_email) );
}


void escaperFreeElement(SetElement escaper) {
    if (NULL == escaper) {
        return;
    }
    escaperDestroy(escaper);
}

SetElement escaperCopyElement(SetElement src_escaper) {
    if (NULL == src_escaper) {
        return NULL;
    }
    Escaper ptr = src_escaper; //to make the code clearer
    EscaperErrorCode CopyResult;
    Escaper escaper = escaperCreate(ptr->escaper_email, ptr->FacultyOfEscaper,
                                    ptr->skill_level, &CopyResult);
    if (CopyResult == ESCAPER_OUT_OF_MEMORY ||
        CopyResult == ESCAPER_INVALID_PARAMETER) {
        return NULL;
    }
    return escaper;
}

char *escaperGetEmail(Escaper escaper, EscaperErrorCode *EscaperError) {
    if (NULL == escaper) {
        *EscaperError = ESCAPER_INVALID_PARAMETER;
        return NULL;
    }
    char *output = malloc(
            (size_t) sizeof(char) * strlen(escaper->escaper_email));
    if (NULL == output) {
        *EscaperError = ESCAPER_OUT_OF_MEMORY;
        return NULL;
    }
    strcpy(output, escaper->escaper_email);
    *EscaperError = ESCAPER_SUCCESS;
    return output;
}

TechnionFaculty escaperGetFaculty(Escaper escaper,
                                  EscaperErrorCode *EscaperError) {
    if (NULL == escaper) {
        *EscaperError = ESCAPER_INVALID_PARAMETER;
        return UNKNOWN;
    }

    *EscaperError = ESCAPER_SUCCESS;
    return escaper->FacultyOfEscaper;
}

int escaperGetSkillLevel(Escaper escaper, EscaperErrorCode *EscaperError) {
    if (NULL == escaper) {
        *EscaperError = ESCAPER_INVALID_PARAMETER;
        return INVALID_PARAMETER;
    }

    *EscaperError = ESCAPER_SUCCESS;
    return escaper->skill_level;
}

static bool checkEscaperArgs (char *escaper_email, int skill_level,
                              TechnionFaculty FacultyOfEscaper) {
    if (!isEmailValid(escaper_email)) {
        return false;
    }
    if (!isFacultyValid(FacultyOfEscaper)) {
        return false;
    }
    if (skill_level <= 0 || skill_level > 10) {
        return false;
    }
    return true;
}

