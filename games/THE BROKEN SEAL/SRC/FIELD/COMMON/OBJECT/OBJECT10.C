#include "TYPES.H"
#include "OBJECT_RUNTIME.H"
#include "EVENT_RUNTIME.H"
#include "OBJECT_LOOKUP.H"
#include "SYSTEM.H"
#include "DMA.H"
#include "GAME_STATE.H"
#include "SCROLL.H"

union EffectMotionSlot {
    u32 word;
    struct {
        u8 unknown0[2];
        u8 active;
        u8 unknown3;
    } bytes;
};

struct EffectMotionContext {
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
    struct EffectMotionContext *context;
};

struct EffectKindObject {
    u8 unknown0[5];
    u8 kind;
};

/* Object table: 192 pointers at gEventWork + 0x14 (see ObjectTable_Get). */
void *ResourceMetadata_RegisterFar(void *, s32);
void Object_SetMode(void *, s32);
s32 GameFlag_SetBitFar(s32);
void ObjectEffect_PrepareContextEffect(s32);

typedef struct {
    u8 unknown00[38];
    u8 first_flag;
    u8 second_flag;
    u8 unknown28[4];
    void *eff;
} EffectCleanupContext;

void ResourceMetadata_ClearRecordFar(void *);
void Object_SetPosition(struct ObjectRuntime *, s32, s32, s32);
void Object_CommitPosition(struct ObjectRuntime *);
s32 GameFlag_TestFar(s32);
void GameFlag_ClearBitFar(s32);
void ObjectEffect_EndContextEffect(s32 arg0);
void Motion_CamBounds(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void Audio_PlayCue(s32);
void Battle_WaitMode0(s32 arg0);
void Object_AttachWorkTargetToObject(s32 arg0, s32 arg1);

extern struct EventRuntime *gEventWork;
void DisplayTransition_Finish(s32 mode, s32 frames);

/* Plays the scene's cue and two sound effects, whitens one palette colour
   (colour 243 in mode 3, the backdrop otherwise), finishes the display
   transition, then fades that colour from white to black over 16 frames.
   It returns no value, but its epilogue is the value-returning one. */
s32 Scene_FadeColorFromWhite(void)
{
    struct EventRuntime *work = gEventWork;
    s32 i;
    s32 c;

    Audio_PlayCue(gGameState.scene_cue);
    Audio_PlayCue(288);
    Audio_PlayCue(147);
    if (work->mode_19e == 3) {
        s32 color = 0x7fff;

        do {
            /* FAKEMATCH: the do/while (0) ends a scheduling region, which keeps
               this store ahead of the call's arguments as the reference has it */
            *(u16 *)0x050001e6 = color;
        } while (0);
        DisplayTransition_Finish(0x401, 16);
        work->status_1c6 = 0;
        WaitFrames(16);
        for (i = 0; i < 16; i++) {
            c = 30 - i * 2;
            color = (c << 10) | (c << 5) | c;
            *(u16 *)0x050001e6 = color;
            WaitFrames(1);
        }
    } else {
        s32 color = 0x7fff;

        *(u16 *)0x05000000 = color;
        DisplayTransition_Finish(0x207, 16);
        work->status_1c6 = 0;
        WaitFrames(16);
        for (i = 0; i < 16; i++) {
            c = 30 - i * 2;
            color = (c << 10) | (c << 5) | c;
            *(u16 *)0x05000000 = color;
            WaitFrames(1);
        }
    }
}

void ObjectEffect_PrepareContextEffect(s32 value)
{
    u32 zero;
    u8 kind;
    struct ObjectRuntime *object;
    struct EffectMotionContext *context;
    struct EffectKindObject *effect;

    object = ObjectTable_Get(gGameState.selected_actor);
    context = object->animation;
    effect = ResourceMetadata_RegisterFar(context, 27);
    zero = 0;
    kind = 15;

    context->slot24.bytes.active = zero;
    effect->kind = kind;
    object->x = (object->x & 0xFFF00000) + 0x80000;
    object->z = (object->z & 0xFFF00000) + 0x100000;
    object->velocity_x = zero;
    object->velocity_z = zero;
    object->target_x = 0x80000000;
    object->target_z = 0x80000000;
    Object_SetMode(object, value);
    WaitFrames(18);
}

void ObjectEffect_BeginContextEffect26(void)
{
    ObjectEffect_PrepareContextEffect(0x1A);
    GameFlag_SetBitFar(0x120);
}

void ObjectEffect_BeginContextEffect25(void)
{
    ObjectEffect_PrepareContextEffect(0x19);
    GameFlag_SetBitFar(0x121);
}

void ObjectEffect_EndContextEffect(s32 arg0)
{
    s32 zero;
    s32 mask;
    struct ObjectRuntime *obj = ObjectTable_Get(gGameState.selected_actor);
    EffectCleanupContext *ctx = obj->animation;
    struct EffectKindObject *eff = ResourceMetadata_RegisterFar(ctx, 27);

    zero = 0;
    mask = 0xfff00000;
    *(u8 *)((u8 *)ctx + 38) = zero;
    eff->kind = 15;
    obj->x = (obj->x & mask) + 0x80000;
    obj->z &= mask;
    Object_SetMode(obj, arg0);
    WaitFrames(30);
    ctx->second_flag = 1;
    ResourceMetadata_ClearRecordFar(ctx->eff);
    ctx->eff = (void *)zero;
    *(u8 *)((u8 *)ctx + 38) = 1;
    obj->acceleration = 0x10000;
    obj->speed_limit = 0x10000;
    Object_SetPosition(obj,
        obj->x, obj->y, obj->z + 0x80000);
    Object_CommitPosition(obj);
}

s32 ObjectEffect_RunPendingFlagEvent(void)
{
    s32 result = 0;
    s32 flag = 0x120;

    if (GameFlag_TestFar(flag)!= 0) {
        ObjectEffect_EndContextEffect(24);
        GameFlag_ClearBitFar(flag);
        result = 1;
    } else {
        flag = 0x121;
        if (GameFlag_TestFar(flag)!= 0) {
            ObjectEffect_EndContextEffect(23);
            GameFlag_ClearBitFar(flag);
            result = 2;
        } else {
            flag = 0x122;
            if (GameFlag_TestFar(flag)!= 0) {
                s32 id;
                struct ObjectRuntime *obj;

                GameFlag_ClearBitFar(flag);
                id = gGameState.selected_actor;
                obj = ObjectTable_Get(id);
                obj->y += 0x00a00000;
                Motion_CamBounds(-1, -1, -1, 0);
                while (obj->y + obj->velocity_y >
                       obj->terrain_height) {
                    WaitFrames(1);
                }
                Audio_PlayCue(159);
                obj->y = obj->terrain_height;
                Object_SetMode(obj, 22);
                Battle_WaitMode0(15);
                Object_AttachWorkTargetToObject(id, 1);
                result = 3;
            }
        }
    }
    return result;
}

void DisplayScroll_ArmHBlankDma(void)
{
    volatile u32 *dma;
    struct DisplayScrollWork *state;
    u32 *source;
    u16 status;
    u32 control;
    volatile u32 *destination;

    state = gHBlankScrollWork;
    source = (u32 *)state->rows[state->page];

    dma = (volatile u32 *)0x040000b0;
    destination = (volatile u32 *)0x04000014;
    status = *(volatile u16 *)((u8 *)dma + 10);
    control = 0xc5ff & status;
    *(volatile u16 *)((u8 *)dma + 10) = control;
    status = *(volatile u16 *)((u8 *)dma + 10);
    control = 0x7fff & status;
    *(volatile u16 *)((u8 *)dma + 10) = control;
    (void)*(volatile u16 *)((u8 *)dma + 10);

    *destination = *source++;
    *destination = *source++;
    *destination = *source++;

    control = 0xa6600003;
    Dma_Set(source, destination, control, dma);
}
