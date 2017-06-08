//
// Created by Shahak on 06/06/2017.
//

#ifndef HW3_ROOM_H
#define HW3_ROOM_H

#include "mtm_ex3.h"
#include "set.h"

/**
 * Room will be implemented in this file as a set
 */
typedef struct Room_t *Room;

/**
 * a type used to return error codes related to Room
 */
typedef enum {
    ROOM_SUCCESS,
    ROOM_INVALID_PARAMETER,
    ROOM_OUT_OF_MEMORY,
    ROOM_RESERVATION_EXISTS,
    ROOM_ID_DOES_NOT_EXIST
} RoomErrorCode;

/**
 * creates a new room according to the relevant input parameters and returns
 * the relevant result code.
 * @param company_email - the email of the company the rooms will be listed
 *        under
 * @param room_id - the id of the room
 * @param price - entry price per person (must be a multiple of 4)
 * @param num_ppl - recommended number of people for this specific room
 * @param opening_time - the calculated opening time
 * @param closing_time - the calculated closing time
 * @param difficulty - the difficulty level of the room
 * @param RoomError - the result of the function
 * @return Room - a pointer to the successfully created room if everything went
 *         ok
 *         NULL - if the allocation was not successful
 */
Room roomCreate(char *company_email, int room_id, int price, int num_ppl,
                int opening_time, int closing_time, int difficulty,
                RoomErrorCode *RoomError);

/**
 * destroys a room and all of it's components. releases all relevant
 * allocated memory.
 * @param room - a pointer to the relevant company to be destroyed
 * @return ROOM_SUCCESS - the room was removed successfuly
 *         ROOM_INVALID_PARAMETER - one of the parameters was invalid
 *         ROOM_ID_DOES_NOT_EXIST - the id of the room wasn't recognized
 *         ROOM_RESERVATION_EXISTS - one of the rooms has an existing
 *         reservation
 */
RoomErrorCode roomDestroy(Room room);

/**
 * a comparison of two rooms by faculty and id
 * @param room_1 - the first room
 * @param room_2 - the second room
 * @return 0 - if both of the rooms are identical by faculty and id
 * @return NEGATIVE NUMBER - if the first room's faculty / id is
 *         lexicographically / numerically smaller than the second room's
 * @return POSITIVE NUMBER - if the first room's faculty / id is
 *         lexicographically / numerically bigger than the second room's
 */
int roomCompareElements(SetElement room_1, SetElement room_2);

/**
 * frees all relevant allocated memory of a specific room element
 * @param room - the room to be freed
 */
void roomFreeElement(SetElement room);

/**
 * receives a source room element and copies it's data into a newly created
 * room element
 * @param src_room - the source room that needs to be copied
 * @return pointer to the new allocated room or NULL if the allocation failed
 */
SetElement roomCopyElement(SetElement src_room);


#endif //HW3_ROOM_H
