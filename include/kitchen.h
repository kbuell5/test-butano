#include "bn_core.h"
#include "bn_log.h"
#include "bn_string.h"
#include "bn_regular_bg_ptr.h"
#include "bn_regular_bg_map_item.h"
#include "bn_vector.h"
#include "bn_utility.h"
#include "bn_sprite_animate_actions.h"
#include "bn_optional.h"

#include "bn_regular_bg_items_map_interactive.h"
#include "bn_sprite_items_piechart.h"

#include "fish_container.h"

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
            Interactable(int min_x_cell, int max_x_cell, bool interact, bool has_a_fish, CellType type_of_cell, Fish* fish_object, int center_x, int center_y) :
                        pie_chart_ptr(bn::sprite_items::piechart.create_sprite(center_x, center_y)),
                        pie_anim(bn::create_sprite_animate_action_once(pie_chart_ptr, 20, bn::sprite_items::piechart.tiles_item(), 1, 2, 3, 4, 5, 6, 7, 8, 9)) {
                min_x = min_x_cell;
                max_x = max_x_cell;
                is_interactable = interact;
                has_fish = has_a_fish;
                type = type_of_cell;
                fish = fish_object;
                center = bn::point(center_x, center_y);
            };

            void do_action() {
                if (type == Butterfly) {
                    fish->give_legs();
                    bn::log(bn::string<32>("gave fish legs (in interactable)"));
                } else if (type == Makeup) {
                    fish->give_makeup();
                    bn::log(bn::string<32>("gave fish makeup"));
                }
            };

            int min_x;
            int max_x;
            bool is_interactable = false;
            bool is_upgrading = false;
            bool has_fish = false;
            CellType type;
            Fish* fish = nullptr;
            bn::point center;
            bn::sprite_ptr pie_chart_ptr;
            bn::sprite_animate_action<9> pie_anim;
    };

    class Kitchen {
        public:
            Kitchen(bn::regular_bg_map_item map) : map_item(map), valid_cell(map_item.cell(0, 0)) {
                // valid_cell = map_item.cell(0, 0);
                valid_tile = bn::regular_bg_map_cell_info(valid_cell).tile_index();
                // ---------- Define interactable tiles
                int i = 1;
                for (i = 1; i <= 8; i++) {
                    Interactable fishtank_inter(1, 8, true, true, FishTank, nullptr, -32, -24);
                    interactables.push_back(bn::pair<int, Interactable>(bn::regular_bg_map_cell_info(map_item.cell(i, 0)).tile_index(), fishtank_inter));
                }
                for (i = 9; i <= 12; i++) {
                    Interactable butterfly_inter(9, 12, true, false, Butterfly, nullptr, -8, -24);
                    interactables.push_back(bn::pair<int, Interactable>(bn::regular_bg_map_cell_info(map_item.cell(i, 0)).tile_index(), butterfly_inter));
                }
                for (i = 13; i <= 16; i++) {
                    Interactable timer_inter(13, 16, true, false, Timer, nullptr, 24, -24);
                    interactables.push_back(bn::pair<int, Interactable>(bn::regular_bg_map_cell_info(map_item.cell(i, 0)).tile_index(), timer_inter));
                }
                for (i = 17; i <= 20; i++) {
                    Interactable green_fishtank_inter(17, 20, true, true, GreenFishTank, nullptr, -56, 8);
                    interactables.push_back(bn::pair<int, Interactable>(bn::regular_bg_map_cell_info(map_item.cell(i, 0)).tile_index(), green_fishtank_inter));
                }
                for (i = 21; i <= 24; i++) {
                    Interactable makeup_inter(21, 24, true, false, Makeup, nullptr, 40, -8);
                    interactables.push_back(bn::pair<int, Interactable>(bn::regular_bg_map_cell_info(map_item.cell(i, 0)).tile_index(), makeup_inter));
                }
                for (i = 25; i <= 28; i++) {
                    Interactable customer_inter(25, 28, true, false, Customer, nullptr, 40, 24);
                    interactables.push_back(bn::pair<int, Interactable>(bn::regular_bg_map_cell_info(map_item.cell(i, 0)).tile_index(), customer_inter));
                }
            };

            int valid_tile_index() {
                return valid_tile;
            };

            uint8_t interact(int search_index, Fish *&held_item) {
                uint8_t interact_int = 0b00000000;
                for (int i = 0; i < interactables.size(); i++) {
                    // If we've found the interactable we've collided with
                    if (interactables[i].first == search_index) {
                        bn::log(bn::string<32>("found interactable"));
                        // Is it interactable at the moment?
                        if (interactables[i].second.is_interactable == false) return interact_int;

                        // Pick up, not holding a fish
                        if (held_item == nullptr) {
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
                        return interact_int;
                    }
                }
                bn::log(bn::string<32>("interaction failed"));
                return false;
            };

            void update() {
                for (bn::vector<bn::pair<int, Interactable>, 64>::iterator it = interactables.begin(); it != interactables.end(); it++) {
                    if (it->second.is_upgrading && !it->second.pie_anim.done()) {
                        it->second.pie_anim.update();
                        if (it->second.pie_anim.done()) {
                            it->second.is_upgrading = false;
                            it->second.pie_anim.reset();
                            it->second.pie_anim.update();
                            it->second.pie_chart_ptr.set_tiles(bn::sprite_items::piechart.tiles_item().create_tiles(0));

                            it->second.do_action();
                        }
                    }
                }
            };

        private:
            bool _pick_up(int i, Fish *&held_item) {
                if (interactables[i].second.type == FishTank) {
                    held_item = fish_container.add_fish(Purple);
                    return true;
                }

                if (interactables[i].second.type == GreenFishTank) {
                    held_item = fish_container.add_fish(Green);
                    return true;
                }

                // If interactable does not have a fish to pick up
                if (interactables[i].second.has_fish == false) return false;

                // If interactable is currently upgrading a fish
                if (interactables[i].second.is_upgrading) {
                    interactables[i].second.is_upgrading = false;
                    interactables[i].second.pie_anim.reset();
                    interactables[i].second.pie_anim.update();
                    interactables[i].second.pie_chart_ptr.set_tiles(bn::sprite_items::piechart.tiles_item().create_tiles(0));
                }
                
                held_item = interactables[i].second.fish;
                bn::log(bn::string<64>("new address for held_item: "));
                bn::log(bn::to_string<32>(held_item));
                // Update flags of current cell
                interactables[i].second.has_fish = false;
                interactables[i].second.fish = nullptr;
                bn::log(bn::string<64>("maybe we fucked up: "));
                bn::log(bn::to_string<32>(held_item));


                // Update flags of associated cells
                // NOTE this only works because i've hardcoded the vector values to be sorted based on x value
                for (int j = i - 1; j >= interactables[i].second.min_x - 1; j--) {
                    interactables[j].second.has_fish = false;
                    interactables[j].second.fish = nullptr;
                }
                for (int k = i + 1; k <= interactables[i].second.max_x - 1; k++) {
                    interactables[k].second.has_fish = false;
                    interactables[k].second.fish = nullptr;
                }
                held_item->put_fish_above();
                bn::log(bn::string<32>("successful pick up"));
                return true;
            };

            uint8_t _put_down(int i, Fish *&held_item) {
                // If interactable already has a fish on it
                // TODO special cases: [ ] fishtank can have fish and also be given back a normal fish
                                    // [ ] fishtank can have a fish but not be given a modified fish
                                    // [x] trash can deletes fish, never "has" a fish
                if ((interactables[i].second.type == FishTank && held_item->get_fish_type() == Purple) || 
                    (interactables[i].second.type == GreenFishTank && held_item->get_fish_type() == Green)) {
                    if (held_item->is_basic()) {
                        fish_container.delete_fish(held_item->get_fish_id());
                        held_item = nullptr;
                        bn::log(bn::string<32>("fish put back :3"));
                        return 0b00000001;
                    } else {
                        bn::log(bn::string<48>("tried to put modified fish in tank >:3"));
                        return 0b00000000;
                    }
                }

                if (interactables[i].second.has_fish) {
                    return 0b00000000;
                }
                
                // If looking at the timer (trash can)
                if (interactables[i].second.type == Timer) {
                    fish_container.delete_fish(held_item->get_fish_id());
                    held_item = nullptr;
                    bn::log(bn::string<32>("frish garbaggio'd"));
                    // bn::log(bn::string<32>("fish id"));
                    // bn::log(bn::to_string<16>(held_item->get_fish_id()));
                    return 0b00000001;
                }

                uint8_t ret = 0b00000000;
                // If looking at customer (sell point)
                if (interactables[i].second.type == Customer) {
                    // fish_container.delete_fish(held_item->get_fish_id());
                    // held_item = nullptr;
                    bn::log(bn::string<32>("attempting to sell fihs"));
                    ret |= 0b00000010;
                }

                interactables[i].second.fish = held_item;
                interactables[i].second.has_fish = true;

                for (int j = i - 1; j >= interactables[i].second.min_x - 1; j--) {
                    interactables[j].second.has_fish = true;
                    interactables[i].second.fish = held_item;
                }
                for (int k = i + 1; k <= interactables[i].second.max_x - 1; k++) {
                    interactables[k].second.has_fish = true;
                    interactables[k].second.fish = held_item;
                }
                interactables[i].second.fish->update_fish_location(interactables[i].second.center.x(), interactables[i].second.center.y());

                // If looking at butterfly (legs upgrade)
                if (interactables[i].second.type == Butterfly && !interactables[i].second.fish->legs()) {
                    interactables[i].second.is_upgrading = true;
                }

                // If looking at makeup upgrade
                if (interactables[i].second.type == Makeup && !interactables[i].second.fish->makeup()) {
                    interactables[i].second.is_upgrading = true;
                }

                interactables[i].second.fish->put_fish_below();

                held_item = nullptr;
                bn::log(bn::string<32>("successful put down"));
                ret |= 0b00000001;
                return ret;
            };

            bn::regular_bg_map_item map_item;
            bn::regular_bg_map_cell valid_cell;
            int valid_tile;
            bn::vector<bn::pair<int, Interactable>, 64> interactables;

            FishContainer fish_container;
    };
}