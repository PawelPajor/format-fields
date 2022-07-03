//
// Created by Pawel Pajor on 08/07/2022.
//

#include <catch2/catch_all.hpp>
#include <field.hpp>

using namespace ff;

using tested_t = field<16>;
using bitset_t = std::bitset<16>;
using enum_names_t = typename field<16>::enum_names_t;
using presenter_t = field_presenter<16>;
using fields_t = presenter_t::fields_t;

namespace {

constexpr uint64_t tested_value = 0x3183; // random value

const auto no_enum_names = enum_names_t();

const auto enum_names = enum_names_t{
    {"key", {
        {0, "Zero"},
        {1, "One"},
        {2, "Two"}
    }}
};

const auto fields_example = fields_t{
    {0, "T0SZ", 6},
    {7, "EPD0"},
    {8, "IRGN0", 2, "key"},
    {10, "ORGN0", 2, "key"},
    {12, "SH0", 2, "key"},
    {14, "TG0", 2, "key"}
};
}

void verify_state(tested_t &&tested,
                  const std::string &name,
                  size_t offset,
                  size_t size,
                  const std::string &enums_key) {
  REQUIRE(tested.name() == name);
  REQUIRE(tested.offset() == offset);
  REQUIRE(tested.size() == size);
  REQUIRE(tested.enums_key() == enums_key);
}

void verify_formatting(uint64_t bitset_value,
                       const enum_names_t &en,
                       presentation_style style,
                       size_t size = 1,
                       const std::string &expected_out = {},
                       const std::string &expected_err = {}) {
  const auto tested = tested_t(1, "field", size, "key");
  const auto value = bitset_t(bitset_value);
  auto out = std::stringstream();
  auto err = std::stringstream();
  tested.format(out, value, en, style, err);
  REQUIRE(out.str() == expected_out);
  REQUIRE(err.str() == expected_err);
}

TEST_CASE("Field", "[lib]") {

  SECTION("Creation") {

    SECTION("Defaults") {
      verify_state(tested_t(1, "field"), "field", 1, 1, {});
    }

    SECTION("All arguments") {
      verify_state(tested_t(1, "field", 2, "key"), "field", 1, 2, "key");
    }
  }

  SECTION("Formatting") {

    SECTION("Horizontal") {

      SECTION("Clear Flag") {
        verify_formatting(0, no_enum_names, presentation_style::horizontal);
      }

      SECTION("Set Flag") {
        verify_formatting(0b010, no_enum_names, presentation_style::horizontal, 1, "field");
      }

      SECTION("Hit enum") {
        verify_formatting(0b100, enum_names, presentation_style::horizontal, 2, "field:Two");
      }

      SECTION("Missed enum") {
        verify_formatting(0b110,
                          enum_names,
                          presentation_style::horizontal,
                          2,
                          "field:11",
                          "Name for value 3 not found in enum 'key'.\n");
      }
    }

    SECTION("Vertical") {

      SECTION("Clear Flag") {
        verify_formatting(0, no_enum_names, presentation_style::vertical, 1, "field\t0");
      }

      SECTION("Set Flag") {
        verify_formatting(0b010, no_enum_names, presentation_style::vertical, 1, "field\t1");
      }

      SECTION("Hit enum") {
        verify_formatting(0b100, enum_names, presentation_style::vertical, 2, "field\tTwo");
      }

      SECTION("Missed enum") {
        verify_formatting(0b110,
                          enum_names,
                          presentation_style::vertical,
                          2,
                          "field\t0b11\t3\t0x3",
                          "Name for value 3 not found in enum 'key'.\n");
      }
    }
  }
}

TEST_CASE("Top Level", "[lib]") {

  SECTION("Horizontal") {
    auto out = std::stringstream();
    auto err = std::stringstream();
    const auto presenter = presenter_t(bitset_t(tested_value), fields_example, enum_names, presentation_style::horizontal, err);
    out << presenter;
    REQUIRE(out.str() == "T0SZ:000011|EPD0|IRGN0:One|ORGN0:Zero|SH0:11|TG0:Zero");
    REQUIRE(err.str() == "Name for value 3 not found in enum 'key'.\n");
  }

  SECTION("Vertical") {
    auto out = std::stringstream();
    auto err = std::stringstream();
    const auto presenter = presenter_t(bitset_t(tested_value), fields_example, enum_names, presentation_style::vertical, err);
    out << presenter;
    REQUIRE(out.str() == "T0SZ\t0b000011\t3\t0x3\n"
                         "EPD0\t1\n"
                         "IRGN0\tOne\n"
                         "ORGN0\tZero\n"
                         "SH0\t0b11\t3\t0x3\n"
                         "TG0\tZero\n");
    REQUIRE(err.str() == "Name for value 3 not found in enum 'key'.\n");
  }
}
