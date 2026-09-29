#include "TASK.H"

/*
 * One fixed line, then three whose fifth or sixth argument is a field of the
 * record fetched for participants 15, 16 and 17.  Records 15 and 16 contribute
 * their word at +8, record 17 its word at +16, which moves to the sixth
 * argument slot while a literal 18 takes the fifth.  The shift is arithmetic,
 * so the fields are signed fixed-point with 20 fractional bits.  Only those two
 * fields are asserted; what the six arguments mean is not established here.
 */
void SceneState_ApplyRectsForActors15To17(void)
{
    s32 field;

    Map_CopyCellAttributes(100, 11, 12, 4, 14, 11);

    field = ((s32 *)Engine_ActorGet(15))[2] >> 20;
    Map_CopyCellAttributes(13, 28, 1, 4, field, 11);

    field = ((s32 *)Engine_ActorGet(16))[2] >> 20;
    Map_CopyCellAttributes(13, 28, 1, 4, field, 11);

    field = ((s32 *)Engine_ActorGet(17))[4] >> 20;
    Map_CopyCellAttributes(13, 28, 4, 1, 18, field);
}

void FieldScene_RunStep15At29By26(void)
{
    KorosseoKabe_RollLogToCell(15, 29, 26);
    SceneState_ApplyRectsForActors15To17();
}

void FieldScene_RunStep15At33By26(void)
{
    KorosseoKabe_RollLogToCell(15, 33, 26);
    SceneState_ApplyRectsForActors15To17();
}

void FieldScene_RunStep16At45By26(void)
{
    KorosseoKabe_RollLogToCell(16, 45, 26);
    SceneState_ApplyRectsForActors15To17();
}

void FieldScene_RunStep16At49By26(void)
{
    KorosseoKabe_RollLogToCell(16, 49, 26);
    SceneState_ApplyRectsForActors15To17();
}

void FieldScene_RunStep17At40By23(void)
{
    KorosseoKabe_RollLogToCell(17, 40, 23);
    SceneState_ApplyRectsForActors15To17();
}

void FieldScene_RunStep17At40By25(void)
{
    KorosseoKabe_RollLogToCell(17, 40, 25);
    SceneState_ApplyRectsForActors15To17();
}

void FieldScene_RunSupplementalSequenceOne(void)
{
    s32 i;
    s32 rec8;
    s32 rec7;
    s32 xa;
    s32 xb;
    s32 xd;
    s32 ya;
    s32 yb;
    s32 yd;
    s32 record;

    rec8 = (s32)Engine_ActorGet(gGameState.selected_actor);
    for (i = 22; i <= 25; i++) {
        rec7 = Value1(Engine_ActorGet, i);
        *(u8 *)(rec7 + 91) = 0;
        xa = *(s32 *)(rec7 + 8);
        xb = *(s32 *)(rec8 + 8);
        xd = xa - xb;
        if (xd >= 0) {
            if (xd > 0x9ffff) {
                continue;
            }
        } else {
            xb = xb - xa;
            if (xb > 0x9ffff) {
                continue;
            }
        }
        ya = *(s32 *)(rec7 + 16);
        yb = *(s32 *)(rec8 + 16);
        yd = ya - yb;
        if (yd >= 0) {
            if (yd > 0x9ffff) {
                continue;
            }
        } else {
            yb = yb - ya;
            if (yb > 0x9ffff) {
                continue;
            }
        }
        if (GameFlag_IsSet(0x104) != 0) {
            yb = *(s32 *)(rec7 + 16);
        } else {
            yb = *(s32 *)(rec8 + 16);
            yd = *(s32 *)(rec7 + 44);
            yb = yb + yd;
        }
        *(s32 *)(rec8 + 16) = yb;
    }
    if (KorosseoKabe_SpectatorTimer != 0
        && *(s32 *)(rec7 + 56) == (s32)0x80000000) {
        if (KorosseoKabe_SpectatorPhase == 0) {
            Map_CopyCellAttributes(58, 28, 7, 1, 58, 13);
        } else {
            Map_CopyCellAttributes(58, 10, 1, 1, 58, 11);
        }
    } else {
        Map_CopyCellAttributes(57, 11, 1, 1, 58, 11);
        Map_CopyCellAttributes(58, 14, 7, 1, 58, 13);
    }
    if (KorosseoKabe_SpectatorTimer == 0) {
        KorosseoKabe_SpectatorPhase ^= 1;
        if (KorosseoKabe_SpectatorPhase != 0) {
            record = (s32)Engine_ActorGet(22);
            Call4(Engine_ObjectSetPosition, record, 0x3a80000, 0, 0xb80000);
            record = (s32)Engine_ActorGet(23);
            Call4(Engine_ObjectSetPosition, record, 0x3c80000, 0, 0xf80000);
            record = (s32)Engine_ActorGet(24);
            Call4(Engine_ObjectSetPosition, record, 0x3e80000, 0, 0xb80000);
            record = (s32)Engine_ActorGet(25);
            Call4(Engine_ObjectSetPosition, record, 0x4080000, 0, 0xf80000);
            Actor_SetAnimation(31, 11);
        } else {
            record = (s32)Engine_ActorGet(22);
            Call4(Engine_ObjectSetPosition, record, 0x3a80000, 0, 0xd80000);
            record = (s32)Engine_ActorGet(23);
            Call4(Engine_ObjectSetPosition, record, 0x3c80000, 0, 0xd80000);
            record = (s32)Engine_ActorGet(24);
            Call4(Engine_ObjectSetPosition, record, 0x3e80000, 0, 0xd80000);
            record = (s32)Engine_ActorGet(25);
            Call4(Engine_ObjectSetPosition, record, 0x4080000, 0, 0xd80000);
            Actor_SetAnimation(31, 10);
        }
    }
    KorosseoKabe_SpectatorTimer++;
    if (KorosseoKabe_SpectatorTimer > 119) {
        if (GameFlag_IsSet(0x104) == 0) {
            KorosseoKabe_SpectatorTimer = 0;
        }
    }
}

void FieldScene_PlaceSpectatorRow(void)
{
    KorosseoKabe_SpectatorTimer = 0;
    KorosseoKabe_SpectatorPhase = 0;
    Call1(Engine_TaskRemoveCallback, 0x2008715);
    Actor_SetPosition(22, 0x3a80000, 0xd80000);
    Actor_SetPosition(23, 0x3c80000, 0xd80000);
    Actor_SetPosition(24, 0x3e80000, 0xd80000);
    Actor_SetPosition(25, 0x4080000, 0xd80000);
    Actor_SetAnimation(31, 10);
}

void SceneState_ApplyTable8715AndValue104(void)
{
    Engine_TaskAddCallback(0x2008715, 0xC85);
    GameFlag_Clear(0x104);
}

/*
 * Spin until the first status word reaches zero with the second equal to 75,
 * giving up after 600 polls. Both words are re-read on every pass, because the
 * poll call lets the task that publishes them run.
 * The plain while loop is the spelling that reproduces the reference. What the
 * two words mean is not established here -- only that another task publishes
 * them while this owner spins.
 */
void SceneState_WaitForStatusWords(void)
{
    s32 cnt;

    /* The frame count is a literal ten. */
    Task_Wait(10);

    cnt = 0;
    while (KorosseoKabe_SpectatorPhase != 0 || KorosseoKabe_SpectatorTimer != 75) {
        Task_Wait(1);
        cnt++;
        if (cnt >= 600) {
            return;
        }
    }
}

void SceneState_InstallTask8714AndApplyTwoRects(void)
{
    BattleEffect_PauseObject(31);
    GameFlag_Set(820);                 /* 205 << 2 */

    if (KorosseoKabe_SpectatorPhase != 0) {
        KorosseoKabe_SpectatorTimer = 0;
    }

    Task_Wait(30);
    Task_Wait(1);

    /* The task is published as its entry address with the Thumb bit set. The
     * `.thumb_set` alias the exact reconstruction emits for a Thumb symbol already carries
     * bit 0, so adding it again here overshoots by one. */
    Engine_TaskRemoveCallback((s32)FieldScene_RunSupplementalSequenceOne);

    Map_CopyCellAttributes(58, 28, 7, 1, 58, 13);
    Map_CopyCellAttributes(57, 11, 1, 1, 58, 11);
}
