#include "types.h"







/* Initialize one encounter entity from the current map actor. Nearby entities
 * inherit the actor's live position; distant or gated entities retain the
 * sentinel position. Every eighth frame also emits its presentation record. */
extern u8 * Func_02001c6a(s32 actorId);
extern void Func_02001c38(u8 *entity, s32 mode);
extern s32 Func_02001c5c(s32 flagId);
extern s32 Func_02001ca4(void);
extern void Func_02000bf6(s32 x, s32 height, s32 y, s32 mode,
    s32 arg4, s32 arg5, s32 descriptorId, void *descriptor);
s32 Func_020009cc(u8 *entity)
{
    s32 mapActorId = *(volatile s32 *)0x02000434;
    u8 *mapActor = Func_02001c6a(mapActorId);
    s32 dx;
    s32 dy;

    *(s32 *)(entity + 52) = 0x4000;
    *(s32 *)(entity + 48) = 0x18000;
    entity[85] = 0;
    Func_02001c38(entity, 0);
    entity[84] ^= 1;

    if (Func_02001c5c(0x104) != 0) {
        *(s32 *)(entity + 56) = (s32)0x80000000;
        *(s32 *)(entity + 60) = (s32)0x80000000;
        *(s32 *)(entity + 64) = (s32)0x80000000;
    } else {
        *(s32 *)(entity + 56) = *(s32 *)(mapActor + 8);
        *(s32 *)(entity + 60) = *(s32 *)(mapActor + 20);
        *(s32 *)(entity + 64) = *(s32 *)(mapActor + 16);

        dx = *(s32 *)(entity + 8) - *(s32 *)(mapActor + 8);
        if (dx < 0)
            dx = -dx;
        dy = *(s32 *)(entity + 16) - *(s32 *)(mapActor + 16);
        if (dy < 0)
            dy = -dy;

        if (dx + dy < 0x80000) {
            u8 *mapState = *(u8 *volatile *)0x03001ebc;
            if (mapActor[85] != 0)
                *(u16 *)(mapState + 0x182) = 55;
            entity[85] = 3;
            *(s32 *)(entity + 56) = *(s32 *)(mapActor + 8);
            *(s32 *)(entity + 60) = *(s32 *)(mapActor + 12);
            *(s32 *)(entity + 64) = *(s32 *)(mapActor + 16);
        }
    }

    if ((*(volatile s32 *)0x03001e40 & 7) == 0) {
        u8 descriptor[36];
        *(u32 *)(descriptor + 8) = 0xcccc;
        *(u32 *)(descriptor + 12) = 0xcccc;
        *(u16 *)(descriptor + 34) =
            (u16)(0xf800 + (((u32)Func_02001ca4() << 12) >> 16));
        Func_02000bf6(*(s32 *)(entity + 8), *(s32 *)(entity + 12),
            *(s32 *)(entity + 16), 0, 0, 0, 0x00880001, descriptor);
    }
    return 1;
}
