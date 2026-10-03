#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

#define NULL ((void *)0)
#define FIELD_AT_OFFSET(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

#include "CREATE_CONFIGURED_OVERLAY_OBJECT.H"
#include "CONFIGURED_EFFECT_SPAWN.H"

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

