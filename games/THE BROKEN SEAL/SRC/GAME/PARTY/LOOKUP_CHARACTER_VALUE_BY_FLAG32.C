#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "CALLBACK_SCHEDULER.H"
#include "RESOURCE.H"
#include "TYPES.H"
#include "IO_REG.H"
#include "SCENE.H"
#include "TBS_EDITION.H"
#include "WINDOW.H"
#include "UIFLOW.H"
#include "BTLSYS.H"
#include "text/MSG_IDS.H"

#define gKeyState (*(volatile u32 *)&gKeyState)

#define UiGlyph_LoadEntryWithPalette ((void (*)(s32,s32,s32*,s32*,s32,s32))UiGlyph_LoadEntryWithPalette)


void UiWindow_DrawDividerLine(struct UiWindow *, s32, s32, s32, s32);
s32 Localization_LookupEntryId(s32);
void UiWork_PushValueSlot(s32, s32);
struct UiChannelSlot *UiText_QueueRenderEntries();
void Audio_PlayCue(s32);
void WaitFrames(s32);


void Djinn_ShowJoinedMessage(s32 pc, s32 element, s32 djinn)
{
    struct UiRenderWork *work = (struct UiRenderWork *)*gWindowWork;
    s32 name;
    struct UiWindow *window;
    s32 sprite[3];
    s32 *entry;
    s32 palette, handle;
    s32 id;
    s32 text;
    u32 zero;
    s32 *p;

    window = NULL;
    zero = 0;
    name = Djinn_ElementSprites[element & 3];
    entry = sprite;
    /* FAKEMATCH: the null window is also the style argument (ROM passes its register). */
    window = UiWindow_Create(DJINN_JOIN_X, 1, DJINN_JOIN_WIDTH, 5, (s32)window);

    if (window != NULL) {
        UiWindow_DrawDividerLine(window, 4, 0, 4, 4);

        work->dirty = 1;

        id = Localization_LookupEntryId(name);
        UiGlyph_LoadEntryWithPalette(id, zero, &handle, &palette, 14, zero);

        p = entry;
        *p++ = zero;
        *p++ = 0x8000000c | ((DJINN_JOIN_X * 8 + 4) << 16);
        *p = palette | 0xe000;

        work->result[0] = zero;
        work->result[1] = zero;

        UiWork_PushValueSlot(pc, 1);
        UiWork_PushValueSlot(element * 20 + djinn + 300, 4);

        text = UiText_BuildRenderEntriesMode1(element + (s32)MsgVenusDjinnJoined);

        UiText_QueueRenderEntries(window, text, 36, 2, zero);

        Audio_PlayCue(81);

        do {
            Runtime_PushSlotEntry(entry, 250);
            WaitFrames(1);
        } while (Audio_Check() != 0 && (gKeyState & (KEY_A | KEY_B | KEYS_SHOULDERS)) == 0);

        UiWork_Finalize(window, 2);
        WaitFrames(1);
        Resource_ResetEntry(handle);
    }
}


s32 Party_LookupCharacterValueByFlag32(u32 index)
{
    if (index > 8) {
        return 0;
    }
    if (GameFlag_TestFar(32) == 0) {
        return Character_SpriteTable[index];
    }
    return Character_ChildSpriteTable[index];
}

void AudioCommand_PlayFar(s32);

void Party_ShowJoinedMessage(s32 member)
{
    struct UiRenderWork *work = (struct UiRenderWork *)*gWindowWork;
    struct UiWindow *window = NULL;
    s32 sprite[3];
    s32 *entry = sprite;
    s32 palette, handle;
    s32 *p;

    window = UiWindow_Create(2, 1, 26, 5, 0);
    if (window != NULL) {
        UiWindow_DrawDividerLine(window, 4, 0, 4, 4);
        work->dirty = 1;
        UiGlyph_LoadEntryWithPalette(Localization_LookupEntryId(Party_LookupCharacterValueByFlag32(member)),
            0, &handle, &palette, 14, 0);
        p = entry;
        *p++ = 0;
        *p++ = 0x8000000c | ((2 * 8 + 4) << 16);
        *p = palette | 0xe000;
        work->result[0] = 0;
        work->result[1] = 0;
        UiWork_PushValueSlot(member, 1);
        UiText_QueueRenderEntries(window, UiText_BuildRenderEntriesMode1((s32)MsgJoinedParty), 36, 2, 0);
        AudioCommand_PlayFar(81);
        do {
            Runtime_PushSlotEntry(entry, 250);
            WaitFrames(1);
        } while (Audio_Check() != 0 && (gKeyState & (KEY_A | KEY_B | KEYS_SHOULDERS)) == 0);
        UiWork_Finalize(window, 2);
        WaitFrames(1);
        Resource_ResetEntry(handle);
    }
}
