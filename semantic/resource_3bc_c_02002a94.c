#include "types.h"

/*
 * resource_3bc owner at 0x02002a94, 184 bytes: pick a story-state message,
 * play it, then gate a follow-up scene behind two story flags.
 *
 * Complete owner: `push {r5, r6, lr}` at 0x02002a94 through `pop {r5, r6} /
 * pop {r1} / bx r1` at 0x02002b2c-0x02002b30, followed by the seven-word
 * literal pool 0x02002b34-0x02002b4f that closes the 184-byte span; the next
 * owner's prologue is expected at 0x02002b50.  Two arguments (r0, r1),
 * s32-returning (the popped word is not the return address; r0 carries a
 * result on every exit path).
 *
 * Not found by the structural inventory walk (unindexed); eleven call sites,
 * all resolved with the `+2` rule against the raw image (this owner starts
 * outside a recorded row, so overlay_call_targets.ts's own table does not
 * cover it -- resolved by hand against overlayImage()/targetOffset()):
 *   0x02002a9a -> veneer 0x02004b70 -> Func_0808a460()
 *   0x02002aa2 -> veneer 0x02004978 -> Func_08015120(arg1, 5)
 *   0x02002ac8, 0x02002b18 -> veneer 0x02004ab0 -> Func_0808a170(msg)   established message-display import (see resource_374_c_020004d4.c)
 *   0x02002ad0 -> veneer 0x02004ac0 -> Func_0808a180(arg0, 0)
 *   0x02002ada, 0x02002aea -> veneer 0x020049b0 -> Func_080770c0(flag)   story-flag query, nonzero if set (see resource_374_c_02000248.c)
 *   0x02002af4 -> veneer 0x02004988 -> Func_080153a0(0)
 *   0x02002b12 -> veneer 0x020049b8 -> Func_080770c8(flag)   set a story flag (see resource_371_c_020039fc.c)
 *   0x02002b20 -> veneer 0x02004ab8 -> Func_0808a178(arg0, 0)
 *   0x02002b28 -> veneer 0x02004a10 -> Func_0808a070(0, 0)
 *
 * The message-state read at Data_02000240[0xe0] (byte offset 0x1c0) selects
 * one of three message ids; the two flag ids (arg1+0x200, arg1+0x208) and
 * the Func_08015120/Func_080153a0 arguments are not otherwise cross-
 * referenced from this owner, so their meanings beyond "story flag" /
 * "story-state message" are not resolved.
 *
 * Control flow, matched to the branch shape rather than simplified: if the
 * first flag is already set, return 2 immediately (0x02002ae0 -> 0x02002afc,
 * the shared "return 2" tail). Otherwise query the second flag; if it is
 * already set too, the return value comes from a three-way classification of
 * Func_080153a0(0)'s result (1 -> 2, 2 -> 3, -1 -> 3, anything else ->
 * passed through unchanged) at 0x02002af4-0x02002b0e. If the second flag is
 * NOT set, this owner sets it, plays a fixed message (0x207c), runs two more
 * calls, and returns whatever Func_0808a070(0, 0) returns -- there is no
 * explicit `movs r0` on that exit path.
 */

extern s16 Data_02000240[];

/* Call symbols are per-site (the raw disassembly shows a DIFFERENT veneer
 * target at every occurrence, including the repeated Func_080770c0 and
 * Func_0808a170 calls) -- declared/named as the literal per-site targets,
 * not the shared ultimate-destination symbol. */
void Func_0200760c();          /* Func_0808a460 veneer */
void Func_0200741c();          /* Func_08015120 veneer */
void Func_0200757a();          /* Func_0808a170 veneer #1 */
void Func_02007592();          /* Func_0808a180 veneer */
s32 Func_0200748c();           /* Func_080770c0 veneer #1 */
s32 Func_0200749c();           /* Func_080770c0 veneer #2 */
s32 Func_0200747e();           /* Func_080153a0 veneer */
void Func_020074cc();          /* Func_080770c8 veneer */
void Func_020075ca();          /* Func_0808a170 veneer #2 */
void Func_020075da();          /* Func_0808a178 veneer */
s32 Func_0200753a();           /* Func_0808a070 veneer */

s32 Func_02002a94(s32 arg0, s32 arg1)
{
    s16 state;
    s32 messageId;
    s32 classification;

    Func_0200760c();
    Func_0200741c(arg1, 5);

    state = Data_02000240[0xe0];
    if (state == 0x8f) {
        messageId = 0x2076;
    } else if (state == 0x90) {
        messageId = 0x2078;
    } else {
        messageId = 0x207a;
    }
    Func_0200757a(messageId);

    Func_02007592(arg0, 0);

    if (Func_0200748c(arg1 + 0x200) != 0) {
        return 2;
    }

    if (Func_0200749c(arg1 + 0x208) != 0) {
        classification = Func_0200747e(0);
        if (classification == 1) {
            return 2;
        }
        if (classification == 2) {
            return 3;
        }
        if (classification == -1) {
            return 3;
        }
        return classification;
    }

    Func_020074cc(arg1 + 0x208);
    Func_020075ca(0x207c);
    Func_020075da(arg0, 0);
    return Func_0200753a(0, 0);
}
