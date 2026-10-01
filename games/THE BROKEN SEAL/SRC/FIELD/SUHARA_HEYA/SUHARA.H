#ifndef SUHARA_H
#define SUHARA_H

#include "TYPES.H"
#include "FIELD_SERVICE.H"
#include "FIELD_SCENE.H"
#include "SCENE.H"



#define STEP_COUNTER (*(u16 *)(*(u8 **)&gEventWork + 0x1d8))

struct SceneRecord {
    u8 unk_00[9];
    u8 flags;
    u8 unk_0a[20];
    u16 angle;
    u8 unk_20[6];
    u8 field_26;
};

/*
 * Byte and field views of one actor share this union so that stores through
 * the linked record do not make the pointer read disappear through alias
 * analysis.
 */
union SceneActor {
    struct {
        u8 unk_00[12];
        s32 y;
        u8 unk_10[64];
        struct SceneRecord *record;
        u8 unk_54;
        u8 mode;
        u8 unk_56[3];
        u8 flags;
    } fields;
    u8 bytes[90];
};

/* The room's scene tables, in the overlay's data; flag 0x96f selects the
 * later placements and events. */
extern u8 gSuharaHeyaEntrances[];
extern u8 gSuharaHeyaRegions[];
extern u8 gSuharaHeyaExits[];
extern u8 gSuharaHeyaPlacements[];
extern u8 gSuharaHeyaPlacements96f[];
extern u8 gSuharaHeyaEvents[];
extern u8 gSuharaHeyaEvents96f[];

/* The imports only this room calls, each behind its inline service. */
void Engine_AudioPlayCueFromEventWork(void);
static inline void Audio_PlayCueFromEventWork(void)
{
    /* FAKEMATCH: scenes reach every engine service through its inline; a direct call lets GCC 2.96 share costly constant arguments between calls. */
    Engine_AudioPlayCueFromEventWork();
}

void Engine_InnCheckIn(s32 first, s32 second);
static inline void Inn_CheckIn(s32 first, s32 second)
{
    /* FAKEMATCH: scenes reach every engine service through its inline; a direct call lets GCC 2.96 share costly constant arguments between calls. */
    Engine_InnCheckIn(first, second);
}

void Engine_MotionLaunchFromFocusedObject(s32 first, s32 second, s32 third, s32 fourth);
static inline void Motion_LaunchFromFocusedObject(s32 first, s32 second, s32 third, s32 fourth)
{
    /* FAKEMATCH: scenes reach every engine service through its inline; a direct call lets GCC 2.96 share costly constant arguments between calls. */
    Engine_MotionLaunchFromFocusedObject(first, second, third, fourth);
}

void Engine_ShopConfirmAct(s32 first);
static inline void Shop_ConfirmAct(s32 first)
{
    /* FAKEMATCH: scenes reach every engine service through its inline; a direct call lets GCC 2.96 share costly constant arguments between calls. */
    Engine_ShopConfirmAct(first);
}

#endif
