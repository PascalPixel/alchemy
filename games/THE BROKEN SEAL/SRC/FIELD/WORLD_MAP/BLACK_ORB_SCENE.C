#include "TYPES.H"
#include "FIELD_EVENT.H"

void WorldMap_PrepareTriggerActor(s32 actor);
void Main_0808a3d8(void);
void Main_0808a3e0(void);
void Main_0808a5c0(s32 speed, s32 frames);

union GameStateRows {
    u8 bytes[512][2];
    s16 halves[512][1];
    s32 words[256];
};

extern union GameStateRows Data_02000240_t;
extern u8 Data_00000002[];
extern s32 WorldMap_TriggerActor;

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

/* World-map Black Orb scene: the trigger actor walks up, the camera shows the site, and the party receives the Black Orb before the map is sent to area 2, entrance 78. */
void WorldMap_RunBlackOrbScene(void)
{
    struct FieldActor *leader;

    Engine_EventBegin();
    WorldMap_TriggerActor = 55;
    WorldMap_PrepareTriggerActor(55);
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Main_0808a3d8();
    leader = Engine_ActorGet(0);
    if (leader != NULL) {
        Engine_ActorSetPosition(WorldMap_TriggerActor, leader->x.fixed, leader->z.fixed);
    }
    Call3(Engine_ActorSetSpeed, WorldMap_TriggerActor, 0x19999, 0xcccc);
    Call3(Engine_ActorWalkToAndWait, WorldMap_TriggerActor, 0x1768, 0xd78);
    Engine_ActorFaceDirection(WorldMap_TriggerActor, 0, 20);
    Engine_ActorShowEmote(WorldMap_TriggerActor, 0x100, 60);
    Engine_ActorStartRepeatedMotion(WorldMap_TriggerActor, 2);
    Engine_EventSetMessage(0x263c);
    Engine_EventShowMessageAndWait(WorldMap_TriggerActor | 0x1000, 0, 10);
    Engine_ActorFaceDirection(0, 0, 0);
    Main_0808a3e0();
    Main_0808a5c0(0x16666, 10);
    Engine_CameraSetSpeed(0x80000, 0x10000);
    Engine_CameraMoveTo(0x17880000, -1, 0xd680000, 1);
    Engine_CameraWaitForMove();
    Engine_EventWait(40);
    Main_0808a3d8();
    Engine_ActorSetAttachedEffect(WorldMap_TriggerActor, 0x102);
    Engine_EventWait(60);
    Engine_ActorRunRepeatedMotion(WorldMap_TriggerActor, 1);
    Engine_EventShowMessageAndWait(WorldMap_TriggerActor | 0x3000, 0, 40);
    Call3(Engine_ActorWalkToAndWait, WorldMap_TriggerActor, 0x1768, 0xd48);
    Call3(Engine_ActorWalkToAndWait, WorldMap_TriggerActor, 0x1794, 0xd48);
    Engine_ActorFaceDirection(WorldMap_TriggerActor, 0x3000, 20);
    Call3(Engine_ActorSetSpeed, 0, 0xcccc, 0x6666);
    Engine_ActorEnableActionCallback(0, (const u8 *)0x200cf20);
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(WorldMap_TriggerActor, 1);
    Engine_EventShowMessageAndWait(WorldMap_TriggerActor, 0, 10);
    while ((s16)Engine_ActorGet(0)->unknown_64 == 0) {
        Engine_TaskWait(1);
    }
    Call3(Engine_ActorFaceDirection, 0, 0x4000, 20);
    Engine_ActorShowEmote(WorldMap_TriggerActor, 0x106, 0);
    Engine_ActorRunRepeatedMotion(WorldMap_TriggerActor, 2);
    Engine_EventShowMessageAndWait(WorldMap_TriggerActor, 0, 10);
    Engine_ActorFaceDirection(WorldMap_TriggerActor, 0x8000, 10);
    Engine_EventShowMessage(WorldMap_TriggerActor, 0);
    Engine_ActorFaceDirection(0, 0, 20);
    Engine_ActorGet(WorldMap_TriggerActor)->unknown_5a &= ~1;
    Call3(Engine_ActorWalkToAndWait, WorldMap_TriggerActor, 0x178c, 0xd48);
    Engine_EventWait(1);
    Engine_ActorGet(WorldMap_TriggerActor)->unknown_5a |= 1;
    Engine_EventWait(20);
    Engine_ActorGet(WorldMap_TriggerActor)->unknown_5a &= ~1;
    Call3(Engine_ActorWalkToAndWait, WorldMap_TriggerActor, 0x1794, 0xd48);
    Engine_EventWait(1);
    Engine_ActorGet(WorldMap_TriggerActor)->unknown_5a |= 1;
    Engine_ItemShowFound(242, 3);
    Engine_PartyGiveItem(242, 0);
    Engine_ActorRunRepeatedMotion(WorldMap_TriggerActor, 1);
    Engine_EventSetMessage(0x2642);
    Engine_EventShowMessageAndWait(WorldMap_TriggerActor, 0, 10);
    Engine_ActorFaceDirection(WorldMap_TriggerActor, 0x3000, 0);
    Main_0808a3e0();
    Main_0808a5c0(0x10000, 10);
    Engine_EventWait(20);
    Engine_GameFlagSet(0x234);
    Engine_GameFlagSet(0x9bf);
    Data_02000240_t.halves[226][0] = (s32)Data_00000002;
    Data_02000240_t.halves[227][0] = 78;
    Engine_EventEnd();
}
