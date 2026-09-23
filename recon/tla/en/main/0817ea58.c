#include "TYPES.H"
typedef volatile u16 vu16;
typedef volatile u32 vu32;

typedef struct Particle {
    s32 x;
    s32 y;
    s32 z;
    s32 vx;
    s32 vy;
    s32 ay;
    s32 timer;
} Particle;

typedef struct Vec3 {
    s32 x;
    s32 y;
    s32 z;
    s32 w;
} Vec3;

typedef struct Vec2 {
    s32 x;
    s32 y;
} Vec2;

typedef struct Sprite {
    u8 unk_00[8];
    u16 tile : 10;
    u16 priority : 2;
    u16 palette : 4;
    u8 unk_0a[6];
    u8 vram;
    u8 unk_11;
    u16 rotation;
    u8 unk_14[2];
    u8 unk_16;
    u8 unk_17;
} Sprite;

typedef struct VramSlot {
    u16 unk_0;
    u16 offset;
} VramSlot;

typedef struct Bitmap {
    u32 width : 8;
    u32 height : 8;
    u32 unk_2 : 16;
    void *data;
} Bitmap;

struct SummonWork {
    Particle particles[48];
    u8 unk_0540[0x1c0];
    u8 frames[0x7080];
    s32 transfer_mode;
    s32 transfer_value;
    u8 unk_7788[8];
    s32 unk_7790;
    s32 unk_7794;
    s32 unk_7798;
    s32 unk_779c;
    u8 unk_77a0[8];
    s32 shake;
    u8 unk_77ac[0x30];
    Sprite *sprites[57];
    u8 unk_78c0[0x28];
    s32 transfer_pending;
};

struct SummonArgument {
    s32 kind;
    s32 side;
    s32 actor;
    s32 unk_0c;
    s32 unk_10;
    s32 count;
    s32 variant;
    s32 unk_1c;
    s32 unk_20;
    s16 actors[8];
};

struct Keys {
    u32 held;
    u32 unk_4;
    u32 unk_8;
    u32 pressed;
};

typedef void (*CopyFunc)(void *, const void *, s32);
typedef void (*ClearFunc)(void *, s32);
typedef void (*BlitFunc)(void *, const void *, s32, s32, s32, s32);

struct SystemWork {
    u8 unk_00[0x24];
    s32 *bank;
    u8 unk_28[0x34];
    struct SummonWork *summon;
    u8 *canvas;
    u8 *map;
    void *blit;
    u8 unk_6c[0x44];
    s32 *control;
};

#define SYSTEM ((struct SystemWork *)0x03000000)
#define Iwram_Copy ((CopyFunc)0x03000730)
#define Iwram_Clear ((ClearFunc)0x03000258)
#define VRAM_SLOTS ((VramSlot *)0x020036e0)
#define DECODE_BUFFER ((u8 *)0x02010000)
#define PARTICLES ((Particle *)0x02014000)
#define KEYS ((struct Keys *)0x03001150)

extern const u16 Ulysses_MapOffsets[];
extern const Vec2 Ulysses_ScaleA;
extern const Vec2 Ulysses_ScaleB;
extern const Vec2 Ulysses_ScaleC;
extern const s8 Ulysses_Shake[];
extern const u8 Ulysses_FishFrames[];
extern const u8 Ulysses_FishStart[][2];
extern const s32 Ulysses_FishVelocity[][2];
extern const u8 Ulysses_FishDelay[];
extern const u16 Ulysses_WaveCueFrames[];
extern const u16 Ulysses_SprayFrames[];
extern const s8 Ulysses_SprayOffsets[];
extern const u8 Ulysses_ShipParts[][2];
extern const u8 Ulysses_Foam[][6][2];
extern const u16 Ulysses_DropFrames[];
extern const u8 Ulysses_BeamPalette[];
extern const u8 Ulysses_BeamSprite[];

void Func_081435e0(s32);
void Func_0813ba50(void);
void Func_08179e6c(void);
s32 Func_081963ec(s32, s32);
void Func_080145a8(void *, s32);
void Func_08014274(s32);
void Func_08118010(void *, s32);
void Func_08118040(s32, s32, s32);
void Func_08157cf4(s32, void *, s32, s32);
u32 Resource_GetTableEntry(u32);
void Func_0801587c(const void *, void *);
Sprite *Func_0815b290(s32, s32, u32, s32);
Sprite *Func_0815b3b0(s32, s32, u32, s32);
void Func_08020010(Sprite *, Vec3 *, Vec2 *, s32);
void Func_08013560(s32);
void Func_0801314c(s32);
void Func_08144aac(s32, void **);
u32 Func_08014878(void);
s32 Func_08002090(s32);
s32 Func_08002096(s32);
void Func_081c0010(s32);
void Func_0815f0a0(s32);
void Func_08138086(Particle *, s32, s32);
void Func_08138058(Particle *, s32, s32);
void *Func_08014dac(s32);
s32 *Func_081969f8(s32);
void Func_08014de4(void);
void Func_08015160(s32, s32, s32);
void Func_080150e4(s32);
void Func_08015024(s32);
void Func_0801521c(s32);
void Func_08196958(const void *, void *, s32);
void Func_08196a7c(s32 *);
void Func_08013164(void *);
void Func_081434f8(void);
void Func_08014644(void *);
void Func_0814cca8(void);
void Func_08020048(Sprite *);
s32 Func_080143e0(s32);
void Func_08014c4c(void);
void Func_08152404(s32, s32, s32);
void Func_0815f000(s32, s32, s32, s32, s32, s32);
void Func_0814cd48(s32, s32, s32, s32, s32);
void Func_08158ce0(s32, s32);
void Func_08143bb8(void);
void Func_08143000(void);
void Func_0813baec(void);

/* The admitted DMA primitive, shared from TBS until it moves to games/COMMON;
 * TLA's TYPES.H already declares the same fixed-width types. */
#define ALCHEMY_TYPES_H
#include "../../../../games/THE BROKEN SEAL/INCLUDE/DMA.H"

void BattleSummon_RunUlysses(struct SummonArgument *arg)
{
    s16 targets[14];
    Vec3 pos;
    Vec3 pos2;
    Vec3 pos3;
    Vec2 scale;
    void *blit[2];
    Vec2 scale2;
    Bitmap bitmap;
    Vec2 scale3;
    u8 *dest;
    s32 frame;
    u8 *map;
    s32 *ctl;
    s32 *bank;
    s32 camera;
    s32 prev_camera;
    s32 wave_x;
    s32 wave_scroll;
    s32 ship_y;
    s32 wave_phase;
    u16 saved_y;
    s32 bg_x;
    struct SummonWork *work;
    u16 *pal;
    s32 sway_x;
    s32 sway_y;
    s32 bob_x;
    s32 bob_y;
    s32 i;
    s32 j;
    s32 k;
    s32 row;
    s32 z;
    s32 v;
    s32 r;
    s32 g;
    s32 b;
    u16 color;
    const u8 *src;
    Sprite *obj;
    Sprite **objp;
    Particle *p;
    Particle *q;

    dest = SYSTEM->canvas;
    map = SYSTEM->map;
    work = SYSTEM->summon;
    ctl = SYSTEM->control;
    bank = SYSTEM->bank;
    Func_081435e0(0);
    Func_0813ba50();
    Func_08179e6c();
    Func_081963ec(104, 3);
    blit[0] = SYSTEM->blit;
    work->transfer_mode = 0;
    Func_080145a8(Func_08143000, 0xc80);
    camera = 0;
    wave_scroll = 0;
    ship_y = -0x1d0000;
    wave_phase = 0;
    saved_y = *(u16 *)0x03001124;
    bg_x = -80;

    for (frame = 0; frame != 52; frame++) {
        if (frame == 27) {
            Func_08014274(bank[21]);
            *(s32 *)0x030011f0 = 240;
            targets[0] = 255;
            Func_08118010(targets, 0);
            Func_08118040(1, 0x45, 0);
            for (i = 0, pal = (u16 *)0x050000c0; i != 128; i++, pal++) {
                color = *pal;
                r = (color & 0x1f) - 4;
                g = ((color >> 5) & 0x1f) - 4;
                b = ((color >> 10) & 0x1f) - 4;
                if (g < 0)
                    g = 0;
                if (r < 0)
                    r = 0;
                if (b < 0)
                    b = 0;
                if (g > 31)
                    g = 31;
                if (r > 31)
                    r = 31;
                if (b > 31)
                    b = 31;
                *pal = (b << 10) | (r << 5) | g;
            }
            work->unk_7790 = 0;
            work->unk_7794 = 5;
            work->unk_7798 = -1;
            work->unk_779c = 0;
            Func_080145a8(Func_0813baec, 0x480);
            ctl[4] = 1;
            Func_08157cf4(0x134, work->frames, 0, 0);
            for (i = 0; i != 10; i++) {
                for (j = 0; j != 32; j++) {
                    v = work->frames[Ulysses_MapOffsets[3] + j];
                    if (v != 0) {
                        v = v - i * 4 - 40;
                        if (v <= 0)
                            v = 1;
                    }
                    map[i * 32 + j] = v;
                }
            }
            src = (const u8 *)Resource_GetTableEntry(0xa0);
            Iwram_Copy((void *)0x050003e0, src, 32);
            Func_0801587c(src + 32, work->frames);
            for (row = 0; row != 2; row++) {
                src = work->frames + (row << 12);
                for (j = 0; j != 8; j++) {
                    obj = Func_0815b290(32, 32, 0x80000000, 0xf000);
                    work->sprites[row * 9 + j] = obj;
                    obj->priority = 1;
                    Iwram_Copy((void *)(VRAM_SLOTS[obj->vram].offset + 0x06010000), src, 0x200);
                    src += 0x200;
                }
                obj = Func_0815b3b0(32, 32, 0x80000000, 0xf000);
                work->sprites[row * 9 + 8] = obj;
                Iwram_Copy(obj, work->sprites[row * 9], 24);
                for (j = 0; j != 8; j++)
                    work->particles[row * 9 + j].timer = work->sprites[row * 9 + j]->tile;
            }
            src = (const u8 *)Resource_GetTableEntry(0xa1);
            Iwram_Copy((void *)0x05000200, src, 0x1c0);
            Func_0801587c(src + 0x1c0, DECODE_BUFFER);
            for (i = 0; i != 21; i++) {
                obj = Func_0815b290(32, 32, 0x80002000, 0);
                obj->priority = 3;
                work->sprites[18 + i] = obj;
                Iwram_Copy((void *)(VRAM_SLOTS[obj->vram].offset + 0x06010000), DECODE_BUFFER + i * 0x400, 0x400);
            }
            obj = Func_0815b290(32, 64, 0xc000a000, 0);
            work->sprites[39] = obj;
            obj->priority = 3;
            Iwram_Copy((void *)(VRAM_SLOTS[obj->vram].offset + 0x06010000), DECODE_BUFFER + i * 0x400, 0x800);
            src = (const u8 *)Resource_GetTableEntry(0xa2);
            Iwram_Copy((void *)0x050003c0, src, 32);
            Func_0801587c(src + 32, DECODE_BUFFER);
            obj = Func_0815b290(16, 8, 0x4000, 0xe000);
            obj->priority = 1;
            work->sprites[41] = obj;
            Iwram_Copy((void *)(VRAM_SLOTS[obj->vram].offset + 0x06010000), DECODE_BUFFER, 0x100);
            for (i = 0; i != 15; i++) {
                work->sprites[42 + i] = Func_0815b3b0(16, 8, 0x4000, 0xf000);
                Iwram_Copy(work->sprites[42 + i], work->sprites[41], 24);
                work->sprites[42 + i]->tile = work->sprites[41]->tile;
                if (i > 6)
                    work->sprites[42 + i]->priority = 2;
            }
        }
        if (frame > 27) {
            scale = Ulysses_ScaleA;
            pos.w = 0;
            pos.y = 0xff0000;
            wave_scroll += 0x8000;
            wave_x = wave_scroll + camera;
            if (wave_x >= 0x200000) {
                wave_phase = (wave_phase + 7) % 8;
                wave_x -= 0x200000;
                wave_scroll -= 0x200000;
            }
            z = 0x670000;
            for (row = 0; row != 2; row++) {
                for (k = 0; k != 9; k++) {
                    pos.x = (k << 21) + wave_x;
                    pos.z = z;
                    if (k == 8) {
                        obj = work->sprites[row * 9 + 8];
                        v = (wave_phase + 71) % 8;
                    } else {
                        obj = work->sprites[row * 9 + k];
                        v = (wave_phase + 64 + k) % 8;
                    }
                    obj->tile = work->particles[row * 9 + v].timer;
                    if (pos.x > 0xffffff)
                        pos.x -= 0x1200000;
                    Func_08020010(work->sprites[row * 9 + k], &pos, &scale, 0);
                }
                z += 0x200000;
            }
        }
        bg_x += 4;
        *(s32 *)0x04000028 = bg_x << 8;
        work->transfer_pending = 1;
        Func_08013560(1);
    }

    Func_0801314c(104);
    Iwram_Clear(dest, 0x3c00);
    Iwram_Clear((void *)0x06004000, 0x4000);
    *(vu16 *)0x0400000c = 0x786;
    *(vu16 *)0x04000020 = 0x80;
    *(s32 *)0x04000028 = 0;
    Func_08144aac(0, blit);
    work->unk_7790 = 0;
    work->unk_7794 = 5;
    work->unk_7798 = -1;
    work->unk_779c = 0;
    ctl[4] = 1;
    Func_08157cf4(0xba, work->frames, 0, 0);
    Func_08157cf4(0x13e, work->frames + 0x6c0, 1, 0);
    Func_08157cf4(0xbb, work->frames + 0x36c0, 0, 0);
    Func_08157cf4(0xc2, DECODE_BUFFER, 0, 0);
    Iwram_Copy((void *)0x05000000, (const void *)Resource_GetTableEntry(0x161), 128);
    work->transfer_mode = 2;
    work->transfer_value = 50;
    *(vu16 *)0x04000052 = 0x1010;
    *(vu16 *)0x04000050 = 0x3f44;
    for (i = 0; i < 224; i++) {
        pal = (u16 *)0x05000200 + i;
        if (((*pal >> 5) & 0x1f) == 31)
            break;
    }
    for (i = 0; i != 512; i++)
        PARTICLES[i].timer = -1;

    for (frame = 0; frame != 800 && !(KEYS->held & 2) && !(KEYS->pressed & 1); frame++) {
        if (frame == 0) {
            *(vu16 *)0x04000020 = 0x100;
            *pal = 0x190b;
            ship_y = -0x1d0000;
            camera = 0;
            work->sprites[39]->rotation = 0xc000;
            work->sprites[38]->rotation = 0;
            work->sprites[31]->rotation = 0;
            work->particles[20].x = 0;
            work->particles[20].y = 0;
            work->particles[20].vx = -0x60000;
            work->particles[20].vy = 0;
            work->particles[20].timer = 0;
            work->particles[23].x = 0;
            work->particles[23].y = 0;
            work->particles[23].vx = -0xc0000;
            work->particles[23].vy = 0;
            work->particles[23].timer = 0;
            work->particles[21].x = 0;
            work->particles[21].y = 0;
            work->particles[21].vx = -0x10000;
            work->particles[21].vy = 0;
            work->particles[21].timer = 0;
            work->particles[22].x = 0xa00000;
            work->particles[22].y = 0;
            work->particles[22].vx = 0;
            work->particles[22].vy = 0;
            work->particles[22].timer = 0;
            for (i = 0; i != 8; i++) {
                p = &work->particles[32 + i];
                p->x = ((i / 3) << 18) + 0xb00000;
                p->y = ((i % 3) * 3 << 17) + 0x4a0000;
                p->vx = 0;
                p->vy = 0;
                p->timer = 0;
            }
            for (i = 0; i != 8; i++) {
                p = &work->particles[40 + i];
                p->x = ((i / 3) << 18) + 0x700000;
                p->y = ((i % 3) * 3 << 17) + 0x520000;
                p->vx = 0;
                p->vy = 0;
                p->timer = 0;
            }
        }
        prev_camera = camera;
        if (frame >= 542 && frame < 558) {
            color = *pal;
            r = color & 0x1f;
            g = (color >> 5) & 0x1f;
            b = (color >> 10) & 0x1f;
            if (r < 31)
                r++;
            if (g < 20)
                g++;
            if (b < 6)
                b++;
            *pal = (b << 10) | (g << 5) | r;
        }
        if (frame > 557) {
            color = *pal;
            r = color & 0x1f;
            g = (color >> 5) & 0x1f;
            b = (color >> 10) & 0x1f;
            if (r > 20)
                r--;
            if (g > 0)
                g--;
            if (b > 0)
                b--;
            *pal = (b << 10) | (g << 5) | r;
        }
        if (frame == 280) {
            Iwram_Copy((void *)0x05000000, (const void *)Resource_GetTableEntry(0x161), 128);
            work->particles[20].timer = 1;
        }
        if (frame == 288)
            work->particles[23].timer = 1;
        if (frame == 279) {
            *(vu16 *)0x04000020 = 0x80;
            *(vu16 *)0x0400000c = 0x784;
            *(vu16 *)0x04000020 = 0x80;
        }
        if (frame == 506)
            work->particles[21].timer = 1;
        if (frame == 489) {
            *(vu16 *)0x04000020 = 0x80;
            *(vu16 *)0x0400000c = 0x784;
            work->unk_7798 = 0;
        }
        if (frame == 528) {
            *(vu16 *)0x04000020 = 0x80;
            *(vu16 *)0x0400000c = 0x785;
        }
        if (frame == 0) {
            work->transfer_mode = 1;
            work->transfer_value = 0;
            work->transfer_mode = 2;
            work->transfer_value = 50;
            for (i = 0; i != 10; i++) {
                p = &PARTICLES[502 + i];
                p->x = Ulysses_FishStart[i][0] << 16;
                p->y = Ulysses_FishStart[i][1] << 16;
                p->vx = Ulysses_FishVelocity[i][0];
                p->vy = Ulysses_FishVelocity[i][1];
                p->timer = Func_08014878() & 127;
            }
        }
        for (i = 0; i != 10; i++) {
            p = &PARTICLES[502 + i];
            if (frame >= Ulysses_FishDelay[i]) {
                ((BlitFunc)blit[0])(dest, work->frames + 0x36c0 + (Ulysses_FishFrames[(p->timer / 4) % 12] << 8),
                    p->x >> 16, p->y >> 16, 16, 16);
                p->x += p->vx;
                p->y += p->vy;
                p->timer++;
            }
        }
        if (frame <= 473)
            wave_scroll += 0x8000;
        wave_x = wave_scroll + camera;
        if (wave_x >= 0x200000) {
            wave_phase = (wave_phase + 7) % 8;
            wave_scroll -= 0x200000;
            wave_x -= 0x200000;
        }
        if (ship_y < 0)
            ship_y += 0x2000;
        scale2 = Ulysses_ScaleB;
        bob_x = 0;
        bob_y = 0;
        if (frame > 537) {
            sway_x = Func_08002090(0x21a00) << 2;
            sway_y = Func_08002096(0x21a00) << 2;
            if (frame >= 542 && frame < 558) {
                v = (frame << 11) - 0x10f000;
                bob_x = Func_08002096(v) * 3;
                bob_y = Func_08002096(v) * 2;
            }
            if (frame >= 560 && frame < 576) {
                v = (frame << 11) - 0x118000;
                bob_x = Func_08002096(v) * 2;
                bob_y = Func_08002096(v) * 3;
            }
        } else {
            sway_x = Func_08002090(frame << 8) << 2;
            sway_y = Func_08002096(frame << 8) << 2;
        }
        if (sway_y > 0)
            sway_y = -sway_y;
        sway_y -= ship_y * 2;
        pos2.w = 0;
        pos2.y = 0xff0000;
        for (i = 0; i != 5; i++) {
            if (frame == Ulysses_WaveCueFrames[i]) {
                work->shake = 23;
                Func_081c0010(144);
            }
        }
        if (frame == 510)
            Func_081c0010(186);
        if (frame == 568) {
            work->shake = 10;
            Func_081c0010(145);
        }
        if (frame == 578)
            Func_081c0010(163);
        for (i = 0; i != 16; i++) {
            p = &work->particles[32 + i];
            if (p->timer != 0) {
                p->timer++;
                if (p->x > -0x100000) {
                    p->x += p->vx;
                    p->y += p->vy;
                    if (p->vy > -0x10000 && p->vy < 0x10000)
                        p->vy += p->ay;
                }
                if (p->vx > -0x80000)
                    p->vx -= 0x8000;
                q = &PARTICLES[i * 32 + (p->timer & 31)];
                q->timer = 0;
                q->x = p->x / 2;
                q->vx = 0;
                q->y = p->y - 0x140000;
                q->vy = 0;
            }
            if (work->particles[20].timer == (i & 7) * 6 + 10) {
                Func_081c0010(138);
                p->timer = 1;
                p->x += sway_x;
                p->y += sway_y - (Ulysses_Shake[work->shake] << 16);
                p->vy = (Func_08014878() & 0xffff) - 0x8000;
                p->ay = (Func_08014878() & 0x3fff) - 0x2000;
            }
        }
        if (frame <= 489) {
            for (i = 8; i != 16; i++) {
                p = &work->particles[32 + i];
                pos2.x = p->x + camera;
                pos2.z = p->y;
                if (p->timer != 0)
                    Func_08020010(work->sprites[40 + i], &pos2, &scale2, 0);
            }
        }
        if (frame == 578) {
            work->unk_7794 = 1;
            work->unk_7790 = 0;
        }
        if (frame >= 578 && frame < 594)
            work->unk_7798 = ~((frame - 578) / 8);
        v = camera - sway_x + bob_x + 0x780000;
        for (i = 0; i != 2; i++) {
            pos2.x = v;
            pos2.z = (((i << 5) - Ulysses_Shake[work->shake]) << 16) + sway_y + bob_y + 0x640000;
            Func_08020010(work->sprites[32 + i], &pos2, &scale2, 0);
        }
        if (frame <= 316) {
            p = &work->particles[23];
            if (p->timer != 0) {
                p->timer++;
                p->x += p->vx;
                p->y += p->vy;
                p->vy += 0x2000;
                p->vx = p->vx * 48 / 64;
                p->vy = p->vy * 63 / 64;
                work->sprites[38]->rotation += 0x200;
            }
            pos2.x = p->x - sway_x + camera + 0x780000;
            pos2.z = ((99 - Ulysses_Shake[work->shake]) << 16) + sway_y + p->y;
            Func_08020010(work->sprites[38], &pos2, &scale2, 0);
        }
        v = camera + bob_x;
        pos2.x = v + 0x8e0000;
        pos2.z = ((72 - Ulysses_Shake[work->shake]) << 16) + sway_y + bob_y;
        Func_08020010(work->sprites[18], &pos2, &scale2, 0);
        pos2.x = v + 0xae0000;
        pos2.z = ((72 - Ulysses_Shake[work->shake]) << 16) + sway_y + bob_y;
        Func_08020010(work->sprites[19], &pos2, &scale2, 0);
        v += 0x800000;
        pos2.x = v;
        pos2.z = ((104 - Ulysses_Shake[work->shake]) << 16) + sway_y + bob_y;
        Func_08020010(work->sprites[20], &pos2, &scale2, 0);
        if (frame > 602)
            Func_0815f0a0(0x148);
        p = &work->particles[22];
        if (frame == 570) {
            p->x = 0x980000;
            p->y = ((92 - Ulysses_Shake[work->shake]) << 16) + sway_y;
            p->timer = 1;
            p->vx = -0x40000;
            p->vy = -0x5000;
            for (i = 0; i != 16; i++) {
                q = &PARTICLES[312 + i];
                q->x = ((Func_08014878() & 15) + 56) << 16;
                q->y = ((Func_08014878() & 31) + 64) << 16;
                q->vx = -((Func_08014878() & 31) + 8) << 12;
                q->vy = -((Func_08014878() & 31) + 8) << 12;
                q->timer = 0;
            }
            for (i = 0; i != 32; i++)
                PARTICLES[400 + i].timer = -1;
            for (i = 0; i != 32; i++)
                PARTICLES[432 + i].timer = -1;
        }
        if (p->timer != 0) {
            if (p->timer > 9) {
                q = &PARTICLES[400 + (p->timer & 31)];
                q->timer = 0;
                q->vx = -(Func_08002096(work->sprites[39]->rotation) * 2);
                q->vy = Func_08002090(work->sprites[39]->rotation) * 2;
                q->x = (p->x + camera) / 2 - ((Func_08002096(work->sprites[39]->rotation) * 27) >> 1);
                q->y = p->y + Func_08002090(work->sprites[39]->rotation) * 27 - 0xc0000;
            }
            if (((p->timer >= 50 && p->timer < 60) || (p->timer >= 113 && p->timer < 200)) && (p->timer & 3) == 0) {
                q = &PARTICLES[432 + (p->timer & 31)];
                q->timer = 0;
                q->vx = ((Func_08014878() & 31) + 16) << 12;
                q->vy = -(((Func_08014878() - 1) & 15) + 1) << 14;
                q->x = 0x580000;
                q->y = 0x800000;
            }
            prev_camera = camera;
            if (p->timer > 229)
                camera += (0x980000 - (p->x + ((p->timer - 230) << 17)) - camera) / 4;
            else if (p->timer > 0)
                camera += (0x980000 - p->x - camera) / 4;
            if (p->timer > 1) {
                p->x += p->vx;
                p->y += p->vy;
            }
            if (p->timer > 7) {
                p->vx = p->vx * 63 / 64;
                p->vy = p->vy * 63 / 64 + 0x8c8;
            }
            if (p->timer < 80)
                p->vy += 0x200;
            if (p->timer > 29)
                p->vx -= 0x4000;
            if (p->timer >= 40 && p->timer < 68)
                p->vy -= 0x1000;
            if (p->timer >= 50 && p->timer < 60) {
                work->sprites[39]->rotation = (p->timer << 7) - 0x5900;
                p->vy -= 0x1000;
            }
            if (p->timer >= 108 && p->timer < 140) {
                if (p->timer >= 120 && p->timer < 123)
                    work->sprites[39]->rotation -= 0x180;
                else
                    work->sprites[39]->rotation += 0x100;
                p->vy -= 0x1000;
            }
            if (p->timer >= 140 && p->timer < 160)
                p->vy -= 0x800;
            if (p->timer > 159)
                p->vy -= 0x2000;
            for (i = 0; i != 32; i++) {
                q = &PARTICLES[400 + i];
                if ((u32)q->timer <= 5) {
                    ((BlitFunc)blit[1])(dest, work->frames + q->timer * 288, (q->x >> 16) - 2, (q->y >> 16) - 12, 12, 24);
                    Func_08138086(q, 64, 0);
                    q->timer++;
                }
            }
            for (i = 0; i != 32; i++) {
                q = &PARTICLES[432 + i];
                if ((u32)q->timer <= 23) {
                    ((BlitFunc)blit[1])(dest, work->frames + 0x6c0 + (q->timer / 4 << 11), (q->x >> 16) - 16, (q->y >> 16) - 32, 32, 64);
                    q->vx += 0x1000;
                    Func_08138086(q, 64, 0);
                    q->timer++;
                }
            }
            p->timer++;
        }
        scale2.x = 0x103ff;
        scale2.y = 0x103ff;
        if (p->timer == 0) {
            pos2.x = camera + bob_x + 0x980000;
            pos2.z = ((92 - Ulysses_Shake[work->shake]) << 16) + sway_y + bob_y;
        } else {
            pos2.x = p->x + camera;
            pos2.z = p->y;
        }
        Func_08020010(work->sprites[39], &pos2, &scale2, 0);
        work->sprites[39]->unk_16 = 0;
        work->sprites[39]->unk_17 = 32;
        if (frame >= 568) {
            s32 *beam;
            void *buffer;

            buffer = Func_08014dac(512);
            beam = Func_081969f8(1);
            bitmap.width = 7;
            bitmap.height = 7;
            bitmap.data = DECODE_BUFFER;
            beam[4] = (s32)&bitmap;
            beam[1] = 0;
            beam[2] = (s32)Ulysses_BeamSprite;
            beam[0] = 7;
            beam[3] = (s32)buffer;
            v = frame - 568;
            z = 96 - (v << 4);
            if (z > 0)
                z = 0;
            if (z >= -63) {
                beam[5] = z;
                Func_08014de4();
                Func_08015160(camera + 0x20000, 0x80000, 0);
                Func_080150e4(0x4300);
                Func_08015024(-0x800);
                Func_0801521c((v << 14) + 0x4180);
                Func_08196958(Ulysses_BeamPalette, buffer, 4);
                Func_08196a7c(beam);
            }
            Func_08013164(beam);
            Func_08013164(buffer);
        }
        if (frame == 568) {
            work->particles[30].x = 0;
            work->particles[30].vx = 0x80000;
        }
        p = &work->particles[30];
        for (i = 0; i != 1; i++) {
            if (frame >= Ulysses_SprayFrames[i] && frame < Ulysses_SprayFrames[i] + 26) {
                s32 radius;
                s32 n;

                n = (frame - Ulysses_SprayFrames[i]) / 2;
                radius = p->x >> 16;
                p->x += p->vx;
                p->vx = p->vx * 48 / 64;
                if (n <= 5) {
                    for (j = 0; j != 32; j++) {
                        v = (radius * Func_08002096(j << 11) / 2 >> 16) + Ulysses_SprayOffsets[i];
                        ((BlitFunc)blit[0])(dest, work->frames + n * 288, v - 6,
                            (radius * Func_08002090(j << 11) * 2 >> 16) + 60, 12, 24);
                    }
                }
            }
        }
        scale2.x = 0x10000;
        scale2.y = 0x10000;
        work->sprites[21]->priority = 2;
        work->sprites[22]->priority = 2;
        work->sprites[23]->priority = 2;
        work->sprites[24]->priority = 2;
        v = prev_camera + bob_x;
        pos2.x = v + 0xa00000;
        pos2.z = ((104 - Ulysses_Shake[work->shake]) << 16) + sway_y + bob_y;
        Func_08020010(work->sprites[21], &pos2, &scale2, 0);
        pos2.x = v + 0x800000;
        pos2.z = ((136 - Ulysses_Shake[work->shake]) << 16) + sway_y + bob_y;
        Func_08020010(work->sprites[22], &pos2, &scale2, 0);
        pos2.x = v + 0xa00000;
        pos2.z = ((136 - Ulysses_Shake[work->shake]) << 16) + sway_y + bob_y;
        Func_08020010(work->sprites[23], &pos2, &scale2, 0);
        pos2.x = v + 0xc00000;
        pos2.z = ((136 - Ulysses_Shake[work->shake]) << 16) + sway_y + bob_y;
        Func_08020010(work->sprites[24], &pos2, &scale2, 0);
        work->sprites[25]->priority = 2;
        pos2.x = v + 0x800000;
        pos2.z = ((104 - Ulysses_Shake[work->shake]) << 16) + sway_y + bob_y;
        Func_08020010(work->sprites[25], &pos2, &scale2, 0);
        for (i = 0; i != 5; i++) {
            pos2.x = (Ulysses_ShipParts[i][0] << 16) + sway_x + prev_camera + bob_x + 0xb00000;
            pos2.z = ((Ulysses_ShipParts[i][1] - Ulysses_Shake[work->shake]) << 16) + sway_y + bob_y + 0x540000;
            work->sprites[26 + i]->priority = 2;
            Func_08020010(work->sprites[26 + i], &pos2, &scale2, 0);
        }
        if (frame <= 489) {
            for (i = 0; i != 8; i++) {
                p = &work->particles[32 + i];
                pos2.x = p->x + camera;
                pos2.z = p->y;
                if (p->timer != 0)
                    Func_08020010(work->sprites[41 + i], &pos2, &scale2, 0);
            }
        }
        if (frame < 540) {
            p = &work->particles[21];
            if (p->timer == 1) {
                p->x += p->vx;
                p->y += p->vy;
                p->vy += 0x2000;
                p->vx = p->vx * 60 / 64;
                p->vy = p->vy * 63 / 64;
            }
            for (i = 0; i != 4; i++) {
                pos2.x = ((i & 1) << 21) + p->x + camera + 0x800000;
                pos2.z = ((((i / 2) << 5) - Ulysses_Shake[work->shake]) << 16) + sway_y + p->y + 0x680000;
                work->sprites[34 + i]->priority = 1;
                Func_08020010(work->sprites[34 + i], &pos2, &scale2, 0);
            }
        }
        if (frame <= 316) {
            p = &work->particles[20];
            if (p->timer != 0) {
                p->timer++;
                p->x += p->vx;
                p->y += p->vy;
                p->vy += 0x2000;
                p->vx = p->vx * 48 / 64;
                p->vy = p->vy * 63 / 64;
                work->sprites[31]->rotation += 0x100;
            }
            pos2.x = sway_x + p->x + camera + 0xb80000;
            pos2.z = ((92 - Ulysses_Shake[work->shake]) << 16) + sway_y + p->y;
            work->sprites[31]->priority = 1;
            Func_08020010(work->sprites[31], &pos2, &scale2, 0);
        }
        for (row = 0; row != 2; row++) {
            for (k = 0; k != 9; k++) {
                pos2.x = (k << 21) + wave_x;
                pos2.z = (((row << 5) - Ulysses_Shake[work->shake]) << 16) + ship_y + 0x840000;
                if (k == 8) {
                    obj = work->sprites[row * 9 + 8];
                    v = (wave_phase + 71) % 8;
                } else {
                    obj = work->sprites[row * 9 + k];
                    v = (wave_phase + 64 + k) % 8;
                }
                obj->tile = work->particles[row * 9 + v].timer;
                if (pos2.x > 0xffffff)
                    pos2.x -= 0x1200000;
                Func_08020010(work->sprites[row * 9 + k], &pos2, &scale2, 0);
            }
        }
        for (i = 0; i != 4; i++) {
            for (j = 0; j != 6; j++) {
                if (frame >= j * 2 + 490 && frame < j * 2 + 502) {
                    ((BlitFunc)blit[0])(dest, work->frames + (frame - (j * 2 + 490)) / 2 * 288,
                        Ulysses_Foam[i][j][0] - 6,
                        Ulysses_Foam[i][j][1] + (sway_y >> 16) - Ulysses_Shake[work->shake] - 12, 12, 24);
                }
            }
        }
        for (i = 0; i != 16; i++) {
            for (j = 0; j != 32; j++) {
                q = &PARTICLES[i * 32 + j];
                if ((u32)q->timer <= 5) {
                    ((BlitFunc)blit[0])(dest, work->frames + q->timer * 288, (q->x >> 16) - 2, (q->y >> 16) - 12, 12, 24);
                    q->x += q->vx;
                    q->y += q->vy;
                    q->vy -= 0x4000;
                    q->timer++;
                }
            }
        }
        v = Ulysses_Shake[work->shake];
        *(u16 *)0x03001126 = v + 32;
        *(s32 *)0x030011f0 = 120 - v;
        if (work->shake > 0)
            work->shake--;
        Func_081434f8();
        work->transfer_pending = 1;
        Func_08013560(1);
    }

    Func_08014644(Func_0813baec);
    *(u16 *)0x03001124 = saved_y;
    ctl[4] = 0;
    Func_0814cca8();
    for (i = 0; i != 57; i++)
        Func_08020048(work->sprites[i]);
    bank[21] = Func_080143e0(128);
    Func_08014c4c();
    Dma_Set((void *)0x05000200, (void *)0x050000a0, 0x80000010, (vu32 *)0x040000d4);
    *(u16 *)0x050001bc = *(u16 *)0x050001e8;
    work->transfer_mode = 2;
    work->transfer_value = 50;
    *(s32 *)0x04000028 = 0;
    *(vu16 *)0x04000050 = 0x3f44;
    *(vu16 *)0x04000020 = 0x80;
    *(vu16 *)0x0400000c = 0x784;
    *(vu16 *)0x04000052 = 0x1010;
    arg->actors[arg->count] = 0xff;
    Func_08118010(arg->actors, 0);
    Func_08152404(1, 663, 1);
    for (i = 0; i != 15; i++) {
        work->sprites[1 + i] = Func_0815b3b0(32, 16, 0x80006000, 0);
        Iwram_Copy(work->sprites[1 + i], work->sprites[0], 24);
        work->sprites[1 + i]->unk_16 = 32;
        work->sprites[1 + i]->unk_17 = 8;
    }
    Func_08157cf4(0x13e, work->frames, 1, 0);
    Func_08157cf4(0xba, work->frames + 0x3000, 0, 0);
    Iwram_Copy((void *)0x05000000, (const void *)Resource_GetTableEntry(0x148), 128);

    for (frame = 0; frame != 109; frame++) {
        if (frame == 0) {
            z = 0;
            for (i = 0; i != 16; i++) {
                p = &work->particles[i];
                p->timer = 0;
                Func_08014878();
                p->ay = 0;
                p->z = z - p->ay * 40 - 0x800;
                p->vx = -Func_08002090(p->z) * 8;
                p->vy = -Func_08002096(p->z) * 8;
                p->x = ((Func_08014878() % 80 + 80) << 16) - p->vx * 24;
                p->y = (((Func_08014878() & 7) + 96) << 16) - p->vy * 24;
                work->sprites[i]->rotation = p->z;
                work->sprites[i]->unk_16 = 0;
                work->sprites[i]->unk_17 = 8;
                z -= 0x100;
            }
            for (i = 0; i != 512; i++)
                PARTICLES[i].timer = -1;
        }
        scale3 = Ulysses_ScaleC;
        pos3.w = 0;
        pos3.y = 0xff0000;
        for (i = 0; i != 16; i++) {
            p = &work->particles[i];
            if (frame >= Ulysses_DropFrames[i] && p->timer == 0) {
                pos3.x = p->x;
                pos3.z = p->y;
                if (p->y > 0 && p->y < 0x800000)
                    Func_08020010(work->sprites[i], &pos3, &scale3, 0);
                if (p->y > 0x6fffff) {
                    p->timer = 1;
                    for (j = 0; j != 2; j++) {
                        s32 speed;
                        s32 angle;

                        q = &PARTICLES[i * 8 + j];
                        speed = Func_08014878() & 31;
                        angle = (Func_08014878() & 0x7fff) + 0x4000;
                        q->x = p->x / 2;
                        q->y = p->y;
                        speed += 32;
                        q->vx = speed * Func_08002096(angle) >> 4;
                        q->vy = speed * Func_08002090(angle) >> 4;
                        q->timer = 0;
                    }
                    Func_081c0010(144);
                    for (j = 0; j < arg->count; j++) {
                        Func_0815f000(arg->actors[j], 1, 0, 0, 0x20000, 100);
                        Func_0814cd48(arg->actors[j], 7, 5, j, 7);
                        work->shake = 4;
                    }
                } else {
                    q = &PARTICLES[256 + i * 16 + (frame - Ulysses_DropFrames[i]) % 16];
                    q->x = p->x / 2;
                    q->y = p->y - 0x100000;
                    q->vx = 0;
                    q->vy = 0;
                    q->timer = 0;
                }
                work->sprites[i]->rotation = p->z;
                Func_08138058(p, 64, 0);
                p->vx = -Func_08002090(work->sprites[i]->rotation) * 8;
                p->vy = -Func_08002096(work->sprites[i]->rotation) * 8;
                work->sprites[i]->tile = work->sprites[0]->tile;
            }
        }
        for (i = 0; i != 256; i++) {
            q = &PARTICLES[256 + i];
            if ((u32)q->timer <= 7) {
                v = q->timer - 2;
                if (v >= 0)
                    ((BlitFunc)blit[1])(dest, work->frames + 0x3000 + v * 288, (q->x >> 16) - 6, (q->y >> 16) - 12, 12, 24);
                q->timer++;
            }
        }
        for (i = 0; i != 128; i++) {
            q = &PARTICLES[i];
            if ((u32)q->timer <= 17) {
                ((BlitFunc)blit[i & 1])(dest, work->frames + (q->timer / 3 << 11), (q->x >> 16) - 16, (q->y >> 16) - 48, 32, 64);
                Func_08138086(q, 56, -0x2000);
                q->timer++;
            }
        }
        Func_08158ce0(8, 8);
        Func_081434f8();
        work->transfer_pending = 1;
        Func_08013560(1);
    }
    for (i = 0; i != 16; i++)
        Func_08020048(work->sprites[i]);
    Func_08014644(Func_08143000);
    Func_0801314c(188);
    Func_0801314c(104);
    Func_08143bb8();
}
