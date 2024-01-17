#include "bn_core.h"
#include "bn_log.h"
#include "bn_string.h"

namespace kt {
    class Fish {
        public:
            Fish() {
                fish_id = fish_id_counter++;
            };

            Fish(const Fish& other) = default;

            ~Fish() { 
                bn::log(bn::string<16>("fish destructed"));
            };

            Fish& operator=(const Fish& other) {
                fish_id = other.fish_id;
                fish_config = other.fish_config;
                return *this;
            };

            int get_fish_id() {
                return fish_id;
            };

            uint8_t get_fish_config() {
                return fish_config;
            };

            void give_legs() {
                fish_config |= 0b10000000;
            };

            void give_kiss() {
                fish_config |= 0b01000000;
            };

            void give_makeup() {
                fish_config |= 0b00100000;
            };

            void give_sparkles() {
                fish_config |= 0b00010000;
            };

        private:
            static uint32_t fish_id_counter;
            uint32_t fish_id;

//          BYTE MAP  |   0  |   1  |    2   |     3    |  4  |  5  |  6  |  7  |
//          BYTE NAME | LEGS | KISS | MAKEUP | SPARKLES | NAN | NAN | NAN | NAN |
            uint8_t fish_config = 0b00000000;
            
// //          BYTE MAP |   0   |  1   |   2   |    3    |   4   |   5   |   6  |   7   |
// //          BYTE NAME| BALLS | LEGS | ANGRY | RAINBOW | HAPPY | COMFY | EEPY | HORNY | 
//             uint8_t fish_config = 0b00000000;
//             fish_config |= 0x1F00;
//                 // if (fish_config || 0x1000){
//                     // fish_config = 0x1000;
//                 // } 
//             fish_config == 0x1000;
//             fish_config != 0x0010;
//             fish_config == 0x1010;
    };

    uint32_t Fish::fish_id_counter = 0;
}