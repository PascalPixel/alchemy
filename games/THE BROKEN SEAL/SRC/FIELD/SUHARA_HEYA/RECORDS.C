#include "SUHARA.H"

s32 SuharaHeya_SelectEvents(void)
{
    if (Engine_GameFlagIsSet(0x96F) != 0) {
        return (s32)gSuharaHeyaEvents96f;
    }
    return (s32)gSuharaHeyaEvents;
}

s32 Scene_InitActorRecords(void)
{
    union SceneActor *work;
    if (gGameState.entrance == 90)
        Engine_GameFlagSet(0x96f);
    ((s32 *)gEventWork)[112] = 521;
    ((s32 *)gEventWork)[114] = 24;
    ((union SceneActor *)Engine_ActorGet(12))->bytes[89] |= 4;
    ((union SceneActor *)Engine_ActorGet(13))->bytes[89] |= 4;
    work = (union SceneActor *)Engine_ActorGet(20);
    work->fields.record->field_26 = 0;
    work->fields.record->angle = 0x4000;
    {
        /* The mask is built in a local, not folded into the store. */
        struct SceneRecord *record = work->fields.record;
        s32 flags = ~12;

        flags = flags & record->flags;
        record->flags = flags | 4;
    }
    work = (union SceneActor *)Engine_ActorGet(21);
    work->fields.record->field_26 = 0;
    work->fields.record->angle = 0x4000;
    work->bytes[85] = 2;
    work->fields.y = 0;
    return 0;
}
