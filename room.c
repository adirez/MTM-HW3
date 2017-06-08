//
// Created by Shahak on 06/06/2017.
//

#include "room.h"
#include "utility.h"

#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define INVALID_PARAMETER -1

struct Room_t {
    char *company_email;
    int room_id;
    int room_price;
    int num_ppl;
    int opening_time;
    int closing_time;
    int difficulty;
};

/**
 * receives all the arguments to create a room and checks if they're all valid
 * @param company_email - the email of the company that the room will be listed
 *        in
 * @param room_id - the id of the room
 * @param price - the price rate of the room
 * @param num_ppl - the recommended number of people for the room
 * @param opening_time - the time the room opens
 * @param closing_time - the time the room closes
 * @param difficulty - the difficulty level of the room
 * @return true - all of the arguments are valid
 *         false - not all of the arguments are valid
 */
static bool checkRoomArgs (char* company_email, int room_id, int price,
                            int num_ppl, int opening_time, int closing_time,
                            int difficulty);
/**
 * receives the price per person of a certain room and makes sure it it's a
 * multiply of 4
 * @param price - the integer to be checked
 * @return true - if it's a multiply of 4
 *         false - if it's not a multiply of 4 or if it's not a positive number
 */
static bool isMultiplyOfFour (int price);


Room roomCreate(char *company_email, int room_id, int price, int num_ppl,
                int opening_time, int closing_time, int difficulty,
                RoomErrorCode *RoomError) {
    assert(NULL != company_email);
    if (NULL == company_email || !checkRoomArgs(company_email, room_id, price,
                                                num_ppl, opening_time,
                                                closing_time, difficulty)) {
        *RoomError = ROOM_INVALID_PARAMETER;
        return NULL;
    }

    Room room = malloc((size_t)sizeof(*room));
    if (NULL == room) {
        *RoomError = ROOM_OUT_OF_MEMORY;
        return NULL;
    }

    room->company_email = malloc((size_t) sizeof(char) * strlen(company_email));
    if (NULL == room->company_email) {
        free(room);
        *RoomError = ROOM_OUT_OF_MEMORY;
        return NULL;
    }
    strcpy(room->company_email, company_email);
    room->room_id = room_id;
    room->room_price = price;
    room->num_ppl = num_ppl;
    room->opening_time = opening_time;
    room->closing_time = closing_time;
    room->difficulty = difficulty;
    *RoomError = ROOM_SUCCESS;
    return room;
}


RoomErrorCode roomDestroy(Room room) {
    if (NULL == room) {
        return ROOM_INVALID_PARAMETER;
    }
    free(room->company_email);
    free(room);
    return ROOM_SUCCESS;
}


int roomCompareElements(SetElement room_1, SetElement room_2) {
    if (NULL == room_1 || NULL == room_2) {
        return INVALID_PARAMETER;
        //TODO gotta make sure that -1 won't be a problem
    }
    Room ptr1 = room_1, ptr2 = room_2;
    return ( (ptr1->room_id) - (ptr2->room_id) );
}


void roomFreeElement(SetElement room) {
    if (NULL == room) {
        return;
    }
    roomDestroy(room);
}

SetElement roomCopyElement(SetElement src_room) {
    if (NULL == src_room) {
        return NULL;
    }
    Room ptr = src_room; //to make the code clearer
    RoomErrorCode CopyResult;
    Room room = roomCreate(ptr->company_email, ptr->room_id ,ptr->room_price,
                           ptr->num_ppl, ptr->opening_time, ptr->closing_time,
                           ptr->difficulty, &CopyResult);
    if (CopyResult == ROOM_OUT_OF_MEMORY ||
        CopyResult == ROOM_INVALID_PARAMETER) {
        return NULL;
    }
    return room;
}

static bool checkRoomArgs (char* company_email, int room_id, int price,
                           int num_ppl, int opening_time, int closing_time,
                           int difficulty) {
    if (!isEmailValid(company_email)) {
        return false;
    }
    if (!isMultiplyOfFour(price)) {
        return false;
    }
    if (room_id <= 0 || num_ppl <= 0 || difficulty <= 0 || difficulty > 10 ) {
        return false;
    }
    return true;
}

static bool isMultiplyOfFour (int price) {
    if (price <= 0) {
        return false;
    }

    if (price % 4 == 0) {
        return true;
    }

    return false;
}

