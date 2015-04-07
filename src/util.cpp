#include "util.h"

template<class T1, class T2, class T3>
Triple<T1, T2, T3> Triple<T1, T2, T3>::make_triple(T1 f, T2 s, T3 t) {
    Triple triple = new Triple(f, s, t);
    return triple;
}

template<class T1, class T2, class T3>
Triple<T1, T2, T3>::Triple(T1 first, T2 second, T3 third) {
    this -> first = first;
    this -> second = second;
    this -> third = third;
}
