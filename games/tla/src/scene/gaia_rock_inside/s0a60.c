#include "types.h"
#include "object_runtime.h"
#include "party_state.h"

extern char SceneId_Ov681Room2;
extern char SceneId_Ov681Room3;
extern char SceneId_Ov681Room4;
extern char SceneId_Ov681Room5;
extern u32 Data_02005970[];
extern u32 Data_020059a0[];
extern s32 Data_02005a54[];
extern u32 Data_02005afc[];
extern u32 Data_02005b8c[];
extern s32 Data_02005c7c[];

/* FAKEMATCH: the native reloads this counter for every use and masks it in the register it was loaded into, as it does a volatile word; declared plain, the mask is built in the constant's register (differ 12). */
extern volatile s32 Data_02005ee4;
extern const u8 Data_02005d6c[];
extern const u8 Data_02005e28[];
extern u8 Data_020023c4[];

void Animation_ApplyChildValues(struct ObjectRuntime *object, s32 value);
void Func_02004dac(struct ObjectRuntime *object, const u8 *script);
void Func_02005014(s32 cue);
struct ObjectRuntime *Object_GetById(u32 id);
u32 Func_02004904(void);
void Func_02004d9c(struct ObjectRuntime *object, s32 mode);
void GameFlag_SetBit(s32 flag);
void WaitFrames(s32 frames);

// 部屋ごとの表を返す
const void *Func_020009e8(void)
{
    if (gPartyState.scene == (s32)&SceneId_Ov681Room2)
        return Data_02005a54;
    if (gPartyState.scene == (s32)&SceneId_Ov681Room3)
        return Data_02005afc;
    if (gPartyState.scene == (s32)&SceneId_Ov681Room4)
        return Data_02005b8c;
    if (gPartyState.scene == (s32)&SceneId_Ov681Room5)
        return Data_02005c7c;
    if (gPartyState.entrance > 8)
        return Data_020059a0;
    return Data_02005970;
}

void Func_02000a60(struct ObjectRuntime *object)
{
    s32 value = 0;

    if ((Data_02005ee4 & 7) <= 0)
        value = 7;
    Animation_ApplyChildValues(object, value & 7);
    if ((Data_02005ee4 & 31) == 0) {
        object->scaleX = 0x10000;
        Func_02004dac(object, Data_02005e28);
        Func_02005014(106);
    }
    Data_02005ee4++;
}

void Func_02000aac(struct ObjectRuntime *object)
{
    s32 value = 0;

    if ((Data_02005ee4 & 7) <= 0)
        value = 7;
    Animation_ApplyChildValues(object, value & 7);
    if ((Data_02005ee4 & 31) == 0) {
        object->scaleY = 0x10000;
        Func_02004dac(object, Data_02005d6c);
        Func_02005014(106);
    }
    Data_02005ee4++;
}

void Func_02000af8(s32 unused, u32 id)
{
    struct ObjectRuntime *object = Object_GetById(id);
    u32 direction;
    /* FAKEMATCH: the state pointer is pinned to r8, loaded through its own variable into r3 behind an empty asm, and the direction is read from r0 after it: the native loads the pointer first after the direction call, where plainly sched2 ranks the mask's movs above it (one reordered pair, differ 2); without the r8 pin the pointer and the flag's address swap r7 and r8 (16), and without the r0 read the direction's copy moves ahead of the mask (2). */
    register s32 *state asm("r8");

    {
        register u32 ret asm("r0") = Func_02004904();
        s32 *address = (s32 *)(Data_020023c4 + 0x288);

        __asm__ volatile("" : : "l"(address));
        direction = ret;
        state = address;
    }
    object->collisionFlags = 0;
    Func_02004d9c(object, ((direction & 0xc000) >> 14) + 2);
    if (direction & 0x4000)
        object->update = (void (*)(void))Func_02000aac;
    else
        object->update = (void (*)(void))Func_02000a60;
    object->collisionFlags = 1;
    GameFlag_SetBit(*state + 0xa50);
    GameFlag_SetBit(0x200);
    WaitFrames(20);
}
