/* TLA-EN BattleFx_UpdateOrbitAndReturn: 170 emitted bytes versus the complete
   172-byte current native template; 2 differing byte positions after excluding
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

void BattleFx_UpdateOrbitAndReturn(struct EffectSlot *effect)
{
    /* FAKEMATCH: retained ordinary-source and scheduling trial; it remains an uncredited draft. */
    struct Output_08097f80 position;
    s8 *state_pointer = &effect->state;
    s32 state;

next_state:
    state = *state_pointer;
    if (state == 0) {
        u32 angle;
        s8 *next;
        s32 x;

        position.x = effect->origin_x;
        position.z = effect->origin_z;
        angle = Random16();
        Vector_AddPolarOffset(0x1e0000, (u16)angle, &position);
        next = state_pointer;
        x = position.x;
    /* FAKEMATCH: retained item-break scheduling trial; see the measured limitation above. */
        asm("" : "+r"(next) : "r"(x));
        effect->target_x = x;
        effect->target_z = position.z;
        effect->acceleration = 0x40000;
        effect->max_speed = 0x40000;
        effect->flag42 = state;
        (*next)++;
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
    /* FAKEMATCH: retained item-break scheduling trial; see the measured limitation above. */
        register s8 *flag asm("r2");
        s32 x;
        do { x = effect->origin_x; } while (0);
        flag = (s8 *)effect;
    /* FAKEMATCH: retained empty-asm value dependency to measure instruction scheduling. */
        asm volatile("" : "+r"(flag) : "r"(x));
        effect->target_x = x;
        {
            s32 z;
            do { z = effect->origin_z; } while (0);
            flag += 66;
    /* FAKEMATCH: retained empty-asm value dependency to measure instruction scheduling. */
            asm volatile("" : "+r"(flag) : "r"(z));
            effect->target_z = z;
        }
        {
            u32 value = 0x400;

            effect->max_turn_step = value;
        }
        *flag = 1;
        (*state_pointer)++;
        return;
    }

    if (state == 3 && BattleFx_HasReachedTarget(effect) == 0)
        BattleFx_ClearOwnedSlot(effect);
}
