/* The item and level debug room: its entry setup, the glyph caption window
 * and the far debug browsers. */
#include "LEVEL.H"

void SceneState_SetWorkWords1c0And1c8(void)
{
    *(s32 *)((*(u8 **)&gEventWork) + 0x1c0) = 0x201;
    *(s32 *)((*(u8 **)&gEventWork) + 0x1c8) = 24;
    Engine_EventRequestExit();
}

/* The overlay's entry driver, the target of the first entry veneer. Actor
 * 11 is fetched twice, once per store, and must not be folded into one
 * local. */
s32 FieldScene_RunEntrySetup(void)
{
    *(s32 *)((*(u8 **)&gEventWork) + 448) = 516;
    *(s32 *)((*(u8 **)&gEventWork) + 456) = 24;
    *(s32 *)(Object_GetById(11) + 28) = 0x19999;
    *(s32 *)(Object_GetById(11) + 24) = 0x19999;
    Engine_ActorSetAnimation(13, 5);
    Engine_ActorSetAnimation(14, 2);
    return 0;
}

void FieldScene_DrawThreeCaptionWindow(void)
{
    /*
     * The frame is 36 bytes: 4 for the stacked fifth argument, plus a 32-byte
     * local that no instruction reads or writes.  Only its size is known, not
     * its element type, so the declaration must stay at 32 bytes.
     */
    u8 buf[32];
    s32 handle = UiWindow_Create(0, 13, 30, 6, 2);

    UiText_DrawStringInWindow(gItemLevelGlyphsUpper, handle, 0, 0);
    UiText_DrawStringInWindow(gItemLevelGlyphsLower, handle, 0, 8);
    UiText_DrawStringInWindow(gItemLevelGlyphsMarks, handle, 0, 16);
}

/* Set the flag byte at +53 of the record the effect work pointer holds. */
void SceneState_SetRecordFlag53(void)
{
    u8 *record = *(u8 **)gEffectWork;

    record[53] = 1;
}

/*
 * The return address is popped into r1, not r0, so r0 is live at return and
 * the wrapper hands its callee's result back; declaring the pair void would
 * compile pop {r0} / bx r0 instead.
 */
int SceneState_GetFarResult100c(void)
{
    return DebugMenu_BrowseIcons();
}

int SceneState_GetFarResult1020(void)
{
    return DebugMenu_BrowseEntryGlyphs();
}
