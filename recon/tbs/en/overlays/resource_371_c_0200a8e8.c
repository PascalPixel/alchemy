/* Near miss: WorldMap_UseBlackOrb, resource_371, 2026-10-10.
 * Sole per-function proposal for resource_371_c_0200a8e8.c.
 * Full current world_map/map.c: pinned baseline363/common363/differ0;
 * both bindings removed363/common353/differ20, verified in all six editions.
 * This minimal includes/declarations/function draft independently retains
 * that EN result. Native extents include pools:
 * JA/EN/DE0200a8e8..0200acb4; ES/FR/IT0200a8e0..0200acac.
 *
 * Residual: the first byte address is copied from orb in r7 to r2 rather
 * than retaining the returned pointer in r0; zero occupies r3 instead of r1.
 * The sprite-byte address consequently uses r2 rather than r3, and the
 * known zero in r3 builds -33 by subtraction rather than mov33/neg.
 * First CSE uses the long-lived orb pseudo for the motion address; late
 * allocation chooses the ordinary zero and byte-address carriers.
 *
 * Bounded ordinary-C full-TU trials, all without new asm/pins:
 * zero types u32/u16/s16/u8/s8 all20; !orb and orb==NULL51; site-derived
 * zero81; existing actor copy79; declaration order and direct motion store20.
 * Split/restored zero under leader or orb!=leader47; restored motion41.
 * Scoped zero, late initialization, and two inlined preparation helpers20.
 * Fresh scoped constructor pointer, split first-store if, and goto around
 * the null object20; motion store before publishing orb59. Ordinary register
 * zero20, const zero53, scoped flag-index stores31, independent zeros20.
 * Single binding removals: zero-only29(362 instructions), motion-only41(364).
 *
 * Earlier current-source notes, not repeated: generic motion output20;
 * zero/motion output53(364); opaque object copy or generic input/output
 * combinations47(364). Those still contain forbidden replacement devices.
 * Full-TU context must be retained for future measurements; full ROM gates
 * remain required for adoption.
 */
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "global_cells.h"
#include "types.h"
#include "field_event.h"
#include "field_scene.h"
#include "map_render_work.h"
#include "field_service.h"
#include "iwram_call.h"
#include "scene_ids.h"
#include "call.h"
#include "text/msg_ids.h"
#include "edition.h"
#include "tbs_edition.h"
#include "inventory.h"
#include "resource.h"
#include "mem.h"
#include "field_effect.h"
#include "io_reg.h"
#include "io_write_queue.h"
#include "event_runtime.h"

#define SCENE_PHASE (gEventWork->start_transition)
#define ACTOR 8

struct Object {
    u8 filler00[8];
    s32 x;
    u8 filler0C[4];
    s32 z;
};

struct StoryDialogueWork {
    u8 reserved000[370];
    u16 story_result;
};

struct StorySharedState {
    u8 reserved00[52];
    u8 active;
};

struct StoryVerticalEffectActor {
    u8 filler00[8];
    s32 x;
    s32 y;
    s32 z;
    u8 filler14[4];
    s32 amplitude_x;
    s32 amplitude_y;
    u8 filler20[0x44];
    u16 frame;
    u8 filler66[2];
    struct StoryVerticalEffectActor *anchor;
};

extern u8 gWorldMapEntrances[];
extern u8 gWorldMapExits[];
extern struct SceneEvent gWorldMapEvents[];
extern struct ScenePlacement gWorldMapPlacements[];
extern struct ScenePlacement gWorldMapPlacements49[];
extern struct ScenePlacement gWorldMapPlacements64[];
extern struct ScenePlacement gWorldMapPlacements65[];
extern struct ScenePlacement gWorldMapPlacements66[];
extern struct ScenePlacement gWorldMapPlacements71[];
extern struct ScenePlacement gWorldMapPlacements72[];
extern struct ScenePlacement gWorldMapPlacements73[];
extern struct ScenePlacement gWorldMapPlacements80[];
extern s32 gWorldMapRewards[];

extern u8 gTransferArrive10[];
extern u8 gTransferDepart10[];
extern u8 gTransferReturn10[];
extern u8 gTransferGather[];
extern u8 gTransferArrive11[];
extern u8 gTransferDepart11[];
extern u8 gTransferReturn11[];
extern u8 gTransferArrive12[];
extern u8 gTransferDepart12[];
extern u8 gTransferReturn12[];
extern u8 gTransferArrive13[];
extern u8 gTransferDepart13[];
extern u8 gTransferReturn13[];
extern u8 gTransferGuide14[];
extern u8 gTransferLeaderIdle[];
extern u8 gTransferLeaderTurn[];

extern s32 gWorldMapTriggerActor;
extern struct StorySharedState *gEffectWork;

void BattleFx_SetPhaseRequest();
void Battle_SetObjectFlag5bWhenMode3();
void Battle_ClearObjectFlag5bWhenMode3();
void BattleFx_ScheduleRatioTransition();
void BattleFx_SetWeightedResult();
void Event_WaitForZoomTransition();
void Event_SetDestination();
void ItemMenu_Open();
void Menu_AnimateSelectionToEntry();
void PaletteGlow_Update();
void Map_LoadDefaultCellsAndUpdateBlock();
void Object_SetActionCallbackAndRefreshById();
void Object_RefreshSelectorById();
s32 GameFlag_GetByte(s32 flag);
void GameFlag_SetByte(s32 flag, s32 value);
void QueueIoWriteDelay2(u32 address, u32 value);

void WorldMap_CreateLinkedEffects();
void StoryActor_AdvanceTimer(struct FieldActor *actor);
void WorldMap_SpawnActorEightPuff(void);
void StoryScene_UpdateSelectedActorProgress(void);
void StoryScene_UpdateTransitionEffect(void);
void WorldMap_UpdateTransferredActorLanding(void);
extern u8 gOpeningLeaderRise[];

s32 StoryScene_ComputeOpposingSlotDelta(void);

enum {
    CONTACT_LAST_ACTOR = 65,
    CONTACT_TRIGGER_BASE = 100,
    FLAG_CONTACT_PAUSED = 0x163,
    FLAG_CONTACT_BLOCKED = 0x104
};

extern u8 gDebugMode[];

void Event_SetDestination(s32 a0, s32 a1);
void RunEventScript01(void);


void StoryActor_ConfigureSpawnedObject(struct FieldActor *actor);
#define StoryActor_ConfigureSpawnedObject ((void (*)(u8*))StoryActor_ConfigureSpawnedObject)
#define SITE 55
#define ITEM_BLACK_ORB 242

void ObjectDispatch_InitFromTable6(struct FieldActor *object);

void WorldMap_UseBlackOrb(void)
{
    struct FieldActor *site;
    struct FieldActor *orb;
    struct FieldActor *leader;
    struct FieldActor *actor;
    s32 message;
    struct FieldSprite *sprite;
    u8 *buffer;

    leader = Actor_Get(ACTOR_PARTY_LEADER);
    Event_Begin();
    Battle_ClearObjectFlag5bWhenMode3();
    BattleFx_ScheduleRatioTransition(0x16666, 6);
    Camera_SetSpeed(0x30000, 0x6000);
    Camera_MoveTo(0x17880000, -1, 0xd680000, 1);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0xcccc, 0x6666);
    site = NULL;
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 2);
    leader->paused = 0;
    ObjectDispatch_InitFromTable6(leader);
    if (leader->z.fixed > 0xd680000) {
        if (leader->x.fixed > 0x176e0000) {
            FieldObject_SetPosition(leader, 0x176e0000, leader->y.fixed, 0xd7d0000);
            Engine_ObjectCommitPosition(leader);
        }
    } else if (leader->x.fixed > 0x177a0000) {
        FieldObject_SetPosition(leader, 0x177a0000, site->y.fixed, 0xd480000);
        Engine_ObjectCommitPosition(leader);
    }
    FieldObject_SetPosition(leader, 0x17690000, 0, 0xd680000);
    Engine_ObjectCommitPosition(leader);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 1);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0, 40);
    Battle_SetObjectFlag5bWhenMode3();
    Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Event_Wait(20);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 28);
    orb = Object_Create(22, leader->x.fixed + 0x20000, 0x260000,
                        leader->z.fixed);
    if (orb != NULL) {
        s32 zero = 0;
        u8 *motion = &orb->motion_flags;
        u8 *attr;

        *motion = zero;
        sprite = orb->sprite;
        attr = &sprite->flags;
        *attr = zero;
        attr++;
        *attr = zero;
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
    Actor_ShowEmote(gWorldMapTriggerActor, EMOTE_IN_FRONT, 0);
    Actor_RunRepeatedMotion(gWorldMapTriggerActor, 2);
    message = (s32)MsgWorldMapMatter;
    Event_SetMessage(message);
    Event_ShowMessageAndWait(gWorldMapTriggerActor, 0, 80);
    if (orb != NULL) {
        Engine_ObjectDispatchRelease(orb);
    }
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 1);
    Event_Wait(40);
    Actor_Jump(gWorldMapTriggerActor, 6, 40);
    Event_ShowMessageAndWait(gWorldMapTriggerActor, 0, 20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xe000, 0);
    Actor_FaceDirection(gWorldMapTriggerActor, 0xd000, 20);
    Event_ShowMessageAndWait(gWorldMapTriggerActor | 0x9000, 0, 40);
    Actor_SetAnimationAndWait(gWorldMapTriggerActor, 4);
    Event_ShowMessageAndWait(gWorldMapTriggerActor | 0x9000, 0, 20);
    Actor_FaceDirection(gWorldMapTriggerActor, 0x3000, 20);
    Event_ShowMessageAndWait(gWorldMapTriggerActor, 0, 10);
    Actor_SetSpeed(gWorldMapTriggerActor, 0xcccc, 0x6666);
    Actor_SetAnimation(gWorldMapTriggerActor, 2);
    site = Actor_Get(SITE);
    FieldObject_SetPosition(site, 0x177a0000, site->y.fixed, 0xd480000);
    Engine_ObjectCommitPosition(site);
    FieldObject_SetPosition(site, 0x17710000, 0, 0xd580000);
    Engine_ObjectCommitPosition(site);
    Actor_SetAnimation(SITE, 1);
    Actor_FaceDirection(gWorldMapTriggerActor, 0x5000, 10);
    Actor_RunRepeatedMotion(gWorldMapTriggerActor, 1);
    Event_ShowMessageAndWait(gWorldMapTriggerActor | 0x1000, 0, 20);
    Actor_SetSpeed(gWorldMapTriggerActor, 0x10000, 0x8000);
    Object_GetById(SITE)->steering_flags &= ~ACTOR_FACES_MOTION;
    Actor_SetAnimation(SITE, 2);
    FieldObject_SetPosition(site, 0x176d0000, 0, 0xd600000);
    Engine_ObjectCommitPosition(site);
    Actor_SetAnimation(SITE, 1);
    Event_Wait(10);
    Actor_SetAnimation(SITE, 2);
    FieldObject_SetPosition(site, 0x17710000, 0, 0xd580000);
    Engine_ObjectCommitPosition(site);
    Actor_SetAnimation(SITE, 1);
    Message_ShowCentered(message + 6, 1);
    gEventWork->message++;
    PartyInventory_Discard(ITEM_BLACK_ORB);
    Event_Wait(20);
    Actor_SetAnimation(gWorldMapTriggerActor, 4);
    Event_ShowMessageAndWait(gWorldMapTriggerActor, 0, 10);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimationAndWait(gWorldMapTriggerActor, 3);
    Actor_SetAnimation(gWorldMapTriggerActor, 2);
    actor = Actor_Get(ACTOR_PARTY_LEADER);
    if (actor != NULL) {
        Actor_SetDestination(gWorldMapTriggerActor, actor->x.part.pixel, actor->z.part.pixel);
    }
    Actor_WaitForMove(gWorldMapTriggerActor);
    Actor_SetPosition(gWorldMapTriggerActor, 0, 0);
    Battle_ClearObjectFlag5bWhenMode3();
    BattleFx_ScheduleRatioTransition(0x10000, 6);
    Event_Wait(20);
    WorldMap_RestoreExitTrigger();
    Engine_ActorDestroy(gWorldMapTriggerActor);
    GameFlag_Clear(0x234);
    GameFlag_Set(0x85d);
    Event_End();
}
