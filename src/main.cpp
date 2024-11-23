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
#include "dialogue.h"

// ---------- my generated files
#include "bn_sprite_items_testturnaround.h"
#include "bn_sprite_items_colortest.h"
#include "bn_regular_bg_items_map_interactive.h"
#include "bn_sprite_items_turnaround32.h"
#include "bn_sprite_items_fish_item.h"

namespace {
    // TODO johan wanted the dialogue samples up here, find out why
    constexpr bn::string_view dialogue_sample[] = { "Mitsuko", "I can't wait to begin my journey with you! This is a long dialogue. I love fish so much!" };
    constexpr bn::string_view dialogue_sample_2[] = {"Mitsuko", "Next page!"};
    // constexpr bn::string_view name[] = {"Mitsuko"};

    void test_startup_scene() {
        bn::sprite_text_generator debug_text(common::variable_8x16_sprite_font);
        debug_text.set_left_alignment();
        bn::vector<bn::sprite_ptr, 32> text_sprites;
        debug_text.generate(-20, -75, "Money: 0", text_sprites);

        bn::regular_bg_ptr map_bg = bn::regular_bg_items::map_interactive.create_bg(0, 0);

        const bn::regular_bg_map_item& map_item = bn::regular_bg_items::map_interactive.map_item();

        // constexpr bn::string<128> dialogue_sample = "I can't wait to begin my journey with you! This is a long dialogue.#I love fish so much!";
        // bn::vector<bn::pair<bn::string_view, bn::string_view>, 2> dialogue_samples;//TODO: Can we make this a defined array?
        const bn::string_view dialogue_samples[][3] = { {dialogue_sample[0], dialogue_sample[1]}, {dialogue_sample_2[0], dialogue_sample_2[1]} };
        // dialogue_samples.push_back(bn::make_pair<bn::string_view, bn::string_view>(name, dialogue_sample));
        // dialogue_samples.push_back(bn::make_pair<bn::string_view, bn::string_view>(name, dialogue_sample_2));

        // test level 1 fish config requirements
        bn::vector<kt::FishConfig, 6> fish_configs;
        // uint8_t fish_config = 0b00000000;
        kt::fishConfig_t config_t_1;
        config_t_1.legs = 0;
        config_t_1.kiss = 0;
        config_t_1.makeup = 0;
        config_t_1.sparkles = 0;
        
        kt::fishConfig_t config_t_2;
        config_t_2.legs = 1;
        config_t_2.kiss = 0;
        config_t_2.makeup = 0;
        config_t_2.sparkles = 0;

        kt::fishConfig_t config_t_3;
        config_t_3.legs = 1;
        config_t_3.kiss = 0;
        config_t_3.makeup = 1;
        config_t_3.sparkles = 0;

        kt::fishConfig_t config_t_4;
        config_t_4.legs = 0;
        config_t_4.kiss = 0;
        config_t_4.makeup = 1;
        config_t_4.sparkles = 0;

        kt::FishConfig config_1 = {
            config_t_1,
            kt::Purple
        };
        kt::FishConfig config_2 = {
            config_t_2,
            kt::Purple
        };
        kt::FishConfig config_3 = {
            config_t_3,
            kt::Green
        };
        kt::FishConfig config_4 = {
            config_t_4,
            kt::Green
        };
        fish_configs.push_back(config_1); // plain purple
        fish_configs.push_back(config_2); // purple with legs
        fish_configs.push_back(config_3); // green with both
        fish_configs.push_back(config_4); // green with lips
        // fish_configs.push_back(config_2); // purple with legs
        fish_configs.push_back(config_3); // green with both
        
        kt::Level test_level(map_item, fish_configs, 5);

        // NOTE this while loop won't work right when working with multiple scenes, need a way to break out
        while(true) {
            if (test_level.is_level_started() && !test_level.showing_dialogue()) {
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
                    uint16_t maybe_money = test_level.interact_player();
                    if (maybe_money != 0) {
                        text_sprites.clear();
                        debug_text.generate(-20, -75, bn::format<32>("Money: {}", maybe_money), text_sprites);
                    }
                }

                test_level.kitchen_update();
            } else if (!test_level.is_level_started()) {
                if (bn::keypad::a_pressed()) {
                    test_level.start_level();
                }
            }

            if (bn::keypad::b_pressed()) {
                bn::log(bn::string<32>("spawned"));
                test_level.spawn_dialogue(dialogue_samples);
            }
            
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
