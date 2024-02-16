#include "bn_core.h"
#include "bn_log.h"
#include "bn_string.h"
#include "bn_sprites_actions.h"
#include "bn_sprite_actions.h"
#include "bn_sprite_builder.h"
#include "bn_sprite_item.h"
#include "bn_regular_bg_map_item.h"
#include "bn_sprite_animate_actions.h"
#include "bn_vector.h"

#include "bn_sprite_items_fish_item.h"
#include "bn_sprite_items_green_fish_item.h"
#include "bn_sprite_items_legs.h"
#include "bn_sprite_items_fish_makeup.h"

namespace kt {
    enum FishType {
        Purple,
        Green
    };

    struct FishConfig {
//          BYTE MAP  |   0  |   1  |    2   |     3    |  4  |  5  |  6  |  7  |
//          BYTE NAME | LEGS | KISS | MAKEUP | SPARKLES | NAN | NAN | NAN | NAN |
        uint8_t config_bool = 0b00000000;
        FishType fish_type;

        bool operator==(const FishConfig& other) const {
            return (config_bool == other.config_bool && fish_type == other.fish_type);
        }
    };

    bn::sprite_item enum_to_sprite_item(FishType type) {
        if (type == Purple) {
            return bn::sprite_items::fish_item;
        } else {
            return bn::sprite_items::green_fish_item;
        }
    };

    class Fish {
        public:
            Fish(FishType fish_type) {
                fish_id = fish_id_counter++;
                fish_config = {
                    0b00000000,
                    fish_type
                };
                switch (fish_type) {
                    case Purple: {
                        bn::sprite_ptr* fish_spr = new bn::sprite_ptr(bn::sprite_items::fish_item.create_sprite(0, 0));
                        // fish_spr = bn::sprite_items::fish_item.create_sprite(0, 0);
                        fish_sprites.push_back(fish_spr);
                        break;
                    }
                    case Green: {
                        bn::sprite_ptr* fish_spr = new bn::sprite_ptr(bn::sprite_items::green_fish_item.create_sprite(0,0));
                        fish_sprites.push_back(fish_spr);
                        break;
                    }
                }
            };

            Fish(const Fish& other) = default;

            ~Fish() { 
                // these do not work
                // delete &fish_spr_item; // for whatever reason this line causes the game to lock when attempting to pick up a third fish
                // delete &fish_spr_ptr;
                bn::log(bn::string<16>("fish destructed"));
            };

            void delete_fish() {
                // upgrade_sprites.clear();
                // delete &fish_spr_ptr;
            };

            Fish& operator=(const Fish& other) {
                fish_id = other.fish_id;
                fish_config = other.fish_config;
                return *this;
            };

            int get_fish_id() {
                return fish_id;
            };

            FishConfig get_fish_config() {
                return fish_config;
            };

            uint8_t get_fish_config_bool() {
                return fish_config.config_bool;
            };

            // bn::sprite_ptr get_fish_spr_ptr() {
            //     return fish_spr_ptr;
            // };

            FishType get_fish_type() {
                return fish_config.fish_type;
            };

            bool legs() {
                return (fish_config.config_bool & (1 << 7));
            };

            bool kiss() {
                return (fish_config.config_bool & (1 << 6));
            };

            bool makeup() {
                return (fish_config.config_bool & (1 << 5));
            };

            bool sparkles() {
                return (fish_config.config_bool & (1 << 4));
            };

            void give_legs() {
                fish_config.config_bool |= 0b10000000;
                bn::sprite_ptr* legs = new bn::sprite_ptr(bn::sprite_items::legs.create_sprite(fish_sprites[0]->position().x(), fish_sprites[0]->position().y()));
                fish_sprites.push_back(legs);
            };

            void give_kiss() {
                fish_config.config_bool |= 0b01000000;
            };

            void give_makeup() {
                fish_config.config_bool |= 0b00100000;
                bn::sprite_ptr* makeup = new bn::sprite_ptr(bn::sprite_items::fish_makeup.create_sprite(fish_sprites[0]->position().x(), fish_sprites[0]->position().y()));
                fish_sprites.push_back(makeup);
                bn::log(bn::string<16>("woooo"));
            };

            void give_sparkles() {
                fish_config.config_bool |= 0b00010000;
            };

            bool is_basic() {
                return (fish_config.config_bool == 0);
            };

            void put_fish_below() {
                // for (bn::vector<bn::sprite_ptr, 8>::iterator it = fish_sprites.begin(); it != fish_sprites.end(); it++) {
                //     it->put_below();
                // }
                // fish_spr_ptr.put_below();
            };

            void put_fish_above() {
                // fish_spr_ptr.put_above();
                // for (bn::vector<bn::sprite_ptr, 8>::iterator it = fish_sprites.begin(); it != fish_sprites.end(); it++) {
                //     it->put_above();
                // }  
            };

            // these may be obsolete
            void show_fish() {
                // fish_spr_ptr.set_tiles(fish_spr_item.tiles_item().create_tiles(1));
                // TODO show customized stuff
            };

            void hide_fish() {
                // fish_spr_ptr.set_tiles(fish_spr_item.tiles_item().create_tiles(0));
            };

            void update_fish_location(int x, int y) {
                // fish_spr_ptr.set_position(x, y);
                for (bn::vector<bn::sprite_ptr*, 8>::iterator it = fish_sprites.begin(); it != fish_sprites.end(); it++) {
                    (*it)->set_position(x, y);
                }
            };

        private:
            static uint32_t fish_id_counter;
            uint32_t fish_id;

            bn::vector<bn::sprite_ptr*, 8> fish_sprites;
            FishConfig fish_config;

            // bn::sprite_item fish_spr_item;
            // bn::sprite_ptr fish_spr_ptr;
    };

    uint32_t Fish::fish_id_counter = 0;
}