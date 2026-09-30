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

s32 Shop_CountUnits(void)
{
    u8 *work = (u8 *)gMenuWork;
    u8 *base;
    s32 active = 0;
    s32 variant = (s8)work[0x3AA];
    s32 index = 0;
    s32 offset;

    if (active < *(s8 *)(work + 0x3A7)) {
        base = work + 2;
        offset = 0x36C;
        do {
            if (Shop_CanServe(*(s16 *)(base + offset), variant) != 0)
                active++;
            index++;
            offset += 2;
        } while (index < *(s8 *)(work + 0x3A7));
    }

    return active;
}
