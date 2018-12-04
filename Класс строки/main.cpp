#include <iostream>
#include <string.h>

using namespace std;

class String
{
private:
    char *data;
    size_t lenght;
    void Free();
public:
    String() : data(nullptr), lenght(0) {};
    String(const char *);
    String(const String &);
    String &operator=(const String &);
    String &operator=(const char *);
    String &operator+=(const String &);
    friend ostream &operator<<(ostream& os, const String &i) {return os << i.data;};
    ~String() {Free();};
};

void String::Free()
{
    delete data;
    lenght = 0;
}

String::String(const char *str) : data(nullptr), lenght(0)
{
    lenght = strlen(str);
    data = new char[lenght + 1];
    memcpy(data, str, lenght);
}

String::String(const String &obj) : data(nullptr), lenght(0)
{
    data = obj.data;
    lenght = obj.lenght;
}

String &String::operator=(const String &obj)
{
    if (data != nullptr) {Free();}
    data = obj.data;
    lenght = obj.lenght;
    //memcpy(data, obj.data, obj.lenght);
}

String &String::operator=(const char *str)
{
    if (data != nullptr) {Free();}
    lenght = strlen(str);
    data = new char[lenght + 1];
    memcpy(data, str, lenght);
}

String &String::operator+=(const String &obj)
{
    char *tmp = new char[lenght+obj.lenght+2];
    tmp = strcat(data,obj.data);
    cout << tmp;
}

int main()
{

    return 0;
}
