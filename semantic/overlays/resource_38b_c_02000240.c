typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;

/*
 * Resource 38b cutscene step at 0x02000240 (72 bytes, 6 call sites).
 *
 * Complete owner: `push {lr}` prologue at 0x02000240 and the matching
 * interworking return `pop {r0} / bx r0` at 0x02000278.  Under HANDOVER
 * section 0's epilogue rule, `pop {r0} ; bx r0` pops the *return address*
 * into r0, so nothing is returned and the owner is `void`.  The literal pool
 * occupies 0x0200027c-0x02000287 and is data, not code.
 *
 * Link base: this overlay is linked at 0x02008000, proven from banked
 * material.  The byte-exact sibling assets/code/resource_38b_c_02000088.c
 * references Data_020091ec / 0200930c / 0200936c / 020091d4, which land at
 * file offsets 0x11ec / 0x130c / 0x136c / 0x11d4 — immediately past the
 * import veneer table, which ends at 0x11c0.  A fourth witness is in
 * 0x02000890, which passes the pool word 0x02008ed9 (= Func_02000ed8 plus
 * the Thumb bit) to the task installer Func_080000d0.
 *
 * All `bl` targets were resolved with tools/overlay_call_targets.ts, which
 * applies the overlay rule `true_target = stored_displacement + 2`; every one
 * of this owner's six sites is an import veneer, so the names below are the
 * real main-image addresses from each veneer's trailing word.
 */

/* The overlay's scene workspace pointer CELL — one dereference, not two. */
extern u8 *Data_03001ebc;

/* Imports, reached through the veneer table at 0x02001018.  Old-style
 * declarations: arities vary per call site across this overlay, and
 * Func_080770c0 is used for its result so it needs a return type. */
extern void Func_0808a018();
extern void Func_0808a020();
extern void Func_0808a170();
extern void Func_0808a180();
extern s32 Func_080770c0();
extern void Func_080770c8();

/* Progress-flag id tested and then set by this step. */
#define FLAG_STEP_0240 0x301

void Func_02000240(void)
{
    u8 *workspace;

    Func_0808a018();

    /* 0x13ae is the cue id; the call site sets only r0. */
    Func_0808a170(0x13ae);

    if (Func_080770c0(FLAG_STEP_0240) != 0) {
        /* The skip-beat counter documented in HANDOVER section 0: the u16 at
         * workspace + 472, built as `movs r3,#236 / lsls r3,#1`.  It is
         * bumped only on the already-seen path, so a replay of this scene
         * advances the counter instead of re-emitting the cue. */
        workspace = Data_03001ebc;
        *(u16 *)(workspace + 472) += 1;
    }

    Func_0808a180(9, 0);
    Func_080770c8(FLAG_STEP_0240);
    Func_0808a020();
}
