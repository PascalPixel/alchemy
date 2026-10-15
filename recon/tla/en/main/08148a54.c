/* Near miss, differ 24 of 500: The Broken Seal's BattleFx_RunVortexMotes as ⚓️
   has it: the effect argument read directly, the vortex sheet (resource 0x191)
   unpacked after the particles, two blitters from Func_081963ec(104, 0x13) and
   (188, 0x1f), Func_0814c928 for the camera and the transfer callback at 0xc80,
   the frame and mote counts from the byte table gData_081979a8 (not const),
   Func_081180e8(133) and Func_08118088 for the struck member. Same frame,
   registers and instructions; what is left is order at nine rows, each a load
   the native issues one instruction earlier (the second argument before the
   first for Func_081963ec(104), the BLDALPHA value and the second blitter
   slot after the second call, the flags base, the point's y and z before the
   target's, a frame reload). Tried: the second argument pinned to r1 fixes the
   first row (differ 22); the BLDALPHA value pinned behind a barrier, the
   second blitter read first, or an empty barrier after the second call leave
   that block as it is or break the call before (differ 22 to 24); alchemy
   permute for 6 minutes (no better). */
#include "types.h"
#include "system.h"
#include "io_reg.h"
#include "ram_buffer.h"
#include "callback_scheduler.h"
#include "mem.h"
#include "effect_step.h"
#include "battle_effect_work.h"
#include "battle_effect.h"
#include "object_runtime.h"

// The battle effect work as the canvas effects use it.
struct CanvasEffectWork
{
    struct EffectStep particles[64];
    u8 sheet[0x6980];
    s32 bg2X[160];
    u8 unk7300[0x480];
    s32 transferMode;
    s32 transferValue;
    u8 unk7788[0x20];
    s32 shakeFrames;
    s32 cameraDelta;
    s32 cameraPhase;
    u8 unk77b4[0x134];
    s32 transferPending;
};

struct BattleObjectSlot
{
    struct ObjectRuntime *object;
};

extern char ResourceId_VortexSheet;
extern u8 gData_081979a8[];
extern u8 gMapCellBuffer[];

void BattleFx_BeginCanvasLayer(s32 bgControl);
void Func_08143000(void);
void Func_08143bb8(void);
void Func_081434f8(void);
void Func_0814c928(void);
void Func_08158ce0(s32 x, s32 y);
void Func_081180e8(s32 phase);
void Func_08118088(s32 id, s32 variant);
void Func_081963ec(s32 slot, s32 flags);
void Func_0814cd48(s32 id, s32 objectValue, s32 groupValue, s32 stateSlot, s32 stateValue);
void Audio_PlayCue(s32 cue);
s32 Battle_GetObjectTableValueFar(s32 id);
struct BattleObjectSlot *GetBattleObjectSlotFar(s32 id);
void Render_ResetTransformState(void);
void Graphics_PrepareTransferInIwramWork(s32 *eye, s32 *target);

void Func_08148a54(struct BattleEffectArgument *effect)
{
    struct EffectPosition pos;
    struct CanvasEffectWork *work;
    void *canvas;
    DrawRectangle draw[2];
    struct ObjectRuntime *source;
    struct ObjectRuntime *object;
    struct EffectStep *point;
    struct EffectStep *seat;
    struct EffectStep *target;
    s32 *camera;
    s32 frame;
    s32 y;
    s32 i;
    s8 *flags;

    flags = (s8 *)gMapCellBuffer;
    work = (struct CanvasEffectWork *)Ram_HeapSlots->battleFxWork[0];
    canvas = Ram_HeapSlots->battleFxWork[1];
    BattleFx_BeginCanvasLayer(1);
    Resource_LoadAndDecompress((s32)&ResourceId_VortexSheet, work->sheet, 1, 1);
    Func_081963ec(104, 0x13);
    draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
    Func_081963ec(188, 0x1f);
    REG_BLDALPHA = 0x0f0f;
    draw[1] = (DrawRectangle)Ram_HeapSlots->unkb8[1];
    source = GetBattleObjectSlotFar(effect->actor)->object;
    y = source->y + Battle_GetObjectTableValueFar(effect->actor);
    for (i = 0; i != 30; i++)
    {
        point = &work->particles[i];
        point->x = source->x;
        point->y = y;
        point->z = source->z;
        point->velocityX = ((s32)((Random16() & 255) - 127) << 16) >> 5;
        point->velocityY = ((s32)((Random16() & 127) - 16) << 16) >> 6;
        point->velocityZ = ((s32)((Random16() & 255) - 127) << 16) >> 5;
        point->variant = -1;
        flags[i] = 0;
    }
    for (i = 0; i != effect->count; i++)
    {
        seat = &work->particles[32 + i];
        object = GetBattleObjectSlotFar(effect->actors[i])->object;
        seat->x = object->x;
        seat->y = 0;
        seat->z = object->z;
    }
    work->cameraDelta = 0;
    work->cameraPhase = 0;
    Scheduler_AddOrUpdateCallback(Func_0814c928, 0xc80);
    work->transferMode = 2;
    work->transferValue = 75;
    Scheduler_AddOrUpdateCallback(Func_08143000, 0xc80);
    Audio_PlayCue(164);

    for (frame = 0; frame != gData_081979a8[effect->variant * 2 + 1]; frame++)
    {
        camera = Ram_HeapSlots->cameraWork;
        if (frame >= 17 && frame < 64)
            work->cameraDelta = 0x180;
        else
            work->cameraDelta = 0;
        if (frame == gData_081979a8[effect->variant * 2 + 1] - 16)
            Func_081180e8(133);
        Render_ResetTransformState();
        Graphics_PrepareTransferInIwramWork(camera, camera + 3);
        for (i = 0; i != gData_081979a8[effect->variant * 2]; i++)
        {
            point = &work->particles[i];
            if (frame > i * 2 && flags[i] == 0)
            {
                EffectPosition_ApplyBaseAndYOffset((s32 *)point, &pos);
                pos.x >>= 1;
                if (pos.depth < 160)
                    pos.depth = 160;
                if (pos.depth > 0x31f)
                    pos.depth = 0x31f;
                draw[0](canvas, work->sheet + 0xc00, pos.x - 6, pos.y - 12, 12, 24);
                point->x += point->velocityX;
                point->y += point->velocityY;
                point->z += point->velocityZ;
            }
            if (frame > i * 2 + 48 && flags[i] == 0)
            {
                target = &work->particles[32 + i % effect->count];
                point->velocityX += (target->x - point->x) >> 9;
                point->velocityY += (target->y - point->y) >> 9;
                point->velocityZ += (target->z - point->z) >> 9;
                if (frame < i * 2 + 85)
                {
                    point->velocityX = point->velocityX * 60 / 64;
                    point->velocityY = point->velocityY * 60 / 64;
                    point->velocityZ = point->velocityZ * 60 / 64;
                }
                if (point->y < 0)
                {
                    flags[i] = 1;
                    point->variant = 0;
                    point->x = pos.x;
                    point->y = pos.y + (Random16() & 31) - 16;
                    Func_0814cd48(effect->actors[i % effect->count], 7, 5, i % effect->count, 4);
                    Func_08118088(effect->actors[i % effect->count], 0);
                    work->shakeFrames = 4;
                    Audio_PlayCue(132);
                }
            }
            if (point->variant >= 0 && point->variant < 16)
            {
                draw[0](canvas, work->sheet + ((point->variant / 2) % 3 << 10), point->x - 16,
                    point->y - 56, 16, 64);
                draw[1](canvas, work->sheet + ((point->variant / 2) % 3 << 10), point->x,
                    point->y - 56, 16, 64);
                point->variant++;
            }
        }
        Func_08158ce0(effect->variant * 2 + 2, effect->variant * 2 + 2);
        if (work->cameraPhase == 0)
            work->cameraPhase = 1;
        Func_081434f8();
        work->transferPending = 1;
        WaitFrames(1);
    }

    Scheduler_RemoveCallback(Func_0814c928);
    Scheduler_RemoveCallback(Func_08143000);
    Runtime_ReleaseHeapBlock(188);
    Runtime_ReleaseHeapBlock(104);
    Func_08143bb8();
}
