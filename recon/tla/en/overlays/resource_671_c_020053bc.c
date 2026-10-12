/* Draft, differ 28 of 51 (the same loop as Func_02005428, drafted here too): the native
 * rebuilds 0xffff for the end test on every pass (movs r2, #255; lsls; adds) and reads the id
 * again at the top of the body; this form keeps 0xffff in r10 across the loop. Measured in
 * the loop dump (alchemy diff --rtl): the u16 test is narrowed to HImode, its constant is
 * three insns (the halfword constant and two shifts), and loop.c moves them (life 4,
 * savings 2). With the test in SImode the constant is one insn and stays: an assignment in
 * the test (while ((end = entry[0]) != 0xffff)) rebuilds it every pass as the native does,
 * but then shares that read with the body's (differ 24). An inline end test builds a flag
 * (differ 36); an s16 table with (u16) casts and a masked read are no better. Wanted: a
 * form whose end test is SImode and whose body reads the id again. */
#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"

struct ObjectRuntime *Object_GetById(s32 id);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object, s32 flags);
void GameFlag_SetBit(s32 flag);
void Func_020054ac(s32 id, u32 mode, s32 flag, s32 layer);

struct EffectWork {
    u8 unknown_00[26];
    s16 target;
};

/* 層の切り替え表: 3 語ずつ (id, mode, flag)、0xffff で終わり */
void Func_020053bc(const u16 *entry)
{
    while (entry[0] != 0xffff) {
        s32 id = entry[0];
        struct ObjectRuntime *object = Object_GetById(id);
        s16 mode = entry[1];
        s16 flag = entry[2];

        object->collision_flags &= 0xfe;
        ObjectDispatch_SetSingleChildFlags(object, 0);
        Func_020054ac(id, (u16)mode, (u16)flag, object->terrain_id);
        entry += 3;
    }
}

void Func_02005428(const u16 *entry, s32 set, s32 step)
{
    struct EffectWork *work = Ram_HeapSlots->effect_work;

    while (entry[0] != 0xffff) {
        s32 id = work->target;

        if (id == entry[0]) {
            struct ObjectRuntime *object = Object_GetById(id);
            s32 mode = entry[1];
            s32 flag = entry[2];

            if (step <= 7)
                object->scale_y -= 0x1f00;
            if (set == 1) {
                GameFlag_SetBit(flag);
                Func_020054ac(id, mode, flag, object->terrain_id);
                object->scale_y = 0x10000;
            }
            return;
        }
        entry += 3;
    }
}
