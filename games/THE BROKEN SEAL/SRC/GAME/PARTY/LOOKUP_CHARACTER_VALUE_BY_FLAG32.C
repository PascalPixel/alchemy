#include "TYPES.H"
#include "SCENE.H"

#if defined(TBS_EDITION_EN)
/* The other editions keep their code here in their scaffolds for now. */

struct MessageWindow;

extern u8 *gWindowWork;
extern s16 Data_080371fe[];
extern u8 MsgVenusDjinnJoined[];

struct MessageWindow *UiWindow_Create(s32, s32, s32, s32, s32);
void UiWindow_DrawDividerLine(struct MessageWindow *, s32, s32, s32, s32);
s32 Localization_LookupEntryId(s32);
void UiGlyph_LoadEntryWithPalette(s32, s32, s32 *, s32 *, s32, s32);
void UiWork_PushValueSlot(s32, s32);
s32 UiText_BuildRenderEntriesMode1(s32);
s32 UiText_QueueRenderEntries(struct MessageWindow *, s32, s32, s32, s32);
void Audio_PlayCue(s32);
void Runtime_PushSlotEntry(s32 *, s32);
void WaitFrames(s32);
s32 Audio_Check(void);
void UiWork_Finalize(struct MessageWindow *, s32);
void Resource_ResetEntry(s32);

extern volatile u32 gKeyState;

/* Announce a Djinni joining: a window with the element's Djinni glyph and
   its joining line, then wait for the jingle or a key. */
void Djinn_ShowJoinedMessage(s32 pc, s32 element, s32 djinn)
{
    u8 *work = gWindowWork;
    s32 name;
    struct MessageWindow *window;
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
    window = UiWindow_Create(2, 1, 26, 5, (s32)window);

    if (window != NULL) {
        UiWindow_DrawDividerLine(window, 4, 0, 4, 4);

        work[0xea3] = 1;

        id = Localization_LookupEntryId(name);
        UiGlyph_LoadEntryWithPalette(id, zero, &handle, &palette, 14, zero);

        p = entry;
        *p++ = zero;
        *p++ = 0x8014000c;
        *p = palette | 0xe000;

        *(u16 *)(work + 0x12f4) = zero;
        *(u16 *)(work + 0x12f6) = zero;

        UiWork_PushValueSlot(pc, 1);
        UiWork_PushValueSlot(element * 20 + djinn + 300, 4);

        text = UiText_BuildRenderEntriesMode1(element + (s32)MsgVenusDjinnJoined);

        UiText_QueueRenderEntries(window, text, 36, 2, zero);

        Audio_PlayCue(81);

        do {
            Runtime_PushSlotEntry(entry, 250);
            WaitFrames(1);
        } while (Audio_Check() != 0 && (gKeyState & 0x303) == 0);

        UiWork_Finalize(window, 2);
        WaitFrames(1);
        Resource_ResetEntry(handle);
    }
}
#endif

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
