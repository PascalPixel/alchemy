/*
 * Draft of Func_02000b20 (overlay 6A9, 02008b20). differ 16 (18 before,
 * 2026-10-12): read through a party pointer, the scene address is built from
 * gPartyState with the constant left from the store, as the native does. Left:
 * the native reads the scene again with ldrsh after the first call; this draft
 * starts the second read (ldrh r1) before the first branch, repeats it after the
 * call and extends it by shifts. An empty asm barrier or a one-pass block
 * between the two tests changes nothing; a volatile pointer extends both reads
 * by shifts (differ 20).
 */
#include "../../../../games/tla/src/field/overlay_6a9/ov6a9.h"

s32 Func_02000b20(void)
{
    struct PartyState *party;

    WORD_AT(Ram_HeapSlots->event_work, 428) = 516;
    party = &gPartyState;
    if (party->scene == (s32)&SceneId_Ov6a9Room1)
        Func_02000ba8();
    if (party->scene == (s32)&SceneId_Ov6a9Room4)
        Func_02000f98();
    return 0;
}
