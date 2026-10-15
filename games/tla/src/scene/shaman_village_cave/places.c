#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "field_scene.h"
#include "object_runtime.h"
#include "party_state.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "call.h"
#include "text/msg_ids.h"

void GameFlag_SetBit();

const struct ScenePlacement *Scene_GetPlacements(void)
{
    return gScenePlacements;
}

void Func_02000358(void)
{
    Call1((void (*)())GameFlag_SetBit, 0x200);
    return;
}

void Func_02000364(void)
{
    Call1((void (*)())GameFlag_SetBit, 0x201);
    return;
}

void Func_02000374(void)
{
    Call1((void (*)())GameFlag_SetBit, 0x202);
    return;
}

void Func_02000384(void)
{
    Call1((void (*)())GameFlag_SetBit, 0x203);
    return;
}
