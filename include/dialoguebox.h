#include "bn_core.h"
#include "bn_log.h"
#include "bn_string.h"
#include "bn_sprites_actions.h"
#include "bn_sprite_actions.h"
#include "bn_sprite_builder.h"
#include "bn_sprite_item.h"
#include "bn_regular_bg_ptr.h"
#include "bn_regular_bg_map_item.h"
#include "bn_sprite_animate_actions.h"
#include "bn_sprite_text_generator.h"
#include "bn_unique_ptr.h"

#include "temp_font_variable_font.h"

#include "bn_regular_bg_items_dialogue_box.h"
#include "bn_sprite_items_testportrait.h"

#include "dialogue.h"

namespace kt {
    constexpr int line_x_pos = -95;
    constexpr int line_1_y_pos = 35;
    constexpr int line_2_y_pos = 50;
    constexpr int line_3_y_pos = 65;
    constexpr int offscreen_pos = 150;
    constexpr int nameplate_x_pos = -100;
    constexpr int nameplate_y_pos = 20;
    constexpr int portrait_x_pos = 90;
    constexpr int portrait_y_pos = 0;

    class DialogueBox {
        public:
            DialogueBox() :
                        bg(bn::regular_bg_items::dialogue_box.create_bg(0, 0)),
                        line_1(temp_font_variable_font),
                        line_2(temp_font_variable_font),
                        line_3(temp_font_variable_font),
                        nameplate(temp_font_variable_font),
                        port_spr(bn::sprite_items::testportrait.create_sprite(0, 0)) {
                bn::log(bn::string<32>("DialogueBox constructed"));
                init_dialogue();
                bg.set_priority(2);
                nameplate.set_bg_priority(1);
                line_1.set_bg_priority(1);
                line_2.set_bg_priority(1);
                line_3.set_bg_priority(1);
                nameplate.set_left_alignment();
                line_1.set_left_alignment();
                line_2.set_left_alignment();
                line_3.set_left_alignment();
                nameplate.generate(nameplate_x_pos, offscreen_pos, "", nameplate_sprites);
                line_1.generate(-50, 150, "", line_1_sprites);
                line_2.generate(-50, 130, "", line_2_sprites);
                line_3.generate(-50, 110, "", line_3_sprites);
                line_1.set_one_sprite_per_character(true);
                line_2.set_one_sprite_per_character(true);
                line_3.set_one_sprite_per_character(true);
            };

            bool is_showing() {
                return showing;
            };

            bool is_typing() {
                return typing;
            };

            uint8_t trigger_dialogue(uint8_t dialogue_to_print, uint8_t num_p) {
                if (!showing) {
                    showing = true;
                    num_pages = num_p;
                    char * name;
                    char * dia1;
                    char * dia2;
                    char * dia3;
                    uint8_t * emote; // NOTE this is called emote rather than mood bc the mood enum is global, TODO make globals have a naming standard
                    call_dialogue(mitsuko, dialogue_to_print, name, dia1, dia2, dia3, emote);
                    nameplate.generate(nameplate_x_pos, nameplate_y_pos, name, nameplate_sprites);
                    line_1.generate(line_x_pos, offscreen_pos, dia1, line_1_sprites);
                    line_2.generate(line_x_pos, offscreen_pos, dia2, line_2_sprites);
                    line_3.generate(line_x_pos, offscreen_pos, dia3, line_3_sprites); // NOTE these may need to be locked behind an if
                    bg.set_position(0, 150);
                    // Determine which emote portrait sprite to use
                    port_spr.set_tiles(bn::sprite_items::testportrait.tiles_item().create_tiles((*emote)));
                    port_spr.set_position(portrait_x_pos, portrait_y_pos);
                    curr_page++;
                    start_dialogue();
                    return 1;
                } else if (curr_page < num_pages) {
                    nameplate_sprites.clear();
                    line_1_sprites.clear();
                    line_2_sprites.clear();
                    line_3_sprites.clear();
                    char * name;
                    char * dia1;
                    char * dia2;
                    char * dia3;
                    uint8_t * emote;
                    call_dialogue(mitsuko, dialogue_to_print + curr_page, name, dia1, dia2, dia3, emote);
                    nameplate.generate(nameplate_x_pos, nameplate_y_pos, name, nameplate_sprites);
                    line_1.generate(line_x_pos, offscreen_pos, dia1, line_1_sprites);
                    line_2.generate(line_x_pos, offscreen_pos, dia2, line_2_sprites);
                    line_3.generate(line_x_pos, offscreen_pos, dia3, line_3_sprites); // NOTE these may need to be locked behind an if
                    // Determine which emote portrait sprite to use
                    port_spr.set_tiles(bn::sprite_items::testportrait.tiles_item().create_tiles((*emote)));
                    port_spr.set_position(portrait_x_pos, portrait_y_pos);
                    curr_page++;
                    start_dialogue();
                    return 1;
                } else { // close the dialogue
                    showing = false;
                    bg.set_position(0, 0);
                    nameplate_sprites.clear();
                    line_1_sprites.clear();
                    line_2_sprites.clear();
                    line_3_sprites.clear();
                    port_spr.set_position(0, 0);
                    num_pages = 1;
                    curr_page = 0;
                    // dialogue.generate(-50, -150, "", text_sprites);
                    return 0;
                }
            };

            auto prio() {
                return bg.priority();
            };

        private:
            void start_dialogue() {
                for (bn::sprite_ptr& line_1_sprite : line_1_sprites) {
                    line_1_sprite.set_y(line_1_y_pos);
                    bn::core::update();
                }
                for (bn::sprite_ptr& line_2_sprite : line_2_sprites) {
                    line_2_sprite.set_y(line_2_y_pos);
                    bn::core::update();
                }
                for (bn::sprite_ptr& line_3_sprite : line_3_sprites) {
                    line_3_sprite.set_y(line_3_y_pos);
                    bn::core::update();
                }
                // wait for player to hit b
                while (true) {
                    if (bn::keypad::b_pressed()) break;
                    bn::core::update();
                }
            };
            
            bn::regular_bg_ptr bg;
            bn::sprite_text_generator line_1;
            bn::sprite_text_generator line_2;
            bn::sprite_text_generator line_3;
            bn::vector<bn::sprite_ptr, 32> line_1_sprites;
            bn::vector<bn::sprite_ptr, 32> line_2_sprites;
            bn::vector<bn::sprite_ptr, 32> line_3_sprites;
            bn::sprite_text_generator nameplate;
            bn::vector<bn::sprite_ptr, 16> nameplate_sprites;
            bn::sprite_ptr port_spr;

            bool showing = false;
            bool typing = false;
            uint8_t num_pages = 1;
            uint8_t curr_page = 0;
    };
}