/* Pin-free VinasuHeya_SpawnRandomParticles trial: 90 native and built
 * instructions, 87 common, differ 6. A generic options output with the
 * existing scale as input restores the pointer to r8; the spin offset
 * uses r2 rather than r3. Plain options differ 37 (87 built), an output
 * alone differs 16, and storing spin through p differs 15 (89 built).
 * Initializing opts after the spin store retains 37. Scale output leaves
 * 36 (88 built), while a volatile options output leaves 10. Reusing scale,
 * phase or coin for the spin offset and signed/unsigned spin types retain
 * six; reassigning opts after the RNG call returns to 37. Moving the
 * barrier after the spin calculation or RNG call leaves 16 or 20.
 * Low, general and early-clobber outputs retain six; a high-register
 * output fails in reload_cse_simplify_operands. All six editions share
 * this residual. A two-minute exact-stack/branch search (seed 463, two
 * jobs) tried 1545 distinct candidates, with 1535 failed evaluations and
 * no improvement. Heat-guided mode cannot parse the function.
 * Explicit tied constraints in either input order, tying p directly and
 * an early-clobber tied p retain six. Adding x or z outputs leaves 22 or
 * 24; adding p or the options memory as input leaves 16. A volatile
 * options target leaves 31 (89 built). A u16-cast negative offset,
 * unsigned RNG shift and reversed spin sum each retain six.
 * Further EN trials: a masked or combined shift, a narrowed shift and
 * unsigned x retain six. A narrowed negative offset leaves 9 (85 built).
 * Halfword-array and byte-pointer spin stores leave 14; ordinary register
 * qualification retains six. Passing x or z through instead of options
 * leaves 45 or 47, both leave 56. Plain options with register or unsigned
 * x retain 37. A generic scale input retains six. */
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "global_cells.h"
#include "edition.h"
#include "../../../../games/tbs/src/field/vinasu_heya/entry_setup.h"
#include "iwram_call.h"
#include "types.h"
#include "field_event.h"
#include "field_scene.h"
#include "call.h"
#include "text/msg_ids.h"
#include "field_effect.h"
#include "field_service.h"
#include "io_reg.h"
#include "scene_ids.h"

#define FieldScene_PrepareActors ((void (*)(u8*))FieldScene_PrepareActors)

extern u8 *gActorEffectWork;
void Object_SetActionById();

void VinasuHeya_SpawnRandomParticles(s32 x, s32 z)
{
    struct EffectOptions options;
    struct EffectOptions *p = &options;
    struct EffectOptions *opts;
    s32 phase;
    u32 coin;
    u32 scale = 0xb333;

    p->start_scale_x = scale;
    p->start_scale_y = scale;

    opts = p;
    /* FAKEMATCH: the generic options output and scale input restore r8 and the scale-store operands without a pin; the spin-offset register still differs. */
    asm("" : "+r"(opts) : "l"(scale));
    {
        s32 spin = ((u32)(Engine_RandomNext() << 12) >> 16) + 0xf800;
        u32 *frame = &gFrameCount; /* FAKEMATCH: the counter's address is loaded before the spin is stored */

        opts->spin = spin;
        phase = *frame & 3;
    }

    if (phase == 0) {
        coin = (u32)(Engine_RandomNext() << 1) >> 16;
        if (coin != 0) {
            s32 r = Engine_RandomNext();
            s32 vz = __divsi3((((u32)(Engine_RandomNext() * 5) >> 16) << 16) + 0x70000, 10);

            Effect_Spawn((x + (((u32)(r << 1) >> 16) << 4)) << 16, 0, z << 19, 0, phase, vz, 0x880000, opts);
        } else {
            s32 r = Engine_RandomNext();

            Effect_Spawn((x + ((u32)(r * 17) >> 16)) << 16, 0, (x << 19) - 0x40000, 0, coin, coin, 0x880000, opts);
        }
    }
}
