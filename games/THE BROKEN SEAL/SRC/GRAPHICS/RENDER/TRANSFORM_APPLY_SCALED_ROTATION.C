#include "TYPES.H"
#include "IWRAM_CALL.H"

s32 Trig_Sin(s32 angle);
s32 Trig_Cos(s32 angle);

/* SceneTransform_ApplyRotationTranslation with each matrix row scaled by
   the matching component of the scale vector. */
void SceneTransform_ApplyScaledRotation(s32 *angles, s32 *position, s32 *scale)
{
    s32 sx, cx, sy, cy, sz, cz;
    s32 s;
    s32 m[12];

    sx = Trig_Sin(angles[0]);
    cx = Trig_Cos(angles[0]);
    sy = Trig_Sin(angles[1]);
    cy = Trig_Cos(angles[1]);
    sz = Trig_Sin(angles[2]);
    cz = Trig_Cos(angles[2]);
    s = scale[0];
    m[0] = Iwram_MulQ16(s, Iwram_MulQ16(cy, cz));
    m[1] = Iwram_MulQ16(s, Iwram_MulQ16(cy, sz));
    m[2] = Iwram_MulQ16(s, -sy);
    s = scale[1];
    m[3] = Iwram_MulQ16(s, Iwram_MulQ16(Iwram_MulQ16(sx, sy), cz) - Iwram_MulQ16(cx, sz));
    m[4] = Iwram_MulQ16(s, Iwram_MulQ16(Iwram_MulQ16(sx, sy), sz) + Iwram_MulQ16(cx, cz));
    m[5] = Iwram_MulQ16(s, Iwram_MulQ16(sx, cy));
    s = scale[2];
    m[6] = Iwram_MulQ16(s, Iwram_MulQ16(Iwram_MulQ16(cx, sy), cz) + Iwram_MulQ16(sx, sz));
    m[7] = Iwram_MulQ16(s, Iwram_MulQ16(Iwram_MulQ16(cx, sy), sz) - Iwram_MulQ16(sx, cz));
    m[8] = Iwram_MulQ16(s, Iwram_MulQ16(cx, cy));
    m[9] = position[0];
    m[10] = position[1];
    m[11] = position[2];
    ((void (*)(s32 *))0x030002c0)(m);
}
