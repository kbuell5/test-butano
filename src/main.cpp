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
        
        // kt::PlayerStruct.spr_items = bn::sprite_items::testturnaround;
        bn::sprite_ptr turnaround_sprite = bn::sprite_items::testturnaround.create_sprite(0, 0);
        kt::Player test_player(bn::sprite_items::turnaround32.create_sprite(0, 0));
        // bn::regular_bg_ptr map_bg = bn::regular_bg_items::testmap.create_bg(0, 0);
        bn::regular_bg_ptr map_bg = bn::regular_bg_items::map_interactive.create_bg(0, 0);

        const bn::regular_bg_map_item& map_item = bn::regular_bg_items::map_interactive.map_item();
        bn::regular_bg_map_cell valid_cell_1 = map_item.cell(0, 0);
        // bn::regular_bg_map_cell valid_cell_2 = map_item.cell(1, 0);
        int valid_index_1 = bn::regular_bg_map_cell_info(valid_cell_1).tile_index();
        // int valid_index_2 = bn::regular_bg_map_cell_info(valid_cell_2).tile_index();
        bn::point player_pos(16, 16);

        // NOTE this while loop won't work right when working with multiple scenes, need a way to break out
        // also NOTE that this jumps the player from cell to cell, doesn't move smoothly
        while(true) {
            bn::point new_player_pos = player_pos;
            if (bn::keypad::left_pressed()) {
                // NOTE these are now 2, not one (since my tiles are 16x16 and not 8x8)
                new_player_pos.set_x(new_player_pos.x() - 2);
                turnaround_sprite.set_tiles(bn::sprite_items::testturnaround.tiles_item().create_tiles(1));
            } else if (bn::keypad::right_pressed()) {
                new_player_pos.set_x(new_player_pos.x() + 2);
                turnaround_sprite.set_tiles(bn::sprite_items::testturnaround.tiles_item().create_tiles(2));
            }
            if (bn::keypad::up_pressed()) {
                new_player_pos.set_y(new_player_pos.y() - 2);
                turnaround_sprite.set_tiles(bn::sprite_items::testturnaround.tiles_item().create_tiles(3));
            } else if (bn::keypad::down_pressed()) {
                new_player_pos.set_y(new_player_pos.y() + 2);
                turnaround_sprite.set_tiles(bn::sprite_items::testturnaround.tiles_item().create_tiles(0));
            }

            bn::regular_bg_map_cell new_map_cell = map_item.cell(new_player_pos);
            int player_tile_index = bn::regular_bg_map_cell_info(new_map_cell).tile_index();
            if (player_tile_index == valid_index_1) {
                // bn::log(bn::string<4>("pink"));
                player_pos = new_player_pos;
            }
            // if (player_tile_index == valid_index_2) {
            //     bn::log(bn::string<5>("green"));
            // }
            // if (player_tile_index == valid_index_1 || player_tile_index == valid_index_2) {
            //     player_pos = new_player_pos;
            // }

            // this only works w/ the * 8 b/c I set the set_x and set_y modifiers to 2 (see note above)
            bn::fixed player_sprite_x = (player_pos.x() * 8) - (map_item.dimensions().width() * 4) + 8;
            bn::fixed player_sprite_y = (player_pos.y() * 8) - (map_item.dimensions().height() * 4) + 8;
            turnaround_sprite.set_position(player_sprite_x, player_sprite_y);
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
