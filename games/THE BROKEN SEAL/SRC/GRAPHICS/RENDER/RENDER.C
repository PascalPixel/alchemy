#include "TYPES.H"
#include "TRANSFORM.H"
#include "IWRAM_CALL.H"

s32 Trig_Sin(s32 angle);
s32 Trig_Cos(s32 angle);

void SceneTransform_ResetMatrix(void)
{
    /* CAMELOT_ASM: the fixed-register identity store of TRANSFORM.H */
    Transform_SetIdentity(gTransform);
}

/* Builds the rotation matrix for the three angles (Q16 sines and cosines,
   rows x, y, z, zero translation) and hands it to the IWRAM transform
   routine. */
void SceneTransform_ApplyRotation(s32 *angles)
{
    s32 sx, cx, sy, cy, sz, cz;
    s32 m[12];

    sx = Trig_Sin(angles[0]);
    cx = Trig_Cos(angles[0]);
    sy = Trig_Sin(angles[1]);
    cy = Trig_Cos(angles[1]);
    sz = Trig_Sin(angles[2]);
    cz = Trig_Cos(angles[2]);
    m[0] = Iwram_MulQ16(cy, cz);
    m[1] = Iwram_MulQ16(cy, sz);
    m[2] = -sy;
    m[3] = Iwram_MulQ16(Iwram_MulQ16(sx, sy), cz) - Iwram_MulQ16(cx, sz);
    m[4] = Iwram_MulQ16(Iwram_MulQ16(sx, sy), sz) + Iwram_MulQ16(cx, cz);
    m[5] = Iwram_MulQ16(sx, cy);
    m[6] = Iwram_MulQ16(Iwram_MulQ16(cx, sy), cz) + Iwram_MulQ16(sx, sz);
    m[7] = Iwram_MulQ16(Iwram_MulQ16(cx, sy), sz) - Iwram_MulQ16(sx, cz);
    m[8] = Iwram_MulQ16(cx, cy);
    m[9] = 0;
    m[10] = 0;
    m[11] = 0;
    Iwram_TransformMatrix(m);
}

/* Each builds one elementary matrix on the identity and hands it to the
   IWRAM transform routine. */
void SceneTransform_ApplyPitch(s32 angle)
{
    s32 matrix[12];
    s32 sin = Trig_Sin(angle);
    s32 cos = Trig_Cos(angle);

    /* CAMELOT_ASM: the fixed-register identity store of TRANSFORM.H */
    Transform_SetIdentity(matrix);
    matrix[4] = cos;
    matrix[5] = sin;
    matrix[7] = -sin;
    matrix[8] = cos;
    Iwram_TransformMatrix(matrix);
}

void SceneTransform_ApplyYaw(s32 angle)
{
    s32 matrix[12];
    s32 sin = Trig_Sin(angle);
    s32 cos = Trig_Cos(angle);

    /* CAMELOT_ASM: the fixed-register identity store of TRANSFORM.H */
    Transform_SetIdentity(matrix);
    matrix[0] = cos;
    matrix[2] = -sin;
    matrix[6] = sin;
    matrix[8] = cos;
    Iwram_TransformMatrix(matrix);
}

void SceneTransform_ApplyRoll(s32 angle)
{
    s32 matrix[12];
    s32 sin = Trig_Sin(angle);
    s32 cos = Trig_Cos(angle);

    /* CAMELOT_ASM: the fixed-register identity store of TRANSFORM.H */
    Transform_SetIdentity(matrix);
    matrix[0] = cos;
    matrix[1] = sin;
    matrix[3] = -sin;
    matrix[4] = cos;
    Iwram_TransformMatrix(matrix);
}

void SceneTransform_ApplyPosition(const s32 *position)
{
    s32 matrix[12];

    /* CAMELOT_ASM: the fixed-register identity store of TRANSFORM.H */
    Transform_SetIdentity(matrix);
    matrix[9] = position[0];
    matrix[10] = position[1];
    matrix[11] = position[2];
    Iwram_TransformMatrix(matrix);
}

void SceneTransform_ApplyScale(const s32 *scale)
{
    s32 matrix[12];

    /* CAMELOT_ASM: the fixed-register identity store of TRANSFORM.H */
    Transform_SetIdentity(matrix);
    matrix[0] = scale[0];
    matrix[4] = scale[1];
    matrix[8] = scale[2];
    Iwram_TransformMatrix(matrix);
}

/* Builds the rotation matrix for the three angles with the given
   translation as its last row and hands it to the IWRAM transform
   routine. */
void SceneTransform_ApplyRotationTranslation(s32 *angles, s32 *position)
{
    s32 sx, cx, sy, cy, sz, cz;
    s32 m[12];

    sx = Trig_Sin(angles[0]);
    cx = Trig_Cos(angles[0]);
    sy = Trig_Sin(angles[1]);
    cy = Trig_Cos(angles[1]);
    sz = Trig_Sin(angles[2]);
    cz = Trig_Cos(angles[2]);
    m[0] = Iwram_MulQ16(cy, cz);
    m[1] = Iwram_MulQ16(cy, sz);
    m[2] = -sy;
    m[3] = Iwram_MulQ16(Iwram_MulQ16(sx, sy), cz) - Iwram_MulQ16(cx, sz);
    m[4] = Iwram_MulQ16(Iwram_MulQ16(sx, sy), sz) + Iwram_MulQ16(cx, cz);
    m[5] = Iwram_MulQ16(sx, cy);
    m[6] = Iwram_MulQ16(Iwram_MulQ16(cx, sy), cz) + Iwram_MulQ16(sx, sz);
    m[7] = Iwram_MulQ16(Iwram_MulQ16(cx, sy), sz) - Iwram_MulQ16(sx, cz);
    m[8] = Iwram_MulQ16(cx, cy);
    m[9] = position[0];
    m[10] = position[1];
    m[11] = position[2];
    Iwram_TransformMatrix(m);
}

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
    Iwram_TransformMatrix(m);
}
