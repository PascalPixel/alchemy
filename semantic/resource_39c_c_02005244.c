#include "types.h"

/*
 * resource_39c movement owner at 0x02005244, complete 324-byte span through
 * its five-word literal pool. The two bytes at 0x02005242 are alignment left
 * by the preceding structural owner, not part of this function.
 *
 * The object is moved toward the record linked at +0x68. A coarse tile-space
 * distance chooses the inexpensive path; nearby objects are remeasured in
 * fixed-point, then either snap to the target or advance by at most one
 * eighth of the remaining distance (also capped by the configured maximum).
 * The final two byte writes select one of two seven-frame presentation banks
 * from bit 1 of the global frame state and mark the display record dirty.
 *
 * The apparent returns at 0x020052d2, 0x020052de, 0x02005326 and
 * 0x0200533a are calls through relocated IWRAM arithmetic helpers, not owner
 * boundaries. Three more BL sites enter call-through veneers backed by the
 * same helper bank. All seven indirect operations are expressed as ordinary
 * typed C calls; Func_08000110 is the owner's sole direct external callee.
 */

typedef s32 (*BinaryFixedHelper)(s32 left, s32 right);





extern s32 Func_0200af24(s32 value);
extern s32 Func_0200ac82(s32 value);
extern s32 Func_0200afac(s32 distance, s32 delta);
extern s32 Func_0200afc2(s32 distance, s32 delta);
void Func_02005244(u8 *object)
{
    u8 *target = *(u8 **)(object + 104);
    s32 targetX = *(s32 *)(target + 8);
    BinaryFixedHelper fixedMultiply = (BinaryFixedHelper)0x03000118;
    s32 targetZ = *(s32 *)(target + 16);
    s32 deltaX = targetX - *(s32 *)(object + 8);
    s32 deltaZ = targetZ - *(s32 *)(object + 16);
    s32 tileX = (deltaX < 0 ? deltaX + 0xffff : deltaX) >> 16;
    s32 tileZ = (deltaZ < 0 ? deltaZ + 0xffff : deltaZ) >> 16;
    s32 distance;
    s32 step;
    u8 *display;
    u8 *metadata;
    u32 frameState;

    *(s32 *)(object + 48) = 0x20000;
    *(s32 *)(object + 52) = 0x10000;
    *(s32 *)(object + 56) = (s32)0x80000000;
    *(s32 *)(object + 60) = (s32)0x80000000;
    *(s32 *)(object + 64) = (s32)0x80000000;

    distance = Func_0200af24(tileX * tileX + tileZ * tileZ) << 16;
    deltaX = targetX - *(s32 *)(object + 8);
    deltaZ = targetZ - *(s32 *)(object + 16);

    if (distance < 0x400000) {
        distance = Func_0200ac82(
            fixedMultiply(deltaX, deltaX) +
            fixedMultiply(deltaZ, deltaZ));
    }

    step = (distance < 0 ? distance + 7 : distance) >> 3;
    if (step > *(s32 *)(object + 48)) {
        step = *(s32 *)(object + 48);
    }

    if (distance < 0x4000) {
        *(s32 *)(object + 8) = targetX;
        *(s32 *)(object + 16) = targetZ;
    } else {
        if (distance > step) {
            deltaX = fixedMultiply(Func_0200afac(distance, deltaX), step);
            deltaZ = fixedMultiply(Func_0200afc2(distance, deltaZ), step);
        }
        *(s32 *)(object + 8) += deltaX;
        *(s32 *)(object + 16) += deltaZ;
    }

    frameState = *(volatile u32 *)0x03001e40;
    display = *(u8 **)(object + 80);
    metadata = *(u8 **)(display + 40);
    metadata[5] = (u8)(((frameState >> 1) & 1) * 7);
    display[37] = 1;
}
