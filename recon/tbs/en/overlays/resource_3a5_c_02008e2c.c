/* Near miss: RamakanSabaku_SweepBack, resource_3a5, 2026-10-10.
 * Sole per-function proposal for resource_3a5_c_02008e2c.c.
 * Full current ramakan_sabaku/setup.c: pinned baseline228/common228/differ0;
 * ordinary offset228/common217/differ22, verified in all six editions.
 * This minimal includes/declarations/function draft independently retains
 * that EN result. Native extents include pools:
 * JA/EN/DE02008e2c..02009054; ES/FR/IT02008e38..02009060.
 *
 * Residual is the countdown/byte-offset exchange: ordinary count takes r6
 * and offset r7 instead of native count r7/offset r6. The source quantities,
 * loop, frame and extent are preserved. greg allocates count pseudo35 ahead
 * of offset38; lreg records higher use cost for count. The current pin fixes
 * exactly that competition.
 *
 * Bounded ordinary-C full-TU trials, all without new asm/pins:
 * unsigned offset22; u16/s16/u8 offset74/83/53; unsigned count22;
 * u16/s16/u8 count124/137/53. For/while/goto footer forms22; scoped count90,
 * scoped offset22, early offset24. Split/restored offset under count or point22.
 * Computing point at the loop top53, inline point computation103, word- or
 * SafePoint-indexed point arithmetic103. Unsigned left, ordinary register
 * count or offset, and explicit count==0 break each22.
 *
 * Earlier current-source notes, not repeated: generic offset input/output
 * boundary10(228/common223), generic offset/count outputs26, mixed
 * count-output/offset-input or reverse22; reusing offset as later frames144
 * (222 instructions). Those still contain forbidden replacement devices.
 * Whole-TU local ramakan.h is read unchanged; the canonical draft uses a
 * relative include of that same current header. Full ROM gates remain
 * required for adoption.
 */
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "../../../../games/tbs/src/field/ramakan_sabaku/ramakan.h"
#include "text/msg_ids.h"
#include "types.h"
#include "field_event.h"
#include "field_scene.h"
#include "scene_ids.h"
#include "call.h"
#include "iwram_call.h"
#include "field_service.h"
#include "field_effect.h"
#include "game_state.h"
#include "callback_scheduler.h"
#include "resource.h"
#include "mem.h"
#include "dma.h"
#include "io_write_queue.h"
#include "menu_list.h"
#include "ram_buffer.h"
#include "vram_block.h"
#include "io_reg.h"

#ifndef MODERN // modern GCC keeps the headers' volatile declaration
extern u32 gFrameCount;
#endif

struct EffectParams {
    u8 pad00[8];
    s32 scaleX;
    s32 scaleY;
    u8 pad10[18];
    u16 angle;
    u8 pad24[4];
};
void Engine_MapCopyCellsLayered();
s32 Engine_DisplayScrollStartHBlankDma();
s32 BattleFx_EmitRandomParticle();
s32 DisplayScroll_DisarmHBlankDma();

void Party_SetFields1ceAnd1d0(s32 scene, s32 entrance);


void RamakanSabaku_WaitForActorHeight(u8 *o, s32 limit);
s32 RamakanSabaku_CalculatePlanarDistance(s32 *position_a, s32 *position_b);
void RamakanSabaku_SetSandPhase(u16 value);

struct SafePoint {
    s32 x;
    s32 z;
};

void RamakanSabaku_SweepBack(void)
{
    u8 *work;
    s32 frames;
    s32 best;
    s32 count;
    const s32 *points;
    s32 left;
    s32 offset;
    const s32 *point;
    s32 nearest;
    s32 distance;
    struct FieldActor *leader;
    struct EffectOptions options;
    struct EffectOptions puff;
    s16 *progress;
    u16 *hold;
    s32 zero;

    work = (u8 *)gEventWork;
    frames = 60;
    best = 0xf00000;
    GameFlag_Set(0x200);
    RamakanSabaku_SetSandPhase(1);
    if (gGameState.scene == (s32)&SceneId_RamakanSabaku1) {
        count = 3;
        points = RamakanSabaku_SafePoints1;
    } else if (gGameState.scene == (s32)&SceneId_RamakanSabaku2) {
        count = 5;
        points = RamakanSabaku_SafePoints2;
    } else {
        count = 2;
        points = RamakanSabaku_SafePointsOther;
    }
    left = count;
    point = points;
    if (count != 0) {
        offset = 0;
        do {
            distance = RamakanSabaku_CalculatePlanarDistance(&Actor_Get(ACTOR_PARTY_LEADER)->x.fixed, (s32 *)point);
            if (distance <= best) {
                best = distance;
                nearest = left - count;
            }
            offset += 8;
            point = (const s32 *)((const u8 *)points + offset);
        } while (--count != 0);
    }
    nearest <<= 1;
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x20000, 0x10000);
    {
        struct FieldActor *actor = Actor_Get(ACTOR_PARTY_LEADER);
        const struct SafePoint *spot = (const struct SafePoint *)&points[nearest];

        FieldObject_SetPosition(actor, spot->x, 0, spot->z);
    }
    Actor_Get(ACTOR_PARTY_LEADER)->velocity_y = 0x60000;
    Event_PlayCue(152);
    leader = Actor_Get(ACTOR_PARTY_LEADER);
    RamakanSabaku_WaitForActorHeight((u8 *)leader, Actor_Get(ACTOR_PARTY_LEADER)->y.fixed);
    Event_PlayCue(241);
    {
        struct FieldActor *actor = Actor_Get(ACTOR_PARTY_LEADER);

        options.type = 214;
        options.start_scale_x = 0x8000;
        options.start_scale_y = 0xcccc;
        options.target_scale_x = 0x10000;
        options.target_scale_y = 0x13333;
        Effect_Spawn(actor->x.fixed, actor->y.fixed, actor->z.fixed, 0, 0, 0,
                     EFFECT_USE_TYPE | EFFECT_USE_START_SCALE | EFFECT_SCALE_TO_TARGET, &options);
    }
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x104, 0);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 18);
    hold = (u16 *)(work + 0xcba);
    progress = &gGameState.hazard_steps;
    zero = 0;
    do {
        s32 held = 600;

        *hold = held;
        frames--;
        if (*progress != 0) {
            *progress -= 5;
            if (*progress <= 0) {
                *progress = zero;
            } else if (frames == 0) {
                frames = 1;
            }
        }
        Task_Wait(1);
    } while (frames != 0);
    {
        struct FieldActor *actor = Actor_Get(ACTOR_PARTY_LEADER);
        s32 scale;

        puff.type = 214;
        /* FAKEMATCH: share one x-scale local across the start-y store. */
        scale = 0x8000;
        puff.start_scale_y = 0xcccc;
        puff.start_scale_x = scale;
        puff.target_scale_x = scale;
        puff.target_scale_y = 0x13333;
        Effect_Spawn(actor->x.fixed, actor->y.fixed, actor->z.fixed, 0, frames, frames,
                     EFFECT_USE_TYPE | EFFECT_USE_START_SCALE | EFFECT_SCALE_TO_TARGET, &puff);
    }
    Event_PlayCue(0x120);
    Event_PlayCue(152);
    Actor_Get(ACTOR_PARTY_LEADER)->velocity_y = 0x60000;
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 1);
    Event_Wait(10);
    *(u16 *)(work + 0xcba) = frames;
    RamakanSabaku_SetSandPhase(0);
}

