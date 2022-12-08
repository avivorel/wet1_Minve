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
};
    //void printList(){
    //    Node<T>* temp = head;
    //    while(temp != NULL){
    //        cout << temp->data << " ";
    //        temp = temp->next;
    //    }
     //   cout << endl;
    //}
#endif //SOLUTION_LINKEDLIST_H
