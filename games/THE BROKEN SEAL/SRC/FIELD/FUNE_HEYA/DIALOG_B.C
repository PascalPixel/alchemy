#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "HEYA.H"

#include "STAGED_ACTOR.H"
extern u8 MsgFuneAnotherMonsterIsntFirstClass[];
extern u8 MsgFuneBoatsRockingMuchImCertain[];
extern u8 MsgFuneDontCareTakesJustHurry[];
extern u8 MsgFuneGoodShipHasArrivedSafely[];
extern u8 MsgFuneHadIdeaThereWereMany[];
extern u8 MsgFuneHateArguing[];
extern u8 MsgFuneHowManyMonstersOutThere[];
extern u8 MsgFuneIfThoseMonstersComeBack[];
extern u8 MsgFuneImSpreadingGoodwillWhereverTravel[];
extern u8 MsgFuneMonstersEverywhereImStuckRowing[];
extern u8 MsgFuneShipStartingListIfWe[];
extern u8 MsgFuneShipsCrewReadyAnything[];
extern u8 MsgFuneShipsCrewReadyForAnything[];
extern u8 MsgFuneThingHasKajaHisMen[];
extern u8 MsgFuneWereSurroundedByMonstersStill[];

struct SceneActor {
    u8 pad00[99];
    u8 mode;
};

struct EffectRecord {
    u8 pad00[6];
    u16 angle;
    u8 pad08[83];
    u8 state;
    u8 pad5c[6];
    u8 active;
};
extern u32 FuneHeya_TurnSteps[];
s32 Object_GetByIdFar();
s32 Object_CheckMovementCollision();
s32 FuneHeya_FindFirstSetFlag();

struct FieldActor *FindActorNearPosition(s32 x, s32 y);

/* Actor placement check for resource_3b1. */

/* Bucket offsets, packed as {s16 hi; s16 lo} per entry. */

/*
 * Offset obj+10 and obj+18 by the bucket's packed hi/lo pair, test the
 * candidate point, and on success pack {x << 16, obj+12, z << 16} into a
 * stack struct for a second check.  Returns 1 only if both checks pass.  The
 * owner includes its one pool word, the bucket table base.  Callees are named
 * by the address their call site computes, not by a runtime address.
 */
s32 SceneActor_CheckBucketOffsetPoint(s32 bucket)
{
    u8 *obj = Object_GetByIdFar(0);
    u32 ofs = FuneHeya_TurnSteps[bucket];
    s32 x = *(s16 *)(obj + 10) + ((s32)ofs >> 16);
    s32 z = *(s16 *)(obj + 18) + (s32)(s16)ofs;

    if (FindActorNearPosition(x, z) != 0) {
        return 0;
    }

    {
        s32 point[3];
        point[0] = x << 16;
        point[1] = *(s32 *)(obj + 12);
        point[2] = z << 16;

        if (Object_CheckMovementCollision(obj, point) != 0) {
            return 0;
        }
    }

    return 1;
}

/* Scene state helper for overlay resource_3b1. */

s32 SceneState_ApplyLevelFromFlags(void)
{
    s32 ret = 0;

    if (GameFlag_IsSet(0x92b) != 0) {
        ret = 3;
    } else if (GameFlag_IsSet(0x92a) != 0) {
        ret = 2;
    } else if (GameFlag_IsSet(0x929) != 0) {
        ret = 1;
    }

    return FuneHeya_FindFirstSetFlag(ret, 1);
}

void SceneDialogue_ShowLine1ECETo1ED0(void)
{
    Event_Begin();
    if (GameFlag_IsSet(0x92c)) Event_SetMessage((s32)MsgFuneThingHasKajaHisMen);
    else if (GameFlag_IsSet(0x935)) Event_SetMessage((s32)MsgFuneMonstersEverywhereImStuckRowing);
    else Event_SetMessage((s32)MsgFuneShipStartingListIfWe);
    Event_ShowMessage(0x12, 0); Event_End();
}

void SceneDialogue_RunActor19TwoFlagLineA(void)
{
    Event_Begin();
    if (GameFlag_IsSet(0x92d)) Event_SetMessage((s32)MsgFuneThingHasKajaHisMen);
    else if (GameFlag_IsSet(0x936)) Event_SetMessage((s32)MsgFuneMonstersEverywhereImStuckRowing);
    else Event_SetMessage((s32)MsgFuneShipStartingListIfWe);
    Event_ShowMessage(0x13, 0); Event_End();
}

void SceneDialogue_RunActor20TwoFlagLine(void)
{
    Event_Begin();
    if (GameFlag_IsSet(0x92e)) Event_SetMessage((s32)MsgFuneThingHasKajaHisMen);
    else if (GameFlag_IsSet(0x937)) Event_SetMessage((s32)MsgFuneMonstersEverywhereImStuckRowing);
    else Event_SetMessage((s32)MsgFuneShipStartingListIfWe);
    Event_ShowMessage(0x14, 0); Event_End();
}

void SceneDialogue_ShowLine1ED1Or1ED2(void)
{
    Event_Begin();
    if (GameFlag_IsSet(0x92f)) Event_SetMessage((s32)MsgFuneHowManyMonstersOutThere);
    else Event_SetMessage((s32)MsgFuneAnotherMonsterIsntFirstClass);
    Event_ShowMessage(21, 0); Event_End();
}

void SceneDialogue_RunActor22TwoFlagLine(void)
{
    Event_Begin();
    if (GameFlag_IsSet(0x930)) Event_SetMessage((s32)MsgFuneThingHasKajaHisMen);
    else if (GameFlag_IsSet(0x939)) Event_SetMessage((s32)MsgFuneMonstersEverywhereImStuckRowing);
    else Event_SetMessage((s32)MsgFuneShipStartingListIfWe);
    Event_ShowMessage(22, 0); Event_End();
}

void SceneDialogue_RunActor23BranchedDialogue(void)
{
    Event_Begin();
    if (GameFlag_IsSet(0x931)) Event_SetMessage((s32)MsgFuneThingHasKajaHisMen);
    else if (GameFlag_IsSet(0x93a)) Event_SetMessage((s32)MsgFuneMonstersEverywhereImStuckRowing);
    else Event_SetMessage((s32)MsgFuneShipStartingListIfWe);
    Event_ShowMessage(23, 0); Event_End();
}

void SceneDialogue_RunActor24BranchedDialogue(void)
{
    Event_Begin();
    if (GameFlag_IsSet(0x932)) Event_SetMessage((s32)MsgFuneThingHasKajaHisMen);
    else if (GameFlag_IsSet(0x93b)) Event_SetMessage((s32)MsgFuneMonstersEverywhereImStuckRowing);
    else Event_SetMessage((s32)MsgFuneShipStartingListIfWe);
    Event_ShowMessage(24, 0); Event_End();
}

void SceneDialogue_RunActor25FlaggedLine(void)
{
    Event_Begin();
    if (GameFlag_IsSet(0x933)) Event_SetMessage((s32)MsgFuneHowManyMonstersOutThere);
    else Event_SetMessage((s32)MsgFuneAnotherMonsterIsntFirstClass);
    Event_ShowMessage(25, 0); Event_End();
}

void SceneDialogue_RunActor18TwoFlagLine(void)
{
    Event_Begin();
    if (GameFlag_IsSet(0x92c)) Event_SetMessage((s32)MsgFuneDontCareTakesJustHurry);
    else if (GameFlag_IsSet(0x935)) Event_SetMessage((s32)MsgFuneIfThoseMonstersComeBack);
    else Event_SetMessage((s32)MsgFuneBoatsRockingMuchImCertain);
    Event_ShowMessage(18, 0); Event_End();
}

void SceneDialogue_RunActor19TwoFlagLineB(void)
{
    Event_Begin();
    if (GameFlag_IsSet(0x92d)) Event_SetMessage((s32)MsgFuneDontCareTakesJustHurry);
    else if (GameFlag_IsSet(0x936)) Event_SetMessage((s32)MsgFuneIfThoseMonstersComeBack);
    else Event_SetMessage((s32)MsgFuneBoatsRockingMuchImCertain);
    Event_ShowMessage(19, 0); Event_End();
}

void SceneDialogue_ShowLine1EDBTo1EDDActor20(void)
{
    Event_Begin();
    if (GameFlag_IsSet(0x92e)) Event_SetMessage((s32)MsgFuneDontCareTakesJustHurry);
    else if (GameFlag_IsSet(0x937)) Event_SetMessage((s32)MsgFuneIfThoseMonstersComeBack);
    else Event_SetMessage((s32)MsgFuneBoatsRockingMuchImCertain);
    Event_ShowMessage(20, 0); Event_End();
}

void SceneDialogue_RunActor21FlaggedLine(void)
{
    Event_Begin();
    if (GameFlag_IsSet(0x92f)) Event_SetMessage((s32)MsgFuneHadIdeaThereWereMany);
    else Event_SetMessage((s32)MsgFuneWereSurroundedByMonstersStill);
    Event_ShowMessage(21, 0); Event_End();
}

void SceneDialogue_RunActor22BranchedDialogue(void)
{
    Event_Begin();
    if (GameFlag_IsSet(0x930)) Event_SetMessage((s32)MsgFuneDontCareTakesJustHurry);
    else if (GameFlag_IsSet(0x939)) Event_SetMessage((s32)MsgFuneIfThoseMonstersComeBack);
    else Event_SetMessage((s32)MsgFuneBoatsRockingMuchImCertain);
    Event_ShowMessage(22, 0); Event_End();
}

void SceneDialogue_ShowLine1EDBTo1EDDActor23(void)
{
    Event_Begin();
    if (GameFlag_IsSet(0x931)) Event_SetMessage((s32)MsgFuneDontCareTakesJustHurry);
    else if (GameFlag_IsSet(0x93a)) Event_SetMessage((s32)MsgFuneIfThoseMonstersComeBack);
    else Event_SetMessage((s32)MsgFuneBoatsRockingMuchImCertain);
    Event_ShowMessage(23, 0); Event_End();
}

void SceneDialogue_ShowLine1EDBTo1EDDActor24(void)
{
    Event_Begin();
    if (GameFlag_IsSet(0x932)) Event_SetMessage((s32)MsgFuneDontCareTakesJustHurry);
    else if (GameFlag_IsSet(0x93b)) Event_SetMessage((s32)MsgFuneIfThoseMonstersComeBack);
    else Event_SetMessage((s32)MsgFuneBoatsRockingMuchImCertain);
    Event_ShowMessage(24, 0); Event_End();
}

void SceneDialogue_ShowLine1EDEOr1EDF(void)
{
    Event_Begin();
    if (GameFlag_IsSet(0x933)) Event_SetMessage((s32)MsgFuneHadIdeaThereWereMany);
    else Event_SetMessage((s32)MsgFuneWereSurroundedByMonstersStill);
    Event_ShowMessage(25, 0); Event_End();
}

void FieldScene_RunPrimarySequence(s32 a0, s32 a1, s32 a2)
{
    u32 i;
    s32 record;

    Event_Begin();
    Event_SetMessage(a1);
    Event_OpenMessage(a0, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        FieldScene_RunStepThen10(a0);
        Actor_SetAnimation(a0, 2);
        record = Value1(Object_GetByIdFar, 0);
        if (record != 0) {
            Actor_SetDestination(a0, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        Actor_WaitForMove(a0);
        Actor_SetPosition(a0, 0, 0);
        GameFlag_Set(0x300);
        GameFlag_Set(a2);
    } else {
        bump_step(1);
        FieldScene_RunStepThen10(a0);
    }
    Event_End();
}

void FieldScene_RunScene3b1SequenceC(void)
{
    struct FieldActor *leader;

    leader = (struct FieldActor *)Object_GetByIdFar(0);
    if ((u16)(leader->facing - 0x2000) > 0xc000) {
        if (GameFlag_IsSet(0x928) != 0 && GameFlag_IsSet(0x93e) == 0) {
            Sanctum_Open(17);
        } else {
            Sanctum_Open(15);
        }
    } else {
        Event_Begin();
        if (GameFlag_IsSet(0x93e) != 0) {
            Event_SetMessage((s32)MsgFuneGoodShipHasArrivedSafely);
        } else if (GameFlag_IsSet(0x8a0) != 0) {
            Event_SetMessage((s32)MsgFuneHateArguing);
        } else if (GameFlag_IsSet(0x928) != 0) {
            Event_SetMessage((s32)MsgFuneShipsCrewReadyAnything);
        } else if (GameFlag_IsSet(0x925) != 0) {
            Event_SetMessage((s32)MsgFuneShipsCrewReadyForAnything);
        } else {
            Event_SetMessage((s32)MsgFuneImSpreadingGoodwillWhereverTravel);
        }
        if (GameFlag_IsSet(0x928) != 0 && GameFlag_IsSet(0x93e) == 0) {
            Event_ShowMessage(17, 0);
        } else {
            Event_ShowMessage(15, 0);
        }
        Event_End();
    }
}
