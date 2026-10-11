/*
 * Draft of Func_02000b20 (overlay 6A9, 02008b20). differ 18: ⚓️ builds the
 * scene's address from gPartyState with the constant left from the store
 * and reads it twice with ldrsh; the draft loads the address from the pool
 * and extends the second read by shifts. Reading gPartyState.scene twice
 * as a field gives the same.
 */
#include "../../../../games/tla/src/field/overlay_6a9/ov6a9.h"

s32 Func_02000b20(void)
{
    s16 *scene;

    WORD_AT(Ram_HeapSlots->event_work, 428) = 516;
    scene = (s16 *)((u8 *)&gPartyState + 480);
    if (*scene == (s32)&SceneId_Ov6a9Room1)
        Func_02000ba8();
    if (*scene == (s32)&SceneId_Ov6a9Room4)
        Func_02000f98();
    return 0;
}
