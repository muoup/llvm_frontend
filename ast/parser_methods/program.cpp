#include "program.h"

#include "../util_methods.h"
#include "../../lexer/lex.h"
#include "statement.h"

using namespace ast;

std::vector<nodes::initialization> pm::parse_method_params(lex_cptr &ptr, const lex_cptr end) {
    return parse_split(ptr, end, ",", parse_initialization);
}

std::vector<nodes::expression> pm::parse_call_params(lex_cptr &ptr, const lex_cptr end) {
    return parse_split(ptr, end, ",", parse_expression);
}

nodes::function pm::parse_method(lex_cptr &ptr, const lex_cptr end) {
    const auto ret_type = parse_value_type(ptr, end);
    const auto function_name = assert_token_type(ptr, lex::lex_type::IDENTIFIER)->span;

    nodes::function function{
        .return_type = ret_type,
        .function_name = function_name,
        .param_types = parse_between(ptr, "(", parse_method_params),
        .body = parse_between(ptr, "{", parse_body)
    };

    auto &code_expressions = function.body.statements;
    if (code_expressions.empty() || !std::holds_alternative<nodes::return_op>(code_expressions.back().value))
        code_expressions.emplace_back(nodes::return_op{
            function_name == "main" ?
                std::make_optional<nodes::expression>(nodes::literal{ 0 }) :
                std::nullopt
        });

    return function;
}

nodes::code_block pm::parse_body(lex_cptr &ptr, lex_cptr end) {
    nodes::code_block body;

    while (ptr != end)
        body.statements.push_back(parse_statement(ptr, end));

    return body;
}

nodes::value_type pm::parse_value_type(lex_cptr &ptr, const lex_cptr) {
    const auto is_const = test_token_val(ptr, "mut").has_value();
    const auto type = assert_token(ptr, is_variable_identifier)->span;
    const auto is_pointer = test_token_val(ptr, "*").has_value();

    const auto intrin = nodes::get_intrinsic_type(type);

    if (!intrin) {
        return nodes::value_type {
            type,
            is_pointer,
            is_const
        };
    } else {
        return nodes::value_type {
            *intrin,
            is_pointer,
            is_const
        };
    }
}
