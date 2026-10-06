#ifndef TLA_OVERLAY_654_KABE_H
#define TLA_OVERLAY_654_KABE_H

#include "TYPES.H"
#include "PARTY_STATE.H"
#include "RAM_BUFFER.H"

struct KabeObject {
    u8 unknown_00[8];
    s32 x;
    s32 y;
    s32 z;
    s32 height;
    u8 unknown_18[0xb];
    u8 priority_flags;
    u8 unknown_24[4];
    s32 speed;
    u8 unknown_2c[0x29];
    u8 motion_flags;
    u8 unknown_56[0xc];
    u8 state;
};

struct KabeEventWork {
    u8 unknown_000[0x15a];
    u16 unknown_15a;
    u8 unknown_15c[0x50];
    s32 screen_effect;
};

struct KabeMapLayer {
    u8 unknown_00[0xc];
    s32 y;
    u8 unknown_10[0x28];
};

struct KabeMapWork {
    u8 unknown_000[0x108];
    struct KabeMapLayer layers[3];
};

struct KabeObject *Object_GetById(s32 id);
void WaitFrames(s32 frames);
void Battle_WaitMode0(s32 frames);
s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
void UiText_ShowPositionedMessageAndWait(s32 message, s32 position);
void ObjectMotion_SetActionVariant(s32 actor, s32 variant);
void ObjectMotion_SetSpeedParameters(s32 actor, s32 speed, s32 acceleration);
void ObjectMotion_SetPositionAndReset(s32 actor, s32 x, s32 z);
void ObjectMotion_ArmCallback(s32 actor, s32 angle, s32 frames);
void ObjectMotion_Launch(s32 actor, s32 speed, s32 frames);
void Object_SetPositionAndResetMotion(struct KabeObject *object, s32 x, s32 y, s32 z);
void Object_SetModeById(s32 actor, s32 mode);
void Func_020039d8(void);
void Func_020039e0(void);
void Func_02003b08(s32 skip);
void Func_02003aa0(s32 actor);
void Func_02003998(s32 src_x, s32 src_y, s32 width, s32 height, s32 dest_x, s32 dest_y);
void Func_020039a8(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_02003ae0(s32 actor, s32 value);
void Func_02003b28(void);
void Func_02003b38(const void *script);
void Func_02003b40(void);
void Func_02003b48(s32 cue);
void Func_02001200(s32 show);
void Func_0200142c(struct KabeObject *object);

static inline void Kabe_CopyRect(s32 sx, s32 sy, s32 w, s32 h, s32 dx, s32 dy)
{
    /* FAKEMATCH: the rectangle copy reaches the engine through one inline layer. */
    Func_02003998(sx, sy, w, h, dx, dy);
}

static inline void Kabe_BlendStart(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f)
{
    /* FAKEMATCH: the blend start reaches the engine through one inline layer. */
    Func_020039a8(a, b, c, d, e, f);
}

#endif
