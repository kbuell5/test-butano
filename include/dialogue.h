#ifndef DIALOGUE_H
#define DIALOGUE_H

#include <bn_core.h>
#include <bn_string.h>

// constexpr bn::string<128> introduction = "I can't woooo to dfsfsdf my journey with you! This is a long dialogue. I love fish so much!"
// static constexpr bn::string_view introduction[] = {
//     "I can't woooo to dfsfsdf my journey with you! This is a long dialogue. I love fish so much!"
// };

//1. We need people
//2. Give each line of dialogue a person
//3. Give each line of dialogue a mood

struct dialogue_part{
    uint8_t person;
    char * name;
    char * dialogue_text1;
    char * dialogue_text2;
    char * dialogue_text3;
    uint8_t emote;
};

enum mitsuko_dialogue : uint8_t{
    mitsuko_angry_at_dolf,
    mitsuko_yell,
    total_mitsuko_dialogue
};

enum people : uint8_t{
    mitsuko,
    dolf
};

enum mood : uint8_t {
    happy,
    wink
};

dialogue_part mitsuko_dialogue[16];
dialogue_part dolf_happy_1;

static constexpr char * mitsuko_name = "Mitsuko";

static constexpr char * mitsuko_1_1 = "Hallo! Hehe";
static constexpr char * mitsuko_1_2 = "FUCK!";
static constexpr char * mitsuko_1_3 = "Fish are so cool!";

static constexpr char * mitsuko_2_1 = "Data is a lot.";
static constexpr char * mitsuko_2_2 = "FUCK!";
static constexpr char * mitsuko_2_3 = "Fish data...";

static constexpr char * dolf_1 = "Hej!";

void init_mitsuko(){
    //mitsuko_angry_at_dolf
    mitsuko_dialogue[mitsuko_angry_at_dolf].person = mitsuko;
    mitsuko_dialogue[mitsuko_angry_at_dolf].name = "Mitsuko";
    mitsuko_dialogue[mitsuko_angry_at_dolf].dialogue_text1 = mitsuko_1_1;
    mitsuko_dialogue[mitsuko_angry_at_dolf].dialogue_text2 = mitsuko_1_2;
    mitsuko_dialogue[mitsuko_angry_at_dolf].dialogue_text3 = mitsuko_1_3;
    mitsuko_dialogue[mitsuko_angry_at_dolf].emote = happy;

    mitsuko_dialogue[mitsuko_yell].person = mitsuko;
    mitsuko_dialogue[mitsuko_yell].name = "Mitsuko";
    mitsuko_dialogue[mitsuko_yell].dialogue_text1 = mitsuko_2_1;
    mitsuko_dialogue[mitsuko_yell].dialogue_text2 = mitsuko_2_2;
    mitsuko_dialogue[mitsuko_yell].dialogue_text3 = mitsuko_2_3;
    mitsuko_dialogue[mitsuko_yell].emote = wink;
}

void init_dolf(){

}


void init_dialogue( ){
    init_mitsuko();
}

void call_dialogue(const uint8_t person, const uint8_t dialogue_option, char *& name, char *& text1, char *& text2, char *& text3, uint8_t *& emote){
    if (person == mitsuko) {
        name = mitsuko_dialogue[dialogue_option].name;
        text1 = mitsuko_dialogue[dialogue_option].dialogue_text1;
        text2 = mitsuko_dialogue[dialogue_option].dialogue_text2;
        text3 = mitsuko_dialogue[dialogue_option].dialogue_text3;
        emote = &(mitsuko_dialogue[dialogue_option].emote);
    } else {
        bn::log(bn::string<16>("haha pussy"));
    }
}


#endif