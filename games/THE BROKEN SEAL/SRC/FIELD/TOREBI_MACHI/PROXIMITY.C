/* Tolbi town: actor proximity, the scene tables and the first dialogue steps. */
#include "MACHI.H"
extern u8 MsgTorebiFaceAwayTolbi[];
extern u8 MsgTorebiTossLuckyMedal[];

s32 SceneActor_UpdatePlayerProximity(struct SceneActor *actor,
                                    struct SceneActor *target,
                                    s32 range, s32 force)
{
    s32 result = 0;
    s32 *targetPos = &target->x;
    s32 *actorPos = &actor->x;

    if (SceneActor_GetPositionDistance(targetPos, actorPos) < range || force != 0) {
        u32 angle = (u16)ArcTan2(target->z - actor->z,
                                            *targetPos - *actorPos);
        u32 farLeft = (angle - 0x2000) & 0xf000;
        u32 farRight = (angle + 0x2000) & 0xf000;
        u32 left = (angle - 0x1000) & 0xf000;
        u32 right = (angle + 0x1000) & 0xf000;
        u32 forward = angle & 0xf000;
        u32 facing = actor->facing & 0xf000;

        if (forward == facing || right == facing || left == facing || force != 0) {
            actor->active = 1;
            Engine_ObjectSetAnimation(actor, 1);
            result = 1;
        }
        if ((u8 *)target == Engine_ActorGet(0) && (farRight == facing || farLeft == facing)) {
            actor->active = 1;
            Engine_ObjectSetAnimation(actor, 1);
            result = 1;
        }
    } else {
        actor->active = 0;
        Engine_ObjectSetAnimation(actor, 2);
    }
    return result;
}

s32 SceneActor_UpdatePartnerProximity(u8 *self)
{
    u8 **globals = (u8 **)gWindowWork;
    u8 *scene = globals[0];
    u8 *work = globals[12];        /* == *(u8 **)0x03001ebc */
    u16 *flags = (u16 *)(self + 100);
    s32 force = 0;
    s32 range = 18;
    u8 *partner;
    u8 *player;

    /*
     * Bit 0 of the actor's own flag halfword selects which partner to test.
     * The branch must stay two calls: as a conditional expression the
     * selector folds into arithmetic on the bit instead.
     */
    if ((*flags & 1) != 0) {
        partner = Engine_ActorGet(17);
    } else {
        partner = Engine_ActorGet(16);
    }
    if (SceneActor_UpdatePlayerProximity(self, partner, 32, 0) != 0) {
        return 0;
    }

    player = Engine_ActorGet(0);

    /*
     * Widen the test when the scene counter at work + 376 is already
     * running, or when the scene byte at scene + 0x0ea4 is set.
     */
    if (*(s16 *)(work + 376) != 0 || scene[0x0ea4] != 0) {
        range = 26;
        if ((*flags & 2) != 0) {
            force = 1;
        }
    }

    SceneActor_UpdatePlayerProximity(self, player, range, force);
    return 0;
}

/*
 * The eight-byte owner includes its one pool word, which holds the address
 * returned here. The word is loaded and returned, never dereferenced.
 */
u8 *TorebiMachi_GetEntrances(void)
{
    return gTorebiMachiEntrances;
}

/* Table slot with no data: reads nothing and returns zero. */
s32 SceneData_ReturnZero(void)
{
    return 0;
}

/* The eight-byte owner includes the pool word holding this address. */
u8 *TorebiMachi_GetExits(void)
{
    return gTorebiMachiExits;
}

/* The eight-byte owner includes the pool word holding this address. */
u8 *TorebiMachi_GetPlacements(void)
{
    return gTorebiMachiPlacements;
}

void FieldScene_RunScene3b5_02000224(void)
{

    u32 i;
    u8 *record;

    record = Value1(Engine_ActorGet, 8);
    if ((s32)record != 0) {
        record[89] = 0;
    }
    record = Engine_ActorGet(8);
    Engine_ActorSetSpriteFlags((s32)record, 0);
    Call4(SetMapCellCollision, 0, 0x2200000, 0x1200000, 253);
    GameFlag_Set(0x200);
}

void ConfigureAndPlaceActorOneHundredTwo(void)
{
    s32 a = 3, b = 26;
    Engine_MapCopyCellAttributes(3, 32, 1, 1, a, b);
    PlaceActor(102, 0x00380000, 0x01a80000);
}

void HideActorOneHundredTwo(void)
{
    s32 a = 3, b = 26;
    Engine_MapCopyCellAttributes(2, 25, 1, 1, a, b);
    PlaceActor_02001104(102, -1, -1);
}

void SceneDialogue_RunMessage0e36(void)
{
    Engine_EventSetMessage((s32)MsgTorebiFaceAwayTolbi);
    Engine_EventShowMessage(-1, 0);
}

void SceneDialogue_RunMessage0e37(void)
{
    Engine_EventSetMessage((s32)MsgTorebiTossLuckyMedal);
    Engine_EventShowMessage(-1, 0);
}
