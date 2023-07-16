#include <exception>
#include <stdexcept>

class AlreadyExistsError : public std::exception {
 public:
  AlreadyExistsError() = default;
};
class PathNotFoundError : public std::exception {
 public:
  PathNotFoundError() = default;
};
