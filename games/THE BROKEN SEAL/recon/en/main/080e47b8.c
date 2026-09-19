#include "TYPES.H"
#include "EFFECT_STEP.H"
#include "MOTION_OBJECT.H"
typedef void (*RectangleBlit)(s32, s32, s32, s32, s32, s32);

s32 Func_080022ec();
s32 Func_080022fc();
s32 Func_0800231c();
s32 Func_08002322();
void Func_08002dd8();
void Func_080030f8();
s32 Func_080041d8();
void Func_08004278();
u32 Func_08004458(void);
void Func_080049ac();
void Func_08004bd4();
void Func_08004c1c();
void Func_08004c6c();
void Func_080051d8();
void Func_08009080();
void Func_08009088();
s32 Func_080b5070();
void Func_080b5088();
struct BattleObjectSlot *Func_080b5098(s32 actor_id);
void Func_080b50e8();
void Func_080cd52c();
void Func_080cd594();
void Func_080cdb24();
void Func_080cdbc0();
void Func_080cef64(s32 alternate, u32 *output);
void Func_080d4604();
void Func_080d52a4();
void Func_080d6888(s32 set_id, s32 object_value, s32 group_value, s32 state_slot, s32 state_value);
void Func_080d9ac4();
void Func_080dea70();
void Func_080df9d0();
void Func_080e0524(s32 resource_id, void *destination, s32 skip_palette, s32 copy_palette);
s32 Func_080e155c();
void Func_080e38b8(struct EffectStep *step, s32 damping, s32 gravity);
s32 Func_080e3944(s32 position, void *output);
void Func_080e396c(s32 actor_id, void *output);
void Func_080e46f0();

s32 Func_080ed408();
void Func_080f9010();

void Func_080e47b8(s32 a0, s32 a1)
{
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
    s32 source_pos[3];
    s32 target_pos[3];
    s32 velocity[3];
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
    s32 base7_0;
    s32 base6_3001e50;
    s32 base7_80eede2;
    s32 v1;
    s32 kind;
    s32 command;
    s32 work;
    s32 canvas;
    s32 matrix;
    s32 sprites;
    s32 source_screen;
    s32 target_screen;
    s32 draw_pair;
    s32 position;
    s32 saved_velocity_x;
    s32 saved_velocity_y;
    s32 saved_velocity_z;
    s32 saved_acceleration;
    s32 saved_vertical_strength;
    s32 frame;
    s32 scroll_pos;
    s32 scroll_speed;
    struct MotionObject *source_actor;
    s32 motion;
    s32 kind_from_two;
    s32 kind_from_four;
    s32 duration;
    s32 slot12;
    struct MotionObject *target_actor;
    u8 *p5;
    u8 *p4;
    u8 *p6;
    s32 spark_screen[3];
    s32 moving_pos[3];
    u32 blitters[2];
    u8 projected[12];

    kind = a1;
    command = a0;
    v0 = *(s32 *)(0x3001eec);
    work = v0;
    canvas = *(s32 *)(0x3001eec + 4);
    matrix = *(s32 *)0x03001e80;
    sprites = *(s32 *)0x03001ef4;
    *(s32 *)((work + 0x7828)) = command;
    switch (kind) {
    case 8:
    case 11:
    case 32:
        Func_080cdb24(0);
        goto L_080e4812;
        break;
    }
    Func_080cd594(0);
    L_080e4812:;
    *(volatile u16 *)0x04000052 = 0x1010;
    Func_080e0524(0x73, sprites, 0, 0);
    Func_080e0524(0x96, work, 1, 0);
    Func_080e0524(0x99, 0x2010000, 1, 0);
    Func_080df9d0(0x2010000, (work + 0x5100), 40, 0x120);
    if (kind == 5 || kind == 23) {
        resource = 0x7d;
    } else if (kind == 12) {
        resource = 0xa9;
    } else if (kind == 6 || kind == 27) {
        Func_080e0524(0xce, 0x02010000, 1, 0);
        Func_080e0524(0xc4, 0x02010c56, 1, 0);
        goto L_080e4912;
    } else if (kind == 31 || kind == 8) {
        if (kind == 31) resource = 0x79;
        else resource = 0xc3;
        Func_080e0524(resource, 0x02010000, 1, 1);
        goto L_080e4912;
    } else if (kind == 14) {
        resource = 0x6f;
    } else if (kind == 30) {
        resource = 0xce;
    } else if (kind == 16) {
        resource = 0xb8;
    } else if (kind == 20) {
        resource = 0xb4;
    } else if ((u32)(kind - 33) <= 1) {
        resource = 0x53;
    } else {
        if (kind != 11 && kind != 32)
            Func_080e0524(0x9e, 0x02010000, 1, 0);
        goto L_080e4912;
    }
    Func_080e0524(resource, 0x02010000, 1, 0);
    L_080e4912:;
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
        Func_080e0524(0x94, 0x02013c56, 1, 1);
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
        Func_080e0524(0x90, 0x02013c56, 1, 1);
        break;
    case 2:
    case 14:
    case 15:
    case 16:
    case 17:
    case 18:
    case 19:
        Func_080e0524(0x92, 0x02013c56, 1, 1);
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
        Func_080e0524(0x8e, 0x02013c56, 1, 1);
        break;
    case 100:
        Func_080e0524(0x92, 0x02013c56, 1, 1);
        break;
    }
    *(s32 *)((work + 0x7780)) = 2;
    if (kind == 12) {
        v3 = 75;
    } else {
        v3 = 50;
    }
    *(s32 *)((work + 0x7784)) = v3;
    Func_080041d8(0x80cd261, 0x480);
    source_screen = (s32)source_pos;
    Func_080e396c(*(s16 *)(*(s32 *)((work + 0x7828)) + 36), (s32)source_pos);
    target_screen = (s32)target_pos;
    Func_080e396c(*(s32 *)(*(s32 *)((work + 0x7828)) + 8), (s32)target_pos);
    draw_pair = (s32)blitters;
    Func_080cef64(*(s32 *)(*(s32 *)((work + 0x7828)) + 4), blitters);
    *(s32 *)((work + 0x77b4)) = 24;
    *(s32 *)((work + 0x77b8)) = 0;
    target_actor = Func_080b5098(*(s32 *)(*(s32 *)((work + 0x7828)) + 8))->object;
    v7 = (s32)target_actor;
    none = 0;
    p8 = none;
    v5 = (work + 0x7080);
    v10 = none;
    do {
        record = Func_08004458();
        *(s32 *)(v5) = ((63 & record) + 32);
        *(s32 *)(v5 + 4) = p8;
        *(s32 *)(v5 + 8) = p8;
        record = Func_08004458();
        *(s32 *)(v5 + 12) = (record & 0xffff);
        record = Func_08004458();
        *(s32 *)(v5 + 16) = (record & 0xffff);
        record = Func_08004458();
        v10 = (v10 + 1);
        *(s32 *)(v5 + 20) = (record & 0xffff);
        v5 = (v5 + 28);
    } while (v10 != 64);
    Func_08009088((s32)target_actor, 0);
    position = (s32)moving_pos;
    *(s32 *)(position) = target_actor->x;
    *(s32 *)(position + 4) = (target_actor->y + 0x500000);
    *(s32 *)(position + 8) = target_actor->z;
    saved_velocity_x = target_actor->velocity_x;
    p5 = target_actor->velocity_y;
    saved_velocity_y = (s32)p5;
    saved_velocity_z = target_actor->velocity_z;
    saved_acceleration = target_actor->acceleration;
    saved_vertical_strength = target_actor->vertical_motion_strength;
    target_actor->velocity_x = 0;
    target_actor->velocity_y = 0;
    target_actor->velocity_z = 0;
    target_actor->acceleration = 0;
    target_actor->vertical_motion_strength = 0;
    Func_080e396c(*(s32 *)(*(s32 *)((work + 0x7828)) + 8), target_screen);
    *(s32 *)(target_screen) = (((s32)(*(s32 *)(target_screen)) / 2));
    Func_080f9010(212);
    frame = 0;
    p8b = spark_screen;
    L_080e4c64:;
    none = 0;
    p10 = none;
    v5 = p10;
    v6 = (work + 0x7080);
    v9 = none;
    v10 = p10;
    do {
        if (*(s32 *)(v6) >= 0) {
            if (frame >= (v10 / 4)) {
                Func_080049ac();
                Func_08004c6c(*(s32 *)(v6 + 20));
                Func_08004bd4(*(s32 *)(v6 + 12));
                Func_08004c1c(*(s32 *)(v6 + 16));
                v5 = spark_screen;
                Func_080e3944(v6, v5);
                *(s32 *)(v5) = ((((s32)(*(s32 *)(v5)) / 2)) + *(s32 *)(target_screen));
                if (kind <= 7) {
                    v3 = ((*(s32 *)(v5 + 4) + *(s32 *)(target_screen + 4)) - 8);
                } else {
                    if (kind == 35) {
                        v3 = ((*(s32 *)(v5 + 4) + *(s32 *)(target_screen + 4)) + 44);
                    } else {
                        v3 = ((*(s32 *)(v5 + 4) + *(s32 *)(target_screen + 4)) + 12);
                    }
                }
                *(s32 *)(v5 + 4) = v3;
                v3 = *(s32 *)(v5 + 8);
                if (*(s32 *)(v5 + 8) < -60) {
                    *(s32 *)(v5 + 8) = -60;
                    v3 = -60;
                }
                if (v3 > 60) {
                    *(s32 *)(v5 + 8) = 60;
                    v3 = 60;
                }
                *(s32 *)(v5 + 8) = (v3 + 60);
                p4 = (u8 *)blitters[1];
                ((RectangleBlit)p4)(canvas, sprites + *(u16 *)(0x080ede48 + 8), spark_screen[0] - 2, spark_screen[1] - 5, 5, 10);
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
            Func_080e3944(moving_pos, p8b);
            v2 = (((s32)(*(s32 *)p8b) / 2));
            *(s32 *)p8b = (((s32)(*(s32 *)p8b) / 2));
            p4 = (u8 *)blitters[0];
            ((RectangleBlit)blitters[0])(canvas, 0x2013c56, (v2 - 10), (*(s32 *)(p8b + 4) - 4), 20, 40);
        }
    }
    *(s32 *)((work + 0x7824)) = 1;
    Func_080030f8(1);
    frame = (frame + 1);
    if (frame != 32) {
        goto L_080e4c64;
    }
    if (kind == 11) {
        *(volatile u16 *)0x04000020 = 0x100;
        if (*(s32 *)(*(s32 *)((work + 0x7828)) + 4) == 0) {
            v3 = (frame - *(s32 *)(source_screen));
            goto L_080e4e54;
        }
        *(volatile s32 *)0x04000028 = ((96 - *(s32 *)(source_screen)) << 8);
    } else {
        if (kind == 32) {
            *(volatile u16 *)0x04000020 = 0x100;
            if (*(s32 *)(*(s32 *)((work + 0x7828)) + 4) == 0) {
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
        *(s32 *)((work + 0x7780)) = 1;
        v6 = 0;
        *(s32 *)((work + 0x7784)) = 0;
        ((void (*)(s32, u32))0x03000164)(0x6004000, 0x4000);
        ((void (*)(s32, u32))0x03000164)(canvas, 0x4000);
        {
            s32 shown = 0;

            *(volatile u16 *)0x04000050 = shown;
        }
    }
    if (kind == 31) {
        *(volatile u16 *)0x04000020 = 0x100;
        if (*(s32 *)(*(s32 *)((work + 0x7828)) + 4) == 0) {
            v3 = 32;
        } else {
            v3 = 96;
        }
        *(volatile s32 *)0x04000028 = ((v3 - *(s32 *)(source_screen)) << 8);
    }
    switch (kind) {
    case 15:
    case 17:
    case 24:
    case 26:
        ((void (*)(s32, u32))0x03000164)(0x6004000, 0x4000);
        ((void (*)(s32, u32))0x03000164)(canvas, 0x4000);
        *(s32 *)(*(s32 *)((work + 0x7828)) + 28) = 0;
        Func_08004278(0x80cd4b5);
        Func_08004278(0x80cd261);
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
        break;
    }
    Func_08009088((s32)target_actor, 16);
    target_actor->velocity_x = saved_velocity_x;
    target_actor->velocity_y = saved_velocity_y;
    target_actor->velocity_z = saved_velocity_z;
    target_actor->acceleration = saved_acceleration;
    target_actor->vertical_motion_strength = saved_vertical_strength;
    if (kind == 35) {
        ((void (*)(s32, u32))0x03000164)(0x6004000, 0x4000);
        ((void (*)(s32, u32))0x03000164)(canvas, 0x4000);
        *(s32 *)(*(s32 *)((work + 0x7828)) + 28) = 0;
        Func_08004278(0x80cd4b5);
        Func_08004278(0x80cd261);
        Func_08002dd8(47);
        Func_08002dd8(46);
        *(s32 *)(command + 24) = 3;
        Func_080d4604(command, 2);
        return;
    }
    source_actor = Func_080b5098(*(s16 *)(*(s32 *)((work + 0x7828)) + 36))->object;
    motion = (s32)velocity;
    record = Func_080022ec((source_actor->x - *(s32 *)(position)), 6);
    *(s32 *)(motion) = record;
    record = Func_080022ec(((source_actor->y - *(s32 *)(position + 4)) + 0x1e0000), 6);
    *(s32 *)(motion + 4) = record;
    record = Func_080022ec((source_actor->z - *(s32 *)(position + 8)), 6);
    *(s32 *)(motion + 8) = record;
    none = 0;
    v3 = (work + 0x7098);
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
        value = Func_080b5070(*(s16 *)(*(s32 *)((work + 0x7828)) + 36));
        v7 = (((s32)(value) / 2));
        none = 0;
        v6 = 255;
        v5 = (work + 0x7080);
        v10 = none;
        do {
            *(s32 *)(v5 + 4) = (((s32)(value) / 2));
            *(s32 *)(v5) = source_actor->x;
            *(s32 *)(v5 + 8) = source_actor->z;
            if (kind == 31) {
                record = Func_08004458();
                *(s32 *)(v5 + 12) = (((record & 255) - 127) << 12);
                record = Func_08004458();
                v0 = (((record & 255) - 64) << 10);
            } else {
                record = Func_08004458();
                *(s32 *)(v5 + 12) = (((record & 255) - 127) << 12);
                record = Func_08004458();
                v0 = (((record & 255) - 64) << 12);
            }
            *(s32 *)(v5 + 16) = v0;
            record = Func_08004458();
            *(s32 *)(v5 + 20) = (((record & 255) - 127) << 12);
            *(s32 *)(v5 + 24) = ((((s32)(v10) / 2)) + 32);
            v10 = (v10 + 1);
            v5 = (v5 + 28);
        } while (v10 != 32);
    }
    if (kind == 11) {
        Func_080e0524(0xab, work, 1, 1);
        Func_080e0524(0xac, 0x2010000, 1, 0);
        *(volatile u16 *)0x04000052 = 0xe10;
    }
    if (kind == 32) {
        Func_080e0524(0xad, work, 1, 1);
        Func_080e0524(0xae, 0x2010000, 1, 0);
        *(volatile u16 *)0x04000052 = 0xe10;
    }
    if (kind != 7) {
        if (kind != 13) {
            if (kind != 18) {
                if (kind != 11) {
                    if (kind != 32) {
                        if (kind != 19) {
                            v7 = 0;
                            if (kind != 12) {
                                v7 = 0x140000;
                            }
                            none = 0;
                            base5_2014000 = 0x2014000;
                            v6 = 255;
                            v10 = none;
                            do {
                                *(s32 *)(base5_2014000 + 4) = v7;
                                *(s32 *)(base5_2014000) = source_actor->x;
                                *(s32 *)(base5_2014000 + 8) = source_actor->z;
                                if (kind != 5) {
                                    if (kind != 23) {
                                        goto L_080e51e0;
                                    }
                                }
                                record = Func_08004458();
                                *(s32 *)(base5_2014000 + 12) = (((record & 255) - 127) << 11);
                                record = Func_08004458();
                                *(s32 *)(base5_2014000 + 16) = ((record & 255) << 11);
                                record = Func_08004458();
                                v0 = (((record & 255) - 127) << 11);
                                goto L_080e522c;
                                L_080e51e0:;
                                if (kind == 25) {
                                    record = Func_08004458();
                                    *(s32 *)(base5_2014000 + 12) = (((record & 255) - 127) << 11);
                                    record = Func_08004458();
                                    *(s32 *)(base5_2014000 + 16) = ((127 & record) << 10);
                                    record = Func_08004458();
                                    v0 = (((record & 255) - 127) << 11);
                                } else {
                                    record = Func_08004458();
                                    *(s32 *)(base5_2014000 + 12) = (((record & 255) - 127) << 10);
                                    record = Func_08004458();
                                    *(s32 *)(base5_2014000 + 16) = ((127 & record) << 10);
                                    record = Func_08004458();
                                    v0 = (((record & 255) - 127) << 10);
                                }
                                L_080e522c:;
                                *(s32 *)(base5_2014000 + 20) = v0;
                                *(s32 *)(base5_2014000 + 24) = 0;
                                v10 = (v10 + 1);
                                base5_2014000 = (base5_2014000 + 28);
                            v5 = base5_2014000;
                            } while (v10 != 64);
                        }
                    }
                }
            }
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
    Func_080041d8(0x80dbb9d, 0x480);
    L_080e5264:;
    kind_from_four = (kind - 4);
    if ((u32)kind_from_four > 2) {
        v5 = kind;
        if (kind != 23) {
            if (kind != 30) {
                if (kind != 27) {
                    if (kind != 33) {
                        if (kind != 34) {
                            if (kind != 100) {
                                goto L_080e528e;
                            }
                        }
                    }
                }
            }
        }
        v5 = kind;
    }
    duration = 32;
    goto L_080e52ea;
    L_080e528e:;
    switch ((u32)kind) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 8:
    case 9:
    case 10:
    case 14:
    case 22:
    case 25:
    case 29:
    case 31:
        duration = 48;
        goto L_080e52ea;
        break;
    }
    v5 = kind;
    duration = 20;
    if (kind != 21) {
        switch (kind) {
        case 11:
        case 20:
        case 32:
            duration = 40;
            v5 = kind;
            goto L_080e52ea;
            break;
        }
        if (kind != 28) {
            duration = 80;
            if (kind != 12) {
                v5 = kind;
                goto L_080e52ea;
            }
        }
        duration = 64;
        v5 = duration;
    }
    L_080e52ea:;
    frame = 0;
    if (duration == 0) {
        goto L_080e657c;
    }
    L_080e52f8:;
    if (kind != 11) {
        if (kind != 32) {
            none = 0;
            v7 = 0x40000;
            v5 = (frame << 12);
            v10 = none;
            v6 = work + 0x6980;
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
        Func_080e396c(*(s32 *)(*(s32 *)((work + 0x7828)) + 8), target_screen);
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
                        if (*(s32 *)(*(s32 *)((work + 0x7828)) + 4) == 0) {
                            ((RectangleBlit)blitters[0])(canvas, (work + ((((((((s32)(frame) / 2)) << 3) - (((s32)(frame) / 2))) << 2) - (((s32)(frame) / 2))) << 7)), (*(s32 *)(target_screen) - 32), (*(s32 *)(target_screen + 4) - 40), 48, 72);
                        } else {
                            ((RectangleBlit)blitters[0])(canvas, (work + ((((((((s32)(frame) / 2)) << 3) - (((s32)(frame) / 2))) << 2) - (((s32)(frame) / 2))) << 7)), *(s32 *)(target_screen), (*(s32 *)(target_screen + 4) - 40), 48, 72);
                        }
                        p4 = (u8 *)blitters[0];
                    }
                }
            }
        }
    }
    L_080e53ea:;
    switch (kind) {
    case 0:
    case 10:
        Func_080e46f0(0x8d);
        break;
    case 1:
    case 28:
        Func_080e46f0(0xa3);
        break;
    case 2:
    case 29:
        Func_080e46f0(0xa4);
        break;
    case 3:
    case 20:
    case 22:
        Func_080e46f0(0xb4);
        break;
    case 5:
    case 23:
        Func_080e46f0(0x7d);
        break;
    case 8:
        Func_080e46f0(0xc3);
        break;
    case 9:
        Func_080e46f0(0xa0);
        break;
    case 12:
    case 13:
    case 25:
        Func_080e46f0(0xbb);
        break;
    case 14:
        Func_080e46f0(0x6f);
        break;
    case 18:
        Func_080e46f0(0xb9);
        break;
    case 19:
        Func_080e46f0(0xc0);
        break;
    case 31:
        Func_080e46f0(0x79);
        break;
    case 33:
        Func_080e46f0(0x53);
        break;
    }
    if (kind != 11) {
        if (kind != 8) {
            if (kind != 32) {
                if ((u32)(frame - 4) <= 11) {
                    p4 = *(s32 *)(draw_pair + 4);
                    ((RectangleBlit)p4)(canvas, ((work + ((((((s32)(frame - 4) / 2)) << 4) - (((s32)(frame - 4) / 2))) << 6)) + 0x5100), ((((s32)(*(s32 *)(source_screen)) / 2)) - 8), (*(s32 *)(target_screen + 4) - 24), 20, 48);
                }
                Func_080049ac();
                Func_080051d8(matrix, (matrix + 12));
                if (frame > 3) {
                    struct EffectStep *particles = (struct EffectStep *)(work + 0x7080);
                    s32 *screen = (s32 *)projected;
                    s32 n;

                    for (n = 0; n != 128; n++) {
                        s32 index = n / 2;
                        struct EffectStep *step = &particles[index];
                        s32 life = step->variant;

                        if (life > 0) {
                            s32 size;

                            Func_080e3944((s32)step, screen);
                            size = (life >> 4) + 1;
                            screen[0] /= 2;
                            ((RectangleBlit)blitters[index & 1])(canvas,
                                sprites + ((u16 *)0x080ede48)[size - 1],
                                screen[0] - size / 2, screen[1] - size,
                                size, size * 2);
                            Func_080e38b8(step, 60, -0x1000);
                            step->variant--;
                        }
                    }
                }
            }
        }
    }
    switch (kind) {
    case 7:
    case 13:
    case 18:
    case 19:
        if (frame == 50) {
            Func_080d6888(*(s32 *)(*(s32 *)((work + 0x7828)) + 8), 7, -1, -1, 0);
        }
        if (frame == 79) {
            Func_080d6888(*(s32 *)(*(s32 *)((work + 0x7828)) + 8), 0, -1, -1, 0);
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
            s32 *screen = (s32 *)projected;
            struct MotionObject *target;
            s32 height;
            s32 n;

            target = Func_080b5098(*(s32 *)(*(s32 *)(work + 0x7828) + 8))->object;
            height = Func_080b5070(*(s32 *)(*(s32 *)(work + 0x7828) + 8)) / 2;
            for (n = 0; n != 32; n++, step++) {
                if (step->variant >= 0) {
                    s32 size = (n & 1) + 6;

                    Func_080e3944((s32)step, screen);
                    screen[0] >>= 1;
                    ((RectangleBlit)blitters[0])(canvas,
                        sprites + ((u16 *)0x080ede48)[size - 1],
                        screen[0] - (u32)size / 2, screen[1] - size,
                        size, size * 2);
                    Func_080e38b8(step, 62, 0);
                    if (frame > n + 22) {
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
        break;
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
                ((RectangleBlit)blitters[0])(canvas, ((((((((((s32)(v5) / 2)) & 3) << 1) + ((((s32)(v5) / 2)) & 3)) << 4) - ((((((s32)(v5) / 2)) & 3) << 1) + ((((s32)(v5) / 2)) & 3))) << 6) + 0x2010c56), ((((s32)(*(s32 *)(source_screen)) / 2)) - 8), 0, 24, 104);
                v10 = (v10 + 1);
                v5 = (v5 + 3);
            } while (v10 != 2);
        }
        if ((u32)(frame - 8) > 15) {
            goto L_080e640e;
        }
        none = 0;
        v10 = none;
        do {
            p8e = v10;
            p8 = ((s32)p8e & 3);
            rec7 = Func_08004458();
            rec8 = Func_08002322((rec7 & 0xffff));
            v6 = ((((rec8 << 3) >> 16) + (((s32)(*(s32 *)(source_screen)) / 2))) - (s32)((u32)*(u8 *)(0x080edeca + (s32)p8) >> 1));
            rec7 = Func_0800231c((rec7 & 0xffff));
            Func_08002dd8(47);
            Func_08002dd8(46);
            record = Func_08004458();
            Func_080ed408(47, 7, 7, (3 | *(u8 *)(0x080eedd0 + (record & 3))), 2);
            p4 = *(s32 *)0x03001f0c;
            ((RectangleBlit)p4)(canvas, (*(u16 *)(0x080edebe + (s32)((s32)p8 << 1)) + 0x2010000), ((((rec8 << 3) >> 16) + (((s32)(*(s32 *)(source_screen)) / 2))) - (s32)((u32)*(u8 *)(0x080edeca + (s32)p8) >> 1)), ((((rec7 << 5) >> 16) - (s32)((u32)*(u8 *)(0x080eded0 + (s32)p8) >> 1)) + 56), *(u8 *)(0x080edeca + (s32)p8), *(u8 *)(0x080eded0 + (s32)p8));
            Func_08002dd8(47);
            Func_080cef64(*(s32 *)(*(s32 *)((work + 0x7828)) + 4), blitters);
            v10 = (p8e + 1);
        } while (v10 != 3);
        goto L_080e640e;
    }
    if (kind != 14) {
    } else {
        Func_08002dd8(47);
        Func_08002dd8(46);
        if ((u32)frame > 23) {
            goto L_080e5ab6;
        }
        slot12 = (((s32)(*(s32 *)(source_screen)) / 2));
        p11b = ((frame << 5) - 232);
        v11 = p11b;
        v7 = ((frame << 4) - 48);
        if (p11b > 0) {
            none = 0;
            v11 = none;
        }
        if (((frame << 4) - 48) > 104) {
            v7 = ((frame << 4) - 48);
            do {
                v7 = (v7 - 104);
            } while (v7 > 104);
        }
        Func_080ed408(47, 7, 7, 3, 2);
        p9 = (v11 + v7);
        p10b = (slot12 - 8);
        p4 = *(s32 *)0x03001f0c;
        ((RectangleBlit)p4)(canvas, 0x2010000, p10b, (p9 - 104), 17, 104);
        p4 = *(s32 *)0x03001f0c;
        ((RectangleBlit)p4)(canvas, 0x2010000, p10b, p9, 17, (104 - v7));
        base5_3001f0c = 0x3001f0c;
        p4 = *(s32 *)base5_3001f0c;
        ((RectangleBlit)p4)(canvas, 0x20106e8, (slot12 - 17), (v11 + 47), 34, 65);
        Func_08002dd8(47);
        if (frame == 8) {
            *(s32 *)((work + 0x77a8)) = frame;
        }
        if (frame <= 1) {
            goto L_080e5ab6;
        }
        none = 0;
        base7_0 = 0;
        v6 = 255;
        v5 = (work + 0x7080);
        v10 = none;
        do {
            if (*(s32 *)(v5 + 24) == 0) {
                *(s32 *)(v5) = source_actor->x;
                *(s32 *)(v5 + 4) = 0x140000;
                *(s32 *)(v5 + 8) = source_actor->z;
                record = Func_08004458();
                *(s32 *)(v5 + 12) = (((record & 255) - 127) << 12);
                record = Func_08004458();
                *(s32 *)(v5 + 16) = (((record & 255) - 64) << 10);
                record = Func_08004458();
                *(s32 *)(v5 + 20) = (((record & 255) - 127) << 12);
                base7_0 = (base7_0 + 1);
                *(s32 *)(v5 + 24) = ((((s32)(v10) / 2)) + 32);
                if (base7_0 == 4) {
                    goto L_080e5ab6;
                }
            }
            v10 = (v10 + 1);
            v5 = (v5 + 28);
        } while (v10 != 64);
        goto L_080e5ab6;
    }
    if (kind == 31) {
        Func_08002dd8(47);
        Func_08002dd8(46);
        if ((u32)(frame - 4) <= 19) {
            p5 = *(s32 *)(source_screen);
            Func_080ed408(47, 7, 7, 3, 2);
            base6_3001e50 = 0x3001e50;
            p4 = *(s32 *)(base6_3001e50 + 188);
            ((RectangleBlit)p4)(canvas, 0x02010000, (((s32)(p5) / 2)) - 24, 48, 24, 48);
            Func_08002dd8(47);
            Func_080ed408(47, 7, 7, 7, 2);
            p4 = *(s32 *)(base6_3001e50 + 188);
            ((RectangleBlit)p4)(canvas, 0x2010000, (((s32)(p5) / 2)), 48, 24, 48);
            Func_08002dd8(47);
        }
        L_080e5ab6:;
        Func_080cef64(*(s32 *)(*(s32 *)((work + 0x7828)) + 4), blitters);
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
        p9b = (((value << 2) + value) << 9);
        ((RectangleBlit)blitters[0])(canvas, (0x2010c56 + p9b), ((((s32)(p6) / 2)) - 20), 16, 40, 32);
        ((RectangleBlit)blitters[0])(canvas, ((((value << 2) + value) << 8) + 0x2012a56), ((((s32)(p6) / 2)) - 20), 48, 40, 32);
        ((RectangleBlit)blitters[0])(canvas, (p9b + 0x2011156), ((((s32)(p6) / 2)) - 20), 80, 40, 32);
        goto L_080e640e;
    }
    if (kind != 5) {
        if (kind != 23) {
            goto L_080e5c32;
        }
    }
    {
        struct EffectStep *step = (struct EffectStep *)0x02014000;
        s32 *screen = (s32 *)projected;
        s32 n;

        for (n = 0; n != 16; n++, step++) {
            if (frame >= n / 2 + 4) {
                s32 age = step->variant;

                if (age <= 11) {
                    s32 image = age / 2;

                    Func_080e3944((s32)step, screen);
                    screen[0] /= 2;
                    ((RectangleBlit)blitters[0])(canvas,
                        0x02010000 + (image << 11),
                        screen[0] - 16, screen[1] - 32, 32, 64);
                    Func_080e38b8(step, 60, 0x1000);
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
            Func_08002322((frame << 9));
            value = Func_0800231c((frame << 9));
            if (frame <= 3) {
                ((RectangleBlit)blitters[0])(canvas, work, *(u8 *)(0x080eedd4 + ((*(s32 *)(*(s32 *)((work + 0x7828)) + 4) << 3) - *(s32 *)(*(s32 *)((work + 0x7828)) + 4))), (((*(s16 *)(source_screen + 6) + ((value << 2) >> 16)) + 16) + *(u8 *)0x080eede2), 57, 98);
                goto L_080e640e;
            }
            if (frame <= 7) {
                ((RectangleBlit)blitters[0])(canvas, work, *(u8 *)(0x080eedd4 + ((*(s32 *)(*(s32 *)((work + 0x7828)) + 4) << 3) - *(s32 *)(*(s32 *)((work + 0x7828)) + 4))), (((*(s16 *)(source_screen + 6) + ((value << 2) >> 16)) + 16) + *(u8 *)0x080eede2), 57, 98);
            }
            base7_80eede2 = 0x80eede2;
            v6 = (work + 0x7828);
            ((RectangleBlit)blitters[0])(canvas, (work + 0x15d2), *(u8 *)(0x080eedd4 + (((*(s32 *)(*(s32 *)((work + 0x7828)) + 4) << 3) - *(s32 *)(*(s32 *)((work + 0x7828)) + 4)) + 1)), (((*(s16 *)(source_screen + 6) + ((value << 2) >> 16)) + 16) + *(u8 *)(base7_80eede2 + 1)), 99, 69);
            if ((u32)(frame - 4) <= 1) {
                ((void (*)(s32, u32, u32))0x03000168)(canvas, 0x4000, 0x3f3f3f3f);
            }
            if ((u32)(frame - 6) <= 1) {
                ((RectangleBlit)blitters[0])(canvas, (work + 0x3081), *(u8 *)(0x080eedd4 + (((*(s32 *)(*(s32 *)((work + 0x7828)) + 4) << 3) - *(s32 *)(*(s32 *)((work + 0x7828)) + 4)) + 2)), (((*(s16 *)(source_screen + 6) + ((value << 2) >> 16)) + 16) + *(u8 *)(base7_80eede2 + 2)), 128, 91);
            }
            if ((u32)(frame - 8) <= 1) {
                ((RectangleBlit)blitters[0])(canvas, 0x2010000, *(u8 *)(0x080eedd4 + (((*(s32 *)(*(s32 *)((work + 0x7828)) + 4) << 3) - *(s32 *)(*(s32 *)((work + 0x7828)) + 4)) + 3)), (((*(s16 *)(source_screen + 6) + ((value << 2) >> 16)) + 16) + *(u8 *)(base7_80eede2 + 3)), 128, 91);
            }
            if ((u32)(frame - 10) <= 1) {
                ((RectangleBlit)blitters[0])(canvas, 0x2012d80, *(u8 *)(0x080eedd4 + (((*(s32 *)(*(s32 *)((work + 0x7828)) + 4) << 3) - *(s32 *)(*(s32 *)((work + 0x7828)) + 4)) + 4)), (((*(s16 *)(source_screen + 6) + ((value << 2) >> 16)) + 16) + *(u8 *)(base7_80eede2 + 4)), 128, 59);
            }
            if ((u32)(frame - 12) <= 1) {
                ((RectangleBlit)blitters[0])(canvas, 0x2014b00, *(u8 *)(0x080eedd4 + (((*(s32 *)(*(s32 *)((work + 0x7828)) + 4) << 3) - *(s32 *)(*(s32 *)((work + 0x7828)) + 4)) + 5)), (((*(s16 *)(source_screen + 6) + ((value << 2) >> 16)) + 16) + *(u8 *)(base7_80eede2 + 5)), 122, 29);
            }
            if ((u32)(frame - 14) > 1) {
                goto L_080e640e;
            }
            ((RectangleBlit)blitters[0])(canvas, 0x20158d2, *(u8 *)(0x080eedd4 + (((*(s32 *)(*(s32 *)((work + 0x7828)) + 4) << 3) - *(s32 *)(*(s32 *)((work + 0x7828)) + 4)) + 6)), (((*(s16 *)(source_screen + 6) + ((value << 2) >> 16)) + 16) + *(u8 *)(base7_80eede2 + 6)), 76, 25);
            goto L_080e640e;
        }
        if (kind != 32) {
        } else {
            scroll_pos = (scroll_pos + scroll_speed);
            if (frame > 6) {
                scroll_speed = ((((scroll_speed << 1) + scroll_speed) << 4) / 64);
            }
            *(volatile s32 *)0x04000028 = ((scroll_pos >> 16) << 8);
            if ((u32)(frame - 16) <= 15) {
                *(volatile u16 *)0x04000052 = ((0x10 - (frame - 16)) | 0x1000);
            }
            if ((u32)(frame - 4) <= 1) {
                ((void (*)(s32, u32, u32))0x03000168)(canvas, 0x4000, 0x3f3f3f3f);
            }
            if (frame <= 3) {
                if (*(s32 *)(*(s32 *)((work + 0x7828)) + 4) == 1) {
                    ((RectangleBlit)blitters[0])(canvas, work, 0, 24, 80, 104);
                } else {
                    ((RectangleBlit)blitters[0])(canvas, work, 48, 24, 80, 104);
                }
                goto L_080e640e;
            }
            if (frame <= 7) {
                if (*(s32 *)(*(s32 *)((work + 0x7828)) + 4) == 1) {
                    ((RectangleBlit)blitters[0])(canvas, work, 0, 24, 80, 104);
                } else {
                    ((RectangleBlit)blitters[0])(canvas, work, 48, 24, 80, 104);
                }
            }
            if (*(s32 *)(*(s32 *)((work + 0x7828)) + 4) == 1) {
                ((RectangleBlit)blitters[0])(canvas, (work + 0x1e00), 16, 16, 80, 104);
            } else {
                ((RectangleBlit)blitters[0])(canvas, (work + 0x1e00), 32, 16, 80, 104);
            }
            if ((u32)(frame - 6) <= 1) {
                ((RectangleBlit)blitters[0])(canvas, (work + 0x3e80), 0, 16, 128, 91);
            }
            if ((u32)(frame - 8) <= 1) {
                ((RectangleBlit)blitters[0])(canvas, 0x2010000, 0, 16, 128, 91);
            }
            if ((u32)(frame - 10) <= 1) {
                ((RectangleBlit)blitters[0])(canvas, 0x2012d80, 0, 16, 128, 59);
            }
            if ((u32)(frame - 12) <= 1) {
                ((RectangleBlit)blitters[0])(canvas, 0x2014b00, 0, 16, 128, 29);
            }
            if ((u32)(frame - 14) > 1) {
                goto L_080e640e;
            }
            ((RectangleBlit)blitters[0])(canvas, 0x2015980, 0, 16, 128, 26);
            goto L_080e640e;
        }
        if (kind == 20) {
            none = 0;
            v10 = none;
            do {
                p5 = v10;
                if (frame >= (v10 + 6)) {
                    if (frame < ((v10 + 6) + 12)) {
                        if ((1 & (s32)p5) != 0) {
                            v6 = ((s32)((((s32)(*(s32 *)(source_screen)) / 2)) - (s32)((u32)*(u8 *)(0x080ede9f + (s32)(((s32)((frame - (s32)p5) - 6) / 2))) >> 1)) + (s32)((s32)((s32)(((s32)((s32)p5 + 1) / 2)) << 1) + (s32)(((s32)((s32)p5 + 1) / 2))));
                        } else {
                            v6 = ((s32)((((s32)(*(s32 *)(source_screen)) / 2)) - (s32)((u32)*(u8 *)(0x080ede9f + (s32)(((s32)((frame - (s32)p5) - 6) / 2))) >> 1)) - (s32)((s32)((s32)(((s32)((s32)p5 + 1) / 2)) << 1) + (s32)(((s32)((s32)p5 + 1) / 2))));
                        }
                        v0 = 1;
                        if ((s32)p5 != 0) {
                            v0 = 0;
                            if (((s32)((s32)p5 - 1) & 3) > 1) {
                                v0 = 1;
                            }
                        }
                        p4 = *(s32 *)((v0 << 2) + draw_pair);
                        ((RectangleBlit)p4)(canvas, (*(u16 *)(0x080edeb2 + (s32)((s32)(((s32)((frame - (s32)p5) - 6) / 2)) << 1)) + 0x2010000), v6, (*(u8 *)(0x080edeab + (s32)(((s32)((frame - (s32)p5) - 6) / 2))) + 48), *(u8 *)(0x080ede9f + (s32)(((s32)((frame - (s32)p5) - 6) / 2))), *(u8 *)(0x080edea5 + (s32)(((s32)((frame - (s32)p5) - 6) / 2))));
                    } else {
                    }
                }
                p10c = (s32)p5 + 1;
                v10 = p10c;
            } while (p10c != 12);
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
                    s32 *screen = (s32 *)projected;
                    s32 *origin = (s32 *)source_screen;
                    s32 n;

                    for (n = 0; n != 64; n++, step++) {
                        if (step->x >= 0 && frame >= n / 2) {
                            Func_080049ac();
                            Func_08004bd4(step->velocity_x);
                            Func_08004c1c(step->velocity_y);
                            Func_080e3944((s32)step, screen);
                            screen[0] = screen[0] / 2 + origin[0] / 2;
                            screen[1] += origin[1] + 32;
                            ((RectangleBlit)blitters[1])(canvas,
                                0x02010000 + ((u16 *)0x080eedea)[n & 3],
                                screen[0] - 4, screen[1] - 4, 8, 8);
                            step->x -= 6;
                            if (step->x < 0 && ((n & 7) == 0 || n == 63)) {
                                Func_080f9010(133);
                                Func_080d6888(*(s16 *)(*(s32 *)(work + 0x7828) + 36),
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
                ((RectangleBlit)blitters[0])(canvas, 0x2010000, 48, (104 - v1), 32, v1);
            } else {
                if ((u32)(kind - 33) <= 1) {
                    if (frame > 5) {
                        goto L_080e640e;
                    }
                    if (*(s32 *)(*(s32 *)((work + 0x7828)) + 4) == 0) {
                        v1 = ((((s32)(*(s32 *)(source_screen)) / 2)) + ((((6 - frame) << 1) + (6 - frame)) << 1));
                    } else {
                        v1 = ((((s32)(*(s32 *)(source_screen)) / 2)) - ((((6 - frame) << 1) + (6 - frame)) << 1));
                    }
                    p4 = *(s32 *)(draw_pair + 4);
                    ((RectangleBlit)p4)(canvas, 0x2010000, (v1 - 16), (((*(s32 *)(source_screen + 4) - ((((6 - frame) << 1) + (6 - frame)) << 2)) + 24) - 32), 32, 64);
                } else {
                    if (kind == 12) {
                        if (frame > 47) {
                            *(volatile u16 *)0x04000052 = ((0x40 - frame) | 0x1000);
                        }
                        {
                            struct EffectStep *step = (struct EffectStep *)0x02014000;
                            s32 *screen = (s32 *)projected;
                            s32 n;

                            for (n = 0; n != 16; n++, step++) {
                                s32 image = Func_080022fc(n, 3);

                                Func_080e3944((s32)step, screen);
                                screen[0] /= 2;
                                ((RectangleBlit)blitters[n & 1])(canvas,
                                    0x02010000 + image * 576,
                                    screen[0] - 12, screen[1] - 12, 24, 24);
                                Func_080e38b8(step, 60, 1 << ((n & 3) + 11));
                                step->variant++;
                            }
                        }
                    } else {
                        if (kind != 100) {
                            struct EffectStep *step = (struct EffectStep *)0x02014000;
                            s32 *screen = (s32 *)projected;
                            s32 n;

                            for (n = 0; n != 16; n++, step++) {
                                if (frame >= n + 4) {
                                    s32 age = step->variant;

                                    if (age <= 23) {
                                        s32 image = age / 4;

                                        Func_080e3944((s32)step, screen);
                                        screen[0] /= 2;
                                        ((RectangleBlit)blitters[n & 1])(canvas,
                                            0x02010000 + image * 1152,
                                            screen[0] - 12, screen[1] - 24, 24, 48);
                                        if (kind == 25)
                                            Func_080e38b8(step, 60, 0x400);
                                        else
                                            Func_080e38b8(step, 60, 0x1000);
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
            Func_080e3944(position, projected);
            v2 = (((s32)(*(s32 *)(projected)) / 2));
            *(s32 *)(projected) = (((s32)(*(s32 *)(projected)) / 2));
            p4 = *(s32 *)(draw_pair + 4);
            ((RectangleBlit)p4)(canvas, 0x2013c56, (v2 - 10), (*(s32 *)(projected + 4) - 4), 20, 40);
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
        Func_080b5088(*(s16 *)(*(s32 *)((work + 0x7828)) + 36), 4);
        goto L_080e650c;
        L_080e64c2:;
        switch (kind) {
        case 14:
        case 20:
        case 33:
            Func_080b5088(*(s16 *)(*(s32 *)((work + 0x7828)) + 36), 1);
            v3 = 2;
            goto L_080e650e;
            break;
        }
        if (kind != 30) {
            if (kind != 8) {
                goto L_080e6510;
            }
        }
        Func_080b5088(*(s16 *)(*(s32 *)((work + 0x7828)) + 36), 3);
        L_080e650c:;
        v3 = 8;
        L_080e650e:;
        *(s32 *)((work + 0x77a8)) = v3;
        L_080e6510:;
        if (frame == 6) {
            Func_080d6888(*(s16 *)(*(s32 *)((work + 0x7828)) + 36), 7, 5, 0, 4);
        }
    }
    if (frame == 14) {
        Func_080d6888(*(s16 *)(*(s32 *)((work + 0x7828)) + 36), 7, 5, 0, 4);
    }
    Func_080e155c(8, 8);
    Func_080cd52c();
    *(s32 *)((work + 0x7824)) = 1;
    Func_080030f8(1);
    frame = (frame + 1);
    if (frame != duration) {
        goto L_080e52f8;
    }
    L_080e657c:;
    if (kind == 21) {
        ((void (*)(s32, u32))0x03000164)(0x6004000, 0x4000);
        ((void (*)(s32, u32))0x03000164)(canvas, 0x4000);
        *(s32 *)(*(s32 *)((work + 0x7828)) + 28) = 0;
        Func_08004278(0x80cd4b5);
        Func_08004278(0x80cd261);
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
        Func_08004278(0x80dbb9d);
        L_080e65e2:;
        Func_08004278(0x80cd261);
        Func_08002dd8(47);
        Func_08002dd8(46);
        Func_080cdbc0();
    }
}
