/* The item merchant's thoughts when read with Psynergy: one line before
 * Lunpa's trade reopens and one after. */
#include "INTERIORS.H"
#include "SCENE_IDS.H"

extern u8 MsgRunpaIsntWeaponsVendors[];
extern u8 MsgRunpaItemMerchantSealedThoughts[];

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

extern u8 MsgRunpaTravelingPriest[];

void ItemMerchant_ReadMind(void)
{
    if (GameFlag_IsSet(FLAG_LUNPA_TRADE_REOPENED) != 0) {
        Event_Begin();
        Event_SetMessage((s32)MsgRunpaIsntWeaponsVendors);
        Event_ShowMessage(ACTOR_ITEM_MERCHANT, 0);
        Event_End();
    } else {
        Event_Begin();
        Event_SetMessage((s32)MsgRunpaItemMerchantSealedThoughts);
        Event_ShowMessage(ACTOR_ITEM_MERCHANT, 0);
        Event_End();
    }
}

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

/* Lunpa's interiors open with the window transition. Entering from the
   Suhara gate house clears the location-name flag and saves these rooms and
   that entrance; the three shop counters' sprite flags are cleared. */
s32 Scene_Initialize(void)
{
    s16 entrance;

    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
    entrance = gGameState.entrance;
    if (entrance == ROOM_SUHARA_GATE_HOUSE) {
        GameFlag_Clear(FLAG_SHOW_LOCATION_NAME);
        gGameState.saved_scene = (s32)&SceneId_RunpaSuhara;
        gGameState.saved_entrance = entrance;
    }
    Actor_SetSpriteFlags(Actor_Get(ACTOR_WEAPON_COUNTER), 0);
    Actor_SetSpriteFlags(Actor_Get(ACTOR_ARMOR_COUNTER), 0);
    Actor_SetSpriteFlags(Actor_Get(ACTOR_ITEM_COUNTER), 0);
    return 0;
}

void TravelingPriest_Talk(void)
{
    struct FieldActor *leader;

    leader = Actor_Get(ACTOR_PARTY_LEADER);
    if (FACING_IS_NORTH(leader->facing)) {
        Sanctum_Open(ACTOR_TEMPLE_PRIEST);
    } else {
        Event_SetMessage((s32)MsgRunpaTravelingPriest);
        Event_ShowMessage(ACTOR_TRAVELING_PRIEST, 0);
    }
}
