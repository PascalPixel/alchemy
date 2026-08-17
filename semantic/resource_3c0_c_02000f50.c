#include "types.h"

struct DeferredWrite_02000f50 { u32 value; u32 address; u32 mask; };

extern s32 Func_080770c0(s32 flagId);
extern void Func_080770c8(s32 flagId);
extern u8 *Func_0808a080(s32 actorId);
extern void Func_0808a5e0(s32 resourceId);

static void QueueDisplayWrite_02000f50(u32 value, u32 address)
{
    volatile u16 *interruptMaster = (volatile u16 *)0x04000208;
    volatile u16 *count = (volatile u16 *)0x02002090;
    u16 saved = *interruptMaster;

    *interruptMaster = (u16)(u32)interruptMaster;
    if (*count <= 31) {
        struct DeferredWrite_02000f50 *entry =
            (struct DeferredWrite_02000f50 *)(0x02002094 + *count * 12);
        (*count)++;
        entry->value = value;
        entry->address = address;
        entry->mask = 0x02000000;
    }
    *interruptMaster = saved;
}

/* Carry the five scene-completion flags into their persistent counterparts,
 * normalize actors 8..12, then queue the display blend registers. */
void Func_02000f50(void)
{
    s32 actorId;
    s32 blend = 0;

    if (Func_080770c0(0x301) != 0) Func_080770c8(0x206);
    if (Func_080770c0(0x302) != 0) Func_080770c8(0x207);
    if (Func_080770c0(0x303) != 0) Func_080770c8(0x208);
    if (Func_080770c0(0x304) != 0) Func_080770c8(0x209);
    if (Func_080770c0(0x305) != 0) Func_080770c8(0x20a);

    for (actorId = 8; actorId <= 12; actorId++) {
        u8 *actor = Func_0808a080(actorId);
        if (actor != 0) {
            if (Func_080770c0(0x109) == 0) {
                *(s32 *)(actor + 24) = 0x800;
                *(s32 *)(actor + 28) = 0x800;
            }
            (*(u8 **)(actor + 80))[38] = 0;
        }
    }

    QueueDisplayWrite_02000f50(0x3f42, 0x04000050);
    if (Func_080770c0(0x340) != 0) {
        blend = 16;
        Func_0808a5e0(244);
    }
    QueueDisplayWrite_02000f50(((16 - blend) << 8) | blend, 0x04000052);
}
