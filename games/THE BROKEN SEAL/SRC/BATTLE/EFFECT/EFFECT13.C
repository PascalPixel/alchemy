#include "MOTION_OBJECT.H"
#include "FX_SCENE.H"
#include "FIXED_MATH.H"
#include "OBJDISP.H"
#include "EFFECT_SLOT.H"
#include "SYSTEM.H"
#include "TYPES.H"
#include "OBJECT_RUNTIME.H"
#include "GAME_STATE.H"
#include "OBJECT_EFX.H"
#include "SOUND_IDS.H"
#include "GLOBAL_CELLS.H"
#include "IWRAM_CALL.H"

u32 BattleFx_HasReachedTarget(struct EffectSlot *);

struct Output_08097f80 {
    s32 x;
    s32 y;
    s32 z;
};

/* LCG: seed = seed * 0x41c64e6d + 0x3039, returns bits 8-23. */
void Vector_AddPolarOffset(s32, s32, struct Output_08097f80 *);


extern struct BattleFxScene *gEffectWork;
void BattleEffect_InitializeSharedScene(void);
void *BattleFx_StartItemBreak(void *object);
void BattleFx_SnapScaleToFull(struct MotionObject *object);
void Object_SetMode(void *object, s32 mode);
void BattleFx_PrepareBufferInterpolation(void);
void UpdateRisingParticleBurst(void *object);

extern void *Object_Spawn(s32, s32, s32, s32);
extern void Motion_SetTargetPositionFromMagnitudeAngle(
    struct ObjectRuntime *object, s32 magnitude, s32 angle);
extern void Object_SetMode(void *, s32);
extern void BattleFx_UpdateItemBreakFragment(void *);

/* LCG: seed = seed * 0x41c64e6d + 0x3039, returns bits 8-23. */
extern void Audio_PlayCue(s32);
#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))
s32 Object_CommitPosition();

extern void Object_Destroy(void *object);

extern u8 gObjectSlots[];

extern u8 *gEventWork;

void BattleFx_UpdateOrbitAndReturn(struct EffectSlot *effect)
{
    struct Output_08097f80 position;
    s8 *state_pointer = &effect->state;
    s32 state;

next_state:
    state = *state_pointer;
    if (state == 0) {
        u32 angle;

        position.x = effect->origin_x;
        position.z = effect->origin_z;
        angle = Random16();
        Vector_AddPolarOffset(0x1e0000, (u16)angle, &position);
        effect->target_x = position.x;
        effect->target_z = position.z;
        effect->acceleration = 0x40000;
        effect->max_speed = 0x40000;
        effect->flag42 = state;
        (*state_pointer)++;
        return;
    }

    if (state == 1) {
        if (BattleFx_HasReachedTarget(effect) == 0) {
            (*state_pointer)++;
            goto next_state;
        }
        return;
    }

    if (state == 2) {
        effect->target_x = effect->origin_x;
        effect->target_z = effect->origin_z;
        {
            u32 value = 0x400;

            effect->max_turn_step = value;
        }
        effect->flag42 = 1;
        (*state_pointer)++;
        return;
    }

    if (state == 3 && BattleFx_HasReachedTarget(effect) == 0)
        BattleFx_ClearOwnedSlot(effect);
}

void BattleFx_RunItemBreakSequence(void)
{
    s32 work[3];
    struct BattleFxScene *scene;
    void *object;

    scene = gEffectWork;
    object = scene->main_object;

    /* FAKEMATCH: removing this one-pass block changes instruction scheduling. */
    do {
        BattleEffect_InitializeSharedScene();
    } while (0);
    object = BattleFx_StartItemBreak(object);
    BattleFx_SnapScaleToFull(object);
    if (object != 0) {
        Object_SetMode(object, 4);
        WaitFrames(30);
    }
    BattleFx_PrepareBufferInterpolation();
    UpdateRisingParticleBurst(object);
}

void *BattleFx_StartItemBreak(void *source)
{
    s32 angle;
    s32 fragment_height;
    s32 fragment_scale;
    s32 fragment_count;
    u32 vel;
    u32 rotation_jitter;
    s32 zero;
    void *parent;
    void *child;

    angle = (((struct MotionObject *)source)->angle + 0x2000) & 0xc000;
    parent = Object_Spawn(0xd7, *(s32 *)((s8 *)source + 8),
                           ((struct MotionObject *)source)->y + 0x100000,
                           *(s32 *)((s8 *)source + 16));
    if (parent == 0)
        return 0;
    *(s32 *)((s8 *)parent + 0x1c) = 0x4000;
    *(s32 *)((s8 *)parent + 0x18) = 0x4000;
    *(s32 *)((s8 *)parent + 0x6c) = (s32)BattleFx_UpdateItemBreakFragment;
    *(s32 *)((s8 *)parent + 0x30) = 0x20000;
    *(s32 *)((s8 *)parent + 0x34) = 0x20000;
    zero = 0;
    *(s8 *)((s8 *)parent + 0x55) = zero;
    Object_SetMode(parent, 3);
    Motion_SetTargetPositionFromMagnitudeAngle(parent, 0x100000, angle);

    fragment_count = 7;
    do {
        child = Object_Spawn(0x11d, *(s32 *)((s8 *)source + 8),
                              ((struct MotionObject *)source)->y + 0x100000,
                              *(s32 *)((s8 *)source + 16));
        if (child != 0) {
            ObjectDispatch_InitializeFar((struct DispatchObject *)child, (u32)&BattleFx_FragmentScript);
            fragment_scale = Random16() + 0x10000;
            *(s32 *)((s8 *)child + 0x34) = 0x10000;
            *(s32 *)((s8 *)child + 0x30) = fragment_scale;
            *(s8 *)((s8 *)child + 0x55) = 2;
            *(s32 *)((s8 *)child + 0x48) = 0x51e;
            vel = Random16();
            *(s32 *)((s8 *)child + 0x28) = vel - Random16();
            fragment_height = Random16() * 0x18 + 0x80000;
            rotation_jitter = Random16();
            Motion_SetTargetPositionFromMagnitudeAngle(
                child, fragment_height,
                          ((rotation_jitter - Random16()) >> 3) +
                          ((struct MotionObject *)source)->angle);
        }
        fragment_count--;
    } while (fragment_count >= 0);
    Audio_PlayCue(SOUND_ITEM_BREAK);
    return parent;
}

void BattleFx_SnapScaleToFull(struct MotionObject *obj)
{
    s32 next;
    s32 scale;

    if (obj != NULL) {
        scale = obj->scale_x;
        if (scale <= 0xFFFF) {
            do {
                next = scale + 0x1000;
                scale = next;
            } while (next <= 0xFFFF);
            obj->scale_x = next;
            obj->scale_y = next;
        }
        Object_CommitPosition();
    }
}

/* UpdateRisingParticleBurst: lift and spin the source for 31 frames, then
   burst eight item-break fragments with random speeds and headings. */
void UpdateRisingParticleBurst(void *source)
{
    s32 count;
    s32 dist;
    u32 vel;
    void *child;
    /* FAKEMATCH: holds 0x10000 in sl across the burst loop as the ROM does */
    register s32 unit asm("sl");

    Audio_PlayCue(154);
    for (count = 30; count >= 0; count--) {
        ((struct MotionObject *)source)->y += 0x10000;
        ((struct MotionObject *)source)->angle += 0x2000;
        ((struct MotionObject *)source)->scale_x += -0x800;
        ((struct MotionObject *)source)->scale_y += -0x800;
        WaitFrames(1);
    }
    count = 7;
    unit = 0x10000;
    for (; count >= 0; count--) {
        child = Object_Spawn(0x11d, *(s32 *)((s8 *)source + 8),
                             ((struct MotionObject *)source)->y,
                             *(s32 *)((s8 *)source + 16));
        if (child != 0) {
            ObjectDispatch_InitializeFar((struct DispatchObject *)child, (u32)&BattleFx_FragmentScript);
            {
                s32 speed = Random16();

                *(s32 *)((s8 *)child + 0x34) = unit;
                *(s32 *)((s8 *)child + 0x30) = speed + unit;
            }
            *(s8 *)((s8 *)child + 0x55) = 2;
            *(s32 *)((s8 *)child + 0x48) = 0xa3d;
            vel = Random16();
            *(s32 *)((s8 *)child + 0x28) = vel - Random16();
            dist = Random16() * 0x18;
            dist += 0x80000;
            Motion_SetTargetPositionFromMagnitudeAngle(child, dist, Random16());
        }
    }
    Audio_PlayCue(131);
    Object_Destroy(source);
}

void ObjectDispatch_ApplyValueToKind200Children(int arg0)
{
  s32 cnt;
  u8 *p;
  u8 kind;
  void *child;
  void *rec;
  void *obj;
  obj = *((void **)((u32)&gObjectSlots));
  cnt = 0x3F;
  do
  {
    if (1)
    {
      if ((*((s32 *)(((u8 *)obj) + 0))) != 0)
      {
        kind = *(p = (u8 *)(((u8 *)obj) + 0x54));
        if (kind == 1)
        {
          child = *((void **)(((u8 *)obj) + 0x50));
          rec = *((void **)(((u8 *)child) + 0x28));
          if ((*((s16 *)(((u8 *)rec) + 0))) == 0xC8)
          {
            *((s8 *)(((u8 *)rec) + 5)) = arg0;
            *((u8 *)(((u8 *)child) + 0x25)) = kind;
          }
        }
      }
      cnt -= 1;
    }
    obj += 0x70;
  }
  while (cnt >= 0);
}

/* Counts down the scene timer while it runs and raises request 0x2090 once
   the leader strays 60 units from the anchor or the timer runs out. */
void FieldEffect_WatchLeaderDistance(void)
{
    u8 *work;
    struct ObjectRuntime *actor;
    s16 *timer;
    s32 dx;
    s32 dz;

    work = gEventWork;
    actor = ObjectTable_Get(gGameState.selected_actor);
    if (*(s16 *)(work + 0xcc0) != 0) {
        timer = (s16 *)(work + 0xcba);
        if (*timer != 0)
            (*timer)--;
    }
    dx = Iwram_MulQ16(*(s16 *)(work + 0xcbc) - actor->x / 0x10000, 0xd105);
    dz = *(s16 *)(work + 0xcbe) - (actor->z - actor->y) / 0x10000;
    if (dx * dx + dz * dz >= 3600 || *(s16 *)(work + 0xcba) == 0) {
        u16 *slot = (u16 *)(work + 382);
        u32 request = 0x2090;

        *slot = request;
    }
}
