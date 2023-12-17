
#pragma once

#include <cstring>
#include <vector>

namespace gecko {

class String {
  std::vector<char> buffer;

public:
  String() {}

  String(const char *str) {
    size_t size = strlen(str);
    buffer.assign(size, 0);
    memcpy(buffer.data(), str, size);
  }

  String operator=(const char *str) {
    size_t size = strlen(str);
    buffer.clear();
    buffer.assign(size, 0);
    memcpy(buffer.data(), str, size);
    return *this;
  }

  const char *c_str() const { return buffer.data(); }
  void operator+=(char c) { buffer.push_back(c); }
};

static String to_string(const float f) {
  char buffer[16];
  snprintf(buffer, 16, "%f", f);
  return String(buffer);
}

} // end namespace gecko