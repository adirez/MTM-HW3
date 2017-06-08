//
// Created by adire on 06-Jun-17.
//

#include "list_mtm1.h"
#include <assert.h>
#include <stdlib.h>

typedef struct list_node {
    ListElement data;
    struct list_node *next;
} *ListNode;

struct list_t {
    ListNode head;
    ListNode cur_node;
    int list_size;
    CopyListElement copy;
    FreeListElement free;
};

static ListNode listNodeCreate(List list, ListElement element) {
    assert(copyElement != NULL && element != NULL);

    ListNode new_node = malloc(sizeof(*new_node));
    if (new_node == NULL) {
        return NULL;
    }
    new_node->next = NULL;

    ListElement new_element = list->copy(element);
    if (new_element == NULL) {
        free(new_node);
        return NULL;
    }
    new_node->data = new_element;
    list->list_size += 1;
    return new_node;
}

static ListNode listGetPreviousNode(List list) {
    assert(list != NULL);

    ListNode ptr_node = list->head;
    while (ptr_node->next != list->cur_node) {
        ptr_node = ptr_node->next;
    }
    return ptr_node;
}

static void swapElements(ListNode node1, ListNode node2) {
    assert(list != NULL && element1 != NULL && element2 != NULL);

    ListElement tmp_element = node1->data;
    node1->data = node2->data;
    node2->data = tmp_element;
}

List listCreate(CopyListElement copyElement, FreeListElement freeElement) {
    assert(copyElement != NULL && freeElement != NULL);
    if (copyElement == NULL || freeElement == NULL) {
        return NULL;
    }

    List list = malloc(sizeof(*list));
    if (list == NULL) {
        return NULL;
    }
    list->head = NULL;
    list->cur_node = list->head;
    list->copy = copyElement;
    list->free = freeElement;
    list->list_size = 0;
    return list;
}


List listCopy(List list) {
    assert(list != NULL);

    if (list == NULL) {
        return NULL;
    }
    List new_list = listCreate(list->copy, list->free);
    if (new_list == NULL) {
        return NULL;
    }

    ListNode ptr_node_src = list->head;
    while (ptr_node_src != NULL) {
        ListResult result = listInsertLast(new_list, ptr_node_src->data);
        if (result != LIST_SUCCESS) {
            listDestroy(new_list);
            return NULL;
        }
        if(list->cur_node == ptr_node_src){
            ListNode tmp_ptr = new_list->head;
            while (tmp_ptr->next != NULL){
                tmp_ptr = tmp_ptr->next;
            }
            new_list->cur_node = tmp_ptr;
        }
        ptr_node_src = ptr_node_src->next;
    }
    return new_list;
}

int listGetSize(List list) {
    assert(list != NULL);

    if (list == NULL) {
        return -1;
    }
    return list->list_size;
}

ListElement listGetFirst(List list) {
    assert(list != NULL);

    if (list == NULL) {
        return NULL;
    }
    list->cur_node = list->head;
    if (list->head == NULL) {
        return NULL;
    }
    return list->cur_node->data;
}

ListElement listGetNext(List list) {
    assert(list != NULL);

    if (list == NULL || list->cur_node == NULL) {
        return NULL;
    }
    list->cur_node = list->cur_node->next;
    if (list->cur_node == NULL) {
        return NULL;
    }
    return list->cur_node->data;
}

ListElement listGetCurrent(List list) {
    assert(list != NULL);

    if (list == NULL || list->cur_node == NULL) {
        return NULL;
    }
    return list->cur_node->data;
}

ListResult listInsertFirst(List list, ListElement element) {
    assert(list != NULL && element != NULL);

    if (list == NULL) {
        return LIST_NULL_ARGUMENT;
    }
    ListNode new_node = listNodeCreate(list, element);
    if (new_node == NULL) {
        return LIST_OUT_OF_MEMORY;
    }
    new_node->next = list->head;
    list->head = new_node;
    return LIST_SUCCESS;
}

ListResult listInsertLast(List list, ListElement element) {
    assert(list != NULL && element != NULL);

    if (list == NULL) {
        return LIST_NULL_ARGUMENT;
    }

    ListNode new_node = listNodeCreate(list, element);
    if (new_node == NULL) {
        return LIST_OUT_OF_MEMORY;
    }
    if (list->head == NULL) {
        list->head = new_node;
        return LIST_SUCCESS;
    }

    ListNode ptr_node = list->head;
    while (ptr_node->next != NULL) {
        ptr_node = ptr_node->next;
    }
    ptr_node->next = new_node;
    return LIST_SUCCESS;
}

ListResult listInsertBeforeCurrent(List list, ListElement element) {
    assert(list != NULL && element != NULL);

    if (list == NULL) {
        return LIST_NULL_ARGUMENT;
    }
    //TODO: check if the iterator can accidentally point on non element
    if (list->cur_node == NULL) {
        return LIST_INVALID_CURRENT;
    }
    ListNode new_node = listNodeCreate(list, element);
    if (new_node == NULL) {
        return LIST_OUT_OF_MEMORY;
    }
    if (list->cur_node == list->head) {
        new_node->next = list->head;
        list->head = new_node;
    } else {
        ListNode previous_node = listGetPreviousNode(list);
        previous_node->next = new_node;
        new_node->next = list->cur_node;
    }
    return LIST_SUCCESS;

}

ListResult listInsertAfterCurrent(List list, ListElement element) {
    assert(list != NULL && element != NULL);

    if (list == NULL) {
        return LIST_NULL_ARGUMENT;
    }
    //TODO: check if the iterator can accidentally point on non element
    if (list->cur_node == NULL) {
        return LIST_INVALID_CURRENT;
    }
    ListNode new_node = listNodeCreate(list, element);
    if (new_node == NULL) {
        return LIST_OUT_OF_MEMORY;
    }
    ListNode tmp_node = list->cur_node->next;
    list->cur_node->next = new_node;
    new_node->next = tmp_node;

    return LIST_SUCCESS;
}

ListResult listRemoveCurrent(List list) {
    assert(list != NULL);

    if (list == NULL) {
        return LIST_NULL_ARGUMENT;
    }
    if (list->cur_node == NULL) {
        return LIST_INVALID_CURRENT;
    }
    if (list->cur_node == list->head) {
        ListNode tmp_node = list->head->next;
        list->free(list->cur_node->data);
        free(list->cur_node);
        list->head = tmp_node;
    } else {
        ListNode previous_node = listGetPreviousNode(list);
        previous_node->next = list->cur_node->next;
        list->free(list->cur_node->data);
        free(list->cur_node);
    }
    list->cur_node = NULL;
    list->list_size -= 1;
    return LIST_SUCCESS;
}

ListResult listSort(List list, CompareListElements compareElement) {
    assert(list != NULL && compareElement != NULL);

    if (list == NULL || compareElement == NULL) {
        return LIST_NULL_ARGUMENT;
    }

    bool swapped = true;
    int n = 0;
    ListNode ptr_node;

    while (swapped) {
        swapped = false;
        ptr_node = list->head;
        for (int i = 0; i < (list->list_size) - n - 1; ++i) {
            if (compareElement(ptr_node->data, ptr_node->next->data) > 0) {
                swapElements(ptr_node, ptr_node->next);
                swapped = true;
            }
            ptr_node = ptr_node->next;
        }
        n++;
    }

    return LIST_SUCCESS;
}

List listFilter(List list, FilterListElement filterElement, ListFilterKey key) {
    assert(list != NULL && filterElement != NULL && key != NULL);
    if (list == NULL || filterElement == NULL || key == NULL) {
        return NULL;
    }

    List new_list = listCopy(list);
    if (new_list == NULL) {
        return NULL;
    }

    ListNode to_check_node = new_list->head;
    while (to_check_node != NULL) {
        ListNode store_next_node = to_check_node->next;
        if (filterElement(to_check_node->data, key) == false) {
            new_list->list_size -= 1;
            if (to_check_node == new_list->head) {
                new_list->free(to_check_node->data);
                free(to_check_node);
                new_list->head = store_next_node;
            } else {
                new_list->cur_node = to_check_node;
                ListNode previous_node = listGetPreviousNode(new_list);
                previous_node->next = store_next_node;
                new_list->free(to_check_node->data);
                free(to_check_node);
            }
        }
        to_check_node = store_next_node;
    }
    new_list->cur_node = new_list->head;

    return new_list;
}

ListResult listClear(List list) {
    assert(list != NULL);

    if (list == NULL) {
        return LIST_NULL_ARGUMENT;
    }
    list->cur_node = list->head;
    while (list->cur_node != NULL) {
        ListResult result = listRemoveCurrent(list);
        if (result != LIST_SUCCESS) {
            return result;
        }
        list->cur_node = list->head;
    }
    return LIST_SUCCESS;
}

void listDestroy(List list) {
    assert(list != NULL);

    if (list == NULL) {
        return;
    }

    listClear(list);
    free(list);
}