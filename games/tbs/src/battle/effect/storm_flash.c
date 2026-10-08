#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "callback_scheduler.h"
#include "resource.h"
#include "mem.h"
#include "global_cells.h"
#include "types.h"
#include "dma.h"
#include "system.h"
#include "battle_effect_runtime.h"
#include "map_scroll.h"
#include "fxblend.h"
#include "io_reg.h"
#include "btlsys.h"
#include "btlpty.h"

#define BattleFx_ApplyColorToTargetBuffer ((void (*)(void*,s32))BattleFx_ApplyColorToTargetBuffer)

void AudioCommand_PlayFar(s32);

extern void *gFieldBlendWork[];

struct GroundParticle {
    u8 unknown_00[4];
    u16 y : 8;
    u16 affine : 2;
    u16 blend_mode : 2;
    u16 mosaic : 1;
    u16 full_color : 1;
    u16 shape : 2;
    u16 x : 9;
    u16 affine_index : 5;
    u16 size : 2;
    u16 tile : 10;
    u16 priority : 2;
    u16 palette : 4;
    u8 unknown_0a[2];
    s32 pos_x;
    s32 pos_y;
    s32 pos_z;
    u8 unknown_18[4];
    u16 timer;
    u8 unknown_1e[2];
};

struct GroundParticleWork {
    s32 entry;
    s32 tile_base;
    struct GroundParticle particles[32];
    s32 unknown_408[2];
};

void Resource_DecodeByteLz(const void *source, void *destination);
void BattleFx_SetQueuedSoundAndPlay(s32 sound);

void FieldFx_UpdateGroundParticles(void)
{
    struct MapScrollWork *view = gMapWork[0];
    struct GroundParticleWork *work = gMapWork[21];
    s32 *camera = &view->view_x;
    s32 camera_x = camera[0];
    s32 camera_z = camera[1];
    struct GroundParticle *particle = work->particles;
    u32 i;

    for (i = 0; i < 32; i++, particle++) {
        s16 *frame;
        s32 x;
        s32 y;

        if (--particle->timer == 0xffff)
            continue;
        if (GameFlag_TestFar(0x166))
            particle->timer++;
        frame = &FieldFx_GroundParticleFrames[5 * particle->timer];
        x = (particle->pos_x - camera_x) / 0x10000 + *frame++;
        y = (particle->pos_z - particle->pos_y - camera_z) / 0x10000 + *frame++;
        if ((x >= -16 && x <= 239) && y >= -32 && y <= 159) {
            particle->priority = 1;
            particle->x = x;
            particle->y = y;
            particle->tile = work->tile_base + *(u16 *)frame;
            frame++;
            particle->shape = *(u8 *)frame;
            particle->size = *(u8 *)(frame + 1);
            Runtime_PushSlotEntry(particle, 240);
        }
        if (particle->timer == 0) {
            s32 *leader = view->origin;

            x = leader[0] + (Random16() << 8) - 0x800000;
            y = leader[2] + (Random16() << 8) - 0x800000;
            particle->pos_x = x;
            particle->pos_z = y;
            particle->pos_y = Map_GetTerrainHeightFar(0, x >> 16, y >> 16) << 16;
            particle->timer = 16;
        }
    }
}

void BattleFx_UpdateStormFlash(void)
{
    struct FieldBlendWork *work = gFieldBlendWork[0];
    struct BattleEffectBuffers *target = gFieldBlendWork[2];
    s16 *timer = &work->timer;

    if (*timer < 0)
        return;
    if (GameFlag_TestFar(358))
        work->timer = 128;
    switch ((*timer)--) {
    case 0:
        if (work->enabled != 0) {
            u32 a = Random16();
            u32 b = Random16();
            work->timer = ((a * 400) >> 16) - ((b * 100) >> 16) + 150;
            if (work->loud != 0)
                AudioCommand_PlayFar(172);
            else
                AudioCommand_PlayFar(171);
        }
    case 5:
    case 10:
        BattleFx_ApplyColorToTargetBuffer(work, 1);
        Dma_Set(work->delta, target->delta, 0x840002a0, REG_DMA3);
        target->duration = 12;
        target->step = 0;
        break;
    case 1:
    case 6:
    case 11:
        BattleFx_ApplyColorToTargetBuffer(work->to, 1);
        target->duration = 1;
        target->step = 0;
        break;
    }
}

void FieldFx_StartGroundParticles(void)
{
    struct GroundParticleWork *work;
    s32 *leader;
    struct GroundParticle *particle;
    u32 *words;
    volatile u32 fill;
    volatile u16 *blend;
    u32 i;
    s32 value;

    /* FAKEMATCH: needed to match: reusing the cursor preserves its register */
    work = Runtime_AllocateBlock(29, sizeof(struct GroundParticleWork));
    leader = ((struct MapScrollWork *)gMapWork[0])->origin;
    BattleFx_SetQueuedSoundAndPlay(170);
    words = 0;
    particle = work->particles;
    fill = (u32)words;
    Dma_Set((const void *)&fill, work, 0x85000000 | (sizeof(struct GroundParticleWork) / 4),
        REG_DMA3);
    words = Runtime_AllocateBlock(14, 0x400);
    Resource_DecodeByteLz(FieldFx_GroundParticleTiles, words);
    work->entry = Resource_FindFreeEntry();
    work->tile_base = VramBlock_LoadCached(work->entry, 0x300, words);
    Runtime_ReleaseHeapBlock(14);
    for (i = 0; i < 32; i++) {
        s32 x;
        s32 z;

        words = (u32 *)particle;
        *words++ = 0;
        *words++ = 0x40000400;
        *words = 0xd400;
        x = leader[0];
        z = leader[2];
        particle->pos_x = x;
        particle->pos_z = z;
        particle->pos_y = Map_GetTerrainHeightFar(0, x >> 16, z >> 16) << 16;
        particle->timer = (i & 15) + 1;
        particle++;
    }
    do {
        blend = &REG_BLDCNT;
        value = 0x3f00;
        *blend = value;
        value = 0x1008;
        *++blend = value;
        *++blend = 0;
    } while (0);
    Scheduler_AddOrUpdateCallback(FieldFx_UpdateGroundParticles, 0xc80);
}
