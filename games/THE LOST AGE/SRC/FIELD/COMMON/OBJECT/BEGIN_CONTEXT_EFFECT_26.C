#include "TYPES.H"
#include "OBJECT_LOOKUP.H"
#include "SYSTEM.H"

union EffectMotionSlot {
    u32 word;
    struct {
        u8 unknown0[2];
        u8 active;
        u8 unknown3;
    } bytes;
};

struct EffectMotionObject {
    u8 unknown0[5];
    u8 kind;
    u8 unknown6[2];
    u32 x;
    u8 unknownC[4];
    u32 y;
    u8 unknown14[16];
    union EffectMotionSlot slot24;
    u8 unknown28[4];
    u32 field2C;
    u8 unknown30[8];
    u32 field38;
    u8 unknown3C[4];
    u32 field40;
    u8 unknown44[12];
    struct EffectMotionObject *context;
};

struct EffectKindObject {
    u8 unknown0[5];
    u8 kind;
};

extern u32 gGameState[];

/* Object table: 192 pointers at gEventWork + 0x14 (see ObjectTable_Get). */
void *ResourceMetadata_RegisterFar(void *, s32);
void Object_SetMode(void *, s32);

void ObjectEffect_PrepareContextEffect(s32 value);

s32 GameFlag_SetBit(s32);
void ObjectEffect_PrepareContextEffect(s32);

void ObjectEffect_BeginContextEffect26(void)
{
    ObjectEffect_PrepareContextEffect(0x1A);
    GameFlag_SetBit(0x120);
}

void ObjectEffect_BeginContextEffect25(void);

typedef struct {
    u8 unknown00[38];
    u8 first_flag;
    u8 second_flag;
    u8 unknown28[4];
    void *eff;
} EffectCleanupContext;

typedef struct {
    u8 unknown00[8];
    s32 x;
    s32 y;
    s32 z;
    u8 unknown14[28];
    s32 speed30;
    s32 speed34;
    u8 unknown38[24];
    EffectCleanupContext *ctx;
} EffectCleanupObject;

void ResourceMetadata_ClearRecordFar(void *);
void Object_SetPosition(EffectCleanupObject *, s32, s32, s32);
void Object_CommitPosition(EffectCleanupObject *);

void ObjectEffect_EndContextEffect(s32 arg0);

s32 GameFlag_TestFar(s32);
void GameFlag_ClearBitFar(s32);
void ObjectEffect_EndContextEffect(s32 arg0);
void Motion_CamBounds(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void Audio_PlayCue(s32);
void Battle_WaitMode0(s32 arg0);
void Object_AttachWorkTargetToObject(s32 arg0, s32 arg1);

s32 ObjectEffect_RunPendingFlagEvent(void);
