#include "PROTO.H"
#include "TYPES.H"
#include "FX_SCENE.H"
#include "GAME_STATE.H"
#include "FIXED_MATH.H"
#include "MOTION_OBJECT.H"

extern struct BattleFxScene *gEffectWork;

struct MotionObject *Object_GetById(s32 id);
void BattleFx_AdvanceSpinAngle(struct MotionObject *object);

int BattleFx_ClearActiveSlotsAndScheduleUpdates();
int ObjectMotion_ArmCallback();
int ObjectMotion_Launch();

void BattleFx_FinishHeavyImpact(s32 id)
{
    struct {
        s32 x;
        s32 y;
        s32 z;
    } pos;
    struct MotionObject *object;
    struct BattleFxScene *scene;
    struct EffectSlot *slot;
    s32 scale;
    s32 count;

    object = Object_GetById(id);
    /* FAKEMATCH: removing this one-pass block changes instruction scheduling. */
    do {
        if (object == NULL)
            return;
        BattleFx_InitializeSlots();
        scene = gEffectWork;
        Shop_RestoreSceneTilesFar(0x20118c);
        Audio_PlayCue(0xad);
        Motion_SetVarCbAndRefresh(id, 1);
    } while (0);
    Audio_PlayCue(0xae);
    Motion_SetVarCbAndRefresh(id, 1);
    /* FAKEMATCH: removing this one-pass block changes instruction scheduling. */
    do {
        Audio_PlayCue(0xaf);
        Motion_SetVarCbAndRefresh(id, 1);
        WaitFrames(20);
        Audio_PlayCue(0x8c);
        object->update = BattleFx_AdvanceSpinAngle;
        WaitFrames(40);
        Audio_PlayCue(0x99);
        ObjectMotion_Launch(id, 12, 22);
        pos.x = object->x;
        pos.y = object->y;
        pos.z = object->z;
        Camera_WorldToScreen(&pos);
        ObjectDispatch_ReleaseFar(object);
        Audio_PlayCue(0xa4);
        slot = scene->slots;
        count = 23;
        do {
            EffectSlot_Initialize(slot, 0x11c, pos.x, pos.z);
            EffectSlot_SetCallback(slot, BattleFx_RunAngledApproachPhases);
            EffectSlot_SetObjectMode(slot, 7);
            ObjectGroup_SetChildValueUnlessFifteenFar(slot->object, (u32)(Random16() * 7) >> 16);
            scale = (u32)Random16() / 3 + 0x10000;
            slot->scale_y = scale;
            slot->scale_x = scale;
            count--;
            WaitFrames(1);
            slot++;
        } while (count >= 0);
    } while (0);
    WaitFrames(60);
    ObjectMotion_ArmCallback(gGameState.selected_actor, 0x4000, 0);
    WaitFrames(20);
    Object_SetMode(Object_GetById(gGameState.selected_actor), 28);
    WaitFrames(40);
    Audio_PlayCue(0xa4);
    WaitFrames(100);
    Shop_InitEffectFar();
    BattleFx_ClearActiveSlotsAndScheduleUpdates();
}
