#include "types.h"
#include "scene.h"
#include "global_cells.h"

extern u8 RomBytes_080308a0[];

/* ui/icon/build_ability_icon_tiles.c */
/* ui/icon/icon_build_ability_icon_tiles.c */
/* ui/icon/build_ability_icon_tiles.c */
typedef struct {
    u8 pad0[0x400];
    u8 f400;
    u8 pad401[0x600 - 0x401];
    s16 f600;
    s16 f602;
    s32 f604;
} FontTransfer;

extern FontTransfer *Runtime_AllocateHeapBlock(s32 arg0, s32 arg1);

extern s32 Resource_FindFreeSlot(void);
extern s32 Resource_CopyData(s32 index, s32 size, u8 *destination);

extern s32 RomBytes_08029a10[];
extern s32 gRom2[];

void UiIcon_BuildAbilityIconTiles(u32 glyph, s32 with_base, s32 *src,
                   s32 *dst, s32 reuse)
{
    FontTransfer *work;
    s32 slot;

    work = Runtime_AllocateHeapBlock(0x11, 0x608);
    slot = 0;

    if (glyph >= Ui_Check())
        glyph = 0;

    if (with_base != 0) {
        work->f604 = RomBytes_08029a10[2];
        work->f600 = 2;
        work->f602 = 2;
        Ui_Apply(work, 0);
        slot = 1;
    }

    work->f604 = gRom2[glyph];
    work->f600 = 2;
    work->f602 = 2;
    Ui_Apply(work, slot);

    if (reuse == 0)
        *src = Resource_FindFreeSlot();

    *dst = Resource_CopyData(*src, 0x80, &work->f400);
    Ui_Do(0x11);
}

/* ui/icon/prepare_transfer_from_table_entry.c */
struct State_0801a4c0 {
    u8 filler0[0x600];
    u16 first;
    u16 second;
    u32 value;
};

extern struct State_0801a4c0 *gIw;

void Ui_PrepareTransferFromTableEntry(u32 index)
{
    struct State_0801a4c0 *state = gIw;

    state->value = RomBytes_080308a0[index];
    state->first = 2;
    state->second = 2;
    Ui_Apply(state, 0);
}

/* resource/clear_owner_list_and_counters.c */
/* resource/clear_owner_list_and_counters.c */
void Resource_ClearOwnerListAndCounters(void)
{
    void *state;

    state = *(void **)ADDR_03001E98;
    FIELD_AT_OFFSET(state, s32 *, 0x348) = 0;
    FIELD_AT_OFFSET(state, s16 *, 0x39A) = 0;
    if (0x80 & FIELD_AT_OFFSET(state, u16 *, 0x39E)) {
        FIELD_AT_OFFSET(state, s16 *, 0x39C) = 0;
        FIELD_AT_OFFSET(state, u16 *, 0x39E) = 0U;
    }
    FIELD_AT_OFFSET(state, s16 *, 0x3A0) = 0;
    FIELD_AT_OFFSET(state, s16 *, 0x394) = 0;
}

/* resource/push_pending_pair.c */
struct State_0801a7c0 {
    u8 filler0[0x354];
    u16 first[16];
    u16 second[16];
    u16 cnt;
};

extern struct State_0801a7c0 *volatile gIwClearOwnerListAndCounters;

void Resource_PushPendingPair(u32 first, u32 second)
{
    struct State_0801a7c0 *state = gIwClearOwnerListAndCounters;
    u16 cnt = state->cnt;

    if (cnt != 16) {
        state->first[cnt] = first;
        state->second[cnt] = second;
        state->cnt++;
    }
}

/* ui/render/reserved_work_no_op.c */
void UiWork_ReservedNoOpA90C(void)
{
}
