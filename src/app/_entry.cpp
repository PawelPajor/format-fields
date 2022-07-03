#include <cassert>
#include <cstdint>
#include <cstdio>
#include <tuple>

using natural = uint64_t;

constexpr natural invalid = static_cast<natural>(-1);

template<typename T>
constexpr T mask_at(size_t position) {
  return T(1) << position;
}

template<typename T>
constexpr bool get_bit(T value, size_t position) {
  return (value & mask_at<T>(position)) != 0;
}

template<typename T>
constexpr T set_bit(T value, size_t position) {
  return value | mask_at<T>(position);
}

template<typename T>
constexpr T clear_bit(T value, size_t position) {
  return value & ~mask_at<T>(position);
}

template<typename T>
constexpr T make_mask(size_t size, size_t offset) {
  auto result = T(1) << size;
  result--;
  result <<= offset;
  return result;
}

template<typename T>
constexpr T get_field_value(T value, size_t size, size_t offset) {
  auto result = value;
  result &= make_mask<T>(size, offset);
  result >>= offset;
  return result;
}

template<typename T>
constexpr T set_field_value(T value, T field_value, size_t size, size_t offset) {
  const T mask = make_mask<T>(size, offset);
  value &= ~mask;
  field_value <<= offset;
  return value | (mask & field_value);
}

struct field {
  size_t size = 1;
  size_t offset = invalid;
  size_t name = invalid;
};

struct id_txt {
  size_t id;
  const char *txt;

  constexpr static id_txt null() {
    return {invalid, nullptr};
  }

  constexpr bool operator==(const id_txt &rhs) const {
    return std::tie(id, txt) == std::tie(rhs.id, rhs.txt);
  }

  constexpr bool operator!=(const id_txt &rhs) const {
    return !(rhs == *this);
  }
};

//constexpr
const char *bit_names_example[] = {
    "A"
};

//constexpr
id_txt bitfields_names_example[] = {
    {0, "zero"},
    {1, "one"},
    {2, "two"},
    {3, "three"},
    id_txt::null()
};

field fields_example[] = {
    {1, 0, 0},
    {1, 1, invalid},
    {2, 2, 0}
};

__attribute__ ((noinline))
//constexpr
void print_bit_name(natural value, const field &f, const char *bit_names[]) {
  const bool is_set = get_bit(value, f.offset);
  if (is_set) {
    if (f.name != invalid) {
      printf("%s", bit_names[f.name]);
    } else {
      putchar('1');
    }
  }
}

__attribute__ ((noinline))
//constexpr
void print_bitfield_name(natural value, const field &f, const id_txt bitfields_names[]) {
  const natural fv = get_field_value(value, f.size, f.offset);
  if (f.name != invalid) {
    for (const id_txt *bfn = &bitfields_names[f.name]; *bfn != id_txt::null(); bfn++) {
      if (bfn->id == fv) {
        printf("%s", bfn->txt);
        return;
      }
    }
  }
  printf("%llx", fv);
}

__attribute__ ((noinline))
//constexpr
void print_field(natural value, const field &f, const char *bit_names[], const id_txt bitfields_names[]) {
  assert(f.size > 0);
  if (f.size == 1) {
    print_bit_name(value, f, bit_names);
  } else {
    print_bitfield_name(value, f, bitfields_names);
  }
}

__attribute__ ((noinline))
//constexpr
void print_fields(natural value, field *fields, natural size, const char *bit_names[], const id_txt bitfields_names[]) {
  if (size == 0) {
    return;
  }

  print_field(value, fields[0], bit_names, bitfields_names);

  for (natural i = 1; i < size; i++) {
    putc('|', stdout);
    print_field(value, fields[i], bit_names, bitfields_names);
  }
}

int main() {
  print_fields(0xF, fields_example, 3, bit_names_example, bitfields_names_example);
  return 0;
}

