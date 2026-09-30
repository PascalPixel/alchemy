#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

#define NULL ((void *)0)

#include "STAGED_ACTOR_PAIR_SCENE.H"
#include "STAGED_ACTOR.H"

enum StagedPairMessage {
    MSG_WARRIORS_HAVE_BEEN_FIGHTING_WHILE = 0x23d2,
    MSG_WE_MISSED_COLOSSO_BECAUSE_WE = 0x23d5,
    MSG_IVE_BEEN_WAITING_FOR_ROBIN = 0x23d9,
    MSG_WHY_GOING_BACK_ROBIN_DO = 0x23da
};

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

struct HeightTrackedObject {
    u8 pad00[12];
    s32 height;                 /* +12 */
};

extern u8 LinkedMessage_DoYouWishCrossInto[];

/* Moves the next dialogue line on by amount messages. */
static __inline__ void bump_step(s32 amount)
{
    gEventWork->message += amount;
}

static __inline__ void PlaceActor(s32 actor, s32 x, s32 y)
{
    Actor_SetPosition(actor, x, y);
}

void SetEffectRecordMode(struct EffectWork *work, s32 mode)
{
    work->record->mode = mode;
}

void *StagedActorPairScene_SpawnPrimaryEffect(s32 x, s32 y, s32 z, s32 kind)
{
    u8 *effect = Object_Create(kind, x, y, z);

    if (effect != NULL) {
        u8 *sprite = *(u8 **)(effect + 0x50);
        s32 sprite_flags;
        s32 flag_mask = 13;

        sprite_flags = sprite[9];
        flag_mask = -flag_mask;
        flag_mask &= sprite_flags;
        sprite[9] = flag_mask;
        effect[0x55] = 0;
        effect[0x59] = 8;
        Actor_SetSpriteFlags(effect, 0);
        Object_SetPalette(effect, 14);
        Object_SetBlendMode(effect, 1);
        return effect;
    }
    return NULL;
}

void *StagedActorPairScene_SpawnSecondaryEffect(s32 x, s32 y, s32 z, s32 kind)
{
    u8 *effect = Object_Create(kind, x, y, z);

    if (effect != NULL) {
        u8 *sprite = *(u8 **)(effect + 0x50);
        s32 sprite_flags;
        s32 flag_mask = 13;

        sprite_flags = sprite[9];
        flag_mask = -flag_mask;
        flag_mask &= sprite_flags;
        flag_mask |= 4;
        sprite[9] = flag_mask;
        effect[0x55] = 0;
        effect[0x59] = 8;
        Actor_SetSpriteFlags(effect, 0);
        Object_SetPalette(effect, 15);
        effect[0x23] = (effect[0x23] & 0xfe) | 2;
        return effect;
    }
    return NULL;
}
