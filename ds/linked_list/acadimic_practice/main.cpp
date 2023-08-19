/* -------------------------------------- */
// STL includes
#include <iterator>
#include <list>

/* -------------------------------------- */
// STUDENT includes
#include "computer.hpp"
#include "utils.cpp"

#define _STD  ::std::
#define _msl     MSL::



int main() {
    // -------------- Задание 1.1 --------------
    std::cout << '\n' << "-------------- Задание 1.1 --------------" << '\n';

    // Демонстрация работы функции push()
    std::list<double> doubleList;
    for (int i = 0; i < 5; i++) {
        _msl push<std::list, double>(doubleList, _msl generateDouble(0, 100));
    }
    _msl print<std::list, double>(doubleList);

    // Демонстрация работы функции pop()
    _msl pop(doubleList, 3);
    _msl print<std::list, double>(doubleList);

    // Демонстрация работы функции filter()
    auto listResult = _msl filter<std::list, double>(doubleList, &MSL::EvaluateFractionalPart, 0.45);
    _msl print(listResult); std::cout << '\n';



    // -------------- Задание 1.2 --------------
    std::cout << '\n' << "-------------- Задание 1.2 --------------" << '\n';

    std::list<MSL::Computer> computerList;

    MSL::Computer Apple  ((std::string) "apple",  1000, 16, 64, 17.8);
    MSL::Computer Lenovo ((std::string) "lenovo", 500,  16, 16, 15.0);
    MSL::Computer Huawei ((std::string) "huawei", 400,  16, 32, 16.7);
    MSL::Computer HP     ((std::string) "hp",     350,   4,  8, 14.5);
    MSL::Computer Dell   ((std::string) "dell",   200,   4,  8, 16.2);

    _msl push(computerList, Apple);
    _msl push(computerList, Lenovo);
    _msl push(computerList, Huawei);
    _msl push(computerList, HP);
    _msl push(computerList, Dell);

    _msl print(computerList);

    auto computerHvalue = _msl POP(computerList, 2);
    std::cout << '\n' << '\n' << computerHvalue << '\n';

    _msl print(computerList); std::cout << '\n';



    // -------------- Задание 1.3 --------------
    std::cout << '\n' << "-------------- Задание 1.3 --------------" << '\n';




}
