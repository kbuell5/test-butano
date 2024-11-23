#ifndef TEMP_FONT_VARIABLE_FONT_H
#define TEMP_FONT_VARIABLE_FONT_H

#include "bn_sprite_font.h"
#include "bn_utf8_characters_map.h"
#include "bn_sprite_items_temp_font_variable.h"

namespace kt
{
    constexpr bn::utf8_character temp_font_variable_font_utf8_characters[] = {
        "Á", "É", "Í", "Ó", "Ú", "Ü", "Ñ", "á", "é", "í", "ó", "ú", "ü", "ñ", "¡", "¿"
    };
    
    constexpr int8_t temp_font_variable_font_character_widths[] = {
        6,  // 32
        3,  // 33 !
        5,  // 34 "
        6,  // 35 #
        6,  // 36 $
        5,  // 37 %
        6,  // 38 &
        5,  // 39 '
        4,  // 40 (
        4,  // 41 )
        5,  // 42 *
        6,  // 43 +
        6,  // 44 ,
        6,  // 45 -
        6,  // 46 .
        6,  // 47 /
        6,  // 48 0
        5,  // 49 1
        6,  // 50 2
        6,  // 51 3
        6,  // 52 4
        6,  // 53 5
        6,  // 54 6
        6,  // 55 7
        6,  // 56 8
        6,  // 57 9
        4,  // 58 :
        4,  // 59 ;
        5,  // 60 <
        6,  // 61 =
        5,  // 62 >
        6,  // 63 ?
        7,  // 64 @
        7,  // 65 A
        7,  // 66 B
        6,  // 67 C
        7,  // 68 D
        7,  // 69 E
        7,  // 70 F
        6,  // 71 G
        7,  // 72 H
        6,  // 73 I
        6,  // 74 J
        7,  // 75 K
        7,  // 76 L
        6,  // 77 M
        7,  // 78 N
        6,  // 79 O
        7,  // 80 P
        7,  // 81 Q
        7,  // 82 R
        7,  // 83 S
        6,  // 84 T
        7,  // 85 U
        6,  // 86 V
        6,  // 87 W
        6,  // 88 X
        6,  // 89 Y
        7,  // 90 Z
        5,  // 91 [
        6,  // 92
        5,  // 93 ]
        6,  // 94 ^
        5,  // 95 _
        6,  // 96
        7,  // 97 a
        7,  // 98 b
        6,  // 99 c
        7,  // 100 d
        7,  // 101 e
        7,  // 102 f
        6,  // 103 g
        7,  // 104 h
        6,  // 105 i
        6,  // 106 j
        7,  // 107 k
        7,  // 108 l
        6,  // 109 m
        7,  // 110 n
        6,  // 111 o
        7,  // 112 p
        7,  // 113 q
        7,  // 114 r
        7,  // 115 s
        6,  // 116 t
        7,  // 117 u
        6,  // 118 v
        6,  // 119 w
        6,  // 120 x
        6,  // 121 y
        7,  // 122 z
        6,  // 123 {
        6,  // 124 |
        6,  // 125 }
        6,  // 126 ~
        7,  // Á
        7,  // É
        5,  // Í
        7,  // Ó
        7,  // Ú
        7,  // Ü
        7,  // Ñ
        7,  // á
        7,  // é
        4,  // í
        7,  // ó
        7,  // ú
        7,  // ü
        7,  // ñ
        3,  // ¡
        7,  // ¿
    };

    constexpr bn::span<const bn::utf8_character> temp_font_variable_font_utf8_characters_span(temp_font_variable_font_utf8_characters);

    constexpr auto temp_font_variable_font_utf8_characters_map =
            bn::utf8_characters_map<temp_font_variable_font_utf8_characters_span>();

    constexpr bn::sprite_font temp_font_variable_font(
            bn::sprite_items::temp_font_variable, temp_font_variable_font_utf8_characters_map.reference(),
            temp_font_variable_font_character_widths);
}

#endif