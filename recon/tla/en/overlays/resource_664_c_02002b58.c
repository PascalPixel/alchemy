/* Draft of Func_02002b58 (overlay 664, 120 bytes), differ 47 of 54: the native
   takes the address of the position inside the loop (add r2, sp, #16) and
   rebuilds the height 0x40000 on every pass, with the spawn record in r5 and
   the counter in r6; this form, a goto loop, still takes the address and the
   height once before the loop (r5 and r10) and counts in r8. A for loop is
   differ 55. */
#include "../../../../games/tla/src/scene/airs_rock_inside/airs_rock_inside.h"

struct AirsRockInsideVector {
    s32 x;
    s32 y;
    s32 z;
};

struct AirsRockInsideSpawn {
    s32 mode;
    s32 unknown_04;
    s32 scale_x;
    s32 scale_y;
    s32 speed;
    s32 acceleration;
    u16 kind;
    const u8 *script;
    s32 unknown_20;
    void *update;
};

s32 Random16Far(void);
void Func_020002f4(s32 x, s32 y, s32 z, s32 vx, s32 vy, s32 vz, s32 flags, struct AirsRockInsideSpawn *spawn);

void Func_02002b58(s32 x, s32 z)
{
    struct AirsRockInsideSpawn spawn;
    struct AirsRockInsideVector position;
    s32 i;

    spawn.kind = 0x2a0;
    spawn.mode = 1;
    spawn.scale_x = 0xcccc;
    spawn.scale_y = 0x30000;
    /* FAKEMATCH: the loop is written with goto so that it carries no loop notes: the bytes rebuild the height and the position's address on every pass, and a for loop hoists both (differ 55). */
    i = 0;
next:
    position.x = x + ((Random16Far() & 31) << 16) - 0x100000;
    position.y = 0x40000;
    position.z = z;
    Func_020002f4(position.x, 0, position.z, 0, position.y, 0, 0x1a0000, &spawn);
    Battle_WaitMode0(5);
    i++;
    if (i != 12)
        goto next;
}
