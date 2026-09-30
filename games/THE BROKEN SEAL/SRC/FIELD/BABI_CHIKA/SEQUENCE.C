/* Platform landing and the late sequences. */
#include "BABI.H"
#include "CALL.H"

/*
 * Scan slots 10 to 14, skipping the subject, keep those whose whole-tile x
 * and z match the subject's, and take the greatest height among them; the
 * winning slot index goes into the subject's tag at +100. The starting best
 * height is -5.0 in 12.20, held in the owner's one pool word, and it is what
 * the move receives when no slot matches -- the tag is then left untouched.
 * Every field read re-fetches its record, which is the shape to keep.
 */
void SceneActor_LandOnHighestPlatform(s32 subject)
{
    s32 best = (s32)0xffb00000;
    u32 i;

    for (i = 0; i <= 4; i++) {
        s32 slot = i + 10;

        if (slot == subject) continue;

        if ((((Slot_02001c2c* (*)())Object_GetById)(slot)->x >> 20) != (((Slot_02001c2c* (*)())Object_GetById)(subject)->x >> 20)) continue;
        if ((((Slot_02001c2c* (*)())Object_GetById)(slot)->z >> 20) != (((Slot_02001c2c* (*)())Object_GetById)(subject)->z >> 20)) continue;

        /*
         * 0x00100000 is one whole unit above the candidate's own height. The
         * comparison is signed, and a tie updates the best.
         */
        if (best > ((Slot_02001c2c* (*)())Object_GetById)(slot)->y + 0x100000) continue;

        best = ((Slot_02001c2c* (*)())Object_GetById)(slot)->y + 0x100000;
        *(u16 *)((u8 *)Actor_Get(subject) + 100) = (u16)slot;
    }

    Actor_SetSpeed(subject, 0x40000, 0x20000);   /* 128 << 11, 128 << 10 */

    /*
     * Three separate lookups of the same record, in this order. The locals
     * fix the sequence, which argument evaluation order would not.
     */
    {
        Slot_02001c2c *target = Actor_Get(subject);
        Slot_02001c2c *from = Actor_Get(subject);
        s32 z = ((Slot_02001c2c* (*)())Object_GetById)(subject)->z;

        Object_SetPosition(target, from->x, best, z);
    }

    Actor_WaitForMove(subject);
    Audio_PlayCue(188);
    SceneEffect_SpawnNineRadialEffects(subject);
    Event_Wait(30);
}

void FieldScene_RunMiddleSequence(void)
{
    struct FieldActor saved;
    s32 i;
    s32 j;
    s32 found;

    Event_Begin();
    for (i = 0; i <= 2; i++) {
        if (((struct FieldActor *)Object_GetById(i + 12))->sprite->priority == 3
            && GameFlag_IsSet(i + 0x200) == 0) {
            Actor_Get(i + 12);
            SceneActor_WaitValueBelowLimit();
            Actor_SetPosition(i + 12, 0, 0);
            GameFlag_Set(i + 0x200);
            break;
        }
        if ((((struct FieldActor *)Object_GetById(i + 12))->z.fixed >> 20) == 9
            && GameFlag_IsSet(i + 0x200) == 0) {
            *(s32 *)(Value1(Object_GetById, i + 12) + 20) = 0;
            ((struct FieldActor *)Object_GetById(i + 12))->velocity_y = 0;
            *(s32 *)(((s32 (*)())Object_GetById)(i + 12) + 60) = -0x80000000;
            ((struct FieldActor *)Actor_Get(i + 12))->motion_flags = 0;
            *(u16 *)(((s32 (*)())Object_GetById)(i + 12) + 100) = 0;
            found = i;
            for (j = 0; j < i; j++) {
                if (GameFlag_IsSet(0x200 + j) == 0) {
                    saved.x.fixed = ((struct FieldActor *)Object_GetById(i + 12))->x.fixed;
                    saved.y.fixed = ((struct FieldActor *)Object_GetById(i + 12))->y.fixed;
                    saved.z.fixed = ((struct FieldActor *)Object_GetById(i + 12))->z.fixed;
                    ((struct FieldActor *)Actor_Get(i + 12))->x.fixed =
                        ((struct FieldActor *)Object_GetById(j + 12))->x.fixed;
                    ((struct FieldActor *)Object_GetById(i + 12))->y.fixed =
                        ((struct FieldActor *)Object_GetById(j + 12))->y.fixed;
                    ((struct FieldActor *)Object_GetById(i + 12))->z.fixed =
                        ((struct FieldActor *)Object_GetById(j + 12))->z.fixed;
                    ((struct FieldActor *)Actor_Get(j + 12))->x.fixed = saved.x.fixed;
                    ((struct FieldActor *)Actor_Get(j + 12))->y.fixed = saved.y.fixed;
                    ((struct FieldActor *)Actor_Get(j + 12))->z.fixed = saved.z.fixed;
                    found = j;
                    break;
                }
            }
            *(s32 *)(((s32 (*)())Object_GetById)(found + 12) + 20) = 0;
            ((struct FieldActor *)Actor_Get(found + 12))->velocity_y = 0;
            *(s32 *)(((s32 (*)())Object_GetById)(found + 12) + 60) = -0x80000000;
            ((struct FieldActor *)Actor_Get(found + 12))->motion_flags = 0;
            *(u16 *)(((s32 (*)())Object_GetById)(found + 12) + 100) = 0;
            Engine_CameraSetSpeed(0x30000, 0x6000);
            ((struct FieldActor *)Battle_GetWorkObject1e0())->motion_flags = 0;
            Camera_MoveTo(0xa80000, 0x80000, 0xb80000, 1);
            Camera_WaitForMove();
            SceneActor_LandOnHighestPlatform(found + 12);
            if ((((struct FieldActor *)Object_GetById(found + 12))->x.fixed >> 20) == 8) {
                (*(s16 *)(((s32 (*)())Object_GetById)(10) + 100))++;
                (*(s16 *)(((s32 (*)())Object_GetById)(11) + 100))--;
            } else {
                (*(s16 *)(((s32 (*)())Object_GetById)(10) + 100))--;
                (*(s16 *)(((s32 (*)())Object_GetById)(11) + 100))++;
            }
            ((struct FieldActor *)Actor_Get(found + 12))->update = (void (*)(union FieldObject *))OverlayObject_SetYAboveLinkedActor;
            BabiChika_SettleSteps(40);
            ((struct FieldActor *)Actor_Get(found + 12))->priority_flags |= ACTOR_PRIORITY_UNDERFOOT;
            GameFlag_Set(0x200 + found);
            break;
        }
    }
    Event_End();
}

void FieldScene_RunThreeStepSequence(void)
{
    typedef s32(*Handler_02001a10)(struct Record_02000ec8 *record);

    Event_Begin();
    StagedActor_AdvancePair();
    Event_End();
}

void SceneState_SetSlot17And18Selectors(void)
{
    Event_Begin();

    if ((((Slot_02001f70* (*)())Object_GetById)(17)->w8 >> 20) == 45) {
        GameFlag_Set(0x974);
    } else {
        GameFlag_Clear(0x974);
    }

    if ((((Slot_02001f70* (*)())Object_GetById)(18)->w8 >> 20) == 46) {
        GameFlag_Set(0x975);
    } else {
        GameFlag_Clear(0x975);
    }

    BabiChika_MarkActorCells();
    Event_End();
}

void FieldScene_RunFourCallSequence(void)
{
    typedef s32(*Handler_02001a10)(struct Record_02000ec8 *record);

    Event_Begin();
    StagedActor_AdvancePair();
    SceneState_SetSlot17And18Selectors();
    Event_End();
}

/* Runs six queued setup calls for this scene: two paired 6-argument calls
 * whose last two args repeat the first two (72,49 / 113,43), two 3-argument
 * id calls (100, 101) with zeroed remaining args, and two more 3-argument
 * calls (id 15, 16) passing a start/end pair where the second call's start
 * value (198<<18) equals the first call's end value. */
void FieldScene_RunLateSequenceHead(void)
{
    Map_CopyCellAttributes(72, 49, 1, 1, 8, 49); /* main:080091c0 */
    Map_CopyCellAttributes(113, 43, 1, 1, 49, 43); /* main:080091c0 */
    MapObject_SetPosition(100, 0, 0);
    MapObject_SetPosition(101, 0, 0);
    Actor_SetPosition(15, 8912896, 51904512); /* 136<<16, 198<<18 */
    Actor_SetPosition(16, 51904512, 45613056); /* 198<<18, 174<<18 */
}

/* Two six-argument calls whose first and fifth arguments repeat the same id
 * (8 and 49 respectively), followed by four three-argument calls each keyed
 * by an id with a trailing pair of values (-1, -1 or 0, 0). */
void FieldScene_RunLateSequenceSecond(void)
{
    Map_CopyCellAttributes(8, 113, 1, 1, 8, 49);
    Map_CopyCellAttributes(49, 107, 1, 1, 49, 43);
    MapObject_SetPosition(100, -1, -1);
    MapObject_SetPosition(101, -1, -1);
    Actor_SetPosition(15, 0, 0);
    Actor_SetPosition(16, 0, 0);
}

void FieldScene_RunScene3c4SequenceA(void)
{
    s32 record;
    s32 v6;
    s32 p5;
    s32 q;

    v6 = 0;
    Engine_EventBegin();
    Map_CopyCellAttributes(83, 45, 11, 8, 19, 45);
    record = Object_GetById(19);
    p5 = *(s32 *)(record + 8);
    q = *(s32 *)(Value1(Object_GetById, 19) + 16);
    q >>= 20;
    p5 >>= 20;
    Map_CopyCellAttributes(20, 56, 1, 1, p5, q);
    record = Object_GetById(20);
    p5 = *(s32 *)(record + 8);
    q = *(s32 *)(Value1(Object_GetById, 20) + 16);
    q >>= 20;
    p5 >>= 20;
    Map_CopyCellAttributes(20, 56, 1, 1, p5, q);
    record = Object_GetById(21);
    p5 = *(s32 *)(record + 8);
    q = *(s32 *)(Value1(Object_GetById, 21) + 16);
    q >>= 20;
    p5 >>= 20;
    Map_CopyCellAttributes(20, 56, 1, 1, p5, q);
    record = Object_GetById(22);
    p5 = *(s32 *)(record + 8);
    q = *(s32 *)(Value1(Object_GetById, 22) + 16);
    q >>= 20;
    p5 >>= 20;
    Map_CopyCellAttributes(20, 56, 1, 1, p5, q);
    record = Object_GetById(23);
    p5 = *(s32 *)(record + 8);
    q = *(s32 *)(Value1(Object_GetById, 23) + 16);
    q >>= 20;
    p5 >>= 20;
    Map_CopyCellAttributes(20, 56, 1, 1, p5, q);
    record = Object_GetById(19);
    if ((*(s32 *)(record + 8) >> 20) == 25) {
        record = Object_GetById(19);
        if ((*(s32 *)(record + 16) >> 20) == 49) {
            v6 = 1;
        }
    }
    record = Object_GetById(20);
    if ((*(s32 *)(record + 8) >> 20) == 23) {
        record = Object_GetById(20);
        if ((*(s32 *)(record + 16) >> 20) == 49) {
            v6 = (v6 + 1);
        }
    }
    record = Object_GetById(21);
    if ((*(s32 *)(record + 8) >> 20) == 25) {
        record = Object_GetById(21);
        if ((*(s32 *)(record + 16) >> 20) == 47) {
            v6 = (v6 + 1);
        }
    }
    record = Object_GetById(22);
    if ((*(s32 *)(record + 8) >> 20) == 23) {
        record = Object_GetById(22);
        if ((*(s32 *)(record + 16) >> 20) == 47) {
            v6 = (v6 + 1);
        }
    }
    record = Object_GetById(23);
    if ((*(s32 *)(record + 8) >> 20) == 24) {
        record = Object_GetById(23);
        if ((*(s32 *)(record + 16) >> 20) == 48) {
            v6 = (v6 + 1);
        }
    }
    if (v6 == 5) {
        if (GameFlag_IsSet(0x984) != 0) {
            Event_End();
            goto done;
        }
        Event_Wait(20);
        Camera_SetSpeed(0xcccc, 0x1999);
        Camera_MoveTo(0x1d80000, -1, 0x30c0000, 1);
        Camera_WaitForMove();
        Event_Wait(30);
        GameFlag_Set(0x984);
        Audio_PlayCue(158);
        Map_AnimateCells(Data_0200b3ec, 32, 46);
        Map_CopyCellAttributes(24, 60, 1, 1, 32, 47);
        Event_Wait(40);
    } else {
        if (GameFlag_IsSet(0x984) != 0) {
            Event_Wait(20);
            Camera_SetSpeed(0xcccc, 0x1999);
            Camera_MoveTo(0x1d80000, -1, 0x30c0000, 1);
            Camera_WaitForMove();
            Event_Wait(30);
            GameFlag_Clear(0x984);
            Audio_PlayCue(159);
            Map_AnimateCells(Data_0200b40c, 32, 46);
            Map_CopyCellAttributes(31, 47, 1, 1, 32, 47);
            Event_Wait(40);
        }
    }
    Event_End();
    done:;
}

void FieldScene_RunLayoutAt83By45(void)
{
    Event_Begin();
    {
        s32 width = 19;
        s32 height = 45;

        Map_CopyCellAttributes(83, 45, 11, 8, width, height);
    }
    StagedActor_AdvancePair();
    FieldScene_RunScene3c4SequenceA();
    Event_End();
}

void SceneState_SetValue268bInScene(void)
{
    Event_Begin();
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 1);
    Message_ShowCentered(MSG_STATUE_SEEMS_SPEAK_YOUR_SOUL, 1);
    Event_End();
}

void FieldScene_RunScriptedStep953(void)
{
    Event_Begin();
    Message_ShowCentered(MSG_DOOR_TIGHTLY_LOCKED, 1);
    Event_End();
}

s32 SceneData_SelectTableByWord224(void)
{
    if (gGameState.scene == (s32)&SceneId_BabiChika1) {
        return (s32)Data_0200bc0c;
    }
    return (s32)Data_0200bef4;
}

void FieldScene_PlaceAndPinSlots8And9(void)
{
    typedef s32(*Handler_02001a10)(struct Record_02000ec8 *record);

    s32 row;

    {
        s32 p5 = 9, p6 = 38;
        Map_CopyCellAttributes(73, 38, 5, 5, p5, p6);
    }
    SceneState_SwapSlotPairByRank(9, 8);

    {
        s32 col = ((Slot_020023a0* (*)())Object_GetById)(8)->column >> 20;
        row = ((Slot_020023a0* (*)())Object_GetById)(8)->row >> 20;
        Map_CopyCellAttributes(2, 36, 1, 1, col, row);
    }

    {
        s32 col = ((Slot_020023a0* (*)())Object_GetById)(9)->column >> 20;
        row = ((Slot_020023a0* (*)())Object_GetById)(9)->row >> 20;
        Map_CopyCellAttributes(2, 36, 1, 1, col, row);
    }
}

void FieldScene_PlaceAndPinSlots10And11(void)
{
    s32 row;

    {
        s32 k5 = 29, k6 = 30;
        Map_CopyCellAttributes(93, 30, 6, 5, k5, k6);
    }
    SceneState_SwapSlotPairByRank(11, 10);

    {
        s32 col20 = ((Slot_02002410* (*)())Object_GetById)(10)->column >> 20;
        row = ((Slot_02002410* (*)())Object_GetById)(10)->row >> 20;
        Map_CopyCellAttributes(2, 36, 1, 1, col20, row);
    }

    {
        s32 col20 = ((Slot_02002410* (*)())Object_GetById)(11)->column >> 20;
        row = ((Slot_02002410* (*)())Object_GetById)(11)->row >> 20;
        Map_CopyCellAttributes(2, 36, 1, 1, col20, row);
    }
}

void FieldScene_RunScene3c4_02002480(void)
{
    s32 record;
    s32 p5;

    Map_CopyCellAttributes(89, 49, 3, 2, 25, 49);
    Map_CopyCellAttributes(89, 51, 8, 5, 25, 51);
    *(u8 *)(((s32 (*)())Object_GetById)(14) + 34) = 1;
    record = Object_GetById(12);
    p5 = *(s32 *)(record + 8);
    record = Object_GetById(12);
    p5 = p5 >> 20;
    Map_CopyCellAttributes(22, 52, 1, 1, p5, (*(s32 *)(record + 16) >> 20));
    record = Object_GetById(13);
    p5 = *(s32 *)(record + 8);
    record = Object_GetById(13);
    p5 = p5 >> 20;
    Map_CopyCellAttributes(22, 52, 1, 1, p5, (*(s32 *)(record + 16) >> 20));
    record = Object_GetById(14);
    p5 = *(s32 *)(record + 8);
    record = Object_GetById(14);
    p5 = p5 >> 20;
    Map_CopyCellAttributes(22, 52, 1, 1, p5, (*(s32 *)(record + 16) >> 20));
}
