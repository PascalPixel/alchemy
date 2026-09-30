/* Draft, not exact (2026-09-30, Mercury): 708 of 708 bytes, 7 differing
   halfwords (+0x16, +0x18, +0x2e, +0x30, +0x3a..+0x3e). Standalone copy of
   the shared ja draft with the build's callee names and the sequence
   callback as BattleEvent_Playback. Two FAKEMATCH r1 copies of saved_input
   bring back both ROM rereads from sp+12 (a volatile r0 clobber stops
   postreload reusing the incoming r0). Left: the first reread lands after
   str r3, [sp, #4] instead of before it (the volatile asm is a scheduling
   barrier; moving the facing load after it changes nothing), the second
   reread is scheduled after lsrs instead of between lsls and lsrs, and the
   +0x6000 branch builds its constant in r1 where the ROM uses r3. */
#include "TYPES.H"

struct Input_080b12c0 {
    u8 primary_id;
    u8 padding_01;
    u8 secondary_id;
    u8 padding_03[0x1b];
    s8 target_adjustment;
    u8 padding_1f[0x0d];
    s8 target_modifier;
    u8 padding_2d[0x2b];
    u32 presentation_flags;
};

struct Work_080b12c0 {
    s32 flags;
    s32 secondary_is_low_id;
    s32 primary_id;
    s32 secondary_id;
    s32 one;
    s32 count;
    s32 mode;
    s32 unknown_1c;
    u8 padding_20[4];
    s16 members[24];
};

struct Motion_080b12c0 {
    u8 padding_00[8];
    s32 x;
    u8 padding_0c[4];
    s32 z;
};

struct Slot_080b12c0 {
    struct Motion_080b12c0 *object;
};

struct Child_080b12c0 {
    s16 value;
};

struct Record_080b12c0 {
    u8 padding_00[40];
    struct Child_080b12c0 *child;
};

struct Unit_080b12c0 {
    u8 padding_00[0x128];
    u8 class_id;
};

extern s32 *Data_03001f00;
extern void *Data_03001e74;

struct Slot_080b12c0 *GetBattleObjectSlot(s32 id);
s32 ArcTan2(s32 first, s32 second);
void WaitFrames(s32 frames);
void BattlePres_SetActorModes(u16 *actors, s32 mode);
void BattlePres_BuildTargetList(void *input, struct Work_080b12c0 *work);
struct Unit_080b12c0 *Func_08077008(s32 id);
struct Record_080b12c0 *GetMotionRecord(
    struct Motion_080b12c0 *object, s32 index);
s32 Func_08009260(s32 value, s32 second, s32 third);
u32 Battle_GetEntryField2HighBits(u32 value);
void BattleMotion_ApproachTarget(s32 first, s32 second, s32 divisor, s32 initial_y);
void ObjectDispatch_ApplyValueToChildrenFar(struct Motion_080b12c0 *object, s32 action);
void UiWindow_DrawPartyStatusContentsFar(s32 value);
void BattleMotion_ResetObjectAtScaledAnchor(s32 id);
void Actor_ResetMotionAtAnchor(s32 id);
u32 BattleEv_Push(u32 opcode, u32 operand);
void BattleEv_DispatchQueued(void);
s32 Func_080b7b6c(void *position, s32 mode);
s32 Math_Div(s32 angle, s32 count);
void BattlePres_SetupTransitionAtPairMidpoint(s32 first, s32 second, s32 mode);
void Scheduler_AddOrUpdateCallback(void *callback, s32 interval);
void BattleFx_DispatchByIdRangeFar(struct Work_080b12c0 *work);
void BattleFx_DispatchModeFar(struct Work_080b12c0 *work);
void BattleEventRuntime_WaitForReady(void);
void BattleParty_ListAllUnitsAndSubmit(void);
void BattlePres_SetupTransitionScene(s32 first, s32 second, s32 third, s32 fourth);

void BattleEvent_Playback(void);

s32 RunBattlePresentation(struct Input_080b12c0 *input)
{
    struct Work_080b12c0 work;
    struct Input_080b12c0 *saved_input;
    struct Motion_080b12c0 *object;
    struct Record_080b12c0 *record;
    struct Unit_080b12c0 *unit;
    s16 position[3];
    s32 scripted;
    s32 *facing;
    s32 angle;
    s32 adjusted;
    s32 facing_angle;
    s32 first_coordinate;
    s32 second_coordinate;
    s32 divisor;
    s32 direct;
    s32 index;
    s32 phase;

    facing = Data_03001f00;
    saved_input = input;
    {
        /* FAKEMATCH: rereads the saved input into r1 as the ROM does */
        register struct Input_080b12c0 *reload asm("r1");

        /* FAKEMATCH: frees r0 so the saved input is reread from the stack */
        asm volatile("" : : : "r0");
        /* FAKEMATCH: an opaque copy makes the ROM reread the saved input */
        asm("" : "=r"(reload) : "0"(saved_input));

        object = GetBattleObjectSlot(reload->primary_id)->object;
    }
    second_coordinate = object->z;
    first_coordinate = object->x;
    angle = (u16)ArcTan2(first_coordinate, second_coordinate);
    {
        /* FAKEMATCH: rereads the saved input into r1 as the ROM does */
        register struct Input_080b12c0 *reload asm("r1");

        /* FAKEMATCH: an opaque copy makes the ROM reread the saved input */
        asm("" : "=r"(reload) : "0"(saved_input));
        adjusted = angle - 0x2000;
        if (reload->primary_id > 7)
            adjusted = angle + 0x6000;
    }
    adjusted &= 0x7fff;
    facing_angle = (adjusted - 0x2000) / 2 + 0x2000;

    if (*facing == facing_angle) {
        *facing = facing_angle;
        WaitFrames(5);
    } else {
        *facing = facing_angle;
        WaitFrames(10);
    }

    BattlePres_SetActorModes(0, 0);
    BattlePres_BuildTargetList(saved_input, &work);
    if (work.flags == 0x87)
        UiWindow_DrawPartyStatusContentsFar(*((u8 *)Data_03001e74 + 0x41) & ~1);

    unit = Func_08077008(work.primary_id);
    Func_08077008(work.members[0]);
    scripted = saved_input->target_modifier;
    direct = saved_input->target_adjustment == 0;

    record = GetMotionRecord(
        GetBattleObjectSlot(saved_input->primary_id)->object, 0);
    divisor = Func_08009260(record->child->value, 2, 1);
    BattleMotion_ApproachTarget(
        work.primary_id,
        work.members[0],
        divisor,
        Battle_GetEntryField2HighBits(unit->class_id) << 16);
    ObjectDispatch_ApplyValueToChildrenFar(GetBattleObjectSlot(work.primary_id)->object, 16);
    GetBattleObjectSlot(work.members[0]);

    if ((u16)work.members[0] <= 7)
        work.secondary_is_low_id = 1;
    else
        work.secondary_is_low_id = 0;

    *(volatile u16 *)0x04000040 = 0x00f0;
    *(volatile u16 *)0x04000044 = 0x1088;
    *(volatile u16 *)0x04000042 = 0x00f0;
    *(volatile u16 *)0x04000046 = 0x1088;
    *(volatile u16 *)0x04000048 = 0x3537;
    *(volatile u16 *)0x0400004a = 0x3f21;
    *(volatile u16 *)0x04000000 |= 0x6000;

    if (direct != 0) {
        WaitFrames(10);
        BattleMotion_ResetObjectAtScaledAnchor(work.members[0]);
        WaitFrames(2);
        WaitFrames(4);
        WaitFrames(10);
        BattleEv_Push(0, saved_input->secondary_id);
        BattleEv_Push(4, 0x853);
        BattleEv_DispatchQueued();
        Actor_ResetMotionAtAnchor(work.members[0]);
    } else {
        phase = 0;
        work.unknown_1c = 0;
        if (saved_input->presentation_flags != 0)
            work.unknown_1c = 1;

        if (scripted != 0) {
            work.flags += 200;
            phase = 1;
            facing[5] = 1;
            position[0] = work.primary_id;
            position[1] = work.secondary_id;
            position[2] = 0xff;
            Func_080b7b6c(position, 0);
        }

        divisor -= 8;
        if (divisor <= 0)
            divisor = 1;
        {
            s32 loop_first;
            s32 loop_second;

            for (index = 0; index != divisor; index++) {
                if (phase != 0) {
                    loop_first = work.primary_id;
                    loop_second = work.secondary_id;
                    BattlePres_SetupTransitionAtPairMidpoint(
                        loop_first,
                        loop_second,
                        Math_Div(index * 30, divisor) + 100);
                }
                WaitFrames(1);
            }
        }

        Scheduler_AddOrUpdateCallback((void *)BattleEvent_Playback, 0xc80);
        if (work.flags != 0) {
            if (saved_input->presentation_flags & 0x4000)
                BattleFx_DispatchByIdRangeFar(&work);
            else
                BattleFx_DispatchModeFar(&work);
        }
        BattleEventRuntime_WaitForReady();
        if (scripted != 0) {
            facing[5] = 0;
            BattleParty_ListAllUnitsAndSubmit();
            BattlePres_SetupTransitionScene(0, 0, 0, 100);
        }
        Actor_ResetMotionAtAnchor(work.members[0]);
    }
    Actor_ResetMotionAtAnchor(work.primary_id);
    return 0;
}
