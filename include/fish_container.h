#include "bn_core.h"
#include "bn_log.h"
#include "bn_string.h"
#include "bn_vector.h"
#include "bn_cstring.h"
#include "bn_unique_ptr.h"

#include "fish.h"

namespace kt {
    class FishContainer {
        public:
            FishContainer() {
                try_sell = false;
            };

            bn::unique_ptr<Fish>& add_fish(FishType fish_type) {
                bn::log(bn::string<16>("gogogogo"));
                // Fish * new_fish = new Fish(fish_type);
                bn::unique_ptr<Fish> new_fish = bn::make_unique<Fish>(fish_type);
                bn::log(bn::string<16>("gogogogo"));
                // fishies.push_back(new_fish);
                fishies.push_back({});
                fishies.back().swap(new_fish);
                if (fishies.back()) {
                    bn::log(bn::string<16>("hoohah"));
                }
                bn::log(bn::string<16>("gogogogo"));
                return fishies.back();
            };

            bool delete_fish(int del_id) {
                // for (bn::vector<kt::Fish*, 32>::iterator it = fishies.begin(); it != fishies.end(); it++) {
                //     if ((*it)->get_fish_id() == del_id) {
                //         (*it)->delete_fish();
                //         fishies.erase(it);
                //         bn::log(bn::string<16>("fish GONE"));
                //         return true;
                //     }
                // }
                for (int i = 0; i < fishies.size(); i++) {
                    if (fishies[i]->get_fish_id() == del_id) {
                        // delete fishies[i];
                        fishies.erase(fishies.begin() + i);
                        bn::log(bn::string<16>("fish GONE"));
                        return true;
                    }
                }
                bn::log(bn::string<16>("no fish deleted"));
                return false;
            };

            void print_fish() {
                bn::log(bn::string<16>("~~~~~~~~"));
                bn::log(bn::to_string<16>(fishies.size()));
                for (auto& f : fishies) {
                    if (f) bn::log(bn::string<16>("exists"));
                    else bn::log(bn::string<16>("doesn't"));
                }
                bn::log(bn::string<16>("~~~~~~~~"));
                bn::log(bn::string<16>("==============="));
                // for (bn::vector<bn::unique_ptr<Fish>, 32>::iterator it = fishies.begin(); it != fishies.end(); it++) {
                //     bn::log(bn::to_string<16>((*it)->get_fish_id()));
                //     bn::log(bn::to_string<16>((*it)->get_fish_config_bool()));
                //     bn::log(bn::to_string<16>((*it)->get_fish_type()));
                //     bn::log(bn::string<16>("==============="));
                // }
                bn::log(bn::to_string<16>(fishies.size()));

                for ( auto& f : fishies) {
                    bn::log(bn::to_string<16>(f->get_fish_id()));
                    bn::log(bn::to_string<16>(f->get_fish_config_bool()));
                    bn::log(bn::to_string<16>(f->get_fish_type()));
                    bn::log(bn::string<16>("==============="));
                }
            };

            bool try_sell;
        private:
            bn::vector<bn::unique_ptr<Fish>, 8> fishies;
    };
}