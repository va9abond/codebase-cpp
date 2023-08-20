#ifndef VECTORUTILS_HPP
#define VECTORUTILS_HPP


#include <fstream>
#include <string>
#include <vector>


using s_sz = std::string::size_type;


// file to double vector
template <
    class Val_t
>
// inline std::vector<Val_t> generate_vector_from_file (std::ifstream& file) {
inline decltype(auto) generate_vector_from_file (const std::string& file_name) {
    std::ifstream file (file_name, std::ios::in); // create an input stream to read from file
    // file.open(file_name, std::ios::in); // open file by file_name
    std::string string_res;

    if (file.is_open()) {
    std::string line;
        while (getline(file, line)) {
            string_res += line;
        }
    }

    file.close();
    return string_res;
}


inline bool is_digit (char c) { return (c > 47 && c < 58 ? true : false); }


// return pos at where symbol is numeric, else return 0
inline s_sz find_first_numeric (const std::string& str, s_sz pos) {
    while ( pos < str.size() && !is_digit(str[pos]) ) { ++pos; }
    return (pos == str.size() ? 0 : pos);
}


// return pos if str[pos - 1] is numeric and str[pos] is not or is reference to CharT (~ pos = size())
inline s_sz find_last_numeric (const std::string& str, s_sz pos) {
    // static_assert(is_digit(str[pos - 1]));
    while (pos < str.size() && ( is_digit(str[pos]) || str[pos] == 46 )) { ++pos; }
    return pos;
}


inline std::vector<int> str_to_vec (const std::string &str) {
    std::vector<int> Result;
    s_sz pos_first {0}, pos_last {0}; /* pos_global {0} */;
    for (
      pos_first = find_first_numeric(str, pos_first);
      pos_first < str.size() && pos_first;
      pos_first = pos_last + 1
    ) {
        pos_last  = find_last_numeric(str, pos_first + 1);
        Result.push_back( std::stoi(str.substr(pos_first, pos_last - pos_first)) );
    }
    return Result;
}


// "{ a, b, c, d, e };" --> " a b c d e;"
inline std::string normalize_string (std::string str) {
    for (s_sz pos {0}; pos < str.size(); ++pos) {
        if (!( is_digit(str[pos]) || str[pos] == 46 || str[pos] == 59 || str[pos] == 32)) { str.erase(pos, 1); }
        //     ^^^^: 0123456789      "."   <---  ^^    ";"   <---  ^^    <space> <-- ^^
    }
    return str;
}


template<
    class Val_t
>
decltype(auto) find_next_value (const std::string& str) {
    // str
}


// template <
//     class Val_t
// >
// // [WARNING]: Val_t should have conversation to std::string with
//            std::{ stoi, stol, stoll, stof, stod, stold, stoul, stoull } or
//            with user-defined function

// [WARNING]: param str should be look like "{ ..., ..., ... };"
// inline std::vector<Val_t> create_vector_by_string (const std::string& str) {
//     std::vector<Val_t> Result;
//     std::string::size_type pos { 0 };
//
//     while (pos != str.size()) {
//         pos;
//
//     }
//     // for (const char )
// }
//


#endif // VECTORUTILS_HPP
