#include "bn_core.h"
#include "bn_log.h"
#include "bn_keypad.h"
#include "bn_string_view.h"
#include "bn_sprite_text_generator.h"
#include "bn_sprites_actions.h"
#include "bn_sprite_actions.h" // these are 2 different files for whatever reason
#include "bn_sprite_builder.h"
#include "bn_bg_palettes.h"
#include "bn_regular_bg_map_cell_info.h"
#include "bn_regular_bg_ptr.h"
#include "bn_string_fwd.h"
#include "bn_string.h"
#include "bn_sprite_animate_actions.h"

#include "common_info.h" // not sure about this one
#include "common_variable_8x16_sprite_font.h"

// ---------- my headers
#include "level.h"

// ---------- my generated files
#include "bn_sprite_items_testturnaround.h"
#include "bn_sprite_items_colortest.h"
#include "bn_regular_bg_items_map_interactive.h"
#include "bn_sprite_items_turnaround32.h"
#include "bn_sprite_items_fish_item.h"

namespace {
    void test_startup_scene() {
        // bn::sprite_text_generator debug_text(common::variable_8x16_sprite_font);
        // debug_text.set_left_alignment();
        // bn::vector<bn::sprite_ptr, 32> text_sprites;
        // debug_text.generate(-50, -75, "Money: 0", text_sprites);

        bn::regular_bg_ptr map_bg = bn::regular_bg_items::map_interactive.create_bg(0, 0);

        const bn::regular_bg_map_item& map_item = bn::regular_bg_items::map_interactive.map_item();

        // test level 1 fish config requirements
        bn::vector<kt::FishConfig, 6> fish_configs;
        // uint8_t fish_config = 0b00000000;
        kt::FishConfig config_1 = {
            0b00000000,
            kt::Purple
        };
        kt::FishConfig config_2 = {
            0b10000000,
            kt::Purple
        };
        kt::FishConfig config_3 = {
            0b10100000,
            kt::Green
        };
        kt::FishConfig config_4 = {
            0b00100000,
            kt::Green
        };
        fish_configs.push_back(config_1);
        fish_configs.push_back(config_2);
        fish_configs.push_back(config_3);
        fish_configs.push_back(config_4);
        fish_configs.push_back(config_2);
        fish_configs.push_back(config_3);
        
        kt::Level test_level(map_item, fish_configs);

        // NOTE this while loop won't work right when working with multiple scenes, need a way to break out
        while(true) {
            if (test_level.is_level_started()) {
                if (bn::keypad::left_held()) {
                    test_level.move_player_left();
                } else if (bn::keypad::right_held()) {
                    test_level.move_player_right();
                } else if (bn::keypad::up_held()) {
                    test_level.move_player_up();
                } else if (bn::keypad::down_held()) {
                    test_level.move_player_down();
                }

                if (bn::keypad::a_pressed()) {
                    // int maybe_money = 
                    test_level.interact_player();
                    // if (maybe_money != 0) {
                    //     text_sprites.clear();
                    //     debug_text.generate(-50, -75, bn::string<32>("Money: ") + bn::to_string<16>(maybe_money), text_sprites);
                    // }
                }

                // if (bn::keypad::b_pressed()) {
                // }

                test_level.player_kitchen_update();
            } else {
                if (bn::keypad::a_pressed()) {
                    test_level.start_level();
                }
            }
            
            // info.update();
            bn::core::update();
        }
    }
}

int main()
{
    bn::core::init();

    // bn::sprite_text_generator text_generator(common::variable_8x16_sprite_font);
    bn::bg_palettes::set_transparent_color(bn::color(2, 2, 2));
    while(true)
    {
        test_startup_scene();
        bn::core::update();
    }
}
