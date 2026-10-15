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
#include "party_state.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "scene_ids.h"


const struct SceneEntrance *Scene_GetEntrances(void)
{
    return gSceneEntrances;
}

const struct SceneRegion *Scene_GetRegions(void)
{
    return 0;
}

const u32 *Scene_GetExits(void)
{
    return gSceneExits;
}

extern const u8 Data_02000228[];
extern const u8 Data_020003a8[];
extern const u8 Data_020003d8[];
extern const u8 Data_020005ac[];
extern const u8 Data_02000780[];

void Func_0200014c(void);
void Func_02000178(s32 a, s32 b);
void Func_02000180(s32 a, s32 b);

const struct ScenePlacement *Func_0200004c(void)
{
    if (gPartyState.scene == (s32)&SceneId_Ov683
        && (u16)(gPartyState.entrance - 8) <= 1)
        return (const struct ScenePlacement *)Data_020003a8;
    return (const struct ScenePlacement *)Data_02000228;
}

void Func_0200008c(s32 value)
{
    Func_02000178(25, value);
}

void Func_02000098(s32 value)
{
    Func_02000180(10, value);
}

const struct SceneEvent *Func_020000a4(void)
{
    if (gPartyState.scene == (s32)&SceneId_Ov683
        && (u16)(gPartyState.entrance - 8) <= 1)
        return (const struct SceneEvent *)Data_02000780;
    if (GameFlag_Test(0x97f))
        return (const struct SceneEvent *)Data_020005ac;
    return (const struct SceneEvent *)Data_020003d8;
}

s32 Func_020000fc(void)
{
    struct EventWork *work = Ram_HeapSlots->eventWork;

#ifdef TLA_EDITION_JA
    work->screenEffect = 0x201;
#else
    work->screenEffect = 0x209;
#endif
    if (gPartyState.scene == (s32)&SceneId_Ov683
        && (u16)(gPartyState.entrance - 8) <= 1)
        Func_0200014c();
    return 0;
}
