/* ported from games/tbs/src/field/common/object/command.c */
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
#include "objdisp.h"
#include "script_object_runtime.h"
#include "io_reg.h"
#include "battle_system.h"

#define gObjectSlots (*(struct ScriptMotionObject * *)&gObjectSlots)

#define TARGET_UNSET ((s32)0x80000000)

s32 ArcTan2(s32, s32);

void ObjectDispatch_ApplyArgumentToChildren(void *, s32);
void Object_SetPositionAndResetMotion(struct ObjectRuntime *, s32, s32, s32);


void Object_SetMoveTarget(struct ObjectRuntime *object, s32 x, s32 y, s32 z)
{
    s32 dx;
    s32 dy;
    s32 dz;
    s32 dist;
    u8 *axis;

    dx = (x - object->x) / 0x10000;
    dy = (y - object->y) / 0x10000;
    dz = (z - object->z) / 0x10000;
    dist = Iwram_Sqrt(dx * dx + dy * dy + dz * dz) << 16;
    if (dist < 0x100000) {
        dx = x - object->x;
        dy = y - object->y;
        dz = z - object->z;
        dist = FixedSqrt(Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dy, dy) + Iwram_MulQ16(dz, dz));
    }
    if (dist < 0x10000) {
        object->x = x;
        object->y = y;
        object->z = z;
        object->targetX = 0x80000000;
        object->targetY = 0x80000000;
        object->targetZ = 0x80000000;
        return;
    }
    if (object->snapToTarget == 0) {
        s32 brake;
        s32 ratio;

        brake = Iwram_RatioMulQ14(object->acceleration,
                             Iwram_MulQ16(object->speedLimit, object->speedLimit));
        if (dist > brake)
            brake = dist - brake / 2;
        else
            brake = dist / 2;
        ratio = Iwram_RatioMulQ14(dist, brake);
        x = object->x + Iwram_MulQ16(x - object->x, ratio);
        y = object->y + Iwram_MulQ16(y - object->y, ratio);
        z = object->z + Iwram_MulQ16(z - object->z, ratio);
    }
    object->targetX = x;
    object->targetY = y;
    object->targetZ = z;
    dx = x - object->x;
    dy = y - object->y;
    dz = z - object->z;
    axis = &object->unk56;
    *axis = 16;
    if ((dx < 0 ? -dx : dx) < (dz < 0 ? -dz : dz)) {
        *axis = 18;
        dx = dz;
    }
    if (object->motionFlags == 0) {
        if (dx < 0)
            dx = -dx;
        if (dx < (dy < 0 ? -dy : dy))
            *axis = 17;
    }
}
