#include "types.h"

/*
 * resource_3a4 owner at 0x02003484, 296 bytes: snap a record to its
 * tile center, settle it through two marks, then pull a companion
 * record from the scene table and stage it one tile-and-a-half north,
 * sprite bits juggled around the hop.
 *
 * Phase 1 (the argument record): stings 0x120 and 232, tile-center
 * coordinates `(word & 0xfff00000) + 0x80000` for x and z, +0x34 set
 * to 0x20000, Func_08009150(rec, cx, rec->y, cz) then
 * Func_08009158(rec), the +0x22 byte cleared, x/z overwritten with the
 * centers, +0x24/+0x2c zeroed, then mark 2, wait 15, mark 1, wait 30.
 * Its sprite: +0x27 = 1, Func_08009060(sprite->+0x2c) releases the
 * held handle, +0x2c zeroed, +0x25 = 1.
 *
 * Phase 2 (companion): Func_0808a400(*(handle *)(0x02000240 + 500))
 * fetches the companion from the shared scene table (+500, next to the
 * +450 selector the sibling rows read), sting 152, companion x/z set
 * to the same centers, +0x28 = 0x60000, +0x48 = 0x10000, sprite
 * flags9 &= 0xf3 (clear bits 2-3, the same two bits the 3c8 package's
 * spawner writes from `params & 3`), mark 7,
 * Func_08009150(companion, cx, companion->y, gz + 0x180000) -- the
 * target z here is the RAW masked z plus 1.5 tiles, not the +0x80000
 * center -- heading halfword set to 0x4000, wait 20, then
 * `flags9 = (flags9 & 0xf3) | 8` and sting 159.
 *
 * Complete owner: `push {r5, r6, lr}` plus r8-fp saves at 0x02003484
 * through the unwind at 0x200358c-0x200359c, trailing pool at
 * 0x200359e-0x20035a7.  The isolated `bx lr` at 0x20035a8 (plus a pad
 * halfword) between this pool and the quarter-turn row at 0x020035ac
 * is a separate degenerate no-op owner, not part of this row.  One
 * argument (the record pointer), void.
 *
 * All 15 bl sites resolve with the `+2` stored-displacement rule, all
 * veneers:
 *   0x2003498 (0x120), 0x200349e (232), 0x2003530 (152),
 *     0x2003588 (159) -> Func_080f9010   sound/sting
 *   0x20034c8, 0x200356a -> Func_08009150   move to (rec, x, y, z)
 *   0x20034ce -> Func_08009158             (rec)
 *   0x20034ea (2), 0x20034f8 (1), 0x2003558 (7) -> Func_08009080
 *   0x20034f0 (15), 0x20034fe (30), 0x2003576 (20) -> Func_080000c0
 *   0x2003512 -> Func_08009060             release handle
 *   0x2003528 -> Func_0808a400             handle -> record
 */

          /* sound/sting, established */
          /* move to (record, x, y, z) */
          /* (record) */
          /* mark record (record, n), established */
          /* wait n frames, established */
          /* release handle */
           /* handle -> record */

extern void Func_020071ba();
extern void Func_020071c0();
extern void Func_02006fc2();
extern void Func_02006fd0();
extern void Func_02006fbc();
extern void Func_02006f6a();
extern void Func_02006fca();
extern void Func_02006f78();
extern void Func_02006fdc();
extern u8 * Func_020071f2();
extern void Func_02007252();
extern void Func_0200702a();
extern void Func_02007064();
extern void Func_02006ff0();
extern void Func_020072aa();
void Func_02003484(u8 *record)
{
    u8 *sprite;
    u8 *companion;
    s32 gx;
    s32 gz;
    s32 cx;
    s32 cz;

    s32 permuted_30;
    Func_020071ba(0x120);      /* 144 << 1 */
    Func_020071c0(232);

    gx = *(s32 *)(record + 8) & 0xfff00000;
    cx = gx + 0x80000;                          /* 128 << 12 */
    gz = *(s32 *)(record + 16) & 0xfff00000;
    cz = gz + 0x80000;

    *(s32 *)(record + 0x34) = 0x20000;          /* 128 << 10 */
    Func_02006fc2(record, cx, *(s32 *)(record + 12), cz);
    Func_02006fd0(record);

    record[0x22] = 0;
    *(s32 *)(record + 0x24) = 0;
    *(s32 *)(record + 8) = cx;
    *(s32 *)(record + 16) = cz;
    *(s32 *)(record + 0x2c) = 0;

    Func_02006fbc(record, 2);
    Func_02006f6a(15);
    Func_02006fca(record, 1);
    Func_02006f78(30);

    permuted_30 = *(u8 **)(record + 0x50);
    sprite  = permuted_30;
    sprite[0x27] = 1;
    Func_02006fdc(*(s32 *)(sprite + 0x2c));
    *(s32 *)(sprite + 0x2c) = 0;
    sprite[0x25] = 1;

    companion = Func_020071f2(*(s32 *)((u8 *)0x02000240 + 500));
    Func_02007252(152);

    *(s32 *)(companion + 8) = cx;
    *(s32 *)(companion + 0x28) = 0x60000;       /* 192 << 11 */
    *(s32 *)(companion + 0x48) = 0x10000;       /* 128 << 9 */
    *(s32 *)(companion + 16) = cz;

    sprite = *(u8 **)(companion + 0x50);
    sprite[9] &= 0xf3;

    Func_0200702a(companion, 7);
    Func_02007064(companion, cx, *(s32 *)(companion + 12),
                  gz + 0x180000);               /* 192 << 13 */
    *(u16 *)(companion + 6) = 0x4000;           /* 128 << 7 */
    Func_02006ff0(20);

    sprite[9] = (sprite[9] & 0xf3) | 8;
    Func_020072aa(159);
}
