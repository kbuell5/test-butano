#include "bn_core.h"
#include "bn_keypad.h"
#include "bn_string_view.h"
#include "bn_sprite_text_generator.h"
#include "bn_sprites_actions.h"
#include "bn_sprite_actions.h" // these are 2 different files for whatever reason
#include "bn_sprite_builder.h"
#include "bn_bg_palettes.h"

#include "common_info.h" // not sure about this one
#include "common_variable_8x16_sprite_font.h"

#include "bn_sprite_items_testturnaround.h"

namespace {
    void test_startup_scene(bn::sprite_text_generator& text_generator) {
        constexpr bn::string_view text_lines[] = {
            "First line",
            "Second",
            "Third"
        };

        common::info info("This is the info's title", text_lines, text_generator);
        // bn::sprite_ptr colortest_sprite = bn::sprite_items::colortest.create_sprite(0, 0);
        bn::sprite_ptr turnaround_sprite = bn::sprite_items::testturnaround.create_sprite(0, 0);

        while (!bn::keypad::start_pressed()) {
            if(bn::keypad::left_held()) {
                turnaround_sprite.set_tiles(bn::sprite_items::testturnaround.tiles_item().create_tiles(1));
                turnaround_sprite.set_x(turnaround_sprite.x() - 1);
            } else if (bn::keypad::right_held()) {
                turnaround_sprite.set_tiles(bn::sprite_items::testturnaround.tiles_item().create_tiles(2));
                turnaround_sprite.set_x(turnaround_sprite.x() + 1);
            }
            if (bn::keypad::up_held()) {
                turnaround_sprite.set_tiles(bn::sprite_items::testturnaround.tiles_item().create_tiles(3));
                turnaround_sprite.set_y(turnaround_sprite.y() - 1);
            } else if (bn::keypad::down_held()) {
                turnaround_sprite.set_tiles(bn::sprite_items::testturnaround.tiles_item().create_tiles(0));
                turnaround_sprite.set_y(turnaround_sprite.y() + 1);
            }
            
            info.update();
            bn::core::update();
        }
    }
}

int main()
{
    bn::core::init();

    bn::sprite_text_generator text_generator(common::variable_8x16_sprite_font);
    bn::bg_palettes::set_transparent_color(bn::color(15, 26, 15));
    while(true)
    {
        test_startup_scene(text_generator);
        bn::core::update();
    }
}
