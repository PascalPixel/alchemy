/* Draft of SceneData_GetTertiaryTable, resource_3c3 at 0x020080e4 (from FIELD/SUHARA_GATE/ACTOR_PROMPT.C).
 * Remaining difference: it compares the scene with 0xaa and 0xab, which the game loads from its literal pool as link-time values; an integer compares with an immediate. Its tables are not yet labelled. */
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

s32 SceneData_GetTertiaryTable(void)
{
    extern s16 gCell[];

    s16 v = gCell[224];

    if (v == (s32)&Value_000000aa) {
        return (s32)gOv8;
    }
    if (v == (s32)&Value_000000ab) {
        return (s32)gOv9;
    }
    return (s32)gOv10;
}
