#include <iostream>
#include <vector>
#include <string>
#include <memory>
using std::vector;
using std::string;
using std::shared_ptr;
using std::cin;
using std::cout;
using std::endl;

class Brain {
protected:
    vector<double> data;
    string phrase;

public:
    Brain() {
        data.resize(1000000);
        phrase = "Booyahg Booyahg Booyahg";
    }
    string speak() {
        return phrase;
    }
};

class Goblin {
    shared_ptr<Brain> brain;
public:
    Goblin(shared_ptr<Brain> _brain) : brain(_brain) {}
    string speak() {return brain->speak();}
};

vector<Goblin> create_goblin_army(unsigned int size)
{
    shared_ptr<Brain> brain = shared_ptr<Brain>(new Brain());
    return vector<Goblin>(size, Goblin(brain));
}

int main()
{
    unsigned int size1 = 1;
    unsigned int size2 = 10;
    vector<Goblin> army1 = create_goblin_army(size1);
    vector<Goblin> army2 = create_goblin_army(size2);

    for(unsigned int i = 0; i < size1; i++) {
        cout << army1[i].speak() << endl;
    }
    for(unsigned int i = 0; i < size2; i++) {
        cout << army2[i].speak() << endl;
    }

    return 0;
}
