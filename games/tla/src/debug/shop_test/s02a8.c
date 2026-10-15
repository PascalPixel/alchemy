#include "types.h"
#include "party_state.h"
#include "flag_call.h"

extern const u8 MsgAhSoYou[];
extern const u8 MsgICanTWait[];
extern const u8 MsgMyWifeHas[];
extern const u8 MsgYouWantMe[];
extern const u8 MsgThisThisIs[];
extern const u8 MsgItTakesOur[];
extern const u8 MsgHereSThe[];
extern const u8 MsgManSunshineWould[];

void Battle_WaitMode0(s32 frames);
s32 UiText_OpenMessageAtObject(s32 id, s32 flags);
s32 Inventory_PromptAndSetObjectMode(s32 id, s32 flags);
s32 PartyInventory_GiveItem(s32 item, s32 count);
void Party_AdjustCoins(s32 amount);
void Func_02000d08(s32 value, s32 slot);
u16 *Func_02000d48(s32 item);
s32 Func_02000dc8(s32 item);
void Func_02000e08(s32 message);
void Func_02000e18(s32 actor, s32 flags);
void Func_02000e28(s32 item, s32 mode);
s32 Func_02000e30(void);
void Func_02000e58(s32 actor);
void Func_02000e68(s32 value);
s32 Func_02000e70(void);

void Func_020002a8(s32 actor)
{
    if (!Flag_IsSet(0x10f)) {
        Func_02000e08((s32)MsgThisThisIs);
        Func_02000e18(actor, 0);
    } else if (Flag_IsSet(0x200)) {
        Func_02000e08((s32)MsgItTakesOur);
        Func_02000e18(actor, 0);
    } else if (Func_02000e70()) {
        s32 item = Func_02000e70();
        u32 price = *Func_02000d48(item);
        s32 message;

        Func_02000d08(item, 2);
        Func_02000d08(price, 5);
        message = (s32)MsgHereSThe;
        Func_02000e08(message);
        UiText_OpenMessageAtObject(actor, 0);
        if (Inventory_PromptAndSetObjectMode(Func_02000e30(), 0) != 0) {
            Func_02000e08(message + 1);
            Func_02000e18(actor, 0);
        } else if (Func_02000dc8(item) < 0) {
            Func_02000e08(message + 2);
            Func_02000e18(actor, 0);
        } else if ((u32)gPartyState.coins < price) {
            Func_02000e08(message + 3);
            Func_02000e18(actor, 0);
        } else {
            Func_02000e08(message + 4);
            Func_02000e18(actor, 0);
            Func_02000e28(item, 3);
            PartyInventory_GiveItem(item, 0);
            Func_02000e68(0);
            Party_AdjustCoins(-price);
        }
    } else {
        Func_02000e08((s32)MsgManSunshineWould);
        Func_02000e18(actor, 0);
    }
}
