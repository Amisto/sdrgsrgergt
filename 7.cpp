#include <iostream>
#include <memory>

class IncorrectIndexException : public std::exception
{
public:
    IncorrectIndexException() : std::exception() {};

};

class Storage
{
private:
    std::unique_ptr<int[]> data;
    unsigned int size;
public:
    // Конструктор хранилища размерности n
    Storage(unsigned int n) : size(n)
    {
        data = std::unique_ptr<int[]>(new int[n]);
    }

    // Добавьте деструктор, если нужно

    // Получение размерности хранилища
    unsigned int getSize() const {return size;}

    // Получение значения i-го элемента из хранилища
    // Если индекс некорректен, нужно выбросить IncorrectIndexException
    int getValue(unsigned int i) const
    {
        if (i >= size)
            throw IncorrectIndexException();
        return data[i];
    }

    // Задание значения i-го элемента из хранилища равным value
    // Если индекс некорректен, нужно выбросить IncorrectIndexException
    void setValue(unsigned int i, int value)
    {
        if (i >= size)
            throw IncorrectIndexException();
        data[i] = value;
    }
};

int main() {
    unsigned int index;
    std::cin >> index;
    Storage s(42);
    s.setValue(index, 0);
    std::cout << s.getValue(index) << std::endl;
    return 0;
}
