#include "TYPES.H"
extern u8 Data_03001e8c[];

#define FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

struct Work;
struct Slot;

struct UiTextMessageWorkGlobals {
    void *state;
    u8 padding4[0x54];
    void *control;
};

extern volatile struct UiTextMessageWorkGlobals gWindowWork;

s32 UiText_BuildRenderEntries(s32, s32);
struct Work *UiWindow_Create(s32, s32, s32, s32, s32);
void UiWindow_MapTextCanvasTiles(s32, s32, s32, s32, s32);
struct Slot *UiWork_ActivateChannel(struct Work *, s32, s32);
void UiWork_Finalize(struct Work *, s32);

s32 UiText_OpenEntryMessage(s32 no, s32 argument)
{
    u8 *base = *(u8 **)((u32)&Data_03001e8c);
    s32 entry;
    s32 entry_offset;
    s32 result = 0;
    /* FAKEMATCH: an unused buffer reproduces the reference's 16-byte frame. */
    u8 unused[8];

    *(u16 *)(base + 0x12f4) = 0;
    *(u16 *)(base + 0x12f6) = 0;
    entry = Func_08018038(argument, 1);
    entry_offset = entry * 2;
    entry_offset += 0xeb0;
    if (*(u16 *)(base + entry_offset) == 0)
        return 0;
    if (no == 0)
        return 0;
    result = UiText_QueueRenderEntries(no, entry, 0, 0, 0, 1);
    if (result == 0)
        return 0;
    return result;
}
