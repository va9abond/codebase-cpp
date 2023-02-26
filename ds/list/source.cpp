// #include <corecrt_terminate.h>
#include <float.h>
#include <iostream>
#include <iterator>
#include <list>
#include <exception>
#include <algorithm>
#include <stdlib.h>


#define _STD  ::std::
#define _ml     ML::


namespace ML {



	class exception {
	public:
		exception (const char* other) noexcept : _message(other) {}
		
		exception (const exception&) noexcept = default;
		
		exception& operator= (const exception&) noexcept = default;
		
		virtual ~exception() {
			delete [] _message;
		}
		
		virtual const char* what() const noexcept {
			return _message;
		}
	
	protected:
		const char *_message;
	};



	class semantic_error : exception {
	public:
		semantic_error (const char* other)  
			: exception("SEMNANTIC_ERROR"),
			_message(other) {}

		semantic_error (const semantic_error&) noexcept = default;
		
		semantic_error& operator= (const semantic_error&) noexcept = default;

		virtual ~semantic_error() {
			delete[] _message;
		};

		const char* what() const noexcept {
			return _message;
		}

	private:
		const char *_message;
	};



	double generateDouble(double value_min, double value_max) {
		double value = (double)rand() / RAND_MAX;
		return value_min + value * (value_max - value_min);
	}



	template <
		template<class...> class TContainer,
		class TObject
	>
	void push (TContainer<TObject> &container, const TObject &object) {
		try {
			auto itt = container.begin();
			
			while ( itt != container.end() ) {
				if ( *itt >= object ) { break; }
				itt++;
			}

			container.insert(itt, object);	
		}
		catch(...) {
			throw semantic_error("BLA BLA BLA");
		}
	}



	template <
		template<class...> class TContainer,
		class TObject
	>
	typename TContainer<TObject>::iterator pop (TContainer<TObject> &container, const int &pos) {
		if (pos >= container.size() || pos < 0) {
			throw std::out_of_range("function pop: Index out of range");
		}
		
		try {
			auto objectToRemove = std::next(container.begin(), pos);
		
			return container.erase(objectToRemove);
		}
		catch(...) {
			throw _ml exception("BLA BLA BLA");
		}
		
	} 
	


	template <
		template<class...> class TContainer,
		class TObject
	>
	void print (const TContainer<TObject> &container) {
		try {
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
		catch (...) {
			throw semantic_error("BLA BLA BLA");
		}
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
								double param) {
		TContainer<TObject> containerResult;

		for (auto object : container) {
			if ( func_key(param, object) == true ) {
				containerResult.push_back(object);
			}
		}

		return containerResult;
	}

}



int main() {
	// -------------- Задание 1.1 --------------

	// Демонстрация работы функции push()
	std::list<double> doubleList;
	for (int i = 0; i < 5; i++) {
		// _ml push<double, std::list>(doubleList, (double) rand());
		_ml push<std::list, double>(doubleList, _ml generateDouble(0, 100));
	}
	_ml print<std::list, double>(doubleList);

	// Демонстрация работы функции pop()
	_ml pop(doubleList, 3);
	_ml print<std::list, double>(doubleList);

	// Демонстрация работы функции filter()
	auto listResult = _ml filter<std::list, double>(doubleList, &ML::EvaluateFractionalPart, 0.45);
	_ml print(listResult);
}