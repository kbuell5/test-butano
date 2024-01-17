#include "bn_core.h"
#include "bn_log.h"
#include "bn_string.h"
#include "bn_regular_bg_ptr.h"
#include "bn_regular_bg_map_item.h"
#include "bn_vector.h"
#include "bn_utility.h"

#include "bn_regular_bg_items_map_interactive.h"

// #include "fish.h"
#include "fish_container.h"

namespace kt {
    enum CellType {
        FishTank,
        Butterfly,
        Timer
    };

    class Interactable {
        public:
            Interactable(int min_x_cell, int max_x_cell, bool interact, bool has_a_fish, CellType type_of_cell, Fish* fish_object) {
                min_x = min_x_cell;
                max_x = max_x_cell;
                is_interactable = interact;
                has_fish = has_a_fish;
                type = type_of_cell;
                fish = fish_object;
            }

            int min_x;
            int max_x;
            bool is_interactable = false;
            bool has_fish = false;
            CellType type;
            Fish* fish = nullptr;
    };

    class Kitchen {
        public:
            Kitchen(bn::regular_bg_map_item map) : map_item(map), valid_cell(map_item.cell(0, 0)) {
                // valid_cell = map_item.cell(0, 0);
                valid_tile = bn::regular_bg_map_cell_info(valid_cell).tile_index();
                // ---------- Define interactable tiles
                int i = 1;
                Fish * tank_fish = fish_container.add_fish();
                for (i = 1; i <= 8; i++) {
                    Interactable fishtank_inter(1, 8, true, true, FishTank, tank_fish);
                    interactables.push_back(bn::pair<int, Interactable>(bn::regular_bg_map_cell_info(map_item.cell(i, 0)).tile_index(), fishtank_inter));
                }
                for (i = 9; i <= 12; i++) {
                    Interactable butterfly_inter(9, 12, true, false, Butterfly, nullptr);
                    interactables.push_back(bn::pair<int, Interactable>(bn::regular_bg_map_cell_info(map_item.cell(i, 0)).tile_index(), butterfly_inter));

                }
                for (i = 13; i <= 16; i++) {
                    Interactable timer_inter(13, 16, true, false, Timer, nullptr);
                    interactables.push_back(bn::pair<int, Interactable>(bn::regular_bg_map_cell_info(map_item.cell(i, 0)).tile_index(), timer_inter));
                }
            };

            int valid_tile_index() {
                return valid_tile;
            };

            bool interact(int search_index, Fish *&held_item) {
                for (int i = 0; i < interactables.size(); i++) {
                    // If we've found the interactable we've collided with
                    if (interactables[i].first == search_index) {
                        bn::log(bn::string<32>("found interactable"));
                        // Is it interactable at the moment?
                        if (interactables[i].second.is_interactable == false) return false;

                        // Pick up, not holding a fish
                        if (held_item == nullptr) {
                            // If interactable does not have a fish to pick up
                            if (interactables[i].second.has_fish == false) return false;
                            // _pick_up(search_index, i, &held_item);
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
                            bn::log(bn::string<32>("successful pick up"));
                            return true;
                        } else { // Put down, holding a fish
                            // If interactable already has a fish on it
                            // TODO special cases: [ ] fishtank can have fish and also be given back a normal fish
                                                // [ ] fishtank can have a fish but not be given a modified fish
                                                // [ ] trash can deletes fish, never "has" a fish
                            if (interactables[i].second.has_fish) return false;
                            // _put_down(search_index, i, &held_item);
                            // If looking at the timer (trash can)
                            if (interactables[i].second.type == Timer) {
                                // bn::log(bn::string<16>("fish id? "));
                                fish_container.delete_fish(held_item->get_fish_id());
                                held_item = nullptr;
                                bn::log(bn::string<32>("frish garbaggio'd"));
                                return true;
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

                            held_item = nullptr;
                            bn::log(bn::string<32>("successful put down"));
                            return true;
                        }
                    }
                }
                bn::log(bn::string<32>("interaction failed"));
                return false;
            };

        private:
            bool _pick_up(int search_index, int i, Fish *&held_item) {
                // TODO put in here
                return false;
            };

            bool _put_down(int search_index, int i, Fish* held_item) {
                // held_item = Fish(interactables[i)
                // Update flags of current cell
                return false;
            };

            bn::regular_bg_map_item map_item;
            bn::regular_bg_map_cell valid_cell;
            int valid_tile;
            bn::vector<bn::pair<int, Interactable>, 64> interactables;

            FishContainer fish_container;
    };
}