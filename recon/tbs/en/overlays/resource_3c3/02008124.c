/* Draft of Scene_RunSupplementalSequenceOne, resource_3c3 at 0x02008124 (from FIELD/SUHARA_GATE/ACTOR_PROMPT.C).
 * Remaining difference: it compares the scene with link-time values the game loads from its literal pool and stores one; its calls still use per-call-site names. */
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "SCENE.H"

extern u8 Value_000000aa;
extern u8 Value_000000ab;
extern u8 gOv[];
extern u8 gOv2[];
extern u8 gOv3[];
extern u8 Value_000000a9;
extern u8 gOv4[];
extern u8 gOv5[];
extern u8 gOv6[];
extern u8 gOv7[];
extern u8 gOv8[];
extern u8 gOv9[];
extern u8 gOv10[];
extern u8 gVal[];
extern u8 gVal2[];
extern u8 gVal3[];

u8 *Talk_unk4_4(s32);

u8 *Talk_unk5_4(s32);

u8 *Talk_unk6_4(s32);

u8 *Talk_unk7_4(s32);

/*
 * The eight-byte owner at 0x02000084 includes its one pool word, which holds
 * the returned table address 0x02008b48.
 */

/*
 * Select a table from the scene id.  The 88-byte owner at 0x0200008c includes
 * its seven-word literal pool at 0x020000c4-0x020000e3.  Element 224 of the
 * cross-overlay block gCell is the signed scene id this overlay keys
 * on.  The compared constants are spelled as the addresses of Value_000000aa
 * and its neighbours: that is how a small integer is pooled here, and the
 * names are not references to symbols.
 */

/* Old-style declarations: overlay import arities vary per call site. */

/*
 * Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds, not a runtime address.
 */

/*
 * Call sites spelled through these wrappers pass their constants straight into
 * the argument registers; a direct call precomputes a costly constant into a
 * pseudo shared with later uses in the block.  A value-returning call also
 * sets r0 last of its arguments.
 */

/*
 * Engine calls: each pseudo symbol is the per-site call word the overlay image
 * holds -- one word can serve two sites with different targets -- and the
 * macro names the engine function the site reaches through the overlay veneer
 * and the main-image veneer island, keeping the site's own calling form.
 * Unbound names are provisional.
 */

/*
 * Runs a fixed sequence of setup calls, mostly in mirrored pairs for entities
 * 8 and 9, followed by two six-argument calls whose second and last arguments
 * match (entity 6/27 and entity 9/26).
 */

                        /* Test a story flag; used in a condition. */

                        /* Open a scripted scene. */

                        /* Close a scripted scene. */

                        /* Dialogue prompt; the result selects the branch. */

                        /* Scene-presentation request. */

                        /* Show a dialogue line by id. */

                        /* Dialogue-line variant with a mode word. */

                        /* Wait for the slot's action to finish. */

s32 Talk_unk8_4();    /* Raw encoded call destination. */

s32 Scene_RunSupplementalSequenceOne(void)
{
    extern u8 gCell[];
    extern u8 gWork[];

    s16 *q;
    s16 *p;
    s16 *r;
    s32 v;

    if (Talk_Check(0x89f) != 0) {
        s32 a = (s32)gVal;

        q = (s16 *)gCell;
        q[226] = a;
        {
            s16 *t = q + 227;
            s32 shown = 10;

            *t = shown;
        }
    }
    p = (s16 *)gCell;
    v = p[224];
    if (v == (s32)gVal2) {
        if (Talk_unk2(0x897) != 0) {
            Talk_unk10_4(10, 0, 0);
        }
        if (p[225] == 3) {
            if (Talk_unk3(0x8fb) != 0) {
                p[288] = v;
                {
                    s16 *t = p + 289;
                    s32 shown = 1;

                    *t = shown;
                }
            }
            if (Talk_unk4(0x8fc) != 0) {
                p[288] = v;
                {
                    s16 *t = p + 289;
                    s32 shown = 5;

                    *t = shown;
                }
            }
            Talk_Do(0x12f);
        }
        r = (s16 *)gCell;
        if (r[225] == 1) {
            Talk_unk2_2(0x8fb);
            if (Talk_unk5(0x96f) == 0) {
                Talk_SetRect(6, 0, 2, 1, 8, 27);
            }
        }
        if (r[225] != 5) {
            goto L_0200024c;
        }
        Talk_unk3_2(0x8fc);
    } else {
        if (v == (s32)gVal3) {
            Talk_unk11_4(8, 4);
            Talk_unk12_4(9, 4);
            Talk_unk13_4(10, 3);
            Talk_unk14_4(11, 4);
            Talk_unk15_4(12, 3);
            *(volatile s32 *)(Talk_unk9(15) + 28) = 0x19999;
            Talk_unk2_5(108, 38, 1, 1, 102, 56);
        }
    }
    L_0200024c:;
    return 0;
}
