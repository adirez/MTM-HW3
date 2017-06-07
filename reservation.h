//
// Created by Shahak on 06/06/2017.
//

#ifndef HW3_RESERVATION_H
#define HW3_RESERVATION_H

#include "mtm_ex3.h"
#include "list.h"

/**
 * Reservation will be implemented in this file as a linked list
 */
typedef struct Reservation_t *Reservation;

/**
 * a type used to return error codes related to Reservation
 */
typedef enum {
    RESERVATION_SUCCESS,
    RESERVATION_INVALID_PARAMETER,
    RESERVATION_OUT_OF_MEMORY
} ReservationErrorCode;

/**
 * creates a new reservation according to the relevant input parameters and
 * returns the relevant result code.
 * @param escaper_email - the email of the escaper who wants to reserve a room
 * @param Faculty - the Faculty the escaper is listed in
 * @param room_id - the id of the room to be reserved
 * @param reservation_time - the time the escaper wants to reserve the room
 * @param num_ppl - the number of people to participate in the reservation
 * @return Reservation - a pointer to the successful reservation if everything
 *         went ok
           NULL - if the allocation was not successful
 */
Reservation reservationCreate(char *escaper_email, TechnionFaculty faculty,
                              int room_id, char* reservation_time, int num_ppl,
                              ReservationErrorCode *reservationError);

/**
 * destroys a reservation and and releases all relevant allocated memory.
 * @param reservation - a pointer to the relevant reservation to be destroyed
 * @return RESERVATION_SUCCESS - the reservation was removed successfuly
 *         RESERVATION_INVALID_PARAMETER - one of the parameters was invalid
 */
ReservationErrorCode reservationDestroy(Reservation reservation);

/**
 * frees all relevant allocated memory of a specific escaper element
 * @param escaper - the escaper to be freed
 * @return RESERVATION_SUCCESS - the function went with no errors
 *         RESERVATION_INVALID_PARAMETER - one of the parameters was invalid
 */
ReservationErrorCode reservationFreeElement(ListElement reservation);

/**
 * receives a source reservation element and copies it's data into a newly
 * created reservation element
 * @param src_reservation - the source reservation that needs to be copied
 * @return pointer to the new allocated reservation or NULL if the allocation
 * failed
 */
ListElement reservationCopyElement(ListElement src_reservation);


#endif //HW3_RESERVATION_H
