/* Drafts of Func_020053bc (differ 6 of 51) and Func_02005428 (differ 49 of 61).
 * Func_020053bc: the test on an int copy of the word (while ((value = *entry) != 0xffff)) is an
 * SImode compare, whose constant is one insn that loop.c leaves inside the loop, as the native
 * rebuilds 0xffff on every pass; compared directly the u16 test is narrowed to HImode, its
 * constant is three insns (life 4, savings 2) and moves above the loop (differ 28). The id as a
 * u16 read with post-increments keeps the body's own read and the parameter in r0 for the first
 * test. Left are three reordered pairs: the native reads the word between the halves of 0xffff
 * in both tests (movs r2, #255; ldrh; lsls; adds), and steps the pointer before the last call.
 * Func_02005428: the native enters its loop at the test (b to the bottom, the test not copied
 * above) and reads the second and third words after stepping the pointer by 2; not shaped yet. */
#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"

struct ObjectRuntime *Object_GetById(s32 id);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object, s32 flags);
void GameFlag_SetBit(s32 flag);
void Func_020054ac(s32 id, u32 mode, s32 flag, s32 layer);

struct EffectWork {
    u8 unk00[26];
    s16 target;
};

/* 層の切り替え表: 3 語ずつ (id, mode, flag)、0xffff で終わり */
void Func_020053bc(const u16 *entry)
{
    s32 value;

    while ((value = *entry) != 0xffff) {
        u16 id = *entry++;
        struct ObjectRuntime *object = Object_GetById(id);
        s16 mode = *entry++;
        s16 flag = *entry++;

        object->collisionFlags &= 0xfe;
        ObjectDispatch_SetSingleChildFlags(object, 0);
        Func_020054ac(id, (u16)mode, (u16)flag, object->terrainId);
    }
}

void Func_02005428(const u16 *entry, s32 set, s32 step)
{
    struct EffectWork *work = Ram_HeapSlots->effectWork;

    s32 value;

    while ((value = *entry) != 0xffff) {
        s32 id = work->target;

        if (id == entry[0]) {
            struct ObjectRuntime *object = Object_GetById(id);
            s32 mode = entry[1];
            s32 flag = entry[2];

            if (step <= 7)
                object->scaleY -= 0x1f00;
            if (set == 1) {
                GameFlag_SetBit(flag);
                Func_020054ac(id, mode, flag, object->terrainId);
                object->scaleY = 0x10000;
            }
            return;
        }
        entry += 3;
    }
}
