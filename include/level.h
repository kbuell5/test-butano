#include "bn_core.h"
#include "bn_log.h"
#include "bn_string.h"
#include "bn_sprite_item.h"
#include "bn_regular_bg_ptr.h"
#include "bn_regular_bg_map_item.h"
#include "bn_sprite_animate_actions.h"

#include "bn_sprite_items_turnaround32.h"

#include "player.h"

namespace kt {
    class Level {
        public:
            Level(bn::regular_bg_map_item map, bn::vector<FishConfig, 6> fish_con) :
                        player(Player(map)) {
                fish_configs = fish_con;
            };

            bool is_level_started() {
                return is_started;
            }

            void move_player_up() {
                player.move_up();
                player.update_walk();
            };

            void move_player_down() {
                player.move_down();
                player.update_walk();
            };

            void move_player_left() {
                player.move_left();
                player.update_walk();
            };

            void move_player_right() {
                player.move_right();
                player.update_walk();
            };

            void interact_player() {
                player.interact();
            };

            void player_kitchen_update() {
                player.kitchen_update();
            };

            void start_level() {
                is_started = true;
            };

            void sell_fish(FishType fish_type) {
                // if (fish_type == Purple) {
                //     purple_fish_needed--;
                // } else if (fish_type == Green) {
                //     green_fish_needed--;
                // }
            }

            // debug
            void debug_fish_address() {
                player.debug_fish_address();
            };
        private:
            Player player;
            bool is_started = false;

            bn::vector<FishConfig, 6> fish_configs;

            // int purple_fish_needed;
            // int green_fish_needed;
            int num_customers;
    };
}