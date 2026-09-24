/* Battle effect with two variants: arg1 picks resource 0x83 or 0x84 and the
   matching cell tables. It seeds one column per table entry in the 28-byte
   records at work + 0x7080 (x, y, height limit, start frame), then for each
   frame grows every started column upward, drawing it with the two
   alternating rectangle blitters. Four frames after a column starts it cues
   the effect targets and drops a spark into the 512-slot pool at 0x02010000;
   the sparks are drawn and aged every frame. A sibling of MEMBER_ORBIT.C.

   Shape notes, measured against the ROM:
   - One i serves every loop, which gives it a live range long enough to put
     work in fp.
   - The column cursor is recomputed from i at the top of the column loop
     (loop.c reduces it after the start frame i * 4 + 8), and the cue loop
     derives the target id offset from its index the same way, so both
     inductions are seeded after the hoisted invariants as in the ROM.
   - The spawn scan is a while loop whose free-slot case breaks out.
   - The spark blits halve a signed table byte with a plain / 2. */
#include "TYPES.H"
#include "BATTLE_EFX.H"

/* One column record at work + 0x7080. */
struct DualColumn {
    s32 x;
    s32 y;
    u8 unknown_08[8];
    s32 height;
    u8 unknown_14[4];
    s32 start;
};

/* One slot of the 512-entry spark pool at 0x02010000; age -1 is free. */
struct DualSpark {
    s32 x;
    s32 y;
    u8 unknown_08[16];
    s32 age;
};

struct DualTableEffect {
    u8 unknown_00[4];
    s32 mirror;
    u8 unknown_08[12];
    s32 target_count;
    s32 table;
    u8 unknown_1c[8];
    s16 targets[8];
};

struct DualTableWork {
    u8 cells[0x7080];
    struct DualColumn columns[16];
    u8 unknown_7240[0x540];
    s32 unknown_7780;
    s32 unknown_7784;
    u8 unknown_7788[0x20];
    s32 cue;
    u8 unknown_77ac[0x78];
    s32 transfer_pending;
    struct DualTableEffect *effect;
};

extern u8 Value_00000083;
extern u8 Value_00000084;

extern void *Data_03001e50[];

extern u8 Data_080eeb4b[];
extern u16 Data_080eeb4e[];
extern u8 Data_080eeb48[];
extern s8 Data_080eeb71[];
extern u16 Data_080eeb58[];
extern u8 Data_080eeb54[];
extern u8 Data_080eeb5e[];
extern u8 Data_080eeb61[];
extern s8 Data_080eeb79[];
extern u8 Data_080eeb80[];
extern u16 Data_080eeb88[];

void Func_080cd594(s32 mode);
s32 Func_08004458(void);
s32 Func_080022fc(s32 a, s32 b);
void Func_080b50e8(s32 id);
s32 Func_080041d8(void *callback, s32 interval);
void Func_080f9010(s32 id);
void Func_080d6888(s32 member_id, s32 b, s32 c, s32 d, s32 e);
void Func_080e155c(s32 a, s32 b);
void Func_080cd52c(void);
void Func_080030f8(s32 frames);
void Func_08004278(void *callback);
void Func_08002dd8(s32 id);
s32 Func_080cdbc0(void);

void BattleFx_RunDualTable(void *object, s32 arg1)
{
    void **heap_cache;
    void **cursor;
    struct DualTableWork *work;
    s32 mode;
    void *draw_destination;
    s32 status;
    void *rectangle[2];
    void *second_slot;

    s32 sp24;
    s32 sp20;
    void **slot_pair;
    s32 sp18;
    s32 sp14;
    s32 sp10;
    s32 sp0C;

    u8 *slot_cursor;

    struct DualColumn *seed;
    s32 temp_r2_134;
    s32 temp_r5_140;
    s32 temp_r3_145;

    u8 *cell_base;
    struct DualColumn *column;
    s32 var_r5_297;
    s32 temp_r3_298;
    u8 temp_r4_315;
    s8 temp_r3_335;
    u8 temp_r4_348;

    s32 n;
    s32 id_offset;
    s32 cue_frame;


    s32 i;
    struct DualSpark *spark;
    s32 temp_r2_474;
    s32 temp_r7_481;
    s8 temp_r5_490;
    s8 temp_r4_498;
    s8 temp_r4_518;
    s32 temp_r3_538;

    mode = arg1;

    heap_cache = (void **)0x03001EEC;
    cursor = heap_cache;
    work = *cursor++;
    draw_destination = *cursor;
    work->effect = object;
    Func_080cd594(1);
    *(s16 *)0x04000020 = 0x100;
    *(s16 *)0x04000050 = 0;
    if (mode == 1) {
        Resource_LoadAndDecompress((s32)&Value_00000083, work, 1, 1);
    } else {
        Resource_LoadAndDecompress((s32)&Value_00000084, work, 1, 1);
    }
    if (work->effect->mirror == 1) {
        *(s32 *)0x04000028 = 0xFFFF9000;
    }
    status = BattleEffect_LoadWork(46, 7, 7, 3, 1);
    rectangle[0] = Data_03001e50[46];
    status = BattleEffect_LoadWork(47, 7, 7, 7, 1);
    second_slot = Data_03001e50[47];
    slot_pair = rectangle;
    slot_pair[1] = second_slot;

    sp20 = Data_080eeb5e[work->effect->table] * 4 + 0x38;

    slot_cursor = (u8 *)0x02010018;
    i = 0;
    do {
        i += 1;
        *(s32 *)slot_cursor = -1;
        slot_cursor += 28;
    } while (i != 0x400);

    i = 0;
    seed = work->columns;
    do {
        temp_r2_134 = (Data_080eeb61[i] + (7 & Func_08004458())) - 4;
        seed->y = i / 2 + 0x6C;
        seed->x = temp_r2_134;
        temp_r5_140 = (63 & Func_08004458()) + 0x37;
        seed->height = temp_r5_140;
        temp_r3_145 = Data_080eeb4b[Func_080022fc(i, 3)];
        if (temp_r3_145 < temp_r5_140) {
            seed->height = temp_r3_145;
        }
        temp_r3_145 = i * 4 + 8;
        seed->start = temp_r3_145;
        i += 1;
        seed++;
    } while (i != 16);

    work->unknown_7780 = 1;
    work->unknown_7784 = 0;
    Func_080041d8((void *)0x080CD261, 0x480);

    sp24 = 0;
    if (sp20 != 0) {
        sp18 = sp20 - 64;
        sp14 = sp20 - 20;
        sp10 = sp20 - 4;
        do {
            if (sp24 == sp18) {
                Func_080b50e8(0x84);
            }
            if (sp24 >= sp14 && sp24 < sp10) {
                *(s16 *)0x04000050 = 0x3F44;
                *(s16 *)0x04000052 = ((sp20 - sp24) - 5) | 0x1000;
            }
            if (sp24 < sp10) {
                i = 0;
                if (Data_080eeb5e[work->effect->table] != 0) {
                    do {
                        column = (struct DualColumn *)((u8 *)work + 0x7080 + i * 0x1C);
                        sp0C = i * 4 + 8;
                        if (sp24 == ((i * 4) + 9)) {
                            work->cue = 2;
                        }
                        if (sp24 > sp0C) {
                            u32 texture_index = Func_080022fc(i, 3);

                            var_r5_297 = (sp24 - sp0C) * 8;
                            temp_r3_298 = column->height;
                            if (var_r5_297 > temp_r3_298) {
                                var_r5_297 = temp_r3_298;
                            }
                            if (mode == 0) {
                                {
                                    s32 sel = 1 & i;
                                    s32 cell = Data_080eeb4e[texture_index];
                                    texture_index = Data_080eeb48[texture_index];
                                    ((DrawRectangleFn)slot_pair[sel])(draw_destination, (u8 *)work + cell,
                                        column->x - (texture_index >> 1),
                                        column->y - var_r5_297, texture_index, var_r5_297);
                                }
                            } else {
                                temp_r3_335 = Data_080eeb71[7 & i];
                                if (var_r5_297 > temp_r3_335) {
                                    var_r5_297 = temp_r3_335;
                                }
                                {
                                    s32 sel = 1 & i;
                                    s32 cell = Data_080eeb58[texture_index];
                                    texture_index = Data_080eeb54[texture_index];
                                    ((DrawRectangleFn)slot_pair[sel])(draw_destination, (u8 *)work + cell,
                                        column->x - (texture_index >> 1),
                                        column->y - var_r5_297, texture_index, var_r5_297);
                                }
                            }
                        }
                        n = 0;
                        if (work->effect->target_count != 0) {
                            cue_frame = sp0C + 4;
                            do {
                                id_offset = n * 2 + 0x24;
                                if (sp24 == cue_frame) {
                                    if (!(i & 1)) {
                                        Func_080f9010(0x85);
                                    }
                                    Func_080d6888(*(s16 *)((u8 *)work->effect + id_offset), 7, 5, n, 3);
                                }
                                n += 1;
                            } while (n != work->effect->target_count);
                        } else {
                            cue_frame = sp0C + 4;
                        }
                        if ((sp24 == cue_frame) || (sp24 == (sp0C + 8))) {
                            var_r5_297 = 0x02010000;
                            n = 0;
                            while (n != 0x200) {
                                if (((struct DualSpark *)var_r5_297)->age == -1) {
                                    ((struct DualSpark *)var_r5_297)->x = ((Func_08004458() & 0xF) + column->x) - 8;
                                    ((struct DualSpark *)var_r5_297)->y = (Func_08004458() & 0xF) + 0x50;
                                    ((struct DualSpark *)var_r5_297)->age = 0;
                                    break;
                                }
                                var_r5_297 += 0x1C;
                                n += 1;
                            }
                        }
                        i += 1;
                    } while (i != Data_080eeb5e[work->effect->table]);
                }
            }

            i = 0;
            spark = (struct DualSpark *)0x02010000;
            do {
                temp_r2_474 = spark->age;
                if (temp_r2_474 >= 0) {
                    temp_r7_481 = temp_r2_474 / 2;
                    cell_base = (u8 *)0x1E59;
                    if (mode != 0) {
                        cell_base = (u8 *)0xAFF;
                    }
                    ((DrawRectangleFn)rectangle[0])(
                        draw_destination,
                        (u8 *)work + (Data_080eeb88[temp_r7_481] + (s32)cell_base),
                        spark->x - Data_080eeb79[temp_r7_481],
                        spark->y - (s8)Data_080eeb80[temp_r7_481] / 2,
                        Data_080eeb79[temp_r7_481], (s8)Data_080eeb80[temp_r7_481]);
                    ((DrawRectangleFn)rectangle[1])(
                        draw_destination,
                        (u8 *)work + (Data_080eeb88[temp_r7_481] + (s32)cell_base),
                        spark->x,
                        spark->y - (s8)Data_080eeb80[temp_r7_481] / 2,
                        Data_080eeb79[temp_r7_481], (s8)Data_080eeb80[temp_r7_481]);
                    temp_r3_538 = spark->age + 1;
                    spark->age = temp_r3_538;
                    if (temp_r3_538 == 0xE) {
                        spark->age = -1;
                    }
                }
                spark++;
                i += 1;
            } while (i != 0x200);

            Func_080e155c(4, 4);
            Func_080cd52c();
            work->transfer_pending = 1;
            Func_080030f8(1);
            sp24 += 1;
        } while (sp24 != sp20);
    }
    Func_08004278((void *)0x080CD261);
    Func_08002dd8(47);
    Func_08002dd8(46);
    Func_080cdbc0();
}
