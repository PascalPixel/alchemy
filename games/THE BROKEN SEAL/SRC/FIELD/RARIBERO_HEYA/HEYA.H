#ifndef HEYA_H
#define HEYA_H

#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"


/* The scene tables of the Lalivero house and sanctum, in the overlay's data.
 * The sanctum and the house share one overlay; flag 0x9a7 selects the later
 * placements and events. */
extern u8 gRariberoEntrances[];
extern u8 gRariberoSanctumRegions[];
extern u8 gRariberoRegions[];
extern u8 gRariberoExits[];
extern u8 gRariberoPlacements[];
extern u8 gRariberoHeyaPlacements[];
extern u8 gRariberoHeyaPlacements9a7[];
extern u8 gRariberoEvents[];
extern u8 gRariberoEvents9a7[];
extern u8 gRariberoHeyaEvents[];
extern u8 gRariberoHeyaEvents9a7[];

/* Main-image services reached through the overlay's import veneers, declared
 * old-style where call sites pass differing arguments. */
void Engine_ResetSceneEffectCounter(void);
void Owner_RefreshActiveRatios();
s32 Motion_LaunchFromFocusedObject();
void FieldScene_RunSecondaryScript();

/* The pose is the attached effect the actor carries. */
static __inline__ void ConfigureActorPose(s32 actor, s32 pose)
{
    Actor_SetAttachedEffect(actor, pose);
}

static __inline__ void Call1(void (*f)(), s32 a0)
{
    void Event_ShowMessage();

    f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    void Event_ShowMessage();

    f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    void Event_ShowMessage();

    f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    void Event_ShowMessage();

    f(a0, a1, a2, a3);
}

static __inline__ s32 Value0(s32 (*f)())
{
    void Event_ShowMessage();

    return f();
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    void Event_ShowMessage();

    return f(a0);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    void Event_ShowMessage();

    return f(a0, a1);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    void Event_ShowMessage();

    return f(a0, a1, a2);
}

static __inline__ s32 Value4(s32 (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    void Event_ShowMessage();

    return f(a0, a1, a2, a3);
}

#endif
