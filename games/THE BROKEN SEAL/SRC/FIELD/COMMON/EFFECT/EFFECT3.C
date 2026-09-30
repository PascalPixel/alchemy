#include "EFFECT_STEP.H"
#include "IWRAM_CALL.H"
#include "TYPES.H"
#include "GLOBAL_CELLS.H"

s32 Render_ProjectPoint(s32 *, s32 *);
void BattleMotion_ProjectScaledPositionFar(s32, struct EffectPosition *);
void BattleMotion_ProjectPositionFar(s32, struct EffectPosition *);

extern u8 *gCameraWork;
s32 **GetBattleObjectSlotFar(s32 unit);
u8 *GetMotionRecordFar(s32 *object, s32 mode);
s32 Battle_GetObjectTableValueFar(s32 unit);
void Render_ResetTransformState(void);
void Graphics_PrepareTransferInIwramWork(u8 *source, u8 *destination);
s32 Render_ProjectPoint(s32 *point, s32 *screen);

extern u8 gBattleFxWork[];
extern u8 Data_03001ae8[];

void *Runtime_ReleaseHeapBlock(s32);
s32 Runtime_AllocateHeapBlock(s32 arg0, s32 arg1);
void *Runtime_AllocateBlock(s32 arg0, s32 arg1);
void *BattlePres_RunBeamSequence(s32 *);
void *Unnamed_080e40a4(s32 *);
void *BattleFx_RunCastingImpact(s32 *);

s32 BattleUnit_ProjectToScreen(s32 unit, s32 *screen);

void EffectStep_AdvanceWithGravity3D(struct EffectStep *step, s32 damping, s32 gravity)
{
    step->x = (s32)((u32)step->x + (u32)step->velocity_x);
    step->y = (s32)((u32)step->y + (u32)step->velocity_y);
    step->z = (s32)((u32)step->z + (u32)step->velocity_z);
    step->velocity_y = (s32)((u32)step->velocity_y + (u32)gravity);
    step->velocity_x = (s32)((u32)step->velocity_x * (u32)damping) / 64;
    step->velocity_y = (s32)((u32)step->velocity_y * (u32)damping) / 64;
    step->velocity_z = (s32)((u32)step->velocity_z * (u32)damping) / 64;
}

void EffectStep_AdvanceWithGravity2D(struct EffectStep *step, s32 damping, s32 gravity)
{
    step->x = (s32)((u32)step->x + (u32)step->velocity_x);
    step->y = (s32)((u32)step->y + (u32)step->velocity_y);
    step->velocity_y = (s32)((u32)step->velocity_y + (u32)gravity);
    step->velocity_x = (s32)((u32)step->velocity_x * (u32)damping) / 64;
    step->velocity_y = (s32)((u32)step->velocity_y * (u32)damping) / 64;
}

s32 EffectPosition_ApplyBaseAndYOffset(s32 *point, struct EffectPosition *position)
{
    s32 result = Render_ProjectPoint(point, &position->x);
    position->y = (s32)((u32)position->y - 0x10);
    return result;
}

void EffectPosition_ApplyAnimationAndYOffset(s32 id, struct EffectPosition *position)
{
    BattleUnit_ProjectToScreen(id, position);
    position->y = (s32)((u32)position->y - 0x10);
}

void EffectPosition_ApplyStepAndYOffset(s32 id, struct EffectPosition *position)
{
    BattleMotion_ProjectScaledPositionFar(id, position);
    position->y = (s32)((u32)position->y - 0x10);
}

void EffectPosition_ApplyAlternateStepAndYOffset(s32 id, struct EffectPosition *position)
{
    BattleMotion_ProjectPositionFar(id, position);
    position->y = (s32)((u32)position->y - 0x10);
}

/* Projects a battle unit's position to the screen and lifts it by its
   scaled height. */
s32 BattleUnit_ProjectToScreen(s32 unit, s32 *screen)
{
    u8 *camera;
    s32 *object;
    u8 *info;
    s32 scale;
    /* FAKEMATCH: an unused vector reproduces the reference's 12-byte frame. */
    s32 unused[3];

    camera = gCameraWork;
    object = *GetBattleObjectSlotFar(unit);
    info = GetMotionRecordFar(object, 0);
    Render_ResetTransformState();
    Graphics_PrepareTransferInIwramWork(camera, camera + 12);
    scale = Iwram_MulQ16(Render_ProjectPoint(object + 2, screen), *(s32 *)(info + 24));
    screen[1] -= Iwram_MulQ16(scale, Battle_GetObjectTableValueFar(unit) >> 17);
    return 0;
}

void ObjectGroup_ProbeKeysWhenField24High(void)
{
    u8 *state = *(u8 **)((u32)&gBattleFxWork);
    u8 *object = *(u8 **)(state + 0x7828);

    if (*(s16 *)(object + 0x24) > 0x7f)
        (void)*(volatile s32 *)((u32)&Data_03001ae8);
}

void BattleFx_DispatchByIdRange(s32 *arg0)
{
  s32 no;
  s32 tmp;
  tmp = (tmp = 0x60E);
  Runtime_AllocateBlock(0x29, tmp);
  Runtime_AllocateHeapBlock(0x27, 0x782C);
  Runtime_AllocateHeapBlock(0x28, 0x4000);
  tmp = *arg0;
  no = tmp;
  tmp = no - 0x64;
  if (((u32)tmp) <= 0x23U)
  {
    BattleFx_RunCastingImpact(arg0);
  } else
    if (no > 0xC7)
  {
    Unnamed_080e40a4(arg0);
  } else
  {
    BattlePres_RunBeamSequence(arg0);
  }
  Runtime_ReleaseHeapBlock(0x28);
  Runtime_ReleaseHeapBlock(0x27);
  Runtime_ReleaseHeapBlock(0x29);
}
