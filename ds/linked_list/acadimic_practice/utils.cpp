#include <cfloat>
#include <cstdlib>
#include <iostream>
#include <stdexcept>


namespace MSL {
double generateDouble(double value_min, double value_max) {
    double value = (double)rand() / RAND_MAX;
    return value_min + value * (value_max - value_min);
}


template <
    template<class...> class TContainer,
    class TObject
>
void push (TContainer<TObject> &container, const TObject &object) {
    auto itt = container.begin();

    while ( itt != container.end() ) {
        if ( *itt >= object ) { break; }
        itt++;
    }

    container.insert(itt, object);
}


template <
    template<class...> class TContainer,
    class TObject
>
typename TContainer<TObject>::iterator pop (TContainer<TObject> &container, const int &pos) {
    if (pos >= static_cast<int>(container.size()) || pos < 0) {
        throw std::out_of_range("function pop: Index out of range");
    }

    auto objectToRemove = std::next(container.begin(), pos);
    return container.erase(objectToRemove);

}



template <
    template<class...> class TContainer,
    class TObject
>
TObject POP (TContainer<TObject> &container, const int &pos) {
    if (pos >= static_cast<int>(container.size()) || pos < 0) {
        throw std::out_of_range("function pop: Index out of range");
    }

    auto objectToRemove = std::next(container.begin(), pos);
    container.erase(objectToRemove);

    auto objectToReturn = container.back();
    return objectToReturn;

}



template <
    template<class...> class TContainer,
    class TObject
>
void print (const TContainer<TObject> &container) {
    // std::for_each(list.begin(), list.end(), [](const Type &n) { std::cout << n << ", "; });
    auto itt = container.begin();

    while ( itt != container.end() ) {
        if ( itt == container.begin() ) {
            std::cout << '\n' << "{ " << *itt;
        }
        else {
            std::cout << ", " << *itt; 
        }

        itt++;
    }

    std::cout << " }";
}



bool EvaluateFractionalPart (const double p, const double value) {
    int value_ceil = int(value);
    double diff = value - value_ceil;
    return (diff - p < DBL_EPSILON);
}


template <
    template<class...> class TContainer,
    class TObject
>
TContainer<TObject> filter (const TContainer<TObject> &container,
                            bool (*func_key)(const TObject, const TObject),
                            double param)
{
    TContainer<TObject> containerResult;

    for (auto object : container) {
        if ( func_key(param, object) == true ) {
            containerResult.push_back(object);
        }
    }
    return containerResult;
}
}
