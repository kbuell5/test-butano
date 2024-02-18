#include "bn_core.h"
#include "bn_log.h"
#include "bn_string.h"
#include "bn_sprite_item.h"
#include "bn_regular_bg_ptr.h"
#include "bn_regular_bg_map_item.h"
#include "bn_sprite_animate_actions.h"
#include "bn_blending_actions.h"

#include "bn_sprite_items_turnaround32.h"

#include "bn_sprite_items_fish_item.h"
#include "bn_sprite_items_green_fish_item.h"
#include "bn_sprite_items_legs.h"
#include "bn_sprite_items_fish_makeup.h"

#include "player.h"

namespace kt {
    int lerp(int a, int b, int fraction) {
        int ret = (b - a) / fraction;
        if ((b - a) == 0) return 0;
        else if (ret != 0) return ret;
        else return 1;
    };

    class Level {
        public:
            Level(bn::regular_bg_map_item map, bn::vector<FishConfig, 6> fish_con) :
                        player(Player(map)) {
                fish_configs = fish_con;

                // Set up fish sprites at the top of the screen
                x_poses.push_back(72);
                x_poses.push_back(24);
                x_poses.push_back(-24);
                x_poses.push_back(-72);

                slide_fish.push_back(false);
                slide_fish.push_back(false);
                slide_fish.push_back(false);
                slide_fish.push_back(false);
            
                int y_pos = -50;

                bn::blending::set_intensity_alpha(0.0);

                for (int i = 0; i < 4; i++) {
                    create_fish_spr_from_config(fish_configs[i], x_poses[i], y_pos);
                }
            };

            bool is_level_started() {
                return is_started;
            }

            void move_player_up() {
                player.move_up();
                player.update_walk();
            };

            void move_player_down() {
                player.move_down();
                player.update_walk();
            };

            void move_player_left() {
                player.move_left();
                player.update_walk();
            };

            void move_player_right() {
                player.move_right();
                player.update_walk();
            };

            void interact_player() {
                if (player.interact()) {
                    bn::log(bn::string<16>("pierogies"));
                    // Check for sell attempt
                    if (player.selling_fish()) {
                        bn::log(bn::string<32>("selling a fsh mayebe"));
                        // Check if the fish being sold matches any of the 1st 4 goal fish
                        int counter = 0;
                        for (bn::vector<FishConfig, 6>::iterator it = fish_configs.begin(); it != fish_configs.end(); it++) {
                            if (counter > 3) break;
                            if (player.get_fish_to_sell()->get_fish_config() == *it) {
                                bn::log(bn::string<32>("fiund a fish to sell match"));

                                // Sell fish
                                // TODO confetti animation
                                add_money(sell_fish(it, counter));

                                return;
                            }
                            counter++;
                        }

                        // Fish does not match a goal fish
                        bn::log(bn::string<16>("fish no match"));
                        player.set_selling_fish(false);
                        // TODO animate the fish
                    }
                }
            };

            void kitchen_update() {
                player.kitchen_update();

                // update goal fish animations
                if (sliding) {
                    int lerp_amt = 0;
                    for (int i = 0; i < slide_fish.size(); i++) {
                        if (slide_fish[i]) {
                            lerp_amt = lerp(goal_fish_sprs[i][0].position().x().integer(), x_poses[i], 5);
                            bn::log(bn::to_string<16>(lerp_amt));
                            goal_fish_sprs[i][0].set_x(goal_fish_sprs[i][0].position().x() + lerp_amt);
                        }
                    }

                    if (lerp_amt == 0) {
                        for (int j = 0; j < slide_fish.size(); j++) {
                            if (slide_fish[j]) {
                                goal_fish_sprs[j][0].set_x(x_poses[j]);
                                slide_fish[j] = false;
                            }
                        }
                        sliding = false;
                    }
                }
            };

            void start_level() {
                is_started = true;

            };

            int sell_fish(FishConfig* it, int counter) {
                fish_configs.erase(it);
                update_fish_sprs(counter);
                player.sell_fish();
                return 45;
            };

            void add_money(int mun) {
                money += mun;
            };

            void create_fish_spr_from_config(FishConfig config, int x_pos, int y_pos) {
                bn::vector<bn::sprite_ptr, 5> curr_fish;
                bn::sprite_ptr test_fish = bn::sprite_items::legs.create_sprite(x_pos, y_pos);
                bn::log(bn::to_string<16>(x_pos));
                switch (config.fish_type) {
                    case Purple:
                        {
                            bn::log(bn::string<32>("ourple"));
                            bn::sprite_ptr sprp = bn::sprite_items::fish_item.create_sprite(x_pos, y_pos);
                            curr_fish.push_back(sprp);
                            break;
                        }
                    case Green:
                        {
                            bn::log(bn::string<32>("greeb"));
                            bn::sprite_ptr sprg = bn::sprite_items::green_fish_item.create_sprite(x_pos, y_pos);
                            curr_fish.push_back(sprg);
                            break;
                        }
                    default:
                        bn::log(bn::string<32>("switch fucked up"));
                        break;
                }

                // Check for upgrades
                // TODO maybe add the other types but idk
                // Legs?
                if (config.config_bool & (1 << 7)) {
                    bn::sprite_ptr upgrade = bn::sprite_items::legs.create_sprite(x_pos, y_pos);
                    curr_fish.push_back(upgrade);
                }

                // Makeup?
                if (config.config_bool & (1 << 5)) curr_fish.push_back(bn::sprite_items::fish_makeup.create_sprite(x_pos, y_pos));

                goal_fish_sprs.push_back(curr_fish);
            };

            void update_fish_sprs(int counter) {
                bn::vector<bn::vector<bn::sprite_ptr, 5>, 10>::iterator it = goal_fish_sprs.begin();
                for (int i = 0; i < counter; i++) {
                    it++;
                }
                // Found correct fish sprite to delete
                // Remove all sprites
                it->clear();
                goal_fish_sprs.erase(it);

                // Move other fish down accordingly
                // TO-DO make this incremental over time so they slide
                // for (int i = counter; i < 3; i++) {
                //     bn::log(bn::to_string<16>(i));
                //     if (goal_fish_sprs.size() > i) {
                //         for (bn::vector<bn::sprite_ptr, 5>::iterator it_spr = goal_fish_sprs[i].begin(); 
                //                 it_spr != goal_fish_sprs[i].end(); 
                //                 it_spr++) {
                //             it_spr->set_x(it_spr->position().x() + 48);
                //         }
                //     }
                // }

                // WIP sliding them down
                for (int i = counter; i < 3; i++) {
                    slide_fish[i] = true;
                }
                sliding = true;

                // Add in new fish (if applicable)
                if (fish_configs.size() >= 4) {
                    create_fish_spr_from_config(fish_configs[counter], -72, -50);
                }
            };

            void show_goal_fish() {

            };

            void goal_fish_wrong() {
                
                
            };

        private:
            Player player;
            bool is_started = false;
            bool sliding = false;

            bn::vector<FishConfig, 6> fish_configs;
            bn::vector<bn::vector<bn::sprite_ptr, 5>, 10> goal_fish_sprs;
            bn::vector<int, 4> x_poses;
            bn::vector<bool, 4> slide_fish;


            int num_customers;
            int money;
            int fish_spacing = 32;
    };
}