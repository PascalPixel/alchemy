typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed int s32;

/*
 * resource_3a4 owner at 0x02000d2c, 404 bytes: a camera orbit cutscene.
 * Fetch actor 10's state, open with a sound and a tint, then sweep the
 * heading at +30 through three phases -- accelerating up to 0x8fff,
 * decelerating back down to 0x7000, then a geometrically accelerating
 * sweep up to 0xbfff -- driving the position at +8 from the sine of the
 * heading every frame, and the depth at +12 from the cosine once past
 * halfway. Close on 0xc000 with a second sound, a tint, and this
 * overlay's own 0x02000ec0.
 *
 * A PUBLISHED CALLBACK (sweep B): no `bl` in the image reaches it. No
 * arguments, void.
 *
 * Complete owner: `push {r5, r6, r7, lr}` plus the r8-r11 save sequence at
 * 0x02000d2c-0x02000d38 and `sub sp, #4`, through the matching restore and
 * interworking return at 0x02000e9e-0x02000eae, followed by the four-word
 * literal pool 0x02000eb0-0x02000ebf; the next owner is 0x02000ec0, which
 * is itself still residue. measureSpan reports 388, undersizing by the
 * pool.
 *
 * IT CALLS THE OTHER RESIDUE ROW, and that resolves an open question.
 * The site at 0x02000e96 encodes `f000 ff5f`, whose stored displacement +2
 * is 0xec0 -- an IN-IMAGE prologue, not a veneer: Func_02000ec0(5). So
 * 0x02000ec0's membership in sweep A (bl-reached, caller exists in image)
 * is explained by this row, which is itself in sweep B. A published
 * callback is the caller of a called function, which is exactly why an
 * image-wide BL walk starting from reachable code finds neither.
 *
 * Twenty-three call sites, every one resolved with the `+2` rule via
 * `bun tools/overlay_call_targets.ts resource_3a4 0xd2c 0xec0` and none
 * read from the listing's arrows, which print twenty-three nonexistent
 * 0x02004xxx addresses. They collapse onto ten targets -- nine veneers and
 * the one in-image prologue -- with four sites sharing Func_080000c0, four
 * Func_080091f0, four Func_080f9010, three Func_08000120 and three
 * Func_0808a010.
 *
 * The trig pair is the established one: Func_08000120 is the sine lobe and
 * Func_08000118 the cosine lobe, both taking an angle. Func_080000c0(1) is
 * the one-frame yield that makes each of the three loops a real animation
 * rather than a computation; Func_0808a080(10) returns the state record.
 *
 * The three sweeps differ in their step law, and getting that wrong is the
 * whole risk in this row:
 *   phase 1  a running accumulator gains 0x80000 per frame and the heading
 *            gains (accumulator >> 16), so the step grows 8, 16, 24 ...
 *            -- linear acceleration. Runs while heading <= 0x8fff.
 *   phase 2  the same accumulator law, SUBTRACTED from the heading, and
 *            the heading is re-read after the yield. Runs while
 *            heading > 0x7000.
 *   phase 3  the accumulator is instead multiplied: v = v + (v >> 3) each
 *            frame, starting at 8 -- geometric, 1.125 per frame -- and the
 *            loop test is on heading + v, not heading. Runs while
 *            heading + v <= 0xbfff.
 * Phase 3's depth write at +12 is gated on heading > 0x8000, so it only
 * applies over the second half of the sweep, and it is subtracted from the
 * +12 value captured ONCE before any of this at 0x02000d42 -- a stack slot,
 * not a re-read, so it is the original depth every frame.
 *
 * Constants are written in the shifted-literal form the encodings build:
 * 128 << 12, 128 << 10, 128 << 9, 128 << 8, 128 << 7, 224 << 7, 192 << 10,
 * 192 << 8. The two 0xe666 tints and the 0x121 sound id come from the pool.
 *
 * Field roles are recorded by offset: the state record's +8 is the swept
 * position, +12 the depth, +80 the actor whose halfword at +30 is the
 * heading. Not declared as a type.
 */

extern s32 Func_0808a080(s32 id);
extern void Func_0808a018(void);
extern void Func_0808a020(void);
extern void Func_080f9010(s32 sound_id);
extern void Func_080091f0(s32 arg0, s32 arg1, s32 arg2);
extern void Func_0808a010(s32 frames);
extern void Func_080000c0(s32 frames);
extern s32 Func_08000120(s32 angle);    /* trig, sine lobe */
extern s32 Func_08000118(s32 angle);    /* trig, cosine lobe */
extern void Func_02000ec0(s32 arg0);

void Func_02000d2c(void)
{
    u8 *state;
    u8 *actor;
    s32 origin;
    s32 depth;
    s32 accumulator;
    s32 step;
    s32 lobe;
    s32 cosine;
    u16 heading;

    state = (u8 *)Func_0808a080(10);
    depth = *(s32 *)(state + 12);
    origin = *(s32 *)(state + 8);
    actor = *(u8 **)(state + 80);

    Func_0808a018();
    Func_080f9010(141);
    Func_080091f0(128 << 10, 128 << 9, 128 << 9);
    Func_0808a010(10);
    Func_080f9010(0x121);
    Func_080091f0(-1, -1, 0xe666);
    Func_0808a010(20);

    /* Phase 1: linear acceleration up to 0x8fff. */
    accumulator = 0;
    for (;;) {
        accumulator += 128 << 12;
        *(u16 *)(actor + 30) =
            (u16)(*(u16 *)(actor + 30) + ((u32)accumulator >> 16));
        *(s32 *)(state + 8) =
            origin + (Func_08000120(*(u16 *)(actor + 30) + (128 << 7)) << 4);
        heading = *(u16 *)(actor + 30);
        if (heading > 0x8fff) {
            break;
        }
        Func_080000c0(1);
    }

    /* Phase 2: the same law, subtracted, down to 224 << 7. */
    accumulator = 0;
    for (;;) {
        accumulator += 128 << 12;
        *(u16 *)(actor + 30) = (u16)(heading - ((u32)accumulator >> 16));
        *(s32 *)(state + 8) =
            origin + (Func_08000120(*(u16 *)(actor + 30) + (128 << 7)) << 4);
        heading = *(u16 *)(actor + 30);
        if (heading <= (224 << 7)) {
            break;
        }
        Func_080000c0(1);
        heading = *(u16 *)(actor + 30);
    }

    /* Phase 3: geometric step, v *= 1.125 per frame, up to 0xbfff. */
    accumulator = 128 << 12;
    for (;;) {
        accumulator = (s32)((((u32)accumulator >> 16) +
                             ((u32)accumulator >> 19)) << 16);
        step = (s32)((u32)accumulator >> 16);
        *(u16 *)(actor + 30) = (u16)(step + heading);

        lobe = Func_08000120(*(u16 *)(actor + 30) + (128 << 7));
        cosine = Func_08000118(*(u16 *)(actor + 30) + (128 << 8));
        *(s32 *)(state + 8) = origin + (lobe << 4);

        if (*(u16 *)(actor + 30) > (128 << 8)) {
            *(s32 *)(state + 12) = depth - (cosine << 3);
        }

        if ((s32)(*(u16 *)(actor + 30) + step) > 0xbfff) {
            break;
        }
        Func_080000c0(1);
        heading = *(u16 *)(actor + 30);
    }

    Func_080000c0(1);
    *(u16 *)(actor + 30) = 192 << 8;
    Func_080f9010(183);
    Func_080091f0(192 << 10, 192 << 10, 128 << 9);
    Func_0808a010(20);
    Func_080f9010(0x121);
    Func_080091f0(-1, -1, 0xe666);
    Func_02000ec0(5);
    Func_0808a020();
}
