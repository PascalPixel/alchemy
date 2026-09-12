#include "object_lookup.h"
#include "types.h"

typedef struct {
    u8 unknown00[5];
    u8 kind;
} Effect08094380;

typedef struct {
    u8 unknown00[38];
    u8 first_flag;
    u8 second_flag;
    u8 unknown28[4];
    void *eff;
} Context08094380;

typedef struct {
    u8 unknown00[8];
    s32 x;
    s32 y;
    s32 z;
    u8 unknown14[28];
    s32 speed30;
    s32 speed34;
    u8 unknown38[24];
    Context08094380 *ctx;
} Object08094380;

Effect08094380 *Func_08009048(Context08094380 *, s32);
void Object_SetMode(Object08094380 *, s32);
void WaitFrames(s32);
void Func_08009060(void *);
void Object_SetPosition(Object08094380 *, s32, s32, s32);
void Object_CommitPosition(Object08094380 *);

extern s32 Data_02000240[];

void ObjectEffect_EndContextEffect(s32 arg0)
{
    s32 zero;
    s32 mask;
    Object08094380 *obj = ObjectTable_Get(Data_02000240[125]);
    Context08094380 *ctx = obj->ctx;
    Effect08094380 *eff = Func_08009048(ctx, 27);

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
