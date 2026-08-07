#include "types.h"

/*
 * resource_3a4 owner at 0x02002d58, 196 bytes: the cutscene bracket
 * around the sink-and-flicker twin Func_020006dc -- clear two actors,
 * flip the shared workspace's +0x1c0 mode word to 0x201, run a camera
 * move to (200.0, -, 249.0), fire the twin, return the camera to the
 * cached record's own x/y/z, play script blob 0x4d6c, and restore the
 * mode word to 0x204.
 *
 * This row is the twin pair's single caller (overlay_unindexed.ts
 * counts one incoming site on 0x020006dc; it is this one).  Note the
 * misdecode trap again: overlay_show prints the site at 0x2002dd6 as
 * `bl 0x20034b4`, which lands mid-stream inside the 0x3484 row; the
 * `+2` stored-displacement rule resolves it to image offset 0x6dc.
 *
 * Complete owner: `push {r5, r6, lr}` plus the r8 save at 0x02002d58
 * through `pop {r3} / mov r8, r3 / pop {r5, r6} / pop {r0} / bx r0` at
 * 0x2002dfe-0x2002e06, trailing five-word literal pool at
 * 0x2002e08-0x2002e1b, immediately followed by the recorded owner at
 * 0x02002e1c (resource_3a4_c_02002e1c.c).  No arguments, void.
 *
 * 0x03001ebc is the established shared-workspace pointer (the
 * Data_03001ebc family); +0x1c0 (224 << 1) is written 0x201 on entry
 * and 0x204 on exit, the value held in r8 across the body.
 * 0x0200cd6c is image offset 0x4d6c, a script blob in the 0x3d24+
 * data band, same class as the twins' Data_0200bd48.
 *
 * All 19 bl sites resolve with the `+2` rule; 18 veneers, one local:
 *   0x2002d60 -> Func_0808a080   record accessor (slot 0)
 *   0x2002d66 -> Func_0808a018   open bracket on the record
 *   0x2002d70, 0x2002d7a -> Func_0808a0f0   place entity (id, x, z)
 *   0x2002d8c -> Func_0808a360, 0x2002d90 -> Func_0808a370
 *   0x2002d96, 0x2002dd2 -> Func_0808a010   frame wait
 *   0x2002d9e -> Func_0808a398   (id, n)
 *   0x2002da6 -> Func_0808a060   (id, 0)
 *   0x2002daa -> Func_0808a228   record for the +0x55 clear
 *   0x2002db8 -> Func_0808a208   (0x19999, 0x3333)
 *   0x2002dc8, 0x2002de2 -> Func_0808a210   camera (x, y, z, mode)
 *   0x2002dcc, 0x2002de6 -> Func_0808a218
 *   0x2002dd6 -> image offset 0x6dc          local Func_020006dc
 *   0x2002dec -> Func_0808a040   play script blob
 *   0x2002dfa -> Func_0808a020   close bracket
 */

extern u8 Data_0200cd6c[];     /* image offset 0x4d6c: script blob */

u8 *Func_0808a080();           /* scene-record accessor, established */
void Func_0808a018();          /* open bracket, established */
void Func_0808a0f0();          /* place entity (id, x, z), established */
void Func_0808a360();
void Func_0808a370();
void Func_0808a010();          /* frame wait, established */
void Func_0808a398();          /* (id, n), established */
void Func_0808a060();          /* (id, 0), established */
u8 *Func_0808a228();
void Func_0808a208();          /* pair of 16.16 rates, established */
void Func_0808a210();          /* camera (x, y, z, mode), established */
void Func_0808a218();
void Func_020006dc(void);
void Func_0808a040();          /* play script blob, established */
void Func_0808a020();          /* close bracket, established */

void Func_02002d58(void)
{
    u8 *record;

    record = Func_0808a080(0);
    Func_0808a018(record);
    Func_0808a0f0(10, 0, 0);
    Func_0808a0f0(8, 0, 0);

    *(s32 *)(*(u8 **)0x03001ebc + 0x1c0) = 0x201;

    Func_0808a360();
    Func_0808a370();
    Func_0808a010(20);
    Func_0808a398(202, 3);
    Func_0808a060(202, 0);
    Func_0808a228()[0x55] = 0;

    Func_0808a208(0x19999, 0x3333);
    Func_0808a210(200 << 15, 0, 249 << 16, 1);
    Func_0808a218();
    Func_0808a010(20);

    Func_020006dc();

    Func_0808a210(*(s32 *)(record + 8), *(s32 *)(record + 12),
                  *(s32 *)(record + 16), 1);
    Func_0808a218();
    Func_0808a040(Data_0200cd6c);

    *(s32 *)(*(u8 **)0x03001ebc + 0x1c0) = 0x204;

    Func_0808a020();
}
