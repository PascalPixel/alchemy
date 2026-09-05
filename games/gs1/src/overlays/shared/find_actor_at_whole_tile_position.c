#include "types.h"

extern u8 *Data_03001ebc;

s32 *FindActorAtWholeTilePosition(s32 *position) {
    s32 **slots = (s32 **)(Data_03001ebc + 0x14);
    u32 actor_index;

    for (actor_index = 8; actor_index <= 65; actor_index++) {
        s32 *actor = slots[actor_index];

        if ((position[0] >> 20) == (actor[2] >> 20)
            && (position[1] >> 20) == (actor[3] >> 20)
            && (position[2] >> 20) == (actor[4] >> 20)) {
            return actor;
        }
    }
    return 0;
}
