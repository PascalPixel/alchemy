/* Near miss: listing 225 instructions, draft 225, differ 8. Statements, registers, stores and
 * the frame (canvas sp+20, the blitter pair sp+12/sp+16, frame sp+8) all match; four places are
 * left where the native loads first and this build does not: Func_08143000's address before
 * work in the prologue, the particle base before the reload pair that clears i in both loop
 * preheaders (after the hoisted heights in the second), and the spilled frame before i * 8 is
 * built. sched2 ties each on priority and takes the other insn for its dependent or its class
 * against the last call. Measured: an empty asm after the spawn pointer fixes the first
 * preheader (differ 6); one-pass blocks or asm operands on i or frame in the actor loop: 25 to
 * 167; a particle-array pointer variable, an inline blitter loader, a volatile pad, copied
 * effect, work through a slot pointer: no gain. Widths and heights are read once each, in
 * argument order, and the tables are not const (the native rereads the heights after the
 * call). Resource ids 0x190 and 0xfd need names in directory.s. */
#include "types.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "battle_effect_work.h"

struct BubbleParticle {
    s32 x;
    s32 y;
    s32 size;
    s32 velocityX;
    s32 velocityY;
    s32 unk14;
    s32 timer;
};

typedef s32 (*CanvasBlitFunc)(u8 *canvas, void *source, s32 x, s32 y, s32 width, s32 height);

extern struct BubbleParticle Data_02015000[];
extern char ResourceId_BlueBeamSheet;
extern char ResourceId_Data0FD;
extern u8 Data_0819966f[];
extern u8 Data_0819967e[];
extern u16 Data_0819968e[];

void Func_08143000(void);
s32 Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 order);
s32 Scheduler_RemoveCallback(void (*callback)(void));
void Func_081963ec(s32 slot, s32 value);
void *Resource_GetTableEntry(s32 id);
void Resource_LoadAndDecompress(s32 id, void *target, s32 unpack, s32 wait);
u32 Random16(void);
void BattleFxKernels_IntegrateVector2(void *particle, s32 mask, s32 gravity);
void Func_0814cd48(s32 id, s32 a, s32 b, s32 c, s32 d);
void Func_081180e8(s32 cue);
void Audio_PlayCue(s32 cue);
void Func_081434f8(void);
void WaitFrames(s32 frames);
void Runtime_ReleaseHeapBlock(s32 slot);
void Func_08143bb8(void);

void Func_08182898(struct BattleEffectArgument *effect)
{
    u8 *canvas = Ram_HeapSlots->battleFxWork[1];
    u8 *work = Ram_HeapSlots->battleFxWork[0];
    /* The blitter pair, of which this effect uses the first: as a two-word array it is one
       DImode pseudo, and its spill takes sp+12 and the unused sp+16 the native frame has. */
    CanvasBlitFunc blit[2];
    s32 frame;
    s32 i;
    struct BubbleParticle *particle;

    Scheduler_AddOrUpdateCallback(Func_08143000, 0xc80);
    *(s32 *)(work + 0x7780) = 2;
    *(s32 *)(work + 0x7784) = 50;
    Func_081963ec(104, 19);
    blit[0] = (CanvasBlitFunc)Ram_HeapSlots->unk64[1];
    Iwram_CopyWords((void *)0x05000000, Resource_GetTableEntry((s32)&ResourceId_BlueBeamSheet), 128);
    Resource_LoadAndDecompress((s32)&ResourceId_Data0FD, work + 0x700, 0, 0);
    for (frame = 0; frame != 60; frame++)
    {
        if (frame == 0)
        {
            struct BubbleParticle *spawn = Data_02015000;

            for (i = 0; i != 256; i++)
            {
                spawn->x = (Random16() & 127) << 16;
                spawn->y = ((Random16() & 127) + 120) << 16;
                spawn->velocityX = ((Random16() & 31) - 16) << 13;
                spawn->velocityY = (8 - (Random16() & 31)) << 12;
                spawn->size = (Random16() & 7) + 5;
                spawn->timer = 32;
                spawn++;
            }
        }
        particle = Data_02015000;
        for (i = 0; i != 256; i++)
        {
            if (particle->timer >= 0)
            {
                s32 size = particle->size;
                u32 width;
                u32 height;

                blit[0](canvas, work + Data_0819968e[size] + 0x700,
                     *(s16 *)((u8 *)&particle->x + 2) - ((width = Data_0819966f[size]) >> 1),
                     *(s16 *)((u8 *)&particle->y + 2) - ((height = Data_0819967e[size]) >> 1),
                     width, height);
                BattleFxKernels_IntegrateVector2(particle, 63, -0x4000);
                if (particle->y < -(Data_0819967e[size] << 16))
                    particle->timer = -1;
            }
            particle++;
        }
        for (i = 0; i != effect->count; i++)
        {
            if (frame == i * 8 + 18)
            {
                if (i == 0)
                {
                    Func_0814cd48(effect->actors[i], 7, -1, i, 16);
                    Func_081180e8(126);
                    Audio_PlayCue(126);
                }
                else
                {
                    Audio_PlayCue(126);
                    Func_0814cd48(effect->actors[i], 7, -1, i, 16);
                }
            }
        }
        Func_081434f8();
        *(s32 *)(work + 0x78e8) = 1;
        WaitFrames(1);
    }
    Runtime_ReleaseHeapBlock(104);
    Scheduler_RemoveCallback(Func_08143000);
    Func_08143bb8();
}
