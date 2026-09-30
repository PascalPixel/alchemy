#include "TYPES.H"
#include "RESOURCE.H"
#include "SYSTEM.H"
#include "RESOURCE_IDS.H"
#include "SCENE.H"
#include "GLOBAL_CELLS.H"
#include "TBS_EDITION.H"

struct SelectionNode_0801c188 {
    u8 padding_00[8];
    u16 no;
    u16 type;
};

struct TransferState_0801c188 {
    u8 padding_00[8];
    u16 no;
    u16 active;
    u16 handle;
    u16 transfer_id;
    u8 padding_10[18];
    s16 x;
    s16 y;
    s16 width;
};

struct ResourceBuffer_0801c188 {
    u8 payload[0x604];
    void *resource;
};

extern u8 *gResQueueWork;
struct SelectionNode_0801c188 *NodeChain_GetNodeAtCount(void *state);
struct ResourceBuffer_0801c188 *Runtime_AllocateHeapBlock(s32 owner, s32 size);
void Resource_DecodeByteLz(void *source, void *destination);
u16 VramBlock_LoadCached(s32 handle, s32 size, void *buffer);

extern u8 Data_03001e98[];
s32 Resource_ResetEntry(u32 index);

void UiWindow_OpenMode1AndWaitFrame(void);
s32 Menu_SelectTopEntry(s32);
s32 UiWork_CloseAndRelease(void);
s32 ItemMenu_Open(void);
s32 Menu_OpenActionFlow(void);
extern u8 *gWork;
s32 Object_GetTriggerTileAheadOfCurrentFar(void);
s32 Menu_OpenConfirmPromptFar(void);
s32 RunAssetSelectionScreenFar(void);
void BattleFx_ClearRandomParticlesFar(void);

/* ui/window/open_mode1_and_wait_frame.c */
void UiWindow_CreateWithLayoutBounds(s32);

void Menu_LoadSelectedResource(void)
{
    u8 *state = gResQueueWork;
    struct SelectionNode_0801c188 *selection = NodeChain_GetNodeAtCount(state);
    struct TransferState_0801c188 *transfer;
    struct ResourceBuffer_0801c188 *buffer;
    u8 *tbl;
    void *resource;
    s32 no;

    if (selection->type != 1 && selection->type != 6)
        return;

    buffer = Runtime_AllocateHeapBlock(17, 0x608);
    transfer = (struct TransferState_0801c188 *)(state + 0x30C);
    no = selection->no;
    tbl = Resource_GetTableEntry((s32)&ResourceId_CommandIcons);
    {
        void **destination = &buffer->resource;
        resource = tbl
            + *(u16 *)(tbl + selection->no * 2);
        *destination = resource;
    }
    Resource_DecodeByteLz(resource, buffer);

    if (transfer->active == 0)
        transfer->handle = Resource_FindFreeEntry();
    transfer->transfer_id =
        VramBlock_LoadCached(transfer->handle, 0x400, buffer);
    transfer->active = 1;
    transfer->no = no;
    transfer->x = 40;
    transfer->y = 40;
    transfer->width = 240;
    Runtime_ReleaseHeapBlock(17);
}

void Resource_ResetPendingTransfer(void)
{
    u32 offset = 0x30c;
    u8 *work = *(u8 **)((u32)&Data_03001e98) + offset;

    if (*(u16 *)(work + 0x0a) != 0) {
        Resource_ResetEntry(*(u16 *)(work + 0x0c));
        *(u16 *)(work + 0x0a) = offset = 0;
    }
}

/* menu/selection/run_top_selection.c */
#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) || \
    defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)

#define HAS_LOCALIZED_MENU_GUARD 1
#endif

s32 Menu_RunTopSelection(void)
{
    s32 ret;
    s32 sel;
    u8 *state;

    state = gWork;
    sel = 0;

loop:
    UiWindow_OpenMode1AndWaitFrame();
    sel = Menu_SelectTopEntry(sel);
#if defined(HAS_LOCALIZED_MENU_GUARD)
    state[0xcca] = 1;
    if (*(s16 *)(state + 0xcb8) != 0) {
        BattleFx_ClearRandomParticlesFar();
        WaitFrames(1);
    }
#endif
    ret = UiWork_CloseAndRelease();

    switch (sel) {
    case 0:
        ret = Object_GetTriggerTileAheadOfCurrentFar();
        if (ret == 0)
            ret = 0xff;
        *(u16 *)(state + 0x17a) = ret;
        break;
    case 1:
        ret = Menu_OpenConfirmPromptFar();
        if (ret == -1)
            goto loop;
        break;
    case 2:
        ret = ItemMenu_Open();
        if (ret != 0)
            goto loop;
        break;
    case 3:
        ret = RunAssetSelectionScreenFar();
        if (ret == -1)
            goto loop;
        break;
    case 4:
        ret = Menu_OpenActionFlow();
        if (ret == -1)
            goto loop;
        break;
    default:
        break;
    }

#if defined(HAS_LOCALIZED_MENU_GUARD)
    state[0xcca] = 0;
#endif
    return ret;
}

void UiWindow_OpenMode1AndWaitFrame(void)
{
    UiWindow_CreateWithLayoutBounds(1);
    WaitFrames(1);
}
