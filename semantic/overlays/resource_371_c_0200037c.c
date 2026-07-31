typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;

/*
 * Resource 371 owner at 0x0200037c (292 bytes).
 *
 * Complete owner: `push {lr}` at 0x0200037c and `pop {r1} ; bx r1` at
 * 0x0200045e.  The popped register is r1, not r0, so r0 survives and IS the
 * result.  0x0200037c + 292 = 0x020004a0, and the span decomposes as
 * dispatcher (0x0200037c-0x02000397), 32-entry jump table
 * (0x02000398-0x02000417, data), nine case bodies
 * (0x02000418-0x02000461) and the literal pool
 * (0x02000464-0x0200049f).
 *
 * The jump table is a third independent witness for the 0x02008000 link
 * base, of exactly the shape HANDOVER records for `resource_3bf`: the
 * dispatcher loads its base as the pool word 0x02008398 while the table is
 * physically at file offset 0x398, and every one of its 32 entries
 * (0x02008418, 0x02008430, 0x0200843e, 0x02008442, 0x02008446, 0x0200844a,
 * 0x0200844e, 0x02008452, 0x02008456) is 0x8000 above a case body inside
 * this same function.  `mov pc, r3` is the dispatch — it is not a return and
 * does not close the owner.
 *
 * Selects the placement/spawn table for the current scene.  The selector is
 * `Data_02000240[225]` read as a signed halfword; `subs #49 ; cmp #31 ; bhi`
 * makes the covered range 49..80 inclusive and sends everything else to the
 * default.  The default result 0x0200db84 is the same placement table the
 * owner at 0x02002768 patches, and the 0x0200e0xx-0x0200e3xx results are its
 * siblings in the same in-image data band (0x0200e3f4, the spawn rule table,
 * sits immediately after 0x0200e3c4).
 *
 * Two cases are conditional and fall through to the default when their
 * condition fails; case 49 needs both 0x020046a8(0x94f) to be zero and
 * 0x020046b2(0x941) to be non-zero, and case 64 needs 0x020046c0(0x85a) to
 * be zero.  The default arm calls 0x020046ee(0x235) before returning.
 *
 * UNCERTAINTY: the inventory row reports `calls=1` for this owner, but four
 * distinct `bl` targets are present.  The undercount is a decoding artifact
 * of the 128-byte jump table sitting in the middle of the executable span —
 * it disassembles as plausible `strh` pairs — not a missing call.  All four
 * are accounted for below.
 *
 * Call-target convention: each `bl` is named by the address its call site
 * computes, which is a per-call-site label for a load-time-relocated import
 * (see resource_371_c_0200008c.c).  Old-style declarations, because the
 * interfaces are unknown.
 */

extern s16 Data_02000240[];

extern u8 Data_0200db84[];
extern u8 Data_0200e04c[];
extern u8 Data_0200e154[];
extern u8 Data_0200e1e4[];
extern u8 Data_0200e25c[];
extern u8 Data_0200e28c[];
extern u8 Data_0200e2ec[];
extern u8 Data_0200e394[];
extern u8 Data_0200e3c4[];

s32 Func_080770c0();
void Func_080770c8();

u8 *Func_0200037c(void)
{
    s16 *table = Data_02000240;
    s32 scene = table[225];

    switch (scene) {
    case 49:
        if (Func_080770c0(0x94f) == 0 && Func_080770c0(0x941) != 0) {
            return Data_0200e154;
        }
        break;
    case 64:
        if (Func_080770c0(0x85a) == 0) {
            return Data_0200e04c;
        }
        break;
    case 65:
    case 70:
        return Data_0200e1e4;
    case 66:
    case 67:
    case 68:
    case 69:
    case 75:
        return Data_0200e25c;
    case 71:
        return Data_0200e28c;
    case 72:
        return Data_0200e394;
    case 73:
        return Data_0200e3c4;
    case 80:
        return Data_0200e2ec;
    default:
        break;
    }

    Func_080770c8(0x235);
    return Data_0200db84;
}
