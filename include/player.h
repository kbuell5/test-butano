#include "bn_core.h"
#include "bn_log.h"
#include "bn_string.h"
#include "bn_sprites_actions.h"
#include "bn_sprite_actions.h"
#include "bn_sprite_builder.h"
#include "bn_sprite_item.h"
#include "bn_regular_bg_ptr.h"
#include "bn_regular_bg_map_item.h"

// my generated headers
#include "bn_sprite_items_turnaround32.h"

namespace kt {
    enum HeldItem {
        None,
        Fish,
        Butterfly
    };
    class Player {
        public:
        // NOTE you need the initializer list or else you get a compile-time error like "no matching function to call"
            Player(bn::sprite_ptr spr_ptr, bn::sprite_item spr_item, bn::regular_bg_map_item map) : player_spr_ptr(spr_ptr), player_spr_item(spr_item), map_item(map) {
                bn::log(bn::string<10>("it worked"));
                player_pos = bn::point(16 * 8 + 8, 16 * 8);
                held_item = None;
            };

            bool move_left(int valid_tile) {
                bn::point new_player_pos = player_pos;
                new_player_pos.set_x(new_player_pos.x() - 1);
                player_spr_ptr.set_tiles(player_spr_item.tiles_item().create_tiles(12));
                bn::regular_bg_map_cell new_map_cell = map_item.cell(bn::point(new_player_pos.x() / 8, new_player_pos.y() / 8));
                int player_tile_index = bn::regular_bg_map_cell_info(new_map_cell).tile_index();
                if (player_tile_index == valid_tile) {
                    player_pos = new_player_pos;
                    player_spr_ptr.set_position(player_pos.x() - (map_item.dimensions().width() * 4), player_pos.y() - (map_item.dimensions().height() * 4));
                    return true;
                }
                return false;
            };

            bool move_right(int valid_tile) {
                bn::point new_player_pos = player_pos;
                new_player_pos.set_x(new_player_pos.x() + 1);
                player_spr_ptr.set_tiles(player_spr_item.tiles_item().create_tiles(8));
                bn::regular_bg_map_cell new_map_cell = map_item.cell(bn::point(new_player_pos.x() / 8, new_player_pos.y() / 8));
                int player_tile_index = bn::regular_bg_map_cell_info(new_map_cell).tile_index();
                if (player_tile_index == valid_tile) {
                    bn::log(bn::string<25>("moved right player class"));
                    player_pos = new_player_pos;
                    player_spr_ptr.set_position(player_pos.x() - (map_item.dimensions().width() * 4), player_pos.y() - (map_item.dimensions().height() * 4));
                    return true;
                }
                return false;
            };

            bool move_up(int valid_tile) {
                bn::point new_player_pos = player_pos;
                new_player_pos.set_y(new_player_pos.y() - 1);
                player_spr_ptr.set_tiles(player_spr_item.tiles_item().create_tiles(4));
                bn::regular_bg_map_cell new_map_cell = map_item.cell(bn::point(new_player_pos.x() / 8, new_player_pos.y() / 8));
                int player_tile_index = bn::regular_bg_map_cell_info(new_map_cell).tile_index();
                if (player_tile_index == valid_tile) {
                    player_pos = new_player_pos;
                    player_spr_ptr.set_position(player_pos.x() - (map_item.dimensions().width() * 4), player_pos.y() - (map_item.dimensions().height() * 4));
                    return true;
                }
                return false;
            };

            bool move_down(int valid_tile) {
                bn::point new_player_pos = player_pos;
                new_player_pos.set_y(new_player_pos.y() + 1);
                player_spr_ptr.set_tiles(player_spr_item.tiles_item().create_tiles(0));
                bn::regular_bg_map_cell new_map_cell = map_item.cell(bn::point(new_player_pos.x() / 8, new_player_pos.y() / 8));
                int player_tile_index = bn::regular_bg_map_cell_info(new_map_cell).tile_index();
                if (player_tile_index == valid_tile) {
                    player_pos = new_player_pos;
                    player_spr_ptr.set_position(player_pos.x() - (map_item.dimensions().width() * 4), player_pos.y() - (map_item.dimensions().height() * 4));
                    return true;
                }
                return false;
            };

            void Pickup(int item) {
                bn::log(bn::string<10>("picked up"));
            };

            void PutDown() {

            };

        private:
            bn::sprite_ptr player_spr_ptr;
            bn::sprite_item player_spr_item;
            bn::point player_pos;
            bn::regular_bg_map_item map_item;
            int held_item;
    };
}