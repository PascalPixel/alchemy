// Near miss, differ 4 of 169 (tla-en, 2026-10-30): overlay 6ad's copy of the
// landing burst drafted in resource_698_c_02001674.c, with this overlay's
// symbols; resource_6ab_c_02003e04.c says what is left: the velocity array's
// address (add r6, sp, #16) is built once before the loop, where the native
// builds it after the cosine call. The gcse dump shows PRE hoisting it
// (PRE/HOIST: edge (0,1)) and the loop dump shows loop then moving the
// in-loop copy out (insn 170, life 38). Tried 30 October: a loop entered at
// its bottom test (goto test; body: ...; test: if (i <= 16) goto body;)
// leaves the address after the cosine call in r6 exactly as the native has
// it, PRE finding nothing to hoist, but keeps the entry branch (b; differ 13
// of 170, 25 with i unpinned); as a goto do-while, cse2 replaces the in-loop
// copy with the hoisted register (differ 4).
#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "field_effect.h"

struct Actor *Actor_GetById(s32 id);
void Func_02005560(void);
void Func_02005648(s32 value);
void Camera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 mode);
void Func_020054b0(void);
void wait(s32 frames);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_020056c0(s32 sound);
void FieldEvent_Wait(s32 frames);
s32 Math_Cosine(s32 angle);
s32 Math_Sine(s32 angle);
u32 Random16Far(void);
void Effect_AdvanceMotion(union FieldObject *object);
void Func_020035b8(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags, struct EffectOptions *options);
void Func_020055c0(s32 id, s32 value);
void Func_02005510(s32 a, s32 b, s32 c);
void Func_02005518(void);
void Func_02005568(void);

static __inline__ void CamBounds(s32 x, s32 y, s32 z, s32 mode) {
  Camera_MoveWithinMapBounds(x, y, z, mode);
}

static __inline__ void SetWorkTriple(s32 a, s32 b, s32 c) {
  Func_02005510(a, b, c);
}

void Func_02003d80(void) {
  struct Actor *actor = Actor_GetById(gMainRoutineWork.currentOwner);
  /* FAKEMATCH(pin): i in r10, which leaves r8 to the options pointer; as a
   * plain local, 22 of 177 instructions differ in tla-en. */
  register u32 i asm("r10");
  struct EffectOptions options;

  Func_02005560();
  Func_02005648(0);
  CamBounds(-1, -1, -1, 0);
  Func_020054b0();
  wait(1);
  actor->y = 0x820000;
  actor->gravity = 0x8000;
  actor->bounce = 0;
  actor->motionFlags = 0;
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 0x100;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Func_020056c0(204);
  actor->motionFlags = 3;
  FieldEvent_Wait(24);
  options.palette = 7;
  options.update = Effect_AdvanceMotion;
  options.startScaleX = 0xa3d7;
  options.startScaleY = 0xa3d7;
  i = 0;
  do {
    s32 angle;
    s32 velocity[3];

    angle = i << 12;
    velocity[0] = Math_Cosine(angle);
    velocity[1] = 0;
    velocity[2] = Math_Sine(angle);
    velocity[0] += velocity[0] >> 1;
    velocity[0] += (Random16Far() * 0xc000 >> 16) - 0x6000;
    velocity[2] += (Random16Far() * 0x6000 >> 16) - 0x3000;
    Func_020035b8(actor->x, actor->y, actor->z, velocity[0], velocity[1],
                  velocity[2], 0x1090001, &options);
    i++;
  } while (i <= 16);
  Func_020056c0(188);
  Func_020055c0(gMainRoutineWork.currentOwner, 0x101);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 49);
  SetWorkTriple(0x50000, 0x50000, 0x10000);
  SetWorkTriple(-1, -1, 0xe666);
  Func_02005518();
  Func_020055c0(gMainRoutineWork.currentOwner, 0x100);
  actor->gravity = 0x10000;
  actor->bounce = 0x4000;
  FieldEvent_Wait(10);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 1);
  Func_02005568();
}
