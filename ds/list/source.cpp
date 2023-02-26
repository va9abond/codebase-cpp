// #include <corecrt_terminate.h>
#include <float.h>

#include <iostream>
#include <fstream>

#include <iterator>
#include <list>

#include <exception>
#include <algorithm>
#include <malloc.h>

// #include <stdlib.h>


#define _STD  ::std::
#define _ml     ML::


namespace ML {



	class exception {
	public:
		exception (const char* other) noexcept : _message(other) {}
		
		exception (const exception&) noexcept = default;
		
		exception& operator= (const exception&) noexcept = default;
		
		exception (exception&& other) noexcept : _message(other._message) {
			other._message = nullptr;
		}

		exception& operator= (exception&& other) {
			if (this != &other)
			{
				delete[] _message;

				_message = other._message;

				other._message = nullptr;
			}

			return *this;
		}

		virtual ~exception() {
			delete[] _message;
			_message = nullptr;
		}
		
		virtual const char* what() const noexcept {
			return _message;
		}
	
	protected:
		const char *_message;
	};



	class semantic_error : exception {
	public:
		semantic_error (const char* other) noexcept :
			exception("SEMANTIC_ERROR"),
			_message(other) 
		{}

		semantic_error (const semantic_error&) noexcept = default;
		
		semantic_error& operator= (const semantic_error&) noexcept = default;

		semantic_error (semantic_error&& other) noexcept
			: exception("SEMANTIC_ERROR"),
			_message(other._message)
		{
			other._message = nullptr;
		}

		semantic_error& operator= (semantic_error&& other) noexcept {
			if (this != &other)
			{
				delete[] _message;

				_message = other._message;

				other._message = nullptr; 
			}

			return *this;
		}

		virtual ~semantic_error() {
			delete[] _message;
			_message = nullptr;
		};

		const char* what() const noexcept {
			return _message;
		}

	private:
		const char *_message;
	};



	class Computer {
	public:

		Computer (std::string model = "unknown",
				  double price = -1,
				  int numCores = -1,
				  int ram = -1,
				  double diaganal = -1
		) noexcept :
			_model(model),
			_price(price),
			_numCores(numCores),
			_ram(ram),
			_diagonal(diaganal)
		{}

		Computer (const Computer&) = default;

		Computer &operator= (const Computer&) = default;

		~Computer() = default;




		std::string GetModel() const noexcept { return _model; }

		double GetPrice() const noexcept { return _price; }

		int GetNumCores() const noexcept { return _numCores; }

		int GetRam() const noexcept { return _ram; }

		double GetDiagonal() const noexcept {return _diagonal; }



		bool operator== (const Computer &other) const noexcept {
			if (_model == other._model &&
				_price == other._price &&
				_numCores == other._numCores &&
				_ram == other._ram &&
				_diagonal == other._diagonal) { return true; }

			return false;
		}

		bool operator!= (const Computer &other) const noexcept {
			return !(*this == other);
		}

		bool operator> (const Computer &other) const noexcept {
			std::string modelLowerCase = _model;
			std::for_each(modelLowerCase.begin(), modelLowerCase.end(),
					  [](unsigned char c) { return std::tolower(c); }); 


			std::string otherModelLowerCase = other._model;
			std::for_each(otherModelLowerCase.begin(), otherModelLowerCase.end(),
					  [](unsigned char c) { return std::tolower(c); });


			if ( modelLowerCase > otherModelLowerCase ) {
				return true;
			}

			else if ( modelLowerCase == otherModelLowerCase ) {
				if ( _price > other._price ) {
					return true;
				}
				
				else if ( _price == other._price ) {
					if ( _diagonal > other._diagonal ) {
						return true;
					}
					
					else if ( _diagonal == other._diagonal ) {
						if ( _numCores > other._numCores ) {
							return true;
						}
						
						else if ( _ram > other._ram ) {
							return true;
					}
				}
			}
		}

			return false;
		}

		bool operator< (const Computer &other) const noexcept {
			return (!(*this > other) && !(*this != other));
		}

		bool operator>= (const Computer &other) const noexcept {
			return (*this > other || *this == other);
		}

		bool operator<= (const Computer &other) const noexcept {
			return (!(*this > other) || *this == other);
		}



		friend std::ostream &operator<< (std::ostream &output, const Computer &computer);



	private:
		std::string _model;
		double      _price;
		int         _numCores;
		int         _ram;
		double      _diagonal;
	};

	std::ostream &operator<< (std::ostream & output, const Computer & computer) {
		if ( typeid(output).name() != typeid(std::ofstream).name() ) {
			output << "{ " <<
			 computer.GetModel() << "; " <<
			 computer.GetPrice() << "; " <<
			 computer.GetDiagonal() << "; "<< 
			 computer.GetNumCores() << "; " <<
			 computer.GetRam() << " }";
		}
		else {
			output << '\n' <<
			 computer.GetModel() << " " <<
			 computer.GetPrice() << " " <<
			 computer.GetDiagonal() << " " <<
			 computer.GetNumCores() << " " <<
			 computer.GetRam() << '\n';
		}


		return output;
	}



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
	TObject POP (TContainer<TObject> &container, const int &pos) {
		if (pos >= container.size() || pos < 0) {
			throw std::out_of_range("function pop: Index out of range");
		}
		
		try {
			auto objectToRemove = std::next(container.begin(), pos);
			container.erase(objectToRemove);
			
			auto objectToReturn = container.back();
			return objectToReturn;
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
	std::cout << '\n' << "-------------- Задание 1.1 --------------" << '\n';

	// Демонстрация работы функции push()
	std::list<double> doubleList;
	for (int i = 0; i < 5; i++) {
		_ml push<std::list, double>(doubleList, _ml generateDouble(0, 100));
	}
	_ml print<std::list, double>(doubleList);

	// Демонстрация работы функции pop()
	_ml pop(doubleList, 3);
	_ml print<std::list, double>(doubleList);

	// Демонстрация работы функции filter()
	auto listResult = _ml filter<std::list, double>(doubleList, &ML::EvaluateFractionalPart, 0.45);
	_ml print(listResult); std::cout << '\n';



	// -------------- Задание 1.2 --------------
	std::cout << '\n' << "-------------- Задание 1.2 --------------" << '\n';

    std::list<ML::Computer> computerList;

    ML::Computer Apple  ((std::string) "apple",  1000, 16, 64, 17.8);
    ML::Computer Lenovo ((std::string) "lenovo", 500,  16, 16, 15.0);
    ML::Computer Huawei ((std::string) "huawei", 400,  16, 32, 16.7);
    ML::Computer HP     ((std::string) "hp",     350,   4,  8, 14.5);
    ML::Computer Dell   ((std::string) "dell",   200,   4,  8, 16.2);

    _ml push(computerList, Apple);
    _ml push(computerList, Lenovo);
    _ml push(computerList, Huawei);
    _ml push(computerList, HP);
    _ml push(computerList, Dell);

    _ml print(computerList);

    auto computerHvalue = _ml POP(computerList, 2);
    std::cout << '\n' << '\n' << computerHvalue << '\n';

    _ml print(computerList); std::cout << '\n';



    // -------------- Задание 1.3 --------------
    std::cout << '\n' << "-------------- Задание 1.3 --------------" << '\n';




}