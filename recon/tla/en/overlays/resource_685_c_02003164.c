/* Draft of overlay 685's Func_02003164, differ 24 of 76: Func_0200320c's set-up from a given origin,
 * with one step on field_1064. Left: (1) the native sets state to 0 in r5 right after the origin's
 * last load, before Func_02002c88, yet its three zero stores take a fresh zero in r2; set there in
 * C, CSE gives the stores state's register (37), set after them it stays after the calls (24); a
 * register pin on state does not hold its place either. (2) loop.c hoists 0x14ccc out of the loop
 * into r7 (push r7); the native builds it in the loop, then finishes it before it starts 999, as in
 * Func_0200320c. Tried a one-case switch, while and do-while loops. */
#include "../../../../games/tla/src/scene/champa_cliff_caves/scene.h"

struct ChampaCliffCavesWork {
    u8 tiles[0x1000];
    s32 origin_x;
    s32 origin_y;
    s32 origin_z;
    struct FieldSprite sprite_a;
    struct FieldSprite sprite_b;
    u16 entry_a;
    u16 vram_a;
    u16 entry_b;
    u16 vram_b;
    s32 field_1064;
    s32 field_1068;
    s32 field_106c;
    s32 field_1070;
    u16 field_1074;
    u8 unknown_1076[2];
};

struct Vec3 {
    s32 x;
    s32 y;
    s32 z;
};

void *Runtime_AllocateHeapBlockFar(s32 slot, s32 size);
void Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 priority);
void WaitFrames(s32 frames);
void Func_02002c88(void);
void Func_02002de4(void);

void Func_02003164(const struct Vec3 *origin)
{
    struct ChampaCliffCavesWork *work = Runtime_AllocateHeapBlockFar(220, sizeof(struct ChampaCliffCavesWork));
    s32 state;

    work->origin_x = origin->x;
    work->origin_y = origin->y;
    work->origin_z = origin->z;
    Func_02002c88();
    work->field_106c = 0;
    work->field_1070 = 0;
    work->field_1074 = 0;
    {
        s32 priority = 0x480;

        Scheduler_AddOrUpdateCallback(Func_02002de4, priority);
    }
    Func_0200362c(139);
    state = 0;
    do {
        if (state == 0) {
            work->field_1064 += 0x1000;
            if (work->field_1064 > 0x14ccb) {
                work->field_1064 = 0x14ccc;
                state = 999;
            }
        }
        WaitFrames(1);
    } while (state != 999);
}
