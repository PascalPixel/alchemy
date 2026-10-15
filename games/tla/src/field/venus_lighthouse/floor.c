#include "types.h"
#include "party_state.h"
#include "ram_buffer.h"
#include "event_work.h"

struct VenusActor {
    u8 unknown_00[6];
    u16 facing;
    s16 x_low;
    s16 x_pixel;
    s32 y;
    s16 z_low;
    s16 z_pixel;
    s32 terrain_height;
    u8 unknown_18[0xa];
    u8 terrain_id;
    u8 unknown_23;
    s32 velocity_x;
    s32 speed;
    s32 velocity_z;
    s32 move_rate_x;
    s32 move_rate_z;
    s32 target_x;
    s32 target_y;
    s32 target_z;
    u8 unknown_44[0x11];
    u8 motion_flags;
    u8 unknown_56[3];
    u8 collision_flags;
    u8 select_flags;
    u8 unknown_5b[7];
    u8 busy;
    u8 unknown_63[9];
    void *update;
};

extern const u32 Data_02001d8c[];
extern const u8 Data_02001dd8[];
extern u8 gMapCellBuffer[];

struct VenusActor *Engine_ActorGet(s32 id);
s32 Engine_PartyGetLeaderActor(void);
void Engine_EventBegin(void);
void Engine_EventEnd(void);
void Engine_EventResolvePendingActions(s32 skip);
void Engine_EventWait(s32 frames);
void Engine_AudioPlayCue(s32 cue);
void Engine_GameFlagSet(s32 flag);
s32 GameFlag_Test(s32 flag);
void GameFlag_ClearBit(s32 flag);
void Engine_MapCopyCellsTo(s32 src_x, s32 src_y, s32 dest_x, s32 dest_y, s32 width, s32 height);
void WaitFrames(s32 frames);
void ObjectMotion_EnableActionAndSetCallback(s32 actor, const void *callback);
void Object_RefreshSelectorById(s32 actor);
void Object_SetPartAttribute(struct VenusActor *object, s32 value);
void ObjectMotion_SetSpeedParameters(s32 actor, s32 speed, s32 acceleration);
void ObjectMotion_SetPositionAndReset(s32 actor, s32 x, s32 z);
void Motion_SetModeAndWaitAnimation(s32 actor, s32 mode);
void Func_02001b2c(struct VenusActor *object, s32 mode);
void Func_02001b44(struct VenusActor *object, s32 x, s32 y, s32 z);
void Func_02001b4c(struct VenusActor *object);
s32 Func_02001b64(struct VenusActor *object, s32 *position);
void Func_02001c34(s32 actor, s32 height, s32 frames);
void Func_02001cbc(void);
s32 Func_020005a8(struct VenusActor *object);

static inline s32 Venus_FlagCheck(s32 flag)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    return GameFlag_Test(flag);
}

static inline void Venus_FlagSet(s32 flag)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Engine_GameFlagSet(flag);
}

static inline void Venus_CopyCells(s32 sx, s32 sy, s32 dx, s32 dy, s32 w, s32 h)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Engine_MapCopyCellsTo(sx, sy, dx, dy, w, h);
}

void Func_020006ac(void)
{
    s32 leader = Engine_PartyGetLeaderActor();
    struct VenusActor *actor = Engine_ActorGet(leader);
    s32 z;

    Engine_EventBegin();
    Engine_EventResolvePendingActions(0);
    ObjectMotion_EnableActionAndSetCallback(leader, Data_02001dd8);
    Object_RefreshSelectorById(leader);
    Object_SetPartAttribute(Engine_ActorGet(leader), 6);
    {
        /* FAKEMATCH: the native copies the leader into r0 before it builds the acceleration. */
        register s32 who __asm__("r0");

        actor->speed = 0x40000;
        who = leader;
        ObjectMotion_SetSpeedParameters(who, 0x40000, 0x20000);
    }
    if (*(s32 *)&actor->z_low >> 20 <= 54) {
        Engine_ActorGet(leader)->select_flags &= ~1;
        z = 210;
    } else {
        Engine_ActorGet(leader)->select_flags &= ~1;
        z = 238;
    }
    ObjectMotion_SetPositionAndReset(leader, actor->x_pixel, z * 4);
    Engine_EventWait(1);
    Engine_ActorGet(leader)->select_flags |= 1;
    Engine_EventWait(20);
    Object_SetPartAttribute(actor, 0);
    actor->update = Func_020005a8;
    Func_02001c34(leader, 258, 60);
    Motion_SetModeAndWaitAnimation(leader, 4);
    Object_SetPartAttribute(Engine_ActorGet(leader), 0);
    Motion_SetModeAndWaitAnimation(leader, 4);
    {
        void *none;

        /* FAKEMATCH: the one-pass block builds the null callback after
         * the wait. A plain unbound initializer hoists it into r5,
         * saves another register and differs 46 of 85 instructions.
         * Reusing z here still changes the store register (differ 4). */
        do {
            none = 0;
        } while (0);

        actor->update = none;
    }
    Engine_EventEnd();
}

void Func_0200078c(void)
{
    struct VenusActor *leader = Engine_ActorGet(Engine_PartyGetLeaderActor());
    s32 x = leader->x_pixel;
    s32 z = leader->z_pixel;

    if ((u32)(x - 676) > 7 || z < 788 || z >= 796) {
        Venus_CopyCells(53, 50, 42, 49, 1, 1);
        Venus_CopyCells(55, 117, 41, 117, 3, 5);
        GameFlag_ClearBit(0x201);
        leader->motion_flags |= 1;
        leader->terrain_height = 0;
        leader->y = 0;
    } else if (!Venus_FlagCheck(0x201)) {
        Engine_EventBegin();
        Engine_EventResolvePendingActions(0);
        Engine_EventWait(5);
        Venus_CopyCells(52, 50, 42, 49, 1, 1);
        Venus_CopyCells(52, 117, 41, 117, 3, 5);
        Venus_FlagSet(0x201);
        Engine_AudioPlayCue(161);
        leader->motion_flags &= ~1;
        leader->terrain_height = -0x20000;
        leader->y = -0x20000;
        Engine_EventEnd();
    }
}
