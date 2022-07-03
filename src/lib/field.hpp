//
// Created by Pawel Pajor on 03/07/2022.
//

#pragma once

#include <bitset>
#include <iostream>
#include <ostream>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

namespace ff {

enum class presentation_style { horizontal, vertical };

template<size_t N>
class field {
public:

  using enums_t = std::unordered_map<uint64_t, std::string>;
  using enum_names_t = std::unordered_map<std::string, enums_t>;

  field(size_t offset, std::string &&name, size_t size = 1, std::string &&enums_key = {})
      : offset_(offset), size_(size), name_(std::move(name)), enums_key_(std::move(enums_key)) {
  }

  size_t offset() const { return offset_; }
  size_t size() const { return size_; }
  const std::string &name() const { return name_; }
  const std::string &enums_key() const { return enums_key_; }

  bool format(std::ostream &os, const std::bitset<N> &bitset, const enum_names_t &enum_names,
              presentation_style style, std::ostream &err) const {
    switch (style) {
    case presentation_style::horizontal:return format_horizontally(os, bitset, enum_names, err);
    case presentation_style::vertical:return format_vertically(os, bitset, enum_names, err);
    }
  }

private:

  bool format_horizontally(std::ostream &os,
                           const std::bitset<N> &bitset,
                           const enum_names_t &enum_names,
                           std::ostream &err) const {
    return size_ == 1
           ? format_bit_horizontally(os, bitset)
           : format_bits_horizontally(os, bitset, enum_names, err);
  }

  bool format_bit_horizontally(std::ostream &os, const std::bitset<N> &bitset) const {
    const auto bit = bitset[offset_];
    if (bit) {
      os << name_;
      return true;
    } else {
      return false;
    }
  }

  bool format_bits_horizontally(std::ostream &os, const std::bitset<N> &bitset,
                                const enum_names_t &enum_names, std::ostream &err) const {
    const auto value = fetch_value(bitset);
    os << name_ << ':';
    if (const auto opt = fetch_enum(value, enum_names, err); opt) {
      os << opt.value();
    } else {
      format_binary(os, bitset);
    }
    return true;
  }

  bool format_vertically(std::ostream &os,
                         const std::bitset<N> &bitset,
                         const enum_names_t &enum_names,
                         std::ostream &err) const {
    os << name_ << '\t';
    if (size_ == 1) {
      os << bitset[offset_];
    } else {
      const auto value = fetch_value(bitset);
      if (const auto opt = fetch_enum(value, enum_names, err); opt) {
        os << opt.value();
      } else {
        os << "0b";
        format_binary(os, bitset);
        os << '\t' << std::dec << value;
        os << '\t' << "0x" << std::hex << value;
      }
    }
    return true;
  }

  std::ostream &format_binary(std::ostream &os, const std::bitset<N> &bitset) const {
    // youngest to oldest = right to left
    for (size_t i = offset_ + size_; i > offset_;) {
      os << bitset[--i];
    }
    return os;
  }

  uint64_t fetch_value(const std::bitset<N> &bitset) const {
    uint64_t value = 0;
    size_t i = offset_ + size_ - 1;
    while (true) {
      value |= bitset[i];
      if (i > offset_) {
        value <<= 1;
        i--;
      } else {
        break;
      }
    }
    return value;
  }

  std::optional<std::string> fetch_enum(uint64_t value, const enum_names_t &enum_names, std::ostream &err) const {
    if (!enums_key_.empty()) {
      if (const auto enums_it = enum_names.find(enums_key_); enums_it != enum_names.end()) {
        const auto &enums = enums_it->second;
        if (const auto found = enums.find(value); found != enums.end()) {
          return found->second;
        } else {
          err << "Name for value " << value << " not found in enum '" << enums_key_ << "'." << std::endl;
        }
      }
    }
    return std::nullopt;
  }

private:
  std::size_t offset_;
  std::size_t size_;
  std::string name_;
  std::string enums_key_;
};

template<size_t N>
class field_presenter {
public:

  using value_t = std::bitset<N>;
  using fields_t = std::vector<field<N>>;
  using enum_names_t = typename field<N>::enum_names_t;

  field_presenter(value_t &&value, const fields_t &fields, const enum_names_t &enum_names,
                  presentation_style style = presentation_style::horizontal, std::ostream &err = std::cerr)
      : value_(std::move(value)), fields_(fields), enum_names_(enum_names), style_(style),
        err_(err) {
    // validate fields against value size
  }

  friend std::ostream &operator<<(std::ostream &os, const field_presenter &obj) {
    switch (obj.style_) {
    case presentation_style::horizontal:return obj.format_horizontally(os);
    case presentation_style::vertical:return obj.format_vertically(os);
    }
  }

private:

  std::ostream &format_horizontally(std::ostream &os) const {
    auto it = fields_.begin();
    if (it == fields_.end()) {
      return os;
    }

    bool presented = it->format(os, value_, enum_names_, style_, err_);

    for (++it; it != fields_.end(); ++it) {
      if (presented) {
        os << '|';
      }
      presented = it->format(os, value_, enum_names_, style_, err_);
    }
    return os;
  }

  std::ostream &format_vertically(std::ostream &os) const {
    for (const auto &field: fields_) {
      if (field.format(os, value_, enum_names_, style_, err_)) {
        os << std::endl;
      }
    }
    return os;
  }

  value_t value_;
  const fields_t &fields_;
  const enum_names_t &enum_names_;
  presentation_style style_;
  std::ostream &err_;
};
}
