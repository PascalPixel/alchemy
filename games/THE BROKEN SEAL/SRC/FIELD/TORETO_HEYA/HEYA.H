#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

#define NULL ((void *)0)
#define FIELD_AT_OFFSET(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

#include "CREATE_CONFIGURED_OVERLAY_OBJECT.H"
#include "CONFIGURED_EFFECT_SPAWN.H"

/* Shared 22-byte head leaf proved identical for this overlay family. */
struct EffectRecord {
    u8 pad[9];
    u8 flags_lo : 2;
    u8 mode : 2;
    u8 flags_hi : 4;
};

struct EffectWork {
    u8 pad[80];
    struct EffectRecord *record;
};

/*
 * Per-frame integrator for an effect record -- resource_396. It adds the
 * velocity at +68/+72/+76 into the position at +8/+12/+16, decays two of the
 * three velocities, accumulates the rates at +48/+52, and turns the sprite.
 */

extern s16 *ToretoHeya_PaletteBuffer;
extern u8 ToretoHeya_ActionTable2[];
extern u8 ToretoHeya_ActionTable1[];
extern u8 ToretoHeya_SparkOrigin[];
extern u8 ToretoHeya_SparkCounter[];

void ToretoHeya_HandleFloorSwitch(int, int, int, int);
void ToretoHeya_PlayGesture();
void PartyInventory_FindOwner();
void Object_SetActionCallbackAndRefreshById();
void ToretoHeya_SpawnSwirlSparks();
void ToretoPalette_ApplyTint(void);
void ToretoHeya_AdvanceEffectMotion();




/* Return the overlay's first persistent workspace block. */

/* Return the overlay's second persistent workspace block. */

/* Return the overlay's third persistent workspace block. */

/* Return the overlay's fourth persistent workspace block. */

/* Return the overlay's presentation workspace block. */

/* Clear the active story variant once the scene controller is idle. */

/*
 * A loader-relocated call word rather than a runtime address; it turns a value
 * and a duration into a per-frame step. Declared without a prototype, and the
 * call site passes two arguments.
 */

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}
