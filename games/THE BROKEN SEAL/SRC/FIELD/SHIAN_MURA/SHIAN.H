#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

#define NULL ((void *)0)

#include "SCENE_EFFECT_SEQUENCE.H"
#include "IWRAM_CALL.H"

enum SpawnSequenceMessage {
    MSG_YOUNG_WARRIORS_VERY_GALLANT_CAME = 0x17e8,
    MSG_WARRIORS_FROM_SCHOOL_STRONG_WARRIORS = 0x17f7,
    MSG_DOING_MADE_ME_SPILL_MY = 0x17fa,
    MSG_NOW_MUST_GET_WATER_AGAIN = 0x17fb
};

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

extern u32 Data_03001e40;
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

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call0(void (*f)())
{
    f();
}

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

/* Moves the next dialogue line on by amount messages. */
static __inline__ void bump_step(s32 amount)
{
    gEventWork->message += amount;
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}
