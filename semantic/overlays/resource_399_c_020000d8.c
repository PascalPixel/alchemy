typedef unsigned char u8;
typedef unsigned short u16;
typedef signed short s16;
typedef signed int s32;

/*
 * resource_399 owner at 0x020000d8, 88 bytes: turn an entity towards its
 * follow target, clamping the per-call turn rate.
 *
 * TRANSPOSED from the byte-exact assets/code/resource_38f_c_02000114.c.  The two
 * owners are the same routine shared verbatim: over all 44 halfwords they differ
 * in exactly 2 places, both halves of the single BL pair.
 *
 * What was changed: the entry symbol, and the one call, re-resolved with
 * 'bun tools/overlay_call_targets.ts resource_399 00d8' to the veneer publishing
 * Func_08000100 (resource_38f's site resolves to the same import; the exact
 * source names it Func_02002af2 under the pre-correction 'bl' reading).
 *
 * The tool reports a SECOND site at +0x54 as 'unknown'.  It is not a call: the
 * span's last word is the literal 0xf896f001 (the negative turn clamp,
 * -0x07690fff), which happens to decode as a BL pair.  The disassembly confirms
 * it - the preceding halfwords are 'movs r0,#1 / pop {r5} / pop {r1} / bx r1'
 * followed by an alignment 'movs r0,r0', so the word is past the return and is
 * loaded by the 'ldr r2,[pc,#20]' inside the body.  There is exactly one call.
 *
 * Returns 1 unconditionally.
 */

struct Ent {
    u8 pad00[6];
    u16 f06;
    s32 f08;
    u8 pad0c[4];
    s32 f10;
    u8 pad14[0x46];
    u8 f5a;
    u8 pad5b[13];
    struct Ent *f68;
};

u16 Func_08000100();   /* atan2-style heading from a (dz, dx) delta */

s32 Func_020000d8(struct Ent *p) {
    struct Ent *q;
    u16 h;
    s32 t;
    s32 v;
    u8 *b;

    q = p->f68;
    if (q != 0) {
        b = &p->f5a;
        v = 0xfe;
        v &= *b;
        *b = v;
        h = Func_08000100(q->f10 - p->f10, q->f08 - p->f08);
        t = h;
        t -= p->f06;
        t <<= 16;
        t >>= 16;
        if (t != 0) {
            if (t > 0x1000) {
                t = 0x1000;
            }
            if (t < (s32)0xf896f001) {
                t = (s32)0xf896f001;
            }
            p->f06 = p->f06 + t;
        }
    }
    return 1;
}
