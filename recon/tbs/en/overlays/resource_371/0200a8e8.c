/* Draft of WorldMap_UseBlackOrb, resource_371 at 0x0200a8e8 (FIELD/WORLD_MAP/USE_BLACK_ORB.C).
 * Remaining difference: register allocation. The ROM keeps the trigger
 * actor's address in r6 and 0x3000 in r8 where this keeps them the other way
 * round; the ROM's store of zero before the scene's start uses r7, the
 * register the uninitialised site pointer is read from in the west branch;
 * the orb's byte stores share r1 through the returned r0. 232 of 900
 * instructions differ, all from these. The veneer names it calls are
 * committed in IMPORT.S. */
/* The world map's Black Orb: Isaac steps up to the site and holds the orb
 * out, the guide explains it, the site's figure walks out and back, and the
 * orb is used up before the party carries on. */
#include "STORY.H"
#include "EVENT_RUNTIME.H"

extern u8 MsgWorldMapMatter[];

void ObjectDispatch_InitFromTable6(struct FieldActor *object);
void Engine_ObjectCommitPosition(struct FieldActor *object);
void WorldMap_RestoreExitTrigger(void);
void PartyInventory_Discard(s32 item);
void StoryActor_ConfigureSpawnedObject(u8 *actor);

#define SITE 55
#define ITEM_BLACK_ORB 242

void WorldMap_UseBlackOrb(void)
{
    struct FieldActor *leader;
    struct FieldActor *orb;
    struct FieldActor *site;
    struct FieldSprite *sprite;
    u8 *buffer;
    s32 message;

    leader = Object_GetById(0);
    Event_Begin();
    Battle_ClearObjectFlag5bWhenMode3();
    BattleFx_ScheduleRatioTransition(0x16666, 6);
    Camera_SetSpeed(0x30000, 0x6000);
    Camera_MoveTo(0x17880000, -1, 0xd680000, 1);
    Actor_SetSpeed(0, 0xcccc, 0x6666);
    Actor_SetAnimation(0, 2);
    leader->unknown_5b = 0;
    ObjectDispatch_InitFromTable6(leader);
    if (leader->z.fixed > 0xd680000) {
        if (leader->x.fixed > 0x176e0000) {
            Engine_ObjectSetPosition(leader, 0x176e0000, leader->y.fixed, 0xd7d0000);
            Engine_ObjectCommitPosition(leader);
        }
    } else if (leader->x.fixed > 0x177a0000) {
        Engine_ObjectSetPosition(leader, 0x177a0000, site->y.fixed, 0xd480000);
        Engine_ObjectCommitPosition(leader);
    }
    Engine_ObjectSetPosition(leader, 0x17690000, 0, 0xd680000);
    Engine_ObjectCommitPosition(leader);
    Actor_SetAnimation(0, 1);
    Actor_FaceDirection(0, 0, 40);
    Battle_SetObjectFlag5bWhenMode3();
    Actor_RunRepeatedMotion(0, 2);
    Event_Wait(20);
    Actor_SetAnimation(0, 28);
    orb = Object_Create(22, leader->x.fixed + 0x20000, 0x260000, leader->z.fixed);
    if (orb != NULL) {
        orb->motion_flags = 0;
        sprite = orb->sprite;
        sprite->flags = 0;
        sprite->part_count = 0;
        sprite->full_color = 0;
        sprite->palette = 0;
        buffer = Heap_Allocate(17, 0x608);
        Item_LoadIcon(ITEM_BLACK_ORB);
        Vram_Load(sprite->vram_block, 128, buffer + 0x400);
        Heap_Release(17);
        Event_Wait(20);
        orb->update = (void (*)(union FieldObject *))StoryActor_ConfigureSpawnedObject;
        Event_Wait(80);
    }
    Object_GetById(gWorldMapTriggerActor)->facing = 0x3000;
    Actor_ShowEmote(gWorldMapTriggerActor, 0x100, 0);
    Actor_RunRepeatedMotion(gWorldMapTriggerActor, 2);
    message = (s32)MsgWorldMapMatter;
    Event_SetMessage(message);
    Event_ShowMessageAndWait(gWorldMapTriggerActor, 0, 80);
    if (orb != NULL)
        Engine_ObjectDispatchRelease(orb);
    Actor_SetAnimation(0, 1);
    Event_Wait(40);
    Actor_Jump(gWorldMapTriggerActor, 6, 40);
    Event_ShowMessageAndWait(gWorldMapTriggerActor, 0, 20);
    Actor_FaceDirection(0, 0xe000, 0);
    Actor_FaceDirection(gWorldMapTriggerActor, 0xd000, 20);
    Event_ShowMessageAndWait(gWorldMapTriggerActor | 0x9000, 0, 40);
    Actor_SetAnimationAndWait(gWorldMapTriggerActor, 4);
    Event_ShowMessageAndWait(gWorldMapTriggerActor | 0x9000, 0, 20);
    Actor_FaceDirection(gWorldMapTriggerActor, 0x3000, 20);
    Event_ShowMessageAndWait(gWorldMapTriggerActor, 0, 10);
    Actor_SetSpeed(gWorldMapTriggerActor, 0xcccc, 0x6666);
    Actor_SetAnimation(gWorldMapTriggerActor, 2);
    site = Object_GetById(SITE);
    Engine_ObjectSetPosition(site, 0x177a0000, site->y.fixed, 0xd480000);
    Engine_ObjectCommitPosition(site);
    Engine_ObjectSetPosition(site, 0x17710000, 0, 0xd580000);
    Engine_ObjectCommitPosition(site);
    Actor_SetAnimation(SITE, 1);
    Actor_FaceDirection(gWorldMapTriggerActor, 0x5000, 10);
    Actor_RunRepeatedMotion(gWorldMapTriggerActor, 1);
    Event_ShowMessageAndWait(gWorldMapTriggerActor | 0x1000, 0, 20);
    Actor_SetSpeed(gWorldMapTriggerActor, 0x10000, 0x8000);
    Object_GetById(SITE)->unknown_5a &= ~1;
    Actor_SetAnimation(SITE, 2);
    Engine_ObjectSetPosition(site, 0x176d0000, 0, 0xd600000);
    Engine_ObjectCommitPosition(site);
    Actor_SetAnimation(SITE, 1);
    Event_Wait(10);
    Actor_SetAnimation(SITE, 2);
    Engine_ObjectSetPosition(site, 0x17710000, 0, 0xd580000);
    Engine_ObjectCommitPosition(site);
    Actor_SetAnimation(SITE, 1);
    Message_ShowCentered(message + 6, 1);
    gWork->value_1d8++;
    PartyInventory_Discard(ITEM_BLACK_ORB);
    Event_Wait(20);
    Actor_SetAnimation(gWorldMapTriggerActor, 4);
    Event_ShowMessageAndWait(gWorldMapTriggerActor, 0, 10);
    Actor_SetAnimationAndWait(0, 3);
    Actor_SetAnimationAndWait(gWorldMapTriggerActor, 3);
    Actor_SetAnimation(gWorldMapTriggerActor, 2);
    leader = Object_GetById(0);
    if (leader != NULL)
        Actor_SetDestination(gWorldMapTriggerActor, leader->x.part.pixel, leader->z.part.pixel);
    Actor_WaitForMove(gWorldMapTriggerActor);
    Actor_SetPosition(gWorldMapTriggerActor, 0, 0);
    Battle_ClearObjectFlag5bWhenMode3();
    BattleFx_ScheduleRatioTransition(0x10000, 6);
    Event_Wait(20);
    WorldMap_RestoreExitTrigger();
    Actor_Destroy(gWorldMapTriggerActor);
    GameFlag_Clear(0x234);
    GameFlag_Set(0x85d);
    Event_End();
}
