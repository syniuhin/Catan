#ifndef __UTIL_H__
#define __UTIL_H__

template<typename T1, typename T2, typename T3> class Triple {
public:
    T1 first;
    T2 second;
    T3 third;

    static Triple<T1, T2, T3> make_triple(T1 f, T2 s, T3 t) {
        return Triple(f, s, t);
    }

private:
    Triple<T1, T2, T3>(T1 f, T2 s, T3 t) : first(f),
                                           second(s),
                                           third(t) {}
};
#endif
