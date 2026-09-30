#include "IWRAM_CALL.H"
#include "EFFECT_0809B11C.H"
#include "FIXED_MATH.H"

struct EffectSprite {
    u8 unknown_00[24];
    s32 scale;
};

struct EffectObject {
    struct EffectSprite *sprite;
    s32 x;
    s32 y;
    u8 unknown_0c[28];
    s32 scale_x;
    s32 scale_y;
    u8 unknown_30[23];
    u8 flags;
};

void Object_ApplyProjectedPlacementFar(struct EffectSprite *sprite, s32 *position, s32 *scale, s32 mode);

/* Draws an effect object at its position, scaled by its own and its
   sprite's factors, when it lies near the screen; flag 4 mirrors it about
   the floor line. */
void BattleFx_DrawScaledObject(struct EffectObject *object)
{
    struct EffectSprite *sprite;
    s32 offset;
    s32 scale[2];
    s32 position[4];

    sprite = object->sprite;
    offset = 0;
    if (object->flags & 4)
        offset = 0x1fc0000 - object->y;
    scale[0] = Iwram_MulQ16(object->scale_x, sprite->scale);
    scale[1] = Iwram_MulQ16(object->scale_y, sprite->scale);
    position[0] = object->x;
    position[1] = offset;
    position[2] = object->y + offset;
    position[3] = 0;
    if (object->x > -0x200000 && object->x < 0x1100000 && object->y > -0x200000 && object->y < 0xe00000)
        Object_ApplyProjectedPlacementFar(sprite, position, scale, 0);
}

#if defined(TBS_EDITION_EN)
/* The other editions keep their code here in their scaffolds for now. */

u16 ArcTan2(s32 x, s32 y);
s32 FixedSqrt(s32 value);

/* Steer an effect slot toward its target: snap to it once within 8.0,
   otherwise turn by at most the slot's turn step, accelerate up to
   the maximum speed and move along the heading. */
void EffectSlot_UpdateMotion(struct EffectSlot *effect)
{
    s32 dx;
    s32 dz;
    s32 distance;
    s32 ix;
    s32 iz;
    s32 speed;
    s16 angle;
    s32 heading;
    s32 facing;
    s16 turn;

    if (effect->target_x == EFFECT_NO_TARGET)
        return;
    dx = effect->target_x - effect->x;
    dz = effect->target_z - effect->z;
    if (effect->flag41 != 0) {
        ix = dx / 0x10000;
        iz = dz / 0x10000;
        distance = Iwram_Sqrt(ix * ix + iz * iz) << 16;
        if (distance < 0x800000)
            distance = FixedSqrt(Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dz, dz));
        if (distance <= 0x80000) {
            EffectSlot_SetPosition(effect, effect->target_x, effect->target_z);
            return;
        }
    }
    angle = ArcTan2(dz, dx);
    if (effect->flag42 != 0) {
        heading = effect->heading;
        turn = angle - heading;
        if ((turn >= 0 ? turn : -turn) >= effect->max_turn_step) {
            if (turn < 0) {
                if (-turn > effect->max_turn_step)
                    turn = -effect->max_turn_step;
            } else if (turn > effect->max_turn_step) {
                turn = effect->max_turn_step;
            }
            angle = turn + heading;
        }
    }
    facing = (u16)angle;
    effect->heading = facing;
    speed = effect->speed + effect->acceleration;
    if (speed > effect->max_speed)
        speed = effect->max_speed;
    angle = (s16)facing;
    effect->speed = speed;
    effect->x += Iwram_MulQ16(Trig_Cos(angle), speed);
    effect->z += Iwram_MulQ16(Trig_Sin(angle), speed);
}
#endif
