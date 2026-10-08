#ifndef STAR_H
#define STAR_H

#include "types.h"
#include "field_event.h"
#include "field_scene.h"

enum StarRoomActor {
    ACTOR_SUKURETA = ACTOR_FIRST_PLACED + 1,
    ACTOR_SATUROS,
    ACTOR_MENARDI,
    ACTOR_GARCIA_MASKED,
    ACTOR_GARCIA,
    ACTOR_ALEX
};

enum StarRoomFlag {
    FLAG_STAR_ROOM_EXPLAINED = 0x83b,
    FLAG_FIRST_STAR_BAGGED = 0x83c,
    FLAG_MERCURY_STAR_BAGGED = 0x83d,
    FLAG_STARS_GIVEN_TO_ALEX = 0x83e
};

enum {
    ITEM_MYTHRIL_BAG = 224
};

#define NULL ((void *)0)
#define FIELD_AT_OFFSET(base, type, offset) (*(type *)((u8 *)(base) + (offset)))
void BattleFx_UpdateObjectMotionScaleAndLinkedAngle(void *object);

#include "create_configured_overlay_object.h"
#include "overlay_object.h"

union MotionWork {
    struct {
        u32 unk_00[2];
        s32 x, y, z;
        u32 unk_14;
        s32 accum_x, accum_y;
        u32 unk_20[4];
        s32 rate_x, rate_y;
        u32 unk_38[3];
        s32 velocity_x, velocity_y, velocity_z;
        u16 *record;
        u8 unk_54[16];
        u16 angle_step;
    } fields;
    u8 bytes[102];
};

extern s32 Inventory_Count(s32 arg0);
extern s32 Inventory_Count(s32 arg0);
extern s32 Shop_PickUnitItem(s32 *out_first, s32 *out_second);
s32 Inventory_Discard(s32 owner, s32 slot);
extern void PartyInventory_Add(s32 arg0);
extern void PartyInventory_Add(s32 arg0);
extern void PartyInventory_Add(s32 arg0);
extern void PartyInventory_Add(s32 arg0);

s32 CalculateAngleFromCoordinateDelta(s32, s32);
void Scene_UnmaskGarcia(void);
void Scene_BagJupiterStar(void);
void UiWork_PushValueSlot();
s32 Scene_PresentItem(s32 item, s32 x, s32 y, s32 z);
void SoruStar_LineUpFollowers(void);
void Party_RemoveOwnerRestored();
void PartyInventory_Discard();
void Engine_ObjectSetPosition();
void SoruStar_RiseActorFourteenSparks();
void UiText_ShowCenteredMessage();
void *UnitGet(u32 owner);
void Engine_EventWaitForScreen();
void Engine_CameraWaitForMove();
void Engine_ActorSetSpeed();
void Engine_EventWait();
void Engine_ActorFaceDirection();

struct ObjectRuntime;

void Event_SayThenWait(s32 speaker, s32 frames);

extern u16 SoruStar_StarCells[];
extern s32 Placement_Scripts[];
extern s32 Placement_Messages[];
extern s32 Placement_Actors[];
extern s32 Placement_Effects[];

#endif
