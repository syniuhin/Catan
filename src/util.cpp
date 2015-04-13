#include "util.h"

template<class T1, class T2, class T3>
Triple<T1, T2, T3> Triple<T1, T2, T3>::make_triple(T1 f,
        T2 s, T3 t) {
    return new Triple(f, s, t);
}

template<class T1, class T2, class T3>
Triple<T1, T2, T3>::Triple(T1 f, T2 s, T3 t)
    : first(f),
      second(s),
      third(t) {}
