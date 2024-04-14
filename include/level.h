#include "bn_core.h"
#include "bn_log.h"
#include "bn_string.h"
#include "bn_sprite_item.h"
#include "bn_regular_bg_ptr.h"
#include "bn_regular_bg_map_item.h"
#include "bn_sprite_animate_actions.h"
#include "bn_blending_actions.h"
#include "bn_utility.h"
#include "bn_timer.h"
#include "bn_format.h"
#include "bn_random.h"

#include "bn_sprite_items_turnaround32.h"

#include "bn_sprite_items_fish_item.h"
#include "bn_sprite_items_green_fish_item.h"
#include "bn_sprite_items_legs.h"
#include "bn_sprite_items_fish_makeup.h"
#include "bn_sprite_items_goal_bubble.h"
#include "bn_sprite_items_heart_animation.h"
#include "bn_sprite_items_run_away_animation.h"
#include "bn_sprite_items_sell_animation.h"
#include "bn_sprite_items_customers.h"

#include "player.h"

namespace kt {
    int lerp(int a, int b, int fraction) {
        int ret = (b - a) / fraction;
        if ((b - a) == 0) return 0;
        else if (ret != 0) return ret;
        else return 1;
    };

    struct Customer {
        int bounce_index = 0;
        int direction;
        bn::pair<int, int> pos;
        bn::sprite_ptr spr;

        Customer(int x_pos, int y_pos, int random_idx, int num) :
                    spr(bn::sprite_items::customers.create_sprite(x_pos, y_pos, random_idx)) {
            pos = bn::make_pair<int, int>(int(x_pos), int(y_pos));
            if (num % 2 == 0) direction = 1;
            else direction = -1;
        };
    };

    class Level {
        public:
            Level(bn::regular_bg_map_item map, bn::vector<FishConfig, 6> fish_con, int number_customers) :
                        player(Player(map)) {
                fish_configs = fish_con;

                // Set up goal fish sprite x positions at the top of the screen
                x_poses.push_back(72);
                x_poses.push_back(24);
                x_poses.push_back(-24);
                x_poses.push_back(-72);

                // Set up customer standing y positions
                cust_y_poses.push_back(32);
                cust_y_poses.push_back(16);
                cust_y_poses.push_back(0);
                cust_y_poses.push_back(-16);
                cust_y_poses.push_back(-32);

                // Set up customers (evens: x = 120, odds: x = 130)
                num_customers = number_customers;

                // Set up customer offsets
                // i hate this so much
                cust_offsets_forward.push_back(bn::make_pair<int, int>(1, -2));
                cust_offsets_forward.push_back(bn::make_pair<int, int>(1, -1));
                cust_offsets_forward.push_back(bn::make_pair<int, int>(2, -1));
                cust_offsets_forward.push_back(bn::make_pair<int, int>(2, 1));
                cust_offsets_forward.push_back(bn::make_pair<int, int>(1, 1));
                cust_offsets_forward.push_back(bn::make_pair<int, int>(1, 3));
                cust_offsets_forward.push_back(bn::make_pair<int, int>(1, 7));
                cust_offsets_forward.push_back(bn::make_pair<int, int>(1, 8));
                cust_offsets_backward.push_back(bn::make_pair<int, int>(-1, -2));
                cust_offsets_backward.push_back(bn::make_pair<int, int>(-1, -1));
                cust_offsets_backward.push_back(bn::make_pair<int, int>(-2, -1));
                cust_offsets_backward.push_back(bn::make_pair<int, int>(-2, 1));
                cust_offsets_backward.push_back(bn::make_pair<int, int>(-1, 1));
                cust_offsets_backward.push_back(bn::make_pair<int, int>(-1, 3));
                cust_offsets_backward.push_back(bn::make_pair<int, int>(-1, 7));
                cust_offsets_backward.push_back(bn::make_pair<int, int>(-1, 8));

                slide_fish = 4;
                slide_customer = num_customers + 1;
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

            int interact_player() {
                if (player.interact()) {
                    // Check for sell attempt
                    if (player.selling_fish()) {
                        // Check if the fish being sold matches any of the 1st 4 goal fish
                        int counter = 0;
                        for (bn::vector<FishConfig, 16>::iterator it = fish_configs.begin(); it != fish_configs.end(); it++) {
                            if (counter > 3) break;
                            if (player.get_fish_to_sell()->get_fish_config() == *it) {
                                bn::log(bn::string<32>("fiund a fish to sell match"));

                                // Sell fish
                                add_money(sell_fish(counter));

                                return money;
                            }
                            counter++;
                        }

                        // Fish does not match a goal fish
                        bn::log(bn::string<16>("fish no match"));
                        player.set_selling_fish(false);
                    }
                }
                return 0;
            };

            void kitchen_update() {
                player.kitchen_update();

                // update fish patience
                for (int i = 0; i < goal_fish_sprs.size(); i++) {
                    // only update fish patience if it's not currently being sold
                    if (!fish_configs[i].sell_in_progress) {
                        if (fish_configs[i].patience_counter <= 0) {
                            // Check if the heart animation has finished
                            if (heart_anims[i].current_index() % 7 == 0 && heart_anims[i].current_index() != ((3 - fish_configs[i].patience) * 8)) {
                                fish_configs[i].patience_counter = fish_configs[i].patience_max;
                                fish_configs[i].patience--;
                                // bump to next tile
                                heart_anims[i].update();
                                heart_anims[i].update();
                                heart_anims[i].update();
                                heart_anims[i].update();
                                heart_anims[i].update();
                                heart_anims[i].update();

                                if (fish_configs[i].patience == 1) shake_fish(i);
                                else if (fish_configs[i].patience == 0) fish_run_away(i);
                            } else if (!heart_anims[i].done()) {
                                heart_anims[i].update();
                            }
                        } else {
                            fish_configs[i].patience_counter--;
                        }
                    }
                }

                // shake bubble if need be
                for (int i = 0; i < goal_fish_sprs.size(); i++) {
                    if (shake_timers[i] % 10 == 0 && shake_timers[i] < 80 && shake_timers[i] != 0) {
                        shake_timers[i]++;

                        int x_move = 6;
                        if (goal_fish_sprs[i][0].position().x() > x_poses[i]) x_move = -6;

                        goal_fish_sprs[i][0].set_x(goal_fish_sprs[i][0].position().x() + x_move);
                    } else if (shake_timers[i] == 80) {
                        goal_fish_sprs[i][0].set_x(x_poses[i]);
                        shake_timers[i]++;
                    } else if (shake_timers[i] < 80) {
                        shake_timers[i]++;
                    }
                    
                }

                // update run anim if need be
                if (!disappear_anims.empty()) {
                    // fill fish to delete with all done run anims
                    bn::vector<int, 4> fish_to_delete;
                    for (auto f : disappear_anims) {
                        if (f.second.done()) {
                            fish_to_delete.push_back(f.first);
                            bn::log(bn::string<32>("pushed to fish to delete"));
                            bn::log(bn::format<64>("fish to delete size: {}", fish_to_delete.size()));
                        }
                    }
                    
                    bn::vector<bn::pair<int, bn::sprite_animate_action<8>>, 4>::iterator it = disappear_anims.begin();
                    while (it != disappear_anims.end()) {
                        if (it->second.done()) {
                            // delete fish and slide the rest down
                            bn::vector<FishConfig, 16>::iterator it_cfg = fish_configs.begin();
                            for (int i = 0; i < it->first; i++) {
                                it_cfg++;
                            }
                            
                            bn::log(bn::format<128>("currently erasing fish config bool={}, type={}", it_cfg->config_bool, it_cfg->fish_type));
                            fish_configs.erase(it_cfg);
                            it = disappear_anims.erase(it);

                            // from it to the end of disappear_anims, decrement it->first
                            bn::vector<bn::pair<int, bn::sprite_animate_action<8>>, 4>::iterator it2;
                            while (it2 != it) {
                                it2++;
                            }
                            while (it2 != disappear_anims.end()) {
                                it2->first--;
                                it2++;
                            }
                        } else {
                            it->second.update();
                            if (!it->second.done()) {
                                if (it->second.current_graphics_index() == 4 && goal_fish_sprs[it->first][0].position().x() >= -120) {
                                    // move fish offscreen to mimic hiding it
                                    for (auto sp : goal_fish_sprs[it->first]) {
                                        sp.set_x(-169);
                                    }
                                }
                            }
                            it++;
                        }
                    }
                    if (!fish_to_delete.empty())
                        update_fish_sprs(fish_to_delete);
                }

                // update goal fish animations
                if (sliding) {
                    int lerp_amt = 0;
                    lerp_amt = lerp(goal_fish_sprs[slide_fish][0].position().x().integer(), x_poses[slide_fish], 5);
                    for (auto sp : goal_fish_sprs[slide_fish]) {
                        sp.set_x(goal_fish_sprs[slide_fish][0].position().x() + lerp_amt);
                    }
                    bn::sprite_ptr temp_heart = heart_anims[slide_fish].sprite();
                    temp_heart.set_x(goal_fish_sprs[slide_fish][0].position().x() + lerp_amt);

                    if (lerp_amt == 0) { // aren't lerping anymore
                        slide_fish++;
                        if (slide_fish >= goal_fish_sprs.size())
                            sliding = false;
                    }
                }

                // update customer sliding (should only happen at the beginning of a level)
                if (cust_sliding) {
                    int lerp_amt = 0;
                    lerp_amt = lerp(customers[slide_customer].spr.position().y().integer(), cust_y_poses[slide_customer], 8);
                    customers[slide_customer].spr.set_y(customers[slide_customer].spr.position().y() + lerp_amt);

                    if (lerp_amt == 0) {
                        slide_customer++;
                        if (slide_customer >= customers.size())
                            cust_sliding = false;
                    }
                }

                // update customer leaving
                if (cust_leaving) {
                    int lerp_amt = 0;
                    lerp_amt = lerp(customers[0].spr.position().y().integer(), 150, 5);
                    customers[0].spr.set_y(customers[0].spr.position().y() + lerp_amt);

                    if (lerp_amt == 0) {
                        cust_leaving = false;
                        num_customers--;
                        customers.erase(customers.cbegin());
                        cust_bouncing = true;
                    }
                }

                // update customer bouncing
                if (cust_bouncing) {
                    for (int i = 0; i < num_customers; i++) {
                        if (customers[i].direction == 1)
                            customers[i].spr.set_position(customers[i].spr.position().x() + cust_offsets_forward[customers[i].bounce_index].first, customers[i].spr.position().y() + cust_offsets_forward[customers[i].bounce_index].second);
                        else
                            customers[i].spr.set_position(customers[i].spr.position().x() + cust_offsets_backward[customers[i].bounce_index].first, customers[i].spr.position().y() + cust_offsets_backward[customers[i].bounce_index].second);
                        customers[i].bounce_index++;

                        if (customers[i].bounce_index >= cust_offsets_forward.size()) {
                            customers[i].bounce_index = 0;
                            customers[i].direction *= -1;

                            if (i == num_customers - 1) {
                                cust_bouncing = false;
                            }
                        }
                    }
                }
            };

            void start_level() {
                is_started = true;

                // create timers
                for (int i = 0; i < 4; i++) {
                    shake_timers.push_back(81);
                }

                // Create goal fish sprites and slide them in
                int y_pos = -50;
                for (int i = 0; i < 4; i++) {
                    create_fish_spr_from_config(fish_configs[i], -140, y_pos);
                }
                slide_fish = 0;
                sliding = true;

                // Create customers and slide them in
                for (int i = 0; i < num_customers; i++) {
                    create_customer(i);
                }
                slide_customer = 0;
                cust_sliding = true;
            };

            int sell_fish(int index) {
                disappear_anims.push_back(bn::make_pair<int, bn::sprite_animate_action<8>>(int(index), bn::create_sprite_animate_action_once(bn::sprite_items::sell_animation.create_sprite(x_poses[index], -50), 7,
                    bn::sprite_items::sell_animation.tiles_item(), 0, 1, 2, 3, 4, 5, 6, 7)));
                fish_configs[index].sell_in_progress = true;
                cust_leaving = true;
                player.sell_fish();
                return 45;
            };

            void add_money(int mun) {
                money += mun;
            };

            void create_fish_spr_from_config(FishConfig config, int x_pos, int y_pos) {
                bn::vector<bn::sprite_ptr, 16> curr_fish;
                bn::sprite_ptr bubble = bn::sprite_items::goal_bubble.create_sprite(x_pos, y_pos);
                bubble.put_below();
                curr_fish.push_back(bubble);
                switch (config.fish_type) {
                    case Purple:
                        {
                            bn::log(bn::string<16>("ourple"));
                            bn::sprite_ptr fish_spr = bn::sprite_items::fish_item.create_sprite(x_pos, y_pos);
                            curr_fish.push_back(fish_spr);
                            break;
                        }
                    case Green:
                        {
                            bn::log(bn::string<16>("gween"));
                            bn::sprite_ptr fish_spr = bn::sprite_items::green_fish_item.create_sprite(x_pos, y_pos);
                            curr_fish.push_back(fish_spr);
                            break;
                        }
                    default:
                        bn::log(bn::string<32>("switch fucked up"));
                        return;
                }

                // add heart animation
                bn::sprite_animate_action<22> heart_anim = bn::create_sprite_animate_action_once(bn::sprite_items::heart_animation.create_sprite(x_pos, y_pos - 13), 5, bn::sprite_items::heart_animation.tiles_item(), 
                                                                                                0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21);
                heart_anims.push_back(heart_anim);

                // Check for upgrades
                // TO-DO maybe add the other types but idk
                // Legs?
                if (config.config_bool & (1 << 7)) {
                    bn::sprite_ptr upgrade = bn::sprite_items::legs.create_sprite(x_pos, y_pos);
                    curr_fish.push_back(upgrade);
                }

                // Makeup?
                if (config.config_bool & (1 << 5)) curr_fish.push_back(bn::sprite_items::fish_makeup.create_sprite(x_pos, y_pos));

                goal_fish_sprs.push_back(curr_fish);
            };

            void update_fish_sprs(bn::vector<int, 4> counters) {
                bn::vector<int, 4> del_counters = bn::vector<int, 4>(counters);
                for (int i = 0; i < del_counters.size(); i++) {
                    bn::vector<bn::vector<bn::sprite_ptr, 16>, 10>::iterator it = goal_fish_sprs.begin();
                    for (int j = 0; j < del_counters[i]; j++) {
                        it++;
                    }
                    // Found correct fish sprite to delete
                    // Remove all sprites
                    it->clear();
                    goal_fish_sprs.erase(it);

                    // do it again for the heart anims
                    bn::vector<bn::sprite_animate_action<22>, 16>::iterator it2 = heart_anims.begin();
                    for (int k = 0; k < del_counters[i]; k++) {
                        it2++;
                    }
                    heart_anims.erase(it2);

                    // decrement the rest of del_counters (because of shift)
                    for (int m = i + 1; m < del_counters.size(); m++) {
                        del_counters[m]--;
                    }
                }
                if (!fish_configs.empty() && counters[0] < fish_configs.size()) {
                    if (!sliding) {
                        slide_fish = counters[0];
                    }
                    sliding = true;
                }

                // Add in new fish (if applicable)
                for (int counter : counters) {
                    if (fish_configs.size() >= 4) {
                        bn::log(bn::format<128>("creating fish sprite @ {}, which is {}", counter, fish_configs[counter].to_string()));
                        create_fish_spr_from_config(fish_configs[counter], -140, -50);
                    }
                }
            };

            void create_customer(int i) {
                int spr_index = rand.get_int(3);
                customers.push_back(Customer(100 + (10 * (i % 2)), -120, spr_index, i));
            };

            void shake_fish(int index) {
                // change bubble to the red one
                goal_fish_sprs[index][0].set_tiles(bn::sprite_items::goal_bubble.tiles_item().create_tiles(1));
                goal_fish_sprs[index][0].set_x(x_poses[index] - 3);
                shake_timers[index] = 0;
            };

            void fish_run_away(int index) {
                // start run away animation
                disappear_anims.push_back(bn::make_pair<int, bn::sprite_animate_action<8>>(int(index), bn::create_sprite_animate_action_once(bn::sprite_items::run_away_animation.create_sprite(x_poses[index], -50), 7,
                                    bn::sprite_items::run_away_animation.tiles_item(), 0, 1, 2, 3, 4, 5, 6, 7)));
            };

            void print_goal_fish() {
                bn::log(bn::string<32>("----- goal fish sprs -----"));
                bn::log(bn::to_string<16>(goal_fish_sprs.size()));
                for (int i = 0; i < goal_fish_sprs.size(); i++) {
                    bn::log(bn::to_string<16>(goal_fish_sprs[i][0].position().x()));
                    // bn::log(bn::to_string<16>(goal_fish_sprs[i][0].))
                }
                bn::log(bn::string<32>("----- fish configs -----"));
                bn::log(bn::to_string<16>(fish_configs.size()));
                for (int i = 0; i < fish_configs.size(); i++) {
                    bn::log(fish_configs[i].to_string());
                }
            };

        private:
            Player player;
            bool is_started = false;
            bool sliding = false;
            bool cust_sliding = false;
            bool cust_leaving = false;
            bool cust_bouncing = false;

            bn::vector<bn::pair<int, bn::sprite_animate_action<8>>, 4> disappear_anims;

            bn::vector<FishConfig, 16> fish_configs;
            bn::vector<bn::sprite_animate_action<22>, 16> heart_anims;
            bn::vector<bn::vector<bn::sprite_ptr, 16>, 10> goal_fish_sprs;
            bn::vector<int, 4> x_poses;
            bn::vector<int, 4> shake_timers;
            bn::vector<int, 5> cust_y_poses;
            bn::vector<Customer, 25> customers;
            bn::vector<bn::pair<int, int>, 8> cust_offsets_forward;
            bn::vector<bn::pair<int, int>, 8> cust_offsets_backward;

            bn::random rand;

            int slide_fish;
            int slide_customer;
            int num_customers;
            int money;
            int fish_spacing = 32;
    };
}