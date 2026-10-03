#include "CALLBACK_SCHEDULER.H"
#include "TYPES.H"
#include "IO_REG.H"
#include "SCENE.H"
#include "RENDER_INPUT.H"
#include "RESOURCE.H"

#if defined(TBS_EDITION_EN)
/* The other editions keep their code here in their scaffolds for now. */

struct MessageWindow;

extern u8 *gWindowWork;
extern u8 MsgPairJoinedParty[];

struct MessageWindow *UiWindow_Create(s32, s32, s32, s32, s32);
void UiWindow_DrawDividerLine(struct MessageWindow *, s32, s32, s32, s32);
s32 Party_LookupCharacterValueByFlag32(s32);
s32 Localization_LookupEntryId(s32);
void UiGlyph_LoadEntryWithPalette(s32, s32, s32 *, s32 *, s32, s32);
void UiWork_PushValueSlot(s32, s32);
s32 UiText_BuildRenderEntriesMode1(s32);
s32 UiText_QueueRenderEntries(struct MessageWindow *, s32, s32, s32, s32);
void Audio_PlayCue(s32);
void WaitFrames(s32);
s32 Audio_Check(void);
void UiWork_Finalize(struct MessageWindow *, s32);

extern volatile u32 gKeyState;

/* Announce two characters joining the party: a window with both members'
   glyphs and the joined line, then wait for the jingle or a key. */

void Party_ShowPairJoinedMessage(s32 first, s32 second)
{
    u8 *work = gWindowWork;
    struct MessageWindow *window;
    s32 sprite2[3];
    s32 sprite1[3];
    s32 *entry1;
    s32 *entry2;
    s32 handle1, palette, handle2;
    u32 zero;
    s32 *p;

    window = NULL;
    entry1 = sprite1;
    /* FAKEMATCH: the null window is also the style argument (ROM passes its register). */
    window = UiWindow_Create(1, 1, 28, 5, (s32)window);
    zero = 0;

    if (window != NULL) {
        UiWindow_DrawDividerLine(window, 8, 0, 4, 4);

        work[0xea3] = 1;

        UiGlyph_LoadEntryWithPalette(Localization_LookupEntryId(Party_LookupCharacterValueByFlag32(first)), 0, &handle1, &palette, 14, zero);
        p = entry1;
        *p++ = zero;
        *p++ = 0x800c000c;
        *p = palette | 0xe000;

        entry2 = sprite2;
        UiGlyph_LoadEntryWithPalette(Localization_LookupEntryId(Party_LookupCharacterValueByFlag32(second)), 0, &handle2, &palette, 15, zero);
        p = entry2;
        *p++ = zero;
        *p++ = 0x802c000c;
        *p = palette | 0xf000;

        *(u16 *)(work + 0x12f4) = zero;
        *(u16 *)(work + 0x12f6) = zero;

        UiWork_PushValueSlot(first, 1);
        UiWork_PushValueSlot(second, 1);

        UiText_QueueRenderEntries(window, UiText_BuildRenderEntriesMode1((s32)MsgPairJoinedParty), 68, 2, zero);

        Audio_PlayCue(81);

        do {
            Runtime_PushSlotEntry(entry1, 250);
            Runtime_PushSlotEntry(entry2, 250);
            WaitFrames(1);
        } while (Audio_Check() != 0 && (gKeyState & (KEY_A | KEY_B | KEYS_SHOULDERS)) == 0);

        UiWork_Finalize(window, 2);
        WaitFrames(1);
        Resource_ResetEntry(handle1);
        Resource_ResetEntry(handle2);
    }
}
