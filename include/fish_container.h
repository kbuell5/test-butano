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
                balls = 0;
            };

            Fish* add_fish(FishType fish_type) {
                Fish * new_fish = new Fish(fish_type);
                fishies.push_back(*new_fish);
                return new_fish;
            };

            bool delete_fish(int del_id) {
                for (bn::vector<kt::Fish, 32>::iterator it = fishies.begin(); it != fishies.end(); it++) {
                    if (it->get_fish_id() == del_id) {
                        fishies.erase(it);
                        bn::log(bn::string<16>("fish GONE"));
                        return true;
                    }
                }
                bn::log(bn::string<16>("no fish deleted"));
                return false;
            };
        private:
            bn::vector<Fish, 32> fishies;
            int balls;
    };
}