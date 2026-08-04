typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;

/*
 * resource_3c9 owner at 0x02005688, 24 bytes: a leaf that PARKS a
 * record -- it stamps the sentinel 0x80000000 into the three mirror
 * fields at +56/+60/+64, zeroes the three at +36/+40/+44, and clears
 * the u16 angle at +100.
 *
 * Complete owner: no prologue at all. `movs r3, #128 / lsls r3, #24` at
 * 0x02005688 through a bare `bx lr` at 0x0200569e, no literal pool, no
 * stack use, lr never touched. The next owner's prologue is exactly at
 * 0x020056a0, no gap. `arg0` is read (it is the base of every store) and
 * never returned; void.
 *
 * INVISIBLE TO SWEEPS A, B AND C AT ONCE, which is the point of it. It
 * saves no register, so sweep C's `push` key finds nothing. Its address
 * appears in no word of the image in the base + 0x8000 spelling, so
 * sweep B finds nothing -- every 4-aligned word was scanned for
 * 0x0200d688 with and without the Thumb bit and there are none. And
 * although two rows DO reach it by `bl`, sweep A looks for unowned
 * bl-reached PROLOGUES, and this has none, so sweep A discards it on
 * the same gate that rejects 185 published words tree-wide.
 * SWEEP D is the only instrument that saw it: `overlay_gaps
 * resource_3c9` reported `CODE-SUSPECT 0x5688-0x569f 24B after 0x4bec,
 * returns at 0x569e`. Same shape and very nearly the same size as
 * resource_3a4's 0x02003410, the row sweep D was written for.
 *
 * It was also identified BY HAND, twice, before any tool caught it:
 * this overlay's 0x020059f0 and 0x020056a0 both declare it as a
 * push-less leaf because the resolver returned `unknown` for their call
 * to it. Repeated hand-flagging did not make it a recorded
 * owner; the unkeyed sweep did. A note that something exists is not the
 * same as the tree knowing it exists.
 *
 * Both callers pass a record and neither uses a return value:
 * 0x020059f0 calls it on its own argument, and 0x020056a0's state-24
 * arm calls it on scene record 23 immediately after setting that
 * record's +8/+12/+16/+24/+28.
 *
 * WHAT IT SETTLES, and it corrects the earlier draft for 0x020037c4.
 * That row gates on
 * `follow[56] == 0x80000000 && follow[60] == follow[56] &&
 * follow[64] == follow[60]`; the condition was described as waiting
 * for the orbit steps to "park on the sentinel", flagging +60 as read
 * by that row and written by nothing. Both parts were wrong, and this
 * leaf is why: the orbit steps write live coordinates into +56 and +64
 * and never the sentinel, and it is THIS routine that writes all three
 * -- including the previously unexplained +60. So 0x020037c4 is not
 * watching an animation finish; it is testing whether the record has
 * been parked by Func_02005688. The 0x020037c4 header is corrected in
 * the same commit rather than left to be inherited.
 *
 * Uncertainties: 0x80000000 is read as a sentinel because it is stamped
 * into three position-family fields at once and then tested for
 * equality rather than compared for magnitude; nothing here proves it
 * is not a legitimate coordinate. +36/+40/+44 are cleared as a group
 * and their roles are not established.
 */

void Func_02005688(u8 *record)
{
    /* The row builds the sentinel as `movs r3, #128 / lsls r3, #24`. */
    *(s32 *)(record + 56) = (s32)0x80000000;
    *(s32 *)(record + 60) = (s32)0x80000000;
    *(s32 *)(record + 64) = (s32)0x80000000;

    *(s32 *)(record + 36) = 0;
    *(s32 *)(record + 40) = 0;
    *(s32 *)(record + 44) = 0;

    *(u16 *)(record + 100) = 0;
}
