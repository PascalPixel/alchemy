/* 2026-09-29 alchemy permute: score 775 to 435 on the permuter's scorer (0
   is exact); remaining 20 register-only, 1 operand, 5 reordered, all the
   cursor pointer in r5 where the reference has r7 and px in r7 where it
   has r5. Kept, after reverting every permuter rewrite that did not pay:
   the step count declared after the coordinates, the cursor y offset and
   the x offset spelled as additions, y offset after the clamp, the y clamp
   through a halfword copy (tagged), and the steps divided by the step
   count itself, which is the __divsi3 call the reference makes with 2 (a
   constant divisor would become shifts). */
/* 2026-09-28: 264/264 bytes, 34 aligned edits (was 40). Reading the cursor
 * y into py before its eight-pixel pull-back, and again after it, gives the
 * reference's single y load kept in r6 for the test and the slide. The
 * remaining edits are register roles only: the reference allocates px and
 * py (r5/r6) before the cursor pointer (r7); here the cursor wins r5, since
 * its 21 references over 72 insns outrank both coordinates. */
#include "TYPES.H"

/* main:080a1ac0, complete 264-byte body through 080a1bc8.
 * H1 transfers the exact position-cursor OAM bitfields and chained position
 * stores, plus named menu/callee interfaces. The two-frame counter starts
 * before the skip-slide guard in the ROM. Caller 080a63e4 supplies x/y;
 * __divsi3 is signed and WaitFrames returns void. Predict exact bitfield
 * reads/writes and the original four-byte frame including literal pools.
 * Gate: whole-owner exact bytes plus compare/test/coverage/verify.
 * H1: 262/264 bytes, 85 aligned edits. OAM accesses match in shape, but the
 * loop rotates WaitFrames to its head, initial pools move into the body,
 * and y's fixed-point conversion precedes rather than follows the x divide.
 * Budget: one corrected model and two evidence-backed variants.
 * H2: explicit tail-wait loop avoids the diagnosed loop rotation; a u16
 * zero in the skip path tests the short literal-pool reach recipe used by
 * other exact halfword stores. Prediction: original loop edges and pools.
 * H2: 256/264 bytes, 71 aligned edits, equal topology. Halfword zero puts
 * the initial pool at the exact +0x2c boundary. The goto loop prevents
 * the 0xffff mask from living across WaitFrames, removing the reference
 * four-byte spill frame. Y conversion is still before the first divide.
 * H3: counted do loop with a conditional wait retains an optimizer-visible
 * loop without executing a wait on the last frame. Keep y in pixel units
 * across the x divide, then convert it in place as the ROM does. Predict
 * reference mask spill/frame, tail wait and coordinate-carrier lifetimes.
 * H3: 264/264 bytes, 64 differing halfwords, 40 aligned edits with equal
 * topology. Frame, early pool and tail wait now agree. Cursor/x carriers
 * remain r5/r7 instead of r7/r5; initial OAM scheduling and a y-carrier copy
 * before the first divide remain. STOP: three structural models exhausted;
 * retain this draft until new lifetime evidence, not register permutations.
 */

struct CursorAttributes {
    u16 y : 8;
    u16 affine_mode : 2;
    u16 object_mode : 2;
    u16 mosaic : 1;
    u16 palette_256 : 1;
    u16 shape : 2;
    u16 x : 9;
    u16 matrix : 5;
    u16 size : 2;
};

struct CursorIcon {
    u8 unknown_00[6];
    u16 x;                          /* 0x06 */
    u16 y;                          /* 0x08 */
    u8 unknown_0a[10];
    struct CursorAttributes attributes; /* 0x14 */
};

struct CursorWindow {
    u8 unknown_00[12];
    u16 tile_x;                     /* 0x0c */
    u16 tile_y;                     /* 0x0e */
};

struct CursorWork {
    u8 unknown_000[0x10];
    struct CursorWindow *window;    /* 0x010 */
    struct CursorIcon *cursor;      /* 0x014 */
    u8 unknown_018[0x20a];
    u16 skip_slide;                 /* 0x222 */
};

extern struct CursorWork *gMenuWork;

void WaitFrames(s32 frames);

void UiMenu_SlideCursor(s32 x, s32 y)
{
    struct CursorWork *work = gMenuWork;
    struct CursorIcon *cursor;
    struct CursorWindow *window;
    s32 px;
    s32 py;
    s32 cnt;
    s32 dx;
    s32 dy;
    u16 cursor_y;

    cnt = 2;
    if (work->skip_slide != 0) {
        u16 zero = 0;
        work->skip_slide = zero;
        return;
    }
    cursor = work->cursor;
    cursor->x = cursor->attributes.x + 64;
    cursor->y = 64 + cursor->attributes.y;
    x = x + 64;
    if (cursor->x - 8 > 0) {
        cursor->x -= 8;
    }
    cursor_y = cursor->y;
    /* FAKEMATCH: the halfword copy and the unsigned pull-back only keep the reference allocation. */
    if ((py = cursor_y) - 8 > 0)
        cursor->y = (u32)py - 8;
    py = cursor->y;
    y += 64;
    px = cursor->x << 4;
    dx = ((x << 4) - px + 1) / cnt;
    py <<= 4;
    dy = ((y << 4) - py + 1) / cnt;
    do {
        window = work->window;
        px += dx;
        cursor->attributes.x = cursor->x =
            (px >> 4) + (window->tile_x << 3) - 56;
        py += dy;
        cursor->attributes.y = cursor->y =
            (py >> 4) + (window->tile_y << 3) - 56;
        cnt--;
        if (cnt != 0)
            WaitFrames(1);
    } while (cnt != 0);
}
