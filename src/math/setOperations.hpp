#include <vector>
#include <algorithm>
#include <numeric>

namespace math{
    template<typename T>
    T cartesianOf(T& s1, T& s2){
        typename T::iterator it1;
        typename T::iterator it2;
        T res;
        for(it1 = s1.begin(); it1 != s1.end(); it1++){
            for(it2 = s1.begin(); it2 != s1.end(); it2++){
                res.insert(*it1 + 'x' + *it2);
            }
        }
        return res;
    }

    template<typename T>
    T getUnion(T& s1, T& s2){
        T res = s1;
        res.insert(s2.begin(), s2.end());

        return res;
    }

    template<typename T>
    T getIntersection(T& s1, T& s2){
        T res;
        std::set_intersection(s1.begin(), s1.end(), s2.begin(), s2.end(), std::inserter(res, res.begin()));
        return res;
    }

    template<typename T>
    T getRange(int32_t a, int32_t b){
        std::vector<int32_t> range(b - a);
        T res;
        std::iota(range.begin(), range.end(), 0);
        std::vector<int32_t>::iterator it;
        for(it = range.begin(); it != range.end(); it++)
            res.insert(std::to_string(*it));

        return res;
    }

    template<typename T>
    T getDifference(T& s1, T& s2){
        T res;
        std::set_difference(s1.begin(), s1.end(), s2.begin(), s2.end(), std::inserter(res, res.begin()));
        return res;
    }
};