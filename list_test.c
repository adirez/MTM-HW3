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

static bool testListCreate() {
    ASSERT_TEST(listCreate(NULL, NULL) == NULL);
    ASSERT_TEST(listCreate(copyStudent, NULL) == NULL);
    ASSERT_TEST(listCreate(NULL, freeStudent) == NULL);

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

static bool testListCopy() {

    List list = listCreate(copyStudent, freeStudent);
    List list_copy = listCopy(list);
    ASSERT_TEST(listGetCurrent(list_copy) == NULL);
    ASSERT_TEST(listGetFirst(list_copy) == NULL);
    ASSERT_TEST(listGetSize(list_copy) == 0);

    listDestroy(list_copy);

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

    List list_copy2 = listCopy(list);
    ASSERT_TEST(listGetCurrent(list_copy2) == NULL);
    ASSERT_TEST(listGetSize(list_copy2) == 6);

    Student test_student = listGetFirst(list_copy2);
    ASSERT_TEST(test_student->id == id[0]);
    ASSERT_TEST(strcmp(test_student->name, name[0]) == 0);
    ASSERT_TEST(test_student->average == average[0]);

    for (int i = 1; i < 5; ++i) {
        test_student = listGetNext(list_copy2);
        ASSERT_TEST(test_student->id == id[i]);
        ASSERT_TEST(strcmp(test_student->name, name[i]) == 0);
        ASSERT_TEST(test_student->average == average[i]);
    }

    listGetFirst(list);
    listGetNext(list);
    listGetNext(list);
    List list_copy3 = listCopy(list);
    test_student = listGetCurrent(list_copy3);
    ASSERT_TEST(test_student->id == id[2]);
    ASSERT_TEST(strcmp(test_student->name, name[2]) == 0);
    ASSERT_TEST(test_student->average == average[2]);

    return true;
}

static bool testListGetSize() {
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

    ASSERT_TEST(listGetSize(NULL) == -1);
    ASSERT_TEST(listGetSize(list) == 0);
    ASSERT_TEST(listInsertFirst(NULL, student1) == LIST_NULL_ARGUMENT);
    ASSERT_TEST(listGetSize(list) == 0);
    ASSERT_TEST(listInsertLast(list, student0) == LIST_SUCCESS);
    listGetFirst(list);
    ASSERT_TEST(listInsertAfterCurrent(list, student1) == LIST_SUCCESS);
    ASSERT_TEST(listInsertBeforeCurrent(list, student2) == LIST_SUCCESS);
    ASSERT_TEST(listGetSize(list) == 3);
    ASSERT_TEST(listInsertFirst(list, student3) == LIST_SUCCESS);
    listGetNext(list);
    ASSERT_TEST(listInsertLast(list, student4) == LIST_SUCCESS);
    ASSERT_TEST(listInsertFirst(list, student5) == LIST_SUCCESS);
    ASSERT_TEST(listGetSize(list) == 6);

    ASSERT_TEST(listRemoveCurrent(list) == LIST_SUCCESS);
    ASSERT_TEST(listGetSize(list) == 5);

    listDestroy(list);
    return true;
}

static bool testListGetFirst() {
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

    ASSERT_TEST(listGetFirst(NULL) == NULL);
    ASSERT_TEST(listGetFirst(list) == NULL);
    ASSERT_TEST(listInsertFirst(NULL, student1) == LIST_NULL_ARGUMENT);
    ASSERT_TEST(listGetFirst(list) == NULL);
    ASSERT_TEST(listInsertLast(list, student0) == LIST_SUCCESS);
    Student test_student = listGetFirst(list);
    ASSERT_TEST(test_student->id == id[0]);
    ASSERT_TEST(strcmp(test_student->name, name[0]) == 0);
    ASSERT_TEST(test_student->average == average[0]);

    ASSERT_TEST(listInsertAfterCurrent(list, student1) == LIST_SUCCESS);
    ASSERT_TEST(listInsertBeforeCurrent(list, student2) == LIST_SUCCESS);
    test_student = listGetFirst(list);
    ASSERT_TEST(test_student->id == id[2]);
    ASSERT_TEST(strcmp(test_student->name, name[2]) == 0);
    ASSERT_TEST(test_student->average == average[2]);

    ASSERT_TEST(listInsertFirst(list, student3) == LIST_SUCCESS);
    listGetNext(list);
    ASSERT_TEST(listInsertLast(list, student4) == LIST_SUCCESS);
    ASSERT_TEST(listInsertAfterCurrent(list, student5) == LIST_SUCCESS);
    test_student = listGetFirst(list);
    ASSERT_TEST(test_student->id == id[3]);
    ASSERT_TEST(strcmp(test_student->name, name[3]) == 0);
    ASSERT_TEST(test_student->average == average[3]);

    listGetFirst(list);
    ASSERT_TEST(listRemoveCurrent(list) == LIST_SUCCESS);
    test_student = listGetFirst(list);
    ASSERT_TEST(test_student->id == id[2]);
    ASSERT_TEST(strcmp(test_student->name, name[2]) == 0);
    ASSERT_TEST(test_student->average == average[2]);

    listDestroy(list);
    return true;
}

static bool testListGetNext() {
    int id[4] = {308571546, 311242440, 21620431, 777777777};
    char *name[4] = {"Adi Reznik", "Shahak Ben Kalifa", "Sagi Barazani",
                     "Horef Cenzor"};
    double average[4] = {88.9, 91.00, 93.5, 100};

    Student student0 = studentCreate(id[0], name[0], average[0]);
    Student student1 = studentCreate(id[1], name[1], average[1]);
    Student student2 = studentCreate(id[2], name[2], average[2]);
    Student student3 = studentCreate(id[3], name[3], average[3]);

    List list = listCreate(copyStudent, freeStudent);

    ASSERT_TEST(listGetNext(NULL) == NULL);
    ASSERT_TEST(listGetNext(list) == NULL);
    ASSERT_TEST(listInsertFirst(NULL, student1) == LIST_NULL_ARGUMENT);
    ASSERT_TEST(listGetFirst(list) == NULL);
    ASSERT_TEST(listInsertLast(list, student0) == LIST_SUCCESS);
    ASSERT_TEST(listInsertLast(list, student1) == LIST_SUCCESS);
    listGetFirst(list);
    Student test_student = listGetNext(list);
    ASSERT_TEST(test_student->id == id[1]);

    ASSERT_TEST(listInsertAfterCurrent(list, student3) == LIST_SUCCESS);
    ASSERT_TEST(listInsertBeforeCurrent(list, student2) == LIST_SUCCESS);


    listDestroy(list);
    return true;
}
//TODO: finish implementing
static bool testListGetCurrent() {
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

    ASSERT_TEST(listGetFirst(NULL) == NULL);
    ASSERT_TEST(listGetFirst(list) == NULL);
    ASSERT_TEST(listInsertFirst(NULL, student1) == LIST_NULL_ARGUMENT);
    ASSERT_TEST(listGetFirst(list) == NULL);
    ASSERT_TEST(listInsertLast(list, student0) == LIST_SUCCESS);
    Student test_student = listGetFirst(list);
    ASSERT_TEST(test_student->id == id[0]);
    ASSERT_TEST(strcmp(test_student->name, name[0]) == 0);
    ASSERT_TEST(test_student->average == average[0]);

    ASSERT_TEST(listInsertAfterCurrent(list, student1) == LIST_SUCCESS);
    ASSERT_TEST(listInsertBeforeCurrent(list, student2) == LIST_SUCCESS);
    test_student = listGetFirst(list);
    ASSERT_TEST(test_student->id == id[2]);
    ASSERT_TEST(strcmp(test_student->name, name[2]) == 0);
    ASSERT_TEST(test_student->average == average[2]);

    ASSERT_TEST(listInsertFirst(list, student3) == LIST_SUCCESS);
    listGetNext(list);
    ASSERT_TEST(listInsertLast(list, student4) == LIST_SUCCESS);
    ASSERT_TEST(listInsertAfterCurrent(list, student5) == LIST_SUCCESS);
    test_student = listGetFirst(list);
    ASSERT_TEST(test_student->id == id[3]);
    ASSERT_TEST(strcmp(test_student->name, name[3]) == 0);
    ASSERT_TEST(test_student->average == average[3]);

    listGetFirst(list);
    ASSERT_TEST(listRemoveCurrent(list) == LIST_SUCCESS);
    test_student = listGetFirst(list);
    ASSERT_TEST(test_student->id == id[2]);
    ASSERT_TEST(strcmp(test_student->name, name[2]) == 0);
    ASSERT_TEST(test_student->average == average[2]);

    listDestroy(list);
    return true;
}


static bool testListInsertFirst() {
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

    ASSERT_TEST(listInsertFirst(NULL, student1) == LIST_NULL_ARGUMENT);
    ASSERT_TEST(listInsertFirst(list, student0) == LIST_SUCCESS);
    ASSERT_TEST(listInsertFirst(list, student1) == LIST_SUCCESS);
    ASSERT_TEST(listInsertFirst(list, student2) == LIST_SUCCESS);
    ASSERT_TEST(listInsertFirst(list, student3) == LIST_SUCCESS);
    ASSERT_TEST(listInsertFirst(list, student4) == LIST_SUCCESS);
    ASSERT_TEST(listInsertFirst(list, student5) == LIST_SUCCESS);

    ASSERT_TEST(listGetSize(list) == 6);

    Student test_student = listGetFirst(list);
    ASSERT_TEST(test_student->id == id[5]);

    for (int i = 4; i > 0; --i) {
        test_student = listGetNext(list);
        ASSERT_TEST(test_student->id == id[i]);
    }

    listDestroy(list);
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
    RUN_TEST(testListFilter);
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
    RUN_TEST(testListClear);
    RUN_TEST(testListDestroy);
    return 0;
}

