#include "bn_core.h"
#include "bn_log.h"
#include "bn_string.h"
#include "bn_vector.h"
#include "bn_cstring.h"

#include "fish.h"

namespace kt {
    class FishContainer {
        public:
            FishContainer() {
                try_sell = false;
            };

            Fish* add_fish(FishType fish_type) {
                Fish * new_fish = new Fish(fish_type);
                fishies.push_back(*new_fish);
                return new_fish;
            };

            bool delete_fish(int del_id) {
                for (bn::vector<kt::Fish, 32>::iterator it = fishies.begin(); it != fishies.end(); it++) {
                    if (it->get_fish_id() == del_id) {
                        it->delete_fish();
                        fishies.erase(it);
                        bn::log(bn::string<16>("fish GONE"));
                        return true;
                    }
                }
                bn::log(bn::string<16>("no fish deleted"));
                return false;
            };

            bool try_sell;
        private:
            bn::vector<Fish, 32> fishies;
    };
}