#include "local.h"

extern char SceneId_Ov690Room3;
extern char SceneId_Ov690Room4;
extern char SceneId_Ov690Room6;
extern char SceneId_Ov690Room7;
extern char SceneId_Ov690Room9;

extern const u8 Data_02002272[];

void Object_SetModeById(u32 id, s32 mode);
void Func_02000860(void);
void Func_02000e04(void);
void Func_0200125c(void);
void Func_02002140(s32 a);
void Func_02002188(const void *data);

static __inline__ void Place(s32 id, s32 x, s32 z)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_020020c0(id, x, z);
}

s32 Func_02000988(void)
{
    *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 428) = 0x100;
    if (gPartyState.scene == (s32)&SceneId_Ov690Room7)
        Func_02000860();
    /* FAKEMATCH: the native reads the scene again at each test; without the barrier the read is moved up both paths (differ 56 over the four tests). */
    __asm__("" : : : "memory");
    if (gPartyState.scene == (s32)&SceneId_Ov690Room9) {
        Func_02002140(0);
        Func_02002188(Data_0200226c);
        if (GameFlag_Test(0x206))
            Func_020020c0(10, 0, 0);
        Object_GetById(10)->priorityFlags |= 2;
        Object_SetModeById(18, 2);
        Object_SetModeById(19, 2);
        Object_SetModeById(21, 2);
        if (GameFlag_Test(0x205))
            Object_SetModeById(10, 2);
        Func_02000e04();
#if !defined(TLA_EDITION_JA)
        if (GameFlag_Test(0x204)) {
            Func_020020c0(9, 0, 0);
            Place(21, 0xb80000, 0xb80000);
            Object_GetById(21)->motionFlags = 0;
            CopyCells2(9, 11, 1, 1, 11, 12);
        }
#endif
    }
    if (gPartyState.scene == (s32)&SceneId_Ov690Room4) {
        if (GameFlag_Test(0x230)) {
            CopyCells2(0, 3, 1, 1, 3, 15);
            Object_SetModeById(9, 4);
        }
        if (GameFlag_Test(0x109) && Flag_Test(0x220)) {
            Flag_Clear(0x220);
            Func_0200125c();
        }
        Func_02002188(Data_02002272);
    }
    /* FAKEMATCH: as at the second test. */
    __asm__("" : : : "memory");
    if (gPartyState.scene == (s32)&SceneId_Ov690Room3)
        Func_02002140(0);
    /* FAKEMATCH: as at the second test. */
    __asm__("" : : : "memory");
    if (gPartyState.scene == (s32)&SceneId_Ov690Room6)
        Func_02002140(0);
    return 0;
}
