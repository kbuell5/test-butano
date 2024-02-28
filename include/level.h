#include "bn_core.h"
#include "bn_log.h"
#include "bn_string.h"
#include "bn_sprite_item.h"
#include "bn_regular_bg_ptr.h"
#include "bn_regular_bg_map_item.h"
#include "bn_sprite_animate_actions.h"
#include "bn_blending_actions.h"
#include "bn_utility.h"

#include "bn_sprite_items_turnaround32.h"

#include "bn_sprite_items_fish_item.h"
#include "bn_sprite_items_green_fish_item.h"
#include "bn_sprite_items_legs.h"
#include "bn_sprite_items_fish_makeup.h"
#include "bn_sprite_items_goal_bubble.h"
#include "bn_sprite_items_heart_animation.h"

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

                // Set up goal fish sprite x positions at the top of the screen
                x_poses.push_back(72);
                x_poses.push_back(24);
                x_poses.push_back(-24);
                x_poses.push_back(-72);

                slide_fish = 4;

                // bn::blending::set_intensity_alpha(0.0);
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
                        for (bn::vector<FishConfig, 16>::iterator it = fish_configs.begin(); it != fish_configs.end(); it++) {
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
                    lerp_amt = lerp(goal_fish_sprs[slide_fish][0].position().x().integer(), x_poses[slide_fish], 5);
                    for (auto sp : goal_fish_sprs[slide_fish]) {
                        sp.set_x(goal_fish_sprs[slide_fish][0].position().x() + lerp_amt);
                    }
                    bn::sprite_ptr temp_heart = heart_anims[slide_fish].sprite();
                    temp_heart.set_x(goal_fish_sprs[slide_fish][0].position().x() + lerp_amt);

                    if (lerp_amt == 0) { // aren't lerping anymore
                        slide_fish++;
                        bn::log(bn::string<16>("slide_fish"));
                        bn::log(bn::to_string<16>(slide_fish));
                        if (slide_fish >= goal_fish_sprs.size())
                            sliding = false;
                    }
                }

                // update fish patience
                for (int i = 0; i < goal_fish_sprs.size(); i++) {
                    if (fish_configs[i].patience_counter <= 0) {
                        // Check if the heart animation has finished
                        // needs to be 8 for whatever reason (imagine index starts at 1?)
                        if (heart_anims[i].current_graphics_index() % 8 == 0 && heart_anims[i].current_graphics_index() != ((3 - fish_configs[i].patience) * 8)) {
                            fish_configs[i].patience_counter = fish_configs[i].patience_max;
                            fish_configs[i].patience--;
                            bn::log(bn::string<16>("hewwo?"));
                        } else {
                            heart_anims[i].update();
                        }
                    } else {
                        fish_configs[i].patience_counter--;
                    }
                }
                
            };

            void start_level() {
                is_started = true;

                // Create goal fish sprites and slide them in
                int y_pos = -50;
                for (int i = 0; i < 4; i++) {
                    create_fish_spr_from_config(fish_configs[i], -140, y_pos);
                }
                slide_fish = 0;
                sliding = true;
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
                bn::vector<bn::sprite_ptr, 16> curr_fish;
                bn::log(bn::to_string<16>(x_pos));
                bn::sprite_ptr bubble = bn::sprite_items::goal_bubble.create_sprite(x_pos, y_pos);
                bubble.put_below();
                curr_fish.push_back(bubble);
                switch (config.fish_type) {
                    case Purple:
                        {
                            bn::log(bn::string<32>("ourple"));
                            bn::sprite_ptr fish_spr = bn::sprite_items::fish_item.create_sprite(x_pos, y_pos);
                            curr_fish.push_back(fish_spr);
                            break;
                        }
                    case Green:
                        {
                            bn::log(bn::string<32>("greeb"));
                            bn::sprite_ptr fish_spr = bn::sprite_items::green_fish_item.create_sprite(x_pos, y_pos);
                            curr_fish.push_back(fish_spr);
                            break;
                        }
                    default:
                        bn::log(bn::string<32>("switch fucked up"));
                        return;
                }

                // add heart animation
                // bn::sprite_ptr heart = bn::sprite_items::heart_animation.create_sprite()
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

            void update_fish_sprs(int counter) {
                bn::vector<bn::vector<bn::sprite_ptr, 16>, 10>::iterator it = goal_fish_sprs.begin();
                for (int i = 0; i < counter; i++) {
                    it++;
                }
                // Found correct fish sprite to delete
                // Remove all sprites
                it->clear();
                goal_fish_sprs.erase(it);

                // do it again for the heart anims
                bn::vector<bn::sprite_animate_action<22>, 16>::iterator it2 = heart_anims.begin();
                for (int i = 0; i < counter; i++) {
                    it2++;
                }
                heart_anims.erase(it2);

                bn::log(bn::string<16>("counter"));
                bn::log(bn::to_string<16>(counter));
                bn::log(bn::to_string<16>(fish_configs.size()));
                if (!fish_configs.empty() && counter < fish_configs.size()) {
                    slide_fish = counter;
                    sliding = true;
                }

                // Add in new fish (if applicable)
                if (fish_configs.size() >= 4) {
                    create_fish_spr_from_config(fish_configs[counter], -140, -50);
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

            bn::vector<FishConfig, 16> fish_configs;
            bn::vector<bn::sprite_animate_action<22>, 16> heart_anims;
            bn::vector<bn::vector<bn::sprite_ptr, 16>, 10> goal_fish_sprs;
            bn::vector<int, 4> x_poses;
            int slide_fish;


            int num_customers;
            int money;
            int fish_spacing = 32;
    };
}