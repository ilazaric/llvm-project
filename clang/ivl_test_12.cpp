// #include <variant>

#  include <__compare/common_comparison_category.h>
#  include <__compare/compare_three_way_result.h>
#  include <__compare/ordering.h>
#  include <__compare/three_way_comparable.h>
#  include <__config>
#  include <__exception/exception.h>
#  include <__functional/hash.h>
#  include <__functional/operations.h>
#  include <__functional/unary_function.h>
#  include <__fwd/variant.h>
#  include <__memory/addressof.h>
#  include <__memory/construct_at.h>
#  include <__tuple/find_index.h>
#  include <__tuple/sfinae_helpers.h>
#  include <__type_traits/add_cv_quals.h>
#  include <__type_traits/add_pointer.h>
#  include <__type_traits/common_type.h>
#  include <__type_traits/conditional.h>
#  include <__type_traits/conjunction.h>
#  include <__type_traits/decay.h>
#  include <__type_traits/dependent_type.h>
#  include <__type_traits/enable_if.h>
#  include <__type_traits/invoke.h>
#  include <__type_traits/is_array.h>
#  include <__type_traits/is_assignable.h>
#  include <__type_traits/is_constructible.h>
#  include <__type_traits/is_convertible.h>
#  include <__type_traits/is_core_convertible.h>
#  include <__type_traits/is_destructible.h>
#  include <__type_traits/is_nothrow_assignable.h>
#  include <__type_traits/is_nothrow_constructible.h>
#  include <__type_traits/is_reference.h>
#  include <__type_traits/is_replaceable.h>
#  include <__type_traits/is_same.h>
#  include <__type_traits/is_swappable.h>
#  include <__type_traits/is_trivially_assignable.h>
#  include <__type_traits/is_trivially_constructible.h>
#  include <__type_traits/is_trivially_destructible.h>
#  include <__type_traits/is_trivially_relocatable.h>
#  include <__type_traits/is_void.h>
#  include <__type_traits/remove_const.h>
#  include <__type_traits/remove_cvref.h>
#  include <__type_traits/remove_reference.h>
#  include <__type_traits/type_identity.h>
#  include <__type_traits/void_t.h>
#  include <__utility/declval.h>
#  include <__utility/forward.h>
#  include <__utility/forward_like.h>
#  include <__utility/in_place.h>
#  include <__utility/integer_sequence.h>
#  include <__utility/move.h>
#  include <__utility/swap.h>
#  include <__variant/monostate.h>
#  include <__verbose_abort>
#  include <initializer_list>
#  include <limits>
#  include <version>

// standard-mandated includes

// [variant.syn]
#  include <compare>

_LIBCPP_PUSH_MACROS
#  include <__undef_macros>

_LIBCPP_BEGIN_UNVERSIONED_NAMESPACE_STD

class _LIBCPP_EXPORTED_FROM_ABI bad_variant_access : public exception {
public:
  const char* what() const _NOEXCEPT override;
};

_LIBCPP_END_UNVERSIONED_NAMESPACE_STD

_LIBCPP_BEGIN_NAMESPACE_STD

#  if _LIBCPP_STD_VER >= 17

// Light N-dimensional array of function pointers. Used in place of std::array to avoid
// adding a dependency.
template <class _Tp, size_t _Size>
struct __farray {
  static_assert(_Size > 0, "N-dimensional array should never be empty in std::visit");
  _Tp __buf_[_Size] = {};

  _LIBCPP_HIDE_FROM_ABI constexpr const _Tp& operator[](size_t __n) const noexcept { return __buf_[__n]; }
};

[[noreturn]] inline _LIBCPP_HIDE_FROM_ABI void __throw_bad_variant_access() {
#    if _LIBCPP_HAS_EXCEPTIONS
  throw bad_variant_access();
#    else
  _LIBCPP_VERBOSE_ABORT("bad_variant_access was thrown in -fno-exceptions mode");
#    endif
}

// variant_size
template <class _Tp>
struct variant_size<const _Tp> : variant_size<_Tp> {};

template <class _Tp>
struct variant_size<volatile _Tp> : variant_size<_Tp> {};

template <class _Tp>
struct variant_size<const volatile _Tp> : variant_size<_Tp> {};

template <class... _Types>
struct variant_size<variant<_Types...>> : integral_constant<size_t, sizeof...(_Types)> {};

// variant_alternative
template <size_t _Ip, class _Tp>
struct variant_alternative<_Ip, const _Tp> : add_const<variant_alternative_t<_Ip, _Tp>> {};

template <size_t _Ip, class _Tp>
struct variant_alternative<_Ip, volatile _Tp> : add_volatile<variant_alternative_t<_Ip, _Tp>> {};

template <size_t _Ip, class _Tp>
struct variant_alternative<_Ip, const volatile _Tp> : add_cv<variant_alternative_t<_Ip, _Tp>> {};

template <size_t _Ip, class... _Types>
struct variant_alternative<_Ip, variant<_Types...>> {
  static_assert(_Ip < sizeof...(_Types), "Index out of bounds in std::variant_alternative<>");
  using type _LIBCPP_NODEBUG = __type_pack_element<_Ip, _Types...>;
};

template <size_t _NumAlternatives>
_LIBCPP_HIDE_FROM_ABI constexpr auto __choose_index_type() {
#    ifdef _LIBCPP_ABI_VARIANT_INDEX_TYPE_OPTIMIZATION
  if constexpr (_NumAlternatives < numeric_limits<unsigned char>::max())
    return static_cast<unsigned char>(0);
  else if constexpr (_NumAlternatives < numeric_limits<unsigned short>::max())
    return static_cast<unsigned short>(0);
  else
#    endif // _LIBCPP_ABI_VARIANT_INDEX_TYPE_OPTIMIZATION
    return static_cast<unsigned int>(0);
}

template <size_t _NumAlts>
using __variant_index_t _LIBCPP_NODEBUG = decltype(std::__choose_index_type<_NumAlts>());

template <class _IndexType>
constexpr _IndexType __variant_npos = static_cast<_IndexType>(-1);

template <class... _Types>
_LIBCPP_HIDE_FROM_ABI constexpr variant<_Types...>& __as_variant(variant<_Types...>& __vs) noexcept {
  return __vs;
}

template <class... _Types>
_LIBCPP_HIDE_FROM_ABI constexpr const variant<_Types...>& __as_variant(const variant<_Types...>& __vs) noexcept {
  return __vs;
}

template <class... _Types>
_LIBCPP_HIDE_FROM_ABI constexpr variant<_Types...>&& __as_variant(variant<_Types...>&& __vs) noexcept {
  return std::move(__vs);
}

template <class... _Types>
_LIBCPP_HIDE_FROM_ABI constexpr const variant<_Types...>&& __as_variant(const variant<_Types...>&& __vs) noexcept {
  return std::move(__vs);
}

namespace __find_detail {

template <class _Tp, class... _Types>
_LIBCPP_HIDE_FROM_ABI constexpr size_t __find_index() {
  constexpr bool __matches[] = {is_same_v<_Tp, _Types>...};
  size_t __result            = __not_found;
  for (size_t __i = 0; __i < sizeof...(_Types); ++__i) {
    if (__matches[__i]) {
      if (__result != __not_found) {
        return __ambiguous;
      }
      __result = __i;
    }
  }
  return __result;
}

template <size_t _Index>
struct __find_unambiguous_index_sfinae_impl : integral_constant<size_t, _Index> {};

template <>
struct __find_unambiguous_index_sfinae_impl<__not_found> {};

template <>
struct __find_unambiguous_index_sfinae_impl<__ambiguous> {};

template <class _Tp, class... _Types>
struct __find_unambiguous_index_sfinae
    : __find_unambiguous_index_sfinae_impl<__find_detail::__find_index<_Tp, _Types...>()> {};

} // namespace __find_detail

namespace __variant_detail {

struct __valueless_t {};

enum class _Trait { _TriviallyAvailable, _Available, _Unavailable };

template <typename _Tp, template <typename> class _IsTriviallyAvailable, template <typename> class _IsAvailable>
constexpr _Trait __trait =
    _IsTriviallyAvailable<_Tp>::value ? _Trait::_TriviallyAvailable
    : _IsAvailable<_Tp>::value
        ? _Trait::_Available
        : _Trait::_Unavailable;

_LIBCPP_HIDE_FROM_ABI constexpr _Trait __common_trait(initializer_list<_Trait> __traits) {
  _Trait __result = _Trait::_TriviallyAvailable;
  for (_Trait __t : __traits) {
    if (static_cast<int>(__t) > static_cast<int>(__result)) {
      __result = __t;
    }
  }
  return __result;
}

template <typename... _Types>
struct __traits {
  static constexpr _Trait __copy_constructible_trait =
      __variant_detail::__common_trait({__trait<_Types, is_trivially_copy_constructible, is_copy_constructible>...});

  static constexpr _Trait __move_constructible_trait =
      __variant_detail::__common_trait({__trait<_Types, is_trivially_move_constructible, is_move_constructible>...});

  static constexpr _Trait __copy_assignable_trait = __variant_detail::__common_trait(
      {__copy_constructible_trait, __trait<_Types, is_trivially_copy_assignable, is_copy_assignable>...});

  static constexpr _Trait __move_assignable_trait = __variant_detail::__common_trait(
      {__move_constructible_trait, __trait<_Types, is_trivially_move_assignable, is_move_assignable>...});

  static constexpr _Trait __destructible_trait =
      __variant_detail::__common_trait({__trait<_Types, is_trivially_destructible, is_destructible>...});
};

namespace __access {

struct __union {
  template <class _Vp>
  _LIBCPP_HIDE_FROM_ABI static constexpr auto&& __get_alt(_Vp&& __v, in_place_index_t<0>) {
    return std::forward<_Vp>(__v).__head;
  }

  template <class _Vp, size_t _Ip>
  _LIBCPP_HIDE_FROM_ABI static constexpr auto&& __get_alt(_Vp&& __v, in_place_index_t<_Ip>) {
    return __get_alt(std::forward<_Vp>(__v).__tail, in_place_index<_Ip - 1>);
  }
};

struct __base {
  template <size_t _Ip, class _Vp>
  _LIBCPP_HIDE_FROM_ABI static constexpr auto&& __get_alt(_Vp&& __v) {
    return __union::__get_alt(std::forward<_Vp>(__v).__data, in_place_index<_Ip>);
  }
};

struct __variant {
  template <size_t _Ip, class _Vp>
  _LIBCPP_HIDE_FROM_ABI static constexpr auto&& __get_alt(_Vp&& __v) {
    return __base::__get_alt<_Ip>(std::forward<_Vp>(__v).__impl_);
  }
};

} // namespace __access

namespace __visitation {

struct __base {
  template <class _Visitor, class... _Vs>
  _LIBCPP_HIDE_FROM_ABI static constexpr decltype(auto)
  __visit_alt_at(size_t __index, _Visitor&& __visitor, _Vs&&... __vs);
  
  template <class _Visitor, class... _Vs>
  _LIBCPP_HIDE_FROM_ABI static constexpr decltype(auto) __visit_alt(_Visitor&& __visitor, _Vs&&... __vs);
  
private:
  template <class _Tp>
  _LIBCPP_HIDE_FROM_ABI static constexpr const _Tp& __at(const _Tp& __elem) {
    return __elem;
  }

  template <class _Tp, size_t _Np, typename... _Indices>
  _LIBCPP_HIDE_FROM_ABI static constexpr auto&&
  __at(const __farray<_Tp, _Np>& __elems, size_t __index, _Indices... __indices) {
    return __at(__elems[__index], __indices...);
  }

  template <class _Fp, class... _Fs>
  static _LIBCPP_HIDE_FROM_ABI constexpr void __std_visit_visitor_return_type_check() {
    static_assert(
        __all<is_same_v<_Fp, _Fs>...>::value, "`std::visit` requires the visitor to have a single return type.");
  }

  template <class... _Fs>
  _LIBCPP_HIDE_FROM_ABI static constexpr auto __make_farray(_Fs&&... __fs) {
    __std_visit_visitor_return_type_check<__remove_cvref_t<_Fs>...>();
    using __result = __farray<common_type_t<__remove_cvref_t<_Fs>...>, sizeof...(_Fs)>;
    return __result{{std::forward<_Fs>(__fs)...}};
  }

  template <size_t... _Is>
  struct __dispatcher {
    template <class _Fp, class... _Vs>
    _LIBCPP_HIDE_FROM_ABI static constexpr decltype(auto) __dispatch(_Fp __f, _Vs... __vs) {
      return std::__invoke(static_cast<_Fp>(__f), __access::__base::__get_alt<_Is>(static_cast<_Vs>(__vs))...);
    }
  };

  template <class _Fp, class... _Vs, size_t... _Is>
  _LIBCPP_HIDE_FROM_ABI static constexpr auto __make_dispatch(index_sequence<_Is...>) {
    return __dispatcher<_Is...>::template __dispatch<_Fp, _Vs...>;
  }

  template <size_t _Ip, class _Fp, class... _Vs>
  _LIBCPP_HIDE_FROM_ABI static constexpr auto __make_fdiagonal_impl() {
    return __make_dispatch<_Fp, _Vs...>(index_sequence<((void)__type_identity<_Vs>{}, _Ip)...>{});
  }

  template <class _Fp, class... _Vs, size_t... _Is>
  _LIBCPP_HIDE_FROM_ABI static constexpr auto __make_fdiagonal_impl(index_sequence<_Is...>) {
    return __base::__make_farray(__make_fdiagonal_impl<_Is, _Fp, _Vs...>()...);
  }

  template <class _Fp, class _Vp, class... _Vs>
  _LIBCPP_HIDE_FROM_ABI static constexpr auto __make_fdiagonal();
  
  template <class _Fp, class... _Vs, size_t... _Is>
  _LIBCPP_HIDE_FROM_ABI static constexpr auto __make_fmatrix_impl(index_sequence<_Is...> __is) {
    return __make_dispatch<_Fp, _Vs...>(__is);
  }

  template <class _Fp, class... _Vs, size_t... _Is, size_t... _Js, class... _Ls>
  _LIBCPP_HIDE_FROM_ABI static constexpr auto
  __make_fmatrix_impl(index_sequence<_Is...>, index_sequence<_Js...>, _Ls... __ls);
  
  template <class _Fp, class... _Vs>
  _LIBCPP_HIDE_FROM_ABI static constexpr auto __make_fmatrix() {
    return __make_fmatrix_impl<_Fp, _Vs...>(
        index_sequence<>{}, make_index_sequence<__remove_cvref_t<_Vs>::__size()>{}...);
  }
};

struct __variant {
  template <class _Visitor, class... _Vs>
  _LIBCPP_HIDE_FROM_ABI static constexpr decltype(auto)
  __visit_alt_at(size_t __index, _Visitor&& __visitor, _Vs&&... __vs) {
    return __base::__visit_alt_at(__index, std::forward<_Visitor>(__visitor), std::forward<_Vs>(__vs).__impl_...);
  }

  template <class _Visitor, class... _Vs>
  _LIBCPP_HIDE_FROM_ABI static constexpr decltype(auto) __visit_alt(_Visitor&& __visitor, _Vs&&... __vs) {
    return __base::__visit_alt(
        std::forward<_Visitor>(__visitor), std::__as_variant(std::forward<_Vs>(__vs)).__impl_...);
  }

  template <class _Visitor, class... _Vs>
  _LIBCPP_HIDE_FROM_ABI static constexpr decltype(auto)
  __visit_value_at(size_t __index, _Visitor&& __visitor, _Vs&&... __vs) {
    return __visit_alt_at(__index, __make_value_visitor(std::forward<_Visitor>(__visitor)), std::forward<_Vs>(__vs)...);
  }

  template <class _Visitor, class... _Vs>
  _LIBCPP_HIDE_FROM_ABI static constexpr decltype(auto) __visit_value(_Visitor&& __visitor, _Vs&&... __vs) {
    return __visit_alt(__make_value_visitor(std::forward<_Visitor>(__visitor)), std::forward<_Vs>(__vs)...);
  }

#    if _LIBCPP_STD_VER >= 20
  template <class _Rp, class _Visitor, class... _Vs>
  _LIBCPP_HIDE_FROM_ABI static constexpr _Rp __visit_value(_Visitor&& __visitor, _Vs&&... __vs) {
    return __visit_alt(__make_value_visitor<_Rp>(std::forward<_Visitor>(__visitor)), std::forward<_Vs>(__vs)...);
  }
#    endif

private:
  template <class _Visitor, class... _Values>
  static _LIBCPP_HIDE_FROM_ABI constexpr void __std_visit_exhaustive_visitor_check() {
    static_assert(is_invocable_v<_Visitor, _Values...>, "`std::visit` requires the visitor to be exhaustive.");
  }

  template <class _Visitor>
  struct __value_visitor {
    template <class... _Alts>
    _LIBCPP_HIDE_FROM_ABI constexpr decltype(auto) operator()(_Alts&&... __alts) const {
      __std_visit_exhaustive_visitor_check< _Visitor, decltype((std::forward<_Alts>(__alts).__value))...>();
      return std::__invoke(std::forward<_Visitor>(__visitor), std::forward<_Alts>(__alts).__value...);
    }
    _Visitor&& __visitor;
  };

#    if _LIBCPP_STD_VER >= 20
  template <class _Rp, class _Visitor>
  struct __value_visitor_return_type {
    template <class... _Alts>
    _LIBCPP_HIDE_FROM_ABI constexpr _Rp operator()(_Alts&&... __alts) const {
      __std_visit_exhaustive_visitor_check< _Visitor, decltype((std::forward<_Alts>(__alts).__value))...>();
      if constexpr (is_void_v<_Rp>) {
        std::__invoke(std::forward<_Visitor>(__visitor), std::forward<_Alts>(__alts).__value...);
      } else {
        return std::__invoke(std::forward<_Visitor>(__visitor), std::forward<_Alts>(__alts).__value...);
      }
    }

    _Visitor&& __visitor;
  };
#    endif

  template <class _Visitor>
  _LIBCPP_HIDE_FROM_ABI static constexpr auto __make_value_visitor(_Visitor&& __visitor) {
    return __value_visitor<_Visitor>{std::forward<_Visitor>(__visitor)};
  }

#    if _LIBCPP_STD_VER >= 20
  template <class _Rp, class _Visitor>
  _LIBCPP_HIDE_FROM_ABI static constexpr auto __make_value_visitor(_Visitor&& __visitor) {
    return __value_visitor_return_type<_Rp, _Visitor>{std::forward<_Visitor>(__visitor)};
  }
#    endif
};

} // namespace __visitation

// Adding semi-colons in macro expansions helps clang-format to do a better job.
// This macro is used to avoid compilation errors due to "stray" semi-colons.
#    define _LIBCPP_EAT_SEMICOLON static_assert(true, "")

template <size_t _Index, class _Tp>
struct __alt {
  using __value_type _LIBCPP_NODEBUG = _Tp;
  static constexpr size_t __index    = _Index;

  template <class... _Args>
  _LIBCPP_HIDE_FROM_ABI explicit constexpr __alt(in_place_t, _Args&&... __args)
      : __value(std::forward<_Args>(__args)...) {}

  __value_type __value;
};

template <_Trait _DestructibleTrait, size_t _Index, class... _Types>
union __union;

template <_Trait _DestructibleTrait, size_t _Index>
union __union<_DestructibleTrait, _Index> {};

#    define _LIBCPP_VARIANT_UNION(destructible_trait, destructor_definition)                                           \
      template <size_t _Index, class _Tp, class... _Types>                                                             \
      union __union<destructible_trait, _Index, _Tp, _Types...> {                                                      \
      public:                                                                                                          \
        _LIBCPP_HIDE_FROM_ABI explicit constexpr __union(__valueless_t) noexcept : __dummy{} {}                        \
                                                                                                                       \
        template <class... _Args>                                                                                      \
        _LIBCPP_HIDE_FROM_ABI explicit constexpr __union(in_place_index_t<0>, _Args&&... __args)                       \
            : __head(in_place, std::forward<_Args>(__args)...) {}                                                      \
                                                                                                                       \
        template <size_t _Ip, class... _Args>                                                                          \
        _LIBCPP_HIDE_FROM_ABI explicit constexpr __union(in_place_index_t<_Ip>, _Args&&... __args)                     \
            : __tail(in_place_index<_Ip - 1>, std::forward<_Args>(__args)...) {}                                       \
                                                                                                                       \
        _LIBCPP_HIDE_FROM_ABI __union(const __union&)            = default;                                            \
        _LIBCPP_HIDE_FROM_ABI __union(__union&&)                 = default;                                            \
        _LIBCPP_HIDE_FROM_ABI __union& operator=(const __union&) = default;                                            \
        _LIBCPP_HIDE_FROM_ABI __union& operator=(__union&&)      = default;                                            \
        destructor_definition;                                                                                         \
                                                                                                                       \
      private:                                                                                                         \
        char __dummy;                                                                                                  \
        __alt<_Index, _Tp> __head;                                                                                     \
        __union<destructible_trait, _Index + 1, _Types...> __tail;                                                     \
                                                                                                                       \
        friend struct __access::__union;                                                                               \
      }

_LIBCPP_VARIANT_UNION(_Trait::_TriviallyAvailable,
                      _LIBCPP_HIDE_FROM_ABI _LIBCPP_CONSTEXPR_SINCE_CXX20 ~__union() = default);
_LIBCPP_VARIANT_UNION(
    _Trait::_Available, _LIBCPP_HIDE_FROM_ABI _LIBCPP_CONSTEXPR_SINCE_CXX20 ~__union() {} _LIBCPP_EAT_SEMICOLON);
_LIBCPP_VARIANT_UNION(_Trait::_Unavailable, _LIBCPP_HIDE_FROM_ABI _LIBCPP_CONSTEXPR_SINCE_CXX20 ~__union() = delete);

#    undef _LIBCPP_VARIANT_UNION

template <_Trait _DestructibleTrait, class... _Types>
class __base {
public:
  using __index_t _LIBCPP_NODEBUG = __variant_index_t<sizeof...(_Types)>;

  _LIBCPP_HIDE_FROM_ABI explicit constexpr __base(__valueless_t __tag) noexcept
      : __data(__tag), __index(__variant_npos<__index_t>) {}

  template <size_t _Ip, class... _Args>
  _LIBCPP_HIDE_FROM_ABI explicit constexpr __base(in_place_index_t<_Ip>, _Args&&... __args)
      : __data(in_place_index<_Ip>, std::forward<_Args>(__args)...), __index(_Ip) {}

  _LIBCPP_HIDE_FROM_ABI constexpr bool valueless_by_exception() const noexcept { return index() == variant_npos; }

  _LIBCPP_HIDE_FROM_ABI constexpr size_t index() const noexcept {
    return __index == __variant_npos<__index_t> ? variant_npos : __index;
  }

protected:

  _LIBCPP_HIDE_FROM_ABI static constexpr size_t __size() { return sizeof...(_Types); }

  __union<_DestructibleTrait, 0, _Types...> __data;
  __index_t __index;

  friend struct __access::__base;
  friend struct __visitation::__base;
};

template <class _Traits, _Trait = _Traits::__destructible_trait>
class __dtor;

#    define _LIBCPP_VARIANT_DESTRUCTOR(destructible_trait, destructor_definition, destroy)                             \
      template <class... _Types>                                                                                       \
      class __dtor<__traits<_Types...>, destructible_trait> : public __base<destructible_trait, _Types...> {           \
        using __base_type _LIBCPP_NODEBUG = __base<destructible_trait, _Types...>;                                     \
        using __index_t _LIBCPP_NODEBUG   = typename __base_type::__index_t;                                           \
                                                                                                                       \
      public:                                                                                                          \
        using __base_type::__base_type;                                                                                \
        using __base_type::operator=;                                                                                  \
        _LIBCPP_HIDE_FROM_ABI __dtor(const __dtor&)            = default;                                              \
        _LIBCPP_HIDE_FROM_ABI __dtor(__dtor&&)                 = default;                                              \
        _LIBCPP_HIDE_FROM_ABI __dtor& operator=(const __dtor&) = default;                                              \
        _LIBCPP_HIDE_FROM_ABI __dtor& operator=(__dtor&&)      = default;                                              \
        destructor_definition;                                                                                         \
                                                                                                                       \
      protected:                                                                                                       \
        destroy;                                                                                                       \
      }

_LIBCPP_VARIANT_DESTRUCTOR(
    _Trait::_TriviallyAvailable,
    _LIBCPP_HIDE_FROM_ABI _LIBCPP_CONSTEXPR_SINCE_CXX20 ~__dtor() = default,
    _LIBCPP_HIDE_FROM_ABI _LIBCPP_CONSTEXPR_SINCE_CXX20 void __destroy() noexcept {
      this->__index = __variant_npos<__index_t>;
    } _LIBCPP_EAT_SEMICOLON);

_LIBCPP_VARIANT_DESTRUCTOR(
    _Trait::_Available,
    _LIBCPP_HIDE_FROM_ABI _LIBCPP_CONSTEXPR_SINCE_CXX20 ~__dtor() { __destroy(); } _LIBCPP_EAT_SEMICOLON,
    _LIBCPP_HIDE_FROM_ABI _LIBCPP_CONSTEXPR_SINCE_CXX20 void __destroy() noexcept {
      if (!this->valueless_by_exception()) {
        __visitation::__base::__visit_alt(
            [](auto& __alt) noexcept {
              using __alt_type = __remove_cvref_t<decltype(__alt)>;
              __alt.~__alt_type();
            },
            *this);
      }
      this->__index = __variant_npos<__index_t>;
    } _LIBCPP_EAT_SEMICOLON);

_LIBCPP_VARIANT_DESTRUCTOR(_Trait::_Unavailable,
                           _LIBCPP_HIDE_FROM_ABI _LIBCPP_CONSTEXPR_SINCE_CXX20 ~__dtor()                 = delete,
                           _LIBCPP_HIDE_FROM_ABI _LIBCPP_CONSTEXPR_SINCE_CXX20 void __destroy() noexcept = delete);

#    undef _LIBCPP_VARIANT_DESTRUCTOR

template <class _Traits>
class __ctor : public __dtor<_Traits> {
  using __base_type _LIBCPP_NODEBUG = __dtor<_Traits>;

public:
  using __base_type::__base_type;
  using __base_type::operator=;

protected:
  template <class _Rhs>
  _LIBCPP_HIDE_FROM_ABI static _LIBCPP_CONSTEXPR_SINCE_CXX20 void __generic_construct(__ctor& __lhs, _Rhs&& __rhs);
};

template <class _Traits, _Trait = _Traits::__move_constructible_trait>
class __move_constructor;

#    define _LIBCPP_VARIANT_MOVE_CONSTRUCTOR(move_constructible_trait, move_constructor_definition)                    \
      template <class... _Types>                                                                                       \
      class __move_constructor<__traits<_Types...>, move_constructible_trait> : public __ctor<__traits<_Types...>> {   \
        using __base_type _LIBCPP_NODEBUG = __ctor<__traits<_Types...>>;                                               \
                                                                                                                       \
      public:                                                                                                          \
        using __base_type::__base_type;                                                                                \
        using __base_type::operator=;                                                                                  \
                                                                                                                       \
        _LIBCPP_HIDE_FROM_ABI __move_constructor(const __move_constructor&)            = default;                      \
        _LIBCPP_HIDE_FROM_ABI ~__move_constructor()                                    = default;                      \
        _LIBCPP_HIDE_FROM_ABI __move_constructor& operator=(const __move_constructor&) = default;                      \
        _LIBCPP_HIDE_FROM_ABI __move_constructor& operator=(__move_constructor&&)      = default;                      \
        move_constructor_definition;                                                                                   \
      }

_LIBCPP_VARIANT_MOVE_CONSTRUCTOR(
    _Trait::_TriviallyAvailable,
    _LIBCPP_HIDE_FROM_ABI _LIBCPP_CONSTEXPR_SINCE_CXX20 __move_constructor(__move_constructor&& __that) = default);

_LIBCPP_VARIANT_MOVE_CONSTRUCTOR(
    _Trait::_Available,
    _LIBCPP_HIDE_FROM_ABI _LIBCPP_CONSTEXPR_SINCE_CXX20 __move_constructor(__move_constructor&& __that) noexcept(
        __all<is_nothrow_move_constructible_v<_Types>...>::value) : __move_constructor(__valueless_t{}) {
      this->__generic_construct(*this, std::move(__that));
    } _LIBCPP_EAT_SEMICOLON);

_LIBCPP_VARIANT_MOVE_CONSTRUCTOR(
    _Trait::_Unavailable,
    _LIBCPP_HIDE_FROM_ABI _LIBCPP_CONSTEXPR_SINCE_CXX20 __move_constructor(__move_constructor&&) = delete);

#    undef _LIBCPP_VARIANT_MOVE_CONSTRUCTOR

template <class _Traits, _Trait = _Traits::__copy_constructible_trait>
class __copy_constructor;

#    define _LIBCPP_VARIANT_COPY_CONSTRUCTOR(copy_constructible_trait, copy_constructor_definition)                    \
      template <class... _Types>                                                                                       \
      class __copy_constructor<__traits<_Types...>, copy_constructible_trait>                                          \
          : public __move_constructor<__traits<_Types...>> {                                                           \
        using __base_type _LIBCPP_NODEBUG = __move_constructor<__traits<_Types...>>;                                   \
                                                                                                                       \
      public:                                                                                                          \
        using __base_type::__base_type;                                                                                \
        using __base_type::operator=;                                                                                  \
                                                                                                                       \
        _LIBCPP_HIDE_FROM_ABI __copy_constructor(__copy_constructor&&)                 = default;                      \
        _LIBCPP_HIDE_FROM_ABI ~__copy_constructor()                                    = default;                      \
        _LIBCPP_HIDE_FROM_ABI __copy_constructor& operator=(const __copy_constructor&) = default;                      \
        _LIBCPP_HIDE_FROM_ABI __copy_constructor& operator=(__copy_constructor&&)      = default;                      \
        copy_constructor_definition;                                                                                   \
      }

_LIBCPP_VARIANT_COPY_CONSTRUCTOR(
    _Trait::_TriviallyAvailable,
    _LIBCPP_HIDE_FROM_ABI _LIBCPP_CONSTEXPR_SINCE_CXX20 __copy_constructor(const __copy_constructor& __that) = default);

_LIBCPP_VARIANT_COPY_CONSTRUCTOR(
    _Trait::_Available,
    _LIBCPP_HIDE_FROM_ABI
    _LIBCPP_CONSTEXPR_SINCE_CXX20 __copy_constructor(const __copy_constructor& __that) : __copy_constructor(
        __valueless_t{}) { this->__generic_construct(*this, __that); } _LIBCPP_EAT_SEMICOLON);

_LIBCPP_VARIANT_COPY_CONSTRUCTOR(
    _Trait::_Unavailable,
    _LIBCPP_HIDE_FROM_ABI _LIBCPP_CONSTEXPR_SINCE_CXX20 __copy_constructor(const __copy_constructor&) = delete);

#    undef _LIBCPP_VARIANT_COPY_CONSTRUCTOR

template <class _Traits>
class __assignment : public __copy_constructor<_Traits> {
  using __base_type _LIBCPP_NODEBUG = __copy_constructor<_Traits>;

public:
  using __base_type::__base_type;
  using __base_type::operator=;

  template <size_t _Ip, class... _Args>
  _LIBCPP_HIDE_FROM_ABI _LIBCPP_CONSTEXPR_SINCE_CXX20 auto& __emplace(_Args&&... __args);
  
protected:
  template <size_t _Ip, class _Tp, class _Arg>
  _LIBCPP_HIDE_FROM_ABI _LIBCPP_CONSTEXPR_SINCE_CXX20 void __assign_alt(__alt<_Ip, _Tp>& __a, _Arg&& __arg);
  
  template <class _That>
  _LIBCPP_HIDE_FROM_ABI _LIBCPP_CONSTEXPR_SINCE_CXX20 void __generic_assign(_That&& __that);
  
};

template <class _Traits, _Trait = _Traits::__move_assignable_trait>
class __move_assignment;

#    define _LIBCPP_VARIANT_MOVE_ASSIGNMENT(move_assignable_trait, move_assignment_definition)                         \
      template <class... _Types>                                                                                       \
      class __move_assignment<__traits<_Types...>, move_assignable_trait> : public __assignment<__traits<_Types...>> { \
        using __base_type _LIBCPP_NODEBUG = __assignment<__traits<_Types...>>;                                         \
                                                                                                                       \
      public:                                                                                                          \
        using __base_type::__base_type;                                                                                \
        using __base_type::operator=;                                                                                  \
                                                                                                                       \
        _LIBCPP_HIDE_FROM_ABI __move_assignment(const __move_assignment&)            = default;                        \
        _LIBCPP_HIDE_FROM_ABI __move_assignment(__move_assignment&&)                 = default;                        \
        _LIBCPP_HIDE_FROM_ABI ~__move_assignment()                                   = default;                        \
        _LIBCPP_HIDE_FROM_ABI __move_assignment& operator=(const __move_assignment&) = default;                        \
        move_assignment_definition;                                                                                    \
      }

_LIBCPP_VARIANT_MOVE_ASSIGNMENT(_Trait::_TriviallyAvailable,
                                _LIBCPP_HIDE_FROM_ABI _LIBCPP_CONSTEXPR_SINCE_CXX20 __move_assignment& operator=(
                                    __move_assignment&& __that) = default);

_LIBCPP_VARIANT_MOVE_ASSIGNMENT(
    _Trait::_Available,
    _LIBCPP_HIDE_FROM_ABI _LIBCPP_CONSTEXPR_SINCE_CXX20 __move_assignment&
    operator=(__move_assignment&& __that) noexcept(
        __all<(is_nothrow_move_constructible_v<_Types> && is_nothrow_move_assignable_v<_Types>)...>::value) {
      this->__generic_assign(std::move(__that));
      return *this;
    } _LIBCPP_EAT_SEMICOLON);

_LIBCPP_VARIANT_MOVE_ASSIGNMENT(
    _Trait::_Unavailable,
    _LIBCPP_HIDE_FROM_ABI _LIBCPP_CONSTEXPR_SINCE_CXX20 __move_assignment& operator=(__move_assignment&&) = delete);

#    undef _LIBCPP_VARIANT_MOVE_ASSIGNMENT

template <class _Traits, _Trait = _Traits::__copy_assignable_trait>
class __copy_assignment;

#    define _LIBCPP_VARIANT_COPY_ASSIGNMENT(copy_assignable_trait, copy_assignment_definition)                         \
      template <class... _Types>                                                                                       \
      class __copy_assignment<__traits<_Types...>, copy_assignable_trait>                                              \
          : public __move_assignment<__traits<_Types...>> {                                                            \
        using __base_type _LIBCPP_NODEBUG = __move_assignment<__traits<_Types...>>;                                    \
                                                                                                                       \
      public:                                                                                                          \
        using __base_type::__base_type;                                                                                \
        using __base_type::operator=;                                                                                  \
                                                                                                                       \
        _LIBCPP_HIDE_FROM_ABI __copy_assignment(const __copy_assignment&)       = default;                             \
        _LIBCPP_HIDE_FROM_ABI __copy_assignment(__copy_assignment&&)            = default;                             \
        _LIBCPP_HIDE_FROM_ABI ~__copy_assignment()                              = default;                             \
        _LIBCPP_HIDE_FROM_ABI __copy_assignment& operator=(__copy_assignment&&) = default;                             \
        copy_assignment_definition;                                                                                    \
      }

_LIBCPP_VARIANT_COPY_ASSIGNMENT(_Trait::_TriviallyAvailable,
                                _LIBCPP_HIDE_FROM_ABI _LIBCPP_CONSTEXPR_SINCE_CXX20 __copy_assignment& operator=(
                                    const __copy_assignment& __that) = default);

_LIBCPP_VARIANT_COPY_ASSIGNMENT(
    _Trait::_Available,
    _LIBCPP_HIDE_FROM_ABI _LIBCPP_CONSTEXPR_SINCE_CXX20 __copy_assignment&
    operator=(const __copy_assignment& __that) {
      this->__generic_assign(__that);
      return *this;
    } _LIBCPP_EAT_SEMICOLON);

_LIBCPP_VARIANT_COPY_ASSIGNMENT(_Trait::_Unavailable,
                                _LIBCPP_HIDE_FROM_ABI _LIBCPP_CONSTEXPR_SINCE_CXX20 __copy_assignment& operator=(
                                    const __copy_assignment&) = delete);

#    undef _LIBCPP_VARIANT_COPY_ASSIGNMENT

template <class... _Types>
class __impl : public __copy_assignment<__traits<_Types...>> {
  using __base_type _LIBCPP_NODEBUG = __copy_assignment<__traits<_Types...>>;

public:
  using __base_type::__base_type; // get in_place_index_t constructor & friends
  _LIBCPP_HIDE_FROM_ABI __impl(__impl const&)            = default;
  _LIBCPP_HIDE_FROM_ABI __impl(__impl&&)                 = default;
  _LIBCPP_HIDE_FROM_ABI __impl& operator=(__impl const&) = default;
  _LIBCPP_HIDE_FROM_ABI __impl& operator=(__impl&&)      = default;

  template <size_t _Ip, class _Arg>
  _LIBCPP_HIDE_FROM_ABI _LIBCPP_CONSTEXPR_SINCE_CXX20 void __assign(_Arg&& __arg);

  inline _LIBCPP_HIDE_FROM_ABI _LIBCPP_CONSTEXPR_SINCE_CXX20 void __swap(__impl& __that);

private:
  constexpr inline _LIBCPP_HIDE_FROM_ABI bool __move_nothrow() const;
};

} // namespace __variant_detail

template <class... _Types>
class _LIBCPP_DECLSPEC_EMPTY_BASES _LIBCPP_NO_SPECIALIZATIONS variant
    : private __sfinae_ctor_base< __all<is_copy_constructible_v<_Types>...>::value,
                                  __all<is_move_constructible_v<_Types>...>::value>,
      private __sfinae_assign_base<
          __all<(is_copy_constructible_v<_Types> && is_copy_assignable_v<_Types>)...>::value,
          __all<(is_move_constructible_v<_Types> && is_move_assignable_v<_Types>)...>::value> {

public:

  template < size_t _Ip,
             class... _Args,
             enable_if_t<(_Ip < sizeof...(_Types)), int>         = 0,
             class _Tp                                           = variant_alternative_t<_Ip, variant<_Types...>>,
             enable_if_t<is_constructible_v<_Tp, _Args...>, int> = 0>
  _LIBCPP_HIDE_FROM_ABI _LIBCPP_CONSTEXPR_SINCE_CXX20 _Tp& emplace(_Args&&... __args) {
    // TODO: this fails now :(
    return __impl_.template __emplace<_Ip>(std::forward<_Args>(__args)...);
  }

private:
  __variant_detail::__impl<_Types...> __impl_;

  friend struct __variant_detail::__access::__variant;
  friend struct __variant_detail::__visitation::__variant;
};

#  endif // _LIBCPP_STD_VER >= 17

_LIBCPP_END_NAMESPACE_STD

_LIBCPP_POP_MACROS

#  if !defined(_LIBCPP_REMOVE_TRANSITIVE_INCLUDES) && _LIBCPP_STD_VER <= 20
#    include <cstddef>
#    include <exception>
#    include <tuple>
#    include <type_traits>
#    include <typeinfo>
#    include <utility>
#  endif
