#include "types.h"

/*
 * resource_3c8 owner at 0x020023d4, 168 bytes: among scene slots 8-13,
 * locate candidates sharing the selected slot's x/z tile, retain the highest
 * candidate at least one 16.16 unit above its y value, store that candidate id
 * at selected+100, then move/release the selected slot and run its local effect.
 *
 * Complete owner: high-register prologue and four-byte frame at 0x020023d4
 * through the sole interworking return at 0x02002468-0x02002474, followed by
 * alignment and one referenced pool word through 0x0200247b.  Eight static
 * calls across seven targets match independently; the two scene-accessor call
 * sites sit inside the bounded six-iteration loop.
 */

/*
 * CALL SYMBOLS ARE PER-SITE: the raw assembly spells each of these eight
 * calls as a direct `bl sub_020072xx` to an address inside this overlay's
 * own 0x0200xxxx range (verified via `arm-none-eabi-objdump -dr -M
 * force-thumb` on the assembled .o, which resolves the *ABS* targets before
 * linking) -- lifted verbatim, not the veneer-math final target names this
 * file used before. The one true indirect call (selector's local-effect
 * dispatch) is routed automatically through this overlay's own
 * `_call_via_rN` bank; Func_02000b08 is correct for it, unchanged.
 */
u8 *Func_020072a2();
u8 *Func_020072aa();
void Func_020072f6();
void Func_02007252();
void Func_02007340();
void Func_02007456();
void Func_020072f2();
void Func_02000b08();

void Func_020023d4(s32 selector)
{
    u8 *candidateRecord;
    s32 highest = (s32)0xffb00000;
    u8 *selectedRecord = 0;
    u32 index;

    for (index = 0; index <= 5; index++) {
        s32 candidate = index + 8;
        s32 candidateY;

        if (candidate == selector) {
            continue;
        }

        candidateRecord = Func_020072a2(candidate);
        selectedRecord = Func_020072aa(selector);

        if ((*(s32 *)(candidateRecord + 8) >> 20)
                != (*(s32 *)(selectedRecord + 8) >> 20)
            || (*(s32 *)(candidateRecord + 16) >> 20)
                != (*(s32 *)(selectedRecord + 16) >> 20)) {
            continue;
        }

        candidateY = *(s32 *)(candidateRecord + 12) + 0x100000;
        if (highest <= candidateY) {
            *(u16 *)(selectedRecord + 100) = (u16)candidate;
            highest = candidateY;
        }
    }

    Func_020072f6(selector, 0x40000, 0x20000);
    Func_02007252(selectedRecord,
                  *(s32 *)(selectedRecord + 8),
                  highest,
                  *(s32 *)(selectedRecord + 16));
    Func_02007340(selector);
    Func_02007456(188);
    Func_02000b08(selector);
    Func_020072f2(30);
}
