#include "types.h"

#define SceneActor_SetActor15ModeZero Func_020009dc
#define ActorPresentation_SetSceneCell58AndMarkActorEight Func_02001a48
#define ActorPresentation_RepaintCellsAtActorsElevenAndTwelve Func_02001df8
#define SceneActor_PublishMarkerBySlotZeroHeight Func_02001edc
void Func_02002fd6(s32 actor, s32 mode, s32 frames);
s32 Func_02003f34(s32, s32, s32, s32, s32, s32);
u8 *Func_02003fa2(s32);
void Func_02003b9e();
s32 *Func_02004344();
void Func_020042fa(s32, s32, s32, s32, s32, s32);
s32 *Func_02004368();
void Func_0200431c(s32, s32, s32, s32, s32, s32);
s32 *Func_02004422();
s32 *Func_02004432();
s32 *Func_0200443e();
s32 *Func_0200444a();
s32 *Func_02004454();
s32 *Func_0200445c();
s32 *Func_02004468();
void Func_020044e0();
void Func_020044ea();
s32 *Func_02004490();
s32 *Func_0200449c();
s32 *Func_020044a8();

/* resource_3b4 actor presentation: cell repaints for slots 11 and 12. */

/*
 * Func_ names below are loader-relocated call words in this overlay's import
 * veneer table, not runtime addresses.  The declarations are old-style
 * because the same imports are reached with differing argument counts from
 * different call sites.
 */

/*
 * Actor presentation for resource_3b4.
 *
 * A Func_ name in the import veneer band 0x02002468-0x0200261f names the
 * main-image address held in the veneer's trailing word, not a runtime
 * address the call reaches directly.  Declarations are old-style because
 * those imports are reached with differing argument counts from different
 * call sites.
 */

/* Slot record lookup, then the mode imports. */

/* Complete 16-byte actor-15 mode wrapper before the no-op leaf at 0x9ec. */
void SceneActor_SetActor15ModeZero(void)
{
    Func_02002fd6(15, 0, 6);
}

void ActorPresentation_SetSceneCell58AndMarkActorEight(void)
{
    s32 extent = 42;
    u8 *entry;

    Func_02003f34(58, 41, 1, 1, extent, extent);
    entry = Func_02003fa2(8) + 35;
    *entry = 2;
}

/*
 * Repaint the four cells, then one cell for each of slots 11 and 12 at that
 * slot's own X tile.  The 92-byte owner at 0x02001df8 includes two bytes of
 * alignment and the pool word 0x000fffff.  The tile divisions must stay
 * spelled `/ 0x100000': the reference biases a negative value before the
 * arithmetic shift, which is exactly this truncating signed division.
 */
void ActorPresentation_RepaintCellsAtActorsElevenAndTwelve(void)
{
    s32 *slot;
    s32 tile;

    Func_02003b9e();

    slot = Func_02004344(11);
    tile = slot[2] / 0x100000;
    Func_020042fa(53, 55, 1, 1, tile, 55);
    slot = Func_02004368(12);
    tile = slot[2] / 0x100000;
    Func_0200431c(53, 55, 1, 1, tile, 55);
}

/*
 * Publish one marker byte at +35 to slots 8, 10, 11 and 12 according to slot
 * 0's height word at +12.  The 156-byte owner includes its one pool word,
 * 0x000fffff, read by the tile division.  The marker local is what carries
 * the value 2 across the high path, which branches over the clear to 0.
 * Slot 11's record is fetched once on each path rather than once before
 * them, and that duplication is what reproduces the reference.
 */
void SceneActor_PublishMarkerBySlotZeroHeight(void)
{
    s32 *slot0 = Func_02004422(0);
    u8 marker;

    if (slot0[3] > 0x100000) {                 /* +12 */
        marker = 2;
        ((u8 *)Func_02004432(8))[35] = marker;
        if (Func_0200443e(10)[3] == 0) {
            ((u8 *)Func_0200444a(10))[35] = marker;
        }
        ((u8 *)Func_02004454(11))[35] = marker;
    } else {
        if (Func_0200445c(10)[3] == 0 &&
            Func_02004468(0)[4] / 0x100000 > 56) {   /* +16 */
            Func_020044e0(10, 3);
        } else {
            Func_020044ea(10, 1);
            ((u8 *)Func_02004490(10))[35] = 1;
        }
        marker = 0;
        ((u8 *)Func_0200449c(11))[35] = marker;
    }

    ((u8 *)Func_020044a8(12))[35] = marker;
}
