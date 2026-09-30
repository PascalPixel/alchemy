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

void UiMessage_ShowResolvedAndWait(s32 value)
{
    s32 no;

    no = BattleFx_GetResourceIdFar(gMenuWork->value);
    UiWork_FinalizePendingCoreFar();
    value = Shop_MsgByMode(value);
    UiText_OpenMessageWindowFar(value, 5, 0, (no << 0x10) | 0x22);
    while (UiWork_IsCompleteFar() == 0) {
        WaitFrames(1U);
    }
    WaitFrames(1U);
}
