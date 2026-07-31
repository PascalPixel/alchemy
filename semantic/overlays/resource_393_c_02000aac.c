typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;

/*
 * resource_393 owner at 0x02000aac, 248 bytes: the "climb down" beat.  Probe
 * the square the caller names; if the party can enter it, play the descent -
 * suppress player control, swap the party's pose, drop it two tiles, then lift
 * it two tiles back - and report that the beat ran.
 *
 * Complete owner: 'push {r5, r6, r7, lr}' plus the 'push {r6, r7}' that banks
 * r8/sl at 0x02000ab2, and the single interworking epilogue at 0x02000b94.
 * One four-byte literal pool at 0x02000ba0 (0xffff0000), derived from the
 * control-flow walk: the body runs straight through it into the epilogue, and
 * nothing branches into it.  244 code + 4 pool = 248, the advertised span.
 *
 * Return type from the epilogue rule: 'pop {r1} ; bx r1' with N != 0, so r0
 * survives and is the result - 1 when the beat ran, 0 when the probe refused.
 *
 * Calls: 16 sites over 10 targets, resolved with
 * 'bun tools/overlay_call_targets.ts resource_393 0aac' (never from the
 * listing's printed 'bl' annotations, which omit the +2 rule).
 *
 * Two details that a register-window reading gets wrong and are deliberate
 * here:
 *  - Func_0808a100 is reached with r1 still holding the 7 stored into the
 *    control byte two instructions earlier.  It is a genuine second argument,
 *    not a leftover: the same import takes two arguments at every other site in
 *    the tree.
 *  - the argument register r5 is REASSIGNED at 0x02000b3e to the pooled step
 *    -0x00010000, and again at 0x02000b64 to +0x00010000.  After the pose swap
 *    it no longer names the probe.
 *
 * UNCERTAINTY: the byte at record + 0x55 is a control/permission flag.  It is
 * masked with 0x7e (clearing bits 0 and 7) for the duration of the beat, then
 * set to the probe result - provably zero on this path - and finally restored
 * from the value saved on entry.  Only the bracket is established, not the
 * meaning of the individual bits.
 */

/* Party/actor record.  Only the fields this beat touches are named. */
struct Beat393Record {
    u8 unknown_00[0x0c];
    s32 x;                          /* 0x0c, 16.16 */
    u8 unknown_10[4];
    s32 z;                          /* 0x14, 16.16 */
    u8 unknown_18[0x10];
    s32 unknown_28;                 /* 0x28 */
    u8 unknown_2c[4];
    s32 unknown_30;                 /* 0x30 */
    s32 unknown_34;                 /* 0x34 */
    u8 unknown_38[0x1d];
    u8 control;                     /* 0x55 */
};

/*
 * The caller's probe.  The s16 views at +0x02 and +0x0a are the integer parts
 * of the 16.16 words at +0x00 and +0x08 - the tile-coordinate idiom, not two
 * extra fields.
 */
struct Beat393Probe {
    s32 x;
    s32 y;
    s32 z;
};

void Func_080000c0();               /* wait N frames */
void Func_08009080();               /* select the record's pose */
s32 Func_080091d8();                /* probe: 0 when the square may be entered */
void Func_080091e0();               /* set presentation mode (record, phase) */
void Func_0808a018();               /* begin an uninterruptible beat */
void Func_0808a020();               /* end it */
struct Beat393Record *Func_0808a080();  /* scene record by index */
void Func_0808a0c0();               /* place a marker at a tile (id, x, z) */
void Func_0808a100();               /* emit a cue (id, parameter) */
void Func_080f9010();               /* play a sound effect */

s32 Func_02000aac(struct Beat393Probe *probe)
{
    struct Beat393Record *record = Func_0808a080(0);
    u8 saved = record->control;
    s32 blocked = Func_080091d8(record, probe);
    s32 step;

    if (blocked != 0) return 0;

    Func_0808a018();
    Func_08009080(record, 6);
    Func_080000c0(6);
    Func_080f9010(0x98);
    Func_08009080(record, 7);

    record->unknown_30 = 192 << 10;
    record->unknown_34 = 128 << 10;
    record->unknown_28 = 128 << 11;
    record->control = (u8)(record->control & 0x7e);

    Func_080091e0(record, 0);
    Func_0808a0c0(0, (s16)(probe->x >> 16), (s16)(probe->z >> 16));
    Func_08009080(record, 6);
    Func_080091e0(record, 1);

    record->control = (u8)blocked;
    Func_0808a100(10, 7);

    /* Two tiles down, a beat apart. */
    step = (s32)0xffff0000;
    record->x += step;
    record->z += step;
    Func_080000c0(2);
    record->x += step;
    record->z += step;
    Func_080000c0(10);

    /* ...then two tiles back up. */
    step = 128 << 9;
    record->x += step;
    record->z += step;
    Func_080000c0(4);
    record->x += step;
    record->z += step;

    record->control = saved;
    Func_0808a020();
    return 1;
}
