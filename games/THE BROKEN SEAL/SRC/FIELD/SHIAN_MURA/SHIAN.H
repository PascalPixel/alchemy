#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

#define NULL ((void *)0)

#include "SCENE_EFFECT_SEQUENCE.H"
#include "IWRAM_CALL.H"

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

struct PeriodicEffectConfig {
    s32 kind;
    s32 variant;
    u8 pad_08[16];
    u16 id;
    u8 pad_1a[2];
    u8 *data;
    /*
     * The record is 40 bytes, not 32: the reference reserves `sub sp, #56' for
     * 16 bytes of outgoing stack arguments plus this object, and only writes
     * +0, +4, +24 and +28.  The trailing two words are never stored.
     */
    u8 pad_20[8];
};

extern u8 ShianMura_EffectConfig[];
extern u8 gShianMuraEntrances[];
extern u8 gShianMuraExits[];
extern u8 gShianMuraEvents[];
extern u8 ShianMura_ActionTable[];

/* The village's callbacks, installed by address. */
s32 ShianMura_WatchGateTrigger();
void Actor_UpdatePresentationFlag(void);
extern u8 gShianMuraPlacements[];
extern u8 ShianMura_GateSteps1[];
extern u8 ShianMura_GateSteps2[];
extern u8 ShianMura_GateSteps3[];
extern u8 ShianMura_GateSteps4[];
extern u8 ShianMura_GateSteps5[];

void BattleFx_RunPageEffectForSlot();
void Effect_Spawn();
void BattleFx_PlayQueuedSound();

/* Moves the next dialogue line on by amount messages. */
static __inline__ void bump_step(s32 amount)
{
    gEventWork->message += amount;
}

