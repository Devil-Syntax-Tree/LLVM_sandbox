#pragma once

namespace kaleidoscope {
class Reader {
public:
  virtual ~Reader() {}
  // TODO: refactor to add UNICODE
  virtual char nextChar() = 0;
};
} // namespace kaleidoscope
