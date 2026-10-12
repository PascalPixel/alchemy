/* Drafts of Func_02000550 (differ 43 of 40) and Func_02000624 (differ 47 of 63), the
 * neighbours of find_near.c.
 * Func_02000550: the native keeps the loop plain: every pass loads the table's address,
 * builds the phase's address (adds r1, r7; adds r1, #98) and counts i to 24; this form
 * walks a pointer through the table and keeps the phase's address in r6. A do-while with
 * the increment before the wait is the same; six minutes of permuter reached differ 22.
 * Func_02000624: the native takes the position's address again inside the loop
 * (mov r4, sp, a copy from a hard register, which loop.c never moves) and keeps only i and
 * the id in r5 and r6; this form keeps the address in r5 across the loop, with the
 * position read directly, through one pointer, or through a pointer declared in the
 * innermost block. */
#include "types.h"
#include "ram_buffer.h"
#include "party_state.h"

struct Orbiter {
    u8 unknown_00[6];
    u16 facing;
    s32 x;
    s32 y;
    s32 z;
    u8 unknown_14[15];
    u8 flags_23;
    u8 unknown_24[44];
    void **animation;
    u8 kind;
    u8 unknown_55[13];
    u8 phase;
    u8 unknown_63;
    s16 center_x;
    s16 center_z;
    void *link;
};

extern s32 Data_02005af0;
extern const s8 Data_02006b38[];
struct Orbiter *Object_GetById(s32 id);
s32 Func_02005834(s32 value);
void Func_020020f0(s32 value);
void GameFlag_SetBit(s32 flag);
void WaitFrames(s32 frames);
void Vector_AddPolarOffsetFar(s32 radius, s32 angle, s32 *position);

#define DIFF_WITHIN(a, b, limit) ((a) - (b) >= 0 ? (a) - (b) <= (limit) : (b) - (a) <= (limit))

void Func_02000550(void)
{
    struct Orbiter *actor = Object_GetById(9);
    s32 direction;
    s32 i;

    if (Func_02005834(0) == 1) {
        direction = Data_02005af0;
        Func_020020f0(0);
        GameFlag_SetBit(515);
        *(u16 *)((u8 *)Ram_HeapSlots->event_work + 0x166) = 0;
        for (i = 0; i <= 24; i++) {
            actor->phase += direction * Data_02006b38[i];
            WaitFrames(1);
        }
    }
}

struct Orbiter *Func_02000624(struct Orbiter *origin, s32 id)
{
    s32 position[3];
    s32 i;

    position[0] = origin->x;
    position[2] = origin->z;
    Vector_AddPolarOffsetFar(0x100000, origin->facing, position);
    for (i = 8; i < 64; i++) {
        struct Orbiter *object = Object_GetById(i);

        if (object != NULL && object->kind == 1 && object->animation[10] != NULL
            && *(s16 *)object->animation[10] == id) {
            if (DIFF_WITHIN(object->x, position[0], 0x17ffff) && DIFF_WITHIN(object->z, position[2], 0x17ffff))
                return object;
        }
    }
    return NULL;
}
