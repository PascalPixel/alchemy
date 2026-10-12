#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "input.h"
#include "party_state.h"
#include "scene_ids.h"
#include "serial_runtime.h"
#include "text/msg_ids.h"

s32 Func_02000f6c(s32 id);
s32 Func_02000fbc(s32 actor);
void Func_0200191c(void);
void Func_0200197c(void);
void Func_02001984(s32 src_x, s32 src_y, s32 dest_x, s32 dest_y, s32 width, s32 height);
void Func_020019e4(s32 value, s32 digits);
void Func_020019ec(void);
void Func_02001a14(void);
void Func_02001a1c(void);
void Func_02001a64(s32 message);
void Func_02001a74(s32 actor, s32 flags);
void Func_02001a7c(s32 actor, s32 animation, s32 flags);
s32 Func_02001a84(s32 scene, s32 entrance);
void Func_02001ab4(s32 skip);
void ObjectMotion_SetAngleToward(s32 actor, s32 target, s32 frames);
void UiText_ShowPositionedMessageAndWait(const u8 *message, s32 flags);

static __inline__ s32 Flag_Test(s32 flag)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    return GameFlag_Test(flag);
}

static __inline__ void Map_CopyCells(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_02001984(a, b, c, d, e, f);
}

s32 Func_02000f6c(s32 id)
{
    s32 count;
    s32 max;
    s32 i;

    count = Party_CountActiveOwners();
    max = 3;
    if (GameFlag_Test(0x172) == 0) {
        max = 4;
    }
    if (count > max) {
        count = max;
    }
    for (i = 0; i < count; i++) {
        if (gPartyState.active_owners[i] == 0xff) {
            return 0;
        }
        if (gPartyState.active_owners[i] == id) {
            return 1;
        }
    }
    return 0;
}

void Func_02000fb8(void)
{
}

s32 Func_02001414(s32 value)
{
    s32 digit;

    if (value > 999)
        value = 999;
    for (digit = 0; digit <= 2; digit++) {
        Map_CopyCells(27, value % 10, 16 - digit, 8, 1, 1);
        value = value / 10;
    }
    Func_0200197c();
}
