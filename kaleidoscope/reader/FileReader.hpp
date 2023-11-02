#include <filesystem>
#include <fstream>

#include "Reader.hpp"

namespace kaleidoscope {
class FileReader : public Reader {
public:
  FileReader(const std::string& path);
  char nextCharacter() override;

private:
  std::ifstream file;
};
} // namespace kaleidoscope
