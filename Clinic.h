#pragma once
#ifndef PATIENTQUEUE_H
#define PATIENTQUEUE_H

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

class Node {
public:

    std::string data;
    Node* next;
    Node(const std::string& data) : data(data), next(nullptr) {}
};


class LinkedList {
protected:
    Node* head;
    Node* tail;

public:
    LinkedList() : head(nullptr), tail(nullptr) {}

    Node* getHead() const { return head; }
    Node* getTail() const { return tail; }

    void setTail(Node* newTail) { tail = newTail; }

    virtual ~LinkedList() {
        while (head) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }
};




class Queue : public LinkedList {
public:
    bool cancelPatient(const std::string& ssn, std::string& removedPatient) {
        Node* current = head;
        Node* prev = nullptr;

        // Traverse the queue to find the patient by SSN
        while (current) {
            std::istringstream ss(current->data);
            std::string firstName, lastName, patientSSN, type;
            ss >> firstName >> lastName >> patientSSN >> type;

            if (patientSSN == ssn) {
                // Patient found
                removedPatient = firstName + " " + lastName;

                // Remove the node
                if (!prev) { // Removing the head
                    head = current->next;
                    if (!head) {
                        tail = nullptr; // Queue becomes empty
                    }
                }
                else { // Removing a node in the middle or end
                    prev->next = current->next;
                    if (!current->next) {
                        tail = prev; // Update tail if the last node is removed
                    }
                }

                delete current; // Free memory
                return true;    // Indicate patient was found and removed
            }

            // Move to the next node
            prev = current;
            current = current->next;
        }

        return false; // Patient not found
    }

    void enqueueCritical(const std::string& data) {
        Node* newNode = new Node(data);

        // If the queue is empty, add as the only node
        if (!head) {
            head = tail = newNode;
            return;
        }

        // Find the last critically ill patient in the queue
        Node* current = head;
        Node* prev = nullptr;
        while (current && current->data.back() == 'C') { // Assuming 'C' marks critical
            prev = current;
            current = current->next;
        }

        // Insert the new node after the last critically ill patient
        if (!prev) { // No critically ill patients, insert at the head
            newNode->next = head;
            head = newNode;
        }
        else { // Insert after the last critical patient
            newNode->next = prev->next;
            prev->next = newNode;
        }

        // Update tail if the new node is added at the end
        if (!newNode->next) {
            tail = newNode;
        }
    }
    //std::string dequeue() {
    //    if (!head) {
    //        throw std::runtime_error("Queue is empty."); // Exception for empty queue
    //    }

    //    Node* temp = head;
    //    std::string data = temp->data;
    //    head = head->next; // Move head to the next node

    //    if (!head) { // If the queue becomes empty, set tail to nullptr
    //        tail = nullptr;
    //    }

    //    delete temp; // Free the memory of the removed node
    //    return data;
    //}


    // Method to display the contents of the queue
    void display() const {
        if (!head) {
            std::cout << "The queue is empty.\n";
            return;
        }

        Node* current = head;
        std::cout << "Patient List:\n";
        while (current) {
            std::cout << current->data << "\n"; // Assuming each patient's data is stored as a single string
            current = current->next;
        }
    }
    // Method to calculate and return the size of the queue

    int size() const {
        int count = 0;
        Node* current = head; // Start at the head of the queue
        std::cout << head->data;
        while (current) {
            count++;
            current = current->next;
        }
        return count;
    }
    void enqueue(const std::string& data) {
        Node* newNode = new Node(data);
        if (!tail) {
            head = tail = newNode;
        }
        else {
            tail->next = newNode;
            tail = newNode;
        }
    }

    std::string dequeue() {
        if (!head) throw std::runtime_error("Queue is empty!");

        Node* temp = head;
        std::string data = head->data;
        head = head->next;

        if (!head) {
            tail = nullptr; // Update tail if the queue is empty
        }

        delete temp;
        return data;
    }
};

#endif

