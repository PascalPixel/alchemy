/* Declarations shared by the Sol Sanctum star room scene's sources (resource_380). */
#ifndef STAR_H
#define STAR_H

#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
/*
 * The Elemental Star Room of Sol Sanctum. Sukureta brings Robin, Gerald and
 * Jasmine in, explains the four Elemental Stars and hands out Mythril Bags,
 * and the party bags the Mercury, Mars and Jupiter Stars. Saturos and
 * Menardi then hold Sukureta and Jasmine hostage, Garcia takes off his mask
 * as their guarantee, and Alex takes the three Stars, leaving the party to
 * fetch the last one.
 *
 * The placed actors are named from what they say: Sukureta explains the
 * Stars, Garcia's masked actor is replaced by his unmasked one, Alex is the
 * one the party hands the Stars to, and in the collapse that follows Menardi
 * answers Saturos by name.
 */

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

#include "CREATE_CONFIGURED_OVERLAY_OBJECT.H"
#include "OVERLAY_OBJECT.H"

typedef struct {
    u8 pad[9];
    u8 f0 : 2;
    u8 f1 : 2;
    u8 f2 : 4;
} S;

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

extern s32 Inventory_Count(s32 arg0);                         /* 0x02004352 -> 08077248 */
extern s32 Inventory_Count(s32 arg0);                         /* 0x0200435c -> 08077248 */
extern s32 Shop_PickUnitItem(s32 *out_first, s32 *out_second);  /* 0x02004372 -> 080b0058 */
extern void Inventory_Discard(s32 arg0, s32 arg1);              /* 0x02004382 -> 080772b0 */
extern void PartyInventory_Add(s32 arg0);                        /* 0x0200438a -> 08077030 */
extern void PartyInventory_Add(s32 arg0);                        /* 0x02004390 -> 08077030 */
extern void PartyInventory_Add(s32 arg0);                        /* 0x02004396 -> 08077030 */
extern void PartyInventory_Add(s32 arg0);                        /* 0x0200439c -> 08077030 */

u16 CalculateAngleFromCoordinateDelta(s32, s32);
void Scene_UnmaskGarcia(void);
void Scene_BagJupiterStar(void);
void UiWork_PushValueSlotFar();
s32 Scene_PresentItem();
void Func_0200227c(void);
void Party_RemoveOwnerRestored();
void PartyInventory_Discard();
void Object_SetMoveTarget();
void SoruStar_RiseActorFourteenSparks();
void UiText_ShowCenteredMessage();
s32 Owner_GetState();
void Engine_EventWaitForScreen();
void Engine_CameraWaitForMove();
void Engine_ActorSetSpeed();
void Engine_EventWait();
void Engine_ActorFaceDirection();

/*
 * resource_380: the talk handler for actor 9, one of a bank of per-actor
 * published callbacks. The handler is published through a script record
 * rather than called, so nothing here reaches it by name.
 */

/*
 * resource_380: the talk handler for actor 5, published through a script
 * record rather than called.
 */

/*
 * Overlay resource_380: the per-actor talk handlers of the party
 * introduction scene.
 */

/*
 * Callback for resource_380 that forwards to Engine_LeaderCheckAhead and nothing else.
 * A script record holds its Thumb pointer; nothing calls it directly. The
 * twelve-byte owner includes the zero alignment halfword after the return and
 * has no literal pool. Engine_LeaderCheckAhead takes no arguments -- nothing is loaded
 * into r0 before the call.
 */

/*
 * resource_380 owner at 0x02004248, 24 bytes including the trailing
 * alignment halfword: a two-call convenience wrapper.  It shows a line
 * and waits.
 */

/* Moves the next dialogue line on by amount messages. */
static __inline__ void bump_step(s32 amount)
{
    gEventWork->message += amount;
}

struct ObjectRuntime;

void Event_SayThenWait(s32 speaker, s32 frames);

extern u16 SoruStar_StarCells[];
extern s32 Placement_Scripts[];
extern s32 Placement_Messages[];
extern s32 Placement_Actors[];
extern s32 Placement_Effects[];

#endif
