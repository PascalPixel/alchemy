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

enum StarRoomMessage {
    MSG_SUKURETA_WHERE_ARE_WE = 0x103c,
    MSG_SUKURETA_THE_ELEMENTS_ARE_THE_SOURCE = 0x1048,
    MSG_SUKURETA_THE_WISDOM_STONE = 0x1056,
    MSG_THE_STARS_HAVE_ENORMOUS_POWER = 0x105b,
    MSG_JASMINE_WONT_YOU_PLEASE = 0x1068,
    MSG_JASMINE_THANK_YOU = 0x1069,
    MSG_THE_BAGS_ARE_FROM_SUKURETAS_COTTAGE = 0x106d,
    MSG_NO_STARS_OUT_THERE = 0x1072,
    MSG_JUPITER_STAR_BAGGED = 0x1077,
    MSG_MARS_STAR_BAGGED = 0x1078,
    MSG_SUKURETA_BRING_THE_STARS_HERE = 0x1079,
    MSG_JASMINE_BE_CAREFUL = 0x107a,
    MSG_SUKURETA_FETCH_THE_OTHERS = 0x107b,
    MSG_JASMINE_SUKURETA_IS_GIDDY = 0x107c,
    MSG_MENARDI_DONT_YOU_WANT_THEM_SAFE = 0x109b,
    MSG_GERALD_ILL_TAKE_THEM = 0x10b4,
    MSG_GERALD_THEY_WONT_LET_JASMINE_GO = 0x10b5,
    MSG_ALEX_YOUR_FRIENDS = 0x10c3,
    MSG_ALEX_THANK_YOU = 0x10c4,
    MSG_ALEX_I_ASK_AGAIN = 0x10c6,
    MSG_MENARDI_BRING_THE_FINAL_GEM = 0x10c7,
    MSG_GARCIA_SILENCE = 0x10c8,
    MSG_JASMINE_MY_BROTHER_IS_ALIVE = 0x10c9,
    MSG_SATUROS_BRING_THE_FINAL_STAR = 0x10ca,
    MSG_SUKURETA_I_AM_RESPONSIBLE = 0x10cb,
    MSG_ALEX_ONLY_ONE_LEFT = 0x10cc,
    MSG_GOT_FOUR_MYTHRIL_BAGS = 0x111b,
    MSG_TOO_MANY_ITEMS = 0x111c
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
void Effect_Move(void *object);

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

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

static __inline__ s32 Value4(s32 (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    return f(a0, a1, a2, a3);
}

/* Moves the next dialogue line on by amount messages. */
static __inline__ void bump_step(s32 amount)
{
    gEventWork->message += amount;
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ s32 Value0(s32 (*f)())
{
    return f();
}

struct ObjectRuntime;

void Event_SayThenWait(s32 speaker, s32 frames);

extern u16 SoruStar_StarCells[];
extern s32 Placement_Scripts[];
extern s32 Placement_Messages[];
extern s32 Placement_Actors[];
extern s32 Placement_Effects[];

enum SoruStarMessage {
    MSG_VENUS_STAR_BAGGED = 0x1075,
    MSG_MERCURY_STAR_BAGGED = 0x1076,
    MSG_TAKE_THE_STARS_TO_THEM = 0x10b0,
    MSG_THEY_WONT_LET_JASMINE_GO = 0x10b2,
    MSG_ALEX_ASKS_FOR_STARS = 0x10b6
};

#endif
