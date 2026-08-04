#include "types.h"

struct DeferredWrite_02001094 { u32 value; u32 address; u32 mask; };

extern s32 Func_080770c0(s32 flagId);
extern void Func_080770c8(s32 flagId);
extern u8 *Func_0808a080(s32 actorId);
extern void Func_0808a5e0(s32 resourceId);

static void QueueDisplayWrite_02001094(u32 value, u32 address)
{
    volatile u16 *interruptMaster = (volatile u16 *)0x04000208;
    volatile u16 *count = (volatile u16 *)0x02002090;
    u16 saved = *interruptMaster;

    *interruptMaster = (u16)(u32)interruptMaster;
    if (*count <= 31) {
        struct DeferredWrite_02001094 *entry =
            (struct DeferredWrite_02001094 *)(0x02002094 + *count * 12);
        (*count)++;
        entry->value = value;
        entry->address = address;
        entry->mask = 0x02000000;
    }
    *interruptMaster = saved;
}

/* Carry the scene-165 completion flags, normalize actors 8..11, and publish
 * the same blend transition used by the scene-164 sibling. */
void Func_02001094(void)
{
    s32 actorId;
    s32 blend = 0;
    u8 *actor;

    if (Func_080770c0(0x311) != 0) Func_080770c8(0x206);
    if (Func_080770c0(0x312) != 0) Func_080770c8(0x207);
    if (Func_080770c0(0x313) != 0) Func_080770c8(0x208);

    for (actorId = 8; actorId <= 10; actorId++) {
        actor = Func_0808a080(actorId);
        if (actor != 0) {
            if (Func_080770c0(0x109) == 0) {
                *(s32 *)(actor + 24) = 0x800;
                *(s32 *)(actor + 28) = 0x800;
            }
            (*(u8 **)(actor + 80))[38] = 0;
        }
    }

    actor = Func_0808a080(11);
    if (actor != 0) {
        u8 *presentation = *(u8 **)(actor + 80);
        if (*(u8 **)(presentation + 40) != 0)
            (*(u8 **)(presentation + 40))[5] = 10;
        presentation[37] = 1;
        presentation[38] = 0;
    }

    if (Func_080770c0(0x315) != 0) Func_080770c8(0x9b7);

    QueueDisplayWrite_02001094(0x3f42, 0x04000050);
    if (Func_080770c0(0x340) != 0) {
        blend = 16;
        Func_0808a5e0(244);
    }
    QueueDisplayWrite_02001094(((16 - blend) << 8) | blend, 0x04000052);
}
