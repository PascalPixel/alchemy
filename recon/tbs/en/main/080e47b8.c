/* NONMATCHING: 7808 bytes, candidate 7768, 3271 differing halfwords,
 * 1536 halfword edits (2026-09-25). All 217 calls follow the reference
 * sequence. Typed particle seeds and direct projection arrays reduce false
 * shared lifetimes; inline drawing scopes reload each image at its call.
 * WALL: Remaining local lifetimes, callback loads and drawing-loop structure.
 * Pascal requested staying on this complete owner until byte exact. */
#include "TYPES.H"
#include "CALLBACK_SCHEDULER.H"
#include "EFFECT_STEP.H"
#include "MOTION_OBJECT.H"
#include "BATTLE_EFX.H"
#include "BATTLE_EFFECT_WORK.H"
typedef BattleEffectDrawRectangle RectangleBlit;
extern u8 Value_0000008d;
extern u8 Value_000000a3;
extern u8 Value_000000a4;
extern u8 Value_000000a0;
extern u8 Value_000000bb;
extern u8 Value_000000b9;
extern u8 Value_000000c0;

extern u8 Value_00000053;
extern u8 Value_0000006f;
extern u8 Value_00000073;
extern u8 Value_00000079;
extern u8 Value_0000007d;
extern u8 Value_0000008e;
extern u8 Value_00000090;
extern u8 Value_00000092;
extern u8 Value_00000094;
extern u8 Value_00000096;
extern u8 Value_00000099;
extern u8 Value_0000009e;
extern u8 Value_000000a9;
extern u8 Value_000000ab;
extern u8 Value_000000ac;
extern u8 Value_000000ad;
extern u8 Value_000000ae;
extern u8 Value_000000b4;
extern u8 Value_000000b8;
extern u8 Value_000000c3;
extern u8 Value_000000c4;
extern u8 Value_000000ce;

s32 Func_080022ec(s32 numerator, s32 denominator);
s32 Func_080022fc(s32 numerator, s32 denominator);
s32 Func_0800231c(s32 angle);
s32 Func_08002322(s32 angle);
void Func_08002dd8();
void Func_080030f8();
u32 Func_08004458(void);
void Func_080049ac();
void Func_08004bd4();
void Func_08004c1c();
void Func_08004c6c();
void Func_080051d8();
void Func_08009080();
void Func_08009088();
u32 Func_080b5070(s32 actor_id);
void Func_080b5088();
struct BattleObjectSlot *Func_080b5098(s32 actor_id);
void Func_080b50e8();
void Func_080cd52c();
void Func_080cd594();
void Func_080cdb24();
void Func_080cdbc0();
void Func_080cef64(s32 alternate, RectangleBlit *output);
void Func_080d4604();
void Func_080d52a4();
void Func_080d6888(s32 set_id, s32 object_value, s32 group_value, s32 state_slot, s32 state_value);
void Func_080d9ac4();
void Func_080dea70();
void Func_080df9d0();
void Func_080e155c(s32 x, s32 y);
void Func_080e46f0();

void Func_080f9010();

/* FAKEMATCH: Inline scope keeps each resource call's arguments local. */
static __inline__ void LoadResource(s32 id, void *dest, s32 skip, s32 copy)
{
    Resource_LoadAndDecompress(id, dest, skip, copy);
}

/* FAKEMATCH: Keep packing arguments within their own inline scope. */
static __inline__ void PackRows(void *src, void *dest, s32 width, s32 height)
{
    Func_080df9d0(src, dest, width, height);
}

/* FAKEMATCH: Inline scope makes each clearing call reload its byte count. */
static __inline__ void ClearWords(s32 dest, u32 size)
{
    ((s32 (*)(s32, u32))0x03000164)(dest, size);
}

extern u16 Data_080ede48[];
extern u8 Data_080ede9f[];
extern u8 Data_080edea5[];
extern u8 Data_080edeab[];
extern u16 Data_080edeb2[];
extern u16 Data_080edebe[];
extern u8 Data_080edeca[];
extern u8 Data_080eded0[];
extern u8 Data_080eedd0[];
extern u8 Data_080eedd4[];
extern u8 Data_080eede2[];

/* FAKEMATCH: Inline scope reloads each image address at its drawing call. */
static __inline__ void DrawImage(s32 canvas, s32 pixels, s32 x, s32 y,
    s32 width, s32 height, RectangleBlit *draw)
{
    (*draw)((void *)canvas, (void *)pixels, x, y, width, height);
}

/* FAKEMATCH: The inline helper retains size across the transform calls. */
static __inline__ void DrawInitialSpark(s32 particle, s32 canvas, s32 sprites,
    s32 *target, s32 *screen, s32 kind, RectangleBlit *blitters, s32 size)
{
    Func_080049ac();
    Func_08004c6c(*(s32 *)(particle + 20));
    Func_08004bd4(*(s32 *)(particle + 12));
    Func_08004c1c(*(s32 *)(particle + 16));
    EffectPosition_ApplyBaseAndYOffset((s32 *)particle, (struct EffectPosition *)screen);
    screen[0] = screen[0] / 2 + target[0];
    if (kind <= 7)
        screen[1] = screen[1] + target[1] - 8;
    else if (kind == 35)
        screen[1] = screen[1] + target[1] + 44;
    else
        screen[1] = screen[1] + target[1] + 12;
    if (screen[2] < -60)
        screen[2] = -60;
    if (screen[2] > 60)
        screen[2] = 60;
    screen[2] += 60;
    ((RectangleBlit)blitters[1])((void *)canvas,
        (void *)(sprites + Data_080ede48[size - 1]),
        screen[0] - size / 2, screen[1] - size, size, size * 2);
}

void Func_080e47b8(s32 a0, s32 a1)
{
    void **heap_cache;
    void **heap_cursor;
    s32 resource;
    s32 p10;
    s32 p10b;
    s32 p10c;
    s32 p11b;
    s32 p5b;
    s32 p5c;
    s32 p8;
    s32 p8b;
    s32 p8e;
    s32 p9;
    s32 p9b;
    s32 rec7;
    s32 rec8;
    s32 record;
    s32 value;
    s32 target_pos[3];
    s32 source_pos[3];
    s32 moving_pos[3];
    s32 r2;
    s32 v0;
    s32 v3;
    s32 none;
    s32 v5;
    s32 v10;
    s32 v6;
    s32 v9;
    s32 v2;
    s32 v7;
    s32 base5_2014000;
    s32 v11;
    s32 base5_3001f0c;
    s32 base6_3001e50;
    s32 base7_80eede2;
    s32 v1;
    s32 kind;
    s32 command;
    struct BattleEffectWork *work;
    s32 canvas;
    s32 frame;
    s32 matrix;
    s32 sprites;
    s32 scroll_pos;
    s32 scroll_speed;
    s32 duration;
    s32 source_screen;
    s32 target_screen;
    s32 draw_pair;
    s32 saved_velocity_x;
    s32 saved_velocity_y;
    s32 saved_velocity_z;
    s32 saved_acceleration;
    s32 saved_vertical_strength;
    struct MotionObject *source_actor;
    s32 motion;
    s32 kind_from_two;
    s32 kind_from_four;
    /* FAKEMATCH: Declaration order preserves the two temporary stack slots. */
    s32 slot12;
    s32 position;
    struct MotionObject *target_actor;
    struct EffectStep *seed;

    u8 *p5;
    RectangleBlit p4;
    u8 *p6;
    s32 velocity[3];
    s32 spark_screen[3];
    RectangleBlit blitters[2];
    u8 projected[12];

    kind = a1;
    command = a0;
    heap_cache = (void **)0x03001eec;
    heap_cursor = heap_cache;
    work = (struct BattleEffectWork *)*heap_cursor++;
    canvas = (s32)*heap_cursor;
    matrix = (s32)*(void **)((u8 *)heap_cache - 0x6c);
    sprites = (s32)heap_cache[2];
    work->effect = (void *)command;
    if (kind == 11 || kind == 8 || kind == 32) {
        Func_080cdb24(0);
    } else {
        Func_080cd594(0);
    }
    *(volatile u16 *)0x04000052 = 0x1010;
    LoadResource((s32)&Value_00000073, sprites, 0, 0);
    LoadResource((s32)&Value_00000096, work, 1, 0);
    LoadResource((s32)&Value_00000099, (void *)0x02010000, 1, 0);
    PackRows((void *)0x02010000, (void *)((s32)work + 0x5100), 40, 0x120);
    if (kind == 5 || kind == 23) {
        LoadResource((s32)&Value_0000007d, (void *)0x02010000, 1, 0);
    } else if (kind == 12) {
        LoadResource((s32)&Value_000000a9, (void *)0x02010000, 1, 0);
    } else if (kind == 6 || kind == 27) {
        LoadResource((s32)&Value_000000ce, (void *)0x02010000, 1, 0);
        LoadResource((s32)&Value_000000c4, (void *)0x02010c56, 1, 0);
    } else if (kind == 31) {
        LoadResource((s32)&Value_00000079, (void *)0x02010000, 1, 1);
    } else if (kind == 8) {
        LoadResource((s32)&Value_000000c3, (void *)0x02010000, 1, 1);
    } else if (kind == 14) {
        LoadResource((s32)&Value_0000006f, (void *)0x02010000, 1, 0);
    } else if (kind == 30) {
        LoadResource((s32)&Value_000000ce, (void *)0x02010000, 1, 0);
    } else if (kind == 16) {
        LoadResource((s32)&Value_000000b8, (void *)0x02010000, 1, 0);
    } else if (kind == 20) {
        LoadResource((s32)&Value_000000b4, (void *)0x02010000, 1, 0);
    } else if ((u32)(kind - 33) <= 1) {
        LoadResource((s32)&Value_00000053, (void *)0x02010000, 1, 0);
    } else if (kind != 11 && kind != 32) {
        LoadResource((s32)&Value_0000009e, (void *)0x02010000, 1, 0);
    }

    switch (kind) {
    case 0:
    case 4:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
    case 33:
        LoadResource((s32)&Value_00000094, (void *)0x02013c56, 1, 1);
        break;
    case 2:
    case 14:
    case 15:
    case 16:
    case 17:
    case 18:
    case 19:
        LoadResource((s32)&Value_00000092, (void *)0x02013c56, 1, 1);
        break;
    case 3:
    case 5:
    case 20:
    case 21:
    case 22:
    case 23:
    case 24:
    case 25:
    case 34:
    case 35:
        LoadResource((s32)&Value_0000008e, (void *)0x02013c56, 1, 1);
        break;
    case 1:
    case 6:
    case 26:
    case 27:
    case 28:
    case 29:
    case 30:
    case 31:
    case 32:
        LoadResource((s32)&Value_00000090, (void *)0x02013c56, 1, 1);
        break;
    case 100:
        LoadResource((s32)&Value_00000092, (void *)0x02013c56, 1, 1);
        break;
    }
    work->transfer_mode = 2;
    if (kind == 12) {
        work->transfer_value = 75;
    } else {
        work->transfer_value = 50;
    }
    Scheduler_AddOrUpdateCallback(0x80cd261, 0x480);
    source_screen = (s32)source_pos;
    EffectPosition_ApplyStepAndYOffset(((struct BattleEffectArgument *)work->effect)->actors[0], (struct EffectPosition *)source_pos);
    target_screen = (s32)target_pos;
    EffectPosition_ApplyStepAndYOffset(((struct BattleEffectArgument *)work->effect)->actor, (struct EffectPosition *)target_pos);
    draw_pair = (s32)blitters;
    Func_080cef64(((struct BattleEffectArgument *)work->effect)->side, blitters);
    *(s32 *)(((s32)work + 0x77b4)) = 24;
    *(s32 *)(((s32)work + 0x77b8)) = 0;
    target_actor = Func_080b5098(((struct BattleEffectArgument *)work->effect)->actor)->object;
    {

        seed = (struct EffectStep *)((u8 *)(s32)work + 0x7080);
        for (v10 = 0; v10 != 64; v10++, seed++) {
            seed->x = (Func_08004458() & 63) + 32;
            seed->y = 0;
            seed->z = 0;
            seed->velocity_x = Func_08004458() & 0xffff;
            seed->velocity_y = Func_08004458() & 0xffff;
            seed->velocity_z = Func_08004458() & 0xffff;
        }
    }
    Func_08009088((s32)target_actor, 0);
    position = (s32)moving_pos;
    *(s32 *)(position) = target_actor->x;
    *(s32 *)(position + 4) = (target_actor->y + 0x500000);
    *(s32 *)(position + 8) = target_actor->z;
    saved_velocity_x = target_actor->velocity_x;
    saved_velocity_y = target_actor->velocity_y;
    saved_velocity_z = target_actor->velocity_z;
    saved_acceleration = target_actor->acceleration;
    saved_vertical_strength = target_actor->vertical_motion_strength;
    target_actor->velocity_x = 0;
    target_actor->velocity_y = 0;
    target_actor->velocity_z = 0;
    target_actor->acceleration = 0;
    target_actor->vertical_motion_strength = 0;
    EffectPosition_ApplyStepAndYOffset(((struct BattleEffectArgument *)work->effect)->actor, (struct EffectPosition *)target_screen);
    *(s32 *)(target_screen) = (((s32)(*(s32 *)(target_screen)) / 2));
    Func_080f9010(212);
    frame = 0;
    p8b = spark_screen;
    do {
    none = 0;
    p10 = none;
    v5 = p10;
    v6 = ((s32)work + 0x7080);
    v9 = none;
    v10 = p10;
    do {
        if (*(s32 *)(v6) >= 0) {
            if (frame >= (v10 / 4)) {
                DrawInitialSpark(v6, canvas, sprites,
                    (s32 *)target_screen, spark_screen, kind, blitters, 5);
                *(s32 *)(v6) = (*(s32 *)(v6) - 4);
            }
            v9 = (v9 + 1);
        }
        v10 = (v10 + 1);
        v6 = (v6 + 28);
    } while (v10 != 64);
    if (kind <= 7) {
        p5b = v9;
        if (p5b <= 63) {
            Func_080049ac();
            Func_080051d8(matrix, (matrix + 12));
            EffectPosition_ApplyBaseAndYOffset((s32 *)moving_pos, (struct EffectPosition *)p8b);
            v2 = (((s32)(*(s32 *)p8b) / 2));
            *(s32 *)p8b = (((s32)(*(s32 *)p8b) / 2));

            blitters[0](canvas, 0x2013c56, (v2 - 10), (*(s32 *)(p8b + 4) - 4), 20, 40);
        }
    }
    work->transfer_pending = 1;
    Func_080030f8(1);
    frame = (frame + 1);
    } while (frame != 32);
    if (kind == 11) {
        *(volatile u16 *)0x04000020 = 0x100;
        if (((struct BattleEffectArgument *)work->effect)->side == 0) {
            v3 = (frame - *(s32 *)(source_screen));
            goto L_080e4e54;
        }
        *(volatile s32 *)0x04000028 = ((96 - *(s32 *)(source_screen)) << 8);
    } else {
        if (kind == 32) {
            *(volatile u16 *)0x04000020 = 0x100;
            if (((struct BattleEffectArgument *)work->effect)->side == 0) {
                scroll_pos = -0x800000;
                scroll_speed = 0xc0000;
            } else {
                scroll_pos = 0x80000;
                scroll_speed = -0xc0000;
            }
            v3 = (scroll_pos >> 16);
            L_080e4e54:;
            *(volatile s32 *)0x04000028 = (v3 << 8);
        }
    }
    if (kind == 8) {
        *(volatile u16 *)0x04000020 = 0x100;
        *(volatile s32 *)0x04000028 = ((64 - *(s32 *)(source_screen)) << 8);
        work->transfer_mode = 1;
        v6 = 0;
        work->transfer_value = 0;
        ClearWords(0x6004000, 0x4000);
        ClearWords(canvas, 0x4000);
        {
            s32 shown = 0;

            *(volatile u16 *)0x04000050 = shown;
        }
    }
    if (kind == 31) {
        *(volatile u16 *)0x04000020 = 0x100;
        if (((struct BattleEffectArgument *)work->effect)->side == 0) {
            v3 = 32;
        } else {
            v3 = 96;
        }
        *(volatile s32 *)0x04000028 = ((v3 - *(s32 *)(source_screen)) << 8);
    }
    if (kind == 15 || kind == 17 || kind == 24 || kind == 26) {
        ClearWords(0x6004000, 0x4000);
        ClearWords(canvas, 0x4000);
        ((struct BattleEffectArgument *)work->effect)->unknown_001c = 0;
        Scheduler_RemoveCallback(0x80cd4b5);
        Scheduler_RemoveCallback(0x80cd261);
        Func_08002dd8(47);
        Func_08002dd8(46);
        Func_08009080((s32)target_actor, 3);
        if (kind == 15) {
            Func_080dea70(command, 9);
        }
        if (kind == 24) {
            Func_080d52a4(command);
        }
        if (kind != 26) {
            return;
        }
        Func_080dea70(command, 8);
        return;
    }
    Func_08009088((s32)target_actor, 16);
    target_actor->velocity_x = saved_velocity_x;
    target_actor->velocity_y = saved_velocity_y;
    target_actor->velocity_z = saved_velocity_z;
    target_actor->acceleration = saved_acceleration;
    target_actor->vertical_motion_strength = saved_vertical_strength;
    if (kind == 35) {
        ClearWords(0x6004000, 0x4000);
        ClearWords(canvas, 0x4000);
        ((struct BattleEffectArgument *)work->effect)->unknown_001c = 0;
        Scheduler_RemoveCallback(0x80cd4b5);
        Scheduler_RemoveCallback(0x80cd261);
        Func_08002dd8(47);
        Func_08002dd8(46);
        *(s32 *)(command + 24) = 3;
        Func_080d4604(command, 2);
        return;
    }
    source_actor = Func_080b5098(((struct BattleEffectArgument *)work->effect)->actors[0])->object;
    motion = (s32)velocity;
    record = Func_080022ec((source_actor->x - *(s32 *)(position)), 6);
    *(s32 *)(motion) = record;
    record = Func_080022ec(((source_actor->y - *(s32 *)(position + 4)) + 0x1e0000), 6);
    *(s32 *)(motion + 4) = record;
    record = Func_080022ec((source_actor->z - *(s32 *)(position + 8)), 6);
    *(s32 *)(motion + 8) = record;
    none = 0;
    v3 = ((s32)work + 0x7098);
    v10 = none;
    do {
        v10 = (v10 + 1);
        p5c = v10;
        v5 = p5c;
        *(s32 *)(v3) = none;
        v3 = (v3 + 28);
        v10 = p5c;
    } while (v5 != 64);
    if (kind != 14) {
        s32 height = (s32)Func_080b5070(((struct BattleEffectArgument *)work->effect)->actors[0]) / 2;

        seed = (struct EffectStep *)((s32)work + 0x7080);
        for (v10 = 0; v10 != 32; v10++, seed++) {
            seed->y = height;
            seed->x = source_actor->x;
            seed->z = source_actor->z;
            if (kind == 31) {
                seed->velocity_x = ((s32)(Func_08004458() & 255) - 127) << 12;
                seed->velocity_y = ((s32)(Func_08004458() & 255) - 64) << 10;
            } else {
                seed->velocity_x = ((s32)(Func_08004458() & 255) - 127) << 12;
                seed->velocity_y = ((s32)(Func_08004458() & 255) - 64) << 12;
            }
            seed->velocity_z = ((s32)(Func_08004458() & 255) - 127) << 12;
            seed->variant = v10 / 2 + 32;
        }
    }
    if (kind == 11) {
        LoadResource((s32)&Value_000000ab, work, 1, 1);
        LoadResource((s32)&Value_000000ac, (void *)0x02010000, 1, 0);
        *(volatile u16 *)0x04000052 = 0xe10;
    }
    if (kind == 32) {
        LoadResource((s32)&Value_000000ad, work, 1, 1);
        LoadResource((s32)&Value_000000ae, (void *)0x02010000, 1, 0);
        *(volatile u16 *)0x04000052 = 0xe10;
    }
    if (kind != 7 && kind != 13 && kind != 18 && kind != 11 &&
        kind != 32 && kind != 19) {
        s32 height = kind == 12 ? 0 : 0x140000;

        seed = (struct EffectStep *)0x02014000;
        for (v10 = 0; v10 != 64; v10++, seed++) {
            seed->y = height;
            seed->x = source_actor->x;
            seed->z = source_actor->z;
            if (kind == 5 || kind == 23) {
                seed->velocity_x = ((s32)(Func_08004458() & 255) - 127) << 11;
                seed->velocity_y = (Func_08004458() & 255) << 11;
                seed->velocity_z = ((s32)(Func_08004458() & 255) - 127) << 11;
            } else if (kind == 25) {
                seed->velocity_x = ((s32)(Func_08004458() & 255) - 127) << 11;
                seed->velocity_y = (Func_08004458() & 127) << 10;
                seed->velocity_z = ((s32)(Func_08004458() & 255) - 127) << 11;
            } else {
                seed->velocity_x = ((s32)(Func_08004458() & 255) - 127) << 10;
                seed->velocity_y = (Func_08004458() & 127) << 10;
                seed->velocity_z = ((s32)(Func_08004458() & 255) - 127) << 10;
            }
            seed->variant = 0;
        }
    }
    kind_from_two = (kind - 2);
    if ((u32)kind_from_two > 1) {
        if (kind != 12) {
            if (kind != 22) {
                if (kind != 29) {
                    if (kind != 28) {
                        goto L_080e5264;
                    }
                }
            }
        }
    }
    Scheduler_AddOrUpdateCallback(0x80dbb9d, 0x480);
    L_080e5264:;
    kind_from_four = kind - 4;
    if ((u32)kind_from_four <= 2 || kind == 23 || kind == 30 ||
        kind == 27 || kind == 33 || kind == 34 || kind == 100) {
        duration = 32;
    } else if ((u32)kind <= 3 || kind == 8 || kind == 9 || kind == 10 ||
               kind == 22 || kind == 25 || kind == 29 || kind == 31 || kind == 14) {
        duration = 48;
    } else if (kind == 21) {
        duration = 20;
    } else if (kind == 11 || kind == 32 || kind == 20) {
        duration = 40;
    } else if (kind == 28 || kind == 12) {
        duration = 64;
    } else {
        duration = 80;
    }

    frame = 0;
    while (frame != duration) {
    if (kind != 11) {
        if (kind != 32) {
            none = 0;
            v7 = 0x40000;
            v5 = (frame << 12);
            v10 = none;
            v6 = (s32)work + 0x6980;
            do {
                value = Func_08002322(v5);
                v10 = (v10 + 1);
                *(s32 *)v6 = ((0x40000 - (value << 2)) >> 10);
                v6 += 4;
                v5 = (v5 + 0x800);
            } while (v10 != 160);
        }
    }
    if (frame <= 2) {
        EffectPosition_ApplyStepAndYOffset(((struct BattleEffectArgument *)work->effect)->actor, (struct EffectPosition *)target_screen);
        *(s32 *)(target_screen) = (((s32)(*(s32 *)(target_screen)) / 2));
        *(s32 *)(target_screen + 4) += 16;
        v5 = 0x7828;
    }
    if (kind != 11) {
        if (kind != 8) {
            if (kind != 32) {
                if (kind == 33) {
                    goto L_080e53ea;
                }
                if (kind != 34) {
                    if (frame <= 11) {
                        if (((struct BattleEffectArgument *)work->effect)->side == 0) {
                            blitters[0](canvas, ((s32)work + (frame / 2) * 3456), (*(s32 *)(target_screen) - 32), (*(s32 *)(target_screen + 4) - 40), 48, 72);
                        } else {
                            blitters[0](canvas, ((s32)work + (frame / 2) * 3456), *(s32 *)(target_screen), (*(s32 *)(target_screen + 4) - 40), 48, 72);
                        }

                    }
                }
            }
        }
    }
    L_080e53ea:;
    switch (kind) {
    case 33:
        Func_080e46f0((s32)&Value_00000053);
        break;
    case 14:
        Func_080e46f0((s32)&Value_0000006f);
        break;
    case 31:
        Func_080e46f0((s32)&Value_00000079);
        break;
    case 8:
        Func_080e46f0((s32)&Value_000000c3);
        break;
    case 0:
    case 10:
        Func_080e46f0((s32)&Value_0000008d);
        break;
    case 12:
    case 13:
    case 25:
        Func_080e46f0((s32)&Value_000000bb);
        break;
    case 18:
        Func_080e46f0((s32)&Value_000000b9);
        break;
    case 19:
        Func_080e46f0((s32)&Value_000000c0);
        break;
    case 2:
    case 29:
        Func_080e46f0((s32)&Value_000000a4);
        break;
    case 1:
    case 28:
        Func_080e46f0((s32)&Value_000000a3);
        break;
    case 3:
    case 20:
    case 22:
        Func_080e46f0((s32)&Value_000000b4);
        break;
    case 9:
        Func_080e46f0((s32)&Value_000000a0);
        break;
    case 5:
    case 23:
        Func_080e46f0((s32)&Value_0000007d);
        break;
    }
    if (kind != 11) {
        if (kind != 8) {
            if (kind != 32) {
                if ((u32)(frame - 4) <= 11) {
                    ((RectangleBlit)(*(s32 *)(draw_pair + 4)))(canvas, ((s32)work + ((frame - 4) / 2) * 960 + 0x5100), ((((s32)(*(s32 *)(source_screen)) / 2)) - 8), (*(s32 *)(target_screen + 4) - 24), 20, 48);
                }
                Func_080049ac();
                Func_080051d8(matrix, (matrix + 12));
                if (frame > 3) {


                    for (v10 = 0; v10 != 128; v10++) {
                        s32 index = v10 / 2;
                        struct EffectStep *step = (struct EffectStep *)((s32)work + index * 28 + 0x7080);
                        s32 life = step->variant;

                        if (life > 0) {
                            s32 size;

                            EffectPosition_ApplyBaseAndYOffset((s32 *)step, (struct EffectPosition *)((s32 *)projected));
                            size = (life >> 4) + 1;
                            ((s32 *)projected)[0] /= 2;
                            blitters[index & 1](canvas,
                                sprites + Data_080ede48[size - 1],
                                ((s32 *)projected)[0] - size / 2, ((s32 *)projected)[1] - size,
                                size, size * 2);
                            EffectStep_AdvanceWithGravity3D(step, 60, -0x1000);
                            step->variant--;
                        }
                    }
                }
            }
        }
    }
    if (kind == 7 || kind == 13 || kind == 18 || kind == 19) {
        if (frame == 50) {
            Func_080d6888(((struct BattleEffectArgument *)work->effect)->actor, 7, -1, -1, 0);
        }
        if (frame == 79) {
            Func_080d6888(((struct BattleEffectArgument *)work->effect)->actor, 0, -1, -1, 0);
        }
        if (frame == 12) {
            base5_2014000 = 0x2014000;
            none = 0;
            v6 = 255;
            v10 = none;
            do {
                *(s32 *)(base5_2014000) = source_actor->x;
                *(s32 *)(base5_2014000 + 4) = 0x140000;
                *(s32 *)(base5_2014000 + 8) = source_actor->z;
                record = Func_08004458();
                *(s32 *)(base5_2014000 + 12) = (((record & 255) - 128) << 10);
                record = Func_08004458();
                *(s32 *)(base5_2014000 + 16) = (((record & 255) - 128) << 10);
                record = Func_08004458();
                v10 = (v10 + 1);
                *(s32 *)(base5_2014000 + 20) = (((record & 255) - 128) << 10);
                *(s32 *)(base5_2014000 + 24) = 0;
                base5_2014000 = (base5_2014000 + 28);
            } while (v10 != 64);
        }
        if (frame <= 11) {
            goto L_080e640e;
        }
        {
            struct EffectStep *step = (struct EffectStep *)0x02014000;

            struct MotionObject *target;
            s32 height;

            target = Func_080b5098(((struct BattleEffectArgument *)work->effect)->actor)->object;
            height = (s32)Func_080b5070(((struct BattleEffectArgument *)work->effect)->actor) / 2;
            for (v10 = 0; v10 != 32; v10++, step++) {
                if (step->variant >= 0) {
                    s32 size = (v10 & 1) + 6;

                    EffectPosition_ApplyBaseAndYOffset((s32 *)step, (struct EffectPosition *)((s32 *)projected));
                    ((s32 *)projected)[0] >>= 1;
                    blitters[0](canvas,
                        sprites + Data_080ede48[size - 1],
                        ((s32 *)projected)[0] - (u32)size / 2, ((s32 *)projected)[1] - size,
                        size, size * 2);
                    EffectStep_AdvanceWithGravity3D(step, 62, 0);
                    if (frame > v10 + 22) {
                        s32 dx = (target->x - step->x) >> 8;
                        s32 dy = (target->y + height - step->y) >> 8;
                        s32 dz = (target->z - step->z) >> 8;

                        step->velocity_x += dx;
                        step->velocity_y += dy;
                        step->velocity_z += dz;
                        if ((u32)(dx + 0xfff) <= 0x1ffe &&
                            (u32)(dz + 0xfff) <= 0x1ffe)
                            step->variant = -1;
                    }
                }
            }
        }
        goto L_080e640e;
    }
    if (kind == 21) {
        goto L_080e640e;
    }
    if (kind != 6) {
        if (kind == 27) {
            goto L_080e57c8;
        }
    } else {
        L_080e57c8:;
        if ((u32)(frame - 6) <= 13) {
            none = 0;
            v5 = frame;
            v10 = none;
            do {
                blitters[0](canvas, ((((v5 / 2) & 3) * 2880) + 0x02010c56), ((((s32)(*(s32 *)(source_screen)) / 2)) - 8), 0, 24, 104);
                v10 = (v10 + 1);
                v5 = (v5 + 3);
            } while (v10 != 2);
        }
        if ((u32)(frame - 8) > 15) {
            goto L_080e640e;
        }
        for (v10 = 0; v10 != 3; v10++) {
            s32 image = v10 & 3;
            s32 angle = Func_08004458() & 0xffff;
            s32 image_x;
            s32 image_y;
            s32 value;

            value = Func_08002322(angle);
            image_x = ((value << 3) >> 16) + *(s32 *)source_screen / 2 -
                Data_080edeca[image] / 2;
            value = Func_0800231c(angle);
            image_y = ((value << 5) >> 16) - Data_080eded0[image] / 2;
            Func_08002dd8(47);
            Func_08002dd8(46);
            value = Func_08004458();
            BattleEffect_LoadWork(47, 7, 7, Data_080eedd0[value & 3] | 3, 2);
            ((RectangleBlit)*(s32 *)0x03001f0c)(canvas,
                0x02010000 + Data_080edebe[image], image_x, image_y + 56,
                Data_080edeca[image], Data_080eded0[image]);
            Func_08002dd8(47);
            Func_080cef64(((struct BattleEffectArgument *)work->effect)->side, blitters);
        }
        goto L_080e640e;
    }
    if (kind != 14) {
    } else {
        s32 rise;
        s32 scroll;

        Func_08002dd8(47);
        Func_08002dd8(46);
        if ((u32)frame > 23) {
            goto L_080e5ab6;
        }
        slot12 = *(s32 *)source_screen / 2;
        rise = frame * 32 - 232;
        scroll = frame * 16 - 48;
        if (rise > 0)
            rise = 0;
        while (scroll > 104)
            scroll -= 104;
        BattleEffect_LoadWork(47, 7, 7, 3, 2);
        p9 = rise + scroll;
        p10b = slot12 - 8;
        DrawImage(canvas, 0x02010000, p10b, p9 - 104, 17, 104,
            (RectangleBlit *)0x03001f0c);
        DrawImage(canvas, 0x02010000, p10b, p9, 17, 104 - scroll,
            (RectangleBlit *)0x03001f0c);
        DrawImage(canvas, 0x020106e8, slot12 - 17, rise + 47, 34, 65,
            (RectangleBlit *)0x03001f0c);
        Func_08002dd8(47);
        if (frame == 8) {
            *(s32 *)(((s32)work + 0x77a8)) = frame;
        }
        if (frame <= 1) {
            goto L_080e5ab6;
        }
        {
            s32 emitted = 0;

            seed = (struct EffectStep *)((s32)work + 0x7080);
            for (v10 = 0; v10 != 64; v10++, seed++) {
                if (seed->variant == 0) {
                    seed->x = source_actor->x;
                    seed->y = 0x140000;
                    seed->z = source_actor->z;
                    seed->velocity_x = ((Func_08004458() & 255) - 127) << 12;
                    seed->velocity_y = ((Func_08004458() & 255) - 64) << 10;
                    seed->velocity_z = ((Func_08004458() & 255) - 127) << 12;
                    emitted++;
                    seed->variant = v10 / 2 + 32;
                    if (emitted == 4)
                        break;
                }
            }
        }
        goto L_080e5ab6;
    }
    if (kind == 31) {
        Func_08002dd8(47);
        Func_08002dd8(46);
        if ((u32)(frame - 4) <= 19) {
            p5 = *(s32 *)(source_screen);
            BattleEffect_LoadWork(47, 7, 7, 3, 2);
            base6_3001e50 = 0x3001e50;
            DrawImage(canvas, 0x02010000, ((s32)p5 / 2) - 24, 48, 24, 48, (RectangleBlit *)(base6_3001e50 + 188));
            Func_08002dd8(47);
            BattleEffect_LoadWork(47, 7, 7, 7, 2);
            DrawImage(canvas, 0x02010000, ((s32)p5 / 2), 48, 24, 48, (RectangleBlit *)(base6_3001e50 + 188));
            Func_08002dd8(47);
        }
        L_080e5ab6:;
        Func_080cef64(((struct BattleEffectArgument *)work->effect)->side, blitters);
        goto L_080e640e;
        v6 = ((u32)r2 >> 17);
    }
    if (kind == 30) {
        if (frame > 15) {
            *(volatile u16 *)0x04000052 = ((0x20 - frame) | 0x1000);
        }
        if (frame <= 5) {
            goto L_080e640e;
        }
        p6 = *(s32 *)(source_screen);
        value = Func_080022fc((((s32)(frame) / 2)), 3);
        v6 = ((((s32)(p6) / 2)) - 20);
        p9b = (value * 2560);
        blitters[0](canvas, (0x2010c56 + p9b), ((((s32)(p6) / 2)) - 20), 16, 40, 32);
        blitters[0](canvas, ((value * 1280) + 0x2012a56), ((((s32)(p6) / 2)) - 20), 48, 40, 32);
        blitters[0](canvas, (p9b + 0x2011156), ((((s32)(p6) / 2)) - 20), 80, 40, 32);
        goto L_080e640e;
    }
    if (kind != 5) {
        if (kind != 23) {
            goto L_080e5c32;
        }
    }
    {
        struct EffectStep *step = (struct EffectStep *)0x02014000;


        for (v10 = 0; v10 != 16; v10++, step++) {
            if (frame >= v10 / 2 + 4) {
                s32 age = step->variant;

                if (age <= 11) {
                    s32 image = age / 2;

                    EffectPosition_ApplyBaseAndYOffset((s32 *)step, (struct EffectPosition *)((s32 *)projected));
                    ((s32 *)projected)[0] /= 2;
                    blitters[0](canvas,
                        0x02010000 + (image << 11),
                        ((s32 *)projected)[0] - 16, ((s32 *)projected)[1] - 32, 32, 64);
                    EffectStep_AdvanceWithGravity3D(step, 60, 0x1000);
                    step->variant++;
                }
            }
        }
    }
    goto L_080e640e;
    L_080e5c32:;
    if (kind == 4) {
    } else {
        if (kind != 11) {
        } else {
            s32 image_y;

            Func_08002322((frame << 9));
            value = Func_0800231c((frame << 9));
            image_y = *(s16 *)(source_screen + 6) + ((value << 2) >> 16) + 16;
            if (frame <= 3) {
                blitters[0](canvas, work, Data_080eedd4[((((struct BattleEffectArgument *)work->effect)->side << 3) - ((struct BattleEffectArgument *)work->effect)->side)], (image_y + Data_080eede2[0]), 57, 98);
                goto L_080e640e;
            }
            if (frame <= 7) {
                blitters[0](canvas, work, Data_080eedd4[((((struct BattleEffectArgument *)work->effect)->side << 3) - ((struct BattleEffectArgument *)work->effect)->side)], (image_y + Data_080eede2[0]), 57, 98);
            }
            v6 = ((s32)work + 0x7828);
            blitters[0](canvas, ((s32)work + 0x15d2), Data_080eedd4[(((((struct BattleEffectArgument *)work->effect)->side << 3) - ((struct BattleEffectArgument *)work->effect)->side) + 1)], (image_y + Data_080eede2[1]), 99, 69);
            if ((u32)(frame - 4) <= 1) {
                ((s32 (*)(s32, u32, u32))0x03000168)(canvas, 0x4000, 0x3f3f3f3f);
            }
            if ((u32)(frame - 6) <= 1) {
                blitters[0](canvas, ((s32)work + 0x3081), Data_080eedd4[(((((struct BattleEffectArgument *)work->effect)->side << 3) - ((struct BattleEffectArgument *)work->effect)->side) + 2)], (image_y + Data_080eede2[2]), 128, 91);
            }
            if ((u32)(frame - 8) <= 1) {
                blitters[0](canvas, 0x2010000, Data_080eedd4[(((((struct BattleEffectArgument *)work->effect)->side << 3) - ((struct BattleEffectArgument *)work->effect)->side) + 3)], (image_y + Data_080eede2[3]), 128, 91);
            }
            if ((u32)(frame - 10) <= 1) {
                blitters[0](canvas, 0x2012d80, Data_080eedd4[(((((struct BattleEffectArgument *)work->effect)->side << 3) - ((struct BattleEffectArgument *)work->effect)->side) + 4)], (image_y + Data_080eede2[4]), 128, 59);
            }
            if ((u32)(frame - 12) <= 1) {
                blitters[0](canvas, 0x2014b00, Data_080eedd4[(((((struct BattleEffectArgument *)work->effect)->side << 3) - ((struct BattleEffectArgument *)work->effect)->side) + 5)], (image_y + Data_080eede2[5]), 122, 29);
            }
            if ((u32)(frame - 14) > 1) {
                goto L_080e640e;
            }
            DrawImage(canvas, 0x20158d2, Data_080eedd4[(((((struct BattleEffectArgument *)work->effect)->side << 3) - ((struct BattleEffectArgument *)work->effect)->side) + 6)], (image_y + Data_080eede2[6]), 76, 25, &blitters[0]);
            goto L_080e640e;
        }
        if (kind != 32) {
        } else {
            scroll_pos = (scroll_pos + scroll_speed);
            if (frame > 6) {
                scroll_speed = (s32)((u32)scroll_speed * 48) / 64;
            }
            *(volatile s32 *)0x04000028 = ((scroll_pos >> 16) << 8);
            if ((u32)(frame - 16) <= 15) {
                *(volatile u16 *)0x04000052 = ((0x10 - (frame - 16)) | 0x1000);
            }
            if ((u32)(frame - 4) <= 1) {
                ((s32 (*)(s32, u32, u32))0x03000168)(canvas, 0x4000, 0x3f3f3f3f);
            }
            if (frame <= 3) {
                if (((struct BattleEffectArgument *)work->effect)->side == 1) {
                    blitters[0](canvas, work, 0, 24, 80, 104);
                } else {
                    blitters[0](canvas, work, 48, 24, 80, 104);
                }
                goto L_080e640e;
            }
            if (frame <= 7) {
                if (((struct BattleEffectArgument *)work->effect)->side == 1) {
                    blitters[0](canvas, work, 0, 24, 80, 104);
                } else {
                    blitters[0](canvas, work, 48, 24, 80, 104);
                }
            }
            if (((struct BattleEffectArgument *)work->effect)->side == 1) {
                blitters[0](canvas, ((s32)work + 0x1e00), 16, 16, 80, 104);
            } else {
                blitters[0](canvas, ((s32)work + 0x1e00), 32, 16, 80, 104);
            }
            if ((u32)(frame - 6) <= 1) {
                blitters[0](canvas, ((s32)work + 0x3e80), 0, 16, 128, 91);
            }
            if ((u32)(frame - 8) <= 1) {
                blitters[0](canvas, 0x2010000, 0, 16, 128, 91);
            }
            if ((u32)(frame - 10) <= 1) {
                blitters[0](canvas, 0x2012d80, 0, 16, 128, 59);
            }
            if ((u32)(frame - 12) <= 1) {
                blitters[0](canvas, 0x2014b00, 0, 16, 128, 29);
            }
            if ((u32)(frame - 14) > 1) {
                goto L_080e640e;
            }
            blitters[0](canvas, 0x2015980, 0, 16, 128, 26);
            goto L_080e640e;
        }
        if (kind == 20) {
            for (v10 = 0; v10 != 12; v10++) {
                if (frame >= v10 + 6 && frame < v10 + 18) {
                    s32 image = (frame - v10 - 6) / 2;
                    s32 image_x = *(s32 *)source_screen / 2 - Data_080ede9f[image] / 2;
                    s32 mirrored = 1;

                    if (v10 & 1)
                        image_x += ((v10 + 1) / 2) * 3;
                    else
                        image_x -= ((v10 + 1) / 2) * 3;
                    if (v10 != 0) {
                        mirrored = 0;
                        if (((v10 - 1) & 3) > 1)
                            mirrored = 1;
                    }
                    blitters[mirrored](canvas, 0x02010000 + Data_080edeb2[image],
                        image_x, Data_080edeab[image] + 48,
                        Data_080ede9f[image], Data_080edea5[image]);
                }
            }
        } else {
            if (kind != 16) {
            } else {
                if (frame == 0) {
                    base5_2014000 = 0x2014000;
                    none = 0;
                    v10 = none;
                    do {
                        record = Func_08004458();
                        *(s32 *)(base5_2014000) = ((127 & record) + 32);
                        *(s32 *)(base5_2014000 + 4) = 0;
                        *(s32 *)(base5_2014000 + 8) = 0;
                        record = Func_08004458();
                        *(s32 *)(base5_2014000 + 12) = (record & 0xffff);
                        record = Func_08004458();
                        *(s32 *)(base5_2014000 + 16) = (record & 0xffff);
                        record = Func_08004458();
                        v10 = (v10 + 1);
                        *(s32 *)(base5_2014000 + 20) = (record & 0xffff);
                        base5_2014000 = (base5_2014000 + 28);
                    } while (v10 != 64);
                    *(s32 *)0x020146e8 = 159;
                }
                {
                    struct EffectStep *step = (struct EffectStep *)0x02014000;

                    s32 *origin = (s32 *)source_screen;

                    for (v10 = 0; v10 != 64; v10++, step++) {
                        if (step->x >= 0 && frame >= v10 / 2) {
                            Func_080049ac();
                            Func_08004bd4(step->velocity_x);
                            Func_08004c1c(step->velocity_y);
                            EffectPosition_ApplyBaseAndYOffset((s32 *)step, (struct EffectPosition *)((s32 *)projected));
                            ((s32 *)projected)[0] = ((s32 *)projected)[0] / 2 + origin[0] / 2;
                            ((s32 *)projected)[1] += origin[1] + 32;
                            blitters[1](canvas,
                                0x02010000 + ((u16 *)0x080eedea)[v10 & 3],
                                ((s32 *)projected)[0] - 4, ((s32 *)projected)[1] - 4, 8, 8);
                            step->x -= 6;
                            if (step->x < 0 && ((v10 & 7) == 0 || v10 == 63)) {
                                Func_080f9010(133);
                                Func_080d6888(((struct BattleEffectArgument *)work->effect)->actors[0],
                                    7, 5, 0, 4);
                            }
                        }
                    }
                }
                goto L_080e640e;
            }
            if (kind == 8) {
                if ((u32)(frame - 5) > 44) {
                    goto L_080e640e;
                }
                if (frame > 25) {
                    v1 = (196 - (frame << 2));
                } else {
                    v1 = ((frame << 4) - 64);
                }
                if (v1 > 96) {
                    v1 = 96;
                }
                blitters[0](canvas, 0x2010000, 48, (104 - v1), 32, v1);
            } else {
                if ((u32)(kind - 33) <= 1) {
                    s32 offset;
                    s32 image_x;

                    if (frame > 5)
                        goto L_080e640e;
                    if (((struct BattleEffectArgument *)work->effect)->side == 0) {
                        offset = (6 - frame) * 3;
                        image_x = *(s32 *)source_screen / 2 + offset * 2;
                    } else {
                        offset = (6 - frame) * 3;
                        image_x = *(s32 *)source_screen / 2 - offset * 2;
                    }
                    blitters[1](canvas, 0x02010000, image_x - 16,
                        *(s32 *)(source_screen + 4) - offset * 4 - 8, 32, 64);
                } else {
                    if (kind == 12) {
                        if (frame > 47) {
                            *(volatile u16 *)0x04000052 = ((0x40 - frame) | 0x1000);
                        }
                        {
                            struct EffectStep *step = (struct EffectStep *)0x02014000;


                            for (v10 = 0; v10 != 16; v10++, step++) {
                                s32 image = Func_080022fc(v10, 3);

                                EffectPosition_ApplyBaseAndYOffset((s32 *)step, (struct EffectPosition *)((s32 *)projected));
                                ((s32 *)projected)[0] /= 2;
                                blitters[v10 & 1](canvas,
                                    0x02010000 + image * 576,
                                    ((s32 *)projected)[0] - 12, ((s32 *)projected)[1] - 12, 24, 24);
                                EffectStep_AdvanceWithGravity3D(step, 60, 1 << ((v10 & 3) + 11));
                                step->variant++;
                            }
                        }
                    } else {
                        if (kind != 100) {
                            struct EffectStep *step = (struct EffectStep *)0x02014000;


                            for (v10 = 0; v10 != 16; v10++, step++) {
                                if (frame >= v10 + 4) {
                                    s32 age = step->variant;

                                    if (age <= 23) {
                                        s32 image = age / 4;

                                        EffectPosition_ApplyBaseAndYOffset((s32 *)step, (struct EffectPosition *)((s32 *)projected));
                                        ((s32 *)projected)[0] /= 2;
                                        blitters[v10 & 1](canvas,
                                            0x02010000 + image * 1152,
                                            ((s32 *)projected)[0] - 12, ((s32 *)projected)[1] - 24, 24, 48);
                                        if (kind == 25)
                                            EffectStep_AdvanceWithGravity3D(step, 60, 0x400);
                                        else
                                            EffectStep_AdvanceWithGravity3D(step, 60, 0x1000);
                                        step->variant++;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    L_080e640e:;
    if (kind <= 7) {
        if (frame <= 5) {
            EffectPosition_ApplyBaseAndYOffset((s32 *)position, (struct EffectPosition *)projected);
            v2 = (((s32)(*(s32 *)(projected)) / 2));
            *(s32 *)(projected) = (((s32)(*(s32 *)(projected)) / 2));
            ((RectangleBlit)(*(s32 *)(draw_pair + 4)))(canvas, 0x2013c56, (v2 - 10), (*(s32 *)(projected + 4) - 4), 20, 40);
            *(s32 *)(position) += *(s32 *)(motion);
            *(s32 *)(position + 4) += *(s32 *)(motion + 4);
            *(s32 *)(position + 8) += *(s32 *)(motion + 8);
        }
    }
    if (frame == 3) {
        Func_080b50e8(-1);
    }
    if (frame == 4) {
        Func_080f9010(134);
    }
    if (frame == 6) {
        if ((u32)kind_from_four > 1) {
            if (kind != 7) {
                if (kind != 13) {
                    if (kind != 18) {
                        if (kind != 19) {
                            if (kind != 23) {
                                if (kind != 34) {
                                    if (kind != 100) {
                                        goto L_080e64c2;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        Func_080b5088(((struct BattleEffectArgument *)work->effect)->actors[0], 4);
        goto L_080e650c;
        L_080e64c2:;
        if (kind == 20 || kind == 14 || kind == 33) {
            Func_080b5088(((struct BattleEffectArgument *)work->effect)->actors[0], 1);
            v3 = 2;
            goto L_080e650e;
        }
        if (kind != 30) {
            if (kind != 8) {
                goto L_080e6510;
            }
        }
        Func_080b5088(((struct BattleEffectArgument *)work->effect)->actors[0], 3);
        L_080e650c:;
        v3 = 8;
        L_080e650e:;
        *(s32 *)(((s32)work + 0x77a8)) = v3;
        L_080e6510:;
    }
    if (frame == 6) {
        Func_080d6888(((struct BattleEffectArgument *)work->effect)->actors[0], 7, 5, 0, 4);
    }
    if (frame == 14) {
        Func_080d6888(((struct BattleEffectArgument *)work->effect)->actors[0], 7, 5, 0, 4);
    }
    Func_080e155c(8, 8);
    Func_080cd52c();
    work->transfer_pending = 1;
    Func_080030f8(1);
    frame = (frame + 1);
    }
    if (kind == 21) {
        ClearWords(0x6004000, 0x4000);
        ClearWords(canvas, 0x4000);
        ((struct BattleEffectArgument *)work->effect)->unknown_001c = 0;
        Scheduler_RemoveCallback(0x80cd4b5);
        Scheduler_RemoveCallback(0x80cd261);
        Func_08002dd8(47);
        Func_08002dd8(46);
        Func_080d9ac4(command);
    } else {
        if ((u32)kind_from_two > 1) {
            if (kind != 12) {
                if (kind != 22) {
                    if (kind != 28) {
                        if (kind != 29) {
                            goto L_080e65e2;
                        }
                    }
                }
            }
        }
        Scheduler_RemoveCallback(0x80dbb9d);
        L_080e65e2:;
        Scheduler_RemoveCallback(0x80cd261);
        Func_08002dd8(47);
        Func_08002dd8(46);
        Func_080cdbc0();
    }
}
