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
                for (bn::vector<kt::Fish, 32>::iterator it = fishies.begin(); it != fishies.end(); it++) {
                    if (it->get_fish_id() == del_id) {
                        bn::log(bn::string<32>("current fishies count: "));
                        bn::log(bn::to_string<16>(fishies.size()));
                        bn::log(bn::string<16>("fish id: "));
                        bn::log(bn::to_string<16>(it->get_fish_id()));
                        fishies.erase(it);
                        bn::log(bn::string<16>("fish GONE"));
                        bn::log(bn::string<16>("fish id: "));
                        bn::log(bn::to_string<16>(it->get_fish_id()));
                        bn::log(bn::string<32>("after fishies count: "));
                        bn::log(bn::to_string<16>(fishies.size()));
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