#include <string>
#include <sstream>

namespace patch {
    template <typename T> std::string to_string(T value) {
        std::ostringstream os;
        os << value;
        return os.str();
    }

    inline unsigned long stoul(const std::string& str, size_t* pos = 0, int base = 10) {
        char* endptr;
        unsigned long res = strtoul(str.c_str(), &endptr, base);
        if (pos) *pos = endptr - str.c_str();
        return res;
    }
}
