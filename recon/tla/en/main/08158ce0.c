/* Near miss of Func_08158ce0, ported from The Broken Seal's Camera_ApplyShake
 * (listing 61 instructions, draft 61, differ 4, one reordered pair): the
 * native copies shake_range (adds r7, r1, #0) before it starts the 0x77a8
 * offset (movs r0, #238) and loads the effect work between the offset's first
 * two instructions; here the offset starts first. sched2's priorities give
 * exactly the native order if a load's latency is 3 cycles instead of
 * arm9tdmi's 2. Tried: the heap base read into a local held by
 * asm volatile("" : "+l"(slots)) before the work load (differ 2: lsls r3
 * then falls after the parameter copies), a one-pass block there (differ 4),
 * barriers or "+l" asms on work, shake_range or remaining_frames (differ
 * 4 to 8), alchemy permute 4 minutes (no better). */
#include "types.h"
#include "battle_effect_work.h"
#include "ram_buffer.h"
#include "bg_scroll.h"

extern u32 gCameraSceneParameters[];
u32 Random16(void);

void Func_08158ce0(s32 random_mask, u32 shake_range)
{
    struct BattleEffectWork *work;
    s32 *remaining_frames;

    work = (struct BattleEffectWork *)Ram_HeapSlots->battle_fx_work[0];
    remaining_frames = &work->shake_frames;
    if (*remaining_frames > 0) {
        s32 random_x = (random_mask - 1) & Random16();
        s32 half_range = (s32)shake_range / 2;
        s32 offset_y = ((shake_range - 1) & Random16()) - half_range;
        s32 offset_x = random_x - half_range;

        gBgScroll[1].x = offset_x;
        gBgScroll[1].y = offset_y + 32;
        gCameraSceneParameters[3] = 120 - offset_x;
        gCameraSceneParameters[4] = 120 - offset_y;
        (*remaining_frames)--;
        return;
    }
    gBgScroll[1].x = work->saved_bg1_x;
    gBgScroll[1].y = work->saved_bg1_y;
    gCameraSceneParameters[3] = 120;
    gCameraSceneParameters[4] = 120;
}
