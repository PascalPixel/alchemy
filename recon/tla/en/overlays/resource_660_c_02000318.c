/* Draft, differ 4 of 50: before each of the two message openings the native builds the flags first (movs r1, #0; adds r0, r5); this form copies the actor first, as a direct call or through a forwarding inline. */
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

void Func_02000318(s32 actor)
{
    s32 message;

    Func_020007f4();
    Func_02000884(0);
    message = (s32)MsgScene660No01;
    Func_02000844(message);
    Message_Open(actor, 0);
    if (Inventory_PromptAndSetObjectMode(gPartyState.current_owner, 0) == 0) {
        Func_02000844(message + 2);
        Message_Open(actor, 0);
        if (Inventory_PromptAndSetObjectMode(gPartyState.current_owner, 0) == 0) {
            Func_0200085c(actor, 0);
        } else {
            ((struct EventWork1c4 *)Ram_HeapSlots->event_work)->count++;
            Func_0200085c(actor, 0);
        }
    } else {
        Func_0200085c(actor, 0);
    }
    Func_020007fc();
}
