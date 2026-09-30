#include "TYPES.H"
#include "SYSTEM.H"

struct ShopServiceWork {
    u8 unk_000[0x380];
    void *mode_state;
    u8 unk_384[0x20];
    u16 value;
    u8 unk_3a6[4];
    s8 mode;
};

u8 *Owner_GetStateFar(s32);
extern struct ShopServiceWork *gMenuWork;
s32 Shop_CanServe(s32 selection, s32 variant);
extern u8 MsgReviveService;
extern u8 MsgCurePoisonService;
extern u8 MsgExorcismService;
extern u8 MsgRemoveCurseService;
s32 BattleFx_GetResourceIdFar(u16);
void UiWork_FinalizePendingCoreFar(void);
s32 Shop_MsgByMode(s32 value);
void UiText_OpenMessageWindowFar(s32, s32, s32, s32);

void UiMessage_ShowResolvedAndRestoreState(s32 arg0)
{
    struct ShopServiceWork *state;
    void **slot;
    s32 value;
    u8 saved;

    state = gMenuWork;
    slot = &state->mode_state;
    saved = *(u8 *)((u8 *)*slot + 5);
    value = BattleFx_GetResourceIdFar(state->value);
    arg0 = Shop_MsgByMode(arg0);
    *(u8 *)((u8 *)*slot + 5) = 13;
    UiWork_FinalizePendingCoreFar();
    UiText_OpenMessageWindowFar(arg0, 5, 0, (value << 16) | 0x22);
    while (UiWork_IsCompleteFar() == 0)
        WaitFrames(1);
    WaitFrames(1);
    *(u8 *)((u8 *)state->mode_state + 5) = saved;
}
