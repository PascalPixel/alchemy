typedef signed int s32;
typedef unsigned char u8;

/*
 * Resource 3c8 at 0x02004a2c: 28-call cutscene step that lowers two scene
 * objects in scripted stages and then in a 128-frame loop.
 *
 * The owner is complete: `push {r5, r6, r7, lr}` plus the high-register save
 * `mov r7, sl ; mov r6, r8 ; push {r6, r7}` at 0x02004a2c, a 4-byte frame,
 * and the matching unwind at 0x02004bba..0x02004bc6, followed by a four-word
 * literal pool at 0x02004bc8-0x02004bd7.  No live frame or register state
 * escapes the row.
 *
 * `pop {r0} ; bx r0` means r0 holds the popped return address, so the owner
 * returns nothing.
 *
 * Field offsets: 0x0c is the `y` of the byte-exact sibling 0x020009c8, which
 * writes exactly the same `0x00820000` into it; 0x44/0x48 are that sibling's
 * `speed`/`range` pair; 0x55 is the `mode55` byte of
 * semantic/overlays/resource_3c8_c_02001d48.c; 0x6c is the callback slot of
 * the byte-exact sibling 0x02000754.  Offset 0x14 is a second word that moves
 * in lockstep with 0x0c on the first object and is left unnamed.
 *
 * `objectA->callback = 0x0200c969` installs the routine at file offset
 * 0x4968: at this overlay's proven 0x02008000 link base, 0x0200c969 - 0x8000
 * - 1 = 0x02004968, which is a real `push {r5, r6, r7, lr}` prologue.  The
 * inventory row for it starts two bytes earlier at 0x02004966 because of the
 * alignment `movs r0, r0` that precedes the prologue.  Unlike the other
 * install sites in this package, this one is never cleared before the owner
 * returns - recorded as observed.
 *
 * The five scripted descents add 0xfffd0000, 0xfffe0000, 0x00020000,
 * 0x00020000 and 0x00010000 to the same three slots, each between two service
 * calls; the loop then adds 0x00010000 once per iteration for 128 iterations
 * (the counter lives in the single stack word and is compared unsigned
 * against 127).  The store order differs between the scripted groups (B, A,
 * A+0x14) and the loop (A, A+0x14, B); both are preserved as written.
 *
 * Constants: 130<<16 = 0x00820000, 128<<8 = 0x8000, 128<<1 = 256,
 * 128<<10 = 0x00020000, 128<<9 = 0x00010000, 154<<1 = 308; 0x0101,
 * 0xfffd0000, 0xfffe0000 and 0x0200c969 are pooled.
 *
 * All 28 `bl` sites reach 28 distinct targets, all accounted for:
 * 0x020098e6, 0x020098ee, 0x020098dc, 0x020099de, 0x02009852, 0x02009808,
 * 0x02009a48, 0x02009a5c, 0x02009a8a, 0x02009920, 0x0200992c, 0x02009a1c,
 * 0x020099ac, 0x0200987a, 0x02009894, 0x020098b0, 0x020098c8, 0x02009ac6,
 * 0x02009a56, 0x020099ec, 0x020099f6, 0x02009ad6, 0x02009ade, 0x02009b7c,
 * 0x02009b84, 0x02009a0a, 0x0200994e, 0x02009b5c.  Import naming follows the
 * note in resource_3c8_c_020002f0.c; these all sit above the overlay image,
 * so they are resident service identities.
 */

struct Object_02004a2c {
    u8 unknown_00[0x0c];
    s32 y;                      /* 0x0c */
    u8 unknown_10[4];
    s32 unk14;                  /* 0x14 */
    u8 unknown_18[0x2c];
    s32 speed;                  /* 0x44 */
    s32 range;                  /* 0x48 */
    u8 unknown_4c[9];
    u8 mode55;                  /* 0x55 */
    u8 unknown_56[0x16];
    s32 callback;               /* 0x6c */
};

/* Used for their return values. */
void Func_080000c0();
void Func_08009128();
void Func_080770c8();
void Func_0808a010();
void Func_0808a018();
struct Object_02004a2c *Func_0808a080();
void Func_0808a100();
void Func_0808a1e0();
void Func_0808a1f0();
void Func_0808a210();
void Func_0808a248();
void Func_0808a360();
void Func_0808a370();
void Func_080f9010();

/* Old-style declarations: the imports' real interfaces are not known here. */

void Func_02004a2c(void)
{
    struct Object_02004a2c *objectA;
    struct Object_02004a2c *objectB;
    s32 step;
    s32 frame;

    objectA = Func_0808a080(0);
    objectB = Func_0808a080(20);

    /* No argument register is written here; r0 still holds the pointer just
     * returned above, and that dataflow is preserved as written. */
    Func_0808a018(objectB);

    Func_0808a210(-1, -1, -1, 0);
    Func_08009128();
    Func_080000c0(1);

    objectA->y = 0x00820000;
    objectA->range = 0x8000;
    objectA->speed = 0;
    objectA->mode55 = 0;

    Func_0808a360();
    Func_0808a370();
    Func_080f9010(204);
    Func_0808a010(30);

    objectA->mode55 = 3;

    Func_0808a010(24);
    Func_0808a1f0(0, 0x101);
    Func_0808a100(0, 22);

    objectA->mode55 &= 0xfe;

    objectB->y += 0xfffd0000;
    objectA->y += 0xfffd0000;
    objectA->unk14 += 0xfffd0000;
    Func_080000c0(2);

    objectB->y += 0xfffe0000;
    objectA->y += 0xfffe0000;
    objectA->unk14 += 0xfffe0000;
    Func_080000c0(10);

    step = 0x00020000;
    objectB->y += step;
    objectA->y += step;
    objectA->unk14 += step;
    Func_080000c0(4);

    objectB->y += step;
    objectA->y += step;
    objectA->unk14 += step;
    Func_080000c0(4);

    step = 0x00010000;
    objectB->y += step;
    objectA->y += step;
    objectA->unk14 += step;

    objectA->mode55 = 0;
    objectB->mode55 = 0;

    Func_0808a1f0(0, 256);
    Func_0808a100(0, 1);
    Func_0808a010(40);

    objectA->callback = 0x0200c969;     /* = Func_02004968 | 1 */
    Func_0808a010(60);

    Func_0808a1e0(0, 1);
    Func_0808a1e0(20, 1);
    Func_080f9010(17);
    Func_080f9010(308);
    Func_080770c8(0x101);

    for (frame = 0; frame <= 127; frame++) {
        objectA->y += step;
        objectA->unk14 += step;
        objectB->y += step;
        Func_080000c0(1);
    }

    Func_0808a248(21);
}
