#include "INTERIORS.H"
extern u8 MsgRunpaArmorMerchantReopened[];
extern u8 MsgRunpaArmorMerchantReopenedThoughts[];
extern u8 MsgRunpaArmorMerchantSealed[];
extern u8 MsgRunpaArmorMerchantSealedThoughts[];
extern u8 MsgRunpaItemMerchantReopened[];
extern u8 MsgRunpaItemMerchantSealed[];
extern u8 MsgRunpaWeaponMerchantReopened[];
extern u8 MsgRunpaWeaponMerchantReopenedThoughts[];
extern u8 MsgRunpaWeaponMerchantSealed[];
extern u8 MsgRunpaWeaponMerchantSealedThoughts[];

void ItemMerchant_Talk(void)
{
    struct FieldActor *leader;

    leader = Actor_Get(ACTOR_PARTY_LEADER);
    if (FACING_IS_NORTH(leader->facing)) {
        Shop_Open(SHOP_LUNPA_ITEMS, ACTOR_ITEM_MERCHANT);
    } else if (GameFlag_IsSet(FLAG_LUNPA_TRADE_REOPENED) != 0) {
        Event_Begin();
        Event_SetMessage((s32)MsgRunpaItemMerchantReopened);
        Event_ShowMessage(ACTOR_ITEM_MERCHANT, 0);
        Event_End();
    } else {
        Event_Begin();
        Event_SetMessage((s32)MsgRunpaItemMerchantSealed);
        Event_ShowMessage(ACTOR_ITEM_MERCHANT, 0);
        Event_End();
    }
}

void WeaponMerchant_ReadMind(void)
{
    if (GameFlag_IsSet(FLAG_LUNPA_TRADE_REOPENED) != 0) {
        Event_SetMessage((s32)MsgRunpaWeaponMerchantReopenedThoughts);
        Event_ShowMessage(ACTOR_WEAPON_MERCHANT, 0);
    } else {
        Event_SetMessage((s32)MsgRunpaWeaponMerchantSealedThoughts);
        Event_ShowMessage(ACTOR_WEAPON_MERCHANT, 0);
    }
}

void WeaponMerchant_Talk(void)
{
    struct FieldActor *leader;
    s32 facing;

    leader = Actor_Get(ACTOR_PARTY_LEADER);
    facing = leader->facing;
    if (GameFlag_IsSet(FLAG_LUNPA_TRADE_REOPENED) != 0) {
        if (FACING_IS_NORTH(facing)) {
            Shop_Open(SHOP_LUNPA_WEAPONS, ACTOR_WEAPON_MERCHANT);
        } else {
            Event_Begin();
            Event_SetMessage((s32)MsgRunpaWeaponMerchantReopened);
            Event_ShowMessage(ACTOR_WEAPON_MERCHANT, 0);
            Event_End();
        }
    } else {
        Event_SetMessage((s32)MsgRunpaWeaponMerchantSealed);
        Event_ShowMessage(ACTOR_WEAPON_MERCHANT, 0);
    }
}

void ArmorMerchant_ReadMind(void)
{
    if (GameFlag_IsSet(FLAG_LUNPA_TRADE_REOPENED) != 0) {
        Event_SetMessage((s32)MsgRunpaArmorMerchantReopenedThoughts);
        Event_ShowMessage(ACTOR_ARMOR_MERCHANT, 0);
    } else {
        Event_SetMessage((s32)MsgRunpaArmorMerchantSealedThoughts);
        Event_ShowMessage(ACTOR_ARMOR_MERCHANT, 0);
    }
}

void ArmorMerchant_Talk(void)
{
    struct FieldActor *leader;
    s32 facing;

    leader = Actor_Get(ACTOR_PARTY_LEADER);
    facing = leader->facing;
    if (GameFlag_IsSet(FLAG_LUNPA_TRADE_REOPENED) != 0) {
        if (FACING_IS_NORTH(facing)) {
            Shop_Open(SHOP_LUNPA_ARMOR, ACTOR_ARMOR_MERCHANT);
        } else {
            Event_Begin();
            Event_SetMessage((s32)MsgRunpaArmorMerchantReopened);
            Event_ShowMessage(ACTOR_ARMOR_MERCHANT, 0);
            Event_End();
        }
    } else {
        Event_SetMessage((s32)MsgRunpaArmorMerchantSealed);
        Event_ShowMessage(ACTOR_ARMOR_MERCHANT, 0);
    }
}

