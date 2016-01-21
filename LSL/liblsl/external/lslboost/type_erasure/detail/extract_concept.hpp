// Boost.TypeErasure library
//
// Copyright 2011 Steven Watanabe
//
// Distributed under the Boost Software License Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.lslboost.org/LICENSE_1_0.txt)
//
// $Id$

#if !defined(BOOST_PP_IS_ITERATING)

#ifndef BOOST_TYPE_ERASURE_DETAIL_EXTRACT_CONCEPT_HPP_INCLUDED
#define BOOST_TYPE_ERASURE_DETAIL_EXTRACT_CONCEPT_HPP_INCLUDED

#include <lslboost/mpl/eval_if.hpp>
#include <lslboost/mpl/identity.hpp>
#include <lslboost/type_traits/remove_cv.hpp>
#include <lslboost/type_traits/remove_reference.hpp>
#include <lslboost/preprocessor/cat.hpp>
#include <lslboost/preprocessor/inc.hpp>
#include <lslboost/preprocessor/iteration/iterate.hpp>
#include <lslboost/preprocessor/repetition/repeat.hpp>
#include <lslboost/preprocessor/repetition/enum_params.hpp>
#include <lslboost/type_erasure/is_placeholder.hpp>
#include <lslboost/type_erasure/concept_of.hpp>
#include <lslboost/type_erasure/config.hpp>

namespace lslboost {
namespace type_erasure {
namespace detail {

template<class T, class U>
struct combine_concepts;

template<class T>
struct combine_concepts<T, T> { typedef T type; };
template<class T>
struct combine_concepts<T, void> { typedef T type; };
template<class T>
struct combine_concepts<void, T> { typedef T type; };
template<>
struct combine_concepts<void, void> { typedef void type; };

template<class T, class U>
struct maybe_extract_concept
{
    typedef typename ::lslboost::mpl::eval_if<
        ::lslboost::type_erasure::is_placeholder<
            typename ::lslboost::remove_cv<
                typename ::lslboost::remove_reference<T>::type
            >::type
        >,
        ::lslboost::type_erasure::concept_of<typename ::lslboost::remove_reference<U>::type>,
        ::lslboost::mpl::identity<void>
    >::type type;
};

#if !defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)

template<class Args, class... U>
struct extract_concept;

template<class R, class T0, class... T, class U0, class... U>
struct extract_concept<R(T0, T...), U0, U...>
{
    typedef typename ::lslboost::type_erasure::detail::combine_concepts<
        typename ::lslboost::type_erasure::detail::maybe_extract_concept<
            T0, U0
        >::type,
        typename ::lslboost::type_erasure::detail::extract_concept<
            void(T...),
            U...
        >::type
    >::type type;
};

template<>
struct extract_concept<void()>
{
    typedef void type;
};

#else
//#endif

#define BOOST_PP_FILENAME_1 <lslboost/type_erasure/detail/extract_concept.hpp>
#define BOOST_PP_ITERATION_LIMITS (1, BOOST_TYPE_ERASURE_MAX_ARITY)
#include BOOST_PP_ITERATE()

#endif

}
}
}

#endif

#else

#define N BOOST_PP_ITERATION()

#define BOOST_TYPE_ERASURE_EXTRACT_CONCEPT(z, n, data)                  \
    typedef typename ::lslboost::type_erasure::detail::combine_concepts<   \
        typename ::lslboost::type_erasure::detail::maybe_extract_concept<  \
            BOOST_PP_CAT(T, n), BOOST_PP_CAT(U, n)                      \
        >::type,                                                        \
        BOOST_PP_CAT(concept, n)                                        \
    >::type BOOST_PP_CAT(concept, BOOST_PP_INC(n));

template<
    BOOST_PP_ENUM_PARAMS(N, class T),
    BOOST_PP_ENUM_PARAMS(N, class U)>
struct BOOST_PP_CAT(extract_concept, N)
{
    typedef void concept0;

    BOOST_PP_REPEAT(N, BOOST_TYPE_ERASURE_EXTRACT_CONCEPT, ~)

    typedef BOOST_PP_CAT(concept, N) type;
};

#undef BOOST_TYPE_ERASURE_EXTRACT_CONCEPT
#undef N

#endif