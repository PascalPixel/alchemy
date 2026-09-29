/* Place one deck slot actor: unless told to keep its value, take the phase
   its mode selects and copy that drifting slot's look; then set its depth
   and its sprite's tilt from the sine of its value, on the near or the far
   side of its resting depth. */
#include "TYPES.H"
#include "FIELD_EVENT.H"

extern u16 FuneKanpan_SlotPhase[];
extern u16 FuneKanpan_SlotValue[];
extern u32 FuneKanpan_SlotMode[];
extern s32 FuneKanpan_SlotDepth[];

void ObjectVisual_CopyAttributes(s32 actor, s32 source);

struct SlotSprite {
    s32 unknown_00[7];
    s16 unknown_1c;
    s16 tilt;
};

void SceneEffect_SelectSlotValueAndPosition(s32 id, s32 slot, s32 flags)
{
    struct FieldActor *actor = Engine_ActorGet(id);
    struct SlotSprite *sprite = (struct SlotSprite *)actor->sprite;
    s32 sine;

    if ((flags & 2) == 0) {
        switch (FuneKanpan_SlotMode[slot]) {
        case 1:
            FuneKanpan_SlotValue[slot] = FuneKanpan_SlotPhase[0];
            ObjectVisual_CopyAttributes(id, 8);
            break;
        case 2:
            FuneKanpan_SlotValue[slot] = FuneKanpan_SlotPhase[1];
            ObjectVisual_CopyAttributes(id, 9);
            break;
        case 3:
            FuneKanpan_SlotValue[slot] = FuneKanpan_SlotPhase[2];
            ObjectVisual_CopyAttributes(id, 10);
            break;
        case 4:
            FuneKanpan_SlotValue[slot] = FuneKanpan_SlotPhase[3];
            ObjectVisual_CopyAttributes(id, 11);
            break;
        }
    }
    if (flags & 1) {
        sine = Engine_MathSin(FuneKanpan_SlotValue[slot]);
        sprite->tilt = Engine_MathSin(FuneKanpan_SlotValue[slot] + 0x8000) >> 5;
        actor->z.fixed = FuneKanpan_SlotDepth[slot] - (sine << 2) - (sine << 1);
    } else {
        sine = Engine_MathSin(FuneKanpan_SlotValue[slot] + 0x8000);
        sprite->tilt = Engine_MathSin(FuneKanpan_SlotValue[slot]) >> 5;
        actor->z.fixed = FuneKanpan_SlotDepth[slot] + (sine << 2) + (sine << 1);
    }
}
