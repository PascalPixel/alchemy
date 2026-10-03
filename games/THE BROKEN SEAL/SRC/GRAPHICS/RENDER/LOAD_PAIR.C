#include "CALLBACK_SCHEDULER.H"
#include "RUNTIME_MEM.H"
#include "TYPES.H"
#include "IO_REG.H"
#include "WINDOW.H"
#include "SYSTEM.H"
#include "SCENE.H"
#include "RENDER_INPUT.H"
#include "RESOURCE.H"
#include "TBS_EDITION.H"

extern u8 MsgPairJoinedParty[];

void UiWindow_DrawDividerLine(struct UiWindow *, s32, s32, s32, s32);
s32 Party_LookupCharacterValueByFlag32(s32);
s32 Localization_LookupEntryId(s32);
void UiGlyph_LoadEntryWithPalette(s32, s32, s32 *, s32 *, s32, s32);
void UiWork_PushValueSlot(s32, s32);
s32 UiText_BuildRenderEntriesMode1(s32);
/* This native caller passes five words; keep its legacy call boundary. */
struct UiChannelSlot *UiText_QueueRenderEntries();
void Audio_PlayCue(s32);
void WaitFrames(s32);
s32 Audio_Check(void);

extern volatile u32 gKeyState;

/* Announce two characters joining the party: a window with both members'
   glyphs and the joined line, then wait for the jingle or a key. */
void Party_ShowPairJoinedMessage(s32 first, s32 second)
{
    u8 *work = gWindowWork[0];
    struct UiWindow *window;
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

        ((struct UiRenderWork *)work)->dirty = 1;

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

        ((struct UiRenderWork *)work)->result[0] = zero;
        ((struct UiRenderWork *)work)->result[1] = zero;

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
u32 Resource_DecodeByteLz(const void *, void *);

/* graphics/resource/RenderOutput_LoadPair.c */
extern const u8 *RenderResource_PairSourceTable[];

void RenderResource_LoadPair(s32 group_index, s32 resource_index)
{
    void *staging_buffer = (void *)Runtime_AllocateBlock(14, 0x400);
    const u8 *resource = RenderResource_PairSourceTable[group_index];

    if (resource_index <= 0x5F) {
        Resource_DecodeByteLz(resource, staging_buffer);
        VramBlock_LoadCached(resource_index, 0x200, staging_buffer);
        Runtime_ReleaseHeapBlock(14);
    }
}

/* graphics/resource/RenderResource_CreatePair.c */
struct RenderOutput *RenderResource_CreatePair(
    s32 group,
    struct RenderInput *input,
    s32 x,
    s32 y)
{
    s32 index;
    struct RenderOutput *first;
    struct RenderOutput *second;

    index = Resource_FindFreeEntry();
    if (index > 95)
        return NULL;

    RenderResource_LoadPair(group, index);
    /* Two 32x16 OBJ sprites; the right half starts eight tiles later. */
    first = RenderOutput_Create(index, 0x80004000, input, x, y);
    first->sentinel = 0xFD;
    second = RenderOutput_Create(index, 0x80004000, input, x + 32, y);
    second->sentinel = 0xFD;
    second->table.bits.index += 8;
    return first;
}
