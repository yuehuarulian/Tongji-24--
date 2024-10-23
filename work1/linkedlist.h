#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <iostream>
#include <sstream>
using namespace std;
struct Node
{
    int data;
    Node *next;
    Node *prev;
    Node(int val) : data(val), next(nullptr), prev(nullptr) {}
};

class SinglyLinkedList
{
public:
    SinglyLinkedList() : head(nullptr), len(0) {}

    // 建立
    void clear();

    // 头部插入
    void insert(int);

    // 删除指定
    bool remove(int);

    // 寻找第一个指定值为val的节点?
    Node *find(int, int&);

    // 显示列表??
    string single_display();
    string circular_display();
    string double_display();

private:
    Node *head;
    int len;
};

class CircularLinkedList
{
public:
    CircularLinkedList() : head(nullptr), tail(nullptr), len(0) {}

    // 头部插入
    void insert(int val)
    {
        Node *newnode = new Node(val);
        // 如果是第一个链??
        if (head == nullptr)
        {
            head = newnode;
            tail = newnode;
            head->next = head;
        }
        else
        {
            newnode->next = head;
            tail->next = newnode;
            head = newnode;
        }
        ++len;
    }

    // 删除指定??
    void remove(int val)
    {
        if (tail == nullptr)
            return;
        Node *current = head;
        Node *prev = tail;
        int count = 1;
        do
        {
            if (current->data == val)
            {
                prev->next = current->next;
                // 如果只剩下一个节??
                if (current == current->next)
                    head = tail = nullptr;
                // 如果删除的是链表最后一个??
                else if (current == tail)
                    tail = prev;
                // 如果删除的是链表第一个??
                else if (current == head)
                    head = current->next;
                delete current;
                len--;
                return;
            }
            prev = current;
            current = current->next;
            count++;
        } while (current != head);
    }

    // 寻找第一个指定值为val的指??
    Node *find(int val)
    {
        if (head == nullptr)
            return nullptr;
        Node *current = head;
        do
        {
            if (current->data == val)
                return current;
            current = current->next;
        } while (current != head);
        return nullptr;
    }

    void display()
    {
        if (head == nullptr)
            return;
        Node *current = tail->next;
        do
        {
            cout << current->data << " ";
            current = current->next;
        } while (current != head);
        cout << endl;
    }

private:
    Node *head;
    Node *tail;
    int len;
};

class DoublyLinkedList
{
public:
    DoublyLinkedList() : head(nullptr), len(0) {}

    void insert(int val)
    {
        Node *newnode = new Node(val);
        if (head == nullptr)
        {
            newnode->next = newnode;
            newnode->prev = newnode;
            head = newnode;
        }
        else
        {
            newnode->next = head;
            newnode->prev = head->prev;
            head->prev = newnode;
            head = newnode;
            head->prev->next = head;
        }
        ++len;
    }

    void remove(int val)
    {
        Node *current = head;
        int count = 1;
        do
        {
            if (current->data == val)
            {
                current->prev->next = current->next;
                current->next->prev = current->prev;
                // 如果只剩下一个节??
                if (current == current->next)
                    head = nullptr;
                // 如果删除的是链表第一个??
                else if (current == head)
                    head = current->next;
                delete current;
                len--;
                return;
            }
            current = current->next;
            count++;
        } while (current != head);
    }

    Node *find(int val)
    {
        if (head == nullptr)
            return nullptr;
        Node *current = head;
        do
        {
            if (current->data == val)
                return current;
            current = current->next;
        } while (current != head);
        return nullptr;
    }

    void display()
    {
        if (head == nullptr)
            return;
        Node *current = head;
        do
        {
            cout << current->data << " ";
            current = current->next;
        } while (current != head);
        cout << endl;
    }

private:
    Node *head;
    int len;
};


#endif // LINKEDLIST_H
