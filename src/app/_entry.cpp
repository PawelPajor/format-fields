#include <iostream>

#include "field.hpp"

using namespace ff;

//using bitset64 = std::bitset<64>;
using presenter64_t = field_presenter<64>;
using fields_t = presenter64_t::fields_t;

fields_t make_fields() {
  return {
      {0, "T0SZ", 6},
      {7, "EPD0"},
      {8, "IRGN0", 2, "zero-two"},
      {10, "ORGN0", 2, "zero-two"},
      {12, "SH0", 2, "zero-two"},
      {14, "TG0", 2, "zero-two"},
      {16, "T1SZ", 6},
      {22, "A1"},
      {23, "EPD1"},
      {24, "IRGN1", 2, "zero-two"},
      {26, "ORGN1", 2, "random"},
      {28, "SH1", 2},
      {30, "TG1", 2},
      {32, "IPS", 3},
      {36, "AS"},
      {37, "TBI0"},
      {38, "TBI1"},
      {39, "HA"},
      {40, "HD"},
      {41, "HPD0"},
      {42, "HPD1"},
      {43, "HWU059"},
      {44, "HWU060"},
      {45, "HWU061"},
      {46, "HWU062"},
      {47, "HWU159"},
      {48, "HWU160"},
      {49, "HWU161"},
      {50, "HWU162"},
      {51, "TBID0"},
      {52, "TBID1"},
      {53, "NFD0"},
      {54, "NFD1"},
      {55, "E0PD0"},
      {56, "E0PD1"},
      {57, "TCMA0"},
      {58, "TCMA1"},
      {59, "DS"}
  };
}

field<64>::enum_names_t make_enums() {
  return {
      {"zero-two", {
          {0, "Zero"},
          {1, "One"},
          {2, "Two"}
      }}
  };
}

int main() {
  constexpr uint64_t value = 0xCCAD73CE77203183; // random value

  std::cout << "---" << std::endl;
  std::cout << presenter64_t(
      std::bitset<64>{value},
      make_fields(),
      make_enums(),
      presentation_style::horizontal) << std::endl;

  std::cout << "---" << std::endl;
  std::cout << presenter64_t(
      std::bitset<64>{value},
      make_fields(),
      make_enums(),
      presentation_style::vertical) << std::endl;

  return 0;
}
