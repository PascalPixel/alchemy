#include "battle_effect_runtime.h"
#include "object_runtime.h"
#include "types.h"

#define M2C_FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

struct ActionDescriptor {
    s16 id;
    s16 kind;
    u8 unused_04[4];
    s32 x;
    s32 y;
    s32 z;
    u8 unused_14[4];
};

extern struct ActionDescriptor *Func_0808d394(s32 id);
extern s32 Func_080770c0(s32 flag_id);
extern s32 Func_080090f0(struct ObjectRuntime *object, s32 x, s32 y, s32 z);
extern void Func_08009080(struct ObjectRuntime *object, s32 mode);

void Func_08095680(void)
{
    u8 *base = (u8 *)&Data_02000240;
    u16 val_u = M2C_FIELD(base, volatile u16 *, 0x234);
    s16 val_s = M2C_FIELD(base, s16 *, 0x234);
    s32 hi = val_s & 0xf000;
    s32 cat = val_u & 0xfff;
    s32 special;
    s32 i;

    if (Func_080770c0(265) == 0 || hi != 0)
        return;

    special = cat & 0x800;
    cat &= 0x7ff;
    if ((u32)(cat - 300) > 80)
        return;
    if (M2C_FIELD(base, s16 *, 0x236) <= 0)
        return;

    {
        s32 *object_id_ptr = (s32 *)(base + 0x1f4);

        for (i = 8; i <= 65; i++) {
            struct ActionDescriptor *desc = Func_0808d394(i);
            struct ObjectRuntime *obj;

            if (desc == 0)
                continue;
            if (desc->kind - 48 != cat - 300)
                continue;

            obj = ObjectTable_Get(i);
            if (obj == 0)
                continue;

            if (special == 0) {
                obj->terrain_height = 0;
                obj->flags = 3;
                Func_080090f0(obj, desc->x, desc->y, desc->z);
            } else {
                struct ObjectRuntime *src = ObjectTable_Get(*object_id_ptr);
                Func_080090f0(obj, src->x, src->y, src->z - 0x200000);
            }
            Func_08009080(obj, 1);
        }
    }
}
