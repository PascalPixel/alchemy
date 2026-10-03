#include "INTERIORS.H"
#include "SCENE_IDS.H"

extern u8 MsgRunpaChefOffersStory[];
extern u8 MsgRunpaChefThinksOfPrisoner[];
extern u8 MsgRunpaInnClerkAsksAboutGuest[];
extern u8 MsgRunpaInnkeeperOffersOwnHome[];
extern u8 MsgRunpaInnkeeperOffersRoom[];
extern u8 MsgRunpaTemplePriestReopened[];
extern u8 MsgRunpaTemplePriestSealed[];

const struct SceneEntrance gInteriorEntrances[] = {
    { 0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { ROOM_FIRST_HOME, CONDITION_ALWAYS, 192, 0, 184, FACING_NORTH, 0, 16, 0, 256, 224, 0 },
    { ROOM_SECOND_HOME, CONDITION_ALWAYS, 432, 0, 264, FACING_NORTH, 0, 296, 48, 536, 288, 0 },
    { ROOM_THIRD_HOME, CONDITION_ALWAYS, 160, 0, 584, FACING_NORTH, 0, 16, 400, 256, 624, 0 },
    { ROOM_ARMS_SHOP, CONDITION_ALWAYS, 464, 0, 616, FACING_NORTH, 0, 296, 384, 536, 640, 0 },
    { ROOM_ITEM_SHOP, CONDITION_ALWAYS, 704, 0, 584, FACING_NORTH, 0, 576, 352, 816, 608, 0 },
    { ROOM_INN, CONDITION_ALWAYS, 288, 0, 840, FACING_NORTH, 0, 40, 656, 368, 944, 0 },
    { ROOM_TEMPLE, CONDITION_ALWAYS, 688, 0, 280, FACING_NORTH, 0, 568, 32, 808, 304, 0 },
    { ROOM_EIGHTH, CONDITION_ALWAYS, 560, 0, 880, FACING_NORTH, 0, 400, 656, 640, 912, 0 },
    { ROOM_NINTH, CONDITION_ALWAYS, 672, 0, 888, FACING_NORTH, 0, 520, 656, 760, 896, 0 },
    { ROOM_SUHARA_GATE_HOUSE, CONDITION_ALWAYS, 832, 0, 848, FACING_NORTH, 0, 680, 656, 920, 896, 0 },
    { ROOM_TEMPLE_SECOND_ENTRANCE, CONDITION_ALWAYS, 688, 0, 184, FACING_NORTH, 0, 568, 32, 808, 288, 0 },
    { SCENE_TABLE_END },
};

const u32 gInteriorExits[] = {
    SCENE_EXITS(SCENE_RUNPA_SUHARA),
    SCENE_EXIT(ROOM_FIRST_HOME, SCENE_RUNPA_MURA, LUNPA_ENTRANCE_FROM_FIRST_HOME),
    SCENE_EXIT(ROOM_SECOND_HOME, SCENE_RUNPA_MURA, LUNPA_ENTRANCE_FROM_SECOND_HOME),
    SCENE_EXIT(ROOM_THIRD_HOME, SCENE_RUNPA_MURA, LUNPA_ENTRANCE_FROM_THIRD_HOME),
    SCENE_EXIT(ROOM_ARMS_SHOP, SCENE_RUNPA_MURA, LUNPA_ENTRANCE_FROM_ARMS_SHOP),
    SCENE_EXIT(ROOM_ITEM_SHOP, SCENE_RUNPA_MURA, LUNPA_ENTRANCE_FROM_ITEM_SHOP),
    SCENE_EXIT(ROOM_INN, SCENE_RUNPA_MURA, LUNPA_ENTRANCE_FROM_INN),
    SCENE_EXIT(ROOM_TEMPLE, SCENE_RUNPA_MURA, LUNPA_ENTRANCE_FROM_TEMPLE),
    SCENE_EXIT(ROOM_EIGHTH, SCENE_RUNPA_MURA, LUNPA_ENTRANCE_FROM_EIGHTH_ROOM),
    SCENE_EXIT(ROOM_SUHARA_GATE_HOUSE, SCENE_SUHARA_GATE, SUHARA_GATE_ENTRANCE_FROM_LUNPA),
    SCENE_EXITS_END,
};

const struct ScenePlacement gInteriorPlacements[] = {
    { SPRITE_TOWNSPERSON_D, CONDITION_ALWAYS, ACTOR_WANDER,
      PIXELS(144), 0, PIXELS(168), FACING_WEST, TALK_FACE_PARTY, 0 },
    { SPRITE_TOWNSPERSON_C, CONDITION_ALWAYS, ACTOR_STAND,
      PIXELS(96), 0, PIXELS(128), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_TOWNSPERSON_A, CONDITION_ALWAYS, ACTOR_WANDER,
      PIXELS(392), 0, PIXELS(240), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_TOWNSPERSON_F, CONDITION_ALWAYS, ACTOR_STAND,
      PIXELS(403), 0, PIXELS(138), FACING_EAST, TALK_FACE_PARTY, 0 },
    { SPRITE_TOWNSPERSON_B, CONDITION_ALWAYS, ACTOR_STAND,
      PIXELS(160), 0, PIXELS(508), FACING_SOUTH - FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_TOWNSPERSON_E, CONDITION_ALWAYS, ACTOR_STAND,
      PIXELS(88), 0, PIXELS(584), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_WEAPON_MERCHANT, CONDITION_ALWAYS, ACTOR_STAND,
      PIXELS(376), 0, PIXELS(531), FACING_SOUTH - FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_ARMOR_MERCHANT, CONDITION_ALWAYS, ACTOR_STAND,
      PIXELS(464), 0, PIXELS(531), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_ITEM_MERCHANT, CONDITION_ALWAYS, ACTOR_STAND,
      PIXELS(720), 0, PIXELS(499), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_INNKEEPER, CONDITION_ALWAYS, ACTOR_STAND,
      PIXELS(264), 0, PIXELS(744), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_TOWNSPERSON_F, CONDITION_ALWAYS, ACTOR_STAND,
      PIXELS(248), 0, PIXELS(808), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_TOWNSPERSON_C, CONDITION_ALWAYS, ACTOR_WANDER,
      PIXELS(144), 0, PIXELS(888), FACING_NORTHWEST, TALK_FACE_PARTY, 0 },
    { SPRITE_CHEF, CONDITION_ALWAYS, ACTOR_STAND,
      PIXELS(152), 0, PIXELS(744), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_PRIEST, CONDITION_ALWAYS, ACTOR_STAND,
      PIXELS(688), 0, PIXELS(152), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_PRIEST, CONDITION_ALWAYS, ACTOR_STAND,
      PIXELS(776), 0, PIXELS(800), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_COUNTER, CONDITION_ALWAYS, ACTOR_STAND,
      PIXELS(376), 0, PIXELS(544), FACING_SOUTH - FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_COUNTER, CONDITION_ALWAYS, ACTOR_STAND,
      PIXELS(464), 0, PIXELS(544), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_COUNTER, CONDITION_ALWAYS, ACTOR_STAND,
      PIXELS(720), 0, PIXELS(512), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { SCENE_TABLE_END },
};

const struct SceneEvent gInteriorSealedEvents[] = {
    ROOM_DOOR(ROOM_FIRST_HOME),
    ROOM_DOOR(ROOM_SECOND_HOME),
    ROOM_DOOR(ROOM_THIRD_HOME),
    ROOM_DOOR(ROOM_ARMS_SHOP),
    ROOM_DOOR(ROOM_ITEM_SHOP),
    ROOM_DOOR(ROOM_INN),
    ROOM_DOOR(ROOM_TEMPLE),
    ROOM_DOOR(ROOM_EIGHTH),
    { EVENT_TALK, ACTOR_FIRST_HOME_RESIDENT_A, CONDITION_ALWAYS, MsgRunpaInteriorsFirstHomeASealed },
    { EVENT_TALK, ACTOR_FIRST_HOME_RESIDENT_B, CONDITION_ALWAYS, MsgRunpaInteriorsFirstHomeBSealed },
    { EVENT_TALK, ACTOR_SECOND_HOME_RESIDENT_A, CONDITION_ALWAYS, MsgRunpaInteriorsSecondHomeASealed },
    { EVENT_TALK, ACTOR_SECOND_HOME_RESIDENT_B, CONDITION_ALWAYS, MsgRunpaInteriorsSecondHomeBSealed },
    { EVENT_TALK, ACTOR_THIRD_HOME_RESIDENT_A, CONDITION_ALWAYS, MsgRunpaInteriorsThirdHomeASealed },
    { EVENT_TALK, ACTOR_THIRD_HOME_RESIDENT_B, CONDITION_ALWAYS, MsgRunpaInteriorsThirdHomeBSealed },
    { EVENT_TALK, ACTOR_WEAPON_MERCHANT, CONDITION_ALWAYS, EVENT_SCRIPT(WeaponMerchant_Talk) },
    { EVENT_TALK, ACTOR_ARMOR_MERCHANT, CONDITION_ALWAYS, EVENT_SCRIPT(ArmorMerchant_Talk) },
    { EVENT_TALK, ACTOR_WEAPON_COUNTER, CONDITION_ALWAYS, EVENT_SCRIPT(WeaponMerchant_Talk) },
    { EVENT_TALK, ACTOR_ARMOR_COUNTER, CONDITION_ALWAYS, EVENT_SCRIPT(ArmorMerchant_Talk) },
    { EVENT_TALK, ACTOR_ITEM_MERCHANT, CONDITION_ALWAYS, EVENT_SCRIPT(ItemMerchant_Talk) },
    { EVENT_TALK, ACTOR_ITEM_COUNTER, CONDITION_ALWAYS, EVENT_SCRIPT(ItemMerchant_Talk) },
    { EVENT_TALK, ACTOR_INNKEEPER, CONDITION_ALWAYS, EVENT_SCRIPT(Innkeeper_Talk) },
    { EVENT_TALK, ACTOR_INN_CLERK, CONDITION_ALWAYS, MsgRunpaInteriorsInnClerkSealed },
    { EVENT_TALK, ACTOR_INN_WORKER, CONDITION_ALWAYS, MsgRunpaInteriorsInnWorkerSealed },
    { EVENT_TALK, ACTOR_CHEF, CONDITION_ALWAYS, EVENT_SCRIPT(Chef_Talk) },
    { EVENT_TALK, ACTOR_TEMPLE_PRIEST, CONDITION_ALWAYS, EVENT_SCRIPT(TemplePriest_Talk) },
    { EVENT_TALK, ACTOR_TRAVELING_PRIEST, CONDITION_ALWAYS, EVENT_SCRIPT(TravelingPriest_Talk) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FIRST_HOME_RESIDENT_A, CONDITION_ALWAYS,
      MsgRunpaInteriorsFirstHomeASealedThoughts },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FIRST_HOME_RESIDENT_B, CONDITION_ALWAYS,
      MsgRunpaInteriorsFirstHomeBSealedThoughts },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SECOND_HOME_RESIDENT_A, CONDITION_ALWAYS,
      MsgRunpaInteriorsSecondHomeASealedThoughts },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SECOND_HOME_RESIDENT_B, CONDITION_ALWAYS,
      MsgRunpaInteriorsSecondHomeBSealedThoughts },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_THIRD_HOME_RESIDENT_A, CONDITION_ALWAYS,
      MsgRunpaInteriorsThirdHomeASealedThoughts },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_THIRD_HOME_RESIDENT_B, CONDITION_ALWAYS,
      MsgRunpaInteriorsThirdHomeBSealedThoughts },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEAPON_MERCHANT, CONDITION_ALWAYS,
      EVENT_SCRIPT(WeaponMerchant_ReadMind) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ARMOR_MERCHANT, CONDITION_ALWAYS,
      EVENT_SCRIPT(ArmorMerchant_ReadMind) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEAPON_COUNTER, CONDITION_ALWAYS,
      EVENT_SCRIPT(WeaponMerchant_ReadMind) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ARMOR_COUNTER, CONDITION_ALWAYS,
      EVENT_SCRIPT(ArmorMerchant_ReadMind) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ITEM_MERCHANT, CONDITION_ALWAYS,
      EVENT_SCRIPT(ItemMerchant_ReadMind) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ITEM_COUNTER, CONDITION_ALWAYS,
      EVENT_SCRIPT(ItemMerchant_ReadMind) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INNKEEPER, CONDITION_ALWAYS,
      MsgRunpaInteriorsInnkeeperSealedThoughts },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INN_CLERK, CONDITION_ALWAYS,
      MsgRunpaInteriorsInnClerkSealedThoughts },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INN_WORKER, CONDITION_ALWAYS,
      MsgRunpaInteriorsInnWorkerSealedThoughts },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_CHEF, CONDITION_ALWAYS,
      EVENT_SCRIPT(Chef_ReadMind) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TEMPLE_PRIEST, CONDITION_ALWAYS,
      MsgRunpaInteriorsTemplePriestSealedThoughts },
    { SEARCH(SEARCH_WOODEN_BOX), TRIGGER_WOODEN_BOX, FLAG_INTERIOR_WOODEN_BOX,
      FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { SEARCH(SEARCH_BARREL), TRIGGER_BARREL, FLAG_INTERIOR_BARREL, FIND_ITEM(ITEM_VIAL) },
    { SEARCH(SEARCH_OVEN), TRIGGER_FIRST_OVEN, CONDITION_ALWAYS, FIND_MESSAGE(MsgRunpaInteriorsFirstOven) },
    { SEARCH(SEARCH_OVEN), TRIGGER_SECOND_OVEN, CONDITION_ALWAYS, FIND_MESSAGE(MsgRunpaInteriorsSecondOven) },
    { SCENE_EVENTS_END },
};

const struct SceneEvent gInteriorReopenedEvents[] = {
    ROOM_DOOR(ROOM_FIRST_HOME),
    ROOM_DOOR(ROOM_SECOND_HOME),
    ROOM_DOOR(ROOM_THIRD_HOME),
    ROOM_DOOR(ROOM_ARMS_SHOP),
    ROOM_DOOR(ROOM_ITEM_SHOP),
    ROOM_DOOR(ROOM_INN),
    ROOM_DOOR(ROOM_TEMPLE),
    ROOM_DOOR(ROOM_EIGHTH),
    { EVENT_TALK, ACTOR_FIRST_HOME_RESIDENT_A, CONDITION_ALWAYS, MsgRunpaInteriorsFirstHomeAReopened },
    { EVENT_TALK, ACTOR_FIRST_HOME_RESIDENT_B, CONDITION_ALWAYS, MsgRunpaInteriorsFirstHomeBReopened },
    { EVENT_TALK, ACTOR_SECOND_HOME_RESIDENT_A, CONDITION_ALWAYS, MsgRunpaInteriorsSecondHomeAReopened },
    { EVENT_TALK, ACTOR_SECOND_HOME_RESIDENT_B, CONDITION_ALWAYS, MsgRunpaInteriorsSecondHomeBReopened },
    { EVENT_TALK, ACTOR_THIRD_HOME_RESIDENT_A, CONDITION_ALWAYS, MsgRunpaInteriorsThirdHomeAReopened },
    { EVENT_TALK, ACTOR_THIRD_HOME_RESIDENT_B, CONDITION_ALWAYS, MsgRunpaInteriorsThirdHomeBReopened },
    { EVENT_TALK, ACTOR_WEAPON_MERCHANT, CONDITION_ALWAYS, EVENT_SCRIPT(WeaponMerchant_Talk) },
    { EVENT_TALK, ACTOR_ARMOR_MERCHANT, CONDITION_ALWAYS, EVENT_SCRIPT(ArmorMerchant_Talk) },
    { EVENT_TALK, ACTOR_ITEM_MERCHANT, CONDITION_ALWAYS, EVENT_SCRIPT(ItemMerchant_Talk) },
    { EVENT_TALK, ACTOR_ITEM_COUNTER, CONDITION_ALWAYS, EVENT_SCRIPT(ItemMerchant_Talk) },
    { EVENT_TALK, ACTOR_WEAPON_COUNTER, CONDITION_ALWAYS, EVENT_SCRIPT(WeaponMerchant_Talk) },
    { EVENT_TALK, ACTOR_ARMOR_COUNTER, CONDITION_ALWAYS, EVENT_SCRIPT(ArmorMerchant_Talk) },
    { EVENT_TALK, ACTOR_INNKEEPER, CONDITION_ALWAYS, EVENT_SCRIPT(Innkeeper_Talk) },
    { EVENT_TALK, ACTOR_INN_CLERK, CONDITION_ALWAYS, EVENT_SCRIPT(InnClerk_Talk) },
    { EVENT_TALK, ACTOR_INN_WORKER, CONDITION_ALWAYS, MsgRunpaInteriorsInnWorkerReopened },
    { EVENT_TALK, ACTOR_CHEF, CONDITION_ALWAYS, MsgRunpaInteriorsChefReopened },
    { EVENT_TALK, ACTOR_TEMPLE_PRIEST, CONDITION_ALWAYS, EVENT_SCRIPT(TemplePriest_Talk) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FIRST_HOME_RESIDENT_A, CONDITION_ALWAYS,
      MsgRunpaInteriorsFirstHomeAReopenedThoughts },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FIRST_HOME_RESIDENT_B, CONDITION_ALWAYS,
      MsgRunpaInteriorsFirstHomeBReopenedThoughts },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SECOND_HOME_RESIDENT_A, CONDITION_ALWAYS,
      MsgRunpaInteriorsSecondHomeAReopenedThoughts },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SECOND_HOME_RESIDENT_B, CONDITION_ALWAYS,
      MsgRunpaInteriorsSecondHomeBReopenedThoughts },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_THIRD_HOME_RESIDENT_A, CONDITION_ALWAYS,
      MsgRunpaInteriorsThirdHomeAReopenedThoughts },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_THIRD_HOME_RESIDENT_B, CONDITION_ALWAYS,
      MsgRunpaInteriorsThirdHomeBReopenedThoughts },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEAPON_MERCHANT, CONDITION_ALWAYS,
      EVENT_SCRIPT(WeaponMerchant_ReadMind) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ARMOR_MERCHANT, CONDITION_ALWAYS,
      EVENT_SCRIPT(ArmorMerchant_ReadMind) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEAPON_COUNTER, CONDITION_ALWAYS,
      EVENT_SCRIPT(WeaponMerchant_ReadMind) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ARMOR_COUNTER, CONDITION_ALWAYS,
      EVENT_SCRIPT(ArmorMerchant_ReadMind) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ITEM_MERCHANT, CONDITION_ALWAYS,
      EVENT_SCRIPT(ItemMerchant_ReadMind) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ITEM_COUNTER, CONDITION_ALWAYS,
      EVENT_SCRIPT(ItemMerchant_ReadMind) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INNKEEPER, CONDITION_ALWAYS,
      MsgRunpaInteriorsInnkeeperReopenedThoughts },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INN_CLERK, CONDITION_ALWAYS,
      MsgRunpaInteriorsInnClerkReopenedThoughts },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INN_WORKER, CONDITION_ALWAYS,
      MsgRunpaInteriorsInnWorkerReopenedThoughts },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_CHEF, CONDITION_ALWAYS,
      MsgRunpaInteriorsChefReopenedThoughts },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TEMPLE_PRIEST, CONDITION_ALWAYS,
      MsgRunpaInteriorsTemplePriestReopenedThoughts },
    { SEARCH(SEARCH_WOODEN_BOX), TRIGGER_WOODEN_BOX, FLAG_INTERIOR_WOODEN_BOX,
      FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { SEARCH(SEARCH_BARREL), TRIGGER_BARREL, FLAG_INTERIOR_BARREL, FIND_ITEM(ITEM_VIAL) },
    { SEARCH(SEARCH_OVEN), TRIGGER_FIRST_OVEN, CONDITION_ALWAYS, FIND_MESSAGE(MsgRunpaInteriorsFirstOven) },
    { SEARCH(SEARCH_OVEN), TRIGGER_SECOND_OVEN, CONDITION_ALWAYS, FIND_MESSAGE(MsgRunpaInteriorsSecondOven) },
    { SCENE_EVENTS_END },
};

const struct SceneEvent gSuharaGateHouseEvents[] = {
    ROOM_DOOR(ROOM_SUHARA_GATE_HOUSE),
    { EVENT_TALK, ACTOR_TRAVELING_PRIEST, CONDITION_ALWAYS, EVENT_SCRIPT(TravelingPriest_Talk) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TRAVELING_PRIEST, CONDITION_ALWAYS,
      MsgRunpaInteriorsTravelingPriestThoughts },
    { SCENE_EVENTS_END },
};

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

const struct SceneEntrance *Scene_GetEntrances(void)
{
    return gInteriorEntrances;
}

const struct SceneRegion *Scene_GetRegions(void)
{
    return NULL;
}

const u32 *Scene_GetExits(void)
{
    return gInteriorExits;
}

const struct ScenePlacement *Scene_GetPlacements(void)
{
    return gInteriorPlacements;
}

const struct SceneEvent *Scene_GetEvents(void)
{
    if (gGameState.entrance == ROOM_SUHARA_GATE_HOUSE) {
        return gSuharaGateHouseEvents;
    }
    if (Engine_GameFlagIsSet(FLAG_LUNPA_TRADE_REOPENED) != 0) {
        return gInteriorReopenedEvents;
    }
    return gInteriorSealedEvents;
}

void Innkeeper_Talk(void)
{
    struct FieldActor *leader;

    leader = Object_GetById(ACTOR_PARTY_LEADER);
    if (FACING_IS_NORTH(leader->facing)) {
        if (Engine_GameFlagIsSet(FLAG_LUNPA_TRADE_REOPENED) != 0) {
            Engine_InnOpen(INN_LUNPA, ACTOR_INNKEEPER);
            return;
        }
    }
    Engine_EventBegin();
    if (Engine_GameFlagIsSet(FLAG_LUNPA_TRADE_REOPENED) != 0) {
        Engine_EventSetMessage((s32)MsgRunpaInnkeeperOffersRoom);
        Engine_EventAskYesNo(ACTOR_INNKEEPER, 0);
    } else {
        Engine_EventSetMessage((s32)MsgRunpaInnkeeperOffersOwnHome);
        Engine_EventAskYesNo(ACTOR_INNKEEPER, 0);
    }
    Engine_EventEnd();
}

void Chef_Talk(void)
{
    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgRunpaChefOffersStory);
    Engine_EventAskYesNo(ACTOR_CHEF, 0);
    Engine_GameFlagSet(FLAG_LUNPA_HEARD_OF_PRISONER);
    Engine_EventEnd();
}

void Chef_ReadMind(void)
{
    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgRunpaChefThinksOfPrisoner);
    Engine_EventShowMessage(ACTOR_CHEF, 0);
    Engine_GameFlagSet(FLAG_LUNPA_HEARD_OF_PRISONER);
    Engine_EventEnd();
}

void InnClerk_Talk(void)
{
    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgRunpaInnClerkAsksAboutGuest);
    Engine_EventAskYesNo(ACTOR_INN_CLERK, 0);
    Engine_EventEnd();
}

void TemplePriest_Talk(void)
{
    struct FieldActor *leader;

    leader = Object_GetById(ACTOR_PARTY_LEADER);
    if (FACING_IS_NORTH(leader->facing)) {
        Engine_SanctumOpen(ACTOR_TEMPLE_PRIEST);
    } else if (Engine_GameFlagIsSet(FLAG_LUNPA_TRADE_REOPENED) != 0) {
        Engine_EventBegin();
        Engine_EventSetMessage((s32)MsgRunpaTemplePriestReopened);
        Engine_EventShowMessage(ACTOR_TEMPLE_PRIEST, 0);
        Engine_EventEnd();
    } else {
        Engine_EventBegin();
        Engine_EventSetMessage((s32)MsgRunpaTemplePriestSealed);
        Engine_EventShowMessage(ACTOR_TEMPLE_PRIEST, 0);
        Engine_EventEnd();
    }
}

void ItemMerchant_ReadMind(void)
{
    if (Engine_GameFlagIsSet(FLAG_LUNPA_TRADE_REOPENED) != 0) {
        Engine_EventBegin();
        Engine_EventSetMessage((s32)MsgRunpaIsntWeaponsVendors);
        Engine_EventShowMessage(ACTOR_ITEM_MERCHANT, 0);
        Engine_EventEnd();
    } else {
        Engine_EventBegin();
        Engine_EventSetMessage((s32)MsgRunpaItemMerchantSealedThoughts);
        Engine_EventShowMessage(ACTOR_ITEM_MERCHANT, 0);
        Engine_EventEnd();
    }
}

void ItemMerchant_Talk(void)
{
    struct FieldActor *leader;

    leader = Object_GetById(ACTOR_PARTY_LEADER);
    if (FACING_IS_NORTH(leader->facing)) {
        Engine_ShopOpen(SHOP_LUNPA_ITEMS, ACTOR_ITEM_MERCHANT);
    } else if (Engine_GameFlagIsSet(FLAG_LUNPA_TRADE_REOPENED) != 0) {
        Engine_EventBegin();
        Engine_EventSetMessage((s32)MsgRunpaItemMerchantReopened);
        Engine_EventShowMessage(ACTOR_ITEM_MERCHANT, 0);
        Engine_EventEnd();
    } else {
        Engine_EventBegin();
        Engine_EventSetMessage((s32)MsgRunpaItemMerchantSealed);
        Engine_EventShowMessage(ACTOR_ITEM_MERCHANT, 0);
        Engine_EventEnd();
    }
}

void WeaponMerchant_ReadMind(void)
{
    if (Engine_GameFlagIsSet(FLAG_LUNPA_TRADE_REOPENED) != 0) {
        Engine_EventSetMessage((s32)MsgRunpaWeaponMerchantReopenedThoughts);
        Engine_EventShowMessage(ACTOR_WEAPON_MERCHANT, 0);
    } else {
        Engine_EventSetMessage((s32)MsgRunpaWeaponMerchantSealedThoughts);
        Engine_EventShowMessage(ACTOR_WEAPON_MERCHANT, 0);
    }
}

void WeaponMerchant_Talk(void)
{
    struct FieldActor *leader;
    s32 facing;

    leader = Object_GetById(ACTOR_PARTY_LEADER);
    facing = leader->facing;
    if (Engine_GameFlagIsSet(FLAG_LUNPA_TRADE_REOPENED) != 0) {
        if (FACING_IS_NORTH(facing)) {
            Engine_ShopOpen(SHOP_LUNPA_WEAPONS, ACTOR_WEAPON_MERCHANT);
        } else {
            Engine_EventBegin();
            Engine_EventSetMessage((s32)MsgRunpaWeaponMerchantReopened);
            Engine_EventShowMessage(ACTOR_WEAPON_MERCHANT, 0);
            Engine_EventEnd();
        }
    } else {
        Engine_EventSetMessage((s32)MsgRunpaWeaponMerchantSealed);
        Engine_EventShowMessage(ACTOR_WEAPON_MERCHANT, 0);
    }
}

void ArmorMerchant_ReadMind(void)
{
    if (Engine_GameFlagIsSet(FLAG_LUNPA_TRADE_REOPENED) != 0) {
        Engine_EventSetMessage((s32)MsgRunpaArmorMerchantReopenedThoughts);
        Engine_EventShowMessage(ACTOR_ARMOR_MERCHANT, 0);
    } else {
        Engine_EventSetMessage((s32)MsgRunpaArmorMerchantSealedThoughts);
        Engine_EventShowMessage(ACTOR_ARMOR_MERCHANT, 0);
    }
}

void ArmorMerchant_Talk(void)
{
    struct FieldActor *leader;
    s32 facing;

    leader = Object_GetById(ACTOR_PARTY_LEADER);
    facing = leader->facing;
    if (Engine_GameFlagIsSet(FLAG_LUNPA_TRADE_REOPENED) != 0) {
        if (FACING_IS_NORTH(facing)) {
            Engine_ShopOpen(SHOP_LUNPA_ARMOR, ACTOR_ARMOR_MERCHANT);
        } else {
            Engine_EventBegin();
            Engine_EventSetMessage((s32)MsgRunpaArmorMerchantReopened);
            Engine_EventShowMessage(ACTOR_ARMOR_MERCHANT, 0);
            Engine_EventEnd();
        }
    } else {
        Engine_EventSetMessage((s32)MsgRunpaArmorMerchantSealed);
        Engine_EventShowMessage(ACTOR_ARMOR_MERCHANT, 0);
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
        Engine_GameFlagClear(FLAG_SHOW_LOCATION_NAME);
        gGameState.saved_scene = (s32)&SceneId_RunpaSuhara;
        gGameState.saved_entrance = entrance;
    }
    Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_WEAPON_COUNTER), 0);
    Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_ARMOR_COUNTER), 0);
    Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_ITEM_COUNTER), 0);
    return 0;
}

void TravelingPriest_Talk(void)
{
    struct FieldActor *leader;

    leader = Object_GetById(ACTOR_PARTY_LEADER);
    if (FACING_IS_NORTH(leader->facing)) {
        Engine_SanctumOpen(ACTOR_TEMPLE_PRIEST);
    } else {
        Engine_EventSetMessage((s32)MsgRunpaTravelingPriest);
        Engine_EventShowMessage(ACTOR_TRAVELING_PRIEST, 0);
    }
}
