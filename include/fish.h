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
        int patience_counter;
        int patience;

        FishConfig(uint8_t cfg, FishType ft) {
            config_bool = cfg;
            fish_type = ft;
            patience = 3;
            // starts at a minute, subtracts in intervals of 15 seconds
            patience_counter = 3600 - (enum_to_int(ft) * 900);
        }

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

    int enum_to_int(FishType type) {
        if (type == Purple) {
            return 0;
        } else {
            return 1;
        }
    };

    class Fish {
        public:
            Fish(FishType fish_type) {
                bn::log(bn::string<16>("muffin"));
                fish_id = fish_id_counter++;
                fish_config = {
                    0b00000000,
                    fish_type
                };
                bn::log(bn::string<16>("corn"));
                switch (fish_type) {
                    case Purple: {
                        bn::sprite_ptr fish_spr = bn::sprite_items::fish_item.create_sprite(0, 0);
                        fish_sprites.push_back(fish_spr);
                        break;
                    }
                    case Green: {
                        bn::sprite_ptr fish_spr = bn::sprite_items::green_fish_item.create_sprite(0, 0);
                        fish_sprites.push_back(fish_spr);
                        break;
                    }
                    default: {
                        bn::log(bn::string<32>("we messed up somehow"));
                    }
                }
                bn::log(bn::string<16>("gee"));
            };

            Fish(const Fish& other) = default;

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
                fish_sprites.push_back(bn::sprite_items::legs.create_sprite(fish_sprites[0].position().x(), fish_sprites[0].position().y()));
            };

            void give_kiss() {
                fish_config.config_bool |= 0b01000000;
            };

            void give_makeup() {
                fish_config.config_bool |= 0b00100000;
                fish_sprites.push_back(bn::sprite_items::fish_makeup.create_sprite(fish_sprites[0].position().x(), fish_sprites[0].position().y()));
                bn::log(bn::string<16>("woooo"));
            };

            void give_sparkles() {
                fish_config.config_bool |= 0b00010000;
            };

            bool is_basic() {
                return (fish_config.config_bool == 0);
            };

            void put_fish_below() {

            };

            void put_fish_above() {
                  
            };

            void update_fish_location(int x, int y) {
                for (bn::vector<bn::sprite_ptr, 8>::iterator it = fish_sprites.begin(); it != fish_sprites.end(); it++) {
                    it->set_position(x, y);
                }
            };

        private:
            static uint32_t fish_id_counter;
            uint32_t fish_id;

            bn::vector<bn::sprite_ptr, 8> fish_sprites;
            FishConfig fish_config;
    };

    uint32_t Fish::fish_id_counter = 0;
}