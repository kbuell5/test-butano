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
#include "player.h"

// ---------- my generated files
#include "bn_sprite_items_testturnaround.h"
#include "bn_sprite_items_colortest.h"
// #include "bn_regular_bg_items_testmap.h"
#include "bn_regular_bg_items_map_interactive.h"
#include "bn_sprite_items_turnaround32.h"

namespace {
    void test_startup_scene(bn::sprite_text_generator& text_generator) {
        constexpr bn::string_view text_lines[] = {
            "First line",
            "Second",
            "Third"
        };

        common::info info("This is the info's title", text_lines, text_generator);
        bn::regular_bg_ptr map_bg = bn::regular_bg_items::map_interactive.create_bg(0, 0);

        const bn::regular_bg_map_item& map_item = bn::regular_bg_items::map_interactive.map_item();
        bn::regular_bg_map_cell valid_cell_1 = map_item.cell(0, 0);
        // NOTE changing this from 0, 0 to somewhere else causes the player to "jump" to the first movement, not sure why rn
        kt::Player test_player(bn::sprite_items::turnaround32.create_sprite(0, 0), bn::sprite_items::turnaround32, map_item, bn::sprite_items::turnaround32.tiles_item());
        int valid_index_1 = bn::regular_bg_map_cell_info(valid_cell_1).tile_index();

        // NOTE this while loop won't work right when working with multiple scenes, need a way to break out
        while(true) {
            if (bn::keypad::left_held()) {
                // NOTE these are now 2, not one (since my tiles are 16x16 and not 8x8)
                // this will change when I implement smooth movement
                test_player.move_left(valid_index_1);
                test_player.update_walk();
            } else if (bn::keypad::right_held()) {
                test_player.move_right(valid_index_1);
                test_player.update_walk();
            }
            if (bn::keypad::up_held()) {
                test_player.move_up(valid_index_1);
                test_player.update_walk();
            } else if (bn::keypad::down_held()) {
                test_player.move_down(valid_index_1);
                test_player.update_walk();
            }
            // this only works w/ the * 8 b/c I set the set_x and set_y modifiers to 2 (see note above)
            info.update();
            bn::core::update();
        }
    }
}

int main()
{
    bn::core::init();

    bn::sprite_text_generator text_generator(common::variable_8x16_sprite_font);
    bn::bg_palettes::set_transparent_color(bn::color(2, 2, 2));
    while(true)
    {
        test_startup_scene(text_generator);
        bn::core::update();
    }
}
