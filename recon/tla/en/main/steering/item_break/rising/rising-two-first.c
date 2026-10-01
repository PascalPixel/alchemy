/* TLA-EN UpdateRisingParticleBurst: 224 emitted bytes versus the complete
   224-byte current native template; 34 differing byte positions after excluding
   the template's external relocation words. Calls and address pools are not
   linked in this comparison. This trial is uncredited and is not an adoption. */

#include "FIXED_MATH.H"
#include "EFFECT_0809B11C.H"
#include "SYSTEM.H"
#include "TYPES.H"
#include "OBJECT_EFX.H"
extern u8 Data_080f0e78;
#include "RAM_BUFFER.H"

#include "IWRAM_CALL.H"

u32 BattleFx_HasReachedTarget(struct EffectSlot *);

struct Output_08097f80 {
    s32 x;
    s32 y;
    s32 z;
};

/* LCG: seed = seed * 0x41c64e6d + 0x3039, returns bits 8-23. */
void Vector_AddPolarOffset(s32, s32, struct Output_08097f80 *);

struct BattleEffectScene {
    u8 pad00[16];
    void *volatile main_object;
};

extern struct BattleEffectScene *gEffectWork;
void BattleEffect_InitializeSharedScene(void);
void *BattleFx_StartItemBreak(void *object);
void BattleFx_SnapScaleToFull(void *object);
void Object_SetMode(void *object, s32 mode);
void BattleFx_PrepareBufferInterpolation(void);
void UpdateRisingParticleBurst(void *object);

extern void *Object_Spawn(s32, s32, s32, s32);
extern void Motion_SetTargetPositionFromMagnitudeAngle(
    struct Object_08096bec *object, s32 magnitude, s32 angle);
extern void Object_SetMode(void *, s32);
extern void Object_SetCallback(void *, void *);
extern void Func_080dcf8c(void *);

/* LCG: seed = seed * 0x41c64e6d + 0x3039, returns bits 8-23. */
extern void Audio_PlayCue(s32);
#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))
s32 Object_CommitPosition();

extern void Object_Destroy(void *object);

extern u8 gObjectSlots[];

struct FieldActor {
    u8 unknown_00[8];
    s32 x;
    s32 y;
    s32 z;
};

struct FieldPartyState {
    u8 unknown_000[500];
    s32 leader;
};

extern u8 *gEventWork;
extern struct FieldPartyState gGameState;
struct FieldActor *ObjectTable_Get(s32 index);

void UpdateRisingParticleBurst(void *source)
{
    /* FAKEMATCH: retained ordinary-source and scheduling trial; it remains an uncredited draft. */
    s32 count;
    s32 dist;
    u32 vel;
    void *child;
    /* FAKEMATCH: holds 0x10000 in sl across the burst loop as the ROM does */
    /* FAKEMATCH: retained item-break scheduling trial; see the measured limitation above. */
    register s32 unit asm("sl");

    s32 step = -0x800;
    /* FAKEMATCH: retained empty-asm value dependency to measure instruction scheduling. */
    asm volatile("" : "+r"(step));
    Audio_PlayCue(154);
    for (count = 30; count >= 0; count--) {
        s32 angle, turn, wait, scale;
        *(s32 *)((s8 *)source + 12) += 0x10000;
        angle = *(u16 *)((s8 *)source + 6);
    /* FAKEMATCH: retained empty-asm value dependency to measure instruction scheduling. */
        asm volatile("" : "+r"(angle));
        turn = 128;
        *(u16 *)((s8 *)source + 6) = angle + (turn << 6);
        scale = *(s32 *)((s8 *)source + 24);
    /* FAKEMATCH: retained empty-asm value dependency to measure instruction scheduling. */
        asm volatile("" : "+r"(scale));
        wait = 1;
        *(s32 *)((s8 *)source + 24) = scale + step;
        *(s32 *)((s8 *)source + 28) += step;
        WaitFrames(wait);
    }
    count = 7;
    unit = 0x10000;
    for (; count >= 0; count--) {
        child = Object_Spawn(0x2a1, *(s32 *)((s8 *)source + 8),
                             *(s32 *)((s8 *)source + 12),
                             *(s32 *)((s8 *)source + 16));
        if (child != 0) {
            Object_SetCallback(child, &Data_080f0e78);
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
