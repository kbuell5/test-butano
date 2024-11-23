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

// #include "common_variable_8x16_sprite_font.h"
#include "temp_font_variable_font.h"

#include "bn_regular_bg_items_dialogue_box.h"

namespace kt {
    class DialogueBox {
        public:
            DialogueBox() :
                        bg(bn::regular_bg_items::dialogue_box.create_bg(0, 0)),
                        line_1(temp_font_variable_font),
                        line_2(temp_font_variable_font),
                        line_3(temp_font_variable_font),
                        nameplate(temp_font_variable_font) {
                bn::log(bn::string<32>("DialogueBox constructed"));
                bg.set_priority(2);
                line_1.set_bg_priority(1);
                line_2.set_bg_priority(1);
                line_3.set_bg_priority(1);
                line_1.set_left_alignment();
                line_2.set_left_alignment();
                line_3.set_left_alignment();
                line_1.generate(-50, 150, "", line_1_sprites);
                line_2.generate(-50, 130, "", line_2_sprites);
                line_3.generate(-50, 110, "", line_3_sprites);
            };

            bool is_showing() {
                return showing;
            };

            bool is_typing() {
                return typing;
            };

            void toggle_dialogue(const bn::string_view dialogue[][3]) {
                if (!showing) {
                    showing = true;
                    line_1_sprites.clear();
                    bn::log(bn::string<128>(dialogue[0][1]));
                    line_1.generate(-70, 30, dialogue[0][1], line_1_sprites);
                    bg.set_position(0, 150);
                    start_dialogue();
                } else if (showing) {
                    showing = false;
                    bg.set_position(0, 0);
                    line_1_sprites.clear();
                    // dialogue.generate(-50, -150, "", text_sprites);
                }
            };

            // void set_speaker(bn::string<16> new_speaker) {
            //     // speaker = new_speaker;
            // };

            // void set_text(bn::string<128> new_text) {
            //     // text = new_text;
            //     prepare_text();
            // };

            void set_map(bn::vector<bn::pair<bn::string_view, bn::string_view>, 24> new_map) {
                // Empty old map (if any)
                dialogue_map.clear();
                
                // Add new map, splitting long text into multiple lines
                for (int i = 0; i < new_map.size(); i++) {
                    int length = 0;
                    bn::string<128> curr_line = "";
                    for (int j = 0; j < new_map[i].second.length(); j++) {
                        // Check for space
                        if (new_map[i].second[j] == ' ') {
                            // Determine if need to split
                            if (length + length_of_word(new_map[i].second, j) > line_width) {
                                // Save the current line
                                // dialogue_map.push_back(bn::make_pair<bn::string_view, bn::string_view>(bn::to_string<16>(new_map[i].first), curr_line));
                                curr_line = "";
                                length = 0;
                            } else {
                                curr_line.push_back(new_map[i].second[j]);
                                length += temp_font_variable_font_character_widths[new_map[i].second[j] - 32];
                            }
                        } else {
                            curr_line.push_back(new_map[i].second[j]);
                            length += temp_font_variable_font_character_widths[new_map[i].second[j] - 32];
                        }
                    }
                }

                // DEBUG print map
                for (int i = 0; i < dialogue_map.size(); i++) {
                    bn::log(bn::to_string<16>(dialogue_map[i].first));
                    bn::log(bn::to_string<128>(dialogue_map[i].second));
                }
            }

            auto prio() {
                return bg.priority();
            };

            void update_text() {

            };
        private:
            void start_dialogue() {
                // text_sprites.clear();
                // dialogue.generate(-100, 40, text, text_sprites);
            };

            void prepare_text() {
                

            };

            int length_of_word(bn::string<128> dialogue, int start) {
                int length = 0;
                for (int i = start; dialogue[i] != ' ' && i < dialogue.length(); i++) {
                    length += temp_font_variable_font_character_widths[dialogue[i] - 32];
                }
                return length;
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

            bn::vector<bn::pair<bn::string_view, bn::string_view>, 24> dialogue_map;

            bool showing = false;
            bool typing = false;
            // int char_width = 6;
            // int char_height = 16;
            int line_width = 200;

            // bn::string<16> speaker = "default";
            // bn::string<128> text = "default text";
    };
}