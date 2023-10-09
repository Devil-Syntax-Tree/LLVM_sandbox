#include <filesystem>
#include <fstream>

#include "Reader.hpp"

namespace kaleidoscope {
class FileReader : public Reader {
public:
  FileReader(std::filesystem::path path);
  char nextCharacter() override;

private:
  std::ifstream file;
};
} // namespace kaleidoscope
