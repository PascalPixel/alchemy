/* Near miss: listing 225 instructions, draft 225, differ 16. Every statement, store and register
 * matches; left are the frame and four load-first orders. The native frame is 24 with canvas at
 * sp+20, an unused slot at sp+16, blit at sp+12 and frame at sp+8; here the same three spills
 * take sp+16, sp+12 and sp+8 (frame 20), so a fourth pseudo numbered between canvas and blit was
 * given a slot it never used. The native also loads before the reload pair that clears a high
 * counter (the spawn and draw loop preheaders), loads the spilled frame before i * 8 + 18 is
 * built, and loads Func_08143000 before work in the prologue; sched2 ties these on priority and
 * takes the zero first for its dependent (rank_for_schedule). Measured: an empty asm after the
 * spawn pointer fixes that preheader (differ 14); the draw preheader cannot take one, as the
 * hoisted heights load must still come first. A volatile pad local, a copied effect, work read
 * through a slot pointer, blit through a kernel pointer, unused locals, both counters shared or
 * not: no fourth spill slot. Widths and heights are read once each, in argument order, and the
 * tables are not const (the native rereads the heights after the call). permute (18 min):
 * nothing below 16 without a volatile pad. Resource ids 0x190 and 0xfd need names in
 * directory.s. */
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
    s32 unk_14;
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
    u8 *canvas = Ram_HeapSlots->battle_fx_work[1];
    u8 *work = Ram_HeapSlots->battle_fx_work[0];
    CanvasBlitFunc blit;
    s32 frame;
    s32 i;
    struct BubbleParticle *particle;

    Scheduler_AddOrUpdateCallback(Func_08143000, 0xc80);
    *(s32 *)(work + 0x7780) = 2;
    *(s32 *)(work + 0x7784) = 50;
    Func_081963ec(104, 19);
    blit = (CanvasBlitFunc)Ram_HeapSlots->unknown_64[1];
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

                blit(canvas, work + Data_0819968e[size] + 0x700,
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
