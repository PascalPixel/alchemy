#ifndef ALCHEMY_ORBIT_H
#define ALCHEMY_ORBIT_H

#include "types.h"

typedef struct OrbitingSceneObjectSprite
{
    u8 padding00[5];
    u8 flags05Low : 5;
    u8 flags05Bit5 : 1;
    u8 flags05High : 2;
    u8 padding06[3];
    u8 flags09Low : 2;
    u8 flags09Mode : 2;
    u8 flags09High : 4;
    u8 padding0a[18];
    u8 palette;
    u8 padding1d[10];
    u8 state;
} OrbitingSceneObjectSprite;

typedef struct OrbitingSceneObject
{
    u8                         padding00[8];
    s32                        x;
    s32                        y;
    s32                        z;
    u8                         padding14[15];
    u8                         flags23;
    u8                         padding24[12];
    s32                        orbitAngle;
    u8                         padding34[4];
    s32                        orbitCenterX;
    s32                        orbitCenterY;
    s32                        orbitCenterZ;
    u8                         padding44[12];
    OrbitingSceneObjectSprite *sprite;
    u8                         padding54;
    u8                         mode;
    u8                         state;
    u8                         padding57[5];
    u8                         active;
    u8                         padding5d[4];
    u8                         visible;
    u8                         padding62[10];
    u32                        callback;
} OrbitingSceneObject;

#endif
