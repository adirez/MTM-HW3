//
// Created by Shahak on 06/06/2017.
//

#include "reservation.h"
#include "utility.h"
#include "escaper.h"

#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define INVALID_PARAMETER -1

struct Reservation_t {
    char *escaper_email;
    char *company_email;
    TechnionFaculty FacultyOfEscaper;
    TechnionFaculty FacultyOfRoom;

    int room_id;
    int escaper_skill;
    int days_to_reservation;
    int reservation_hour;
    int num_ppl;
    int total_cost;
};


/**
 * receives all the arguments to create a reservation and checks if they're all
 * valid
 * @param escaper_email - the email of the escaper
 * @param company_email - the email of the company the room is listed in
 * @param FacultyOfEscaper - the faculty the escaper is listed in
 * @param FacultyOfRoom - the faculty the room is listed in
 * @param room_id - the id of the asked room
 * @param num_ppl - number of escapers for the reservation
 * @param escaper_skill - the expertise level of the escaper
 * @param days_to_reservation - number of days for the reservation to take place
 * @param reservation_hour - the hour the room is reserved for
 * @param total_cost - the total amount to pay
 * @return true - all fields are valid
 *         false - not all fields are valid
 */
static bool checkReservationArgs(char *escaper_email, char *company_email,
                                 TechnionFaculty FacultyOfEscaper,
                                 TechnionFaculty FacultyOfRoom, int room_id,
                                 int num_ppl, int escaper_skill,
                                 int days_to_reservation, int reservation_hour,
                                 int total_cost);

Reservation reservationCreate(char *escaper_email, char *company_email,
                              TechnionFaculty FacultyOfEscaper,
                              TechnionFaculty FacultyOfRoom, int room_id,
                              int num_ppl, int escaper_skill,
                              int days_to_reservation, int reservation_hour,
                              int total_cost,
                              ReservationErrorCode *ReservationError) {

    assert(NULL != escaper_email && NULL != company_email);
    if (NULL == escaper_email || NULL == company_email ||
        !checkReservationArgs(escaper_email, company_email, FacultyOfEscaper,
                              FacultyOfRoom, room_id, num_ppl, escaper_skill,
                              days_to_reservation, reservation_hour,
                              total_cost)) {
        *ReservationError = RESERVATION_INVALID_PARAMETER;
        return NULL;
    }

    Reservation reservation = malloc((size_t) sizeof(*reservation));
    if (NULL == reservation) {
        *ReservationError = RESERVATION_OUT_OF_MEMORY;
        return NULL;
    }

    reservation->escaper_email = malloc(
            (size_t) sizeof(char) * strlen(escaper_email));
    if (NULL == reservation->escaper_email) {
        free(reservation);
        *ReservationError = RESERVATION_OUT_OF_MEMORY;
        return NULL;
    }
    strcpy(reservation->escaper_email, escaper_email);

    reservation->company_email = malloc(
            (size_t) sizeof(char) * strlen(company_email));
    if (NULL == reservation->company_email) {
        free(reservation->escaper_email);
        free(reservation);
        *ReservationError = RESERVATION_OUT_OF_MEMORY;
        return NULL;
    }
    strcpy(reservation->company_email, company_email);

    reservation->FacultyOfEscaper = FacultyOfEscaper;
    reservation->FacultyOfRoom = FacultyOfRoom;
    reservation->room_id = room_id;
    reservation->num_ppl = num_ppl;
    reservation->escaper_skill = escaper_skill;
    reservation->days_to_reservation = days_to_reservation;
    reservation->reservation_hour = reservation_hour;
    reservation->total_cost = total_cost;
    *ReservationError = RESERVATION_SUCCESS;
    return reservation;
}


ReservationErrorCode reservationDestroy(Reservation reservation) {
    if (NULL == reservation) {
        return RESERVATION_INVALID_PARAMETER;
    }
    free(reservation->escaper_email);
    free(reservation->company_email);
    free(reservation);
    return RESERVATION_SUCCESS;
}

void reservationFreeElement(ListElement reservation) {
    if (NULL == reservation) {
        return;
    }
    reservationDestroy(reservation);
}

ListElement reservationCopyElement(ListElement src_reservation) {
    if (NULL == src_reservation) {
        return NULL;
    }
    Reservation ptr = src_reservation; //to make the code clearer
    ReservationErrorCode CopyResult;
    Reservation reservation = reservationCreate(ptr->escaper_email,
                                                ptr->company_email,
                                                ptr->FacultyOfEscaper,
                                                ptr->FacultyOfRoom,
                                                ptr->room_id, ptr->num_ppl,
                                                ptr->escaper_skill,
                                                ptr->days_to_reservation,
                                                ptr->reservation_hour,
                                                ptr->total_cost, &CopyResult);
    if (CopyResult == RESERVATION_OUT_OF_MEMORY ||
        CopyResult == RESERVATION_INVALID_PARAMETER) {
        return NULL;
    }
    return reservation;
}

bool isReservationCompanyEmailEqual(Reservation reservation,
                                    char *company_email,
                                    ReservationErrorCode *ReservationError) {
    if (NULL == reservation || NULL == company_email) {
        *ReservationError = RESERVATION_INVALID_PARAMETER;
        return false;
    }
    *ReservationError = RESERVATION_SUCCESS;

    if (strcmp(reservation->company_email, company_email) == 0) {
        return true;
    }
    return false;
}

bool isReservationEscaperEmailEqual(Reservation reservation,
                                    char *escaper_email,
                                    ReservationErrorCode *ReservationError) {
    if (NULL == reservation || NULL == escaper_email) {
        *ReservationError = RESERVATION_INVALID_PARAMETER;
        return false;
    }
    *ReservationError = RESERVATION_SUCCESS;

    if (strcmp(reservation->escaper_email, escaper_email) == 0) {
        return true;
    }
    return false;
}

static bool checkReservationArgs(char *escaper_email, char *company_email,
                                 TechnionFaculty FacultyOfEscaper,
                                 TechnionFaculty FacultyOfRoom, int room_id,
                                 int num_ppl, int escaper_skill,
                                 int days_to_reservation, int reservation_hour,
                                 int total_cost) {
    if (!isEmailValid(escaper_email) || !isEmailValid(company_email)) {
        return false;
    }
    if (!isFacultyValid(FacultyOfEscaper) || !isFacultyValid(FacultyOfRoom)) {
        return false;
    }
    if (room_id <= 0 || num_ppl <= 0 || total_cost < 0 || escaper_skill <= 0 ||
        escaper_skill > 10) {
        return false;
    }
    return true;
}
