/* Pin-free Camera_FollowLeaderInBounds trial: 224 native/built,
 * 222 common, differ 4. The rounding literal load and add use r3
 * instead of r2. An input-only barrier, an extra t input, and an inline
 * rounding layer leave differ 4. Removing the barrier as well differs
 * 66. A 3-minute search (seed 167, exact stack/branches) did not improve
 * the remaining pair. The staging permuter (seed 239, two jobs, two
 * minutes) also found no improvement across 2,129 distinct candidates.
 * Keeping obj->x as an extra input moves its load to r2 and differs 10;
 * adding t as an output keeps differ 4. A zero-initialized rounding value
 * joined after the branch differs 13; using 0x10000 minus one differs 20.
 * Tying the literal directly as an asm input, memory clobbers, unsigned
 * k and one-pass initialization/addition/whole-rounding blocks also retain
 * 4. Moving the barrier after addition with k as input or t as output
 * retains 4; a t-only output after ordinary addition retains the 66
 * differences of deleting the k barrier. An early-clobber k output with
 * obj->x as input retains 10. Initializing k beside t before the negative
 * branch differs 6: its load moves before the test. Initializing k first
 * differs 108. Moving only its declaration to the enclosing block or the
 * function, and retaining the conditional assignment, still differs 4.
 * The linked source keeps its binding. */
#define OWN_AudioCommand_PlayFar
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "object_runtime.h"
#include "iwram_call.h"
#include "dma.h"
#include "mem.h"
#include "types.h"
#include "script.h"
#include "script_object_entry.h"
#include "global_cells.h"
#include "objdisp.h"
#include "script_object_runtime.h"
#include "script_motion.h"
#include "fieldobj.h"
#include "io_reg.h"
#include "btlsys.h"

#define gObjectSlots (*(struct ScriptMotionObject * *)&gObjectSlots)

#define TARGET_UNSET ((s32)0x80000000)
struct MapScrollBounds {
    u8 unknown_00[0xec];
    s32 left;
    s32 top;
    s32 right;
    s32 bottom;
};

s32 Camera_FollowLeaderInBounds(struct ObjectRuntime *obj)
{
    struct ObjectRuntime *leader;
    struct MapScrollBounds *bounds;
    s32 min_x;
    s32 min_z;
    s32 max_x;
    s32 max_z;
    s32 x;
    s32 y;
    s32 z;
    s32 dz;
    s32 dy;
    s32 dx;
    s32 dist;
    s32 step;
    s32 (*div)(s32, s32);

    bounds = gMapWork[0];
    leader = obj->linked_object;
    min_x = bounds->left + 0x780000;
    min_z = bounds->top + leader->y + 0x600000;
    max_x = bounds->right - 0x780000;
    max_z = bounds->bottom + leader->y - 0x400000;
    obj->flags = 0;
    if (leader != NULL && leader->script != NULL) {
        x = leader->x;
        y = leader->y;
        /* FAKEMATCH: an extra use of y ranks it above the bounds in global allocation, giving it r5. */
        __asm__("" : : "r"(y));
        z = leader->z;
        obj->target_x = TARGET_UNSET;
        obj->target_y = TARGET_UNSET;
        obj->target_z = TARGET_UNSET;
        if (x < min_x)
            x = min_x;
        if (z < min_z)
            z = min_z;
        if (x > max_x)
            x = max_x;
        if (z > max_z)
            z = max_z;
        if (obj->action != 0) {
            obj->x = x;
            obj->y = y;
            obj->z = z;
        } else {
            {
                s32 t = x - obj->x;

                if (t < 0) {
                    /* FAKEMATCH: the rounding constant in r2 outside reload keeps reload's rotation as the native has it. */
                    s32 k = 0xffff;
                    /* FAKEMATCH: keeps k from folding into the add. */
                    __asm__("" : "+r"(k));
                    t += k;
                }
                dx = t >> 16;
            }
            dz = (z - obj->z) / 0x10000;
            dist = Iwram_Sqrt(dx * dx + dz * dz) << 16;
            dx = x - obj->x;
            dy = y - obj->y;
            dz = z - obj->z;
            if (dist < 0x400000)
                dist = FixedSqrt(Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dz, dz));
            if ((step = dist / 8) > obj->speed_limit)
                step = obj->speed_limit;
            if (dist < 0x4000) {
                obj->x = x;
                obj->z = z;
            } else {
                if (dist > step) {
                    div = Iwram_RatioMulQ14;
                    dx = Iwram_MulQ16(div(dist, dx), step);
                    dz = Iwram_MulQ16(div(dist, dz), step);
                }
                obj->x += dx;
                obj->z += dz;
            }
            if ((dy < 0 ? -dy : dy) > 0x8000)
                dy /= 4;
            obj->y += dy;
        }
    }
    obj->step++;
    return 1;
}
