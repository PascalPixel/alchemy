#include "TYPES.H"
#include "SCENE.H"
#include "RENDER_INPUT.H"
#include "RESOURCE.H"
#include "TBS_EDITION.H"

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
void Runtime_PushSlotEntry(s32 *, s32);
void WaitFrames(s32);
s32 Audio_Check(void);
void UiWork_Finalize(struct MessageWindow *, s32);
void Resource_ResetEntry(s32);

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

        work[RENDER_DIRTY_OFS] = 1;

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

        *(u16 *)(work + RENDER_RESULT_OFS) = zero;
        *(u16 *)(work + RENDER_RESULT_OFS + 2) = zero;

        UiWork_PushValueSlot(first, 1);
        UiWork_PushValueSlot(second, 1);

        UiText_QueueRenderEntries(window, UiText_BuildRenderEntriesMode1((s32)MsgPairJoinedParty), 68, 2, zero);

        Audio_PlayCue(81);

        do {
            Runtime_PushSlotEntry(entry1, 250);
            Runtime_PushSlotEntry(entry2, 250);
            WaitFrames(1);
        } while (Audio_Check() != 0 && (gKeyState & 0x303) == 0);

        UiWork_Finalize(window, 2);
        WaitFrames(1);
        Resource_ResetEntry(handle1);
        Resource_ResetEntry(handle2);
    }
}
u32 Resource_DecodeByteLz(const void *, void *);
void Runtime_ReleaseHeapBlock(s32);

/* graphics/resource/RenderOutput_LoadPair.c */
extern s32 RenderResource_PairSourceTable[];

void RenderResource_LoadPair(s32 group_index, s32 resource_index)
{
    void *staging_buffer = (void *)Runtime_AllocateBlock(14, 0x400);
    s32 resource_address = RenderResource_PairSourceTable[group_index];

    if (resource_index <= 0x5F) {
        Resource_DecodeByteLz((const void *)resource_address, staging_buffer);
        VramBlock_LoadCached(resource_index, 0x200, staging_buffer);
        Runtime_ReleaseHeapBlock(14);
    }
}

/* graphics/resource/RenderResource_CreatePair.c */
void RenderResource_LoadPair(s32 arg0, s32 arg1);
void *RenderResource_CreatePair(
    s32 arg0,
    struct RenderInput *arg1,
    s32 arg2,
    s32 arg3)
{
    s32 index;
    struct RenderOutput *first;
    struct RenderOutput *second;

    index = Resource_FindFreeEntry();
    if (index > 95)
        return NULL;

    RenderResource_LoadPair(arg0, index);
    /* Two 32x16 OBJ sprites; the right half starts eight tiles later. */
    first = RenderOutput_Create(index, 0x80004000, arg1, arg2, arg3);
    first->sentinel = 0xFD;
    second = RenderOutput_Create(index, 0x80004000, arg1, arg2 + 32, arg3);
    second->sentinel = 0xFD;
    second->table.bits.index += 8;
    return first;
}
