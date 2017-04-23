#ifndef _NIBO_UTILS_H_
#define _NIBO_UTILS_H_

/*! @file    utils.h
 * \~german
 *  @brief   Nützliche Funktionen, die in C als Makros und in C++ als Templates implementiert sind.
 *  Die Funktionen sind frei von Seiteneffekten, das bedeutet, dass der Ausdruck max(++i, j++) ohne
 *  Probleme verwendet werden kann!
 *
 *  @author  Nils Springob (nils.springob@nicai-systems.com)
 *  @date    2015-08-27
 * \~english
 *  @brief   Usefull functions implemented as macros for C and templates for C++.
 *  The functions have no side effects, the expression max(++i, j++) can be used without problems!
 *
 *  @author  Nils Springob (nils.springob@nicai-systems.com)
 *  @date    2015-08-27
 */


#ifdef DOXYGEN
/**
 * \~german
 * Maximum von zwei Werten. (Typ-unabhängig, ohne Seiteneffekte)
 * @returns maximum value
 * \~english
 * maximum of two values. (type independent, no side effects)
 * @returns maximum value
 */
any_type max(any_type a, any_type b);

/**
 * \~german
 * Minimum von zwei Werten. (Typ-unabhängig, ohne Seiteneffekte)
 * @returns minimum value
 * \~english
 * minimum of two values. (type independent, no side effects)
 * @returns minimum value
 */
any_type min(any_type a, any_type b);

/**
 * \~german
 * Beschränkt den Wert x auf das Interval [lo,hi].  (Typ-unabhängig, ohne Seiteneffekte)
 * @returns x falls x im Interval [lo,hi] liegt<br/>
 *          lo falls x&lt;lo <br/>
 *          hi falls x&gt;hi
 * \~english
 * clip value x to the interval [lo,hi]. (type independent, no side effects)
 * @returns x if x is in the interval [lo,hi]<br/>
 *          lo if x&lt;lo <br/>
 *          hi if x&gt;hi
 */
any_type constrain(any_type x, any_type lo, any_type hi);

/**
 * \~german
 * Absolute (positive) Differenz von zwei Werten.  (Typ-unabhängig, ohne Seiteneffekte)
 * @returns Absolute (positive) Differenz
 * \~english
 * absolute (positive) difference of two values. (type independent, no side effects)
 * @returns absolute (positive) difference
 */
any_type absdiff(any_type a, any_type b);

/**
 * \~german
 * Absolutwert (Wert ohne Vorzeichen).  (Typ-unabhängig, ohne Seiteneffekte)
 * @returns Absolutwert
 * \~english
 * absolute value. (type independent, no side effects)
 * @returns absolute value
 */
any_type absall(any_type x);


#else

#ifdef ARDUINO
#include <Arduino.h>
#endif



#ifndef __cplusplus

// use GCC statement expressions...
// will only work with gcc compiler
#ifndef __GNUC__
#error GCC statement expressions will only work with gcc compiler!
#endif

#ifndef ARDUINO

#ifndef max
#define max(a,b) \
   ({ __typeof__ (a) _a = (a); \
      __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })
#define max3(a,b,c) max(max(a,b),c)
#define max4(a,b,c,d) max(max(a,b),max(c,d))
#endif


#ifndef min
#define min(a,b) \
   ({ __typeof__ (a) _a = (a); \
      __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })
#define min3(a,b,c) min(min(a,b),c)
#define min4(a,b,c,d) min(min(a,b),min(c,d))
#endif


#define constrain(x,a,b) \
   ({ __typeof__ (x) _x = (x); \
      __typeof__ (a) _a = (a); \
      __typeof__ (b) _b = (b); \
     _x < _a ? _a : (_x > _b ? _b : _x); })

#else  // ARDUINO

#define max3(a,b,c) max(max(a,b),c)
#define max4(a,b,c,d) max(max(a,b),max(c,d))
#define min3(a,b,c) min(min(a,b),c)
#define min4(a,b,c,d) min(min(a,b),min(c,d))

#endif // ARDUINO

#define absdiff(a,b) \
   ({ __typeof__ (a) _a = (a); \
      __typeof__ (b) _b = (b); \
     (_a < _b) ? (_b-_a) : (_a-_b); })


#define absall(a) \
   ({ __typeof__ (a) _a = (a); \
     (_a < 0) ? (-_a) : (_a); })

#else // __cplusplus

#ifndef ARDUINO

template <typename T>
inline static T max(T a, T b) {
  return a > b ? a : b;
};

template <typename T>
inline static T max(T a, T b, T c) {
  return max(max(a,b),c);
};

template <typename T>
inline static T max(T a, T b, T c, T d) {
  return max(max(a,b),max(c,d));
};

#define max3 max
#define max4 max

template <typename T>
inline static T min(T a, T b) {
  return a < b ? a : b;
};

template <typename T>
inline static T min(T a, T b, T c) {
  return min(min(a,b),c);
};

template <typename T>
inline static T min(T a, T b, T c, T d) {
  return min(min(a,b),min(c,d));
};

#define min3 min
#define min4 min

template <typename T>
inline static T constrain(T x, T a, T b) {
  return x < a ? a : (x > b ? b : x);
};

#else  // ARDUINO

#define max3(a,b,c) max(max(a,b),c)
#define max4(a,b,c,d) max(max(a,b),max(c,d))
#define min3(a,b,c) min(min(a,b),c)
#define min4(a,b,c,d) min(min(a,b),min(c,d))

#endif // ARDUINO

template <typename T>
inline static T absall(T x) {
  return (x < 0) ? -x : x;
};

template <typename T>
inline static T absdiff(T a, T b) {
  return (a < b) ? (b-a) : (a-b);
};


#endif // __cplusplus

#endif // DOXYGEN

#endif // _NIBO_UTILS_H_



