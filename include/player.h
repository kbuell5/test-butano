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
                // here 16 is the cell we want to be in, * 8 is to convert to pixels (mb switch to just 128 later?)
                int start_pt = 16 * 8;
                player_pos = bn::point(start_pt, start_pt);
                hitbox_bl = bn::point(start_pt - 5, start_pt + 15);
                hitbox_br = bn::point(start_pt + 5, start_pt + 15);
                hitbox_tl = bn::point(start_pt - 5, start_pt + 1);
                hitbox_tr = bn::point(start_pt + 5, start_pt + 1);
                bn::log(bn::string<32>(bn::to_string<16>(player_pos.x()) + " " + bn::to_string<16>(player_pos.y())));
                held_item = None;
            };

            bool move_left(int valid_tile) {
                bn::point new_player_pos = player_pos;
                new_player_pos.set_x(new_player_pos.x() - 1);
                player_spr_ptr.set_tiles(player_spr_item.tiles_item().create_tiles(12));
                bn::point new_hitbox_tr = bn::point(hitbox_tr.x() - 1, hitbox_tr.y());
                bn::point new_hitbox_br = bn::point(hitbox_br.x() - 1, hitbox_br.y());
                bn::point new_hitbox_tl = bn::point(hitbox_tl.x() - 1, hitbox_tl.y());
                bn::point new_hitbox_bl = bn::point(hitbox_bl.x() - 1, hitbox_bl.y());
                if (!_hitbox_collided(new_hitbox_tl, new_hitbox_bl, valid_tile)) {
                    bn::log(bn::string<15>("no hitbox left"));
                    player_pos = new_player_pos;
                    hitbox_tr = new_hitbox_tr;
                    hitbox_br = new_hitbox_br;
                    hitbox_tl = new_hitbox_tl;
                    hitbox_bl = new_hitbox_bl;
                    player_spr_ptr.set_position(player_pos.x() - (map_item.dimensions().width() * 4), player_pos.y() - (map_item.dimensions().height() * 4));
                    bn::log(bn::string<64>("sprite ptr pos: " + bn::to_string<32>(player_pos.x()) + " " + bn::to_string<32>(player_pos.y())));
                    return true;
                }
                return false;
            };

            bool move_right(int valid_tile) {
                bn::point new_player_pos = player_pos;
                new_player_pos.set_x(new_player_pos.x() + 1);
                player_spr_ptr.set_tiles(player_spr_item.tiles_item().create_tiles(8));
                bn::point new_hitbox_tr = bn::point(hitbox_tr.x() + 1, hitbox_tr.y());
                bn::point new_hitbox_br = bn::point(hitbox_br.x() + 1, hitbox_br.y());
                bn::point new_hitbox_tl = bn::point(hitbox_tl.x() + 1, hitbox_tl.y());
                bn::point new_hitbox_bl = bn::point(hitbox_bl.x() + 1, hitbox_bl.y());
                if (!_hitbox_collided(new_hitbox_tr, new_hitbox_br, valid_tile)) {
                    bn::log(bn::string<25>("moved right player class"));
                    player_pos = new_player_pos;
                    hitbox_tr = new_hitbox_tr;
                    hitbox_br = new_hitbox_br;
                    hitbox_tl = new_hitbox_tl;
                    hitbox_bl = new_hitbox_bl;
                    player_spr_ptr.set_position(player_pos.x() - (map_item.dimensions().width() * 4), player_pos.y() - (map_item.dimensions().height() * 4));
                    bn::log(bn::string<64>("sprite ptr pos: " + bn::to_string<32>(player_pos.x()) + " " + bn::to_string<32>(player_pos.y())));
                    return true;
                }
                return false;
            };

            bool move_up(int valid_tile) {
                bn::point new_player_pos = player_pos;
                new_player_pos.set_y(new_player_pos.y() - 1);
                player_spr_ptr.set_tiles(player_spr_item.tiles_item().create_tiles(4));
                bn::point new_hitbox_tr = bn::point(hitbox_tr.x(), hitbox_tr.y() - 1);
                bn::point new_hitbox_br = bn::point(hitbox_br.x(), hitbox_br.y() - 1);
                bn::point new_hitbox_tl = bn::point(hitbox_tl.x(), hitbox_tl.y() - 1);
                bn::point new_hitbox_bl = bn::point(hitbox_bl.x(), hitbox_bl.y() - 1);
                if (!_hitbox_collided(new_hitbox_tr, new_hitbox_tl, valid_tile)) {
                    bn::log(bn::string<15>("no hitbox up"));
                    player_pos = new_player_pos;
                    hitbox_tr = new_hitbox_tr;
                    hitbox_br = new_hitbox_br;
                    hitbox_tl = new_hitbox_tl;
                    hitbox_bl = new_hitbox_bl;
                    player_spr_ptr.set_position(player_pos.x() - (map_item.dimensions().width() * 4), player_pos.y() - (map_item.dimensions().height() * 4));
                    bn::log(bn::string<64>("sprite ptr pos: " + bn::to_string<32>(player_pos.x()) + " " + bn::to_string<32>(player_pos.y())));
                    return true;
                }
                return false;
            };

            bool move_down(int valid_tile) {
                bn::point new_player_pos = player_pos;
                new_player_pos.set_y(new_player_pos.y() + 1);
                player_spr_ptr.set_tiles(player_spr_item.tiles_item().create_tiles(0));
                bn::point new_hitbox_tr = bn::point(hitbox_tr.x(), hitbox_tr.y() + 1);
                bn::point new_hitbox_br = bn::point(hitbox_br.x(), hitbox_br.y() + 1);
                bn::point new_hitbox_tl = bn::point(hitbox_tl.x(), hitbox_tl.y() + 1);
                bn::point new_hitbox_bl = bn::point(hitbox_bl.x(), hitbox_bl.y() + 1);
                if (!_hitbox_collided(new_hitbox_br, new_hitbox_bl, valid_tile)) {
                    bn::log(bn::string<15>("no hitbox down"));
                    player_pos = new_player_pos;
                    hitbox_tr = new_hitbox_tr;
                    hitbox_br = new_hitbox_br;
                    hitbox_tl = new_hitbox_tl;
                    hitbox_bl = new_hitbox_bl;
                    player_spr_ptr.set_position(player_pos.x() - (map_item.dimensions().width() * 4), player_pos.y() - (map_item.dimensions().height() * 4));
                    bn::log(bn::string<64>("sprite ptr pos: " + bn::to_string<32>(player_pos.x()) + " " + bn::to_string<32>(player_pos.y())));
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
            bool _hitbox_collided(bn::point p1, bn::point p2, int valid_index) {
                bn::log(bn::to_string<16>(p1.x()) + " " + bn::to_string<16>(p1.y()));
                bn::log(bn::to_string<16>(p2.x()) + " " + bn::to_string<16>(p2.y()));
                bn::regular_bg_map_cell p1_map_cell = map_item.cell(bn::point(p1.x() / 8, p1.y() / 8));
                bn::regular_bg_map_cell p2_map_cell = map_item.cell(bn::point(p2.x() / 8, p2.y() / 8));
                int p1_index = bn::regular_bg_map_cell_info(p1_map_cell).tile_index();
                int p2_index = bn::regular_bg_map_cell_info(p2_map_cell).tile_index();

                return !(p1_index == valid_index && p2_index == valid_index);
            };

            bn::sprite_ptr player_spr_ptr;
            bn::sprite_item player_spr_item;
            bn::point player_pos;
            bn::regular_bg_map_item map_item;
            int held_item;
            bn::point hitbox_tr;
            bn::point hitbox_br;
            bn::point hitbox_tl;
            bn::point hitbox_bl;
            
    };
}