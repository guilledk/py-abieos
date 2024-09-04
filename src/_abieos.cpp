#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "abieos.h"
#include "abieos.hpp"

#include <eosio/abi.hpp>
#include <eosio/name.hpp>

struct abieos_context_s {
    const char* last_error;
    std::string last_error_buffer;
    std::string result_str;
    std::vector<char> result_bin;

    std::map<abieos::name, abieos::abi> contracts;
};

namespace py = pybind11;


void init_abi_classes(py::module_& m) {
    // Enum for abi_error
    py::enum_<eosio::abi_error>(m, "AbiError")
        .value("no_error", eosio::abi_error::no_error)
        .value("recursion_limit_reached", eosio::abi_error::recursion_limit_reached)
        .value("invalid_nesting", eosio::abi_error::invalid_nesting)
        .value("unknown_type", eosio::abi_error::unknown_type)
        .value("missing_name", eosio::abi_error::missing_name)
        .value("redefined_type", eosio::abi_error::redefined_type)
        .value("base_not_a_struct", eosio::abi_error::base_not_a_struct)
        .value("extension_typedef", eosio::abi_error::extension_typedef)
        .value("bad_abi", eosio::abi_error::bad_abi);

    // Class for type_def
    py::class_<eosio::type_def>(m, "TypeDef")
        .def(py::init<>())
        .def_readwrite("new_type_name", &eosio::type_def::new_type_name)
        .def_readwrite("type", &eosio::type_def::type);

    // Class for field_def
    py::class_<eosio::field_def>(m, "FieldDef")
        .def(py::init<>())
        .def_readwrite("name", &eosio::field_def::name)
        .def_readwrite("type", &eosio::field_def::type);

    // Class for struct_def
    py::class_<eosio::struct_def>(m, "StructDef")
        .def(py::init<>())
        .def_readwrite("name", &eosio::struct_def::name)
        .def_readwrite("base", &eosio::struct_def::base)
        .def_readwrite("fields", &eosio::struct_def::fields);

    // Class for action_def
    py::class_<eosio::action_def>(m, "ActionDef")
        .def(py::init<>())
        .def_readwrite("name", &eosio::action_def::name)
        .def_readwrite("type", &eosio::action_def::type)
        .def_readwrite("ricardian_contract", &eosio::action_def::ricardian_contract);

    // Class for table_def
    py::class_<eosio::table_def>(m, "TableDef")
        .def(py::init<>())
        .def_readwrite("name", &eosio::table_def::name)
        .def_readwrite("index_type", &eosio::table_def::index_type)
        .def_readwrite("key_names", &eosio::table_def::key_names)
        .def_readwrite("key_types", &eosio::table_def::key_types)
        .def_readwrite("type", &eosio::table_def::type);

    // Class for clause_pair
    py::class_<eosio::clause_pair>(m, "ClausePair")
        .def(py::init<>())
        .def_readwrite("id", &eosio::clause_pair::id)
        .def_readwrite("body", &eosio::clause_pair::body);

    // Class for error_message
    py::class_<eosio::error_message>(m, "ErrorMessage")
        .def(py::init<>())
        .def_readwrite("error_code", &eosio::error_message::error_code)
        .def_readwrite("error_msg", &eosio::error_message::error_msg);

    // Class for variant_def
    py::class_<eosio::variant_def>(m, "VariantDef")
        .def(py::init<>())
        .def_readwrite("name", &eosio::variant_def::name)
        .def_readwrite("types", &eosio::variant_def::types);

    // Class for action_result_def
    py::class_<eosio::action_result_def>(m, "ActionResultDef")
        .def(py::init<>())
        .def_readwrite("name", &eosio::action_result_def::name)
        .def_readwrite("result_type", &eosio::action_result_def::result_type);

    // Class for abi_def
    py::class_<eosio::abi_def>(m, "AbiDef")
        .def(py::init<>())
        .def_readwrite("version", &eosio::abi_def::version)
        .def_readwrite("types", &eosio::abi_def::types)
        .def_readwrite("structs", &eosio::abi_def::structs)
        .def_readwrite("actions", &eosio::abi_def::actions)
        .def_readwrite("tables", &eosio::abi_def::tables)
        .def_readwrite("ricardian_clauses", &eosio::abi_def::ricardian_clauses)
        .def_readwrite("error_messages", &eosio::abi_def::error_messages)
        .def_readwrite("abi_extensions", &eosio::abi_def::abi_extensions)
        .def_readwrite("variants", &eosio::abi_def::variants)
        .def_readwrite("action_results", &eosio::abi_def::action_results);

    py::class_<eosio::abi_type>(m, "AbiType")
        .def(py::init<std::string, eosio::abi_type::builtin, const eosio::abi_serializer*>(),
             py::arg("name"), py::arg("builtin"), py::arg("ser") = nullptr)
        .def(py::init<std::string, const eosio::abi_type::alias_def*, const eosio::abi_serializer*>(),
             py::arg("name"), py::arg("alias_def"), py::arg("ser") = nullptr)
        .def(py::init<std::string, const eosio::struct_def*, const eosio::abi_serializer*>(),
             py::arg("name"), py::arg("struct_def"), py::arg("ser") = nullptr)
        .def(py::init<std::string, const eosio::variant_def*, const eosio::abi_serializer*>(),
             py::arg("name"), py::arg("variant_def"), py::arg("ser") = nullptr)
        .def("optional_of", &eosio::abi_type::optional_of)
        .def("extension_of", &eosio::abi_type::extension_of)
        .def("array_of", &eosio::abi_type::array_of)
        .def("as_struct", &eosio::abi_type::as_struct)
        .def("as_variant", &eosio::abi_type::as_variant)
        .def("bin_to_json", &eosio::abi_type::bin_to_json)
        .def("json_to_bin", &eosio::abi_type::json_to_bin)
        .def("json_to_bin_reorderable", &eosio::abi_type::json_to_bin_reorderable);

    // Class for abi
    py::class_<eosio::abi>(m, "Abi")
        .def(py::init<>())
        .def_readwrite("action_types", &eosio::abi::action_types)
        .def_readwrite("table_types", &eosio::abi::table_types)
        .def_readwrite("abi_types", &eosio::abi::abi_types)
        .def_readwrite("action_result_types", &eosio::abi::action_result_types)
        .def("get_type", &eosio::abi::get_type)
        .def("add_type", &eosio::abi::add_type<std::string>);  // Example for adding a string type, you can extend it for other types.
}


PYBIND11_MODULE(_abieos, m) {
    m.doc() = "Python bindings for the abieos C library";

    // init_abi_classes(m);

    py::class_<eosio::name>(m, "Name")
        .def(py::init<>())
        .def(py::init<uint64_t>())
        .def(py::init<std::string_view>())
        .def("suffix", &eosio::name::suffix)
        .def("prefix", &eosio::name::prefix)
        .def("__len__", &eosio::name::length)
        .def("__str__", &eosio::name::to_string)
        .def("__bool__", [](const eosio::name& n) { return bool(n); })
        .def("__int__", [](const eosio::name& n) { return static_cast<uint64_t>(n.value); })
        .def("__hash__", [](const eosio::name& n) {
            return std::hash<uint64_t>{}(n.value);
        })
        .def("__eq__", [](const eosio::name& a, const eosio::name& b) {
            return a.value == b.value;
        })
        .def_static("char_to_value", &eosio::name::char_to_value)
        .def_readwrite("value", &eosio::name::value);

    py::class_<abieos_context_s>(m, "AbieosContext")
        .def(py::init<>())  // Constructor
        .def_readwrite("last_error", &abieos_context_s::last_error)
        .def_readwrite("last_error_buffer", &abieos_context_s::last_error_buffer)
        .def_readwrite("result_str", &abieos_context_s::result_str)
        .def_property_readonly("result_bin", [](const abieos_context_s& ctx) {
            return py::bytes(ctx.result_bin.data(), ctx.result_bin.size());
        });

    // Context management
    m.def("abieos_create", &abieos_create, py::return_value_policy::reference, "Create a new abieos context");
    m.def("abieos_destroy", &abieos_destroy, py::arg("context"), "Destroy the abieos context");
    m.def("abieos_get_error", &abieos_get_error, py::arg("context"), "Get last error from the context");

    // Binary-related functions
    m.def("abieos_get_bin_size", &abieos_get_bin_size, py::arg("context"), "Get generated binary size");
    m.def("abieos_get_bin_data", &abieos_get_bin_data, py::arg("context"), "Get generated binary data");
    m.def("abieos_get_bin_hex", &abieos_get_bin_hex, py::arg("context"), "Convert generated binary to hex");

    // Name conversion functions
    m.def("abieos_string_to_name", &abieos_string_to_name, py::arg("context"), py::arg("str"), "Convert string to name");
    m.def("abieos_name_to_string", &abieos_name_to_string, py::arg("context"), py::arg("name"), "Convert name to string");

    // ABI-related functions
    m.def("abieos_set_abi", &abieos_set_abi, py::arg("context"), py::arg("contract"), py::arg("abi"), "Set ABI in JSON format");
    m.def("abieos_set_abi_bin", &abieos_set_abi_bin, py::arg("context"), py::arg("contract"), py::arg("data"), py::arg("size"), "Set ABI in binary format");
    m.def("abieos_set_abi_hex", &abieos_set_abi_hex, py::arg("context"), py::arg("contract"), py::arg("hex"), "Set ABI in hex format");

    // Type lookup functions
    m.def("abieos_get_type_for_action", &abieos_get_type_for_action, py::arg("context"), py::arg("contract"), py::arg("action"), "Get type name for an action");
    m.def("abieos_get_type_for_table", &abieos_get_type_for_table, py::arg("context"), py::arg("contract"), py::arg("table"), "Get type name for a table");
    m.def("abieos_get_type_for_action_result", &abieos_get_type_for_action_result, py::arg("context"), py::arg("contract"), py::arg("action_result"), "Get type name for an action result");

    // JSON <-> Binary conversion
    m.def("abieos_json_to_bin", &abieos_json_to_bin, py::arg("context"), py::arg("contract"), py::arg("type"), py::arg("json"), "Convert JSON to binary");
    m.def("abieos_json_to_bin_reorderable", &abieos_json_to_bin_reorderable, py::arg("context"), py::arg("contract"), py::arg("type"), py::arg("json"), "Convert JSON to binary (reorderable)");
    m.def("abieos_bin_to_json", &abieos_bin_to_json, py::arg("context"), py::arg("contract"), py::arg("type"), py::arg("data"), py::arg("size"), "Convert binary to JSON");
    m.def("abieos_hex_to_json", &abieos_hex_to_json, py::arg("context"), py::arg("contract"), py::arg("type"), py::arg("hex"), "Convert hex to JSON");

    // ABI JSON <-> Binary conversion
    m.def("abieos_abi_json_to_bin", &abieos_abi_json_to_bin, py::arg("context"), py::arg("json"), "Convert ABI JSON to binary");
    m.def("abieos_abi_bin_to_json", &abieos_abi_bin_to_json, py::arg("context"), py::arg("abi_bin_data"), py::arg("abi_bin_data_size"), "Convert ABI binary to JSON");

    // Contract deletion
    m.def("abieos_delete_contract", &abieos_delete_contract, py::arg("context"), py::arg("contract"), "Delete a contract from the context");
}

