/* Draft of overlay 671 Func_02005428, first attempt, differ 38 of 61; overlay
 * 6a0's Func_02001a48 is the same function with Func_02001acc for
 * Func_020054ac. The native enters the loop at its test (b), starts the end
 * mark 0xffff before the entry jump and again at the end of each pass
 * (movs r1, #255) and finishes it at the test, and reads the id from the
 * work on every pass; this while loop copies the test above the loop and
 * hoists the mark and the id. Overlay 6a1's Func_02000094 has the same
 * entry shape, reached there with a jump into a do-while. */
#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"

struct GabombaStatueEffectWork {
    u8 unknown_00[0x1a];
    s16 object_id;
};

struct ObjectRuntime *Object_GetById(s32 id);
void GameFlag_SetBit(s32 flag);
void Func_020054ac(s32 id, s32 a, s32 flag, s32 terrain);

// 表から今の相手の行を探して縮める
void Func_02005428(u16 *list, s32 mode, s32 count)
{
    struct GabombaStatueEffectWork *work = Ram_HeapSlots->effect_work;

    while (*list != 0xffff) {
        s32 id = work->object_id;

        if (id == *list) {
            struct ObjectRuntime *object = Object_GetById(id);
            s32 a;
            s32 flag;

            list++;
            a = list[0];
            flag = list[1];
            if (count < 8)
                object->scale_y -= 0x1f00;
            if (mode == 1) {
                GameFlag_SetBit(flag);
                Func_020054ac(id, a, flag, object->terrain_id);
                object->scale_y = 0x10000;
            }
            break;
        }
        list += 3;
    }
}
