#include "INTERIORS.H"

void ItemMerchant_Talk(void)
{
    struct FieldActor *leader;

    leader = Actor_Get(ACTOR_PARTY_LEADER);
    if (FACING_IS_NORTH(leader->facing)) {
        Shop_Open(SHOP_LUNPA_ITEMS, ACTOR_ITEM_MERCHANT);
    } else if (GameFlag_IsSet(FLAG_LUNPA_TRADE_REOPENED) != 0) {
        Event_Begin();
        Event_SetMessage(MSG_ITEM_MERCHANT_REOPENED);
        Event_ShowMessage(ACTOR_ITEM_MERCHANT, 0);
        Event_End();
    } else {
        Event_Begin();
        Event_SetMessage(MSG_ITEM_MERCHANT_SEALED);
        Event_ShowMessage(ACTOR_ITEM_MERCHANT, 0);
        Event_End();
    }
}

void WeaponMerchant_ReadMind(void)
{
    if (GameFlag_IsSet(FLAG_LUNPA_TRADE_REOPENED) != 0) {
        Event_SetMessage(MSG_WEAPON_MERCHANT_REOPENED_THOUGHTS);
        Event_ShowMessage(ACTOR_WEAPON_MERCHANT, 0);
    } else {
        Event_SetMessage(MSG_WEAPON_MERCHANT_SEALED_THOUGHTS);
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
            Event_SetMessage(MSG_WEAPON_MERCHANT_REOPENED);
            Event_ShowMessage(ACTOR_WEAPON_MERCHANT, 0);
            Event_End();
        }
    } else {
        Event_SetMessage(MSG_WEAPON_MERCHANT_SEALED);
        Event_ShowMessage(ACTOR_WEAPON_MERCHANT, 0);
    }
}

void ArmorMerchant_ReadMind(void)
{
    if (GameFlag_IsSet(FLAG_LUNPA_TRADE_REOPENED) != 0) {
        Event_SetMessage(MSG_ARMOR_MERCHANT_REOPENED_THOUGHTS);
        Event_ShowMessage(ACTOR_ARMOR_MERCHANT, 0);
    } else {
        Event_SetMessage(MSG_ARMOR_MERCHANT_SEALED_THOUGHTS);
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
            Event_SetMessage(MSG_ARMOR_MERCHANT_REOPENED);
            Event_ShowMessage(ACTOR_ARMOR_MERCHANT, 0);
            Event_End();
        }
    } else {
        Event_SetMessage(MSG_ARMOR_MERCHANT_SEALED);
        Event_ShowMessage(ACTOR_ARMOR_MERCHANT, 0);
    }
}

