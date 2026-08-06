#include "types.h"

struct DeferredWrite_02001094 { u32 value; u32 address; u32 mask; };






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
extern s32 Func_020022ee(s32 flagId);
extern void Func_02002300(s32 flagId);
extern s32 Func_020022fe(s32 flagId);
extern void Func_02002310(s32 flagId);
extern s32 Func_0200230e(s32 flagId);
extern void Func_02002322(s32 flagId);
extern u8 * Func_02002366(s32 actorId);
extern s32 Func_02002332(s32 flagId);
extern u8 * Func_02002390(s32 actorId);
extern s32 Func_02002378(s32 flagId);
extern void Func_0200238a(s32 flagId);
extern s32 Func_020023b8(s32 flagId);
extern void Func_020024e6(s32 resourceId);
void Func_02001094(void)
{
    s32 actorId;
    s32 blend = 0;
    u8 *actor;

    if (Func_020022ee(0x311) != 0) Func_02002300(0x206);
    if (Func_020022fe(0x312) != 0) Func_02002310(0x207);
    if (Func_0200230e(0x313) != 0) Func_02002322(0x208);

    for (actorId = 8; actorId <= 10; actorId++) {
        actor = Func_02002366(actorId);
        if (actor != 0) {
            (*(u8 **)(actor + 80))[38] = 0;
            if (Func_02002332(0x109) == 0) {
                *(s32 *)(actor + 24) = 0x800;
                *(s32 *)(actor + 28) = 0x800;
            }
        }
    }

    actor = Func_02002390(11);
    if (actor != 0) {
        u8 *presentation = *(u8 **)(actor + 80);
        presentation[38] = 0;
        if (*(u8 **)(presentation + 40) != 0)
            (*(u8 **)(presentation + 40))[5] = 10;
        presentation[37] = 1;
    }

    if (Func_02002378(0x315) != 0) Func_0200238a(0x9b7);

    QueueDisplayWrite_02001094(0x3f42, 0x04000050);
    if (Func_020023b8(0x340) != 0) {
        blend = 16;
        Func_020024e6(244);
    }
    QueueDisplayWrite_02001094(((16 - blend) << 8) | blend, 0x04000052);
}
