#include "bn_core.h"
#include "bn_log.h"
#include "bn_string.h"
#include "bn_sprites_actions.h"
#include "bn_sprite_actions.h"
#include "bn_sprite_builder.h"
#include "bn_sprite_item.h"

namespace kt {
    enum HeldItem {
        None,
        Fish,
        Butterfly
    };
    class Player {
        public:
        // NOTE you need the initializer list or else you get a compile-time error like "no matching function to call"
            Player(bn::sprite_ptr spr_items) : player_spr_ptr(spr_items) {
                // = spr_items.create_sprite(0, 0);
                bn::log(bn::string<10>("it worked"));
                held_item = None;
            };

            void Pickup(int item) {
                bn::log(bn::string<10>("picked up"));
            };

            void PutDown() {

            };

        private:
            bn::sprite_ptr player_spr_ptr;
            int held_item;
    };
}