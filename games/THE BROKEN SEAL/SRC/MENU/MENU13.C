#include "SELECT.H"
#include "TYPES.H"
#include "RESOURCE.H"
#include "SYSTEM.H"
#include "RESOURCE_IDS.H"
#include "SCENE.H"
#include "GLOBAL_CELLS.H"
#include "TBS_EDITION.H"

struct ResourceBuffer_0801c188 {
    u8 payload[0x604];
    void *resource;
};

extern struct SelectionScreen *gResQueueWork;
struct SelectionNode *NodeChain_GetNodeAtCount(void *state);
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
    struct SelectionScreen *state = gResQueueWork;
    struct SelectionNode *selection = NodeChain_GetNodeAtCount(state);
    struct SelectionNode *transfer;
    struct ResourceBuffer_0801c188 *buffer;
    u8 *tbl;
    void *resource;
    s32 no;

    if (selection->kind != 1 && selection->kind != 6)
        return;

    buffer = Runtime_AllocateHeapBlock(17, 0x608);
    transfer = &state->records[15];
    no = selection->base;
    tbl = Resource_GetTableEntry((s32)&ResourceId_CommandIcons);
    {
        void **destination = &buffer->resource;
        resource = tbl
            + *(u16 *)(tbl + selection->base * 2);
        *destination = resource;
    }
    Resource_DecodeByteLz(resource, buffer);

    if (transfer->kind == 0)
        transfer->slot = Resource_FindFreeEntry();
    transfer->tile =
        VramBlock_LoadCached(transfer->slot, 0x400, buffer);
    transfer->kind = 1;
    transfer->base = no;
    transfer->scale = 40;
    transfer->scale_step = 40;
    transfer->scale_end = 240;
    Runtime_ReleaseHeapBlock(17);
}

void Resource_ResetPendingTransfer(void)
{
    struct SelectionNode *transfer = &gResQueueWork->records[15];

    if (transfer->kind != 0) {
        Resource_ResetEntry(transfer->slot);
        transfer->kind = 0;
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
