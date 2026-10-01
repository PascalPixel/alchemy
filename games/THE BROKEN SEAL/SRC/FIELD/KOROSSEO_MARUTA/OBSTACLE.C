/* Marking the stage's progress and choosing the nearest obstacle. */
#include "LOG_ROLLING.H"
extern u8 MsgKorosseoMatchAboutBeginPleaseTake[];

void ColossoLogRollingStage_MarkSceneProgress(void)
{

    u8 *state;
    s16 *table;
    s32 slotValue;
    s32 value;
    u16 *field;

    state = *(u8 **)&gEventWork;
    table = (s16 *)&gGameState;
    slotValue = *(s32 *)&table[250];
    if (slotValue != 0) {
        if ((s16)*(u16 *)(state + 382) >> 10 == slotValue) {
            if (GameFlag_IsSet(0x141) != 0) {
                field = (u16 *)(state + 386);
                value = 99;
                *field = value;
            }
        }
    }
}

void ColossoLogRollingStage_SelectNearestObstacle(void)
{
    extern void GameFlag_SetByte();


    u8 *state;
    s16 *table;
    StageActor *target;
    StageActor *actor;
    s32 *frame;
    s32 active_slot;
    s32 slot;
    s32 best;
    s32 best_slot;
    s32 dx;
    s32 adx;
    s32 dz;
    s32 base;
    s32 z;

    state = *(u8 **)&gEventWork;
    best_slot = 8;
    best = 0x100000;
    table = (s16 *)&gGameState;
    active_slot = *(s32 *)&table[250];
    target = Object_GetById(active_slot);
    Engine_EventBegin();
    for (slot = 8; slot <= 66; slot++) {
        actor = Object_GetById(slot);
        if (actor == 0) {
            continue;
        }
        if (actor->state != 1) {
            continue;
        }
        if (*actor->sprite->entry != 165) {
            continue;
        }
        dx = (target->x - actor->x) / 65536;
        dz = (target->z - actor->z) / 65536;
        if (dz > 0) {
            continue;
        }
        adx = dx;
        if (adx < 0) {
            adx = -adx;
        }
        if (dz < 0) {
            dz = -dz;
        }
        if (adx + dz < best) {
            best_slot = slot;
            best = adx + dz;
        }
    }
    Engine_EventSetMessage((s32)MsgKorosseoMatchAboutBeginPleaseTake);
    Event_ShowMessage(best_slot, 0);
    frame = (s32 *)(state + 448);
    *frame = 0x200;
    *(s32 *)(state + 456) = 15;
    Engine_EventWait(20);
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    base = active_slot * 16;
    GameFlag_SetByte(base + 880, target->x >> 20);
    z = target->z >> 20;
    GameFlag_SetByte(base + 888, z);
    active_slot = active_slot + 1;
    if (active_slot > 3) {
        Engine_EventRequestExit(10);
        GameFlag_Set(282);
    } else {
        Korosseo_SelectSoloCompetitor(active_slot);
        Engine_EventOpenScreen();
        Engine_EventWaitForScreen();
        *frame = 0;
    }
    Engine_EventEnd();
}
