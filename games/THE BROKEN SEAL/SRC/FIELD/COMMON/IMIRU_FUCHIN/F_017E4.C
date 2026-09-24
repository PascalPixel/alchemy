#include "TYPES.H"

void ImiruFuchin_ApplyRoomLayout();
s32 Engine_GameFlagIsSet();
void FieldScene_RunScene39a_02001b1c();
s32 OverlayObject_CreateAndInitialize();
u8 * Engine_ActorGet();
void Engine_ActorSetSpritePriority();
void Engine_ActorSetChildValue();
void Main_0808a408();
void Engine_MapCopyCellsTo();
void Engine_ColorBufferApplySource();
void Engine_ColorBufferApplyTarget();
void Engine_ColorBufferInterpolate();
void Engine_TaskWait();


extern u8 Data_00000040[];
extern u8 Data_00000043[];
extern u8 Data_02000240[];
extern s16 Data_02000240_t[][1];

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

static __inline__ s32 Value4(s32 (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    return f(a0, a1, a2, a3);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

void ImiruFuchin_Func020017e4(void)
{
    u8 *record;
    s32 v5;
    s32 base7_200b328;

    ImiruFuchin_ApplyRoomLayout();
    if (Data_02000240_t[224][0] == (s32)Data_00000040) {
        if (Value1(Engine_GameFlagIsSet, 0xf13) == 0) {
            if (Data_02000240_t[225][0] == 1) {
                FieldScene_RunScene39a_02001b1c();
            }
        }
        if ((u32)(((u16)Data_02000240_t[225][0] - 2) << 16) > 0x30000) {
            goto L_0200191e;
        }
        OverlayObject_CreateAndInitialize(0x9c0000, 0, 0x1c40000, 223);
        OverlayObject_CreateAndInitialize(0xbc0000, 0, 0x1c40000, 223);
    } else {
        if (Data_02000240_t[224][0] == (s32)Data_00000043) {
            v5 = 0;
            record = Engine_ActorGet(8);
            base7_200b328 = 0x200b328;
            *(s32 *)base7_200b328 = v5;
            record[85] = v5;
            *(s32 *)((s32)record + 12) = v5;
            Engine_ActorSetSpritePriority(8, 1);
            Engine_ActorSetChildValue(8, 15);
            switch (Data_02000240_t[225][0]) {
            case 1:
            case 2:
                Main_0808a408(0);
                *(s32 *)base7_200b328 = 1;
                break;
            case 5:
                Main_0808a408(0);
                *(s32 *)base7_200b328 = 1;
                v5 = *(s32 *)0x03001ee0;
                *(s32 *)(v5 + 24) = 0;
                break;
            }
            if (Data_02000240_t[225][0] <= 6) {
                if (Value1(Engine_GameFlagIsSet, 0x820) != 0) {
                    Call6(Engine_MapCopyCellsTo, 30, 57, 19, 57, 1, 1);
                    Call6(Engine_MapCopyCellsTo, 30, 8, 12, 8, 8, 7);
                } else {
                    *(s32 *)((*(s32 *)0x03001ebc + 0x1c0)) = 0x100;
                    Call2(Engine_ColorBufferApplySource, 0x203108, 1);
                    Call2(Engine_ColorBufferApplyTarget, 0x203108, 1);
                    Engine_ColorBufferInterpolate(1);
                    Engine_TaskWait(1);
                }
            }
        }
    }
    L_0200191e:;
}
