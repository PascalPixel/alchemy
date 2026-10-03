#include "CALLBACK_SCHEDULER.H"
#include "RESOURCE.H"
#include "TYPES.H"
#include "IO_REG.H"
#include "SCENE.H"
#include "TBS_EDITION.H"
#include "WINDOW.H"


extern s16 Data_080371fe[];
extern u8 MsgVenusDjinnJoined[];

struct UiWindow *UiWindow_Create(s32, s32, s32, s32, s32);
void UiWindow_DrawDividerLine(struct UiWindow *, s32, s32, s32, s32);
s32 Localization_LookupEntryId(s32);
void UiGlyph_LoadEntryWithPalette(s32, s32, s32 *, s32 *, s32, s32);
void UiWork_PushValueSlot(s32, s32);
s32 UiText_BuildRenderEntriesMode1(s32);
/* These joining messages retain their five-word channel transport. */
struct UiChannelSlot *UiText_QueueRenderEntries();
void Audio_PlayCue(s32);
void WaitFrames(s32);
s32 Audio_Check(void);
void UiWork_Finalize(struct UiWindow *, s32);

extern volatile u32 gKeyState;

/* Announce a Djinni joining: a window with the element's Djinni glyph and
   its joining line, then wait for the jingle or a key. */
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
    name = Data_080371fe[element & 3];
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
        /* the Djinni sits half a tile into the window, 12 lines down */
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

extern s16 Party_CharacterValues[];
extern s16 Party_CharacterValuesFlag32[];
s32 GameFlag_TestFar(s32);

s32 Party_LookupCharacterValueByFlag32(u32 index)
{
    if (index > 8) {
        return 0;
    }
    if (GameFlag_TestFar(32) == 0) {
        return Party_CharacterValues[index];
    }
    return Party_CharacterValuesFlag32[index];
}

extern u8 MsgJoinedParty[];
void AudioCommand_PlayFar(s32);

/* Announce a character joining: a window with the character's portrait
   glyph and the joining line, then wait for the jingle or a key. */
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
        /* the portrait sits half a tile into the window, 12 lines down */
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
