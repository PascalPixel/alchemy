/* Draft of overlay 6a1 Func_02000094. differ 2 of 50: after the loop the
 * listing clears the third argument (movs r2, #0) right after the motion
 * flag store and before it loads x and z; this draft clears it after the
 * two loads. The loop is settled: the native enters at the test, reads the
 * height and the floor before the entry jump and again at the end of each
 * pass, which a jump into a do-while gives once a barrier keeps the two
 * reads from being merged. Measured for the last pair: the argument as a
 * local behind an in-out barrier (volatile or not, alone or after a plain
 * barrier) shares the store's zero and swaps the store's two registers
 * (differ 8); an unprototyped three-argument call drops an instruction. */
#include "types.h"
#include "object_runtime.h"

struct ObjectRuntime *Object_GetById(s32 id);
void GameFlag_SetBit(s32 flag);
void WaitFrames(s32 frames);
void Func_020021e0(s32 x, s32 z, s32 a, s32 b);
void Func_020022c0(s32 x, s32 z, s32 a, s32 b);
void Func_020022c8(s32 cue);

// 落ちて止まるまで待つ
void Func_02000094(s32 unused, s32 id)
{
    struct ObjectRuntime *object = Object_GetById(id);
    s32 i;
    s32 y;
    s32 floor;

    if (object->x >> 20 == 6 && object->z >> 20 == 14) {
        GameFlag_SetBit(0xa18);
        WaitFrames(10);
        Func_020022c8(134);
        y = object->y;
        floor = object->terrain_height;
        /* FAKEMATCH: the native reads the height and the floor before it clears the counter and again at the end of each pass; without the barrier the counter is cleared first and the two reads are merged into one at the test (two instructions fewer). */
        __asm__ volatile("");
        i = 0;
        goto test;
        do {
            WaitFrames(1);
            if (++i > 29)
                break;
            y = object->y;
            floor = object->terrain_height;
        test:;
        } while (y > floor || object->velocity_y != 0);
        object->motion_flags = 0;
        Func_020021e0(object->x, object->z, 0, 0);
        Func_020022c0(object->x, object->z, 0, 128);
    }
}
