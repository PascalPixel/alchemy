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
extern void *Data_03000118;

s32 Func_0800ebec(struct ScriptObjectEntry *actor)
{
    s32 blocked_flags;
    s32 handled_flags;
    s32 mode;
    s32 angle_q16;
    s32 dir;
    s32 final_dir;
    s32 posA[3];
    s32 posB[3];
    s32 posC[3];
    s16 deltas[6];
    s32 extra[7];
    s32 i;
    struct ScriptObjectEntry *entry;
    u8 *base;
    u16 *p100;

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
                u8 v;

                v = *p;
                p++;
                if (v == 0xff)
                    count++;
                n--;
            } while (n != 0);
            if (count - 136 < 0)
                Func_080f9010(135);
        }
    }

    if (Data_03001f54 != 0) {
        s32 mask;

        mask = 0x200;
        if (Data_03001ae8 & mask) {
            s32 count;

            count = mask;
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

    if (Data_03001ae8 & Data_02000240[270]) {
        *(s32 *)((u8 *)actor + 48) = 0x18000;
        *(s32 *)((u8 *)actor + 52) = 0x4000;
        mode = 5;
    } else {
        *(s32 *)((u8 *)actor + 48) = 0x10000;
        *(s32 *)((u8 *)actor + 52) = 0x4000;
        mode = 2;
    }

    if (Func_080770c0(0x17f /* 383 */) != 0) {
        if (Data_03001ae8 & 2) {
            *(s32 *)((u8 *)actor + 48) = 0x40000;
            *(s32 *)((u8 *)actor + 52) = 0x10000;
            mode = 5;
        }
    }

    dir = Data_08013254[(Data_03001ae8 >> 4) & 15];
    angle_q16 = dir << 16;
    if (((u32)angle_q16 >> 16) == 0xffffu) {
        blocked_flags |= 4;
        goto tail_merge;
    }

    dir = (u16)angle_q16;

    posA[0] = actor->values_08[0];
    posA[1] = actor->values_08[1];
    posA[2] = actor->values_08[2];
    Func_0800447c(0x80000, dir, (struct WorldPosition *)posA);

    if (Data_03001f54 != 0) {
        if (Data_03001ae8 & 0x200) {
            goto tail_merge;
        }
    }

    if (Func_080120dc(actor, (struct WorldPosition *)posA) != 0)
        goto region_search;

    posB[0] = actor->values_08[0];
    posB[1] = actor->values_08[1];
    posB[2] = actor->values_08[2];
    Func_0800447c(0x80000, dir + 0x1000, (struct WorldPosition *)posB);
    if (Func_080120dc(actor, (struct WorldPosition *)posB) != 0)
        goto region_search;

    posB[0] = actor->values_08[0];
    posB[1] = actor->values_08[1];
    posB[2] = actor->values_08[2];
    Func_0800447c(0x80000, dir - 0x1000, (struct WorldPosition *)posB);
    if (Func_080120dc(actor, (struct WorldPosition *)posB) != 0)
        goto region_search;

    posB[0] = actor->values_08[0];
    posB[1] = actor->values_08[1];
    posB[2] = actor->values_08[2];
    Func_0800447c(0x80000, dir + 0x2000, (struct WorldPosition *)posB);
    if (Func_080120dc(actor, (struct WorldPosition *)posB) != 0)
        goto region_search;

    posB[0] = actor->values_08[0];
    posB[1] = actor->values_08[1];
    posB[2] = actor->values_08[2];
    Func_0800447c(0x80000, dir - 0x2000, (struct WorldPosition *)posB);
    if (Func_080120dc(actor, (struct WorldPosition *)posB) != 0)
        goto region_search;

    final_dir = dir;
    goto success_tail;

region_search:
    dir = (u16)angle_q16;
    deltas[0] = dir + 0x1000;
    deltas[1] = dir - 0x1000;
    deltas[2] = dir + 0x2000;
    deltas[3] = dir - 0x2000;
    deltas[4] = dir + 0x3000;
    deltas[5] = dir - 0x3000;

    for (i = 0; i < 6; i++) {
        s32 cand;
        s32 cand_u16;

        cand = deltas[i];
        final_dir = cand;
        cand_u16 = (u16)cand;

        posA[0] = actor->values_08[0];
        posA[1] = actor->values_08[1];
        posA[2] = actor->values_08[2];
        Func_0800447c(0x80000, cand_u16, (struct WorldPosition *)posA);
        if (Func_080120dc(actor, (struct WorldPosition *)posA) != 0)
            continue;

        posB[0] = actor->values_08[0];
        posB[1] = actor->values_08[1];
        posB[2] = actor->values_08[2];
        Func_0800447c(0x80000, cand_u16 + 0x1000, (struct WorldPosition *)posB);
        if (Func_080120dc(actor, (struct WorldPosition *)posB) != 0)
            continue;

        posB[0] = actor->values_08[0];
        posB[1] = actor->values_08[1];
        posB[2] = actor->values_08[2];
        Func_0800447c(0x80000, cand_u16 - 0x1000, (struct WorldPosition *)posB);
        if (Func_080120dc(actor, (struct WorldPosition *)posB) != 0)
            continue;

        posB[0] = actor->values_08[0];
        posB[1] = actor->values_08[1];
        posB[2] = actor->values_08[2];
        Func_0800447c(0x80000, cand_u16 + 0x2000, (struct WorldPosition *)posB);
        if (Func_080120dc(actor, (struct WorldPosition *)posB) != 0)
            continue;

        posB[0] = actor->values_08[0];
        posB[1] = actor->values_08[1];
        posB[2] = actor->values_08[2];
        Func_0800447c(0x80000, cand_u16 - 0x2000, (struct WorldPosition *)posB);
        if (Func_080120dc(actor, (struct WorldPosition *)posB) == 0)
            goto success_tail;
    }

    posA[0] = actor->values_08[0];
    posA[1] = actor->values_08[1];
    posA[2] = actor->values_08[2];
    blocked_flags |= 1;

success_tail:
    posC[0] = actor->values_08[0];
    posC[1] = actor->values_08[1];
    posC[2] = actor->values_08[2];
    Func_0800447c(0x40000, (u16)final_dir, (struct WorldPosition *)posC);

    entry = (struct ScriptObjectEntry *)(*(u8 **)ADDR_03001E64);
    for (i = 63; i >= 0; i--) {
        s32 dist;

        if (entry->data == NULL)
            goto scan_next;
        if (!(entry->flags_59 & 1))
            goto scan_next;
        if (entry == actor)
            goto scan_next;

        dist = Func_0800eba0(entry->values_08, actor->value_20 - 2,
                posC, entry->value_20 - 2);
        if (dist < 0)
            goto scan_next;

        base = (u8 *)entry;
        if ((*(u32 *)(base + 88) & 0xff000200) != 0x200) {
            blocked_flags |= 2;
            goto scan_next;
        }

        {
            s32 dy;
            s32 dx;
            s32 angle_raw;
            s16 angle_signed;
            s32 angle_u16;

            dy = entry->values_08[2] - actor->values_08[2];
            dx = entry->values_08[0] - actor->values_08[0];
            angle_raw = Func_080044d0(dy, dx);
            angle_signed = (s16)angle_raw;
            (void)angle_signed;
            angle_u16 = (u16)angle_raw;

            posB[0] = entry->values_08[0];
            posB[1] = entry->values_08[1];
            posB[2] = entry->values_08[2];
            Func_0800447c(0x4000, angle_u16, (struct WorldPosition *)posB);
            if (Func_0800d924(entry, posB) != 0) {
                blocked_flags |= 2;
                goto scan_next;
            }

            posB[0] = entry->values_08[0];
            posB[1] = entry->values_08[1];
            posB[2] = entry->values_08[2];
            Func_0800447c(0xa0000, angle_u16, (struct WorldPosition *)posB);
            if (Func_080120dc(entry, (struct WorldPosition *)posB) != 0) {
                blocked_flags |= 2;
                goto scan_next;
            }

            posB[0] = entry->values_08[0];
            posB[1] = entry->values_08[1];
            posB[2] = entry->values_08[2];
            Func_0800447c(0xa0000, angle_u16 + 0x1000, (struct WorldPosition *)posB);
            if (Func_080120dc(entry, (struct WorldPosition *)posB) != 0) {
                blocked_flags |= 2;
                goto scan_next;
            }

            if (Func_080120dc(entry, (struct WorldPosition *)posB) != 0) {
                blocked_flags |= 2;
                goto scan_next;
            }

            posB[0] = entry->values_08[0];
            posB[1] = entry->values_08[1];
            posB[2] = entry->values_08[2];
            Func_0800447c(0xa0000, angle_u16 - 0x1000, (struct WorldPosition *)posB);
            if (Func_080120dc(entry, (struct WorldPosition *)posB) != 0) {
                blocked_flags |= 2;
                goto scan_next;
            }

            Func_0800447c(0x4000, angle_u16, (struct WorldPosition *)entry->values_08);
            *(s32 *)(base + 56) = 0x80000000;
            *(s32 *)(base + 60) = 0x80000000;
            *(s32 *)(base + 64) = 0x80000000;
            handled_flags |= 1;
        }

scan_next:
        entry++;
    }

    if (blocked_flags == 0 && handled_flags != 0) {
        *(s32 *)((u8 *)actor + 48) = 0x4000;
        *(s32 *)((u8 *)actor + 52) = 0x2000;
    }

tail_merge:
    {
        u16 *table_ptr;

        table_ptr = *(u16 **)Data_03001ebc_a;
        if (table_ptr != 0) {
            if (blocked_flags & 3) {
                table_ptr[206]++;
            } else {
                table_ptr[206] = 0;
            }
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
        Func_0800c300(actor, mode);
    }

    if (blocked_flags != 0) {
        *(s32 *)((u8 *)actor + 56) = 0x80000000;
        *(s32 *)((u8 *)actor + 60) = 0x80000000;
        *(s32 *)((u8 *)actor + 64) = 0x80000000;
        *(s32 *)((u8 *)actor + 36) = 0;
        *(s32 *)((u8 *)actor + 44) = 0;

        if (blocked_flags & 3) {
            s32 diff;
            u16 *field6;

            field6 = (u16 *)((u8 *)actor + 6);
            diff = (s16)((u16)(angle_q16 >> 16) - *field6);
            if (diff > 0x1000)
                diff = 0x1000;
            if (diff < -0x1000)
                diff = -0x1000;
            *field6 = *field6 + diff;
        }

        p100 = (u16 *)((u8 *)actor + 100);
        *p100 = 0;
        *(u16 *)((u8 *)actor + 102) = 2;

        goto fragment_tail;
    } else {
        Func_0800d14c(actor, posA[0], posA[1], posA[2]);
        goto fragment_tail;
    }

fragment_tail:
    {
        void *global_table;
        s32 spawned_flag;
        s32 facing_final;
        struct ScriptObjectEntry *spawned;
        void *r5;

        global_table = Data_03000118;
        spawned_flag = ((u8 *)global_table)[23];
        facing_final = (u16)(angle_q16 >> 16);

        if (spawned_flag != 0 && *(s16 *)p100 == 0 && blocked_flags == 0) {
            spawned = Func_0800c150(actor->values_08[0], actor->values_08[1],
                    actor->values_08[2], 25);
            if (spawned != NULL) {
                spawned->values_08[3] = actor->values_08[3];
                r5 = *(void **)((u8 *)spawned + 80);
                Func_0800c2d8(spawned, Data_08013274_a);
                *(u8 *)((u8 *)spawned + 35) = 2;
                *(u8 *)((u8 *)spawned + 85) = (u8)blocked_flags;

                if (r5 != NULL) {
                    Func_0800ba30(r5, 1);
                    *(u8 *)((u8 *)r5 + 38) = (u8)blocked_flags;
                    *(u16 *)((u8 *)r5 + 30) = 0x4000 + facing_final;
                    *(u8 *)((u8 *)r5 + 9) |= 0xc;
                }

                {
                    u16 v;

                    v = *(u16 *)((u8 *)actor + 102);
                    if (v == 2) {
                        Func_0800ba30(r5, 2);
                        *(u16 *)((u8 *)actor + 102) = (u16)blocked_flags;
                        v = 0;
                    }

                    if (v != 0) {
                        *(u16 *)((u8 *)spawned + 6) = 0x8000;
                    }
                }
            }
        }

        if (mode == 5) {
            *p100 = 12;
        } else {
            *p100 = 18;
        }
        *(u16 *)((u8 *)actor + 102) ^= 1;
    }

    Func_0800eaf8();
    *(u16 *)((u8 *)actor + 4) += 1;
    return 1;
}
