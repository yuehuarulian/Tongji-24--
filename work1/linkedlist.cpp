#include "linkedlist.h"

void SinglyLinkedList::clear()
{
    while (head != nullptr){
        Node *current = head;
        delete current;
        head = head->next;
    }
}

void SinglyLinkedList::insert(int val)
{
    Node *newnode = new Node(val);
    newnode->next = head;
    head = newnode;
    ++len;
}

bool SinglyLinkedList::remove(int val)
{
    Node *current = head;
    Node *prev = nullptr;
    int count = 1;
    while (current != nullptr && current->data != val)
    {
        prev = current;
        current = current->next;
        ++count;
    }
    // 没有找到
    if (current == nullptr)
    {
        cout << "链表中没有" << val << endl;
        return false;
    }
    if (prev == nullptr)
    {
        cout << "删除第1个值" << endl;
        head = head->next;
        len--;
    }
    else
    {
        cout << "删除第" << count << "个值" << endl;
        prev->next = current->next;
        len--;
    }
    delete current;
    return true;
}

Node* SinglyLinkedList::find(int val, int& count)
{
    Node *p = head;
    count = 1;
    while (p != nullptr)
    {
        if (p->data == val)
            return p;
        p = p->next;
        ++count;
    }
    return nullptr;
}

string SinglyLinkedList::single_display()
{
    std::stringstream ss;
    Node *current = head;
    while (current != nullptr)
    {
        ss << current->data ;
        current = current->next;
        if(current != nullptr)
            ss << "---->";
    }

    return ss.str();
}

string SinglyLinkedList::circular_display()
{
    std::stringstream ss;
    Node *current = head;
    int length = 0;
    ss << "--->";
    length += 4;
    while (current != nullptr)
    {
        ss << current->data ;
        length += std::to_string(current->data).size();
        current = current->next;
        if(current != nullptr){
            ss << "---->";
            length += 5;
        }
    }
    ss << "---";
    length += 3;

    ss << "\n|";
    for (int i = 0; i < length - 2; ++i)
        ss << ' ';

    ss << "|\n";
    for (int i = 0; i < length; ++i)
        ss << '-';
    ss << "\n";

    // 测试字体
    // for (int i = 0; i < length; ++i)
    //     ss << '-';
    // ss<<"*\n";
    // for (int i = 0; i < length; ++i)
    //     ss << ' ';
    // ss<<"*\n";
    // for (int i = 0; i < length; ++i)
    //     ss << '0';

    return ss.str();
}

string SinglyLinkedList::double_display()
{
    std::stringstream ss_t;
    std::stringstream ss_f;
    std::stringstream ss;
    Node *current = head;
    int length = 0;
    ss_t << "--->";
    ss_f << "----";
    length += 4;
    while (current != nullptr)
    {
        ss_t << current->data;
        ss_f << current->data;
        length += std::to_string(current->data).size();
        current = current->next;
        if(current != nullptr){
            ss_t << "---->";
            ss_f << "<----";
            length += 5;
        }
    }
    ss_t << "----";
    ss_f << "<---";
    length += 4;

    ss_t << "\n|";
    for (int i = 0; i < length - 2; ++i)
        ss_t << ' ';
    ss_t << "|\n";
    for (int i = 0; i < length; ++i)
        ss_t << '-';
    ss_t << "\n";

    for (int i = 0; i < length; ++i)
        ss << '-';
    ss << "\n|";
    for (int i = 0; i < length - 2; ++i)
        ss << ' ';
    ss << "|\n" << ss_f.str() << '\n' << ss_t.str();

    return ss.str();
}
