#include "bn_core.h"
#include "bn_log.h"
#include "bn_string.h"
#include "bn_regular_bg_ptr.h"
#include "bn_regular_bg_map_item.h"
#include "bn_vector.h"
#include "bn_utility.h"

#include "bn_regular_bg_items_map_interactive.h"

#include "fish.h"

namespace kt {
    enum CellType {
        FishTank,
        Butterfly,
        Timer
    };

    class Interactable {
        public:
            Interactable(int number_cells, bool interact, bool has_a_fish, CellType type_of_cell, Fish* fish_object) {
                num_cells = number_cells;
                is_interactable = interact;
                has_fish = has_a_fish;
                type = type_of_cell;
                fish = fish_object;
            }

            int num_cells;
            bool is_interactable = false;
            bool has_fish = false;
            CellType type;
            Fish* fish = nullptr;
    };

    class Kitchen {
        public:
            Kitchen(bn::regular_bg_item map) : map_item(map.map_item()), valid_cell(map_item.cell(0, 0)) {
                // valid_cell = map_item.cell(0, 0);
                valid_tile = bn::regular_bg_map_cell_info(valid_cell).tile_index();
                // ---------- Define interactable tiles
                for (int i = 1; i <= 8; i++) {
                    Interactable fishtank_inter(8, true, true, FishTank, nullptr);
                    interactables.push_back(bn::pair<bn::regular_bg_map_cell, Interactable>(map_item.cell(0, i), fishtank_inter));
                }
                for (int j = 9; j <= 12; j++) {
                    Interactable butterfly_inter(4, true, false, Butterfly, nullptr);
                    interactables.push_back(bn::pair<bn::regular_bg_map_cell, Interactable>(map_item.cell(0, j), butterfly_inter));

                }
                for (int k = 13; k <= 16; k++) {
                    Interactable timer_inter(4, true, false, Timer, nullptr);
                    interactables.push_back(bn::pair<bn::regular_bg_map_cell, Interactable>(map_item.cell(0, k), timer_inter));
                }
            };

            int valid_tile_index() {
                return valid_tile;
            };


        private:
            bn::regular_bg_map_item map_item;
            bn::regular_bg_map_cell valid_cell;
            int valid_tile;
            bn::vector<bn::pair<bn::regular_bg_map_cell, Interactable>, 64> interactables;
    };
}