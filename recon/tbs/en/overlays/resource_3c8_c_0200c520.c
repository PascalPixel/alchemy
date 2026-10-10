/* Pin-free VinasuHeya_SpawnRandomParticles trial: 90 native instructions,
 * 87 built, 70 common, differ 37. The ordinary options pointer occupies
 * r7 and x moves to r8; pointer copies and call preparation also change.
 * A plain copy block or pointer-equality guard retains differ 37.
 * An XOR copy guard differs 38 and loses the two scale stores, so it is
 * rejected. An integer pointer carrier retains differ 37.
 * The linked source retains its binding until every edition matches. */
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

    p->start_scale_x = 0xb333;
    p->start_scale_y = 0xb333;
    opts = p;
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
