/* Declarations shared by the Bilibin palace garden's sources (resource_38e). */
#ifndef NIWA_H
#define NIWA_H

#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

#define NULL ((void *)0)

#include "FACING_OBJECT.H"

enum {
    /* Message 0x182 + 181. */
    ITEM_NUT = 181
};

enum OrbitingMessage {
    MSG_ROBIN_PEERED_INTO = 0x947,
    MSG_TELLING_ME_IM_RESPONSIBLE_FOR = 0x13c3,
    MSG_DO_THINK_CAN_BECOME_AS = 0x1751,
    MSG_UPON_CLOSER_INSPECTION_SEEMS_DRIED = 0x29de
};

struct SceneHandle {
    u8 unknown_00[9];
    u8 flags09;                     /* 0x09 */
};

struct SceneEntity {
    u8 unknown_00[0x23];
    u8 fp;                     /* 0x23 */
    u8 unknown_24[0x2c];
    struct SceneHandle *h;     /* 0x50 */
};

/* Entity and handle as the lobe-orbit callback reads them. */
struct SceneHandle_0200090c {
    u8 unknown_00[30];
    s16 field1e;                    /* 0x1e */
};

struct SceneEntity_0200090c {
    u8 unknown_00[8];
    s32 x;                          /* 0x08 */
    s32 y;                          /* 0x0c */
    u8 unknown_10[0x20];
    s32 phase;                      /* 0x30 */
    u8 unknown_34[4];
    s32 origin_x;                    /* 0x38 */
    s32 origin_y;                    /* 0x3c */
    u8 unknown_40[0x10];
    struct SceneHandle_0200090c *handle;     /* 0x50 */
};

typedef struct OrbitingSceneObjectSprite {
    u8 padding_00[5];
    u8 flags_05_low : 5;
    u8 flags_05_bit_5 : 1;
    u8 flags_05_high : 2;
    u8 padding_06[3];
    u8 flags_09_low : 2;
    u8 flags_09_mode : 2;
    u8 flags_09_high : 4;
    u8 padding_0a[18];
    u8 palette;
    u8 padding_1d[10];
    u8 state;
} OrbitingSceneObjectSprite;

typedef struct OrbitingSceneObject {
    u8 padding_00[8];
    s32 x;
    s32 y;
    u8 padding_10[19];
    u8 flags_23;
    u8 padding_24[12];
    s32 orbit_angle;
    u8 padding_34[4];
    s32 orbit_center_x;
    s32 orbit_center_y;
    u8 padding_40[16];
    OrbitingSceneObjectSprite *sprite;
    u8 padding_54;
    u8 mode;
    u8 state;
    u8 padding_57[5];
    u8 active;
    u8 padding_5d[4];
    u8 visible;
    u8 padding_62[10];
    u32 callback;
} OrbitingSceneObject;

/* The scene's tables and the gate's animated cell steps, laid out after
 * the code. */
extern u8 Placement_Scripts[];
extern u8 Placement_Messages[];
extern const u16 Niwa_GateCells[];


s32 ArcTan2(s32, s32);
void FieldScene_OpenGate(void);

/* FAKEMATCH: Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

#endif
