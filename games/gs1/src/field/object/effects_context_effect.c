#include "types.h"
#include "object_lookup.h"

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

extern u32 Data_02000240[];

/* Object table: 192 pointers at Data_03001ebc + 0x14 (object/table/get.c). */
void *Func_08009048(void *, s32);
void Object_SetMode(void *, s32);
void WaitFrames(s32);

void ObjectEffect_PrepareContextEffect(s32 value)
{
    u32 zero;
    u8 kind;
    struct EffectMotionObject *object;
    struct EffectMotionObject *context;
    struct EffectKindObject *effect;

    object = ObjectTable_Get(Data_02000240[125]);
    context = object->context;
    effect = Func_08009048(context, 27);
    zero = 0;
    kind = 15;

    context->slot24.bytes.active = zero;
    effect->kind = kind;
    object->x = (object->x & 0xFFF00000) + 0x80000;
    object->y = (object->y & 0xFFF00000) + 0x100000;
    object->slot24.word = zero;
    object->field2C = zero;
    object->field38 = 0x80000000;
    object->field40 = 0x80000000;
    Object_SetMode(object, value);
    WaitFrames(18);
}

s32 GameFlag_Set(s32);
void ObjectEffect_PrepareContextEffect(s32);

void ObjectEffect_BeginContextEffect26(void)
{
    ObjectEffect_PrepareContextEffect(0x1A);
    GameFlag_Set(0x120);
}


void ObjectEffect_BeginContextEffect25(void)
{
    ObjectEffect_PrepareContextEffect(0x19);
    GameFlag_Set(0x121);
}

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

void Func_08009060(void *);
void Object_SetPosition(EffectCleanupObject *, s32, s32, s32);
void Object_CommitPosition(EffectCleanupObject *);


void ObjectEffect_EndContextEffect(s32 arg0)
{
    s32 zero;
    s32 mask;
    EffectCleanupObject *obj = ObjectTable_Get(Data_02000240[125]);
    EffectCleanupContext *ctx = obj->ctx;
    struct EffectKindObject *eff = Func_08009048(ctx, 27);

    zero = 0;
    mask = 0xfff00000;
    *(u8 *)((u8 *)ctx + 38) = zero;
    eff->kind = 15;
    obj->x = (obj->x & mask) + 0x80000;
    obj->z &= mask;
    Object_SetMode(obj, arg0);
    WaitFrames(30);
    ctx->second_flag = 1;
    Func_08009060(ctx->eff);
    ctx->eff = (void *)zero;
    *(u8 *)((u8 *)ctx + 38) = 1;
    obj->speed34 = 0x10000;
    obj->speed30 = 0x10000;
    Object_SetPosition(obj,
        obj->x, obj->y, obj->z + 0x80000);
    Object_CommitPosition(obj);
}

s32 GameFlag_IsSet(s32);
void GameFlag_Clear(s32);
void Func_08094380(s32 arg0);
void Func_080933f8(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void Audio_PlayCue(s32);
void Func_0809163c(s32 arg0);
void Func_0809335c(s32 arg0, s32 arg1);

s32 ObjectEffect_RunPendingFlagEvent(void)
{
    s32 result = 0;
    s32 flag = 0x120;

    if (GameFlag_IsSet(flag)!= 0) {
        Func_08094380(24);
        GameFlag_Clear(flag);
        result = 1;
    } else {
        flag = 0x121;
        if (GameFlag_IsSet(flag)!= 0) {
            Func_08094380(23);
            GameFlag_Clear(flag);
            result = 2;
        } else {
            flag = 0x122;
            if (GameFlag_IsSet(flag)!= 0) {
                s32 id;
                void *obj;

                GameFlag_Clear(flag);
                id = Data_02000240[125];
                obj = ObjectTable_Get(id);
                *(s32 *)((u8 *)obj + 12) += 0x00a00000;
                Func_080933f8(-1, -1, -1, 0);
                while (*(s32 *)((u8 *)obj + 12) + *(s32 *)((u8 *)obj + 40) >
                       *(s32 *)((u8 *)obj + 20)) {
                    WaitFrames(1);
                }
                Audio_PlayCue(159);
                *(s32 *)((u8 *)obj + 12) = *(s32 *)((u8 *)obj + 20);
                Object_SetMode(obj, 22);
                Func_0809163c(15);
                Func_0809335c(id, 1);
                result = 3;
            }
        }
    }
    return result;
}
