/*
 *  Copyright (c) 2016, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree. An additional grant
 *  of patent rights can be found in the PATENTS file in the same directory.
 */

#ifndef FATAL_INCLUDE_fatal_type_data_member_getter_h
#define FATAL_INCLUDE_fatal_type_data_member_getter_h

#include <fatal/preprocessor.h>
#include <fatal/type/add_reference_from.h>
#include <fatal/type/constify_from.h>
#include <fatal/type/sequence.h>

#include <type_traits>
#include <utility>

namespace fatal {

/**
 * TODO: DOCUMENT
 *
 *  template <typename Owner>
 *  using type;
 *
 *  template <typename Owner>
 *  using reference;
 *
 *  using name = typename impl::name;
 *
 *  template <typename Owner>
 *  static reference<Owner> ref(Owner &&owner);
 *
 *  struct ref_getter {
 *    template <typename Owner>
 *    reference<Owner> operator ()(Owner &&owner) const;
 *  };
 *
 *  template <typename Owner>
 *  using pointer;
 *
 *  template <typename Owner>
 *  static pointer<Owner> ptr(Owner &owner);
 *
 *  struct ptr_getter {
 *    template <typename Owner>
 *    pointer<Owner> operator ()(Owner &owner) const;
 *  };
 *
 * @author: Marcelo Juchem <marcelo@fb.com>
 */
#define FATAL_DATA_MEMBER_GETTER(Name, ...) \
  FATAL_IMPL_DATA_MEMBER_GETTER( \
    Name, \
    FATAL_UID(FATAL_CAT(Name, _data_member_getter_impl)), \
    __VA_ARGS__ \
  ) \

namespace detail {

template <typename> struct tag {};

template <typename Impl>
class data_member_getter {
  using impl = Impl;

  struct has_impl {
    template <typename T, typename U = typename impl::template type<T>>
    static std::true_type sfinae(tag<T>);

    template <typename...>
    static std::false_type sfinae(...);
  };

public:
  template <typename Owner>
  using type = typename impl::template type<Owner>;

  template <typename Owner>
  using reference = typename impl::template reference<Owner>::ref_impl;

  using name = typename impl::name;

  template <typename Owner>
  using has = decltype(has_impl::sfinae(tag<Owner>()));

  template <typename Owner>
  static reference<Owner> ref(Owner &&owner) {
    static_assert(std::is_reference<reference<Owner>>::value, "");
    return impl::ref(std::forward<Owner>(owner));
  }

  struct ref_getter {
    template <typename Owner>
    reference<Owner> operator ()(Owner &&owner) const {
      static_assert(std::is_reference<reference<Owner>>::value, "");
      return ref(std::forward<Owner>(owner));
    }
  };

  template <typename Owner>
  using pointer = typename std::remove_reference<reference<Owner>>::type *;

  template <typename Owner>
  static pointer<Owner> ptr(Owner &owner) {
    static_assert(std::is_pointer<pointer<Owner>>::value, "");
    return std::addressof(ref(owner));
  }

  struct ptr_getter {
    template <typename Owner>
    pointer<Owner> operator ()(Owner &owner) const {
      static_assert(std::is_pointer<pointer<Owner>>::value, "");
      return ptr(owner);
    }
  };
};

#define FATAL_IMPL_DATA_MEMBER_GETTER(Class, Impl, ...) \
  struct Impl { \
    template <typename Owner> \
    using type = decltype(::std::declval<Owner>().__VA_ARGS__); \
    \
    template <typename Owner> \
    struct reference { \
      using ref_impl = typename ::fatal::add_reference_from< \
        typename ::fatal::constify_from< \
          type<Owner>, \
          typename ::std::remove_reference<Owner>::type \
        >::type, \
        Owner && \
      >::type; \
      \
      static_assert(std::is_reference<ref_impl>::value, ""); \
    }; \
    \
    FATAL_S(name, FATAL_TO_STR(__VA_ARGS__)); \
    \
    template <typename Owner> \
    static typename reference<Owner>::ref_impl ref(Owner &&owner) { \
      return static_cast<typename reference<Owner>::ref_impl>( \
        ::std::forward<Owner>(owner).__VA_ARGS__ \
      ); \
    } \
  }; \
  \
  using Class = ::fatal::detail::data_member_getter<Impl>

} // namespace detail {

////////////////////////
// data_member_getter //
////////////////////////

/**
 * Instantiations of FATAL_DATA_MEMBER_GETTER
 * for some popular names.
 *
 * @author: Marcelo Juchem <marcelo@fb.com>
 */
struct data_member_getter {
# define FATAL_IMPL_WELL_KNOWN_DATA_MEMBER_GETTER(...) \
  FATAL_DATA_MEMBER_GETTER(__VA_ARGS__, __VA_ARGS__); \
  FATAL_DATA_MEMBER_GETTER(FATAL_CAT(__VA_ARGS__, _), FATAL_CAT(__VA_ARGS__, _))

  FATAL_IMPL_WELL_KNOWN_DATA_MEMBER_GETTER(allocator);
  FATAL_IMPL_WELL_KNOWN_DATA_MEMBER_GETTER(args);
  FATAL_IMPL_WELL_KNOWN_DATA_MEMBER_GETTER(array);
  FATAL_IMPL_WELL_KNOWN_DATA_MEMBER_GETTER(category);
  FATAL_IMPL_WELL_KNOWN_DATA_MEMBER_GETTER(config);
  FATAL_IMPL_WELL_KNOWN_DATA_MEMBER_GETTER(data);
  FATAL_IMPL_WELL_KNOWN_DATA_MEMBER_GETTER(decoder);
  FATAL_IMPL_WELL_KNOWN_DATA_MEMBER_GETTER(difference);
  FATAL_IMPL_WELL_KNOWN_DATA_MEMBER_GETTER(element);
  FATAL_IMPL_WELL_KNOWN_DATA_MEMBER_GETTER(encoder);
  FATAL_IMPL_WELL_KNOWN_DATA_MEMBER_GETTER(extension);
  FATAL_IMPL_WELL_KNOWN_DATA_MEMBER_GETTER(first);
  FATAL_IMPL_WELL_KNOWN_DATA_MEMBER_GETTER(flag);
  FATAL_IMPL_WELL_KNOWN_DATA_MEMBER_GETTER(hash);
  FATAL_IMPL_WELL_KNOWN_DATA_MEMBER_GETTER(id);
  FATAL_IMPL_WELL_KNOWN_DATA_MEMBER_GETTER(ids);
  FATAL_IMPL_WELL_KNOWN_DATA_MEMBER_GETTER(index);
  FATAL_IMPL_WELL_KNOWN_DATA_MEMBER_GETTER(info);
  FATAL_IMPL_WELL_KNOWN_DATA_MEMBER_GETTER(information);
  FATAL_IMPL_WELL_KNOWN_DATA_MEMBER_GETTER(instance);
  FATAL_IMPL_WELL_KNOWN_DATA_MEMBER_GETTER(item);
  FATAL_IMPL_WELL_KNOWN_DATA_MEMBER_GETTER(iterator);
  FATAL_IMPL_WELL_KNOWN_DATA_MEMBER_GETTER(key);
  FATAL_IMPL_WELL_KNOWN_DATA_MEMBER_GETTER(list);
  FATAL_IMPL_WELL_KNOWN_DATA_MEMBER_GETTER(map);
  FATAL_IMPL_WELL_KNOWN_DATA_MEMBER_GETTER(mapped);
  FATAL_IMPL_WELL_KNOWN_DATA_MEMBER_GETTER(mapping);
  FATAL_IMPL_WELL_KNOWN_DATA_MEMBER_GETTER(mappings);
  FATAL_IMPL_WELL_KNOWN_DATA_MEMBER_GETTER(member);
  FATAL_IMPL_WELL_KNOWN_DATA_MEMBER_GETTER(members);
  FATAL_IMPL_WELL_KNOWN_DATA_MEMBER_GETTER(name);
  FATAL_IMPL_WELL_KNOWN_DATA_MEMBER_GETTER(names);
  FATAL_IMPL_WELL_KNOWN_DATA_MEMBER_GETTER(pair);
  FATAL_IMPL_WELL_KNOWN_DATA_MEMBER_GETTER(pointer);
  FATAL_IMPL_WELL_KNOWN_DATA_MEMBER_GETTER(predicate);
  FATAL_IMPL_WELL_KNOWN_DATA_MEMBER_GETTER(ptr);
  FATAL_IMPL_WELL_KNOWN_DATA_MEMBER_GETTER(reader);
  FATAL_IMPL_WELL_KNOWN_DATA_MEMBER_GETTER(ref);
  FATAL_IMPL_WELL_KNOWN_DATA_MEMBER_GETTER(reference);
  FATAL_IMPL_WELL_KNOWN_DATA_MEMBER_GETTER(request);
  FATAL_IMPL_WELL_KNOWN_DATA_MEMBER_GETTER(response);
  FATAL_IMPL_WELL_KNOWN_DATA_MEMBER_GETTER(result);
  FATAL_IMPL_WELL_KNOWN_DATA_MEMBER_GETTER(second);
  FATAL_IMPL_WELL_KNOWN_DATA_MEMBER_GETTER(set);
  FATAL_IMPL_WELL_KNOWN_DATA_MEMBER_GETTER(size);
  FATAL_IMPL_WELL_KNOWN_DATA_MEMBER_GETTER(str);
  FATAL_IMPL_WELL_KNOWN_DATA_MEMBER_GETTER(string);
  FATAL_IMPL_WELL_KNOWN_DATA_MEMBER_GETTER(tag);
  FATAL_IMPL_WELL_KNOWN_DATA_MEMBER_GETTER(tuple);
  FATAL_IMPL_WELL_KNOWN_DATA_MEMBER_GETTER(type);
  FATAL_IMPL_WELL_KNOWN_DATA_MEMBER_GETTER(types);
  FATAL_IMPL_WELL_KNOWN_DATA_MEMBER_GETTER(value);
  FATAL_IMPL_WELL_KNOWN_DATA_MEMBER_GETTER(values);
  FATAL_IMPL_WELL_KNOWN_DATA_MEMBER_GETTER(version);
  FATAL_IMPL_WELL_KNOWN_DATA_MEMBER_GETTER(writer);

# undef FATAL_IMPL_WELL_KNOWN_DATA_MEMBER_GETTER
};

/**
 * TODO: DOCUMENT
 *
 * @author: Marcelo Juchem <marcelo@fb.com>
 */
template <typename...> class chained_data_member_getter;

template <typename OuterGetter, typename... Getters>
class chained_data_member_getter<OuterGetter, Getters...> {
  using tail = chained_data_member_getter<Getters...>;

public:
  template <typename Owner>
  using type = typename tail::template type<
    typename OuterGetter::template type<Owner>
  >;

  template <typename Owner>
  using reference = typename tail::template reference<
    typename OuterGetter::template reference<Owner>
  >;

  // TODO: IMPLEMENT `has`

  template <typename Owner>
  static reference<Owner> ref(Owner &&owner) {
    return tail::ref(OuterGetter::ref(std::forward<Owner>(owner)));
  }

  struct ref_getter {
    template <typename Owner>
    reference<Owner> operator ()(Owner &&owner) const {
      return ref(std::forward<Owner>(owner));
    }
  };

  template <typename Owner>
  using pointer = typename tail::template pointer<
    typename OuterGetter::template reference<Owner>
  >;

  template <typename Owner>
  static pointer<Owner> ptr(Owner &owner) {
    auto &&local = OuterGetter::ref(std::forward<Owner>(owner));
    return tail::ptr(local);
  }

  struct ptr_getter {
    template <typename Owner>
    pointer<Owner> operator ()(Owner &owner) const { return ptr(owner); }
  };
};

template <>
class chained_data_member_getter<> {
public:
  template <typename Owner>
  using type = Owner;

  template <typename Owner>
  using reference = Owner &&;

  template <typename Owner>
  static reference<Owner> ref(Owner &&owner) {
    return std::forward<Owner>(owner);
  }

  struct ref_getter {
    template <typename Owner>
    reference<Owner> operator ()(Owner &&owner) const {
      return ref(std::forward<Owner>(owner));
    }
  };

  template <typename Owner>
  using pointer = typename std::remove_reference<Owner>::type *;

  template <typename Owner>
  static pointer<Owner> ptr(Owner &owner) { return std::addressof(owner); }

  struct ptr_getter {
    template <typename Owner>
    pointer<Owner> operator ()(Owner &owner) const { return ptr(owner); }
  };
};

} // namespace fatal

#endif // FATAL_INCLUDE_fatal_type_data_member_getter_h
