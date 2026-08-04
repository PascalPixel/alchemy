#include "types.h"

/*
 * resource_39c placement-event dispatcher at 0x020019b8
 * (544-byte whole-owner span, 37 static call sites).
 *
 * The executable body ends at 0x02001bbe.  Six referenced pool words extend
 * the owner to 0x02001bd8, where the next function begins.  The six-word
 * placement record is produced by the reviewed 0x02000758 search owner and is
 * consumed by the reviewed 0x020008ec staged-movement owner.
 *
 * Actor slot 8 selects one of two presentation sequences by its x tile.  Slot
 * 10 selects arrival/departure sequences by its z tile.  Each branch keeps its
 * own flag update and callback, and all exits share the scene close.  The
 * repeated rectangle calls are intentionally written out: each is a distinct
 * static call site and the source-order audit must be able to see all of them.
 */

typedef struct PlacementResult {
    s32 deltaIndex;
    s32 actorSlot;
    s32 x;
    s32 y;
    s32 z;
    void (*callback)(void);
} PlacementResult;

extern void Func_0808a018(void);
extern s32 Func_02000758(PlacementResult *result);
extern void Func_020008ec(s32 deltaIndex, s32 actorSlot,
                          s32 x, s32 y, s32 z,
                          void (*callback)(void));
extern void Func_0808a010(s32 frames);
extern void Func_080f9010(s32 sound);
extern void Func_02003860(void);
extern void Func_08009180(s32 left, s32 top, s32 right, s32 bottom,
                          s32 style, s32 enabled);
extern void Func_080770c8(s32 flag);
extern void Func_080770d0(s32 flag);
extern s32 Func_080770c0(s32 flag);
extern void Func_0808a208(s32 distance, s32 height);
extern void Func_0808a210(const void *resource, s32 direction,
                          s32 position, s32 mode);
extern void Func_0808a218(void);
extern void Func_020017a8(s32 handle);
extern void Func_020016c4(s32 handle);
extern void Func_0808a020(void);

void Func_020019b8(void)
{
    PlacementResult placement;

    Func_0808a018();

    if (Func_02000758(&placement) != 0) {
    if (placement.actorSlot == 8) {
        if ((placement.x >> 20) == 11) {
            Func_020008ec(placement.deltaIndex, 8,
                          placement.x, placement.y, placement.z,
                          placement.callback);
            Func_0808a010(30);
            Func_080f9010(0xd3);
            Func_02003860();

            Func_08009180(76, 60, 74, 38, 3, 1);
            Func_08009180(77, 60, 76, 38, 2, 1);
            Func_08009180(75, 58, 86, 41, 1, 3);
            Func_08009180(75, 59, 86, 43, 1, 2);
            Func_08009180(76, 59, 80, 49, 2, 1);
            Func_08009180(77, 59, 82, 49, 2, 1);
            Func_080770c8(0x302);
        } else {
            placement.callback = (void (*)(void))0x0200b949;

            Func_08009180(75, 57, 86, 41, 1, 1);
            Func_08009180(71, 59, 86, 42, 1, 1);
            Func_08009180(71, 59, 86, 43, 1, 1);
            Func_08009180(71, 59, 86, 44, 1, 1);
            Func_08009180(71, 59, 80, 49, 1, 1);
            Func_08009180(71, 59, 81, 49, 1, 1);
            Func_08009180(71, 59, 82, 49, 1, 1);
            Func_08009180(78, 58, 83, 49, 1, 1);

            Func_020008ec(placement.deltaIndex, placement.actorSlot,
                          placement.x, placement.y, placement.z,
                          placement.callback);
            Func_080770d0(0x302);
        }
    } else if (placement.actorSlot == 10) {
        s32 zTile = placement.z >> 20;

        if (zTile == 40) {
            Func_020008ec(placement.deltaIndex, 10,
                          placement.x, placement.y, placement.z,
                          placement.callback);

            if (Func_080770c0(0x307) == 0) {
                Func_0808a208(0x18000, 0x3000);
                Func_0808a210((const void *)0x02ca0000,
                              -1, 0x2500000, 1);
                Func_0808a218();
                Func_080770c8(0x307);
                Func_020017a8(5);
                Func_0808a010(50);
            } else {
                Func_020017a8(5);
            }

            Func_080770c8(0x306);
        } else if (zTile == 42) {
            placement.callback = (void (*)(void))0x020098f9;
            Func_020008ec(placement.deltaIndex, 10,
                          placement.x, placement.y, placement.z,
                          placement.callback);
            Func_020016c4(5);
            Func_080770d0(0x306);
        }
    }
    }

    Func_0808a020();
}
