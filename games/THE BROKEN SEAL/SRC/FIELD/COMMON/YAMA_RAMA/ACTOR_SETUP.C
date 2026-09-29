#include "TYPES.H"

/* Actor callbacks that open the mountain overlay. */

#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

struct EventActor {
    u8 reserved_00[0x23];
    u8 flags;
    u8 reserved_24[0x2c];
    u8 *render_state;
};

s16 ArcTan2(s32, s32);
void *Object_GetById(s32);

s32 EventScript_PrepareActorRenderFlags(struct EventActor *actor)
{
    actor->flags &= ~1;
    actor->render_state[9] |= 0xc;
    actor->render_state[21] |= 0xc;
    return 0;
}

s32 OverlayObject_SetFacingTowardObject10(void *self)
{
    void *obj;

    obj = Object_GetById(0xA);
    FIELD_AT_OFFSET(self, s16 *, 6) = ArcTan2(FIELD_AT_OFFSET(obj, s32 *, 0x10) - FIELD_AT_OFFSET(self, s32 *, 0x10), FIELD_AT_OFFSET(obj, s32 *, 8) - FIELD_AT_OFFSET(self, s32 *, 8));
    return 0;
}
