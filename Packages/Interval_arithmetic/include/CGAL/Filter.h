// ============================================================================
//
// Copyright (c) 1998 The CGAL Consortium
//
// This software and related documentation is part of an INTERNAL release
// of the Computational Geometry Algorithms Library (CGAL). It is not
// intended for general use.
//
// ----------------------------------------------------------------------------
//
// release       :
// release_date  :
//
// file          : include/CGAL/Filter.h
// revision      : $Revision$
// revision_date : $Date$
// package       : Interval Arithmetic
// author(s)     : Sylvain Pion <Sylvain.Pion@sophia.inria.fr>
//
// coordinator   : INRIA Sophia-Antipolis (<Mariette.Yvinec@sophia.inria.fr>)
//
// ============================================================================

// This file contains a wrapper type for number types, that helps for
// specializing template predicates, to use interval arithmetic as a filter.

#ifndef CGAL_FILTER_H
#define CGAL_FILTER_H

#include <iostream.h>
#include <CGAL/IO/io_tags.h>            // For CGAL_io_Operator().
#include <CGAL/number_type_tags.h>      // For CGAL_number_type_tag()
#include <CGAL/enum.h>  // Because we overload CGAL_{sign,compare,abs,min,max}
#include <CGAL/number_utils.h>  // For CGAL_max(double, double).


// CT = construction type (filtered)
// ET = exact type, used for exact predicate evaluation
// (Interval_nt_advanced) = used for filtering.
//
// 2 functions must be provided for the whole thing to work for a particular
// instantiation:
// - CGAL_to_interval_nt_advanced CGAL_to_interval_nt(const CT &);
//     which gives an interval surely containing the CT value.
// - ET CGAL_to_exact_type<ET>(const CT &)
//     which converts _exactly_ the CT value to ET.

template <class CT, class ET>
class CGAL_Filtering
{
public:
  CT value;

  CGAL_Filtering () {}
  CGAL_Filtering (int i) : value(i)  {}
  CGAL_Filtering (CT ct) : value(ct) {}

  typedef CGAL_Filtering<CT,ET> Fil;

  Fil operator-()               const { return Fil(-value); }
  bool operator< (const Fil& fil) const { return value <  fil.value; }
  bool operator> (const Fil& fil) const { return value >  fil.value; }
  bool operator<=(const Fil& fil) const { return value <= fil.value; }
  bool operator>=(const Fil& fil) const { return value >= fil.value; }
  bool operator==(const Fil& fil) const { return value == fil.value; }
  bool operator!=(const Fil& fil) const { return value != fil.value; }
};

template <class CT, class ET>
class CGAL_Filtering_allow_inexact : public CGAL_Filtering<CT,ET>
{
  typedef CGAL_Filtering_allow_inexact<CT,ET> Fil;
  Fil operator+(const Fil& fil) const { return Fil(value + fil.value); }
  Fil operator-(const Fil& fil) const { return Fil(value - fil.value); }
  Fil operator*(const Fil& fil) const { return Fil(value * fil.value); }
  Fil operator/(const Fil& fil) const { return Fil(value / fil.value); }

  Fil& operator+=(const Fil& fil) { value += fil.value; return *this; }
  Fil& operator-=(const Fil& fil) { value -= fil.value; return *this; }
  Fil& operator*=(const Fil& fil) { value *= fil.value; return *this; }
  Fil& operator/=(const Fil& fil) { value /= fil.value; return *this; }
};

// We forward all the following functions to the CT value:
// CGAL_is_valid, CGAL_is_finite, CGAL_to_double, CGAL_sign, CGAL_compare,
// CGAL_abs, CGAL_min, CGAL_max, sqrt, CGAL_io_tag, CGAL_number_type_tag,
// operator>>, operator<<.

template <class CT, class ET>
inline bool CGAL_is_valid    (const CGAL_Filtering<CT,ET>& fil)
{ return CGAL_is_valid(fil.value); }

template <class CT, class ET>
inline bool CGAL_is_finite   (const CGAL_Filtering<CT,ET>& fil)
{ return CGAL_is_finite(fil.value); }

template <class CT, class ET>
inline double CGAL_to_double (const CGAL_Filtering<CT,ET>& fil)
{ return CGAL_to_double(fil.value); }

template <class CT, class ET>
inline CGAL_Sign CGAL_sign (const CGAL_Filtering<CT,ET>& fil)
{ return CGAL_Sign(fil.value); }

template <class CT, class ET>
inline CGAL_Comparison_result CGAL_compare (const CGAL_Filtering<CT,ET>& fil,
					    const CGAL_Filtering<CT,ET>& fil2)
{ return CGAL_compare(fil.value, fil2.value); }

template <class CT, class ET>
inline CGAL_Filtering<CT,ET> CGAL_abs (const CGAL_Filtering<CT,ET>& fil)
{ return CGAL_abs(fil.value); }

template <class CT, class ET>
inline CGAL_Filtering<CT,ET> CGAL_min (const CGAL_Filtering<CT,ET>& fil,
				       const CGAL_Filtering<CT,ET>& fil2)
{ return CGAL_min(fil.value, fil2.value); }

template <class CT, class ET>
inline CGAL_Filtering<CT,ET> CGAL_max (const CGAL_Filtering<CT,ET>& fil,
				       const CGAL_Filtering<CT,ET>& fil2)
{ return CGAL_max(fil.value, fil2.value); }

template <class CT, class ET>
inline CGAL_io_Operator CGAL_io_tag(CGAL_Filtering<CT,ET> &fil)
{ return CGAL_io_tag(fil.value); }

template <class CT, class ET>
inline CGAL_Number_tag CGAL_number_type_tag(CGAL_Filtering<CT,ET> &fil)
{ return CGAL_number_type_tag(fil.value); }

// Sqrt() is inexact => restricted.
template <class CT, class ET>
inline CGAL_Filtering_allow_inexact<CT,ET> sqrt
	(const CGAL_Filtering_allow_inexact<CT,ET>& fil)
{ return sqrt(fil.value); }

template <class CT, class ET>
inline ostream& operator<<(ostream& os, const CGAL_Filtering<CT,ET>& d)
{ return os << d.value; }

template <class CT, class ET>
inline istream &operator>>(istream &is, const CGAL_Filtering<CT,ET>& d)
{ return is >> d.value; }


// template <class ET>
  // template <class CT>
    // ET CGAL_to_exact_type(const CT &);

// All exact types should reasonnably have a built-in exact conversion
// from doubles ?  If not, it will fail, and you have to provide it.
//
// It's bad to provide such a default, because it can be an inexact cast:
// ex: CGAL_Gmpz accepts it, but it's false !!!

template <class ET>
inline ET CGAL_to_exact_type (const double & d)
{ return ET(d); }

// When CT == ET.
template <class ET>
inline ET CGAL_to_exact_type (const ET & e)
{ return e; }


#ifdef CGAL_PREDICATES_ON_FTC2_H
#include <CGAL/Filter/predicates_on_ftC2.h>
#endif

#ifdef CGAL_PREDICATES_ON_RTH2_H
#include <CGAL/Filter/predicates_on_rtH2.h>
#endif

#endif // CGAL_FILTER_H
