#include "bn_core.h"
#include "bn_log.h"
#include "bn_string.h"

namespace kt {
    class Fish {
        public:
            Fish() {
                fish_num = 69;
            };

            Fish(const Fish& other) = default;

            Fish& operator=(const Fish& other) {
                fish_num = other.fish_num;
                return *this;
            }
        private:
            int fish_num;
    };
}