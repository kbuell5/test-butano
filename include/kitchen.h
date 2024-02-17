#include "bn_core.h"
#include "bn_log.h"
#include "bn_string.h"
#include "bn_regular_bg_ptr.h"
#include "bn_regular_bg_map_item.h"
#include "bn_vector.h"
#include "bn_utility.h"
#include "bn_sprite_animate_actions.h"
#include "bn_optional.h"
#include "bn_unique_ptr.h"

#include "bn_regular_bg_items_map_interactive.h"
#include "bn_sprite_items_piechart.h"

// #include "fish_container.h"
#include "fish.h"

namespace kt {
    enum CellType {
        FishTank,
        Butterfly,
        Timer,
        Makeup,
        Customer,
        GreenFishTank
    };

    class Interactable {
        public:
            Interactable(bool interact, bool has_a_fish, CellType type_of_cell, int center_x, int center_y) :
                        pie_chart_ptr(bn::sprite_items::piechart.create_sprite(center_x, center_y)),
                        pie_anim(bn::create_sprite_animate_action_once(pie_chart_ptr, 20, bn::sprite_items::piechart.tiles_item(), 1, 2, 3, 4, 5, 6, 7, 8, 9)) {
                is_interactable = interact;
                has_fish = has_a_fish;
                type = type_of_cell;
                fish.reset();
                center = bn::point(center_x, center_y);
            };

            void do_action() {
                if (type == Butterfly) {
                    fish->give_legs();
                    bn::log(bn::string<32>("gave fish legs (in interactable)"));
                } else if (type == Makeup) {
                    fish->give_makeup();
                    bn::log(bn::string<32>("gave fish makeup"));
                    bn::log(bn::string<32>("gave fish makeup"));
                }
            };

            bool is_interactable = false;
            bool is_upgrading = false;
            bool has_fish = false;
            CellType type;
            bn::unique_ptr<Fish> fish;
            bn::point center;
            bn::sprite_ptr pie_chart_ptr;
            bn::sprite_animate_action<9> pie_anim;

            bn::vector<int, 32> tile_indices;
    };

    class Kitchen {
        public:
            Kitchen(bn::regular_bg_map_item map) : map_item(map), valid_cell(map_item.cell(0, 0)) {
                // valid_cell = map_item.cell(0, 0);
                valid_tile = bn::regular_bg_map_cell_info(valid_cell).tile_index();
                // ---------- Define interactable tiles
                int i = 1;

                interactables.push_back(Interactable(true, true, FishTank, -32, -24));
                for (i = 1; i <= 8; i++) {
                    interactables.back().tile_indices.push_back(bn::regular_bg_map_cell_info(map_item.cell(i, 0)).tile_index());
                }

                interactables.push_back(Interactable(true, false, Butterfly, -8, -24));
                for (i = 9; i <= 12; i++) {
                    interactables.back().tile_indices.push_back(bn::regular_bg_map_cell_info(map_item.cell(i, 0)).tile_index());
                }

                interactables.push_back(Interactable(true, false, Timer, 24, -24));
                for (i = 13; i <= 16; i++) {
                    interactables.back().tile_indices.push_back(bn::regular_bg_map_cell_info(map_item.cell(i, 0)).tile_index());
                }

                interactables.push_back(Interactable(true, true, GreenFishTank, -56, 8));
                for (i = 17; i <= 20; i++) {
                    interactables.back().tile_indices.push_back(bn::regular_bg_map_cell_info(map_item.cell(i, 0)).tile_index());
                }

                interactables.push_back(Interactable(true, false, Makeup, 40, -8));
                for (i = 21; i <= 24; i++) {
                    interactables.back().tile_indices.push_back(bn::regular_bg_map_cell_info(map_item.cell(i, 0)).tile_index());
                }

                interactables.push_back(Interactable(true, false, Customer, 40, 24));
                for (i = 28; i <= 28; i++) {
                    interactables.back().tile_indices.push_back(bn::regular_bg_map_cell_info(map_item.cell(i, 0)).tile_index());
                }
                // int i = 1;
                // for (i = 1; i <= 8; i++) {
                //     Interactable fishtank_inter(1, 8, true, true, FishTank, -32, -24);
                //     interactables.push_back(bn::pair<int, Interactable>(bn::regular_bg_map_cell_info(map_item.cell(i, 0)).tile_index(), Interactable(1, 8, true, true, FishTank, -32, -24)));
                // }
                // for (i = 9; i <= 12; i++) {
                //     Interactable butterfly_inter(9, 12, true, false, Butterfly, -8, -24);
                //     interactables.push_back(bn::pair<int, Interactable>(bn::regular_bg_map_cell_info(map_item.cell(i, 0)).tile_index(), Interactable(9, 12, true, false, Butterfly, -8, -24)));
                // }
                // for (i = 13; i <= 16; i++) {
                //     Interactable timer_inter(13, 16, true, false, Timer, 24, -24);
                //     interactables.push_back(bn::pair<int, Interactable>(bn::regular_bg_map_cell_info(map_item.cell(i, 0)).tile_index(), Interactable(13, 16, true, false, Timer, 24, -24)));
                // }
                // for (i = 17; i <= 20; i++) {
                //     Interactable green_fishtank_inter(17, 20, true, true, GreenFishTank, -56, 8);
                //     interactables.push_back(bn::pair<int, Interactable>(bn::regular_bg_map_cell_info(map_item.cell(i, 0)).tile_index(), Interactable(17, 20, true, true, GreenFishTank, -56, 8)));
                // }
                // for (i = 21; i <= 24; i++) {
                //     Interactable makeup_inter(21, 24, true, false, Makeup, 40, -8);
                //     interactables.push_back(bn::pair<int, Interactable>(bn::regular_bg_map_cell_info(map_item.cell(i, 0)).tile_index(), Interactable(21, 24, true, false, Makeup, 40, -8)));
                // }
                // for (i = 25; i <= 28; i++) {
                //     Interactable customer_inter(25, 28, true, false, Customer, 40, 24);
                //     interactables.push_back(bn::pair<int, Interactable>(bn::regular_bg_map_cell_info(map_item.cell(i, 0)).tile_index(), Interactable(25, 28, true, false, Customer, 40, 24)));
                // }

                try_sell = false;
            };

            int valid_tile_index() {
                return valid_tile;
            };

            uint8_t interact(int search_index, bn::unique_ptr<Fish>& held_item) {
                uint8_t interact_int = 0b00000000;
                for (int i = 0; i < interactables.size(); i++) {
                    // If we've found the interactable we've collided with
                    // if (interactables[i].first == search_index) {
                    for (int j = 0; j < interactables[i].tile_indices.size(); j++) {
                        if (interactables[i].tile_indices[j] == search_index) {
                            bn::log(bn::string<32>("found interactable"));
                            // Is it interactable at the moment?
                            // if (interactables[i].second.is_interactable == false) return interact_int;
                            if (interactables[i].is_interactable == false) return interact_int;

                            // Pick up, not holding a fish
                            if (!held_item) {
                                if (_pick_up(i, held_item)) interact_int |= 0b00000001;
                            } else { // Put down, holding a fish
                                uint8_t putted = _put_down(i, held_item);
                                if (putted & (1 << 0)) {
                                    interact_int |= 0b00000001;
                                    bn::log(bn::string<16>("putted"));
                                }

                                // If we sold a fish
                                if (putted & (1 << 1)) {
                                    interact_int |= 0b00000010;
                                    bn::log(bn::string<16>("again"));
                                }
                            }
                            // fish_container.print_fish();
                            return interact_int;
                        }
                    }
                    
                }
                bn::log(bn::string<32>("interaction failed"));
                return false;
            };

            void update() {
                // for (bn::vector<Interactable, 32>::iterator it = interactables.begin(); it != interactables.end(); it++) {
                //     if (it->second.is_upgrading && !it->second.pie_anim.done()) {
                //         it->second.pie_anim.update();
                //         if (it->second.pie_anim.done()) {
                //             it->second.is_upgrading = false;
                //             it->second.pie_anim.reset();
                //             it->second.pie_anim.update();
                //             it->second.pie_chart_ptr.set_tiles(bn::sprite_items::piechart.tiles_item().create_tiles(0));

                //             it->second.do_action();
                //         }
                //     }
                // }
                for (Interactable& inter : interactables) {
                    if (inter.is_upgrading && !inter.pie_anim.done()) {
                        inter.pie_anim.update();
                        if (inter.pie_anim.done()) {
                            inter.is_upgrading = false;
                            inter.pie_anim.reset();
                            inter.pie_anim.update();
                            inter.pie_chart_ptr.set_tiles(bn::sprite_items::piechart.tiles_item().create_tiles(0));

                            inter.do_action();
                        }
                    }
                }
            };

            bool selling_fish() {
                // return fish_container.try_sell;
                return try_sell;
            };

            void set_selling_fish(bool val) {
                // fish_container.try_sell = val;
                try_sell = val;
            };

            bn::unique_ptr<Fish>& get_fish_to_sell() {
                // NOTE customer interactable is hard coded
                // return interactables[25].second.fish;
                return interactables[5].fish;
            };

            void sell_fish() {
                // for (int i = 24; i <= 27; i++) {
                //     interactables[i].second.fish.reset();
                //     interactables[i].second.has_fish = false;
                //     bn::log(bn::to_string<16>(i));
                // }
                interactables[5].fish.reset();
                interactables[5].has_fish = false;

            };

        private:
            bool _pick_up(int i, bn::unique_ptr<Fish>& held_item) {
                bn::log(bn::string<16>("gogogogo"));
                if (interactables[i].type == FishTank) {
                    bn::log(bn::string<16>("dd"));
                    // held_item.swap(fish_container.add_fish(Purple));
                    fish_counter++;
                    bn::unique_ptr<Fish> new_fish = bn::make_unique<Fish>(Purple); // this should go out of scope and die?
                    held_item.swap(new_fish);
                    return true;
                }

                if (interactables[i].type == GreenFishTank) {
                    // fish_container.add_fish(Green);
                    fish_counter++;
                    bn::unique_ptr<Fish> new_fish = bn::make_unique<Fish>(Green); // this should go out of scope and die?
                    held_item.swap(new_fish);
                    return true;
                }

                // If interactable does not have a fish to pick up
                if (interactables[i].has_fish == false) return false;

                // If interactable is currently upgrading a fish
                if (interactables[i].is_upgrading) {
                    interactables[i].is_upgrading = false;
                    interactables[i].pie_anim.reset();
                    interactables[i].pie_anim.update();
                    interactables[i].pie_chart_ptr.set_tiles(bn::sprite_items::piechart.tiles_item().create_tiles(0));
                }
                bn::log(bn::string<64>("dfgdfg: "));
                // held_item = interactables[i].second.fish;
                held_item.swap(interactables[i].fish);
                bn::log(bn::string<64>("eeeee: "));
                bn::log(bn::string<64>("new address for held_item: "));
                // bn::log(bn::to_string<32>(*held_item));
                // Update flags of current cell
                interactables[i].has_fish = false;
                // interactables[i].second.fish = nullptr;
                bn::log(bn::string<64>("maybe we fucked up: "));
                // bn::log(bn::to_string<32>(*held_item));


                // Update flags of associated cells
                // NO-TE this only works because i've hardcoded the vector values to be sorted based on x value
                // for (int j = i - 1; j >= interactables[i].second.min_x - 1; j--) {
                //     interactables[j].second.has_fish = false;
                //     // interactables[j].second.fish = nullptr;
                // }
                // for (int k = i + 1; k <= interactables[i].second.max_x - 1; k++) {
                //     interactables[k].second.has_fish = false;
                //     // interactables[k].second.fish = nullptr;
                // }
                held_item->put_fish_above();
                bn::log(bn::string<32>("successful pick up"));
                return true;
            };

            uint8_t _put_down(int i, bn::unique_ptr<Fish>& held_item) {
                // If interactable already has a fish on it
                if ((interactables[i].type == FishTank && held_item->get_fish_type() == Purple) || 
                    (interactables[i].type == GreenFishTank && held_item->get_fish_type() == Green)) {
                    if (held_item->is_basic()) {
                        // fish_container.delete_fish(held_item->get_fish_id());
                        fish_counter--;
                        held_item.reset();
                        bn::log(bn::string<32>("fish put back :3"));
                        return 0b00000001;
                    } else {
                        bn::log(bn::string<48>("tried to put modified fish in tank >:3"));
                        return 0b00000000;
                    }
                }

                if (interactables[i].has_fish) {
                    return 0b00000000;
                }
                
                // If looking at the timer (trash can)
                if (interactables[i].type == Timer) {
                    // int temp_id = held_item->get_fish_id();
                    fish_counter--;
                    held_item.reset();
                    // fish_container.delete_fish(temp_id);
                    // held_item->delete_fish();
                    
                    // bn::log(bn::to_string<16>(held_item->get_fish_config_bool()));
                    // held_item = nullptr;
                    bn::log(bn::string<32>("frish garbaggio'd"));
                    // bn::log(bn::string<32>("fish id"));
                    // bn::log(bn::to_string<16>(held_item->get_fish_id()));
                    return 0b00000001;
                }

                uint8_t ret = 0b00000000;
                // If looking at customer (sell point)
                if (interactables[i].type == Customer) {
                    // fish_container.delete_fish(held_item->get_fish_id());
                    // held_item = nullptr;
                    bn::log(bn::string<32>("attempting to sell fihs"));
                    try_sell = true;

                    ret |= 0b00000010;
                }

                interactables[i].fish.swap(held_item);
                interactables[i].has_fish = true;

                // for (int j = i - 1; j >= interactables[i].second.min_x - 1; j--) {
                //     interactables[j].second.has_fish = true;
                //     // interactables[i].second.fish = held_item;
                // }
                // for (int k = i + 1; k <= interactables[i].second.max_x - 1; k++) {
                //     interactables[k].second.has_fish = true;
                //     // interactables[k].second.fish = held_item;
                // }
                interactables[i].fish->update_fish_location(interactables[i].center.x(), interactables[i].center.y());

                // If looking at butterfly (legs upgrade)
                if (interactables[i].type == Butterfly && !interactables[i].fish->legs()) {
                    interactables[i].is_upgrading = true;
                } else if (interactables[i].type == Makeup && !interactables[i].fish->makeup()) { // If looking at makeup upgrade
                    interactables[i].is_upgrading = true;
                }

                interactables[i].fish->put_fish_below();

                held_item.reset(); // NOTE may not be necessary since we're swapping
                bn::log(bn::string<32>("successful put down"));
                ret |= 0b00000001;
                return ret;
            };

            bn::regular_bg_map_item map_item;
            bn::regular_bg_map_cell valid_cell;
            int valid_tile;
            bn::vector<Interactable, 32> interactables;

            // FishContainer fish_container;
            // bn::vector<bn::unique_ptr<Fish>, 8> fishies;
            int fish_counter;
            bool try_sell;
    };
}