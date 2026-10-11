#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "callback_scheduler.h"
#include "mem.h"
#include "types.h"
#include "io_reg.h"
#include "window.h"
#include "system.h"
#include "scene.h"
#include "render_input.h"
#include "resource.h"
#include "tbs_edition.h"
#include "text/msg_ids.h"
s32 AudioCommand_GetStateByteFar(void);

#define gKeyState (*(volatile u32 *)&gKeyState)

#define Party_LookupCharacterValueByFlag32 ((s32 (*)(s32))Party_LookupCharacterValueByFlag32)
#define UiGlyph_LoadEntryWithPalette ((void (*)(s32,s32,s32*,s32*,s32,s32))UiGlyph_LoadEntryWithPalette)


void UiWindow_DrawDividerLine(struct UiWindow *, s32, s32, s32, s32);
void UiWork_PushValueSlot(s32, s32);
struct UiChannelSlot *UiText_QueueRenderEntries();
void AudioCommand_PlayFar(s32);


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

    // the window starts as NULL, though it is assigned before it is read; the same zero is the style argument below
    window = NULL;
    entry1 = sprite1;
    window = UiWindow_Create(1, 1, 28, 5, 0);
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

        AudioCommand_PlayFar(81);

        do {
            Runtime_PushSlotEntry(entry1, 250);
            Runtime_PushSlotEntry(entry2, 250);
            WaitFrames(1);
        } while (AudioCommand_GetStateByteFar() != 0 && (gKeyState & (KEY_A | KEY_B | KEYS_SHOULDERS)) == 0);

        UiWork_Finalize(window, 2);
        WaitFrames(1);
        Resource_ResetEntry(handle1);
        Resource_ResetEntry(handle2);
    }
}
u32 Resource_DecodeByteLz(const void *, void *);

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
    first = RenderOutput_Create(index, 0x80004000, input, x, y);
    first->sentinel = 0xFD;
    second = RenderOutput_Create(index, 0x80004000, input, x + 32, y);
    second->sentinel = 0xFD;
    second->table.bits.index += 8;
    return first;
}
