#ifndef KORIMAKI_H
#define KORIMAKI_H

#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

#define NULL ((void *)0)
#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

#include "PALETTE_SCENE.H"


struct PaletteEffectFrame {
    s32 pad00[6];
    s32 progress;
    s32 pad1c[7];
    s32 limit;
    s32 second_limit;
    s32 third_limit;
};

struct EffectSprite { u8 pad00[9]; u8 flags; u8 pad0a[28]; u8 state; };

struct PaletteEffect {
    u8 pad00[0x18];
    s32 progress;
    u8 pad1c[7];
    u8 flags;
    u8 pad24[12];
    s32 rate_x;
    s32 rate_y;
    u8 pad38[24];
    struct EffectSprite *sprite;
    u8 pad54[1];
    u8 mode;
};

struct OrbitingPaletteEffect {
    s32 pad00[2];
    s32 x;
    s32 y;
    s32 z;
    s32 pad14;
    s32 angle_x;
    s32 angle_y;
    s32 pad20[6];
    s32 anchor_x;
    s32 anchor_y;
    s32 anchor_z;
    s32 pad44[3];
    u8 *owner;
    s32 pad54[4];
    s16 step;
    s16 heading;
};

struct PaletteSceneRecord {
    u8 pad00[216];
    u16 values[1];                 /* +216 */
};

/* The tree's scene tables, in the overlay's data. */
extern u8 gKorimaKiEntrances[];
extern u8 gKorimaKiRegions[];
extern u8 gKorimaKiExits[];
extern u8 gKorimaKiPlacements[];
extern u8 gKorimaKiEvents[];

/* The overlay's own work, past its image: where the sparks gather, how long
 * they have run, whether each spawn sounds, and the transition's step. */
extern s32 gKorimaKiSparkOrigin[3];
extern s32 gKorimaKiSparkCount;
extern s32 gKorimaKiSparkSound;
extern s32 gKorimaKiTransitionStep;
void PaletteScene_SpawnEffect(void);
extern const s32 SceneAction_ActorOneEntry[];
extern const s32 SceneAction_ActorTwoEntry[];
extern const s32 SceneAction_ActorThreeEntry[];
extern const s32 SceneAction_GroupFinish[];
extern u8 gKorimaKiEffectScript[];

/* Main-image services reached through the overlay's import veneers, declared
 * old-style where call sites pass differing arguments. */
void Battle_ResetEffectCounter();
void Object_RefreshSelectorById();
void Object_SetActionCallbackAndRefreshById();
void Vector_AddPolarOffset();
void Resource_ResetEntry();
s32 PartyInventory_FindOwner();
s32 Inventory_Find();
struct PaletteSceneRecord *Owner_GetState();
void Audio_PlayCueFromEventWork();

/* The tree's own scenes, callbacks and palette helpers. */
void KorimaKi_PlayGesture(s32 actor, s32 gesture);
void KorimaKi_SpawnOrbitSparks(void);
void KorimaPalette_SaveFirst(void);
void KorimaPalette_SaveSecond(void);
void KorimaPalette_Capture(void);
void KorimaPalette_Restore(s32 second);
void PaletteScene_AdvanceTransition(void);

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

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}


#endif
