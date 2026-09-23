/* Draft, not exact (2026-09-24): 44 differing halfwords, 320 of 320
   bytes. Distance test and motion match; the turn-limit block differs:
   the reference loads the heading with ldrh and both halves of the turn
   limit up front, and reloads the stored heading as s16 for cos/sin. */

#include "EFFECT_0809B11C.H"
#include "IWRAM_CALL.H"

u16 ArcTan2(s32 x, s32 y);
s32 FixedSqrt(s32 value);
s32 Func_0800231c(s32 angle);
s32 Func_08002322(s32 angle);

void Func_0809b8f4(struct EffectSlot *effect)
{
    s32 dx;
    s32 dz;
    s32 distance;
    s32 ix;
    s32 iz;
    s32 speed;
    s16 angle;
    u16 heading;
    s16 turn;
    s16 limit;

    if (effect->target_x == EFFECT_NO_TARGET)
        return;
    dx = effect->target_x - effect->x;
    dz = effect->target_z - effect->z;
    if (effect->flag41 != 0) {
        ix = dx / 0x10000;
        iz = dz / 0x10000;
        distance = ((s32 (*)(s32))0x030001d8)(ix * ix + iz * iz) << 16;
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
        limit = effect->max_turn_step;
        if ((turn >= 0 ? turn : -turn) >= limit) {
            if (turn < 0) {
                if (-turn > limit)
                    turn = -effect->max_turn_step;
            } else if (turn > limit) {
                turn = limit;
            }
            angle = turn + heading;
        }
    }
    effect->heading = angle;
    speed = effect->speed + effect->acceleration;
    if (speed > effect->max_speed)
        speed = effect->max_speed;
    effect->speed = speed;
    effect->x += Iwram_MulQ16(Func_0800231c(angle), speed);
    effect->z += Iwram_MulQ16(Func_08002322(angle), speed);
}
