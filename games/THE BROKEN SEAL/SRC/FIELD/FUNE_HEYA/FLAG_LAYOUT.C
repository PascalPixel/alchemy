#include "TYPES.H"
#include "HEYA.H"
#include "FIELD_EVENT.H"
#include "CALL.H"

void FuneHeya_PlaceAnchorCharm(void);
void FieldScene_RunSceneStep(s32 step, s32 a, s32 b);
void OverlayObject_SetPositionAndHeading(s32 object, s32 x, s32 z, s32 heading);

void FuneHeya_RunWalkerStep();
void UpdateActorNineEffectMode();

struct Half {
    u16 v;
};

/* The zero is a one-halfword struct, so its pool load has a short reach and the pool lands mid-function as in the ROM. */
void FuneHeya_ApplyFlaggedLayout(void)
{
    struct FieldActor *actor;
    struct FieldActor *other;
    s32 done;
    s32 x;
    s32 heading;
    struct Half zero;

    if (!((s32 (*)())Engine_GameFlagIsSet)(0x911)) {
        FuneHeya_PlaceAnchorCharm();
        return;
    }
    if (Value1((s32 (*)())Engine_GameFlagIsSet, 0x928)) {
        FuneHeya_PlaceAnchorCharm();
    }
    done = ((s32 (*)())Engine_GameFlagIsSet)(0x93e);
    if (done != 0) {
        return;
    }
    if (((s32 (*)())Engine_GameFlagIsSet)(0x8a0)) {
        actor = Engine_ActorGet(9);
        FieldScene_RunSceneStep(13, 0, 0);
        OverlayObject_SetPositionAndHeading(8, 0x1c8, 0x28c, 0);
        x = 0x1e0;
        OverlayObject_SetPositionAndHeading(9, x, 0x258, 0xb000);
        Call3((void (*)())Engine_ActorSetSpeed, 9, 0xcccc, 0x6666);
        actor->unknown_66 = done;
        zero.v = 0;
        actor->rise_enabled = zero.v;
        actor->collision_flags |= 128;
        actor->update = (void (*)(union FieldObject *))&FuneHeya_RunWalkerStep;
        other = Engine_ActorGet(8);
        other->rise_counter = zero.v;
        other->update = (void (*)(union FieldObject *))&UpdateActorNineEffectMode;
        if (((s32 (*)())Engine_GameFlagIsSet)(0x109)) {
            OverlayObject_SetPositionAndHeading(0, x, 0x29a, 0xa000);
        }
    } else if (((s32 (*)())Engine_GameFlagIsSet)(0x928)) {
        OverlayObject_SetPositionAndHeading(8, 0x1bc, 0x266, 0xd000);
        FieldScene_RunSceneStep(13, 0, 0);
    } else if (((s32 (*)())Engine_GameFlagIsSet)(0x925)) {
        OverlayObject_SetPositionAndHeading(8, 0x1c8, 0x288, 0);
        FieldScene_RunSceneStep(13, 0, 0);
    } else if (((s32 (*)())Engine_GameFlagIsSet)(0x921)) {
        heading = 0xb000;
        OverlayObject_SetPositionAndHeading(8, 0x1db, 0x256, 0x8000);
        OverlayObject_SetPositionAndHeading(9, 0x1ce, 0x26a, heading);
        Engine_ActorGet(12)->facing = 0x3000;
        Engine_ActorGet(11)->facing = heading;
        OverlayObject_SetPositionAndHeading(13, 0x1b6, 0x293, 0xd000);
        OverlayObject_SetPositionAndHeading(10, 0x1e8, 0x2b0, heading);
    }
}
