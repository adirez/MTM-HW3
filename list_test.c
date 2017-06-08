#include "test_utilities.h"
#include "list_mtm1.h"
#include <string.h>
#include <stdlib.h>

typedef struct student_t {
    int id;
    char *name;
    double average;
} *Student;

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
    return (int) (student1->average - student2->average);
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

    //test return value for NULL params and list size
    ASSERT_TEST(listInsertFirst(NULL, NULL) == LIST_NULL_ARGUMENT);
    ASSERT_TEST(listInsertFirst(NULL, student0) == LIST_NULL_ARGUMENT);

    //TODO: change
//    ASSERT_TEST(listInsertFirst(list, NULL) == LIST_NULL_ARGUMENT);
    ASSERT_TEST(listGetSize(list) == 0);

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

    //insert another element and test that the iterator is still on that element
    ASSERT_TEST(listInsertFirst(list, student2) == LIST_SUCCESS);
    test_student = listGetCurrent(list);
    ASSERT_TEST(test_student->id == id[1]);

    ASSERT_TEST(listGetSize(list) == 3);

    listDestroy(list);
    return true;
}

static bool testListInsertLast() {
    List list = listCreate(copyStudent, freeStudent);
    int id[3] = {308571546, 311242440, 234122};
    char *name[3] = {"Adi Reznik", "Shahak Ben Kalifa", "MTM"};
    double average[3] = {88.9, 91.00, 71};
    Student student0 = studentCreate(id[0], name[0], average[0]);
    Student student1 = studentCreate(id[1], name[1], average[1]);
    Student student2 = studentCreate(id[2], name[2], average[2]);

    //test return value for NULL params and list size
    ASSERT_TEST(listInsertLast(NULL, NULL) == LIST_NULL_ARGUMENT);
    ASSERT_TEST(listInsertLast(NULL, student0) == LIST_NULL_ARGUMENT);
    //TODO: change
//    ASSERT_TEST(listInsertFirst(list, NULL) == LIST_NULL_ARGUMENT);
    ASSERT_TEST(listGetSize(list) == 0);

    //insert an element to the list and test all its fields
    ASSERT_TEST(listInsertLast(list, student0) == LIST_SUCCESS);
    Student test_student = listGetFirst(list);
    ASSERT_TEST(test_student->id == id[0]);
    ASSERT_TEST(strcmp(test_student->name, name[0]) == 0);
    ASSERT_TEST(test_student->average == average[0]);

    //insert another element and test that it is the last
    ASSERT_TEST(listInsertLast(list, student1) == LIST_SUCCESS);
    listGetFirst(list);
    test_student = listGetNext(list);
    ASSERT_TEST(test_student->id == id[1]);

    //insert another element and test that the iterator is still on that element
    ASSERT_TEST(listInsertLast(list, student2) == LIST_SUCCESS);
    test_student = listGetCurrent(list);
    ASSERT_TEST(test_student->id == id[1]);

    ASSERT_TEST(listGetSize(list) == 3);

    listDestroy(list);
    return true;
}

static bool testListInsertBeforeCurrent() {
    List list = listCreate(copyStudent, freeStudent);
    int id[3] = {308571546, 311242440, 234122};
    char *name[3] = {"Adi Reznik", "Shahak Ben Kalifa", "MTM"};
    double average[3] = {88.9, 91.00, 71};
    Student student0 = studentCreate(id[0], name[0], average[0]);
    Student student1 = studentCreate(id[1], name[1], average[1]);
    Student student2 = studentCreate(id[2], name[2], average[2]);

    //test return value for NULL params and list size
    ASSERT_TEST(listInsertBeforeCurrent(NULL, NULL) == LIST_NULL_ARGUMENT);
    ASSERT_TEST(listInsertBeforeCurrent(NULL, student0) == LIST_NULL_ARGUMENT);
    //TODO: change
//    ASSERT_TEST(listInsertFirst(list, NULL) == LIST_NULL_ARGUMENT);
    ASSERT_TEST(listGetSize(list) == 0);

    //test return value for invalid iterator for empty and non empty lists
    // and list size
    ASSERT_TEST(listInsertBeforeCurrent(list, student0) == LIST_INVALID_CURRENT);
    listInsertFirst(list, student0);
    ASSERT_TEST(listInsertBeforeCurrent(list, student0) == LIST_INVALID_CURRENT);

    //move iterator, insert an element to the list and test all its fields
    // and if it's the first
    listGetFirst(list);
    ASSERT_TEST(listInsertBeforeCurrent(list, student1) == LIST_SUCCESS);
    Student test_student = listGetFirst(list);
    ASSERT_TEST(test_student->id == id[1]);
    ASSERT_TEST(strcmp(test_student->name, name[1]) == 0);
    ASSERT_TEST(test_student->average == average[1]);

    //insert third element and test if it's in the middle
    listGetNext(list);
    ASSERT_TEST(listInsertBeforeCurrent(list, student2) == LIST_SUCCESS);
    listGetFirst(list);
    test_student = listGetNext(list);
    ASSERT_TEST(test_student->id == id[2]);

    //move iterator to the end of the list (NULL) and test the return value
    listGetNext(list);
    listGetNext(list);
    ASSERT_TEST(listInsertBeforeCurrent(list, student1) == LIST_INVALID_CURRENT);

    listDestroy(list);
    return true;
}

static bool testListInsertAfterCurrent() {
    List list = listCreate(copyStudent, freeStudent);
    int id[3] = {308571546, 311242440, 234122};
    char *name[3] = {"Adi Reznik", "Shahak Ben Kalifa", "MTM"};
    double average[3] = {88.9, 91.00, 71};
    Student student0 = studentCreate(id[0], name[0], average[0]);
    Student student1 = studentCreate(id[1], name[1], average[1]);
    Student student2 = studentCreate(id[2], name[2], average[2]);

    //test return value for NULL params and list size
    ASSERT_TEST(listInsertAfterCurrent(NULL, NULL) == LIST_NULL_ARGUMENT);
    ASSERT_TEST(listInsertAfterCurrent(NULL, student0) == LIST_NULL_ARGUMENT);
    //TODO: change
//    ASSERT_TEST(listInsertFirst(list, NULL) == LIST_NULL_ARGUMENT);
    ASSERT_TEST(listGetSize(list) == 0);

    //test return value for invalid iterator for empty and non empty list
    // with current iterator not valid and test list size
    ASSERT_TEST(listInsertAfterCurrent(list, student0) == LIST_INVALID_CURRENT);
    listInsertFirst(list, student0);
    ASSERT_TEST(listInsertAfterCurrent(list, student0) == LIST_INVALID_CURRENT);

    //move iterator, insert an element to the list and test all its fields
    // and if it's the second
    listGetFirst(list);
    ASSERT_TEST(listInsertAfterCurrent(list, student1) == LIST_SUCCESS);
    listGetFirst(list);
    Student test_student = listGetNext(list);
    ASSERT_TEST(test_student->id == id[1]);
    ASSERT_TEST(strcmp(test_student->name, name[1]) == 0);
    ASSERT_TEST(test_student->average == average[1]);

    //insert third element and test if it's in the middle
    listGetFirst(list);
    ASSERT_TEST(listInsertAfterCurrent(list, student2) == LIST_SUCCESS);
    listGetFirst(list);
    test_student = listGetNext(list);
    ASSERT_TEST(test_student->id == id[2]);

    //move iterator to the end of the list (NULL) and test the return value
    listGetNext(list);
    listGetNext(list);
    ASSERT_TEST(listInsertAfterCurrent(list, student1) == LIST_INVALID_CURRENT);

    listDestroy(list);
    return true;
}

static bool testListRemoveCurrent() {
    int id[3] = {308571546, 311242440, 234122};
    char *name[3] = {"Adi Reznik", "Shahak Ben Kalifa", "MTM"};
    double average[3] = {88.9, 91.00, 71};
    Student student0 = studentCreate(id[0], name[0], average[0]);
    Student student1 = studentCreate(id[1], name[1], average[1]);
    Student student2 = studentCreate(id[2], name[2], average[2]);

    //test return value for NULL param
    ASSERT_TEST(listRemoveCurrent(NULL) == LIST_NULL_ARGUMENT);
    //test return value for empty list
    List list = listCreate(copyStudent, freeStudent);
    ASSERT_TEST(listRemoveCurrent(list) == LIST_INVALID_CURRENT);
    //insert an element ans test if returns invalid iterator
    listInsertLast(list, student0);
    ASSERT_TEST(listRemoveCurrent(list) == LIST_INVALID_CURRENT);

    //move iterator to element and test that it was removed properly
    listGetFirst(list);
    ASSERT_TEST(listRemoveCurrent(list) == LIST_SUCCESS);
    ASSERT_TEST(listGetSize(list) == 0);
    ASSERT_TEST(listGetCurrent(list) == NULL);
    ASSERT_TEST(listGetFirst(list) == NULL);

    //insert 3 elements and test removal of middle, last and first
    listInsertLast(list, student0);
    listInsertLast(list, student1);
    listInsertLast(list, student2);

    listGetFirst(list);
    listGetNext(list);
    ASSERT_TEST(listRemoveCurrent(list) == LIST_SUCCESS);
    ASSERT_TEST(listGetSize(list) == 2);
    ASSERT_TEST(listGetCurrent(list) == NULL);
    Student test_student = listGetFirst(list);
    ASSERT_TEST(test_student->id == id[0]);
    test_student = listGetNext(list);
    ASSERT_TEST(test_student->id == id[2]);

    ASSERT_TEST(listRemoveCurrent(list) == LIST_SUCCESS);
    ASSERT_TEST(listGetSize(list) == 1);
    ASSERT_TEST(listGetCurrent(list) == NULL);
    test_student = listGetFirst(list);
    ASSERT_TEST(test_student->id == id[0]);

    ASSERT_TEST(listRemoveCurrent(list) == LIST_SUCCESS);
    ASSERT_TEST(listGetSize(list) == 0);
    ASSERT_TEST(listGetCurrent(list) == NULL);

    listDestroy(list);
    return true;
}

static bool testListSort() {
    //test return value for NULL param
    List list = listCreate(copyStudent, freeStudent);
    ASSERT_TEST(listSort(NULL, NULL) == LIST_NULL_ARGUMENT);
    ASSERT_TEST(listSort(list, NULL) == LIST_NULL_ARGUMENT);
    ASSERT_TEST(listSort(NULL, compareNames) == LIST_NULL_ARGUMENT);

    int id[7] = {308571546, 311242440, 234122, 123, 000, 654654, 2313};
    char *name[7] = {"Adi Reznik", "Shahak Ben Kalifa", "MTM", "Ran", "aaa",
                     "bbb", "abc"};
    double average[7] = {88.9, 91.00, 71, 91.1, 10, 10.4, 100};
    Student student0 = studentCreate(id[0], name[0], average[0]);
    Student student1 = studentCreate(id[1], name[1], average[1]);
    Student student2 = studentCreate(id[2], name[2], average[2]);
    Student student3 = studentCreate(id[3], name[3], average[3]);
    Student student4 = studentCreate(id[4], name[4], average[4]);
    Student student5 = studentCreate(id[5], name[5], average[5]);
    Student student6 = studentCreate(id[6], name[6], average[6]);

    //insert all the elements to the list
    listInsertLast(list, student0);
    listInsertLast(list, student1);
    listInsertLast(list, student2);
    listInsertLast(list, student3);
    listInsertLast(list, student4);
    listInsertLast(list, student5);
    listInsertLast(list, student6);


    //sort by double param and test the order of the elements after sort
    ASSERT_TEST(listSort(list, compareAverage) == LIST_SUCCESS);

    Student test_student = listGetFirst(list);
    ASSERT_TEST(test_student->id == id[4]);
    ASSERT_TEST(strcmp(test_student->name, name[4]) == 0);
    ASSERT_TEST(test_student->average == average[4]);
    test_student = listGetNext(list);
    ASSERT_TEST(test_student->average == average[5]);
    test_student = listGetNext(list);
    ASSERT_TEST(test_student->average == average[2]);
    test_student = listGetNext(list);
    ASSERT_TEST(test_student->average == average[0]);
    test_student = listGetNext(list);
    ASSERT_TEST(test_student->average == average[1]);
    test_student = listGetNext(list);
    ASSERT_TEST(test_student->average == average[3]);
    test_student = listGetNext(list);
    ASSERT_TEST(test_student->average == average[6]);

    //move iterator to the third element, sot by string param and test if all
    // the elements are in the right order and the iterator is still there
    listGetFirst(list);
    listGetNext(list);
    listGetNext(list);

    ASSERT_TEST(listSort(list, compareNames) == LIST_SUCCESS);

    test_student = listGetCurrent(list);
    ASSERT_TEST(test_student->id == id[3]);
    ASSERT_TEST(strcmp(test_student->name, name[3]) == 0);
    ASSERT_TEST(test_student->average == average[3]);
    test_student = listGetFirst(list);
    ASSERT_TEST(strcmp(test_student->name, name[0]) == 0);
    test_student = listGetNext(list);
    ASSERT_TEST(strcmp(test_student->name, name[2]) == 0);
    listGetNext(list);
    test_student = listGetNext(list);
    ASSERT_TEST(strcmp(test_student->name, name[1]) == 0);
    test_student = listGetNext(list);
    ASSERT_TEST(strcmp(test_student->name, name[4]) == 0);
    test_student = listGetNext(list);
    ASSERT_TEST(strcmp(test_student->name, name[6]) == 0);
    test_student = listGetNext(list);
    ASSERT_TEST(strcmp(test_student->name, name[5]) == 0);

    listDestroy(list);
    return true;
}

static bool testListFilter() {
    //test return value for NULL params
    List list = listCreate(copyStudent, freeStudent);
    double key = 55;
    ASSERT_TEST(listFilter(NULL, NULL, NULL) == NULL);
    ASSERT_TEST(listFilter(list, NULL, NULL) == NULL);
    ASSERT_TEST(listFilter(NULL, isAverageGreaterThan, NULL) == NULL);
    ASSERT_TEST(listFilter(NULL, NULL, &key) == NULL);
//    ASSERT_TEST(listFilter(list, isAverageGreaterThan, NULL) == NULL);
    ASSERT_TEST(listFilter(list, NULL, &key) == NULL);
    ASSERT_TEST(listFilter(NULL, isAverageGreaterThan, &key) == NULL);

    int id[7] = {308571546, 311242440, 234122, 123, 000, 654654, 2313};
    char *name[7] = {"Adi Reznik", "Shahak Ben Kalifa", "MTM", "Ran", "aaa",
                     "bbb", "abc"};
    double average[7] = {40.259, 55, 71, 91.1, 10, 10.4, 100};
    Student student0 = studentCreate(id[0], name[0], average[0]);
    Student student1 = studentCreate(id[1], name[1], average[1]);
    Student student2 = studentCreate(id[2], name[2], average[2]);
    Student student3 = studentCreate(id[3], name[3], average[3]);
    Student student4 = studentCreate(id[4], name[4], average[4]);
    Student student5 = studentCreate(id[5], name[5], average[5]);
    Student student6 = studentCreate(id[6], name[6], average[6]);

    //insert all the elements to the list
    listInsertLast(list, student0);
    listInsertLast(list, student1);
    listInsertLast(list, student2);
    listInsertLast(list, student3);
    listInsertLast(list, student4);
    listInsertLast(list, student5);
    listInsertLast(list, student6);

    //filter by double param
    List list_filtered = listFilter(list, isAverageGreaterThan, &key);
    ASSERT_TEST(listGetSize(list_filtered) == 3);
    //test if the iterator is to the first element
    Student test_student = listGetCurrent(list_filtered);
    ASSERT_TEST(test_student->id == id[2]);
    ASSERT_TEST(strcmp(test_student->name, name[2]) == 0);
    ASSERT_TEST(test_student->average == average[2]);

    //check the order of the filtered list
    test_student = listGetNext(list_filtered);
    ASSERT_TEST(test_student->id == id[3]);
    test_student = listGetNext(list_filtered);
    ASSERT_TEST(test_student->id == id[6]);
    ASSERT_TEST(listGetNext(list_filtered) == NULL);

    char* key2 = "Ran";
    //filter the filtered list by string param
    list_filtered = listFilter(list_filtered, areNameEqual, key2);
    ASSERT_TEST(listGetSize(list_filtered) == 1);
    test_student = listGetCurrent(list_filtered);
    ASSERT_TEST(test_student->id == id[3]);
    ASSERT_TEST(strcmp(test_student->name, name[3]) == 0);
    ASSERT_TEST(test_student->average == average[3]);

    ASSERT_TEST(listGetNext(list_filtered) == NULL);

    listDestroy(list);
    listDestroy(list_filtered);
    return true;
}

static bool testListClear() {
    //test return value for NULL param
    ASSERT_TEST(listClear(NULL) == LIST_NULL_ARGUMENT);
    //test return value for empty list
    List list = listCreate(copyStudent, freeStudent);
    ASSERT_TEST(listClear(list) == LIST_SUCCESS);

    int id[7] = {308571546, 311242440, 234122, 123, 000, 654654, 2313};
    char *name[7] = {"Adi Reznik", "Shahak Ben Kalifa", "MTM", "Ran", "aaa",
                     "bbb", "abc"};
    double average[7] = {88.9, 91.00, 71, 91.1, 10, 10.4, 100};
    Student student0 = studentCreate(id[0], name[0], average[0]);
    Student student1 = studentCreate(id[1], name[1], average[1]);
    Student student2 = studentCreate(id[2], name[2], average[2]);
    Student student3 = studentCreate(id[3], name[3], average[3]);
    Student student4 = studentCreate(id[4], name[4], average[4]);
    Student student5 = studentCreate(id[5], name[5], average[5]);
    Student student6 = studentCreate(id[6], name[6], average[6]);

    //insert all the elements to the list
    listInsertLast(list, student0);
    listInsertLast(list, student1);
    listInsertLast(list, student2);
    listInsertLast(list, student3);
    listInsertLast(list, student4);
    listInsertLast(list, student5);
    listInsertLast(list, student6);

    //test the return value and the success of list clear
    ASSERT_TEST(listClear(list) == LIST_SUCCESS);
    ASSERT_TEST(listGetFirst(list) == NULL);
    ASSERT_TEST(listGetSize(list) == 0);

    listDestroy(list);
    return true;
}

static bool testListDestroy() {
    //test NULL param doesn't crush
    listDestroy(NULL);
    //test empty list doesn't crush
    List list = listCreate(copyStudent, freeStudent);
    ASSERT_TEST(listClear(list) == LIST_SUCCESS);

    int id[7] = {308571546, 311242440, 234122, 123, 000, 654654, 2313};
    char *name[7] = {"Adi Reznik", "Shahak Ben Kalifa", "MTM", "Ran", "aaa",
                     "bbb", "abc"};
    double average[7] = {88.9, 91.00, 71, 91.1, 10, 10.4, 100};
    Student student0 = studentCreate(id[0], name[0], average[0]);
    Student student1 = studentCreate(id[1], name[1], average[1]);
    Student student2 = studentCreate(id[2], name[2], average[2]);
    Student student3 = studentCreate(id[3], name[3], average[3]);
    Student student4 = studentCreate(id[4], name[4], average[4]);
    Student student5 = studentCreate(id[5], name[5], average[5]);
    Student student6 = studentCreate(id[6], name[6], average[6]);

    //insert all the elements to the list
    listInsertLast(list, student0);
    listInsertLast(list, student1);
    listInsertLast(list, student2);
    listInsertLast(list, student3);
    listInsertLast(list, student4);
    listInsertLast(list, student5);
    listInsertLast(list, student6);

    listDestroy(list);
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
    RUN_TEST(testListInsertLast);
    RUN_TEST(testListInsertBeforeCurrent);
    RUN_TEST(testListInsertAfterCurrent);
    RUN_TEST(testListRemoveCurrent);
    RUN_TEST(testListSort);
    RUN_TEST(testListFilter);
    RUN_TEST(testListClear);
    RUN_TEST(testListDestroy);
    return 0;
}

