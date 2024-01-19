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
            Level(bn::regular_bg_map_item map) :
                        player(Player(map)) {
                // player = Player(map);
            };

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

            // debug
            void debug_fish_address() {
                player.debug_fish_address();
            };
        private:
            Player player;
    };
}