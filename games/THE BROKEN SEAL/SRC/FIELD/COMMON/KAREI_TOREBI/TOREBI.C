#include "EDITION.H"
#include "TBS_EDITION.H"
#include "KAREI.H"
#include "text/MSG_IDS.H"
TEXT_MESSAGE_ENUM(MsgYourCoins);
TEXT_MESSAGE_ENUM(MsgCoins);
#include "SCENE_IDS.H"
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "CALL.H"

extern const struct SceneEntrance gKareiTorebiEntrances1[];
extern const struct SceneEntrance gKareiTorebiEntrances2[];
extern const struct SceneEntrance gKareiTorebiEntrances3[];
extern const struct SceneEntrance gKareiTorebiEntrancesOther[];

extern const struct ScenePlacement gKareiTorebiPlacements1[];
extern const struct ScenePlacement gKareiTorebiPlacements1Flag93e[];
extern const struct ScenePlacement gKareiTorebiPlacements2[];
extern const struct ScenePlacement gKareiTorebiPlacements2Flag93e[];
extern const struct ScenePlacement gKareiTorebiPlacements2Flag950[];
extern const struct ScenePlacement gKareiTorebiPlacements3[];
extern const struct ScenePlacement gKareiTorebiPlacements3Flag950[];
extern const struct ScenePlacement gKareiTorebiPlacementsOther[];

extern const struct SceneEvent gKareiTorebiEvents1[];
extern const struct SceneEvent gKareiTorebiEvents1Flag93e[];
extern const struct SceneEvent gKareiTorebiEvents2[];
extern const struct SceneEvent gKareiTorebiEvents2Flag93e[];
extern const struct SceneEvent gKareiTorebiEvents2Flag950[];
extern const struct SceneEvent gKareiTorebiEvents3[];
extern const struct SceneEvent gKareiTorebiEvents3Flag950[];
extern const struct SceneEvent gKareiTorebiEventsOther[];

extern u8 MsgKareiFinishedOutHereBoardShip[];
extern u8 MsgKareiGoingTakeShip[];
extern u8 MsgKareiPassMessageDaughter[];
extern u8 MsgKareiWantGoToTolbi[];

extern u8 MsgKareiWantGoAshore[];
extern u8 MsgKareiTicketsPlease[];

extern u8 MsgKareiAfterHandingOverTicket[];
extern u8 MsgKareiBoardWaitSetSail[];
extern u8 MsgKareiHandInTicketPlank[];
extern u8 MsgKareiHaveTicketsGiveHim[];
extern u8 MsgKareiHearScaryTrip[];
extern u8 MsgKareiLookingForTickets[];
extern u8 MsgKareiSilkRoadBlocked[];
extern u8 MsgKareiThanksMessageParents[];
extern u8 MsgKareiTouristsLookUpset[];

extern const u16 KareiTorebi_LeaveCells1[];
extern const u16 KareiTorebi_LeaveCells3[];

void FieldScene_ConfigureFlaggedActors(void);

void FieldScene_PlaceSlots14And15(void);

union GameStateRows {
    u8 bytes[512][2];
    s16 halves[512][1];
    s32 words[256];
};

/* The sprite's attribute bytes as the scene scripts write them. */
struct SpriteBytes {
    u8 unknown_00[9];
    u8 priority;
    u8 unknown_0a[11];
    u8 second_priority;
    u8 unknown_16[8];
    u16 rotation;
    u8 unknown_20[6];
    u8 flags;
};

extern u8 KareiTorebi_ActorSixteenScript[];
extern u8 MsgKareiIncredibleOcean[];
extern u8 MsgKareiReturnedTicketCost[];

void SceneState_ApplyValues14And0And5(void)
{
    BattleFx_RunPageEffectForSlot(0xE, 0, 5);
}

/* Where the party appears in each of the road's three scenes. */
const struct SceneEntrance *Scene_GetEntrances(void)
{
    s16 scene = gGameState.scene;

    if (scene == (s32)&SceneId_KareiTorebi1) {
        return gKareiTorebiEntrances1;
    }
    if (scene == (s32)&SceneId_KareiTorebi3) {
        return gKareiTorebiEntrances3;
    }
    if (scene == (s32)&SceneId_KareiTorebi2) {
        return gKareiTorebiEntrances2;
    }
    return gKareiTorebiEntrancesOther;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

u8 *SceneData_GetTable98a0(void)
{
    return KareiTorebi_SceneTable;
}

/* The actors placed in each scene, which change as flags 0x93e and 0x950
   are set. */
const struct ScenePlacement *Scene_GetPlacements(void)
{
    s16 room = gGameState.scene;

    if (room == (s32)&SceneId_KareiTorebi1) {
        if (GameFlag_IsSet(0x93e) != 0) {
            return gKareiTorebiPlacements1Flag93e;
        }
        return gKareiTorebiPlacements1;
    }

    if (room == (s32)&SceneId_KareiTorebi3) {
        if (GameFlag_IsSet(0x950) != 0) {
            return gKareiTorebiPlacements3Flag950;
        }
        return gKareiTorebiPlacements3;
    }

    if (room == (s32)&SceneId_KareiTorebi2) {
        if (GameFlag_IsSet(0x950) != 0) {
            return gKareiTorebiPlacements2Flag950;
        }
        if (GameFlag_IsSet(0x93e) != 0) {
            return gKareiTorebiPlacements2Flag93e;
        }
        return gKareiTorebiPlacements2;
    }

    return gKareiTorebiPlacementsOther;
}

void FieldScene_RunScene3ae_02000144(void)
{
    u32 i;
    s32 record;

    Engine_EventBegin();
    GameFlag_Set(0x8aa);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x188, 0x128);
    Actor_SetSpeed(8, 0x13333, 0x9999);
    Actor_WalkToAndWait(8, 0x198, 0x128);
    Actor_FaceDirection(8, 0x8000, 0);
    Engine_EventWait(20);
    Engine_EventEnd();
}

/* What each scene answers, which changes as flags 0x93e and 0x950 are set. */
const struct SceneEvent *Scene_GetEvents(void)
{
    s16 scene = gGameState.scene;

    if (scene == (s32)&SceneId_KareiTorebi1) {
        if (GameFlag_IsSet(0x93e) != 0) {
            return gKareiTorebiEvents1Flag93e;
        }
        return gKareiTorebiEvents1;
    }

    if (scene == (s32)&SceneId_KareiTorebi3) {
        if (GameFlag_IsSet(0x950) != 0) {
            return gKareiTorebiEvents3Flag950;
        }
        return gKareiTorebiEvents3;
    }

    if (scene == (s32)&SceneId_KareiTorebi2) {
        if (GameFlag_IsSet(0x950) != 0) {
            return gKareiTorebiEvents2Flag950;
        }
        if (GameFlag_IsSet(0x93e) != 0) {
            return gKareiTorebiEvents2Flag93e;
        }
        return gKareiTorebiEvents2;
    }

    return gKareiTorebiEventsOther;
}

void KareiTorebi_AskGoToTolbi(void)
{
    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgKareiWantGoToTolbi);
    Event_AskYesNo(8, 0);
    Engine_EventEnd();
}

void FieldScene_RunScene3ae_02000260(void)
{
    u32 i;
    s32 record;

    Engine_EventBegin();
    if (GameFlag_IsSet(0x8a6) == 0) {
        Engine_EventSetMessage((s32)MsgKareiGoingTakeShip);
        Event_OpenMessage(11, 0);
        if (Engine_EventChooseYesNo(0, 0) == 0) {
            Event_ShowMessage(11, 0);
            GameFlag_Set(0x8a6);
            goto L_020002c2;
        }
        bump_step(1);
        Event_ShowMessage(11, 0);
    } else {
        Engine_EventSetMessage((s32)MsgKareiPassMessageDaughter);
        Event_ShowMessage(11, 0);
    }
    L_020002c2:;
    Engine_EventEnd();
}

void FieldScene_RunScene3ae_020002dc(void)
{
    u32 i;
    s32 record;

#if !defined(TBS_EDITION_ES) && !defined(TBS_EDITION_FR) && !defined(TBS_EDITION_IT)
    ((void)Object_GetById(0));
    Engine_EventBegin();
#endif
    if (GameFlag_IsSet(0x8a7) != 0) {
        if (GameFlag_IsSet(0x8a9) != 0) {
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
            Engine_EventBegin();
#endif
            Engine_EventSetMessage((s32)MsgKareiFinishedOutHereBoardShip);
            Event_OpenMessage(12, 0);
            Actor_FaceDirection(12, 0x4000, 0);
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
            Engine_EventEnd();
#endif
        }
    }
}

/* The ticket taker at the gangway, actor 12. Before boarding he answers only
 * a leader facing him from the pier: with the ticket (item 235) in the party
 * he takes it, steps aside and lets the party aboard. Once aboard he offers
 * to let the party ashore, and after that asks them to come back aboard. */
void SceneDialogue_RunActor12Event(void)
{
    s32 owner;
    s32 slot;
    u8 *record;
    s16 angle;

    record = (u8 *)Actor_Get(ACTOR_PARTY_LEADER);
    angle = (u16)((*(u16 *)(record + 6) + 0x2000) & ~0x3fff);
    Engine_EventBegin();
    if (GameFlag_IsSet(0x8a7) != 0) {
        if (GameFlag_IsSet(0x8a9) != 0) {
            Engine_EventSetMessage((s32)MsgKareiFinishedOutHereBoardShip);
            Event_OpenMessage(12, 0);
            goto done;
        }
        Engine_EventSetMessage((s32)MsgKareiWantGoAshore);
        Event_OpenMessage(12, 0);
        if (Engine_EventChooseYesNo(0, 0) == 0) {
            Engine_EventWait(10);
            Engine_EventSetMessage((s32)MsgKareiWantGoAshore + 1);
            Event_ShowMessage(12, 0);
            Actor_WalkToAndWait(12, 88, 0x508);
            Actor_FaceDirection(12, 0x4000, 0);
            Engine_EventWait(20);
            GameFlag_Set(0x8a9);
            goto done;
        }
        Engine_EventSetMessage((s32)MsgKareiWantGoAshore + 2);
        Event_ShowMessage(12, 0);
    } else {
        if ((u16)angle != 0x8000) {
            return;
        }
        Engine_EventSetMessage((s32)MsgKareiTicketsPlease);
        Event_ShowMessage(12, 0);
        if (GameFlag_IsSet(0x8a5) != 0) {
            owner = PartyInventory_FindOwner(235);
            slot = Inventory_Find(owner, 235);
            Engine_ActorSetAnimationAndWait(12, 3);
            Actor_WalkToAndWait(12, 88, 0x508);
            Actor_FaceDirection(12, 0x4000, 0);
            bump_step(1);
            Event_ShowMessage(12, 0);
            Inventory_Discard(owner, slot);
            GameFlag_Set(0x8a7);
            record = (u8 *)Actor_Get(ACTOR_PARTY_LEADER);
            Actor_WalkToAndWait(ACTOR_PARTY_LEADER, *(s16 *)(record + 10), 0x518);
            Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 72, 0x518);
            Actor_WalkToAndWait(12, 88, 0x518);
            Actor_FaceDirection(12, 0, 0);
        } else {
            Event_ShowMessage(12, 0);
        }
    }
done:
    Engine_EventEnd();
}

void FieldScene_RunActorThirteenFlagDialogue(void)
{
    Engine_EventBegin();

    if (GameFlag_IsSet(0x8A7) != 0) {
        Engine_EventSetMessage((s32)MsgKareiBoardWaitSetSail);
        Engine_EventOpenMessage(13, 0);
    } else if (GameFlag_IsSet(0x8A5) != 0) {
        Engine_EventSetMessage((s32)MsgKareiHaveTicketsGiveHim);
        Event_ShowMessage(13, 0);
    } else {
        Engine_EventSetMessage((s32)MsgKareiAfterHandingOverTicket);
        Event_ShowMessage(13, 0);
    }

    Engine_EventEnd();
}

void FieldScene_RunOpeningAuxiliarySequence(void)
{
    u32 i;
    s32 rec7;
    s32 record;
    s32 base6_2000240;
    s32 six00;

#if !EDITION_INTERNATIONAL
    /* FAKEMATCH: one coin-label selector keeps its saved register through
       the number draw. Separate whole enum arguments rematerialize the unit
       label and shorten the complete Japanese callback by twelve bytes. */
    s32 msg;
#endif

    six00 = 0x258;
    Engine_EventBegin();
    if (GameFlag_IsSet(0x8a5) != 0) {
        Engine_EventSetMessage((s32)MsgKareiHandInTicketPlank);
        Event_ShowMessage(8, 0);
    } else {
        Engine_EventSetMessage((s32)MsgKareiLookingForTickets);
        Event_OpenMessage(8, 0);
        if (Engine_EventChooseYesNo(0, 0) == 1) {
            Event_ShowMessageAndWait(8, 0, 10);
        } else {
            bump_step(1);
            UiWork_PushValueSlot(six00, 5);
            Event_OpenMessage(8, 0);
            rec7 = UiWindow_Create(TICKET_COUNTER_WINDOW_X, 8, TICKET_COUNTER_WIDTH, 4, 2);
#if EDITION_INTERNATIONAL
            UiText_DrawCharacterAtOffset(MsgYourCoins, rec7, 0, 0);
#else
            msg = MsgYourCoins;
            UiText_DrawCharacterAtOffset(msg, rec7, 0, 0);
#endif
            base6_2000240 = (s32)Data_02000240;
#if !EDITION_INTERNATIONAL
            msg += MsgCoins - MsgYourCoins;
#endif
            UiText_DrawNumberInWindow(*(s32 *)(base6_2000240 + 16), 6, rec7, TICKET_COUNTER_X, 8);
#if !EDITION_INTERNATIONAL
            UiText_DrawCharacterAtOffset(msg, rec7, 48, 8);
#endif
            if (Engine_EventChooseYesNo(-1, 0) == 1) {
                UiWork_Finalize(rec7, 2);
                Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 4);
                Engine_EventWait(10);
                Event_ShowMessage(8, 0);
                goto L_02000660;
            } else {
                if ((u32)six00 <= (u32)*(s32 *)(base6_2000240 + 16)) {
                    goto L_0200061e;
                }
                UiWork_Finalize(rec7, 2);
                Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
                Engine_EventWait(10);
                bump_step(1);
                Audio_PlayCue(113);
                Event_ShowMessage(8, 0);
                goto L_02000660;
            }
            L_0200061e:;
            UiWork_Finalize(rec7, 2);
            Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
            Engine_EventWait(10);
            bump_step(3);
            Event_ShowMessage(8, 0);
            Engine_PartyGiveItem(235, 0);
            GameFlag_Set(0x8a5);
            Party_AdjustSixDigitCounterA(-six00);
        }
        L_02000660:;
        Engine_EventEnd();
    }
}

void KareiTorebi_TalkScaryTrip(void)
{
    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgKareiHearScaryTrip);
    Event_AskYesNo(8, 0);
    Engine_EventEnd();
}

void KareiTorebi_TalkSeasickTourists(void)
{
    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgKareiTouristsLookUpset);
    Event_AskYesNo(10, 0);
    Engine_EventEnd();
}

void FieldScene_RunScene3ae_020006c8(void)
{
    u32 i;
    s32 record;

    Engine_EventBegin();
    if (GameFlag_IsSet(0x8a8) != 0) {
        Actor_FaceActor(11, ACTOR_PARTY_LEADER, 0);
        Engine_EventWait(20);
        Engine_EventSetMessage((s32)MsgKareiThanksMessageParents);
        Event_ShowMessage(11, 0);
        Engine_EventEnd();
    } else {
        Engine_EventWait(20);
        Actor_ShowEmote(11, 0x100, 50);
        Actor_FaceActor(11, ACTOR_PARTY_LEADER, 0);
        Engine_EventWait(20);
        Engine_EventSetMessage((s32)MsgKareiSilkRoadBlocked);
        Event_ShowMessage(11, 0);
        if (GameFlag_IsSet(0x8a6) != 0) {
            Engine_EventWait(20);
            Actor_ShowEmote(11, 0x102, 40);
            Event_OpenMessage(11, 0);
            if (Engine_EventChooseYesNo(0, 0) == 0) {
                Engine_EventWait(20);
                Event_ShowMessage(11, 0);
                GameFlag_Set(0x8a8);
                goto L_020007be;
            }
            Engine_EventWait(10);
            bump_step(1);
            Event_ShowMessage(11, 0);
            Engine_EventWait(10);
            Actor_FaceDirection(11, 0, 0);
            Engine_EventWait(30);
        } else {
            Engine_EventWait(10);
            Actor_FaceDirection(11, 0, 0);
            Engine_EventWait(30);
        }
        L_020007be:;
        Engine_EventEnd();
    }
}

void KareiTorebi_LeaveScene(void)
{
    Engine_EventBegin();
    Audio_PlayCue(158);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x8000, 0x4000);
    Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 3);
    if (gGameState.scene == (s32)&SceneId_KareiTorebi1) {
        Actor_WalkTo(ACTOR_PARTY_LEADER, 0x130, 0x570);
        Map_AnimateCells(KareiTorebi_LeaveCells1, 78, 86);
    } else {
        if (gGameState.scene == (s32)&SceneId_KareiTorebi3) {
            Actor_WalkTo(ACTOR_PARTY_LEADER, 248, 192);
            Map_AnimateCells(KareiTorebi_LeaveCells3, 74, 9);
        }
    }
    Engine_EventWait(16);
    Engine_EventRequestExit(3);
    Engine_EventEnd();
}

s32 Scene_Initialize(void)
{
    u32 i;
    s32 record;

    if (gGameState.entrance == 90) {
        Engine_GameFlagSet(0x950);
    }
    if (gGameState.scene == (s32)&SceneId_KareiTorebi1) {
        FieldScene_RunScene3ae_020008cc();
    } else {
        if (gGameState.scene == (s32)&SceneId_KareiTorebi3) {
            FieldScene_ConfigureFlaggedActors();
        } else {
            if (gGameState.scene == (s32)&SceneId_KareiTorebi2) {
                SceneState_SetRuntimeWord448To521AndSend303();
            }
        }
    }
    return 0;
}

void FieldScene_RunScene3ae_020008cc(void)
{
    u32 i;
    s32 record;

    if (gGameState.entrance == 1) {
        if (GameFlag_IsSet(0x8ac) == 0) {
            GameFlag_Set(0x8ac);
            FieldScene_RunScene3aeSequenceA();
        }
    }
    if (gGameState.entrance == 2) {
        if (GameFlag_IsSet(0x109) == 0) {
            GameFlag_Clear(0x8a9);
        }
    }
    if (GameFlag_IsSet(0x911) != 0) {
        if (GameFlag_IsSet(0x8a9) == 0) {
            Actor_SetPosition(12, 0x580000, 0x5180000);
            Actor_FaceDirection(12, 0, 0);
        }
    }
}

/* Places the flagged actors: the guards and posts moved by flags 0x8aa and 0x8ab, and the cells opened by flag 0x950. */
void FieldScene_ConfigureFlaggedActors(void)
{
    struct FieldActor *actor;
    u8 zero;

    FieldScene_PlaceSlots14And15();
    if (Value1(Engine_GameFlagIsSet, 0x950)) {
        Engine_ActorSetChildValue(12, 2);
    }
    if (((union GameStateRows *)&gGameState)->halves[225][0] == 3) {
        Engine_GameFlagClear(0x12f);
    }
    if (((union GameStateRows *)&gGameState)->halves[225][0] == 1) {
        Engine_GameFlagClear(0x8aa);
    }
    if (Engine_GameFlagIsSet(0x8aa)) {
        Call3(Engine_ActorSetPosition, 8, 0x1980000, 0x1280000);
        Call3(Engine_ActorFaceDirection, 8, 0x8000, 0);
    }
    if (Engine_GameFlagIsSet(0x8ab)) {
        Call3(Engine_ActorSetPosition, 13, 0x1180000, 0x1280000);
        Call3(Engine_ActorFaceDirection, 13, 0xc000, 0);
        Call3(Engine_ActorSetPosition, 16, 0x1200000, 0x1180000);
        Call3(Engine_ActorFaceDirection, 16, 0xe000, 0);
        Call3(Engine_ActorSetPosition, 10, 0xe80000, 0x1300000);
        Call3(Engine_ActorFaceDirection, 10, 0x4000, 0);
        Call3(Engine_ActorSetPosition, 11, 0xf00000, 0x1380000);
        Call3(Engine_ActorFaceDirection, 11, 0xc000, 0);
        actor = Object_GetById(10);
        actor->collision_flags = 0;
        actor->priority_flags = ACTOR_PRIORITY_UNDERFOOT;
        ((u8 *)actor->sprite)[9] |= 12;
        ((u8 *)actor->sprite)[38] = 0;
        zero = 0;
        ((struct SpriteBytes *)actor->sprite)->rotation = 0xc000;
        actor = Object_GetById(11);
        actor->priority_flags = zero;
        ((u8 *)actor->sprite)[9] |= 12;
        ((u8 *)actor->sprite)[21] |= 12;
    }
    if (Engine_GameFlagIsSet(0x950)) {
        Call6(Engine_MapCopyCellAttributes, 18, 18, 1, 1, 14, 18);
        Engine_MapCopyCellAttributes(18, 18, 1, 1, 15, 18);
    }
}

void SceneState_SetRuntimeWord448To521AndSend303(void)
{
    u8 *state = Data_03001ebc;
    s32 *slot = (s32 *)(state + 0x1C0);

    *slot = 0x209;
    GameFlag_Clear(0x12F);
}

void FieldScene_PlaceSlots14And15(void)
{
    s32 pos14;
    s32 pos15;

    pos14 = ((s32 *)Object_GetById(14))[2] >> 20;   /* [r0,#8], asrs #20 */
    pos15 = ((s32 *)Object_GetById(15))[2] >> 20;

    Map_CopyCellAttributes(5, 12, 5, 1, 5, 11);
    Map_CopyCellAttributes(1, 0, 1, 1, pos15, 11);
    Map_CopyCellAttributes(1, 0, 1, 1, pos14, 11);

    SceneActor_SetMode3AndFlagBit1(14);
    SceneActor_SetMode3AndFlagBit1(15);
}

void SceneActor_SetMode3AndFlagBit1(s32 no)
{
    u8 *p = ((u8 *)Object_GetById(no));
    u8 *flag;

    Engine_ActorSetSpriteFlags(((u8 *)Object_GetById(no)), 0);
    Engine_ActorSetSpritePriority(no, 3);
    flag = p + 85;
    *flag = 0;
    p += 35;
    {
        u8 bit = 2;

        *p = bit | *p;
    }
}

void FieldScene_RunScene3aeSequenceA(void)
{
    u32 i;
    s32 record;

    Engine_EventBegin();
    Battle_ResetEffectCounter();
    Actor_SetPosition(8, 0x1480000, 0x5900000);
    /* Set the flag byte at +91 of record 8. */
    *(u8 *)(((s32)Object_GetById(8)) + 91) = 1;
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_EventWait(20);
    Call4(Motion_LaunchFromFocusedObject, 1, -16, 0, 0x8000);
    Engine_ActorWaitForMove(ACTOR_GERALD);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xa000, 0);
    Engine_EventWait(20);
    Engine_EventSetMessage((s32)MsgKareiIncredibleOcean);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xa000, 0);
    Actor_SetSpeed(ACTOR_GERALD, 0x19999, 0xcccc);
    Actor_WalkToAndWait(ACTOR_GERALD, 232, 0x590);
    Actor_FaceDirection(ACTOR_GERALD, 0x8000, 0);
    Camera_MoveTo(0xb80000, -1, 0x5a00000, 1);
    Engine_CameraWaitForMove();
    Engine_EventWait(10);
    Engine_ActorJump(ACTOR_GERALD, 6, 15);
    Engine_ActorJump(ACTOR_GERALD, 6, 40);
    Event_ShowMessage(ACTOR_GERALD, 0);
    Engine_EventWait(20);
    Camera_MoveTo(0x1080000, -1, 0x5a80000, 1);
    Engine_CameraWaitForMove();
    Engine_EventWait(20);
    Actor_ShowEmote(8, 0x100, 50);
    Actor_SetSpeed(8, 0x13333, 0x9999);
    Actor_WalkToAndWait(8, 0x108, 0x590);
    Actor_FaceDirection(8, 0x8000, 0);
    Engine_EventWait(10);
    Actor_FaceDirection(ACTOR_GERALD, 0, 0);
    Engine_EventWait(20);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(8, 4);
    Engine_EventWait(10);
    Event_ShowMessage(8, 0);
    Engine_EventWait(20);
    Actor_ShowEmote(ACTOR_GERALD, 0x102, 40);
    Engine_EventWait(30);
    Actor_FaceDirection(ACTOR_GERALD, 0x8000, 0);
    Engine_EventWait(50);
    Event_ShowMessage(ACTOR_GERALD, 0);
    Engine_EventWait(20);
    Actor_FaceDirection(ACTOR_GERALD, 0x2000, 0);
    Engine_EventWait(30);
    Actor_SetSpeed(ACTOR_GERALD, 0x10000, 0x8000);
    Actor_WalkToAndWait(ACTOR_GERALD, 0x108, 0x5b8);
    Actor_FaceDirection(ACTOR_GERALD, 0, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x8000, 0);
    Actor_FaceDirection(8, 0x4000, 0);
    Engine_EventWait(30);
    Event_ShowMessage(ACTOR_GERALD, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Engine_EventWait(30);
    Engine_ActorSetAnimationAndWait(ACTOR_GERALD, 3);
    Engine_EventWait(30);
    Actor_SetSpeed(ACTOR_GERALD, 0x13333, 0x9999);
    Engine_ActorSetAnimation(ACTOR_GERALD, 2);
    /* If a record is returned, pass its s16 fields at +10 and +18 back in as
     * arguments. */
    record = Object_GetById(0);
    if (record != 0) {
        Actor_SetDestination(ACTOR_GERALD, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Engine_ActorWaitForMove(ACTOR_GERALD);
    Actor_SetPosition(ACTOR_GERALD, 0, 0);
    Engine_EventWait(20);
    /* Clear the flag byte at +91 of record 8. */
    *(u8 *)(((s32)Object_GetById(8)) + 91) = 0;
    Engine_ActorEnableActionCallback(8, 2);
    record = Object_GetById(8);
    /* Store the integer part of the 16.16 fixed-point fields at +8 and +16
     * into the halfwords at +100 and +102. */
    {
        s32 shown = *(s32 *)(record + 8) / 0x10000;

        *(u16 *)(record + 100) = shown;
    }
    {
        s32 shown = *(s32 *)(record + 16) / 0x10000;

        *(u16 *)(record + 102) = shown;
    }
    Engine_EventEnd();
}

void FieldScene_RunScene3aeSequenceB(void)
{
    u32 i;
    u8 *record;
    s32 none;
    s32 v5;
    s32 v6;

    GameFlag_Set(0x8ab);
    Engine_EventBegin();
    Battle_ResetEffectCounter();
    Engine_EventSetMessage((s32)MsgKareiReturnedTicketCost);
    record = ((u8 *)Object_GetById(11));
    none = 0;
    record[35] = none;
    *(u8 *)(*(s32 *)((s32)record + 80) + 9) |= 12;
    *(u8 *)(*(s32 *)((s32)record + 80) + 21) |= 12;
    Camera_MoveTo(0xe80000, -1, 0x1300000, 1);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 216, 0x110);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x2000, 0);
    Engine_CameraWaitForMove();
    Engine_EventWait(20);
    Actor_ShowEmote(13, 0x102, 50);
    Event_ShowMessage(13, 0);
    Engine_EventWait(10);
    Actor_ShowEmote(10, 0x107, 50);
    Actor_FaceDirection(10, 0, 0);
    Engine_ActorJump(10, 4, 13);
    Engine_ActorJump(10, 4, 30);
    Event_ShowMessage(10, 0);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(11, 2);
    Engine_EventWait(20);
    Event_ShowMessage(11, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(13, 4);
    Engine_EventWait(20);
    Engine_EventShowMessage(13, 0);
    Engine_EventWait(10);
    Actor_ShowEmote(10, 0x103, 55);
    Actor_SetSpeed(10, 0x20000, 0x10000);
    Actor_WalkByAndWait(10, 16, 0);
    Engine_ActorJump(10, 7, 0);
    v5 = 254;
    Actor_WalkByAndWait(10, 24, 0);
    *(u8 *)(((s32)Object_GetById(10)) + 90) &= v5;
    Actor_WalkBy(10, -16, 0);
    Audio_PlayCue(153);
    Actor_SetSpeed(13, 0x26666, 0x13333);
    Actor_WalkByAndWait(13, 16, 0);
    Engine_EventWait(10);
    v6 = 1;
    Engine_ActorSetAnimation(10, 1);
    *(u8 *)(((s32)Object_GetById(10)) + 90) |= v6;
    Actor_SetAttachedEffect(13, 0x102);
    Engine_ActorStartRepeatedMotion(13, 2);
    Audio_PlayCue(155);
    Engine_EventWait(10);
    Audio_PlayCue(155);
    Engine_EventWait(10);
    Audio_PlayCue(155);
    Engine_EventWait(10);
    Engine_EventWait(20);
    Actor_SetSpeed(13, 0x6666, 0x3333);
    Engine_ActorJump(13, 6, 0);
    Audio_PlayCue(159);
    Actor_WalkByAndWait(13, -8, 0);
    Engine_EventWait(20);
    Event_ShowMessage(10, 0);
    Engine_EventWait(10);
    Actor_ShowEmote(13, 0x102, 70);
    Actor_SetSpeed(16, 0x10000, 0x8000);
    Actor_WalkByAndWait(16, -8, 0);
    Actor_FaceDirection(16, 0x5000, 0);
    Engine_EventWait(30);
    Engine_ActorSetAnimationAndWait(16, 4);
    Engine_EventWait(20);
    Event_ShowMessage(16, 0);
    Engine_EventWait(10);
    Actor_FaceDirection(10, 0xe000, 0);
    Engine_EventWait(35);
    Engine_ActorRunRepeatedMotion(10, 2);
    Engine_EventWait(20);
    Event_ShowMessage(10, 0);
    Engine_EventWait(10);
    Actor_FaceDirection(16, 0x2000, 0);
    Engine_EventWait(55);
    Actor_FaceDirection(16, 0x5000, 0);
    Engine_EventWait(30);
    Event_ShowMessage(16, 0);
    Engine_EventWait(10);
    Actor_FaceDirection(11, 0xe000, 0);
    Engine_EventWait(20);
    Actor_ShowEmote(11, 0x102, 50);
    Event_ShowMessage(11, 0);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(13, 2);
    Engine_EventWait(20);
    Event_ShowMessage(13, 0);
    Engine_EventWait(10);
    Actor_FaceDirection(13, 0xa000, 0);
    Engine_EventWait(60);
    Actor_FaceDirection(13, 0x8000, 0);
    Engine_EventWait(30);
    Event_ShowMessage(13, 0);
    Engine_EventWait(10);
    Actor_FaceDirection(10, 0, 0);
    Actor_FaceDirection(11, 0, 0);
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(10, 2);
    Engine_EventWait(20);
    Event_ShowMessage(10, 0);
    Actor_SetSpeed(10, 0x13333, 0x9999);
    Actor_WalkByAndWait(10, 8, 0);
    Actor_SetSpeed(16, 0x20000, 0x10000);
    Actor_WalkByAndWait(16, -8, 16);
    Actor_FaceDirection(16, 0x8000, 0);
    Event_ShowMessage(16, 0);
    Engine_EventWait(10);
    Actor_ShowEmote(10, 0x102, 50);
    *(u8 *)(((s32)Object_GetById(10)) + 90) &= v5;
    Actor_SetSpeed(10, 0xcccc, 0x6666);
    Actor_WalkByAndWait(10, -8, 0);
    *(u8 *)(((s32)Object_GetById(10)) + 90) |= v6;
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(10, 2);
    Engine_EventWait(20);
    Event_ShowMessage(10, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(16, 4);
    Engine_EventWait(20);
    Event_ShowMessage(16, 0);
    Engine_EventWait(10);
    *(u8 *)(((s32)Object_GetById(10)) + 90) &= v5;
    Actor_SetSpeed(10, 0xcccc, 0x6666);
    Actor_WalkByAndWait(10, -16, 0);
    *(u8 *)(((s32)Object_GetById(10)) + 90) |= v6;
    Actor_FaceDirection(10, 0, 0);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(10, 4);
    Engine_EventWait(20);
    Actor_SetSpeed(10, 0x1cccc, 0xe666);
    Actor_WalkByAndWait(10, 8, 0);
    Engine_ActorJump(10, 6, 0);
    Actor_WalkByAndWait(10, 24, 0);
    Audio_PlayCue(133);
    Engine_ActorJump(16, 6, 0);
    Engine_ActorEnableActionCallback(16, KareiTorebi_ActorSixteenScript);
    *(u8 *)(((s32)Object_GetById(10)) + 90) &= v5;
    Engine_ActorJump(10, 6, 0);
    Actor_WalkByAndWait(10, -12, 4);
    record = Value1(Object_GetById, 10);
    record[89] = none;
    record[35] = 2;
    *(u8 *)(*(s32 *)((s32)record + 80) + 9) |= 12;
    *(u8 *)(*(s32 *)((s32)record + 80) + 38) = none;
    {
        s32 target = *(s32 *)((s32)record + 80);
        s32 shown = 0xc000;

        *(u16 *)(target + 30) = shown;
    }
    Actor_WalkByAndWait(10, -12, 4);
    Actor_FaceDirection(10, 0x4000, 0);
    {
        u8 *flags = ((u8 *)Object_GetById(10)) + 90;
        u8 value = *flags | v6;

        *flags = value;
    }
    Audio_PlayCue(159);
    Engine_EventWait(20);
    Actor_ShowEmote(11, 0x102, 50);
    Actor_SetSpeed(11, 0x18000, 0xc000);
    Actor_WalkByAndWait(11, 24, 0);
    Actor_FaceDirection(11, 0xc000, 0);
    Engine_EventWait(10);
    Event_ShowMessage(11, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(16, 4);
    Engine_EventWait(20);
    Event_ShowMessage(16, 0);
    Engine_EventWait(20);
    Actor_SetSpeed(16, 0xcccc, 0x6666);
    Actor_WalkByAndWait(16, -8, 0);
    Engine_EventWait(20);
    Event_ShowMessage(16, 0);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(10, 2);
    Engine_EventWait(20);
    Engine_EventWait(10);
    Actor_FaceDirection(16, 0, 0);
    Engine_EventWait(40);
    Engine_ActorSetAnimationAndWait(13, 3);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(13, 3);
    Engine_EventWait(20);
    Engine_EventWait(10);
    Actor_SetSpeed(16, 0x10000, 0x8000);
    Actor_WalkByAndWait(16, 24, -24);
    Actor_WalkByAndWait(16, 8, 0);
    Actor_FaceDirection(16, 0xe000, 0);
    Engine_EventWait(20);
    Actor_SetSpeed(13, 0x10000, 0x8000);
    Actor_WalkByAndWait(13, 0, -8);
    Engine_EventWait(10);
    Engine_EventEnd();
}
