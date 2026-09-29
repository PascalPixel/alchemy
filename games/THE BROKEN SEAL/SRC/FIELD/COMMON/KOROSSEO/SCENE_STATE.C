/* Colosso: store a halfword into the stage's scene state, the block the
 * engine's work slot 59 points at. The same function sits in each of the
 * three Colosso trial overlays. */
#include "TYPES.H"

/* The engine's work blocks, one pointer per slot. */
extern u8 gWorkSlot[];

enum {
    WORK_SLOT_STAGE = 59
};

void SceneState_SetWorkHalfwordDc(s16 v)
{
    *(s16 *)(*(u8 **)(gWorkSlot + WORK_SLOT_STAGE * 4) + 0xdc) = v;
}
