#include <iostream>

using namespace std;

template<typename TYPE> class List
{
private:
    typedef struct element
    {
        TYPE data;
        struct element *next;
        struct element *prev;
    }element;
    element *head;
    element *tail;
    bool ifEmpty();

public:
    List();
    ~List();
    List(const List &) = delete;
    List &operator=(const List &) = delete;
    void AddTail(const TYPE);
    void AddHead(const TYPE);
    void DeleteHead();
    void DeleteTail();
    void ShowAll();
};
//конструктор инициализация
template <typename TYPE>
List<TYPE>::List(void) : head(0), tail(0)
{}
//проверка на пустоту
template <typename TYPE>
bool List<TYPE>::ifEmpty()
{
    if (head == NULL) return true;
    else return false;
}
//добавляем в конец
template <typename TYPE>
void List<TYPE>::AddTail(const TYPE data)
{
    struct element* tmp = new struct element;
    tmp -> data = data;
    tmp -> next = NULL;

    if (ifEmpty() == true)
    {
        head = tmp;
        tail = tmp;
        tmp -> prev = NULL;
    }
    else
    {
        tail -> next = tmp;
        tmp -> prev = tail;
        tail = tmp;
    }
}
//добавляем в начало
template <typename TYPE>
void List<TYPE>::AddHead(const TYPE data)
{
    struct element* tmp = new struct element;
    tmp -> data = data;
    tmp -> prev = NULL;

    if (ifEmpty() == true)
    {
        head = tmp;
        tail = tmp;
        tmp -> next = NULL;
    }
    else
    {
        head -> prev = tmp;
        tmp -> next = head;
        head = tmp;
    }
}
//Показываем список
template <typename TYPE>
void List<TYPE>::ShowAll()
{
    if (ifEmpty() == true) cout << "empty";
    else
    {
        element* tmp = head;
        while (tmp != NULL)
        {
             cout << tmp -> data << " ";
             tmp = tmp -> next;
        }
    }
}
//удаляем первый
template <typename TYPE>
void List<TYPE>::DeleteHead()
{
    if (ifEmpty() == true) cout << "empty";
    else
    {
        element *tmp = head;
        if (head -> next == NULL)
        {
            delete tmp;
            head = NULL;
            tail = NULL;
        }
        else
        {
            head -> next -> prev = NULL;
            head = head -> next;
            delete tmp;
        }
    }
}
//удаляем конец
template <typename TYPE>
void List<TYPE>::DeleteTail()
{
    if (ifEmpty() == true) cout << "empty";
    else
    {
        element *tmp = tail;
        if (tail -> prev == NULL)
        {
            delete tmp;
            head = NULL;
            tail = NULL;
        }
        else
        {
            tail -> prev -> next = NULL;
            tail = tail -> prev;
            delete tmp;
        }
    }
}

template <typename TYPE>
List<TYPE>::~List(void)
{
    if (ifEmpty() == true);
    else
    {
        element *tmp = tail;
        element *temp = NULL;
        while (tmp != NULL)
        {
            temp = tmp;
            tmp = tmp -> prev;
            delete temp;
        }
        head = NULL;
        tail = NULL;
    }
}


int main()
{return 0;}
