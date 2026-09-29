#include "TIMED_EVENTS.H"
extern u8 MsgHaidiaDidYouHearAboutDora[];
extern u8 MsgHaidiaLetsScareSukuretasVisitors[];
extern u8 MsgHaidiaSukuretaCameToStudyMt[];
extern u8 MsgHaidiaTheCulpritsHadStrangePowers[];
extern u8 MsgHaidiaThisIsMyFarewellGift[];
extern u8 MsgHaidiaYouMustSaveJasmine[];

s32 OverlayObject_UpdateFacingTowardTarget(struct FacingObject *obj)
{
    s32 delta;
    u16 old;
    s32 angle;
    struct FacingObject *target;

    target = obj->facing_target;
    if (target != NULL) {
        obj->facing_flags = (u8)(0xFE & obj->facing_flags);
        angle = (u16)ArcTan2(target->position_z - obj->position_z, target->position_x - obj->position_x);
        old = obj->facing;
        delta = (s16)(angle - old);
        if (delta != 0) {
            if (delta > 0x1000) {
                delta = 0x1000;
            }
            /* The loader relocates the stored pool word to -0x1000. */
            if (delta < -0x1000) {
                delta = -0x1000;
            }
            obj->facing = (u16)(old + delta);
        }
    }
    return 1;
}

s32 AdvancePositionScaleAndVelocity(ScaledMotion *motion)
{
    motion->x += motion->velocity_x << 8;
    motion->y += motion->velocity_y << 8;
    motion->scale_x += 0x666;
    motion->scale_y += 0x666;
    motion->velocity_x += 5;
    motion->velocity_y -= 1;
    return 0;
}

/* The eight-byte owner includes its one pool word. */
void *SceneData_GetTable9478(void)
{
    return gValeHouseEntrances;
}

/* A four-byte leaf that returns zero. */
int SceneData_ReturnZero(void)
{
    return 0;
}

/* The 36-byte owner includes its three pool words. */
void *SceneData_SelectTable9568ByFlag(void)
{
    if (GameFlag_IsSet(0x834) != 0)
        return gValeHouseLateExits;
    return gValeHouseExits;
}

void *SceneData_SelectFlaggedTable(void)
{
    void *tbl;

    if (GameFlag_IsSet(0x87a)) {
        tbl = gValeHouseReturnPlacements;
    } else if (GameFlag_IsSet(FLAG_PARTY_LEFT_VALE)) {
        tbl = gValeHousePlacementsAfterLeaving;
    } else {
        tbl = gValeHousePlacements;
    }
    SceneEvents_UpdateInView(tbl);
    return tbl;
}

/* The 80-byte owner includes its seven pool words. */
void *SceneData_SelectTable9c00ByFlags(void)
{
    if (GameFlag_IsSet(0x834) != 0)
        return gValeHouseLateEvents;
    if (GameFlag_IsSet(0x87a) != 0)
        return gValeHouseReturnEvents;
    if (GameFlag_IsSet(FLAG_PARTY_LEFT_VALE) != 0)
        return gValeHouseEventsAfterLeaving;
    return gValeHouseEvents;
}

/* The 44-byte actor-15 scene owner includes its one pool word. */
void Villager_AskWhySukuretaCame(void)
{
    Event_Begin();
    Event_SetMessage((s32)MsgHaidiaSukuretaCameToStudyMt);
    Actor_FaceEachOther(ACTOR_PARTY_LEADER, 15, 6);
    Event_AskYesNo(15, 0);
    Event_End();
}

/* The 44-byte actor-19 scene owner includes its one pool word. */
void Villager_PlanToScareVisitors(void)
{
    Event_Begin();
    Event_SetMessage((s32)MsgHaidiaLetsScareSukuretasVisitors);
    Actor_FaceEachOther(ACTOR_PARTY_LEADER, 19, 6);
    Event_AskYesNo(19, 0);
    Event_End();
}

void Scene_GiveFarewellHerb(void)
{
    s32 callback;
    s32 base5_11a4;

    Event_Begin();
    if (GameFlag_IsSet(FLAG_GOT_FAREWELL_HERB) != 0) {
        Event_SetMessage((s32)MsgHaidiaYouMustSaveJasmine);
        Event_ShowMessage(20, 0);
        callback = (s32)gValeFaceTargetScript;
        Call3(Object_SetTargetAndCallback, 20, 0x10000, callback);
    } else {
        base5_11a4 = (s32)MsgHaidiaThisIsMyFarewellGift;
        Event_SetMessage(base5_11a4);
        Event_ShowMessageAndWait(20, 0, 20);
        Message_ShowCentered((base5_11a4 + 1), 1);
        Party_GiveItem(ITEM_HERB, 0);
        GameFlag_Set(FLAG_GOT_FAREWELL_HERB);
    }
    Event_End();
}

/* The 32-byte actor-16 dialogue owner includes its one pool word. */
void Villager_AskAboutStrangePowers(void)
{
    Event_Begin();
    Event_SetMessage((s32)MsgHaidiaTheCulpritsHadStrangePowers);
    Event_AskYesNo(16, 0);
    Event_End();
}

/* The 32-byte actor-10 dialogue owner includes its one pool word. */
void Villager_AskAboutDora(void)
{
    Event_Begin();
    Event_SetMessage((s32)MsgHaidiaDidYouHearAboutDora);
    Event_AskYesNo(10, 0);
    Event_End();
}
