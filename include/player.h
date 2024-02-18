#include "bn_core.h"
#include "bn_log.h"
#include "bn_string.h"
#include "bn_sprites_actions.h"
#include "bn_sprite_actions.h"
#include "bn_sprite_builder.h"
#include "bn_sprite_item.h"
#include "bn_regular_bg_ptr.h"
#include "bn_regular_bg_map_item.h"
#include "bn_sprite_animate_actions.h"
#include "bn_unique_ptr.h"

// my generated headers
#include "bn_sprite_items_turnaround32.h"
#include "bn_sprite_items_fish_item.h"

// my written headers
#include "kitchen.h"

namespace kt {
    enum Direction {
        Up,
        Down,
        Left,
        Right
    };
    class Player {
        public:
        // NOTE you need the initializer list or else you get a compile-time error like "no matching function to call"
            Player(bn::regular_bg_map_item map) :
                        player_spr_ptr(bn::sprite_items::turnaround32.create_sprite(0, 0)),
                        map_item(map), 
                        player_tiles(bn::sprite_items::turnaround32.tiles_item()),
                        walk_cycle(bn::create_sprite_animate_action_forever(player_spr_ptr, 16, player_tiles, 0, 1, 2, 3)),
                        kitchen(map) {
                bn::log(bn::string<20>("Player constructed"));
                // here 16 is the cell we want to be in, * 8 is to convert to pixels (mb switch to just 128 later?)
                int start_pt = 16 * 8;
                player_pos = bn::point(start_pt, start_pt);
                hitbox_bl = bn::point(start_pt - 5, start_pt + 15);
                hitbox_br = bn::point(start_pt + 5, start_pt + 15);
                hitbox_tl = bn::point(start_pt - 5, start_pt + 1);
                hitbox_tr = bn::point(start_pt + 5, start_pt + 1);
                dir = Down;

                // kitchen = Kitchen(map);
            };

            bool move_left() {
                bn::point new_player_pos = player_pos;
                new_player_pos.set_x(new_player_pos.x() - 1);
                if (dir != Left) {
                    dir = Left;
                    walk_cycle = bn::create_sprite_animate_action_forever(player_spr_ptr, 16, player_tiles, 12, 13, 14, 15);
                }
                bn::point new_hitbox_tr = bn::point(hitbox_tr.x() - 1, hitbox_tr.y());
                bn::point new_hitbox_br = bn::point(hitbox_br.x() - 1, hitbox_br.y());
                bn::point new_hitbox_tl = bn::point(hitbox_tl.x() - 1, hitbox_tl.y());
                bn::point new_hitbox_bl = bn::point(hitbox_bl.x() - 1, hitbox_bl.y());
                if (!_hitbox_collided(new_hitbox_tl, new_hitbox_bl, kitchen.valid_tile_index())) {
                    player_pos = new_player_pos;
                    hitbox_tr = new_hitbox_tr;
                    hitbox_br = new_hitbox_br;
                    hitbox_tl = new_hitbox_tl;
                    hitbox_bl = new_hitbox_bl;
                    player_spr_ptr.set_position(player_pos.x() - (map_item.dimensions().width() * 4), player_pos.y() - (map_item.dimensions().height() * 4));
                    return true;
                }
                return false;
            };

            bool move_right() {
                bn::point new_player_pos = player_pos;
                new_player_pos.set_x(new_player_pos.x() + 1);
                if (dir != Right) {
                    dir = Right;
                    walk_cycle = bn::create_sprite_animate_action_forever(player_spr_ptr, 16, player_tiles, 8, 9, 10, 11);
                }
                bn::point new_hitbox_tr = bn::point(hitbox_tr.x() + 1, hitbox_tr.y());
                bn::point new_hitbox_br = bn::point(hitbox_br.x() + 1, hitbox_br.y());
                bn::point new_hitbox_tl = bn::point(hitbox_tl.x() + 1, hitbox_tl.y());
                bn::point new_hitbox_bl = bn::point(hitbox_bl.x() + 1, hitbox_bl.y());
                if (!_hitbox_collided(new_hitbox_tr, new_hitbox_br, kitchen.valid_tile_index())) {
                    player_pos = new_player_pos;
                    hitbox_tr = new_hitbox_tr;
                    hitbox_br = new_hitbox_br;
                    hitbox_tl = new_hitbox_tl;
                    hitbox_bl = new_hitbox_bl;
                    player_spr_ptr.set_position(player_pos.x() - (map_item.dimensions().width() * 4), player_pos.y() - (map_item.dimensions().height() * 4));
                    return true;
                }
                return false;
            };

            bool move_up() {
                bn::point new_player_pos = player_pos;
                new_player_pos.set_y(new_player_pos.y() - 1);
                if (dir != Up) {
                    dir = Up;
                    walk_cycle = bn::create_sprite_animate_action_forever(player_spr_ptr, 16, player_tiles, 4, 5, 6, 7);
                }
                bn::point new_hitbox_tr = bn::point(hitbox_tr.x(), hitbox_tr.y() - 1);
                bn::point new_hitbox_br = bn::point(hitbox_br.x(), hitbox_br.y() - 1);
                bn::point new_hitbox_tl = bn::point(hitbox_tl.x(), hitbox_tl.y() - 1);
                bn::point new_hitbox_bl = bn::point(hitbox_bl.x(), hitbox_bl.y() - 1);
                if (!_hitbox_collided(new_hitbox_tr, new_hitbox_tl, kitchen.valid_tile_index())) {
                    player_pos = new_player_pos;
                    hitbox_tr = new_hitbox_tr;
                    hitbox_br = new_hitbox_br;
                    hitbox_tl = new_hitbox_tl;
                    hitbox_bl = new_hitbox_bl;
                    player_spr_ptr.set_position(player_pos.x() - (map_item.dimensions().width() * 4), player_pos.y() - (map_item.dimensions().height() * 4));
                    return true;
                }
                return false;
            };

            bool move_down() {
                bn::point new_player_pos = player_pos;
                new_player_pos.set_y(new_player_pos.y() + 1);
                if (dir != Down) {
                    dir = Down;
                    walk_cycle = bn::create_sprite_animate_action_forever(player_spr_ptr, 16, player_tiles, 0, 1, 2, 3);
                }
                bn::point new_hitbox_tr = bn::point(hitbox_tr.x(), hitbox_tr.y() + 1);
                bn::point new_hitbox_br = bn::point(hitbox_br.x(), hitbox_br.y() + 1);
                bn::point new_hitbox_tl = bn::point(hitbox_tl.x(), hitbox_tl.y() + 1);
                bn::point new_hitbox_bl = bn::point(hitbox_bl.x(), hitbox_bl.y() + 1);
                if (!_hitbox_collided(new_hitbox_br, new_hitbox_bl, kitchen.valid_tile_index())) {
                    player_pos = new_player_pos;
                    hitbox_tr = new_hitbox_tr;
                    hitbox_br = new_hitbox_br;
                    hitbox_tl = new_hitbox_tl;
                    hitbox_bl = new_hitbox_bl;
                    player_spr_ptr.set_position(player_pos.x() - (map_item.dimensions().width() * 4), player_pos.y() - (map_item.dimensions().height() * 4));
                    return true;
                }
                return false;
            };

            bool interact() {
                // Calculate interaction collision point
                bn::point interact_point;
                if (dir == Up) {
                    interact_point = player_pos;
                } else if (dir == Down) {
                    interact_point = bn::point(player_pos.x(), player_pos.y() + 16);
                } else if (dir == Left) {
                    interact_point = bn::point(player_pos.x() - 8, player_pos.y() + 1);
                } else if (dir == Right) {
                    interact_point = bn::point(player_pos.x() + 8, player_pos.y() + 1);
                }

                // Determine if this is a collision or not
                bn::regular_bg_map_cell interact_cell = map_item.cell(bn::point(interact_point.x() / 8, interact_point.y() / 8));
                int interact_index = bn::regular_bg_map_cell_info(interact_cell).tile_index();
                // If player is looking at a floor tile
                if (interact_index == kitchen.valid_tile_index()) return false;

                uint8_t interact_int = kitchen.interact(interact_index, held_fish);
                // If successful interaction
                if (interact_int & (1 << 0)) update_item_sprite();
                // If selling a fish
                if (interact_int & (1 << 1)) return true;

                return false;
            };

            void update_walk() { 
                walk_cycle.update();
                if (held_fish != nullptr) held_fish->update_fish_location(player_pos.x() - (map_item.dimensions().width() * 4), player_pos.y() - (map_item.dimensions().height() * 4));
            };

            void update_item_sprite() {
                if (held_fish == nullptr) {
                    
                } else {
                    held_fish->update_fish_location(player_pos.x() - (map_item.dimensions().width() * 4), player_pos.y() - (map_item.dimensions().height() * 4));
                    bn::log(bn::string<32>("updated fish sprite"));
                }
            };

            void kitchen_update() {
                kitchen.update();
            };

            bool selling_fish() {
                return kitchen.selling_fish();
            };

            void set_selling_fish(bool val) {
                kitchen.set_selling_fish(val);
            };

            bn::unique_ptr<Fish>& get_fish_to_sell() {
                return kitchen.get_fish_to_sell();
            };

            void sell_fish() {
                kitchen.sell_fish();
            };

        private:
            bool _hitbox_collided(bn::point p1, bn::point p2, int valid_index) {
                bn::regular_bg_map_cell p1_map_cell = map_item.cell(bn::point(p1.x() / 8, p1.y() / 8));
                bn::regular_bg_map_cell p2_map_cell = map_item.cell(bn::point(p2.x() / 8, p2.y() / 8));
                int p1_index = bn::regular_bg_map_cell_info(p1_map_cell).tile_index();
                int p2_index = bn::regular_bg_map_cell_info(p2_map_cell).tile_index();

                return !(p1_index == valid_index && p2_index == valid_index);
            };

            bn::sprite_ptr player_spr_ptr;
            bn::regular_bg_map_item map_item;
            bn::sprite_tiles_item player_tiles;
            bn::sprite_animate_action<4> walk_cycle;
            bn::point player_pos;
            bn::unique_ptr<Fish> held_fish;
            int dir;
            bn::point hitbox_tr;
            bn::point hitbox_br;
            bn::point hitbox_tl;
            bn::point hitbox_bl;

            Kitchen kitchen;
            
    };
}