#include "types.h"
#include "map.h"
#include "global_cells.h"
#include "script_object_entry.h"

/* FunctionHead_0800ebec / Region_0800ee14 / Fragment_0800f1fa are one large
   C function split across three retained-assembly labels by literal-pool and
   branch-range boundaries: control reaches sub_0800ee14 by plain 'bne' with
   the FunctionHead prologue's frame still live, and Region_0800ee14 tail
   branches into Fragment_0800f1fa through a 'mov r12,pc / bx r4' veneer.
   One stack frame, one register set, one literal pool. This source covers
   the whole span. */

extern volatile u8 Data_03001f54;
extern u8 Data_03001810_a[];
extern volatile u32 Data_03001ae8;
extern u16 Data_02000240[];
extern s16 Data_08013254[];

s32 Func_080770c0(s32 flag);
void Func_080f9010(s32 cue);
void Func_0800447c(s32 magnitude, s32 angle, struct WorldPosition *out);
s32 Func_080120dc(struct ScriptObjectEntry *object, struct WorldPosition *position);
s32 Func_0800eba0(s32 *a, s32 arg1, s32 *b, s32 arg3);
s32 Func_0800d924(struct ScriptObjectEntry *object, s32 *values);
s32 Func_080044d0(s32, s32);
void Func_0800c300(void *object, s32 argument);
void Func_0800c2d8(void *object, void *table);
struct ScriptObjectEntry *Func_0800c150(s32 x, s32 y, s32 z, s32 kind);
void Func_0800ba30(void *object, s32 argument);
void Func_0800d14c(void *object, s32 arg1, s32 arg2, s32 arg3);
void *Func_08077008(s32 id);
void Func_0800eaf8(void);

extern s16 Data_08013274_a[];

s32 Func_0800ebec(struct ScriptObjectEntry *actor)
{
    s32 blocked_flags;
    s32 handled_flags;
    s32 mode;
    s32 angle_q16;
    s32 facing;
    s32 sp8_mode;
    s16 deltas[6];
    struct WorldPosition posA;
    struct WorldPosition posB;
    struct WorldPosition posC;
    s32 i;
    struct ScriptObjectEntry *entry;
    s32 speed;
    s32 accel;

    blocked_flags = 0;
    handled_flags = 0;

    if (Data_03001f54 != 0) {
        if (Func_080770c0(350) != 0) {
            s32 count;
            u8 *p;
            s32 n;

            count = 0;
            p = Data_03001810_a;
            n = 512;
            do {
                if (*p == 0xff)
                    count++;
                p++;
                n--;
            } while (n != 0);
            if (count - 136 < 0)
                Func_080f9010(135);
        }
    }

    if (Data_03001f54 != 0) {
        if (Data_03001ae8 & 0x200) {
            s32 count;

            count = 0x200;
            do {
                count--;
            } while (count != 0);
            count = 95;
            do {
                count--;
            } while (count >= 0);
            count = 63;
            do {
                count--;
            } while (count >= 0);
            count = 63;
            do {
                count--;
            } while (count >= 0);
        }
    }

    if (Data_03001ae8 & (Data_02000240[135])) {
        speed = 0x180000;
        accel = 0x80000;
        sp8_mode = 5;
    } else {
        speed = 0x100000;
        accel = 0x80000;
        sp8_mode = 2;
    }
    actor->values_08[4] = speed;
    actor->values_08[5] = accel;

    if (Func_080770c0(0x2c0 /* 704 */) != 0) {
        if (Data_03001ae8 & 2) {
            actor->values_08[4] = 0x400000;
            actor->values_08[5] = 0x200000;
            sp8_mode = 5;
        }
    }

    facing = Data_08013254[(Data_03001ae8 >> 4) & 15];
    angle_q16 = facing << 16;
    if ((angle_q16 >> 16) == (s16)0xffff) {
        blocked_flags |= 4;
        goto after_direction_checks;
    }

    {
        s32 dir;

        dir = (u16)angle_q16;

        for (i = 0; i < 4; i++) {
            s32 sample_angle;

            switch (i) {
            case 0: sample_angle = dir; break;
            case 1: sample_angle = dir + 0x2000; break;
            case 2: sample_angle = dir + 0x3000; break;
            default: sample_angle = dir - 0x1000; break;
            }

            posB.x = actor->values_08[0];
            posB.y = actor->values_08[1];
            /* placeholder for third field, overwritten below */
            Func_0800447c(0x80000, sample_angle, &posB);
            if (Func_080120dc(actor, &posB) != 0) {
                blocked_flags |= 1;
                break;
            }
        }
    }

after_direction_checks:
    /* TODO(unfinished): Region_0800ee14's real control flow here is a nested
       search, not the single 4-direction scan above: for each of 6 broad
       angle candidates (facing +-0x1000, +-0x2000, +-0x3000, built into a
       stack s16[6] "deltas" array), it re-checks a 5-point fan around that
       candidate angle (deltas[i], deltas[i]+0x1000, -0x1000, +0x2000, and a
       fifth pc-relative constant) with Func_080120dc before accepting it;
       only a fully-clear 5-point fan breaks out to the success label
       (.L_0800ef44), otherwise it advances to the next of the 6 candidates.
       All 6 failing sets blocked_flags|=1 and copies actor->values_08[0..2]
       into a third WorldPosition scratch (posC, corresponds to reference
       sp+68) before falling into the shared .L_0800ef44 continuation, which
       this draft does not yet reconstruct. See the dossier
       (games/gs1/recon/en/main/0800ebec.json) for the traced instruction
       sequence and the remaining Fragment_0800f1fa tail (Func_0800c150
       spawn / Func_0800c2d8 / Func_0800ba30 / Func_0800d14c dispatch)
       neither of which is represented below yet. */
    posC.x = actor->values_08[0];
    posC.y = actor->values_08[1];

    entry = (struct ScriptObjectEntry *)(*(u8 **)ADDR_03001E64);
    for (i = 63; i >= 0; i--) {
        if (entry->data == NULL)
            continue;
        if (!(entry->flags_59 & 1))
            continue;
        if (entry == actor)
            continue;
        if (Func_0800eba0(entry->values_08, entry->value_20 - 2,
                actor->values_08, actor->value_20 - 2) < 0)
            continue;

        if ((entry->unknown_22[80 - 34] & 0x200) == 0x200) {
            blocked_flags |= 2;
            continue;
        }

        handled_flags |= 1;
        break;
    }

    if (blocked_flags == 0 && handled_flags != 0) {
        actor->values_08[4] = 0x100000;
        actor->values_08[5] = 0x80000;
    }

    if (Data_03001e70_a[0] != 0) {
        if (blocked_flags & 3) {
            Data_02000240[270]++;
        } else {
            Data_02000240[270] = 0;
        }
    }

    if (handled_flags != 0) {
        Func_0800c300(actor, 8);
    } else if (blocked_flags != 0) {
        void *record;
        s32 kind;

        record = Func_08077008(Data_02000240[250]);
        kind = *(s16 *)((u8 *)record + 56);
        Func_0800c300(actor, kind != 0 ? 9 : 22);
    } else {
        Func_0800c300(actor, sp8_mode);
    }

    return 0;
}
