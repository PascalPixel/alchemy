/*
 * Actor presentation for resource_3b4.
 *
 * A Func_ name in the import veneer band 0x02002468-0x0200261f names the
 * main-image address held in the veneer's trailing word, not a runtime
 * address the call reaches directly.  Declarations are old-style because
 * those imports are reached with differing argument counts from different
 * call sites.
 */
#include "types.h"

/* Slot record lookup, then the mode imports. */
extern s32 *Func_02004422();
extern s32 *Func_02004432();
extern s32 *Func_0200443e();
extern s32 *Func_0200444a();
extern s32 *Func_02004454();
extern s32 *Func_0200445c();
extern s32 *Func_02004468();
extern void Func_020044e0();
extern void Func_020044ea();
extern s32 *Func_02004490();
extern s32 *Func_0200449c();
extern s32 *Func_020044a8();
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
