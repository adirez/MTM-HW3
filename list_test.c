#include "test_utilities.h"
#include "list_mtm1.h"
#include <string.h>
#include <stdlib.h>

typedef struct student_t {
    int id;
    char *name;
    double average;
} *Student;

/*
static void printStudent(Student student){
    printf("\n------------------------------------\n");
    printf("ID = %d\n", student->id);
    printf("NAME = %s\n", student->name);
    printf("AVERAGE = %.2f\n", student->average);
}
*/
static Student studentCreate(int id, char *name, double average) {
    assert(name != NULL);
    if (name == NULL) {
        return NULL;
    }
    Student student = malloc(sizeof(*student));
    if (student == NULL) {
        return NULL;
    }
    student->id = id;
    student->name = malloc(sizeof(strlen(name) + 1));
    if (student->name == NULL) {
        free(student);
        return NULL;
    }
    strcpy(student->name, name);
    student->average = average;
    return student;
}

static ListElement copyStudent(ListElement element) {
    assert(element != NULL);
    if (element == NULL) {
        return NULL;
    }
    Student student = element;
    Student new_student = studentCreate(student->id, student->name,
                                        student->average);
    return new_student;
}

static void freeStudent(ListElement element) {
    assert(element != NULL);

    Student student = element;

    free(student->name);
    free(student);
}

static bool isAverageGreaterThan(ListElement element, ListFilterKey number) {
    Student student = element;
    if (student->average > *(double *) number) {
        return true;
    }
    return false;
}

static bool areNameEqual(ListElement element, ListFilterKey name) {
    Student student = element;

    if (strcmp(student->name, (char *) name) == 0) {
        return true;
    }
    return false;
}

static int compareAverage(ListElement element1, ListElement element2) {
    Student student1 = element1;
    Student student2 = element2;
    return (int)(student1->average - student2->average);
}

static int compareNames(ListElement element1, ListElement element2) {
    Student student1 = element1;
    Student student2 = element2;
    return strcmp(student1->name, student2->name);
}

/** Tests for each function */

static bool testListCreate() {
    //test NULL params returns value
    ASSERT_TEST(listCreate(NULL, NULL) == NULL);
    ASSERT_TEST(listCreate(copyStudent, NULL) == NULL);
    ASSERT_TEST(listCreate(NULL, freeStudent) == NULL);

    //test success create
    List list = listCreate(copyStudent, freeStudent);
    ASSERT_TEST(list != NULL);

    listDestroy(list);
    return true;
}

static bool testListCopy() {
    //test result value for NULL param
    ASSERT_TEST(listCopy(NULL) == NULL);
    //create the original list
    List list = listCreate(copyStudent, freeStudent);
    int id[2] = {308571546, 311242440};
    char *name[2] = {"Adi Reznik", "Shahak Ben Kalifa"};
    double average[2] = {88.9, 91.00};
    Student student0 = studentCreate(id[0], name[0], average[0]);
    Student student1 = studentCreate(id[1], name[1], average[1]);
    listInsertLast(list, student0);
    listInsertLast(list, student1);
    //put the iterator to the second element
    listGetFirst(list);
    listGetNext(list);

    List list_copy = listCopy(list);
    //test that the iterator is on the same element and the copied elements
    // are equal and in the same order
    Student test_student = listGetCurrent(list_copy);
    ASSERT_TEST(test_student->id == id[1]);
    ASSERT_TEST(strcmp(test_student->name, name[1]) == 0);
    ASSERT_TEST(test_student->average == average[1]);

    test_student = listGetFirst(list_copy);
    ASSERT_TEST(test_student->id == id[0]);
    ASSERT_TEST(strcmp(test_student->name, name[0]) == 0);
    ASSERT_TEST(test_student->average == average[0]);

    //test the infrastructure functions of the copied list: copy and free
    List list_copy_of_copy = listCopy(list_copy);
    test_student = listGetCurrent(list_copy_of_copy);
    ASSERT_TEST(test_student->id == id[0]);

    test_student = listGetNext(list_copy_of_copy);
    ASSERT_TEST(test_student->id == id[1]);

    listDestroy(list);
    listDestroy(list_copy);
    listDestroy(list_copy_of_copy);
    return true;
}

static bool testListGetSize() {
    //test return value for NULL param
    ASSERT_TEST(listGetSize(NULL) == -1);
    //test return value for empty list
    List list = listCreate(copyStudent, freeStudent);
    ASSERT_TEST(listGetSize(list) == 0);

    //enter elements to the list and test size
    int id[2] = {308571546, 311242440};
    char *name[2] = {"Adi Reznik", "Shahak Ben Kalifa"};
    double average[2] = {88.9, 91.00};
    Student student0 = studentCreate(id[0], name[0], average[0]);
    Student student1 = studentCreate(id[1], name[1], average[1]);
    listInsertLast(list, student0);
    ASSERT_TEST(listGetSize(list) == 1);
    listInsertLast(list, student1);
    ASSERT_TEST(listGetSize(list) == 2);

    //remove elements from list and test size
    listGetFirst(list);
    listRemoveCurrent(list);
    ASSERT_TEST(listGetSize(list) == 1);
    listGetFirst(list);
    listRemoveCurrent(list);
    ASSERT_TEST(listGetSize(list) == 0);

    listDestroy(list);
    return true;
}

static bool testListGetFirst() {
    //test return value for NULL param
    ASSERT_TEST(listGetFirst(NULL) == NULL);
    //test return value for empty list
    List list = listCreate(copyStudent, freeStudent);
    ASSERT_TEST(listGetFirst(list) == NULL);

    int id[3] = {308571546, 311242440, 234122};
    char *name[3] = {"Adi Reznik", "Shahak Ben Kalifa", "MTM"};
    double average[3] = {88.9, 91.00, 71};
    Student student0 = studentCreate(id[0], name[0], average[0]);
    Student student1 = studentCreate(id[1], name[1], average[1]);
    Student student2 = studentCreate(id[2], name[2], average[2]);

    //enter 1 element to the list and test if it gets back
    listInsertLast(list, student0);
    Student test_student = listGetFirst(list);
    ASSERT_TEST(test_student->id == id[0]);
    ASSERT_TEST(strcmp(test_student->name, name[0]) == 0);
    ASSERT_TEST(test_student->average == average[0]);

    //add element after the first and test if the first got back
    listInsertLast(list, student1);
    test_student = listGetFirst(list);
    ASSERT_TEST(test_student->id == id[0]);

    //add element to the beginning of the list and test if it got back
    listInsertFirst(list, student2);
    test_student = listGetFirst(list);
    ASSERT_TEST(test_student->id == id[2]);

    listDestroy(list);
    return true;
}

static bool testListGetNext() {
    //test return value for NULL param
    ASSERT_TEST(listGetNext(NULL) == NULL);
    //test return value for empty list
    List list = listCreate(copyStudent, freeStudent);
    ASSERT_TEST(listGetNext(list) == NULL);

    int id[4] = {308571546, 311242440, 234122, 7};
    char *name[4] = {"Adi Reznik", "Shahak Ben Kalifa", "MTM", "Cenzor"};
    double average[4] = {88.9, 91.00, 71, 100};
    Student student0 = studentCreate(id[0], name[0], average[0]);
    Student student1 = studentCreate(id[1], name[1], average[1]);
    Student student2 = studentCreate(id[2], name[2], average[2]);
    Student student3 = studentCreate(id[3], name[3], average[3]);

    //insert an element and test if returns NULL because iterator is at an
    // invalid state
    listInsertLast(list, student0);
    ASSERT_TEST(listGetNext(list) == NULL);
    //move iterator to the element and test if NULL comes back (end of the list)
    listGetFirst(list);
    ASSERT_TEST(listGetNext(list) == NULL);
    listGetFirst(list);
    //insert 2 more elements and test that the first of them is returned
    listInsertLast(list, student1);
    listInsertLast(list, student2);
    Student test_student = listGetNext(list);
    ASSERT_TEST(test_student->id == id[1]);
    //insert element to the beginning of list and test if the last element
    // returns
    listInsertLast(list, student3);
    test_student = listGetNext(list);
    ASSERT_TEST(test_student->id == id[2]);

    listDestroy(list);
    return true;
}

static bool testListGetCurrent() {
    //test return value for NULL param
    ASSERT_TEST(listGetCurrent(NULL) == NULL);
    //test return value for empty list
    List list = listCreate(copyStudent, freeStudent);
    ASSERT_TEST(listGetCurrent(list) == NULL);

    int id[4] = {308571546, 311242440, 234122, 7};
    char *name[4] = {"Adi Reznik", "Shahak Ben Kalifa", "MTM", "Cenzor"};
    double average[4] = {88.9, 91.00, 71, 100};
    Student student0 = studentCreate(id[0], name[0], average[0]);
    Student student1 = studentCreate(id[1], name[1], average[1]);
    Student student2 = studentCreate(id[2], name[2], average[2]);
    Student student3 = studentCreate(id[3], name[3], average[3]);

    //insert an element and test if returns NULL because iterator is at an
    // invalid state
    listInsertLast(list, student0);
    ASSERT_TEST(listGetCurrent(list) == NULL);
    //move iterator to first element and test if is is returned
    listGetFirst(list);
    Student test_student = listGetCurrent(list);
    ASSERT_TEST(test_student->id == id[0]);
    //insert elements using all the insert funcs and test if the currnet
    // element has not moved
    listInsertFirst(list, student1);
    test_student = listGetCurrent(list);
    ASSERT_TEST(test_student->id == id[0]);
    listInsertBeforeCurrent(list, student2);
    test_student = listGetCurrent(list);
    ASSERT_TEST(test_student->id == id[0]);
    listInsertAfterCurrent(list, student3);
    test_student = listGetCurrent(list);
    ASSERT_TEST(test_student->id == id[0]);

    listDestroy(list);
    return true;
}


static bool testListInsertFirst() {
    List list = listCreate(copyStudent, freeStudent);
    int id[3] = {308571546, 311242440, 234122};
    char *name[3] = {"Adi Reznik", "Shahak Ben Kalifa", "MTM"};
    double average[3] = {88.9, 91.00, 71};
    Student student0 = studentCreate(id[0], name[0], average[0]);
    Student student1 = studentCreate(id[1], name[1], average[1]);
    Student student2 = studentCreate(id[2], name[2], average[2]);

    //test return value for NULL params
    ASSERT_TEST(listInsertFirst(NULL, NULL) == LIST_NULL_ARGUMENT);
    ASSERT_TEST(listInsertFirst(NULL, student0) == LIST_NULL_ARGUMENT);

    //TODO: change
//    ASSERT_TEST(listInsertFirst(list, NULL) == LIST_NULL_ARGUMENT);
    //insert an element to the list and test all its fields
    ASSERT_TEST(listInsertFirst(list, student0) == LIST_SUCCESS);
    Student test_student = listGetFirst(list);
    ASSERT_TEST(test_student->id == id[0]);
    ASSERT_TEST(strcmp(test_student->name, name[0]) == 0);
    ASSERT_TEST(test_student->average == average[0]);

    //insert another element and test that it is the first
    ASSERT_TEST(listInsertFirst(list, student1) == LIST_SUCCESS);
    test_student = listGetFirst(list);
    ASSERT_TEST(test_student->id == id[1]);

    //move iterator to the next element, insert another element and test that
    // the iterator is still on that element
    ASSERT_TEST(listInsertFirst(list, student1) == LIST_SUCCESS);
    test_student = listGetCurrent(list);
    ASSERT_TEST(test_student->id == id[1]);
    return true;
}

static bool testListInsertLast() {
    int id[6] = {308571546, 311242440, 21620431, 777777777, 1, 23};
    char *name[6] = {"Adi Reznik", "Shahak Ben Kalifa", "Sagi Barazani",
                     "Horef Cenzor", "aaa", "abc"};
    double average[6] = {88.9, 91.00, 93.5, 100, 30.21, 55.5};

    Student student0 = studentCreate(id[0], name[0], average[0]);
    Student student1 = studentCreate(id[1], name[1], average[1]);
    Student student2 = studentCreate(id[2], name[2], average[2]);
    Student student3 = studentCreate(id[3], name[3], average[3]);
    Student student4 = studentCreate(id[4], name[4], average[4]);
    Student student5 = studentCreate(id[5], name[5], average[5]);

    List list = listCreate(copyStudent, freeStudent);

    ASSERT_TEST(listInsertLast(NULL, student1) == LIST_NULL_ARGUMENT);
    ASSERT_TEST(listInsertLast(list, student2) == LIST_SUCCESS);
    ASSERT_TEST(listInsertLast(list, student3) == LIST_SUCCESS);
    ASSERT_TEST(listInsertLast(NULL, student1) == LIST_NULL_ARGUMENT);
    ASSERT_TEST(listInsertLast(list, student4) == LIST_SUCCESS);

    ASSERT_TEST(listGetSize(list) == 3);

    Student test_student = listGetFirst(list);
    ASSERT_TEST(test_student->id == id[2]);

    for (int i = 3; i < 5; ++i) {
        test_student = listGetNext(list);
        ASSERT_TEST(test_student->id == id[i]);
    }

    ASSERT_TEST(listInsertFirst(list, student1) == LIST_SUCCESS);
    ASSERT_TEST(listInsertLast(list, student5) == LIST_SUCCESS);
    ASSERT_TEST(listInsertFirst(list, student0) == LIST_SUCCESS);

    ASSERT_TEST(listGetSize(list) == 6);

    //make sure the iterator didn't move
    ASSERT_TEST(test_student->id == id[4]);

    test_student = listGetFirst(list);
    ASSERT_TEST(test_student->id == id[0]);

    for (int i = 1; i < 2; ++i) {
        test_student = listGetNext(list);
        ASSERT_TEST(test_student->id == id[i]);
    }


    listDestroy(list);
    return true;
}

static bool testListInsertBeforeCurrent() {
    int id[6] = {308571546, 311242440, 21620431, 777777777, 1, 23};
    char *name[6] = {"Adi Reznik", "Shahak Ben Kalifa", "Sagi Barazani",
                     "Horef Cenzor", "aaa", "abc"};
    double average[6] = {88.9, 91.00, 93.5, 100, 30.21, 55.5};

    Student student0 = studentCreate(id[0], name[0], average[0]);
    Student student1 = studentCreate(id[1], name[1], average[1]);
    Student student2 = studentCreate(id[2], name[2], average[2]);
    Student student3 = studentCreate(id[3], name[3], average[3]);
    Student student4 = studentCreate(id[4], name[4], average[4]);
    Student student5 = studentCreate(id[5], name[5], average[5]);

    List list = listCreate(copyStudent, freeStudent);

    ASSERT_TEST(listInsertBeforeCurrent(NULL, student1) == LIST_NULL_ARGUMENT);
    ASSERT_TEST(
            listInsertBeforeCurrent(list, student1) == LIST_INVALID_CURRENT);
    ASSERT_TEST(listInsertFirst(list, student5) == LIST_SUCCESS);
    listGetFirst(list);
    ASSERT_TEST(listInsertBeforeCurrent(list, student0) == LIST_SUCCESS);
    ASSERT_TEST(listInsertBeforeCurrent(list, student1) == LIST_SUCCESS);
    ASSERT_TEST(listInsertBeforeCurrent(NULL, student1) == LIST_NULL_ARGUMENT);
    ASSERT_TEST(listInsertBeforeCurrent(list, student2) == LIST_SUCCESS);
    ASSERT_TEST(listInsertBeforeCurrent(list, student3) == LIST_SUCCESS);
    ASSERT_TEST(listInsertBeforeCurrent(list, student4) == LIST_SUCCESS);

    ASSERT_TEST(listGetSize(list) == 6);

    Student test_student = listGetFirst(list);
    ASSERT_TEST(test_student->id == id[0]);

    for (int i = 1; i < 5; ++i) {
        test_student = listGetNext(list);
        ASSERT_TEST(test_student->id == id[i]);
    }

    listDestroy(list);
    return true;
}

static bool testListInsertAfterCurrent() {
    int id[6] = {308571546, 311242440, 21620431, 777777777, 1, 23};
    char *name[6] = {"Adi Reznik", "Shahak Ben Kalifa", "Sagi Barazani",
                     "Horef Cenzor", "aaa", "abc"};
    double average[6] = {88.9, 91.00, 93.5, 100, 30.21, 55.5};

    Student student0 = studentCreate(id[0], name[0], average[0]);
    Student student1 = studentCreate(id[1], name[1], average[1]);
    Student student2 = studentCreate(id[2], name[2], average[2]);
    Student student3 = studentCreate(id[3], name[3], average[3]);
    Student student4 = studentCreate(id[4], name[4], average[4]);
    Student student5 = studentCreate(id[5], name[5], average[5]);

    List list = listCreate(copyStudent, freeStudent);

    ASSERT_TEST(listInsertAfterCurrent(NULL, student1) == LIST_NULL_ARGUMENT);
    ASSERT_TEST(listInsertAfterCurrent(list, student1) == LIST_INVALID_CURRENT);
    ASSERT_TEST(listInsertLast(list, student0) == LIST_SUCCESS);
    listGetFirst(list);
    ASSERT_TEST(listInsertAfterCurrent(list, student5) == LIST_SUCCESS);
    ASSERT_TEST(listInsertAfterCurrent(list, student4) == LIST_SUCCESS);
    ASSERT_TEST(listInsertAfterCurrent(NULL, student1) == LIST_NULL_ARGUMENT);
    ASSERT_TEST(listInsertAfterCurrent(list, student3) == LIST_SUCCESS);
    ASSERT_TEST(listInsertAfterCurrent(list, student2) == LIST_SUCCESS);
    ASSERT_TEST(listInsertAfterCurrent(list, student1) == LIST_SUCCESS);

    ASSERT_TEST(listGetSize(list) == 6);

    Student test_student = listGetFirst(list);
    ASSERT_TEST(test_student->id == id[0]);

    for (int i = 1; i < 5; ++i) {
        test_student = listGetNext(list);
        ASSERT_TEST(test_student->id == id[i]);
    }

    listDestroy(list);
    return true;
}

static bool testListRemoveCurrent() {
    int id[5] = {308571546, 311242440, 21620431, 777777777, 1};
    char *name[5] = {"Adi Reznik", "Shahak Ben Kalifa", "Sagi Barazani",
                     "Horef Cenzor", "aaa"};
    double average[5] = {88.9, 91.00, 93.5, 100, 30.21};

    Student student0 = studentCreate(id[0], name[0], average[0]);
    Student student1 = studentCreate(id[1], name[1], average[1]);
    Student student2 = studentCreate(id[2], name[2], average[2]);
    Student student3 = studentCreate(id[3], name[3], average[3]);
    Student student4 = studentCreate(id[4], name[4], average[4]);

    List list = listCreate(copyStudent, freeStudent);

    ASSERT_TEST(listInsertLast(list, student0) == LIST_SUCCESS);
    ASSERT_TEST(listInsertLast(list, student1) == LIST_SUCCESS);
    ASSERT_TEST(listInsertLast(list, student2) == LIST_SUCCESS);
    ASSERT_TEST(listInsertLast(list, student3) == LIST_SUCCESS);
    ASSERT_TEST(listInsertLast(list, student4) == LIST_SUCCESS);


    ASSERT_TEST(listRemoveCurrent(NULL) == LIST_NULL_ARGUMENT);
    ASSERT_TEST(listRemoveCurrent(list) == LIST_INVALID_CURRENT);

    listGetFirst(list);
    ASSERT_TEST(listRemoveCurrent(list) == LIST_SUCCESS);
    Student test_student = listGetFirst(list);
    ASSERT_TEST(test_student->id == id[1]);

    listGetFirst(list);
    listGetNext(list);
    ASSERT_TEST(listRemoveCurrent(list) == LIST_SUCCESS);
    test_student = listGetFirst(list);
    ASSERT_TEST(test_student->id == id[1]);
    listGetNext(list);
    test_student = listGetNext(list);
    ASSERT_TEST(test_student->id == id[4]);

    listGetFirst(list);
    listGetNext(list);
    listGetNext(list);
    ASSERT_TEST(listRemoveCurrent(list) == LIST_SUCCESS);
    test_student = listGetFirst(list);
    ASSERT_TEST(test_student->id == id[1]);
    listGetNext(list);
    ASSERT_TEST(listRemoveCurrent(list) == LIST_SUCCESS);
    ASSERT_TEST(listGetSize(list) == 1);

    listGetFirst(list);
    ASSERT_TEST(listRemoveCurrent(list) == LIST_SUCCESS);
    ASSERT_TEST(listGetFirst(list) == NULL);
    ASSERT_TEST(listGetSize(list) == 0);

    listDestroy(list);
    return true;
}

static bool testListSort() {
    List list = listCreate(copyStudent, freeStudent);
    ASSERT_TEST(listSort(NULL, compareAverage) == LIST_NULL_ARGUMENT);
    ASSERT_TEST(listSort(list, NULL) == LIST_NULL_ARGUMENT);
    ASSERT_TEST(listSort(list, compareAverage) == LIST_SUCCESS);
    ASSERT_TEST(listGetCurrent(list) == NULL);
    ASSERT_TEST(listGetFirst(list) == NULL);

    int id[6] = {308571546, 311242440, 21620431, 777777777, 1, 23};
    char *name[6] = {"Adi Reznik", "Shahak Ben Kalifa", "Sagi Barazani",
                     "Horef Cenzor", "aaa", "abc"};
    double average[6] = {88.9, 91.00, 93.5, 100, 30.21, 55.5};

    Student student0 = studentCreate(id[0], name[0], average[0]);
    Student student1 = studentCreate(id[1], name[1], average[1]);
    Student student2 = studentCreate(id[2], name[2], average[2]);
    Student student3 = studentCreate(id[3], name[3], average[3]);
    Student student4 = studentCreate(id[4], name[4], average[4]);
    Student student5 = studentCreate(id[5], name[5], average[5]);

    ASSERT_TEST(listInsertLast(list, student0) == LIST_SUCCESS);
    ASSERT_TEST(listInsertLast(list, student1) == LIST_SUCCESS);
    ASSERT_TEST(listInsertLast(list, student2) == LIST_SUCCESS);
    ASSERT_TEST(listInsertLast(list, student3) == LIST_SUCCESS);
    ASSERT_TEST(listInsertLast(list, student4) == LIST_SUCCESS);
    ASSERT_TEST(listInsertLast(list, student5) == LIST_SUCCESS);

    ASSERT_TEST(listSort(list, compareAverage) == LIST_SUCCESS);
    Student test_student = listGetFirst(list);
    ASSERT_TEST(test_student->id == id[4]);
    ASSERT_TEST(strcmp(test_student->name, name[4]) == 0);
    ASSERT_TEST(test_student->average == average[4]);
    test_student = listGetNext(list);
    ASSERT_TEST(test_student->id == id[5]);
    ASSERT_TEST(strcmp(test_student->name, name[5]) == 0);
    ASSERT_TEST(test_student->average == average[5]);
    test_student = listGetNext(list);
    ASSERT_TEST(test_student->id == id[0]);
    ASSERT_TEST(strcmp(test_student->name, name[0]) == 0);
    ASSERT_TEST(test_student->average == average[0]);
    test_student = listGetNext(list);
    ASSERT_TEST(test_student->id == id[1]);
    ASSERT_TEST(strcmp(test_student->name, name[1]) == 0);
    ASSERT_TEST(test_student->average == average[1]);
    test_student = listGetNext(list);
    ASSERT_TEST(test_student->id == id[2]);
    ASSERT_TEST(strcmp(test_student->name, name[2]) == 0);
    ASSERT_TEST(test_student->average == average[2]);
    test_student = listGetNext(list);
    ASSERT_TEST(test_student->id == id[3]);
    ASSERT_TEST(strcmp(test_student->name, name[3]) == 0);
    ASSERT_TEST(test_student->average == average[3]);

    listGetFirst(list);
    listGetNext(list);
    listGetNext(list);
    ASSERT_TEST(listSort(list, compareNames) == LIST_SUCCESS);
    test_student = listGetCurrent(list);
    ASSERT_TEST(test_student->id == id[2]);

    test_student = listGetFirst(list);
    ASSERT_TEST(test_student->id == id[0]);
    ASSERT_TEST(strcmp(test_student->name, name[0]) == 0);
    ASSERT_TEST(test_student->average == average[0]);
    test_student = listGetNext(list);
    ASSERT_TEST(test_student->id == id[3]);
    ASSERT_TEST(strcmp(test_student->name, name[3]) == 0);
    ASSERT_TEST(test_student->average == average[3]);
    test_student = listGetNext(list);
    ASSERT_TEST(test_student->id == id[2]);
    ASSERT_TEST(strcmp(test_student->name, name[2]) == 0);
    ASSERT_TEST(test_student->average == average[2]);
    test_student = listGetNext(list);
    ASSERT_TEST(test_student->id == id[1]);
    ASSERT_TEST(strcmp(test_student->name, name[1]) == 0);
    ASSERT_TEST(test_student->average == average[1]);
    test_student = listGetNext(list);
    ASSERT_TEST(test_student->id == id[4]);
    ASSERT_TEST(strcmp(test_student->name, name[4]) == 0);
    ASSERT_TEST(test_student->average == average[4]);
    test_student = listGetNext(list);
    ASSERT_TEST(test_student->id == id[5]);
    ASSERT_TEST(strcmp(test_student->name, name[5]) == 0);
    ASSERT_TEST(test_student->average == average[5]);

    return true;
}

static bool testListFilter() {
    int id[6] = {308571546, 311242440, 21620431, 777777777, 1, 23};
    char *name[6] = {"Adi Reznik", "Shahak Ben Kalifa", "Sagi Barazani",
                     "Horef Cenzor", "aaa", "abc"};
    double average[6] = {88.9, 91.00, 93.5, 100, 30.21, 55.5};

    Student student0 = studentCreate(id[0], name[0], average[0]);
    Student student1 = studentCreate(id[1], name[1], average[1]);
    Student student2 = studentCreate(id[2], name[2], average[2]);
    Student student3 = studentCreate(id[3], name[3], average[3]);
    Student student4 = studentCreate(id[4], name[4], average[4]);
    Student student5 = studentCreate(id[5], name[5], average[5]);

    List list = listCreate(copyStudent, freeStudent);

    ASSERT_TEST(listInsertLast(list, student0) == LIST_SUCCESS);
    ASSERT_TEST(listInsertLast(list, student1) == LIST_SUCCESS);
    ASSERT_TEST(listInsertLast(list, student2) == LIST_SUCCESS);
    ASSERT_TEST(listInsertLast(list, student3) == LIST_SUCCESS);
    ASSERT_TEST(listInsertLast(list, student4) == LIST_SUCCESS);
    ASSERT_TEST(listInsertLast(list, student5) == LIST_SUCCESS);

    char *key1 = "Horef Cenzor";
    List filtered1 = listFilter(list, areNameEqual, key1);
    ASSERT_TEST(listGetSize(filtered1) == 1);
    Student test_student = listGetFirst(filtered1);
    ASSERT_TEST(test_student->id == id[3]);
    ASSERT_TEST(strcmp(test_student->name, name[3]) == 0);
    ASSERT_TEST(test_student->average == average[3]);

    listDestroy(filtered1);

    double key2 = 91;
    List filtered2 = listFilter(list, isAverageGreaterThan, &key2);
    ASSERT_TEST(listGetSize(filtered2) == 2);
    test_student = listGetFirst(filtered2);
    ASSERT_TEST(test_student->id == id[2]);
    ASSERT_TEST(strcmp(test_student->name, name[2]) == 0);
    ASSERT_TEST(test_student->average == average[2]);

    listDestroy(filtered2);

    double key3 = 101;
    List filtered3 = listFilter(list, isAverageGreaterThan, &key3);
    ASSERT_TEST(listGetSize(filtered3) == 0);
    ASSERT_TEST(listGetFirst(filtered3) == NULL);

    listDestroy(filtered3);

    listDestroy(list);
    return true;
}

static bool testListClear() {
    int id[6] = {308571546, 311242440, 21620431, 777777777, 1, 23};
    char *name[6] = {"Adi Reznik", "Shahak Ben Kalifa", "Sagi Barazani",
                     "Horef Cenzor", "aaa", "abc"};
    double average[6] = {88.9, 91.00, 93.5, 100, 30.21, 55.5};

    Student student0 = studentCreate(id[0], name[0], average[0]);
    Student student1 = studentCreate(id[1], name[1], average[1]);
    Student student2 = studentCreate(id[2], name[2], average[2]);
    Student student3 = studentCreate(id[3], name[3], average[3]);
    Student student4 = studentCreate(id[4], name[4], average[4]);
    Student student5 = studentCreate(id[5], name[5], average[5]);

    ASSERT_TEST(listClear(NULL) == LIST_NULL_ARGUMENT);

    List list = listCreate(copyStudent, freeStudent);
    ASSERT_TEST(listClear(list) == LIST_SUCCESS);

    ASSERT_TEST(listInsertLast(list, student0) == LIST_SUCCESS);
    ASSERT_TEST(listInsertLast(list, student1) == LIST_SUCCESS);
    ASSERT_TEST(listInsertLast(list, student2) == LIST_SUCCESS);
    ASSERT_TEST(listInsertLast(list, student3) == LIST_SUCCESS);
    ASSERT_TEST(listInsertLast(list, student4) == LIST_SUCCESS);
    ASSERT_TEST(listInsertLast(list, student5) == LIST_SUCCESS);

    ASSERT_TEST(listClear(list) == LIST_SUCCESS);
    ASSERT_TEST(listGetFirst(list) == NULL);
    ASSERT_TEST(listGetSize(list) == 0);

    listDestroy(list);
    return true;
}

static bool testListDestroy() {
    int id[6] = {308571546, 311242440, 21620431, 777777777, 1, 23};
    char *name[6] = {"Adi Reznik", "Shahak Ben Kalifa", "Sagi Barazani",
                     "Horef Cenzor", "aaa", "abc"};
    double average[6] = {88.9, 91.00, 93.5, 100, 30.21, 55.5};

    Student student0 = studentCreate(id[0], name[0], average[0]);
    Student student1 = studentCreate(id[1], name[1], average[1]);
    Student student2 = studentCreate(id[2], name[2], average[2]);
    Student student3 = studentCreate(id[3], name[3], average[3]);
    Student student4 = studentCreate(id[4], name[4], average[4]);
    Student student5 = studentCreate(id[5], name[5], average[5]);

    listDestroy(NULL);

    List list1 = listCreate(copyStudent, freeStudent);
    listDestroy(list1);

    //destroy list with elements
    List list2 = listCreate(copyStudent, freeStudent);

    ASSERT_TEST(listInsertLast(list2, student0) == LIST_SUCCESS);
    ASSERT_TEST(listInsertLast(list2, student1) == LIST_SUCCESS);
    ASSERT_TEST(listInsertLast(list2, student2) == LIST_SUCCESS);
    ASSERT_TEST(listInsertLast(list2, student3) == LIST_SUCCESS);
    ASSERT_TEST(listInsertLast(list2, student4) == LIST_SUCCESS);
    ASSERT_TEST(listInsertLast(list2, student5) == LIST_SUCCESS);

    listDestroy(list1);
    return true;
}

int main(int argv, char **arc) {
    RUN_TEST(testListCreate);
    RUN_TEST(testListCopy);
    RUN_TEST(testListGetSize);
    RUN_TEST(testListGetFirst);
    RUN_TEST(testListGetNext);
    RUN_TEST(testListGetCurrent);
    RUN_TEST(testListInsertFirst);
//    RUN_TEST(testListInsertLast);
//    RUN_TEST(testListInsertBeforeCurrent);
//    RUN_TEST(testListInsertAfterCurrent);
//    RUN_TEST(testListRemoveCurrent);
//    RUN_TEST(testListSort);
//    RUN_TEST(testListFilter);
//    RUN_TEST(testListClear);
//    RUN_TEST(testListDestroy);
    return 0;
}

