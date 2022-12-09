//
// Created by darkm on 08/12/2022.
//

#ifndef SOLUTION_LINKEDLIST_H
#define SOLUTION_LINKEDLIST_H

template<typename T> class Node{
public:
    T data;
    Node* next;
    Node* prev;
    Node(T data){
        this->data = data;
        next = NULL;
        prev = NULL;
    }
    void setData(T newdata){
        data = newdata;
    }
};

template<typename T>
class LinkedList {
private:
    Node<T> *head;
    Node<T> *tail;
public:
    LinkedList() {
        head = NULL;
        tail = NULL;
    }

    void insert(T data) {
        Node<T> *newNode = new Node<T>(data);
        if (head == NULL) {
            head = newNode;
            tail = newNode;
        } else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
    }
    Node<T>* getHead(){
        return this->head;
    }
    void remove(Node<T> *node) {
        // if the node to be removed is the head
        if (node == head) {
            head = node->next;
        } else {
            node->prev->next = node->next;
        }

        // if the node to be removed is the tail
        if (node == tail) {
            tail = node->prev;
        } else {
            node->next->prev = node->prev;
        }

        delete node;
    }
    void deleteList() {
        Node<T> *current = head;
        while (current != NULL) {
            Node<T> *next = current->next;
            delete current;
            current = next;
        }

        head = NULL;
        tail = NULL;
    }
};

#endif //SOLUTION_LINKEDLIST_H
