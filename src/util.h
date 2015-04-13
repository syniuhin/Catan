#ifndef __UTIL_H__
#define __UTIL_H__

template<class T1, class T2, class T3> class Triple {
public:
    T1 first;
    T2 second;
    T3 third;

    static Triple<T1, T2, T3> make_triple(T1, T2, T3);
private:
    Triple<T1, T2, T3>(T1, T2, T3);
};

#endif
