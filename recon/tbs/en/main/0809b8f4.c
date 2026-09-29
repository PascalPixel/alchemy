/* Draft, not exact: EffectSlot_UpdateMotion, main:0809b8f4, complete
   320-byte extent. 2026-09-29 permuter score 715 (1 register-only, 6
   operand, 3 reordered, 4 deleted), was 1135 with the old spellings. The
   IWRAM square root goes through Iwram_Sqrt, the trigonometry through
   Trig_Cos/Trig_Sin, and a word heading keeps the ldrh value zero-extended
   in r5 as the reference does (a u16 local held it shifted left by 16).
   Remaining: at the merge the reference narrows the angle to u16 for the
   heading store and widens it back to s16 for the cos/sin argument it
   spills; a u16 temporary or a reread of the stored heading reallocates
   r4/r5/r7 (scores 1155-2210). The speed sum loads speed before
   acceleration, and the turn limit forms its offset in r7. */

#include "EFFECT_0809B11C.H"
#include "IWRAM_CALL.H"
#include "FIXED_MATH.H"

u16 ArcTan2(s32 x, s32 y);
s32 FixedSqrt(s32 value);

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
    s16 turn;
    s16 limit;

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
    effect->x += Iwram_MulQ16(Trig_Cos(angle), speed);
    effect->z += Iwram_MulQ16(Trig_Sin(angle), speed);
}
