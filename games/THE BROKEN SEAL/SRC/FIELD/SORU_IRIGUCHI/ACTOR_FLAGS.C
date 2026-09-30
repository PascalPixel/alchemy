#include "SORU.H"
#include "CALL.H"

void Scene_UpdateOuterActor9Flags(void)
{
    s32 *work = Engine_ActorGet(9);
    s32 pos;

    if (work == 0) {
        return;
    }
    pos = work[2] >> 20;
    Call1(Engine_GameFlagClear, 0x302);
    Call1(Engine_GameFlagClear, 0x303);
    if (pos == 93) {
        Engine_GameFlagSet(0x303);
    } else if (pos == 95) {
        Engine_GameFlagSet(0x302);
    }
}

void Scene_UpdateOuterActor10Flags(void)
{
    s32 *work = Engine_ActorGet(10);
    s32 pos;

    if (work == 0) {
        return;
    }
    pos = work[2] >> 20;
    Call1(Engine_GameFlagClear, 0x300);
    Call1(Engine_GameFlagClear, 0x301);
    if (pos == 115) {
        Engine_GameFlagSet(0x300);
    } else if (pos == 113) {
        Engine_GameFlagSet(0x301);
    }
}

void Scene_UpdateFormationActor9Flags(void)
{
    s32 *work = Engine_ActorGet(9);
    s32 pos;

    if (work == 0) {
        return;
    }
    pos = work[2] >> 20;
    Call1(Engine_GameFlagClear, 0x310);
    Call1(Engine_GameFlagClear, 0x311);
    if (pos == 99) {
        Engine_GameFlagSet(0x311);
    } else if (pos == 101) {
        Engine_GameFlagSet(0x310);
    }
    Scene_RunActorFormation(0);
}

void Scene_UpdateFormationActor10Flags(void)
{
    s32 *work = Engine_ActorGet(10);
    s32 pos;

    if (work == 0) {
        return;
    }
    pos = work[2] >> 20;
    Call1(Engine_GameFlagClear, 0x312);
    Call1(Engine_GameFlagClear, 0x313);
    if (pos == 103) {
        Engine_GameFlagSet(0x313);
    } else if (pos == 105) {
        Engine_GameFlagSet(0x312);
    }
    Scene_RunActorFormation(0);
}

void Scene_UpdateFormationActor11Flags(void)
{
    s32 *work = Engine_ActorGet(11);
    s32 pos;

    if (work == 0) {
        return;
    }
    pos = work[2] >> 20;
    Call1(Engine_GameFlagClear, 0x314);
    Call1(Engine_GameFlagClear, 0x315);
    if (pos == 107) {
        Engine_GameFlagSet(0x315);
    } else if (pos == 109) {
        Engine_GameFlagSet(0x314);
    }
    Scene_RunActorFormation(0);
}

void Scene_UpdateFormationActor12Flags(void)
{
    s32 *work = Engine_ActorGet(12);
    s32 pos;

    if (work == 0) {
        return;
    }
    pos = work[2] >> 20;
    Call1(Engine_GameFlagClear, 0x316);
    Call1(Engine_GameFlagClear, 0x317);
    if (pos == 111) {
        Engine_GameFlagSet(0x317);
    } else if (pos == 113) {
        Engine_GameFlagSet(0x316);
    }
    Scene_RunActorFormation(0);
}

void Scene_UpdateFormationActor13Flags(void)
{
    s32 *work = Engine_ActorGet(13);
    s32 pos;

    if (work == 0) {
        return;
    }
    pos = work[2] >> 20;
    Call1(Engine_GameFlagClear, 0x318);
    Call1(Engine_GameFlagClear, 0x319);
    if (pos == 115) {
        Engine_GameFlagSet(0x319);
    } else if (pos == 117) {
        Engine_GameFlagSet(0x318);
    }
    Scene_RunActorFormation(0);
}

void Scene_UpdateFormationActor14Flags(void)
{
    s32 *work = Engine_ActorGet(14);
    s32 pos;

    if (work == 0) return;
    pos = work[2] >> 20;
    Call1(Engine_GameFlagClear, 0x31a);
    Call1(Engine_GameFlagClear, 0x31b);
    if (pos == 119) {
        Engine_GameFlagSet(0x31b);
    } else if (pos == 121) {
        Engine_GameFlagSet(0x31a);
    }
    Scene_RunActorFormation(0);
}

s32 *SceneActor_FindSlotByTilePosition(s32 x, s32 z)
{

    s32 **slots = (s32 **)(gWork + 0x14);
    u32 i;

    for (i = 8; i <= 65; i++) {
        s32 *p = slots[i];

        if (x == (p[2] >> 20) && z == (p[4] >> 20)) {
            return p;
        }
    }
    return 0;
}

