/* Draft, differ 2 of 174: the glow that grows over actor 17 and sinks away, written from the
 * listing. Its loops are goto loops: as for loops loop.c hoists their steps into r7 (40 differ).
 * The grow loop's one-pass block keeps the scale load ahead of the 0x666 step, as the native has
 * (4 to 2). Left: the sink loop loads y before it builds the 0x8000 step (ldr r3, [r6, #12];
 * movs r2, #128); the draft builds the step first. A one-pass block around the y load fixes it but
 * holds back the WaitFrames argument and the counter, which the native schedules early (4); an
 * "+l" barrier on y (4), a step variable (2) and alchemy permute 3 minutes (2) did not. */
#include "../../../../games/tla/src/field/overlay_694/ov694.h"

struct Ov694Glow {
    u8 unknown_00[0x0c];
    s32 y;
    u8 unknown_10[8];
    s32 scale_x;
    s32 scale_y;
    u8 unknown_20[0x35];
    u8 motion_flags;
};

void UiText_ShowPositionedMessageAndWait(s32 message, s32 position);
void Func_020041cc(s32 color, s32 b);
void Func_020041d4(s32 color, s32 b);
void Func_020041dc(s32 frames);
void Func_02004264(void);
void Func_0200426c(void);
void Func_02004274(void);
void Func_0200427c(struct Ov694Object *object, s32 mode);
struct Ov694Glow *Func_02003fec(s32 type, s32 x, s32 y, s32 z);
void Func_02003fdc(struct Ov694Glow *object, s32 mode);
void Func_02003ff4(struct Ov694Glow *object);
void ObjectDispatch_SetSingleChildFlags(struct Ov694Glow *object, u32 value);
void Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 priority);
void Scheduler_RemoveCallbackFar(void (*callback)(void));
void Func_02000a80(void);

static inline struct Ov694Glow *Ov694_Create(s32 type, s32 x, s32 y, s32 z)
{
    return Func_02003fec(type, x, y, z);
}

void Func_02000af4(void)
{
    struct Ov694Glow *effect;
    u32 i;

    Func_0200406c();
    Func_02004204(0);
    if (GameFlag_Test(0x9b2))
        UiText_ShowPositionedMessageAndWait((s32)MsgScene694No163, 1);
    else
        UiText_ShowPositionedMessageAndWait((s32)MsgScene694No64, 1);
    Ov694_ObjMode(17, 3);
    Func_020041d4(0x10000, 0);
    Func_020041cc(0x204084, 1);
    Func_020041dc(16);
    Func_02004264();
    Func_0200427c(Object_GetById(17), 2);
    effect = Ov694_Create(280, 0x2c20000, 0, 0x740000);
    if (effect != 0)
    {
        ObjectDispatch_SetSingleChildFlags(effect, 0);
        Func_02003fdc(effect, 2);
        effect->motion_flags = 0;
        effect->scale_x = 0x1999;
        effect->scale_y = 0x1999;
        i = 0;
    grow:
        {
            s32 x;

            do {
                x = effect->scale_x;
            } while (0);
            effect->scale_x = x + 0x666;
        }
        effect->scale_y += 0x666;
        WaitFrames(1);
        i++;
        if (i < 28)
            goto grow;
    }
    Scheduler_AddOrUpdateCallback(Func_02000a80, 0x480);
    Ov694_Wait(120);
    Scheduler_RemoveCallbackFar(Func_02000a80);
    if (effect != 0)
    {
        i = 0;
    shrink:
        effect->scale_x += 0xfffff99a;
        effect->scale_y += 0xfffff99a;
        effect->y += 0x8000;
        WaitFrames(1);
        i++;
        if (i < 28)
            goto shrink;
        Func_02003ff4(effect);
    }
    Func_0200427c(Object_GetById(17), 0);
    Func_02004274();
    Func_0200426c();
    Func_020041cc(0x10000, 1);
    Func_020041dc(16);
    if (GameFlag_Test(0x9b2))
        Ov694_MesSet((s32)MsgScene694No164);
    else
        Ov694_MesSet((s32)MsgScene694No65);
    Ov694_ObjVar(17, 2);
    Ov694_MesPut(17, 0);
    Ov694_ObjMode(17, 1);
    Ov694_Jump(14, 259, 0);
    Ov694_ObjTurn(14, 0x8000, 20);
    Ov694_MesPut(14, 0);
    Ov694_ObjFace(15, 0);
    Ov694_MesPut(15, 0);
    Ov694_ObjMode(14, 3);
    Ov694_ObjModeWait(15, 3);
    Ov694_ObjTurn(15, 0xd000, 0);
    Ov694_ObjFace(14, 0x3000);
    Func_02004074();
}
