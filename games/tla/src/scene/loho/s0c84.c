#include "types.h"
#include "object_runtime.h"
#include "party_state.h"

extern char SceneId_Ov6A6Room1;
extern char SceneId_Ov6A6Room2;
extern char MsgLoho57;
extern const u8 Data_02001ae0[];
extern const u8 Data_02001aec[];
extern const u8 Data_02001dd4[];

s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
struct ObjectRuntime *Object_GetById(s32 id);
void UiText_ShowPositionedMessageAndWait(s32 message, s32 mode);
void Func_02001348(s32 id, s32 x, s32 z);

static __inline__ void Func_02001348_(s32 id, s32 x, s32 z)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_02001348(id, x, z);
}

void Func_02000c84(void)
{
    GameFlag_SetBit(0x201);
    Object_GetById(22)->collision_flags &= 0xfe;
    if (GameFlag_Test(0xff7) == 0)
        Func_02001348_(66, 0x2080000, 0x880000);
}

const u8 *Func_02000cc0(void)
{
    if (gPartyState.scene == (s32)&SceneId_Ov6A6Room1)
        return Data_02001aec;
    if (gPartyState.scene == (s32)&SceneId_Ov6A6Room2)
        return Data_02001dd4;
    return Data_02001ae0;
}

void Func_02000d00(void)
{
    struct ObjectRuntime *leader = Object_GetById(gPartyState.current_owner);

    /* FAKEMATCH: the one-pass block keeps the angle load before the constant; as plain statements the constant is built first (differ 2) */
    do {
        u16 facing = (leader->angle + 0x2000) & ~0x3fff;

        if (facing == 0xc000)
            UiText_ShowPositionedMessageAndWait((s32)&MsgLoho57, 1);
    } while (0);
}
