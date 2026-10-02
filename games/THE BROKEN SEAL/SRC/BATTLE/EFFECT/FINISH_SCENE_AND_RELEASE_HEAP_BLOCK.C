#include "TYPES.H"
#include "CALLBACK_SCHEDULER.H"

/* battle/effects/scene_transition/finish_and_release_heap_block.c */
#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

struct FxVector {
    s32 x;
    s32 y;
    s32 z;
};

struct FxSprite {
    u32 unknown_00[2];
    u8 unknown_08;
    u32 unknown_09_0 : 2;
    u32 mode : 2;               /* 0x09, bits 2-3 */
    u32 unknown_09_4 : 4;
    u8 unknown_0a[0x1c];
    u8 palette;                 /* 0x26 */
};

struct FxObject {
    u16 unknown_00[3];
    u16 angle;                  /* 0x06 */
    s32 x;
    s32 y;
    s32 z;
    u32 unknown_14[7];
    s32 scale_x;                /* 0x30 */
    s32 scale_y;
    u32 unknown_38[6];
    struct FxSprite *sprite;    /* 0x50 */
    u32 unknown_54 : 8;
    u32 visible : 8;            /* 0x55 */
    u32 unknown_56 : 16;
    u32 unknown_58[5];
    void *callback;             /* 0x6c */
};

/* The wave scene's work: two pages of per-line offsets, the hue the buffer
   is tinted with, and the two objects its bolts run between. */
struct WaveFxWork {
    u16 lines[2][162];
    u16 phase;
    u8 page;
    s8 blue;
    s8 green;
    s8 red;
    u8 unknown_28e[2];
    u16 source_id;
    u16 target_id;
    u8 delay;
    u8 counter;
};

extern struct WaveFxWork *gBattleBgFxWork;

void Animation_ApplyChildPalette(struct FxObject *object, s32 palette);
void Runtime_ReleaseHeapBlock(s32 a);
void Ui_SetBank15PaletteAndClearRenderMode(void);
struct FxObject *Object_GetById(s32 id);
void BattleFx_PrepareBufferInterpolation(void);
u32 __udivsi3(u32 numerator, u32 denominator);
s32 Trig_Sin(s32 angle);
void BattleFx_ApplyColorToTargetBuffer(s32 color, s32 mode);
void BattleFx_StartBufferInterpolation(s32 mode);
void BattleFx_AdvanceHueCycle(void);
s16 *BattleAction_FindDescriptor(s32 id);
s8 *Resource_GetMetadataRecordFar(s32 id);
struct FxObject *Object_CreateFar(s32 sprite, s32 x, s32 y, s32 z);
s32 ArcTan2(s32 y, s32 x);
void Object_SetMoveTargetFar(struct FxObject *object, s32 x, s32 y, s32 z);
void AudioCommand_PlayFar(s32 cue);
void BattleFx_SetCallbackWhenTargetUnset(void);
void BattleFx_UpdateWaveScene(void);

void BattleFx_FinishSceneAndReleaseHeapBlock(void)
{
    void *work;

    work = *(void **)((u32)&gBattleBgFxWork);
    Ui_SetBank15PaletteAndClearRenderMode();
    Scheduler_RemoveCallback((u32)BattleFx_UpdateWaveScene);
    Animation_ApplyChildPalette(Object_GetById(FIELD_AT_OFFSET(work, u16, 0x290)), 1);
    BattleFx_PrepareBufferInterpolation();
    Runtime_ReleaseHeapBlock(0x16);
}

/* The wave scene's frame callback: refills the hidden page of line offsets
   with a sine that advances four steps a frame and swaps the pages, steps
   the hue on every other frame, and on frames 0, 8 and 16 of each 61-frame
   round sends a bolt from the source object to the target, turned to face
   along the way. */
void BattleFx_UpdateWaveScene(void)
{
    struct WaveFxWork *work = gBattleBgFxWork;
    u16 *line;
    u32 i;
    struct FxObject *source;
    struct FxObject *target;
    struct FxObject *object;
    struct FxSprite *sprite;
    struct FxVector from;
    struct FxVector to;
    s8 *meta;

    if (work->delay != 0) {
        work->delay--;
        return;
    }

    line = work->lines[work->page ^ 1];
    for (i = 0; i < 160; i++)
        *line++ = Trig_Sin(__udivsi3((work->phase + i * 8) << 16, 160)) >> 14;
    work->phase += 4;
    work->page ^= 1;
    if (work->page != 0) {
        BattleFx_ApplyColorToTargetBuffer(
            (work->red << 10) | (work->green << 5) | work->blue | 0x200000, 1);
        BattleFx_StartBufferInterpolation(1);
        BattleFx_AdvanceHueCycle();
    }
    Animation_ApplyChildPalette(Object_GetById(work->source_id), 0);

    if (work->counter == 0 || work->counter == 8 || work->counter == 16) {
        source = Object_GetById(work->source_id);
        target = Object_GetById(work->target_id);
        if (source != 0 && target != 0) {
            from.x = source->x;
            meta = Resource_GetMetadataRecordFar(*BattleAction_FindDescriptor(work->source_id));
            from.y = source->y + (meta[8] << 16) - 0x20000;
            from.z = source->z;
            to.x = target->x;
            meta = Resource_GetMetadataRecordFar(*BattleAction_FindDescriptor(work->target_id));
            to.y = target->y + (meta[8] << 16) - 0x20000;
            to.z = target->z;
            object = Object_CreateFar(0x119, to.x, to.y, to.z);
            if (object != 0) {
                sprite = object->sprite;
                object->visible = 0;
                object->scale_x = 0xa3d7;
                object->scale_y = 0xa3d7;
                object->angle = ArcTan2(from.z - to.z, from.x - to.x);
                object->callback = BattleFx_SetCallbackWhenTargetUnset;
                sprite->palette = 0;
                sprite->mode = 1;
                Object_SetMoveTargetFar(object, from.x, from.y, from.z);
            }
        }
    }

    if (work->counter == 0)
        AudioCommand_PlayFar(130);
    if (++work->counter > 60)
        work->counter = 0;
}
