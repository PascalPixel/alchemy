/* Draft, differ 41 of 51: the native loads the party base (r1) and the first room id (r4) once before the flag branch and shares one return of Data_02000e44 between the two arms (cmp r2, r4; beq into the other arm). This form shares the return through a label; two plain returns are differ 37 and duplicate it. */
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

const u8 *Func_020004f0(void)
{
    s32 moved = GameFlag_Test(0x91b);
    struct PartyState *party = &gPartyState;
    s32 room1 = (s32)&SceneId_Ov660Room1;

    if (moved) {
        if (party->scene == room1)
            goto arrived;
        if (party->scene == (s32)&SceneId_Ov660Room2)
            return Data_02000fe8;
    } else {
        if (party->scene == room1) {
            if (party->entrance != 99)
                return Data_02000b80;
arrived:
            return Data_02000e44;
        }
        if (party->scene == (s32)&SceneId_Ov660Room2)
            return Data_02000d18;
    }
    if (party->scene == (s32)&SceneId_Ov660Room3)
        return Data_02000de4;
    return Data_02000b5c;
}
