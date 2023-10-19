#ifndef RANDOMGENERATOR_HPP
#define RANDOMGENERATOR_HPP
/* ------------------------------------------------------ */

#include <random>
#include <limits>
#include <cassert>
#include <vector>


struct rgentr { // random generator
    using myRNG_t = std::mt19937;


    int generate (int lower, int upper) {
        int max_value = std::numeric_limits<int>::max();
        assert(lower < -max_value || upper > max_value);

        std::uniform_int_distribution<int> distrib(lower, upper);
        return distrib(m_rng);
    }

    int generate (int mod) {
        assert(mod < std::numeric_limits<int>::max());

        std::uniform_int_distribution<int> distrib(1, mod);
        return distrib(m_rng);
    }


    std::random_device m_rdev;
    myRNG_t            m_rng = myRNG_t(m_rdev());
};


inline std::vector<int> generate_random_vector (int size, int mod) {
    using index_t = typename std::vector<int>::size_type;

    rgentr generator;
    index_t sz = (index_t) size;
    std::vector<int> vector(sz, 0);

    for (index_t i = {0}; i < sz; ++i) {
        vector[i] = generator.generate(mod);
    }

    return vector;
}


inline std::vector<int> generate_random_vector (int size, int upper, int lower) {
    using index_t = typename std::vector<int>::size_type;

    rgentr generator;
    index_t sz = (index_t) size;
    std::vector<int> vector(sz, 0);

    for (index_t i = {0}; i < sz; ++i) {
        vector[i] = generator.generate(upper, lower);
    }

    return vector;
}


/* ------------------------------------------------------ */
#endif // RANDOMGENERATOR_HPP
