/* Draft of overlay 6ae's Func_0200326c (0x0200b26c), ported from ☀️'s
 * SceneEffect_SpawnParticlesAboveActor: differ 15 of 92. Flag 0x200, actor
 * 23 and type 0x2a0 are ⚓️'s. With the spawn call written once after both
 * random shifts and the y load after a one-pass block, the arms merge as the
 * native's do (as ☀️'s two calls, differ 20). Left: the native builds 0x2a0's
 * shift after the x and z loads, and copies the effect back to r0 for the
 * script call where this draft reuses r0. */
#include "../../../../games/tla/src/field/overlay_6ae/ov6ae.h"
#include "field_sprite.h"
#include "flag_call.h"

extern const u8 Data_02006afc[];

s32 Math_Sine(s32 angle);
struct ObjectRuntime *Func_02005574(s32 type, s32 x, s32 y, s32 z);
void Func_0200556c(struct ObjectRuntime *object, const void *script);
void Func_02003210(struct ObjectRuntime *object);

// Every third frame, or every frame once flag 0x200 is set, spawns a particle
// above actor 23 that orbits it.
void Func_0200326c(void)
{
    struct ObjectRuntime *center;
    struct ObjectRuntime *effect;
    struct FieldSprite *sprite;
    s32 angle;
    u32 r;
    s32 y;

    if (Flag_IsSet(0x200) == 0 && gFrameCount % 3 != 0)
        return;
    center = Object_GetById(23);
    if (Flag_IsSet(0x200) != 0) {
        r = Random16Far();
        do {
        } while (0);
        y = center->y;
        r <<= 8;
    } else {
        r = Random16Far();
        do {
        } while (0);
        y = center->y;
        r <<= 6;
    }
    effect = Func_02005574(0x2a0, center->x, (s32)(r >> 16 << 16) + y - 0x1c0000, center->z);
    if (effect != 0) {
        sprite = effect->animation;
        Func_0200556c(effect, Data_02006afc);
        Object_SetPartAttribute(effect, 1);
        effect->motion_flags = 0;
        angle = Random16Far() & 0xffff000;
        effect->action = angle;
        effect->unknown_66 = 0;
        effect->update = (void (*)(void))Func_02003210;
        effect->speed_limit = Math_Sine((u32)(Random16Far() * 0xffff) >> 20) * 24 >> 16;
        sprite->next_frame = 0;
        sprite->priority = 2;
    }
}
