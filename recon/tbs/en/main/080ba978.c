/* Draft, whole main:080ba978, 612 bytes including pool.
 * Baseline: 612/612 bytes, 261 differing halfwords, 144 aligned edits.
 * H1: transfer exact RUN_SIMPLE's typed work and unsigned angle input;
 * narrow after the offset, correct the 0x80000 angle, use signed member IDs,
 * and snapshot each motion record's child count before copying its values.
 * The caller passes mode 0/1/2; LIST_TARGETS owns the complete 84-byte output.
 * H1 result: 592/612 bytes, 275 differing halfwords, 114 aligned edits;
 * frame 88 and high-register save set now match. The signed target loop and
 * child-copy body are structurally correct. First divergence is the target
 * angle branch; the same-team ternary also omits reference materialized
 * boolean branches. Input/transition and flags/object roles remain swapped.
 * No matching-C credit claimed.
 */
#include "TYPES.H"
#include "MOTION_OBJECT.H"
#include "BATTLE_PRESENTATION.H"
#include "FIXED_MATH.H"

struct PresentationInput { u8 primary; u8 reserved_01; u8 secondary; u8 reserved_03[0x4d]; s32 coordinate; u8 reserved_54[4]; u32 flags; s32 script; };
struct PresentationWork {
    s32 flags;
    s32 secondary_is_low_id;
    s32 primary_id;
    s32 secondary_id;
    s32 initial_value;
    s32 entry_count;
    s32 battle_mode;
    s32 scripted;
    s32 reserved_20;
    s16 members[8];
    u8 values[8][4];
};
struct MotionEntry { u8 reserved_00[39]; u8 count; void *children[1]; };
struct MotionChild { u8 reserved_00[5]; u8 value; };
extern struct BattlePresentationTransition *Data_03001f00;
extern u8 *Data_03001e74;
s32 Func_080041d8(void *, s32);
void Func_08009080(void *, s32);
void Func_08009088(void *, s32);
void Func_08015130(s32);
void Func_080b8000(s32);
s32 Func_080b9d34(void *, struct PresentationWork *);
u32 Func_080bb938(void);
u32 Func_080bbabc(u32, u32);
void Func_080be02c(void);
void Func_080c10e8(u16 *, s32);
void Func_080c1798(s32, s32, s32, s32);
void Func_080c1a14(void);
void Func_080c9008(struct PresentationWork *);
void Func_080c9018(struct PresentationWork *);
void Func_080f9010(s32);

s32 Func_080ba978(struct PresentationInput *input, s32 flags)
{
    struct PresentationWork work;
    struct BattlePresentationTransition *transition = Data_03001f00;
    struct MotionObject *object;
    s32 i;

    if (input->flags & 0x40000) {
        transition->target_yaw = input->primary <= 7 ? -0x2000 : 0x5000;
        transition->frames = 60;
    } else {
        struct MotionObject *actor = GetBattleObjectSlot(input->primary)->object;
        s32 angle = (u16)ArcTan2(actor->x, actor->z);
        s32 current = angle - 0x1800;
        s32 target;
        if (input->primary > 7)
            current = angle + 0x1800;
        current = (s16)current;
        if (input->primary <= 7)
            target = 0x2000;
        else
            target = -0x2000;
        current += (target - current) * 3 / 4;
        if (input->secondary <= 7 ? input->primary <= 7 : input->primary > 7)
            current = input->primary <= 7 ? 0x2400 : -0x2400;
        if (transition->target_yaw != current)
            transition->target_yaw = current;
    }
    if (input->flags & 0x80000) {
        transition->target_yaw = input->primary <= 7 ? -0x2000 : 0x2000;
        transition->frames = 60;
    }

    Func_080b9d34(input, &work);
    i = flags & 1;
    if (i)
        work.scripted = 1;
    Func_080c10e8(0, 0);
    Func_08015130(Data_03001e74[65] & ~1);
    object = GetBattleObjectSlot(work.primary_id)->object;
    Func_08009080(object, 3);
    Func_08009088(object, 16);
    Func_080f9010(0x9a);
    if (flags & 2)
        Func_080c1798(work.primary_id, input->coordinate, 1, 0);
    else if (!i)
        Func_080c1798(work.primary_id, input->coordinate, 0, 0);
    if (input->secondary <= 7)
        work.secondary_is_low_id = 1;
    else
        work.secondary_is_low_id = 0;

    for (i = 0; i != work.entry_count; i++) {
        struct MotionEntry *entry = GetMotionRecord(
            GetBattleObjectSlot(work.members[i])->object, 0);
        s32 count = entry->count - 1;
        s32 j;
        for (j = 0; j != count; j++)
            work.values[i][j] =
                ((struct MotionChild *)entry->children[j])->value;
    }
    if (input->script != 0) {
        if (input->script == 1) {
            Func_080bbabc(0, input->primary);
            Func_080bbabc(4, 0x856);
        } else {
            Func_080bbabc(4, 0x855);
        }
        Func_080bb938();
        Func_080c1a14();
    } else {
        Func_080041d8((void *)0x080bd899, 0xc80);
        if (work.flags) {
            if (input->flags & 0x4000)
                Func_080c9008(&work);
            else
                Func_080c9018(&work);
        } else {
            Func_080c1a14();
        }
        Func_080be02c();
        Func_08009080(object, 1);
        for (i = 0; i != work.entry_count; i++)
            Func_080b8000(work.members[i]);
    }
    return 0;
}
