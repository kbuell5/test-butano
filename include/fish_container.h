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

            Fish* add_fish() {
                Fish * new_fish = new Fish();
                fishies.push_back(*new_fish);
                return new_fish;
            };

            bool delete_fish(int del_id) {
                for (int i = 0; i < fishies.size(); i++) {
                    bn::log(bn::to_string<16>(fishies[i].get_fish_id()));
                    bn::log(bn::to_string<16>(del_id));
                    if (fishies[i].get_fish_id() == del_id) {
                        // fishies.erase(fishies[i]);
                        bn::log(bn::string<16>("fish address: "));
                        bn::log(bn::to_string<16>(fishies[i].get_fish_id()));
                        delete &fishies[i];
                        // bn::memcpy(&fishies[i], &balls, sizeof(fishies[i]));
                        bn::log(bn::string<16>("fish GONE"));
                        bn::log(bn::string<16>("fish address: "));
                        bn::log(bn::to_string<16>(fishies[i].get_fish_id()));
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