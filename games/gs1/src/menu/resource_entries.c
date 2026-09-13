#include "types.h"
#include "sound_ids.h"
#include "resource.h"
#include "global_cells.h"

struct MenuSelectionState {
    u8 padding000[0x78];
    void *work;
    u8 padding07c[8];
    u8 resource_ids[8];
    s16 selection;
    s16 item_count;
    s16 field090;
    s16 resource_base;
};

extern struct MenuSelectionState *Data_03001f38;
extern u8 Data_080373ef[];
extern u8 Data_0000001f;

void RenderOutput_PrepareForRedraw(void *work);
void Func_0801e7c0(s32 resource_id, void *work, s32 x, s32 y);
void WaitFrames(s32 frames);
void Audio_PlayCue(s32 sound_id);

static inline s32 AbsoluteDifference(s32 diff, s32 lhs, s32 rhs)
{
    if (diff >= 0)
        return diff;
    return rhs - lhs;
}

s32 Menu_SelectResource(s32 start, s32 goal)
{
    s16 resource_base;
    s16 cur;
    s32 diff;
    s32 resource_id;
    s32 dist;
    s32 pos;
    s32 step;
    s32 delay;
    const u8 *tbl;
    struct MenuSelectionState *state;

    state = Data_03001f38;
    step = 1;
    delay = 12;
    state->selection = (s16)start;
    if (goal < start)
        step = -1;
    pos = start;

    for (;;) {
        RenderOutput_PrepareForRedraw(state->work);
        resource_base = state->resource_base;
        if (resource_base != 0) {
            resource_id = resource_base + state->selection;
        } else {
            resource_id = state->resource_ids[state->selection] + (s32)&Data_0000001f;
        }
        Func_0801e7c0(resource_id, state->work, 0, 0);

        cur = state->selection;
        tbl = Data_080373ef;
        diff = cur - goal;
        dist = AbsoluteDifference(diff, cur, goal);
        WaitFrames(tbl[dist] + delay);

        if (pos == goal)
            break;

        state->selection = (s16)((u16)state->selection + step);
        Audio_PlayCue(SOUND_MENU_CURSOR_MOVE);
        delay = 0;
        pos += step;
    }

    WaitFrames(48);
    Audio_PlayCue(SOUND_MENU_CONFIRM);
    return goal;
}


extern u8 Data_000000f1[];

u32 Runtime_BumpAllocate(s32 size);
u32 Func_080053e8(const void *, void *);
void Resource_CopyData(s32, s32, void *);
void Func_08002df0(void *);

void Menu_LoadResourceSlot(s32 slot, s32 index)
{
    s32 size = 1024;
    void *buffer = (void *)Runtime_BumpAllocate(size);
    u16 *base = GetResource((s32)Data_000000f1);

    /* 表内の相対位置から転送元を求める。 */
    Func_080053e8((void *)((u32)base + base[index]), buffer);
    Resource_CopyData(slot, size, buffer);
    Func_08002df0(buffer);
}


extern s32 Func_08004080(void);

void Menu_AppendResourceEntry(s32 no)
{
    u8 *base;
    u8 *entry;
    s16 index;
    s32 slot;
    s32 off;
    s32 flags;

    base = *(u8 **)ADDR_03001F38;
    index = *(s16 *)(base + 142);
    if (index <= 5)
    {
        *(u16 *)(base + 142) = *(u16 *)(base + 142) + 1;
        entry = base + index * 20;
        slot = Func_08004080();
        Menu_LoadResourceSlot(slot, no);
        *(u16 *)(entry + 12) = index * 24 + 32;
        flags = 136;
        *(u16 *)(entry + 14) = flags;
        off = index + 132;
        *(u16 *)(entry + 18) = slot;
        base[off] = (u8)no;
    }
}
