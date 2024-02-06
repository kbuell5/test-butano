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

            int interact_player() {
                bn::optional<FishConfig> maybe_sell_fish = player.interact();
                if (maybe_sell_fish) {
                    // Check if the fish attempting to be sold is one of the goal fish
                    bn::log(bn::string<32>("tripped a maybe sell fish"));
                    for (bn::vector<FishConfig, 6>::iterator it = fish_configs.begin(); it != fish_configs.end(); it++) {
                        if (maybe_sell_fish == *it) {
                            bn::log(bn::string<32>("sell fish found iterator"));
                            
                            // Sell fish
                            int money_earned = sell_fish(it);
                            return money_earned;
                        }
                    }
                }
                return 0;
            };

            void player_kitchen_update() {
                player.kitchen_update();
            };

            void start_level() {
                is_started = true;
            };

            int sell_fish(bn::vector<FishConfig, 6>::iterator fish) {
                bn::log(bn::string<32>("this shit worked"));
                bn::log(bn::string<32>(bn::to_string<16>(fish->config_bool)));
                fish_configs.erase(fish);
                // TODO scale these based on fish patience?
                return 45;
            };

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