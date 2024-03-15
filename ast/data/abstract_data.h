#pragma once
#include <optional>
#include <string_view>
#include <variant>

namespace ast::nodes {
    enum class intrinsic_types {
        i8, i16, i32, i64,
        u8, u16, u32, u64,
        f32, f64,

        char_, bool_, void_,

        infer_type
    };

    enum class bin_op_type {
        invalid,

        add, sub, mul, div, mod, pow,
        and_, or_, xor_, shl, shr,
        l_and, l_or, l_not, l_xor,

        eq, neq, lt, gt, lte, gte,
    };

    enum class un_op_type {
        invalid,

        dereference, address_of, bit_not, l_not, negate
    };

    enum var_type_category {
        INTRINSIC,
        NON_INTRINSIC
    };

    struct value_type {
        std::variant<intrinsic_types, std::string_view> type;
        bool is_const, is_pointer;
    };

    std::optional<intrinsic_types> get_intrinsic_type(std::string_view type);
}
