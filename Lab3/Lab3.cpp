#include <iostream>
#include <windows.h>
#include <string>
#include <complex>
#include <functional>
#include "BinTree.h"

/*
void printString(String string, const char sep[] = " ") {
    for (size_t i = 0; i < string.Size(); i++) {
        if (i != 0) {
            std::cout << sep;
        }
        std::cout << string[i];

    }
    std::cout << std::endl;
}
*/

template<typename T>
T a;

template<typename T>
T f(T x) {
    return x + x;
}

template<typename T>
int comp(T x) {
    return x == a<T>;
}

template<typename T>
T red(T x, T y) {
    return x + y;
}

void sep() {
    std::cout << "-----------------------------------------------------------\n";
}
/*
template<typename T>
int compT(T x) {
    extern T con;
    return x == con;
}
*/

template<typename T>
BinTree<T> UI(bool child = false) {
    BinTree<T> bintree = BinTree<T>();
    while (true) {
        //----------------------------------------------------------
        {
            sep();
            std::cout << "комманды:\n";
            std::cout << "	s - показать дерево\n";
            std::cout << "	1 - вставить\n";
            std::cout << "	2 - удалить\n";
            std::cout << "	3 - найти\n";
            std::cout << "	4 - map\n";
            std::cout << "	5 - reduce\n";
            std::cout << "	6 - where\n";
            std::cout << "	7 - извлечь поддерево\n";
            std::cout << "	8 - проверка на вхождение поддерева\n";
            std::cout << "	9 - сохранение в строку\n";
            std::cout << "	0 - exit\n";
            sep();
        }
        //----------------------------------------------------------
        char command;
        std::cin >> command;
        if (command == '0') {
            break;
        }
        if (command == 's') {
            bintree.Print();
        }
        if (command == '1') {
            T obj;
            int key;
            std::cout << "введите значение: ";
            std::cin >> obj;
            std::cout << "введите ключ: ";
            std::cin >> key;
            bintree.Insert(obj, key);
        }
        if (command == '2') {
            int key;
            std::cout << "введите ключ: ";
            std::cin >> key;
            bintree.Remove(key);
        }
        if (command == '3') {
            int key;
            std::cout << "введите ключ: ";
            std::cin >> key;
            std::cout << "значение: " << bintree.Find(key) << std::endl;
        }
        if (command == '4') {
            bintree = bintree.Map(f);
        }
        if (command == '5') {
            T start;
            std::cout << "введите начальное значение: ";
            std::cin >> start;
            std::cout << bintree.Reduce(red, start) << std::endl;
        }
        if (command == '6') {
            /*
            auto compT[&con](T x) {
                return x == con;
            };
            */
            T con;
            std::cout << "введите нежелательное значение: ";
            std::cin >> con;
            bintree = bintree.Where([&con](T x)->int { return x != con; });
        }
        if (command == '7') {
            int key;
            std::cout << "введите ключ: ";
            std::cin >> key;
            bintree = bintree.GetSubTree(key);
        }
        if (command == '8') {
            std::cout << "создание поддерева, нажмите 0 когда закончите процедуру\n";
            BinTree<T> subtree = UI<T>(true);
            if (bintree.FindSubTree(subtree)) {
                std::cout << "входит\n";
            } else std::cout << "не входит\n";
        }
        if (command == '9') { std::cout << bintree.toString() << std::endl; }
    }
    return bintree;
}

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    sep();
    std::cout << "                 Лабораторная работа 3\n";
    std::cout << "                Карачурин Рауль Б19-501\n";
    sep();

    // tests
    char command;
    while (true) {
        sep();
        std::cout << "Хотите провести тестирование? +/-\n";
        std::cin >> command;
        if (command == '+') {
            // test on int
            // base
            bool ok = true;
            BinTree<int> bintree = BinTree<int>();
            bintree.Insert(10, 10);
            bintree.Insert(5, 5);
            if (!(
                bintree.Find(10) == 10 &&
                bintree.Find(5) == 5
                )) ok = false;
            
            bintree.Insert(50, 50);
            bintree.Insert(500, 500);
            bintree.Insert(-1000, -1000);

            bintree.Remove(-1000);
            if (bintree.Find(-1000)) ok = false;

            // Getsubtree and Findsubtree
            //bintree.Print();
            BinTree<int> bin = bintree.GetSubTree(50);
            if (!bintree.FindSubTree(bin)) ok = false;
            
            // map, reduce, where
            BinTree<int> binMAP = bin.Map(f);
            BinTree<int> binWHERE = bin.Where(comp<int>);
            if (
                binMAP.Find(5) != 25 &&
                binMAP.Find(10) != 100 &&
                binMAP.Find(50) != 2500 &&
                binMAP.Find(500) != 250000 &&
                binWHERE.Find(500) != 500 &&
                bin.Reduce(red, 1) != 551
                ) ok = false;

            if (ok) std::cout << "тесты пройдены успешно\n";
            else std::cout << "тесты не пройдены\n";

        }
        else {
            break;
            //std::cin.get(command);
        }
    }

    //----------------------------------------------------------
    sep();
    std::cout << "поддерживаемые типы:\n";
    std::cout << "	1 - int\n";
    std::cout << "	2 - double\n";
    std::cout << "	3 - string\n";
    std::cout << "	4 - complex\n";
    std::cout << "	0 - exit\n";
    sep();
    //----------------------------------------------------------
    
    std::cin >> command;

    if (command == '0') {
        exit;
    }
    if (command == '1') {
        UI<int>();
    }
    if (command == '2') {
        UI<double>();
    }
    if (command == '3') {
        UI<std::string>();
    }
    if (command == '4') {
        UI<std::complex<double>>();
    }
    

}

