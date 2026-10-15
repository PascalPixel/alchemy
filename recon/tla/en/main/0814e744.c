/* Near miss, differ 56 of 337: The Broken Seal's BattleFx_RunIceShardBursts as
   ⚓️ has it: the effect argument read directly, resource 0x134 unpacked into
   the sheet slot and the ice block palette (resource 0x17d) copied, one blitter
   from Func_081963ec(104, 0x13), the callback at 0xc80, 128 shards a member
   moved by BattleFxKernels_IntegrateVector2 with gravities from gData_081982e4.
   Left: the sheet pointer and the member counter take each other's frame
   slots (sp+16 and sp+20), and at about a dozen rows the native issues a load
   one instruction earlier than here. */
#include "types.h"
#include "system.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "callback_scheduler.h"
#include "mem.h"
#include "effect_step.h"
#include "battle_effect_work.h"
#include "battle_effect.h"
#include "fixed_math.h"
#include "object_runtime.h"

#define SHARDS_PER_MEMBER 128
#define HI(v) (((s16 *)&(v))[1])

// The battle effect work as the canvas effects use it.
struct CanvasEffectWork
{
    struct EffectStep particles[64];
    u8 sheet[0x6980];
    s32 bg2X[160];
    u8 unk7300[0x480];
    s32 transferMode;
    s32 transferValue;
    u8 unk7788[0x160];
    s32 transferPending;
};

struct BattleObjectSlot
{
    struct ObjectRuntime *object;
};

extern char ResourceId_Data134;
extern char ResourceId_IceBlockSheet;
extern u16 gData_08197410[];
extern s32 gData_081982e4[];

void BattleFx_BeginCanvasLayer(s32 bgControl);
void Func_08143000(void);
void Func_08143bb8(void);
void Func_081434f8(void);
void Func_081180e8(s32 phase);
void Func_081963ec(s32 slot, s32 flags);
void Func_0814cd48(s32 id, s32 objectValue, s32 groupValue, s32 stateSlot, s32 stateValue);
void BattleFxKernels_IntegrateVector2(struct EffectStep *step, s32 damping, s32 gravity);
void Audio_PlayCue(s32 cue);
void *Resource_GetTableEntry(s32 id);
s32 Battle_GetObjectTableValueFar(s32 id);
struct BattleObjectSlot *GetBattleObjectSlotFar(s32 id);
void Render_ResetTransformState(void);
void Transform_SetLookAt(s32 *eye, s32 *target);

void Func_0814e744(struct BattleEffectArgument *effect)
{
    struct CanvasEffectWork *work;
    void *canvas;
    DrawRectangle draw;
    u8 *sheet;
    s32 *camera;
    s32 point[3];
    struct EffectPosition screen;
    s32 member;
    s32 frame;
    s32 i;

    work = (struct CanvasEffectWork *)Ram_HeapSlots->battleFxWork[0];
    canvas = Ram_HeapSlots->battleFxWork[1];
    sheet = Ram_HeapSlots->unk64[0];
    camera = Ram_HeapSlots->cameraWork;
    BattleFx_BeginCanvasLayer(1);
    Resource_LoadAndDecompress((s32)&ResourceId_Data134, sheet, 0, 0);
    Iwram_CopyWords((void *)0x05000000, Resource_GetTableEntry((s32)&ResourceId_IceBlockSheet), 128);
    Func_081963ec(104, 0x13);
    draw = (DrawRectangle)Ram_HeapSlots->unk64[1];

    for (i = 0; i != 8 * SHARDS_PER_MEMBER; i++)
        ((struct EffectStep *)Ram_MapCellBuffer)[i].variant = -1;

    Render_ResetTransformState();
    Transform_SetLookAt(camera, camera + 3);

    for (member = 0; member != effect->count; member++)
    {
        struct ObjectRuntime *object;
        s32 offset;
        s32 height;
        struct EffectStep *shard;

        offset = member * SHARDS_PER_MEMBER * sizeof(struct EffectStep);
        object = GetBattleObjectSlotFar(effect->actors[member])->object;
        height = Battle_GetObjectTableValueFar(effect->actors[member]) / 2;
        point[0] = object->x;
        point[1] = height;
        point[2] = object->z;
        EffectPosition_ApplyBaseAndYOffset(point, &screen);
        screen.x >>= 1;
        for (i = 0, shard = (struct EffectStep *)(Ram_MapCellBuffer + offset);
             i != SHARDS_PER_MEMBER; i++)
        {
            s32 radius;
            s32 angle;
            s32 lift;

            radius = Random16() & 0xff;
            angle = Random16() & 0xffff;
            shard->x = ((Trig_Sin(angle) * radius) >> 7) + (screen.x << 16);
            shard->y = ((Trig_Cos(angle) * radius) >> 3) + (screen.y << 16);
            shard->velocityX = (128 - (Random16() & 0xff)) << 9;
            lift = Random16() & 0xff;
            shard->variant = 0;
            shard->velocityY = (-lift - 128) << 10;
            shard++;
        }
    }

    work->transferMode = 2;
    work->transferValue = 50;
    Scheduler_AddOrUpdateCallback(Func_08143000, 0xc80);

    for (frame = 0; frame != effect->count * 20 + 56; frame++)
    {
        if (frame == 32)
            Func_081180e8(0);
        for (member = 0; member != effect->count; member++)
        {
            s32 offset;

            offset = member * SHARDS_PER_MEMBER * sizeof(struct EffectStep);
            if (frame == member * 20)
            {
                Audio_PlayCue(143);
                Func_0814cd48(effect->actors[member], 7, -1, member, 20);
            }
            // Shards falling past y = 112 go out.
            if (frame > member * 20)
            {
                struct EffectStep *shard;

                for (i = 0, shard = (struct EffectStep *)(Ram_MapCellBuffer + offset);
                     i != SHARDS_PER_MEMBER; i++)
                {
                    if (shard->variant >= 0)
                    {
                        s32 size;

                        size = i % 3 + 1;
                        draw(canvas, sheet + gData_08197410[size - 1],
                            HI(shard->x) - size / 2, HI(shard->y) - size, size, size * 2);
                        BattleFxKernels_IntegrateVector2(shard, 62, gData_081982e4[i & 3]);
                        shard->variant++;
                        if (shard->velocityY > 0 && HI(shard->y) > 112)
                            shard->variant = -1;
                    }
                    shard++;
                }
            }
        }
        Func_081434f8();
        work->transferPending = 1;
        WaitFrames(1);
    }

    Scheduler_RemoveCallback(Func_08143000);
    Runtime_ReleaseHeapBlock(104);
    Func_08143bb8();
}
