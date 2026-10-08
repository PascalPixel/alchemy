#include "types.h"
#include "object_runtime.h"

extern s32 Data_02005af0;
struct ObjectRuntime *Object_GetById(s32 id);
struct ObjectRuntime *Func_02000624(struct ObjectRuntime *origin, s32 id);
void Func_020006ac(struct ObjectRuntime *object, s32 dx, s32 dz, s32 velocity);
s32 Func_0200584c(void);

void Func_02000c38(void)
{
    s32 *speed = &Data_02005af0;

    if (*speed != 0) {
        struct ObjectRuntime *actor = Object_GetById(Func_0200584c());
        struct ObjectRuntime *found = Func_02000624(actor, 359);
        s32 dx = *speed << 20;

        if (found != NULL) {
            u16 facing = *(u16 *)((u8 *)found + 6);

            /* FAKEMATCH: ⚓️ reads the facing before it builds the half turn */
            asm volatile("");
            if (facing == 0x8000)
                dx = -dx;
            facing = *(u16 *)((u8 *)actor + 6);
            if ((facing == 0 && dx > 0) || (facing == 0x8000 && dx < 0))
                Func_020006ac(actor, dx, 0, 0x40000);
        }
    }
}

void Func_02000c8c(void)
{
    s32 *speed = &Data_02005af0;

    if (*speed != 0) {
        struct ObjectRuntime *actor = Object_GetById(Func_0200584c());
        struct ObjectRuntime *found = Func_02000624(actor, 360);
        s32 dz = *speed << 20;
        u16 facing;

        if (found == NULL) {
            found = Func_02000624(actor, 361);
            dz = -dz;
            if (found == NULL)
                return;
        }
        facing = *(u16 *)((u8 *)actor + 6);
        /* FAKEMATCH: ⚓️ reads the facing before it builds the quarter turn */
        asm volatile("");
        if ((facing == 0x4000 && dz > 0) || (facing == 0xc000 && dz < 0))
            Func_020006ac(actor, 0, dz, 0x40000);
    }
}
