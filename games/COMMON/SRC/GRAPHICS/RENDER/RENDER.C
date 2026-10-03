#include "RUNTIME_MEM.H"
#include "EDITION.H"
#include "TYPES.H"
#include "TRANSFORM.H"
#include "DMA.H"

/* Lost Age source currently covers rewind, push and the two matrix copies.
   Its reset, pop and identity helpers remain raw until their C matches.
   These game branches describe source presence; both games retain their
   ordinary compiler and options, and each native bank keeps its order. */
#if defined(TLA_EDITION_JA) || defined(TLA_EDITION_EN) || \
    defined(TLA_EDITION_DE) || defined(TLA_EDITION_ES) || \
    defined(TLA_EDITION_FR) || defined(TLA_EDITION_IT)
#include "RAM_BUFFER.H"

void SceneTransform_RewindStack(void)
{
    gTransformStackTop = Ram_HeapSlots->transform_stack;
    gTransformStackDepth = 0;
}

#else
#include "SYSTEM.H"
#include "IWRAM_CALL.H"

s32 Trig_Sin(s32 angle);
s32 Trig_Cos(s32 angle);

typedef s32 (*Dot3Fn)(s32 ax, s32 bx, s32 ay, s32 by, s32 az, s32 bz);
extern const u32 RelocatedQ16DotProduct[];
s32 FixedSqrt(s32 value);

/* Allocates the transform stack's 48-byte slots, empties it and loads the
   identity into the current transform. */
void Render_ResetTransformState(void)
{
    gTransformStackTop = Runtime_AllocateBlock(2, sizeof(gTransform));
    gTransformStackDepth = 0;
    /* CAMELOT_ASM: the fixed-register identity store of TRANSFORM.H */
    Transform_SetIdentity(gTransform);
}

#endif

void Graphics_SaveTransferWorkOnce(void)
{
    if (gTransformStackDepth <= 0) {
        Dma_Set(gTransform, gTransformStackTop, 0x8400000c, (volatile u32 *)0x040000d4);
        gTransformStackDepth++;
        gTransformStackTop += 48;
    }
}

void Graphics_SaveTransferWork(void *destination)
{
    Dma_Set(gTransform, destination, 0x8400000c, (volatile u32 *)0x040000d4);
}

void Graphics_LoadTransferWork(const void *source)
{
    Dma_Set(source, gTransform, 0x8400000c, (volatile u32 *)0x040000d4);
}

#if !(defined(TLA_EDITION_JA) || defined(TLA_EDITION_EN) || \
    defined(TLA_EDITION_DE) || defined(TLA_EDITION_ES) || \
    defined(TLA_EDITION_FR) || defined(TLA_EDITION_IT))
void Graphics_RestoreTransferWork(void)
{
    if (gTransformStackDepth > 0) {
        --gTransformStackDepth;
        gTransformStackTop -= 48;
        Dma_Set(gTransformStackTop, gTransform, 0x8400000c, (volatile u32 *)0x040000d4);
    }
}

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

/* Builds the look-at matrix from the eye and target, using the stack copy
   of the native dot-product routine for normalization and row offsets. */
void Graphics_PrepareTransfer(s32 *eye, s32 *target, s32 *out)
{
    u32 code[7];
    s32 dx, dy, dz;
    s32 ux;
    /* FAKEMATCH: the used eye x component remains in r11 through the row dot calls. */
    register s32 ex asm("r11");
    Dot3Fn dot;
    s32 ey;
    /* FAKEMATCH: the used perpendicular axis stays in r8 through the cross product. */
    register s32 uz asm("r8");
    /* FAKEMATCH: the used third-row x component stays in r10 through normalization. */
    register s32 vx asm("r10");
    s32 vy, vz;
    s32 scale;
    s32 ez;
    s32 rest;
    u32 one = 0x80000000;
    u32 (*divide)(u32 dividend, u32 divisor);

    dot = (Dot3Fn)code;
    Dma_Set(RelocatedQ16DotProduct, code, 0x84000007, (volatile u32 *)0x040000d4);
    dx = target[0] - eye[0];
    dy = target[1] - eye[1];
    dz = target[2] - eye[2];
    rest = Iwram_Sqrt(dot(dx >> 8, dx >> 8, dy >> 8, dy >> 8, dz >> 8, dz >> 8));
    divide = Iwram_UnsignedDivide;
    scale = divide(one, rest) >> 15;
    scale = -scale;
    dx = Iwram_MulQ16(dx, scale);
    dy = Iwram_MulQ16(dy, scale);
    rest = Iwram_MulQ16(dz, scale);
    uz = -dx;
    /* FAKEMATCH: keep the used negated axis before spilling the third product. */
    asm("" : "+r"(rest) : "r"(uz));
    dz = rest;
    rest = Iwram_MulQ16(dy, dy);
    scale = 0x10000;
    if (scale - rest > 0)
        scale = divide(one, FixedSqrt(scale - rest)) << 1;
    ux = Iwram_MulQ16(dz, scale);
    uz = Iwram_MulQ16(uz, scale);
    vx = Iwram_MulQ16(dy, uz);
    vy = Iwram_MulQ16(dz, ux) - Iwram_MulQ16(dx, uz);
    vz = -Iwram_MulQ16(dy, ux);
    scale = divide(one, FixedSqrt(dot(vx, vx, vy, vy, vz, vz))) << 1;
    vx = Iwram_MulQ16(vx, scale);
    vy = Iwram_MulQ16(vy, scale);
    vz = Iwram_MulQ16(vz, scale);
    ex = eye[0];
    ey = eye[1];
    ez = eye[2];
    {
        /* FAKEMATCH: the used zero argument remains in r0 beside the first row stores. */
        register s32 zero asm("r0");
        out[0] = ux;
        zero = 0;
        /* FAKEMATCH: keep the used zero in r0 and delay the used axis copy into r1. */
        asm("" : "+r"(zero) : : "r1");
        out[3] = zero;
        out[6] = uz;
        out[9] = -dot(ex, ux, ez, uz, zero, zero);
    }
    out[4] = vy;
    out[7] = vz;
    out[1] = vx;
    out[10] = -dot(ex, vx, ey, vy, ez, vz);
    out[2] = dx;
    out[5] = dy;
    out[8] = dz;
    out[11] = -dot(ex, dx, ey, dy, ez, dz);
}
#endif
