/* TLA-EN BattleFx_SnapScaleToFull: 42 emitted bytes versus the complete
   44-byte current native template; 10 differing byte positions after excluding
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

void BattleFx_SnapScaleToFull(void *obj)
{
    /* FAKEMATCH: retained ordinary-source and scheduling trial; it remains an uncredited draft. */
    s32 next;
    s32 scale;
    /* FAKEMATCH: Probe the existing loop-bound carrier independently of its comparison. */
    /* FAKEMATCH: retained item-break scheduling trial; see the measured limitation above. */
    register s32 bound asm("ip");

    if (obj != NULL) {
        scale = FIELD_AT_OFFSET(obj, s32, 0x18);
        if (scale <= 0xFFFF) {
            bound = 0xffff;
            /* FAKEMATCH: Probe the loop-bound carrier at the branch boundary. */
    /* FAKEMATCH: retained item-break scheduling trial; see the measured limitation above. */
            asm("" : "+r"(bound) : "r"(scale));
            do {
                next = scale + 0x1000;
                scale = next;
            } while (next <= bound);
            FIELD_AT_OFFSET(obj, s32, 0x18) = next;
            FIELD_AT_OFFSET(obj, s32, 0x1C) = next;
        }
        Object_CommitPosition();
    }
}
