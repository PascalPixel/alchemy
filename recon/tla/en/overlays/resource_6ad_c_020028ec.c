/* Near miss of overlay 6ad (Func_02000c80 and Func_02000cb8 are C in s0cf4.c): Func_020028ec differ 8 with the forced turn step, 4 without it: the listing
 * builds 0x1000 across the two scale updates. */
#include "../../../../games/tla/src/field/overlay_6ad/room.h"
#include "iwram_call.h"

struct SpriteParts {
    u8 unknown_00[9];
    u8 unknown_09_low : 2;
    u8 part : 2;
    u8 unknown_0a[6];
    u8 unknown_10;
    u8 unknown_11;
    u16 unknown_12;
};

extern u8 Data_020023c4[];
extern u16 Data_020074c8;

void Func_02002f6c(void);
void Func_02005640(void);

void Func_020028ec(struct ObjectRuntime *object)
{
    s32 dx;
    s32 dz;
    s32 turn;

    Object_GetById(gPartyState.current_owner);
    dx = object->bounce;
    object->x += dx;
    object->y += object->gravity;
    dz = object->unknown_4c;
    object->z += dz;
    object->bounce = dx - dx / 18;
    object->unknown_4c = dz - dz / 16;
    /* FAKEMATCH: ⚓️ stores the Z speed before it loads the scale rate */
    asm volatile("");
    turn = 0x1000;
    object->scale_x += object->speed_limit;
    object->scale_y += object->acceleration;
    /* FAKEMATCH: ⚓️ stores the Y scale, with the turn step built, before it loads the sprite */
    asm volatile("" : "+l"(turn));
    ((struct SpriteParts *)object->animation)->unknown_12 += turn;
}

