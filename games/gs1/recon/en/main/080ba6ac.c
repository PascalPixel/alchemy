#include "types.h"

#define FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

struct PresentationInput { u8 primary; u8 reserved_01; u8 secondary; u8 reserved_03[0x55]; u32 flags; };
struct ActionSelection { s16 actor; u8 reserved_02[6]; s16 index; };
struct PresentationWork {
    s32 field_00;
    s32 field_04;
    s32 field_08;
    u8 reserved_0c[8];
    s32 count;
    u8 reserved_18[12];
    s16 table[8];
    u8 values[32];
};
struct MotionEntry { u8 reserved_00[39]; u8 count; void *children[1]; };
struct MotionChild { u8 reserved_00[5]; u8 value; };
struct AbilityInfo { u8 reserved_00[12]; u8 kind; };

extern s32 *Data_03001f00;
extern u8 *Data_03001e74;
s32 WaitFrames(s32);
void Func_080041d8(void *, s32);
void Func_08009080(s32, s32);
void Func_08009088(s32, s32);
u8 *Func_08077008(s16);
struct AbilityInfo *Func_08077018(u16);
s32 Func_08077058(s16, s16);
void Func_08077060(s16, s16);
s32 Func_080771a0(void);
s32 *Func_080b7dd0(s32);
struct MotionEntry *Func_080b7f70(s32, s32);
void Func_080b8000(s16);
void Func_080b9d34(void *, struct PresentationWork *);
u32 Func_080bb938(void);
u32 Func_080bbabc(u32, u32);
void Func_080be02c(void);
void Func_080c10e8(u16 *, s32);
void Func_080c1724(void *, void *, s32, s32);
void Func_080c9008(struct PresentationWork *);
void Func_080c9018(struct PresentationWork *);

s32 Func_080ba6ac(struct PresentationInput *input, s32 unused,
                  struct ActionSelection *selection)
{
    register struct PresentationInput *saved_input = input;
    register struct ActionSelection *saved_selection = selection;
    s32 saved_offset;
    struct PresentationWork work;
    s32 object;
    s32 i;
    u16 ability;
    u8 *ability_table;
    s32 kind;

    s32 facing = -0x2000;
    if (saved_input->primary <= 4)
        facing = 0x2000;
    if (*Data_03001f00 != facing)
        *Data_03001f00 = facing;
    Func_080b9d34(saved_input, &work);
    Func_080c10e8(0, 0);
    object = *Func_080b7dd0(work.field_08);
    Func_08009080(object, 3);
    Func_08009088(object, 16);
    work.field_04 = saved_input->secondary <= 7;

    {
        s32 i1;
        for (i1 = 0; i1 < work.count; i1++) {
            struct MotionEntry *entry = Func_080b7f70(
                *Func_080b7dd0(work.table[i1]), 0);
            s32 j;
            for (j = 0; j != entry->count - 1; j++)
                work.values[i1 * 4 + j] =
                    ((struct MotionChild *)entry->children[j])->value;
        }
    }

    Func_080041d8((void *)0x050000c0, 0xc80);
    if (work.field_00 != 0) {
        s32 fade = 0;
        for (i = 0; i <= 19; i++, fade += 0x444) {
            u8 *battle = Data_03001e74;
            if (i <= 19) {
                s32 value = 0x10000 - fade;
                FIELD(battle, s32, 0x644) = value;
                Func_080c1724(battle + 0x544, (void *)0x050000c0, value, 0x80);
            }
            WaitFrames(1);
        }
        if (saved_input->flags & 0x4000)
            Func_080c9008(&work);
        else
            Func_080c9018(&work);
    } else {
        WaitFrames(60);
    }
    Func_080be02c();
    Func_08009080(object, 1);
    for (i = 0; i < work.count; i++) {
        saved_offset = 36 + i * 2;
        Func_080b8000(FIELD(&work, s16, saved_offset));
    }

    ability_table = Func_08077008(saved_selection->actor);
    ability = *(u16 *)(ability_table + 0xd8 + saved_selection->index * 2);
    kind = Func_08077018(ability)->kind;
    if (kind == 1) {
        if (Func_08077058(saved_selection->actor, saved_selection->index) == 2) {
            u8 *battle = Data_03001e74;
            for (i = 0; i <= 19; i++) {
                u8 *slot = battle + 0x2ec + i * 16;
                if (FIELD(slot, s16, 6) == 2 &&
                    FIELD(slot, s16, 0) == saved_selection->actor) {
                    s16 current = FIELD(slot, s16, 8);
                    if (current == saved_selection->index)
                        FIELD(slot, s16, 8) = 0xffff;
                    else if (current > saved_selection->index)
                        FIELD(slot, s16, 8) = current - 1;
                }
            }
        }
    } else if ((u8)kind == 2) {
        if ((Func_080771a0() & 7) == 0) {
            Func_080bbabc(2, ability);
            Func_080bbabc(4, 0x81c);
            Func_08077060(saved_selection->actor, saved_selection->index);
            Func_080bb938();
        }
    } else if ((u8)kind == 4) {
        if ((ability & 0x1ff) == 0xb8)
            ability = 0xb9;
        *(u16 *)(ability_table + 0xd8 + saved_selection->index * 2) = ability;
    }
    return 0;
}
