/* The item and level debug room: its scene tables, actor 13's steps and the
 * record counts it adjusts. */
#include "LEVEL.H"

u8 *ItemLevel_GetEntrances(void)
{
    return gItemLevelEntrances;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

u8 *ItemLevel_GetExits(void)
{
    return gItemLevelExits;
}

u8 *ItemLevel_GetPlacements(void)
{
    return gItemLevelPlacements;
}

void FieldScene_RunActor13Mode102Step(void)
{
    Call1(Engine_EventSetMessage, 0x23cd);
    Call3(Engine_ActorShowEmote, 13, 0x102, 0);
    Engine_EventShowMessage(13, 0);
}

void FieldScene_RunActor13Mode105Step(void)
{
    Engine_ActorShowEmote(13, 0x105, 0);
    Engine_EventSetMessage(0x23cd);
    Engine_EventShowMessage(13, 0);
}

u8 *ItemLevel_GetEvents(void)
{
    return gItemLevelEvents;
}

void SceneState_AddToRecordCount(s32 id, s32 amount)
{
    u8 *entry = Owner_GetState(id);

    Party_AdvanceOwnerCountToTarget(id, entry[15] + amount);
    Owner_RecalculateStats(id);
}

/* The wrapper keeps the list's address out of r0 until the call. */
static __inline__ s32 Scene_ListRecords(s32 (*func)(u16 *), u16 *list)
{
    return func(list);
}

void Scene_AddToListedRecordCounts(s32 amount)
{
    u16 list[16];
    u16 *p;
    s32 n;

    n = Scene_ListRecords((s32 (*)(u16 *))Party_ListActiveOwners, list);
    if (n > 0) {
        s32 count;
        p = list;
        count = n;
        do {
            SceneState_AddToRecordCount(*p++, amount);
        } while (--count != 0);
    }
}
