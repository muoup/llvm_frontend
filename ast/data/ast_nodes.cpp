#include "ast_nodes.h"
#include "../parser_methods/operator.h"
#include "data_maps.h"
#include <iostream>

using namespace ast::nodes;

void print_depth(const size_t depth) {
    for (size_t i = 0; i < depth; ++i)
        std::cout << "  ";
}

void root::print(const size_t depth) const {
    print_depth(depth);

    std::cout << "Root\n";
    for (const auto& stmt : program_level_statements)
        stmt->print(depth + 1);
}

void scope_block::print(const size_t depth) const {
    print_depth(depth);

    std::cout << "Code block\n";
    for (const auto& stmt : statements)
        stmt->print(depth + 1);
}

void function::print(const size_t depth) const {
    print_depth(depth);

    std::cout << "Function " << fn_name << "\n";

    print_depth(depth + 1);
    std::cout << "Return Type\n";
    return_type.print(depth + 2);

    if (!param_types.empty()) {
        print_depth(depth);
        std::cout << "Parameters\n";
    }

    for (const auto& param : param_types)
        param.print(depth + 1);

    body.print(depth + 1);
}

void return_op::print(const size_t depth) const {
    print_depth(depth);

    std::cout << "Return\n";

    if (val)
        val->print(depth + 1);
}

void var_ref::print(const size_t depth) const {
    print_depth(depth);

    std::cout << "Variable Reference: " << name << "\n";

    if (type)
        type->print(depth + 1);
    else {
        print_depth(depth + 1);
        std::cout << "Type Not Found\n";
    }
}

void type_instance::print(const size_t depth) const {
    print_depth(depth);

    std::cout << "Initialization: " << var_name << '\n';
    type.print(depth + 1);
}

void value_type::print(size_t depth) const {
    print_depth(depth);

    std::cout << "Type: ";

    if (!std::holds_alternative<intrinsic_types>(type)) {
        std::cout << std::get<std::string_view>(type) << "\n";
        return;
    }

    auto intrinsic = std::get<intrinsic_types>(type);

    for (const auto &[key, val] : pm::intrin_map) {
        if (val == intrinsic) {
            std::cout << "Intrinsic Type (" << key << ")\n";
            break;
        }
    }
}

void initialization::print(const size_t depth) const {
    print_depth(depth);

    std::cout << "Variable Initialization\n";
    variable.print(depth + 1);
}

void loop::print(const size_t depth) const {
    print_depth(depth);

    std::cout << "Loop; pre-eval=" << (pre_eval ? "true" : "false") << "\n";
    condition->print(depth + 1);
    body.print(depth + 1);
}

void if_statement::print(const size_t depth) const {
    print_depth(depth);
    std::cout << "If Statement\n";
    condition->print(depth + 1);
    body.print(depth + 1);

    if (!else_body)
        return;

    print_depth(depth);
    std::cout << "Else\n";

    else_body->print(depth + 1);
}

void method_call::print(const size_t depth) const {
    print_depth(depth);

    std::cout << "Method call " << method_name << "\n";
    for (const auto& arg : arguments)
        arg->print(depth + 1);
}

void bin_op::print(const size_t depth) const {
    print_depth(depth);

    std::cout << "Binary " << pm::from_binop(type) << "\n";
    left->print(depth + 1);
    right->print(depth + 1);
}

void un_op::print(const size_t depth) const {
    print_depth(depth);

    std::cout << "Unary " << pm::from_unop(type) << "\n";
    value->print(depth + 1);
}

void literal::print(const size_t depth) const {
    print_depth(depth);

    switch (value.index()) {
        case 0:
            std::cout << "Unsigned Int Literal " << std::get<unsigned int>(value) << "\n";
            break;
        case 1:
            std::cout << "Int Literal " << std::get<int>(value) << "\n";
            break;
        case 2:
            std::cout << "Double Literal " << std::get<double>(value) << "\n";
            break;
        case 3:
            std::cout << "Char Literal " << std::get<char>(value) << "\n";
            break;
        case 4:
            std::cout << "String Literal " << std::get<std::string_view>(value) << "\n";
            break;
        default:
            throw std::runtime_error("Invalid literal type");
    }
}

void assignment::print(const size_t depth) const {
    print_depth(depth);

    std::cout << "Assignment\n";
    lhs->print(depth + 1);
    rhs->print(depth + 1);
}

void for_loop::print(size_t depth) const {
    print_depth(depth);
    std::cout << "For loop\n";

    print_depth(depth + 1);
    std::cout << "Init\n";
    init->print(depth + 2);

    print_depth(depth + 1);
    std::cout << "Condition\n";
    condition->print(depth + 2);

    print_depth(depth + 1);
    std::cout << "Update\n";
    update->print(depth + 2);

    print_depth(depth + 1);
    std::cout << "Body\n";
    body.print(depth + 2);
}

void struct_declaration::print(size_t depth) const {
    print_depth(depth);
    std::cout << "Struct " << name << "\n";

    for (const auto& field : fields)
        field.print(depth + 1);
}

void match::print(size_t depth) const {
    print_depth(depth);
    std::cout << "Match\n";

    print_depth(depth + 1);
    std::cout << "Matching:\n";
    match_expr->print(depth + 2);

    for (const auto& case_ : cases) {
        print_depth(depth + 1);
        std::cout << "Case\n";

        print_depth(depth + 2);
        std::cout << "Pattern\n";
        case_.match_expr->print(depth + 3);

        print_depth(depth + 2);
        std::cout << "Body\n";
        case_.body.print(depth + 3);
    }

    if (default_case) {
        print_depth(depth + 1);
        std::cout << "Default\n";
        default_case->print(depth + 2);
    }
}