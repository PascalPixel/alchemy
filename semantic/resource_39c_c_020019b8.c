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
 *
 * Call symbols are per-site, lifted in program order from the reference
 * disassembly (bun tools/overlay_show.ts resource_39c 19b8 1bd8), not from
 * the imported function identity -- the same main-image import gets a
 * different veneer symbol at each call site.
 */

typedef struct PlacementResult {
    s32 deltaIndex;
    s32 actorSlot;
    s32 x;
    s32 y;
    s32 z;
    void (*callback)(void);
} PlacementResult;

extern void Func_02007476(void);
extern s32 Func_02002122(PlacementResult *result);
extern void Func_020022e0(s32 deltaIndex, s32 actorSlot,
                          s32 x, s32 y, s32 z,
                          void (*callback)(void));
extern void Func_020074a6(s32 frames);
extern void Func_02007664(s32 sound);
extern void Func_02005264(void);
extern void Func_0200741e(s32 left, s32 top, s32 right, s32 bottom,
                          s32 style, s32 enabled);
extern void Func_02007430(s32 left, s32 top, s32 right, s32 bottom,
                          s32 style, s32 enabled);
extern void Func_02007442(s32 left, s32 top, s32 right, s32 bottom,
                          s32 style, s32 enabled);
extern void Func_02007452(s32 left, s32 top, s32 right, s32 bottom,
                          s32 style, s32 enabled);
extern void Func_02007462(s32 left, s32 top, s32 right, s32 bottom,
                          s32 style, s32 enabled);
extern void Func_02007472(s32 left, s32 top, s32 right, s32 bottom,
                          s32 style, s32 enabled);
extern void Func_02007500();
extern void Func_02007490(s32 left, s32 top, s32 right, s32 bottom,
                          s32 style, s32 enabled);
extern void Func_020074a0(s32 left, s32 top, s32 right, s32 bottom,
                          s32 style, s32 enabled);
extern void Func_020074b0(s32 left, s32 top, s32 right, s32 bottom,
                          s32 style, s32 enabled);
extern void Func_020074c0(s32 left, s32 top, s32 right, s32 bottom,
                          s32 style, s32 enabled);
extern void Func_020074d0(s32 left, s32 top, s32 right, s32 bottom,
                          s32 style, s32 enabled);
extern void Func_020074e0(s32 left, s32 top, s32 right, s32 bottom,
                          s32 style, s32 enabled);
extern void Func_020074f0(s32 left, s32 top, s32 right, s32 bottom,
                          s32 style, s32 enabled);
extern void Func_020023fc(s32 deltaIndex, s32 actorSlot,
                          s32 x, s32 y, s32 z,
                          void (*callback)(void));
extern void Func_020075aa(s32 flag);
extern void Func_02002424(s32 deltaIndex, s32 actorSlot,
                          s32 x, s32 y, s32 z,
                          void (*callback)(void));
extern s32 Func_020075c2(s32 flag);
extern void Func_020076fa(s32 distance, s32 height);
extern void Func_02007712(const void *resource, s32 direction,
                          s32 position, s32 mode);
extern void Func_0200771e(void);
extern void Func_020075f4(s32 flag);
extern void Func_02003316(s32 handle);
extern void Func_02007620(s32 frames);
extern void Func_02003324(s32 handle);
extern void Func_0200760e(s32 flag);
extern void Func_0200248c(s32 deltaIndex, s32 actorSlot,
                          s32 x, s32 y, s32 z,
                          void (*callback)(void));
extern void Func_0200326a(s32 handle);
extern void Func_02007640(s32 flag);
extern void Func_0200766c(void);

void Func_020019b8(void)
{
    PlacementResult placement;

    Func_02007476();

    if (Func_02002122(&placement) != 0) {
    if (placement.actorSlot == 8) {
        if ((placement.x >> 20) == 11) {
            Func_020022e0(placement.deltaIndex, 8,
                          placement.x, placement.y, placement.z,
                          placement.callback);
            Func_020074a6(30);
            Func_02007664(0xd3);
            Func_02005264();

            Func_0200741e(76, 60, 74, 38, 3, 1);
            Func_02007430(77, 60, 76, 38, 2, 1);
            Func_02007442(75, 58, 86, 41, 1, 3);
            Func_02007452(75, 59, 86, 43, 1, 2);
            Func_02007462(76, 59, 80, 49, 2, 1);
            Func_02007472(77, 59, 82, 49, 2, 1);
            Func_02007500(0x302);
        } else {
            placement.callback = (void (*)(void))0x0200b949;

            Func_02007490(75, 57, 86, 41, 1, 1);
            Func_020074a0(71, 59, 86, 42, 1, 1);
            Func_020074b0(71, 59, 86, 43, 1, 1);
            Func_020074c0(71, 59, 86, 44, 1, 1);
            Func_020074d0(71, 59, 80, 49, 1, 1);
            Func_020074e0(71, 59, 81, 49, 1, 1);
            Func_020074f0(71, 59, 82, 49, 1, 1);
            Func_02007500(78, 58, 83, 49, 1, 1);

            Func_020023fc(placement.deltaIndex, placement.actorSlot,
                          placement.x, placement.y, placement.z,
                          placement.callback);
            Func_020075aa(0x302);
        }
    } else if (placement.actorSlot == 10) {
        s32 zTile = placement.z >> 20;

        if (zTile == 40) {
            Func_02002424(placement.deltaIndex, 10,
                          placement.x, placement.y, placement.z,
                          placement.callback);

            if (Func_020075c2(0x307) == 0) {
                Func_020076fa(0x18000, 0x3000);
                Func_02007712((const void *)0x02ca0000,
                              -1, 0x2500000, 1);
                Func_0200771e();
                Func_020075f4(0x307);
                Func_02003316(5);
                Func_02007620(50);
            } else {
                Func_02003324(5);
            }

            Func_0200760e(0x306);
        } else if (zTile == 42) {
            placement.callback = (void (*)(void))0x020098f9;
            Func_0200248c(placement.deltaIndex, 10,
                          placement.x, placement.y, placement.z,
                          placement.callback);
            Func_0200326a(5);
            Func_02007640(0x306);
        }
    }
    }

    Func_0200766c();
}
