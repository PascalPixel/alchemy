typedef signed short s16;
typedef signed int s32;
typedef unsigned char u8;
typedef unsigned int u32;

/*
 * Resource 3bf per-mode task installer at 0x02004f60, 388 bytes including
 * the nine-word trailing pool through 0x020050e3.
 *
 * Complete owner: `push {lr} / sub sp, #8` at 0x02004f60 through
 * `add sp, #8 / pop {r0} / bx r0` at 0x020050ba.  The epilogue pops the return
 * address into r0, so the owner is `void` (HANDOVER section 0).  Two blocks
 * inside the advertised 364-byte row are DATA, not code:
 *   0x02004f84-0x02004fcb  the 18-entry `mov pc, r3` jump table below;
 *   0x020050c0-0x020050e3  the literal pool (nine words), now included in the
 *                          semantic owner up to the next prologue at 0x50e4.
 *
 * LINK BASE, six fresh witnesses.  The jump table is loaded with base
 * 0x0200cf84 and sits physically at file offset 0x4f84, and its entries are
 * 0x0200cfcc / 0x0200d076 / 0x0200cffa / 0x0200d05c / 0x0200d00c, i.e. exactly
 * 0x8000 above the case bodies at 0x02004fcc / 0x02005076 / 0x02004ffa /
 * 0x0200505c / 0x0200500c.  Independently, five pool words handed to the
 * installer are 0x02009719, 0x0200975d, 0x020097bd, 0x02009529, 0x020099e9 and
 * 0x0200969d — each one is a known function start plus the Thumb bit once
 * 0x8000 is subtracted (0x02001718, 0x0200175c, 0x020017bc, 0x02001528,
 * 0x020019e8, 0x0200169c; all six already have sources in this tree).  So the
 * module is linked at 0x02008000 and those words are in-image function
 * pointers, not RAM addresses.  0x02001718's own byte-exact source passes
 * 0x02009719 — its own tagged address — to Func_02006cd4, which closes the
 * loop.
 *
 * CALL TARGETS.  An overlay `bl` stores the target's image offset minus two,
 * so `overlay_show.ts`'s annotations are wrong; every name below comes from
 * `bun tools/overlay_call_targets.ts resource_3bf 4f60`.  Accounting: 19 call
 * sites, 7 distinct callees (18 sites reach the import veneer table at
 * 0x02005588+, one is the in-image sibling 0x02005470).  The inventory's
 * `calls=8` is the weaker figure documented in HANDOVER section 0 and does not
 * match either number.
 *
 * Imports resolved through the veneer table:
 *   0x02005588 -> Func_080000c0   ROM dispatch[0] -> Func_080030f8
 *   0x02005590 -> Func_080000d0   ROM dispatch[2] -> Func_080041d8, the task
 *                                 registrar (see semantic/main/080041d8.c: it
 *                                 files a pointer/s16 pair in the 20-entry
 *                                 table at 0x03001a20)
 *   0x020055b8 -> Func_08009128
 *   0x020055d0 -> Func_080091b8   six arguments, last two stack-carried
 *   0x020055f8 -> Func_08009258
 *   0x020056e0 -> Func_0808a158
 *
 * SHARED STATE.  Data_02000240 is the signed-halfword work area the byte-exact
 * sources in this and neighbouring overlays already use; index 225 (byte
 * offset 450, built as `0xe1 << 1`) is the same mode slot
 * assets/code/resource_36f_c_02000054.c reads.  0x03001ebc is the workspace
 * pointer those sources load directly; the field written here is the s32 at
 * +0x1C0 (`0xe0 << 1`), the same field
 * assets/code/resource_372_c_0200015c.c writes.
 *
 * Uncertainty: Func_08009128 is reached with no argument register set — r0
 * still holds whatever the preceding Func_080000c0 left — so it is declared
 * without a prototype and called with no arguments rather than given an
 * invented one.  Likewise Func_02005470 takes none (its byte-exact source at
 * assets/code/resource_3bf_c_02005470.c is `void(void)`).
 */

extern s16 Data_02000240[];

/* In-image task bodies, installed by tagged pointer.  All are `void(void)`. */
void Func_02001718(void);
void Func_0200175c(void);
void Func_020017bc(void);
void Func_02001528(void);
void Func_020019e8(void);
void Func_0200169c(void);

void Func_02005470(void);

typedef void (*Task_02004f60)(void);

/* Imports, named by the main-image address in their veneer's trailing word. */
void Func_080000c0();
void Func_080000d0();
void Func_08009128();
void Func_080091b8();
void Func_08009258();
void Func_0808a158();

void Func_02004f60(void)
{
    s16 mode;
    s32 install_id;
    u8 *state;

    Func_02005470();

    mode = Data_02000240[225];

    /*
     * `mode - 2` is range-checked with an UNSIGNED `cmp #17 / bhi`, so any
     * mode below 2 or above 19 takes the default arm.  Table indices 6..9
     * (modes 8..11) point at the default body too, which is why they are not
     * listed separately below.
     */
    state = *(u8 **)0x03001ebc;

    switch (mode) {
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
        *(s32 *)(state + 0x1C0) = 0x200;
        Func_080000d0((Task_02004f60)Func_02001718, 200 << 4);
        Func_080000d0((Task_02004f60)Func_0200175c, 200 << 4);
        Func_080000d0((Task_02004f60)Func_020017bc, 200 << 4);
        install_id = 224;
        goto install_mode;

    case 12:
    case 19:
        *(s32 *)(state + 0x1C0) = 0x209;
        install_id = 192;
        goto install_mode;

    case 16:
    case 17:
    case 18:
        *(s32 *)(state + 0x1C0) = 0x200;
        Func_080000d0((Task_02004f60)Func_02001528, 200 << 4);
        Func_080000d0((Task_02004f60)Func_020019e8, 200 << 4);
        Func_080000c0(1);
        Func_08009128();
        Func_080000c0(1);
        Func_080091b8(101, 9, 10, 8, 110, 9);
        install_id = 224;

install_mode:
        Func_08009258(install_id << 4);
        goto tail;

    case 13:
    case 14:
    case 15:
        *(s32 *)(state + 0x1C0) = 0x200;
        Func_080000d0((Task_02004f60)Func_0200169c, 200 << 4);
        goto tail;

    default:
        *(s32 *)(state + 0x1C0) = 0x200;
        Func_08009258(224 << 4);
        goto tail;
    }

tail:
    Func_0808a158(18, 1);
    Func_0808a158(17, 1);
    Func_0808a158(21, 1);
    Func_0808a158(12, 1);
    Func_0808a158(13, 1);
    Func_080000c0(1);
}
