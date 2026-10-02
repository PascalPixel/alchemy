/* DRAFT: 548 of 548 bytes, every instruction present; one pair is swapped.
   After ArcTan2 the ROM has `orrs r3, r2; strh r0, [r6, #6]` and this build
   stores the angle first. The post-reload scheduler ranks the two equally
   here because the angle store depends on the three vector loads for the
   last call: FxObject's alias set has int members, as the vector does. In
   the ROM that store does not depend on them (while the scale stores do).
   What fixed the rest (2026-10-02): Object_CreateFar takes all three
   coordinates, so to.x's reload finds r0 to r3 busy and brings r4 into the
   spill set; the pooled 0 for the palette is what the compiler gives a byte
   zero stored after a halfword register store, with no local needed. */
#include "TYPES.H"

struct FxVector {
    s32 x;
    s32 y;
    s32 z;
};

struct FxSprite {
    u8 unknown_00[9];
    u8 unknown_09_0 : 2;
    u8 mode : 2;
    u8 unknown_09_4 : 4;
    u8 unknown_0a[0x1c];
    u8 palette;
};

struct FxObject {
    u16 unknown_00[3]; u16 angle;
    s32 x;
    s32 y;
    s32 z;
    u32 unknown_14[7];
    s32 scale_x;
    s32 scale_y;
    u32 unknown_38[6];
    struct FxSprite *sprite;
    u32 unknown_54 : 8;
    u32 visible : 8;
    u32 unknown_56 : 16; u32 unknown_58[5];
    void *callback;
};

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
extern u8 Sound_MaxLines[];

u32 __udivsi3(u32 numerator, u32 denominator);
s32 Trig_Sin(s32 angle);
void BattleFx_ApplyColorToTargetBuffer(s32 color, s32 mode);
void BattleFx_StartBufferInterpolation(s32 mode);
void BattleFx_AdvanceHueCycle(void);
struct FxObject *Object_GetById(s32 id);
void Animation_ApplyChildPalette(struct FxObject *object, s32 palette);
s16 *BattleAction_FindDescriptor(s32 id);
s8 *Resource_GetMetadataRecordFar(s32 id);
struct FxObject *Object_CreateFar(s32 sprite, s32 x, s32 y, s32 z);
s32 ArcTan2(s32 y, s32 x);
void Object_SetMoveTargetFar(struct FxObject *object, s32 x, s32 y, s32 z);
void AudioCommand_PlayFar(s32 cue);
void BattleFx_SetCallbackWhenTargetUnset(void);

void Func_08097644(void)
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
        BattleFx_ApplyColorToTargetBuffer((work->red << 10) | (work->green << 5) | work->blue | 0x200000, 1);
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
