/* Draft of resource_371 0x0200a4a8..0x0200a768 (704 bytes with pool),
 * WorldMap_RunBlackOrbScene; the listing keeps the rows. Remaining
 * difference: the reference loads the saved scene 2 from its literal pool, a
 * link-time value; the integer scene is an immediate (700 bytes, 8 differ
 * from +0x255). */
#include "TYPES.H"
#include "FIELD_EVENT.H"
extern u8 MsgWorldMapLook[];
extern u8 MsgWorldMapNowUseOnShip[];

void WorldMap_ActivateSite138(s32 actor);
void Battle_SetObjectFlag5bWhenMode3(void);
void Battle_ClearObjectFlag5bWhenMode3(void);
void BattleFx_ScheduleRatioTransition(s32 speed, s32 frames);

extern s32 gWorldMapTriggerActor;

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

/* World-map Black Orb scene: the trigger actor walks up, the camera shows the site, and the party receives the Black Orb before the map is sent to area 2, entrance 78. */
void WorldMap_RunBlackOrbScene(void)
{
    struct FieldActor *leader;

    Engine_EventBegin();
    gWorldMapTriggerActor = 55;
    WorldMap_ActivateSite138(55);
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Battle_SetObjectFlag5bWhenMode3();
    leader = Engine_ActorGet(0);
    if (leader != NULL) {
        Engine_ActorSetPosition(gWorldMapTriggerActor, leader->x.fixed, leader->z.fixed);
    }
    Call3(Engine_ActorSetSpeed, gWorldMapTriggerActor, 0x19999, 0xcccc);
    Call3(Engine_ActorWalkToAndWait, gWorldMapTriggerActor, 0x1768, 0xd78);
    Engine_ActorFaceDirection(gWorldMapTriggerActor, 0, 20);
    Engine_ActorShowEmote(gWorldMapTriggerActor, 0x100, 60);
    Engine_ActorStartRepeatedMotion(gWorldMapTriggerActor, 2);
    Engine_EventSetMessage((s32)MsgWorldMapLook);
    Engine_EventShowMessageAndWait(gWorldMapTriggerActor | 0x1000, 0, 10);
    Engine_ActorFaceDirection(0, 0, 0);
    Battle_ClearObjectFlag5bWhenMode3();
    BattleFx_ScheduleRatioTransition(0x16666, 10);
    Engine_CameraSetSpeed(0x80000, 0x10000);
    Engine_CameraMoveTo(0x17880000, -1, 0xd680000, 1);
    Engine_CameraWaitForMove();
    Engine_EventWait(40);
    Battle_SetObjectFlag5bWhenMode3();
    Engine_ActorSetAttachedEffect(gWorldMapTriggerActor, 0x102);
    Engine_EventWait(60);
    Engine_ActorRunRepeatedMotion(gWorldMapTriggerActor, 1);
    Engine_EventShowMessageAndWait(gWorldMapTriggerActor | 0x3000, 0, 40);
    Call3(Engine_ActorWalkToAndWait, gWorldMapTriggerActor, 0x1768, 0xd48);
    Call3(Engine_ActorWalkToAndWait, gWorldMapTriggerActor, 0x1794, 0xd48);
    Engine_ActorFaceDirection(gWorldMapTriggerActor, 0x3000, 20);
    Call3(Engine_ActorSetSpeed, 0, 0xcccc, 0x6666);
    Engine_ActorEnableActionCallback(0, (const u8 *)0x200cf20);
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(gWorldMapTriggerActor, 1);
    Engine_EventShowMessageAndWait(gWorldMapTriggerActor, 0, 10);
    while ((s16)Engine_ActorGet(0)->unknown_64 == 0) {
        Engine_TaskWait(1);
    }
    Call3(Engine_ActorFaceDirection, 0, 0x4000, 20);
    Engine_ActorShowEmote(gWorldMapTriggerActor, 0x106, 0);
    Engine_ActorRunRepeatedMotion(gWorldMapTriggerActor, 2);
    Engine_EventShowMessageAndWait(gWorldMapTriggerActor, 0, 10);
    Engine_ActorFaceDirection(gWorldMapTriggerActor, 0x8000, 10);
    Engine_EventShowMessage(gWorldMapTriggerActor, 0);
    Engine_ActorFaceDirection(0, 0, 20);
    Engine_ActorGet(gWorldMapTriggerActor)->unknown_5a &= ~1;
    Call3(Engine_ActorWalkToAndWait, gWorldMapTriggerActor, 0x178c, 0xd48);
    Engine_EventWait(1);
    Engine_ActorGet(gWorldMapTriggerActor)->unknown_5a |= 1;
    Engine_EventWait(20);
    Engine_ActorGet(gWorldMapTriggerActor)->unknown_5a &= ~1;
    Call3(Engine_ActorWalkToAndWait, gWorldMapTriggerActor, 0x1794, 0xd48);
    Engine_EventWait(1);
    Engine_ActorGet(gWorldMapTriggerActor)->unknown_5a |= 1;
    Engine_ItemShowFound(242, 3);
    Engine_PartyGiveItem(242, 0);
    Engine_ActorRunRepeatedMotion(gWorldMapTriggerActor, 1);
    Engine_EventSetMessage((s32)MsgWorldMapNowUseOnShip);
    Engine_EventShowMessageAndWait(gWorldMapTriggerActor, 0, 10);
    Engine_ActorFaceDirection(gWorldMapTriggerActor, 0x3000, 0);
    Battle_ClearObjectFlag5bWhenMode3();
    BattleFx_ScheduleRatioTransition(0x10000, 10);
    Engine_EventWait(20);
    Engine_GameFlagSet(0x234);
    Engine_GameFlagSet(0x9bf);
    gGameState.saved_scene = 2;
    gGameState.saved_entrance = 78;
    Engine_EventEnd();
}
