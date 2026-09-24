/* Draft, not exact (2026-09-24): 92 differing halfwords, 544 of 548 bytes.
   Written from the listing. The palette clear goes through a u8 local zero
   so its short-reach pool constant splits the literal pool where the ROM
   has it. Remaining: the reference uses r4 for most short-lived constants
   and hi-register copies (r0 here), loads 0x28b from the pool instead of
   deriving it from 0x28d, and is 4 bytes longer. */
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
    u8 unknown_00[6];
    u16 angle;
    s32 x;
    s32 y;
    s32 z;
    u8 unknown_14[0x1c];
    s32 scale_x;
    s32 scale_y;
    u8 unknown_38[0x18];
    struct FxSprite *sprite;
    u8 unknown_54;
    u8 visible;
    u8 unknown_56[0x16];
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

extern struct WaveFxWork *Data_03001ea8;
extern u8 Data_00000000[];

u32 Math_DivU(u32 numerator, u32 denominator);
s32 Trig_Sin(s32 angle);
void BattleFx_ApplyColorToTargetBuffer(s32 color, s32 mode);
void BattleFx_StartBufferInterpolation(s32 mode);
void BattleFx_AdvanceHueCycle(void);
struct FxObject *Object_GetById(s32 id);
void Animation_ApplyChildPalette(struct FxObject *object, s32 palette);
s16 *BattleAction_FindDescriptor(s32 id);
s8 *Resource_GetMetadataRecordFar(s32 id);
struct FxObject *Object_CreateFar(s32 sprite, s32 x);
s32 ArcTan2(s32 y, s32 x);
void Object_SetPosition(struct FxObject *object, s32 x, s32 y, s32 z);
void Audio_PlayCue(s32 cue);
void BattleFx_SetCallbackWhenTargetUnset(void);

void Func_08097644(void)
{
    struct WaveFxWork *work = Data_03001ea8;
    u16 *line;
    u32 i;
    struct FxObject *source;
    struct FxObject *target;
    struct FxObject *object;
    struct FxSprite *sprite;
    struct FxVector from;
    struct FxVector to;
    s8 *meta;
    u8 zero;

    if (work->delay != 0) {
        work->delay--;
        return;
    }

    line = work->lines[work->page ^ 1];
    for (i = 0; i < 160; i++)
        *line++ = Trig_Sin(Math_DivU((work->phase + i * 8) << 16, 160)) >> 14;
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
            object = Object_CreateFar(0x119, to.x);
            if (object != 0) {
                sprite = object->sprite;
                object->visible = 0;
                object->scale_x = 0xa3d7;
                object->scale_y = 0xa3d7;
                object->angle = ArcTan2(from.z - to.z, from.x - to.x);
                object->callback = BattleFx_SetCallbackWhenTargetUnset;
                zero = 0;
                sprite->palette = zero;
                sprite->mode = 1;
                Object_SetPosition(object, from.x, from.y, from.z);
            }
        }
    }

    if (work->counter == 0)
        Audio_PlayCue(130);
    if (++work->counter > 60)
        work->counter = 0;
}
