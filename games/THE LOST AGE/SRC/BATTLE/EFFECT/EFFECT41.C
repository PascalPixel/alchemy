#include "TYPES.H"
#include "OBJECT_RUNTIME.H"
#include "OBJDISP.H"
#include "OBJECT_EFX.H"

u32 Random16(void);
void RunBattleEffect05(void);

/* The fields of a falling effect object this step reads. */
struct FallingObject {
    u8 unknown_00[0xc];
    s32 y;
    u8 unknown_10[4];
    s32 floor;
    u8 unknown_18[0x46];
    s16 dispatch_state;
    u8 unknown_60[0xc];
    void *update;
};

/* Once the object reaches its floor, hands it to the common particle
   script and stops this step. */
void BattleFx_LandFallingObject(struct FallingObject *object)
{
    if (object->y <= object->floor) {
        object->dispatch_state = 2;
        ObjectDispatch_InitializeFar((struct DispatchObject *)object,
            (u32)BattleFx_CommonParticleScript);
        object->update = 0;
    }
}

void BattleFx_UpdateDriftingFallObject(struct ObjectRuntime *object)
{
    s32 r;

    object->y = (s32)((u32)object->y - 0x4ccc);
    r = Random16();
    object->x += r - Random16();
    if (object->y <= object->terrain_height) {
        ObjectDispatch_InitializeFar((struct DispatchObject *)object,
            (u32)BattleFx_CommonParticleScript);
    }
}

/* Effect 5 by its entry in the effect dispatch. */
void BattleFx_CallEffect05(void)
{
    RunBattleEffect05();
}
