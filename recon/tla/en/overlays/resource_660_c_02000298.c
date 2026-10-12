/* Draft, differ 4 of 47: the native reloads the owner for the turn (ldr r1, [r6]) before it masks the facing, sign-extends the facing after the turn call, and builds the quarter turn 0x4000 around the actor id (movs r1; movs r0, #8; lsls r1); this form extends before the call. */
#include "types.h"

#include "ram_buffer.h"
#include "object_runtime.h"
#include "party_state.h"
#include "scene_ids.h"
#include "scene_call.h"
#include "text/msg_ids.h"

struct EventWork1c4 {
    u8 unknown_000[0x1c4];
    u16 count;
};

extern const u8 Data_0200094c[];
extern const u8 Data_02000964[];
extern const u8 Data_02000ab4[];
extern const u8 Data_02000b2c[];
extern const u8 Data_02000b5c[];
extern const u8 Data_02000b80[];
extern const u8 Data_02000d18[];
extern const u8 Data_02000de4[];
extern const u8 Data_02000e44[];
extern const u8 Data_02000fe8[];

struct ObjectRuntime *Object_GetById(s32 id);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Motion_SetVarCbAndRefresh(s32 id, s32 value);
void Motion_SetModeAndWaitAnimation(s32 id, s32 mode);
void Func_020007bc(void);
void Func_020007cc(void);
void Func_020007d4(void);
void Func_020007e4(s32 message, s32 a, s32 b);
void Func_020007f4(void);
void Func_020007fc(void);
void Func_02000844(s32 message);
void Func_02000854(s32 id, s32 a, s32 b);
void Func_0200085c(s32 id, s32 flags);
void Func_02000884(s32 a);
s32 Func_0200088c(void);
void Func_0200089c(s32 slot, s32 actor);
void Func_020008a4(s32 id);
void Func_020008ac(s32 slot, s32 actor);
void Func_020008b4(s32 cue);

static __inline__ void Actor_Turn(s32 id, s32 angle, s32 frames)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    ObjectMotion_ArmCallback(id, angle, frames);
}

static __inline__ void Message_Open(s32 speaker, s32 flags)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    UiText_OpenMessageAtObject(speaker, flags);
}

void Func_02000298(void)
{
    struct ObjectRuntime *leader = Object_GetById(gPartyState.current_owner);
    s16 facing;

    /* FAKEMATCH: the one-pass block keeps the angle load before the constant (differ 2 without it). */
    do {
        facing = (leader->angle + 0x2000) & ~0x3fff;
    } while (0);
    ObjectMotion_SetAngleToward(8, gPartyState.current_owner, 0);
    if ((u16)facing == 0xc000) {
        Func_020008a4(8);
    } else {
        if (GameFlag_Test(0x91b))
            Func_02000844((s32)MsgScene660No66);
        else
            Func_02000844((s32)MsgScene660No31);
        Func_0200085c(8, 0);
    }
    Actor_Turn(8, 0x4000, 0);
}
