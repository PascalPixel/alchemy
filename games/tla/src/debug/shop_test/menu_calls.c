#include "types.h"
#include "text/msg_ids.h"

void Func_02000e40(s32 kind, s32 value);
void Func_02000e48(void);
void Func_02000e50(s32 kind, s32 value);
void Func_02000e60(s32 *a, s32 *b);
void Func_02000d20(s32 index);
void PartyInventory_GiveItem(s32 item, s32 count);
void Func_02000d38(s32 value);
void Func_02000dd0(s32 value);
void Func_02000d28(void);
void Func_02000d30(void);
void Func_02000c90(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_02000054(s32 message, s32 count);

void Func_0200012c(void) {
  Func_02000054((s32)MsgWeaponShopWelcome,
                MsgArmorShopWelcome - MsgWeaponShopWelcome);
}

void Func_02000144(void) {
  Func_02000054((s32)MsgArmorShopWelcome,
                MsgArmorShopWelcome - MsgWeaponShopWelcome);
}

void Func_0200015c(void) {
  Func_02000054((s32)MsgItemShopWelcome,
                MsgArmorShopWelcome - MsgWeaponShopWelcome);
}

void Func_02000178(void) {
  Func_02000054((s32)MsgWelcomeWearyWanderers,
                MsgRightWhenI - MsgWelcomeWearyWanderers);
}

void Func_02000190(s32 value) { Func_02000e40(1, value); }

void Func_0200019c(s32 value) { Func_02000e40(2, value); }

void Func_020001a8(s32 value) { Func_02000e40(3, value); }

void Func_020001b4(s32 value) { Func_02000e40(24, value); }

void Func_020001c0(s32 value) { Func_02000e50(0, value); }

void Func_020001cc(void) { Func_02000e48(); }
