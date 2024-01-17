#include "bn_core.h"
#include "bn_log.h"
#include "bn_string.h"
#include "bn_sprites_actions.h"
#include "bn_sprite_actions.h"
#include "bn_sprite_builder.h"
#include "bn_sprite_item.h"
#include "bn_regular_bg_map_item.h"
#include "bn_sprite_animate_actions.h"

#include "bn_sprite_items_fish_item.h"

namespace kt {
    class Fish {
        public:
            Fish() :
                    fish_spr_item(bn::sprite_items::fish_item),
                    fish_spr_ptr(fish_spr_item.create_sprite(0, 0)) {
                fish_id = fish_id_counter++;
                show_fish();
            };

            Fish(const Fish& other) = default;

            ~Fish() { 
                // these do not work
                // delete &fish_spr_item; // for whatever reason this line causes the game to lock when attempting to pick up a third fish
                // delete &fish_spr_ptr;
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

            bn::sprite_ptr get_fish_spr_ptr() {
                return fish_spr_ptr;
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

            // Sprite functions
            void show_fish() {
                fish_spr_ptr.set_tiles(fish_spr_item.tiles_item().create_tiles(1));
                // TODO show customized stuff
            };

            void hide_fish() {
                fish_spr_ptr.set_tiles(fish_spr_item.tiles_item().create_tiles(0));
            };

            void update_fish_location(int x, int y) {
                fish_spr_ptr.set_position(x, y);
            };

        private:
            static uint32_t fish_id_counter;
            uint32_t fish_id;

//          BYTE MAP  |   0  |   1  |    2   |     3    |  4  |  5  |  6  |  7  |
//          BYTE NAME | LEGS | KISS | MAKEUP | SPARKLES | NAN | NAN | NAN | NAN |
            uint8_t fish_config = 0b00000000;

            bn::sprite_item fish_spr_item;
            bn::sprite_ptr fish_spr_ptr;
    };

    uint32_t Fish::fish_id_counter = 0;
}