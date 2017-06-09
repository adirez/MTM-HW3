//
// Created by Shahak on 07/06/2017.
//

#include <stdlib.h>
#include <assert.h>

#include "EscapeTechnion.h"
#include "utility.h"

struct EscapeTechnion_t {
    Set companies;
    Set escapers;
    List reservations;

    int *faculties_earnings;
    int current_day;
};

/**
 * receives an email and an escapeTechnion system and checks if the email
 * already exists in the system
 * @param email - the email to be checked
 * @param escapeTechnion - the system to iterate through
 * @return true - if the email is in use
 *         false - if the email doesn't exist in the system
 */
static bool isCompanyWithEmail(char *email, EscapeTechnion escapeTechnion);

/**
 * receives an email and an escapeTechnion system and checks if the email is
 * listed in a reservation as a company's email
 * @param email - the email to be checked
 * @param escapeTechnion - the system to iterate through
 * @return true - if the email exists
 *         false - if the email doesn't exist in the system
 */
static bool isReservationForCompany(char *email,
                                    EscapeTechnion escapeTechnion);

/**
 * receives an email and an escapeTechnion system and checks if the email is
 * listed in a reservation as an escaper's email
 * @param email - the email to be checked
 * @param escapeTechnion - the system to iterate through
 * @return true - if the email exists
 *         false - if the email doesn't exist in the system
 */ /*
static bool isReservationForEscaper(char *email,
                                    EscapeTechnion escapeTechnion);
*/
/**
 * receives an email and an escapeTechnion system and checks if the email is
 * listed as a company's email in the system
 * @param email - the email to be checked
 * @param escapeTechnion - the system to iterate through
 * @return a pointer to the company if found, NULL if not found
 */
static Company findCompany(char *email, EscapeTechnion escapeTechnion);

EscapeTechnion escapeTechnionCreate(MtmErrorCode *EscapeTechnionError) {
    EscapeTechnion escapeTechnion = malloc((size_t) sizeof(*escapeTechnion));
    if (NULL == escapeTechnion) {
        *EscapeTechnionError = MTM_OUT_OF_MEMORY;
        return NULL;
    }

    escapeTechnion->faculties_earnings = malloc((size_t) sizeof(int) * UNKNOWN);
    if (NULL == escapeTechnion->faculties_earnings) {
        *EscapeTechnionError = MTM_OUT_OF_MEMORY;
        free(escapeTechnion);
        return NULL;
    }
    initializeArr(escapeTechnion->faculties_earnings, UNKNOWN);

    escapeTechnion->companies = setCreate(companyCopyElement,
                                          companyFreeElement,
                                          companyCompareElements);
    if (NULL == escapeTechnion->companies) {
        *EscapeTechnionError = MTM_OUT_OF_MEMORY;
        free(escapeTechnion->faculties_earnings);
        free(escapeTechnion);
        return NULL;
    }

    escapeTechnion->escapers = setCreate(escaperCopyElement, escaperFreeElement,
                                       escaperCompareElements);
    if (NULL == escapeTechnion->escapers) {
        *EscapeTechnionError = MTM_OUT_OF_MEMORY;
        setDestroy(escapeTechnion->companies);
        free(escapeTechnion->faculties_earnings);
        free(escapeTechnion);
        return NULL;
    }

    escapeTechnion->reservations = listCreate(reservationCopyElement,
                                            reservationFreeElement);
    if (NULL == escapeTechnion->reservations) {
        *EscapeTechnionError = MTM_OUT_OF_MEMORY;
        setDestroy(escapeTechnion->escapers);
        setDestroy(escapeTechnion->companies);
        free(escapeTechnion->faculties_earnings);
        free(escapeTechnion);
        return NULL;
    }

    escapeTechnion->current_day = 0;
    *EscapeTechnionError = MTM_SUCCESS;
    return escapeTechnion;
}

MtmErrorCode escapeTechnionDestroy(EscapeTechnion escapeTechnion) {
    if (NULL == escapeTechnion) {
        return MTM_NULL_PARAMETER;
    }
    listDestroy(escapeTechnion->reservations);
    setDestroy(escapeTechnion->escapers);
    setDestroy(escapeTechnion->companies);
    free(escapeTechnion->faculties_earnings);
    free(escapeTechnion);
    return MTM_SUCCESS;
}

MtmErrorCode escapeTechnionAddCompany(EscapeTechnion escapeTechnion,
                                      char *email, TechnionFaculty Faculty) {
    if (NULL == escapeTechnion || NULL == email || !isEmailValid(email) ||
        !isFacultyValid(Faculty)) {
        return MTM_INVALID_PARAMETER;
    }
    if (isCompanyWithEmail(email, escapeTechnion)) {
        return MTM_EMAIL_ALREADY_EXISTS;
    }

    CompanyErrorCode CompanyError;
    Company company = companyCreate(email, Faculty, &CompanyError);
    if (CompanyError == COMPANY_INVALID_PARAMETER) {
        return MTM_INVALID_PARAMETER;
    } else if (CompanyError == COMPANY_OUT_OF_MEMORY) {
        return MTM_OUT_OF_MEMORY;
    }
    SetResult SetError = setAdd(escapeTechnion->companies, company);
    assert(SetError != SET_ITEM_ALREADY_EXISTS &&
           SetError != SET_NULL_ARGUMENT);
    if (SetError == SET_OUT_OF_MEMORY) {
        companyDestroy(company);
        return MTM_OUT_OF_MEMORY;
    }
    return MTM_SUCCESS;
}

MtmErrorCode escapeTechnionRemoveCompany(EscapeTechnion escapeTechnion,
                                         char *company_email) {
    if (NULL == escapeTechnion || NULL == company_email ||
        !isEmailValid(company_email)) {
        return MTM_INVALID_PARAMETER;
    }
    if (isReservationForCompany(company_email, escapeTechnion)) {
        return MTM_RESERVATION_EXISTS;
    }

    Company company = findCompany(company_email, escapeTechnion);
    if (NULL == company) {
        return MTM_COMPANY_EMAIL_DOES_NOT_EXIST;
    }
    setRemove(escapeTechnion->companies, company);
    return MTM_SUCCESS;
}

static bool isCompanyWithEmail(char *email, EscapeTechnion escapeTechnion) {
    if (NULL == email || NULL == escapeTechnion) {
        return false;
    }
    Company company_iterator = setGetFirst(escapeTechnion->companies);
    //TODO should we check NULL separately?
    while (NULL != company_iterator) {
        CompanyErrorCode CompanyError = COMPANY_SUCCESS;
        if (isCompanyEmailEqual(company_iterator, email, &CompanyError)) {
            return true;
        }
        assert(CompanyError != COMPANY_INVALID_PARAMETER);
        company_iterator = setGetNext(escapeTechnion->companies);
    }
    return false;
}

static bool isReservationForCompany(char *email,
                                    EscapeTechnion escapeTechnion) {
    if (NULL == email || NULL == escapeTechnion) {
        return false;
    }
    Reservation reservation_iterator;
    reservation_iterator = listGetFirst(escapeTechnion->reservations);
    while (NULL != reservation_iterator) {
        ReservationErrorCode ReservationError = RESERVATION_SUCCESS;
        if (isReservationCompanyEmailEqual(reservation_iterator, email,
                                           &ReservationError)) {
            return true;
        }
        assert(ReservationError != RESERVATION_INVALID_PARAMETER);
        reservation_iterator = listGetNext(escapeTechnion->reservations);
    }
    return false;
}
/*
static bool isReservationForEscaper(char *email,
                                    EscapeTechnion escapeTechnion) {
    if (NULL == email || NULL == escapeTechnion) {
        return false;
    }
    Reservation reservation_iterator;
    reservation_iterator = listGetFirst(escapeTechnion->reservations);
    while (NULL != reservation_iterator) {
        ReservationErrorCode ReservationError = RESERVATION_SUCCESS;
        if (isReservationEscaperEmailEqual(reservation_iterator, email,
                                           &ReservationError)) {
            return true;
        }
        assert(ReservationError != RESERVATION_INVALID_PARAMETER);
        reservation_iterator = listGetNext(escapeTechnion->reservations);
    }
    return false;
}
*/
static Company findCompany(char *email, EscapeTechnion escapeTechnion) {
    if (NULL == escapeTechnion || NULL == email) {
        return NULL;
    }
    Company company_iterator = setGetFirst(escapeTechnion->companies);
    while (NULL != company_iterator) {
        CompanyErrorCode CompanyError = COMPANY_SUCCESS;
        if (isCompanyEmailEqual(company_iterator, email, &CompanyError)) {
            return company_iterator;
        }
        assert(CompanyError != COMPANY_INVALID_PARAMETER);
        company_iterator = setGetNext(escapeTechnion->companies);
    }
    return NULL;
}