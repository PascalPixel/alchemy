#include "TYPES.H"

struct Particle {
    s32 x;
    s32 y;
    s32 z;
    s32 vx;
    s32 vy;
    s32 vz;
    s32 timer;
};

struct SpriteObject {
    u16 unk_00;
    u16 unk_02;
    u16 unk_04 : 10;
    u16 oam_priority : 2;
    u16 unk_04_palette : 4;
    u16 unk_06;
    u16 tile : 10;
    u16 priority : 2;
    u16 palette : 4;
    u8 unk_0a[6];
    u8 slot;
    u8 unk_11[6];
    u8 unk_17;
};

struct TileSlot {
    u16 unk_0;
    u16 offset;
};

struct ModelDesc {
    u32 a : 8;
    u32 b : 8;
    u32 c : 16;
    void *tiles;
};

struct Model {
    s32 kind;
    s32 unk_04;
    const void *mesh;
    void *texture;
    struct ModelDesc *desc;
    s32 depth;
};

struct ActorPos {
    u8 unk_00[8];
    s32 x;
    s32 y;
    s32 z;
    u8 unk_14[0x34];
    s32 unk_48;
};

struct SummonWork {
    struct Particle drops[16];
    struct Particle members[16];
    struct Particle homes[16];
    struct Particle columns[12];
    u8 unk_690[0x6c];
    s32 wave;
    u8 buffer[0x7080];
    s32 transfer_mode;
    s32 transfer_value;
    u8 unk_7788[0x20];
    s32 unk_77a8;
    u8 unk_77ac[0x30];
    struct SpriteObject *objects[14];
    u8 unk_7814[0xd0];
    s32 frame_sync;
    s32 frame_done;
};

struct BattleEffectArgument {
    s32 kind;
    s32 side;
    s32 actor;
    s32 unknown_000c;
    s32 unknown_0010;
    s32 count;
    s32 variant;
    s32 unknown_001c;
    s32 unknown_0020;
    s16 actors[8];
};

struct Scene68 {
    u8 unk_000[0x56c];
    u16 palette[0x82];
    u16 unk_670;
    u8 unk_672[0x1f5];
    u8 unk_867;
};

struct Unit30 {
    u8 unk_00[0x36];
    s16 unk_36;
};

struct Unit88 {
    u8 unk_00[0x10];
    s32 unk_10;
};

struct Local264 {
    u16 value;
    u8 unk_02[26];
};

struct SystemWork {
    u8 unk_00[0x24];
    struct Scene68 *scene;
    u8 unk_28[0x8];
    struct Unit30 *unit30;
    u8 unk_34[0x28];
    struct SummonWork *summon;
    u8 *canvas;
    u8 *glyphs;
    void (*blit)(void *, const void *, s32, s32, s32, s32);
    u8 unk_6c[0x44];
    struct Unit88 *unit88;
    u8 unk_b4[0x8];
    void (*blit_bc)(void *, const void *, s32, s32, s32, s32);
};

struct Input {
    u32 unk_0;
    u32 unk_4;
    u32 unk_8;
    u32 pressed;
};

struct Display1120 {
    u16 unk_0;
    u16 unk_2;
    u16 unk_4;
};

struct Display11e0 {
    u32 unk_0[4];
    u32 width;
};

struct RegisterQueueEntry {
    u32 value;
    u32 address;
    u32 mode;
};

struct RegisterQueue {
    u16 count;
    u16 unk_2;
    struct RegisterQueueEntry entries[32];
};

typedef void (*DrawRectangleFn)(void *canvas, const void *source,
    s32 x, s32 y, s32 width, s32 height);

#define SYSTEM_WORK ((struct SystemWork *)0x03000000)
#define INPUT ((struct Input *)0x03001150)
#define DISPLAY_1120 ((struct Display1120 *)0x03001120)
#define DISPLAY_11E0 ((struct Display11e0 *)0x030011e0)
#define REGISTER_QUEUE ((struct RegisterQueue *)0x020038e0)
#define TILE_SLOTS ((struct TileSlot *)0x020036e0)
#define SPRITES ((struct SpriteObject **)0x02010000)
#define SPRITE_BUFFER ((u8 *)0x02010000)
#define OAM_BUFFER ((u32 *)0x02014000)
#define REVEAL_BUFFER ((u8 *)0x02014000)
#define PARTICLES ((struct Particle *)0x02014200)
#define OBJ_VRAM ((u8 *)0x06010000)
#define BG_VRAM_A900 ((u8 *)0x0600a900)
#define PLTT ((u16 *)0x05000000)
#define REG_DISPCNT (*(u16 *)0x04000000)
#define REG_BG2CNT (*(u16 *)0x0400000c)
#define REG_BG2PA (*(u16 *)0x04000020)
#define REG_BG2X (*(u32 *)0x04000028)
#define REG_WIN0H (*(u16 *)0x04000040)
#define REG_WININ (*(u16 *)0x04000048)
#define REG_BLDCNT (*(u16 *)0x04000050)
#define REG_BLDALPHA (*(u16 *)0x04000052)
#define REG_IME (*(u16 *)0x04000208)

#define CpuCopy ((void (*)(void *, const void *, u32))0x03000730)
#define CpuFill ((void (*)(void *, u32, u32))0x03000260)

extern u8 Value_00000000;
extern u8 Value_00000080;
extern u8 Value_00000075;
extern u8 Value_0000009b;
extern u8 Value_0000009c;
extern u8 Value_0000009d;
extern u8 Value_0000009e;
extern u8 Value_0000009f;
extern u8 Value_000000b4;
extern u8 Value_000000b5;
extern u8 Value_000000b6;
extern u8 Value_000000b8;
extern u8 Value_000000bd;
extern u8 Value_000000be;
extern u8 Value_000000c2;
extern u8 Value_000000c3;
extern u8 Value_00000134;
extern u8 Value_00000137;
extern u8 Value_0000013e;
extern u8 Value_000000ff;

extern const u16 Data_08197410[];
extern const u8 Data_08198c67[];
extern const u8 Data_08199210[];
extern const u8 Data_08199340[];
extern const u8 Data_08199364[];
extern u8 Data_08143001[];

void Func_081435e0(s32);
void Func_0813ba50(void);
void Func_080145a8(void *, s32);
void Func_08163c2c(s32, s32);
void Func_08118010(struct Local264 *, s32);
void Func_08013560(s32);
void Func_08014c4c(void);
void Func_08118040(s32, s32, s32);
void Func_08164b2c(s32, s32, s32);
void Func_08144aac(s32, DrawRectangleFn *);
u16 *Resource_GetTableEntry(s32);
void Func_0801587c(const void *, void *);
struct SpriteObject *Func_0815b290(s32, s32, u32, u32);
struct SpriteObject *Func_0815b3b0(s32, s32, u32, u32);
void Func_08157cf4(s32, void *, s32, s32);
void Func_081c0010(s32);
u32 Func_08014878(void);
s32 Func_0800206c(s32, s32);
void Func_08020010(struct SpriteObject *, s32 *, s32 *, s32);
s32 Func_08002096(s32);
s32 Func_08002090(s32);
s32 Func_08002054(s32, s32);
void Func_08020048(struct SpriteObject *);
void *Func_08014dac(s32);
struct Model *Func_081969f8(s32);
void Func_08014de4(void);
void Func_080151e4(s32, s32, s32);
void Func_08015160(s32, s32, s32);
void Func_08015024(s32);
void Func_080150e4(s32);
void Func_08015068(s32);
void Func_0801521c(s32);
void Func_08196958(const void *, void *, s32);
void Func_08196a7c(struct Model *);
void Func_08013164(void *);
void Func_0815b410(void);
void Func_0815b434(void *, s32, s32);
void Func_0801314c(s32);
void Func_0818caa8(void *, s32, s32, s32);
void Func_081963ec(s32, s32);
void Func_081180f0(void *, void *, s32, s32);
struct ActorPos **Func_08118098(s32);
void Func_0814cd48(s32, s32, s32, s32, s32);
void Func_081180e8(s32);
void Func_0815f000(s32, s32, s32, s32, s32, s32);
void Func_08138086(struct Particle *, s32, s32);
void Func_0815e20c(s32, struct Particle *);
void Func_08158d68(s32);
void Func_081434f8(void);
void Func_08014644(void *);
void Func_08143bb8(void);

void Func_08175f74(struct BattleEffectArgument *arg)
{
    u16 tiles[16];
    struct Local264 local264;
    s32 pos248[4];
    s32 pos232[4];
    s32 pos216[4];
    s32 pos200[4];
    s32 pos184[4];
    DrawRectangleFn routine[2];
    struct ModelDesc desc144;
    struct ModelDesc desc128;
    struct ModelDesc desc112;
    struct ModelDesc desc104;
    u8 *canvas;
    struct SummonWork *work;
    struct Unit88 *unit88;
    s32 j;
    s32 frame;
    u32 saved_width;
    u8 *glyphs;
    struct Scene68 *scene;
    DrawRectangleFn *blit;
    s32 pos_x;
    s32 pos_z;
    s32 t56;
    s32 red;
    s32 green;
    s32 blue;
    struct Unit30 *unit30;
    s32 shake;
    s16 saved_unit30;
    s32 i;
    s32 fade;
    u16 *palette;
    u8 *source;
    struct SpriteObject *object;
    struct Particle *p;
    struct Model *model;
    void *texture;
    s32 t;
    s32 scale;
    s32 depth;

    canvas = SYSTEM_WORK->canvas;
    work = SYSTEM_WORK->summon;
    unit88 = SYSTEM_WORK->unit88;
    saved_width = DISPLAY_1120->unk_4;
    glyphs = SYSTEM_WORK->glyphs;
    scene = SYSTEM_WORK->scene;
    Func_081435e0(0);
    Func_0813ba50();
    REG_BG2CNT = 0x784;
    PLTT[0] = (u32)&Value_00000000;
    PLTT[1] = (u32)&Value_00000000;
    work->transfer_mode = 0;
    Func_080145a8(Data_08143001, 0xc80);
    Func_08163c2c(1, 0);
    DISPLAY_11E0->width = 240;
    DISPLAY_1120->unk_4 = 0;
    unit88->unk_10 = 1;
    local264.value = 255;
    Func_08118010(&local264, 0);
    REG_WININ = 0x2737;
    REG_WIN0H = 240;
    Func_08013560(1);
    Func_08014c4c();
    scene->unk_867 = 1;
    Func_08118040(1, (s32)&Value_00000075, 0);
    Func_08164b2c(0, -20, -20);
    Func_08163c2c(1, 1);
    REG_BLDALPHA = 0x1010;
    REG_BLDCNT = 0x3f44;
    REG_BG2PA = (u32)&Value_00000080;
    REG_BG2X = 0;
    blit = routine;
    Func_08144aac(0, blit);

    source = (u8 *)Resource_GetTableEntry((s32)&Value_0000009c);
    CpuCopy(PLTT + 0x120, source, 0x140);
    source += 0x140;
    Func_0801587c(source, work->buffer);
    for (i = 0; i != 0x1000; i++) {
        if (work->buffer[i] != 0) {
            work->buffer[i] += 32;
        }
    }
    object = Func_0815b290(64, 64, 0xc0002000, 0);
    object->priority = 3;
    work->objects[0] = object;
    CpuCopy(OBJ_VRAM + TILE_SLOTS[object->slot].offset, work->buffer, 0x1000);
    work->objects[0]->oam_priority = 0;
    work->objects[0]->unk_17 = 32;

    source = (u8 *)Resource_GetTableEntry((s32)&Value_0000009e);
    CpuCopy(PLTT + 0x1f0, source, 32);
    Func_0801587c(source + 32, work->buffer);
    for (i = 0; i != 16; i++) {
        object = Func_0815b290(16, 16, 0, 0xf000);
        object->priority = 1;
        SPRITES[i] = object;
        CpuCopy(OBJ_VRAM + TILE_SLOTS[object->slot].offset, work->buffer + i * 32, 32);
        tiles[i] = SPRITES[i]->tile;
    }
    for (i = 0; i != 32; i++) {
        object = Func_0815b3b0(8, 8, 0, 0xf000);
        SPRITES[16 + i] = object;
        CpuCopy(object, SPRITES[0], 24);
    }
    Func_08157cf4((s32)&Value_00000134, glyphs, 0, 0);
    Func_08157cf4((s32)&Value_00000134, work->buffer, 0, 0);
    for (j = 0; j != 10; j++) {
        for (i = 0; i != 32; i++) {
            s32 value = work->buffer[Data_08197410[3] + i];

            if (value != 0) {
                value -= j * 7;
                if (value <= 0) {
                    value = 1;
                }
            }
            glyphs[j * 32 + i] = value;
        }
    }
    Func_08157cf4((s32)&Value_000000b4, work->buffer, 0, 0);
    Func_08157cf4((s32)&Value_000000b6, work->buffer + 0x1000, 1, 1);
    source = (u8 *)Resource_GetTableEntry((s32)&Value_000000bd);
    CpuCopy(PLTT, source, 128);
    REG_BG2CNT = 0x784;
    REG_BG2X = 0x800;
    pos_x = 0;
    pos_z = 0;

    for (frame = 0; frame != 394 && !(INPUT->pressed & 3); frame++) {
        if (frame >= 32 && frame < 54) {
            Func_08164b2c(32 - frame, -20, frame - 52);
        }
        if (frame == 55) {
            Func_08164b2c(0, -10, -20);
        }
        if (frame == 4) {
            Func_081c0010(107);
        }
        if (frame == 55) {
            Func_081c0010(208);
        }
        if (frame == 78) {
            Func_081c0010(219);
        }
        if (frame == 127) {
            Func_081c0010(212);
        }
        if (frame == 131) {
            Func_081c0010(149);
        }
        if (frame == 206) {
            Func_081c0010(142);
        }
        if (frame == 244) {
            Func_081c0010(212);
        }
        if (frame == 302) {
            Func_081c0010(212);
        }
        if (frame == 308) {
            Func_081c0010(212);
        }
        if (frame == 312) {
            Func_081c0010(212);
        }
        if (frame == 350) {
            Func_081c0010(104);
        }
        if (frame == 0) {
            work->transfer_mode = 2;
            work->transfer_value = 75;
            REG_BLDALPHA = 0x1010;
            for (i = 0; i != 64; i++) {
                OAM_BUFFER[i * 2] = (240 << 16) + 160;
                OAM_BUFFER[i * 2 + 1] = tiles[PARTICLES[i].timer / 16] | 0xf000;
            }
        }
        if (frame >= 132 && frame < 240) {
            s32 scale168[2] = { 0x10000, 0x10000 };

            scale168[1] = 0x14000;
            scale168[0] = 0x14000;
            if (frame == 132) {
                for (i = 0; i != 128; i++) {
                    PARTICLES[i].x = (Func_08014878() % 160 + 40) << 16;
                    PARTICLES[i].y = ((Func_08014878() & 63) + 32) << 16;
                    PARTICLES[i].timer = Func_08014878() & 255;
                    PARTICLES[i].vx = ((Func_08014878() & 255) - 127) << 9;
                    PARTICLES[i].vy = (-(Func_08014878() & 255) - 128) << 8;
                }
            }
            for (i = 0; i != 48; i++) {
                if (i == 10) {
                    scale168[1] = 0x10000;
                    scale168[0] = 0x10000;
                }
                pos248[3] = 0;
                pos248[1] = 0xff0000;
                pos248[0] = PARTICLES[64 + i].x;
                pos248[2] = PARTICLES[64 + i].y + 0x200000;
                Func_08020010(SPRITES[i], pos248, scale168, 0);
                SPRITES[i]->tile = tiles[PARTICLES[64 + i].timer / 16];
            }
            for (i = 0; i != 128; i++) {
                if (PARTICLES[i].y > -0x100000 && PARTICLES[i].x > -0x100000
                    && PARTICLES[i].x < 0x1000000) {
                    PARTICLES[i].x += PARTICLES[i].vx;
                    PARTICLES[i].y += PARTICLES[i].vy;
                }
                PARTICLES[i].timer += i / 16 + 2;
                if (PARTICLES[i].timer > 255) {
                    PARTICLES[i].timer -= 256;
                }
            }
        }
        t56 = frame - 56;
        if (t56 >= 0 && t56 < 76) {
            s32 scale160[2] = { 0x10000, 0x10000 };

            scale160[1] = scale160[0] = Func_08002096(t56 << 7) * 2 + 0x4000;
            pos232[3] = 0;
            pos232[1] = 0xff0000;
            pos232[0] = 114 << 16;
            pos232[2] = (t56 < 0 ? 100 : 78) << 16;
            Func_08020010(work->objects[0], pos232, scale160, 0);
        }
        if (frame == 131) {
            Func_08118040(1, (s32)&Value_00000075, 0);
            CpuFill(PLTT + 0x60, 0x100, 0x7fff7fff);
            CpuFill(canvas, 0x3c00, 0x3f3f3f3f);
        }
        if (frame >= 132 && frame < 147) {
            CpuFill(canvas, 0x3c00, (0x3f - (frame - 132) * 4) * 0x01010101);
        }
        if ((frame & 3) == 0 && frame >= 132 && frame < 240) {
            u16 *target = Resource_GetTableEntry((s32)&Value_00000075);
            u16 *color = PLTT + 0x60;

            for (i = 0; i != 128; i++) {
                s32 r = (*color >> 10) & 31;
                s32 g = (*color >> 5) & 31;
                s32 b = *color & 31;
                s32 tr;
                s32 tg;
                s32 tb;

                if (frame < 168) {
                    tr = (*target >> 10) & 31;
                    tg = (*target >> 5) & 31;
                    tb = *target & 31;
                } else {
                    tr = ((*target >> 10) & 31) - 16;
                    tg = ((*target >> 5) & 31) - 16;
                    tb = (*target & 31) + 8;
                }
                if (tr < 0) {
                    tr = 0;
                }
                if (tg < 0) {
                    tg = 0;
                }
                if (tb > 31) {
                    tb = 31;
                }
                if (r > tr) {
                    r--;
                }
                if (g > tg) {
                    g--;
                }
                if (b > tb) {
                    b--;
                }
                *color = (r << 10) | (g << 5) | b;
                target++;
                color++;
            }
            for (i = 0; i != 15; i++) {
                s32 r = 31 - (frame - 132) / 4;
                s32 g = r;
                s32 b = r;

                if (r > 31) {
                    r = 31;
                }
                if (r < 12) {
                    r = 12;
                }
                if (g > 31) {
                    g = 31;
                }
                if (g < 4) {
                    g = 4;
                }
                if (b > 31) {
                    b = 31;
                }
                if (b < 4) {
                    b = 4;
                }
                PLTT[0x1e1 + i] = (b << 10) | (g << 5) | r;
            }
        }
        if (frame >= 132 && frame < 240) {
            u16 *target = Resource_GetTableEntry((s32)&Value_0000009b);
            u16 *color = PLTT + 0x100;

            for (i = 0; i != 192; i++) {
                s32 r = (*color >> 10) & 31;
                s32 g = (*color >> 5) & 31;
                s32 b = *color & 31;
                s32 tr = (*target >> 10) & 31;
                s32 tg = (*target >> 5) & 31;
                s32 tb = *target & 31;

                if (r > tr) {
                    r--;
                }
                if (g > tg) {
                    g--;
                }
                if (b > tb) {
                    b--;
                }
                *color = (r << 10) | (g << 5) | b;
                target++;
                color++;
            }
        }
        fade = 0;
        if (frame >= 214 && frame < 246) {
            blue = (frame - 214) / 4;
            red = blue / 2;
            green = 0;
            fade = 1;
        }
        if (frame >= 251 && frame < 283) {
            t = (frame - 251) / 2;
            red = 4 - t / 4;
            green = -(t / 2);
            blue = 8 - t / 2;
            fade = 1;
        }
        if (frame >= 348 && frame < 412) {
            red = (frame - 348) / 4;
            green = red / 2 - 8;
            blue = red / 8 - 4;
            fade = 1;
        }
        if (fade == 1) {
            u16 *color = Resource_GetTableEntry((s32)&Value_0000009b);

            for (i = 0; i != 192; i++) {
                s32 r = (*color & 31) + red;
                s32 g = ((*color >> 5) & 31) + green;
                s32 b = ((*color >> 10) & 31) + blue;

                if (r > 31) {
                    r = 31;
                }
                if (g > 31) {
                    g = 31;
                }
                if (b > 31) {
                    b = 31;
                }
                if (r < 0) {
                    r = 0;
                }
                if (g < 0) {
                    g = 0;
                }
                if (b < 0) {
                    b = 0;
                }
                PLTT[0x100 + i] = (b << 10) | (g << 5) | r;
                color++;
            }
        }
        if (frame >= 132 && frame < 860) {
            if (frame == 132) {
                Func_08020048(work->objects[0]);
                source = (u8 *)Resource_GetTableEntry((s32)&Value_0000009b);
                CpuFill(PLTT + 0x100, 0x180, 0x7fff7fff);
                source += 0x180;
                Func_0801587c(source, work->buffer + 0x2400);
                for (i = 0; i != 12; i++) {
                    object = Func_0815b290(32, 32, 0x80002000, 0);
                    object->priority = 3;
                    work->objects[i] = object;
                    CpuCopy(OBJ_VRAM + TILE_SLOTS[object->slot].offset,
                        work->buffer + 0x2400 + i * 0x400, 0x400);
                }
                source = (u8 *)Resource_GetTableEntry((s32)&Value_0000009f);
                CpuCopy(PLTT + 0x1e0, source, 32);
                CpuFill(PLTT + 0x1e0, 32, 0x7fff7fff);
                Func_0801587c(source + 32, work->buffer + 0x2400);
                object = Func_0815b290(64, 64, 0xc0000000, 0xe000);
                work->objects[12] = object;
                object->priority = 3;
                CpuCopy(OBJ_VRAM + TILE_SLOTS[object->slot].offset, work->buffer + 0x2400, 0x800);
                object = Func_0815b290(64, 64, 0xc0002000, 0);
                work->objects[13] = object;
                object->unk_17 = 32;
                object->priority = 2;
                CpuFill(OBJ_VRAM + TILE_SLOTS[object->slot].offset, 0x1000, 0);
                source = (u8 *)Resource_GetTableEntry((s32)&Value_000000b8);
                CpuCopy(PLTT, source, 128);
                Func_08157cf4((s32)&Value_00000137, work->buffer + 0x2400, 0, 0);
                CpuCopy(glyphs, work->buffer + 0x2400, 0x302);
                Func_08157cf4((s32)&Value_000000be, work->buffer + 0x1000, 1, 1);
                pos_x = 144 << 15;
                pos_z = 224 << 14;
            }
            if (frame == 243) {
                source = (u8 *)Resource_GetTableEntry((s32)&Value_0000009d);
                CpuFill(PLTT + 0x1c0, 64, 0);
                Func_0801587c(source + 64, REVEAL_BUFFER);
                for (i = 0; i != 0x1000; i++) {
                    if (REVEAL_BUFFER[i] != 0) {
                        REVEAL_BUFFER[i] += 192;
                    }
                }
                for (i = 0; i != 32; i++) {
                    REVEAL_BUFFER[0x1000 + i] = i;
                }
                for (i = 0; i != 32; i++) {
                    u8 *other = &REVEAL_BUFFER[0x1000 + (Func_08014878() & 31)];
                    u8 swap = REVEAL_BUFFER[0x1000 + i];

                    REVEAL_BUFFER[0x1000 + i] = *other;
                    *other = swap;
                }
            }
            {
                s32 scale152[2];

                pos216[3] = 0;
                pos216[1] = 0xff0000;
                if (frame < 160) {
                    pos_x += 0x2000;
                    pos_z -= 0x4000;
                } else if (frame < 332) {
                    pos_x += 0x2000;
                    pos_z += 0x1000;
                } else {
                    pos_x += 0x2000;
                }
                scale152[1] = 0x14000;
                scale152[0] = 0x14000;
                scale152[1] = scale152[0] = pos_z / 48 + 0x4000;
                pos216[0] = pos_x + 0x400000;
                pos216[2] = 144 << 16;
                Func_08020010(work->objects[12], pos216, scale152, 0);
                scale152[1] = 0x10000;
                scale152[0] = 0x10000;
                for (i = 0; i != 12; i++) {
                    pos216[0] = pos_x + ((i % 4) << 21);
                    pos216[2] = pos_z + ((i / 4) << 21);
                    Func_08020010(work->objects[i], pos216, scale152, 0);
                }
            }
            if (frame == 200) {
                source = (u8 *)Resource_GetTableEntry((s32)&Value_000000bd);
                CpuCopy(PLTT, source, 128);
            }
            if (frame >= 200 && frame < 302) {
                texture = Func_08014dac(32);
                model = Func_081969f8(1);
                t = frame - 200;
                depth = t * 4 - 64;
                scale = 100000 - t * 1524;
                if (frame > 253) {
                    depth = (254 - frame) * 8;
                }
                if (scale < 13800) {
                    scale = 13800;
                }
                if (depth > 0) {
                    depth = 0;
                }
                model->depth = depth;
                work->transfer_mode = 2;
                work->transfer_value = 75;
                desc144.a = 7;
                desc144.b = 7;
                desc144.tiles = work->buffer + 0x1000;
                model->kind = 7;
                model->desc = &desc144;
                model->mesh = Data_08199364;
                model->texture = texture;
                if (frame <= 282) {
                    Func_08014de4();
                    Func_080151e4(scale * 2, scale * 2, scale * 2);
                    Func_08015160(-0x100000, -0x80000, 0);
                    Func_08015024(0xe000);
                    Func_080150e4(0x4000 - t * 256);
                    Func_08015068(-t << 10);
                    Func_08196958(Data_08199210, texture, 4);
                    Func_08196a7c(model);
                }
                if (frame > 231) {
                    t = frame - 232;
                    depth = t * 2 - 64;
                    scale = 100000 - t * 1200;
                    if (depth < -64) {
                        depth = -64;
                    }
                    if (depth > 0) {
                        depth = 0;
                    }
                    desc144.a = 6;
                    desc144.b = 6;
                    model->depth = depth;
                    desc144.tiles = work->buffer;
                    model->desc = &desc144;
                    model->mesh = Data_08199340;
                    model->kind = 7;
                    model->texture = texture;
                    Func_08014de4();
                    Func_08015160(-0x80000, -0x80000, 0);
                    Func_080151e4(scale, scale, scale);
                    Func_08015024(0xc000);
                    Func_08015068(-t << 10);
                    Func_08196958(Data_08199210, texture, 4);
                    Func_08196a7c(model);
                }
                Func_08013164(model);
                Func_08013164(texture);
            }
            if (frame >= 244 && frame < 310) {
                s32 scale136[2] = { 0x10000, 0x10000 };

                if (frame < 260) {
                    object = work->objects[13];
                    for (i = 0; i != 64; i++) {
                        s32 column = REVEAL_BUFFER[0x1000 + (frame - 244) * 2];

                        *(u16 *)(OBJ_VRAM + i * 64 + TILE_SLOTS[object->slot].offset + column * 2) =
                            REVEAL_BUFFER[i * 64 + column * 2] | (REVEAL_BUFFER[i * 64 + column * 2 + 1] << 8);
                        column = REVEAL_BUFFER[0x1000 + (frame - 244) * 2 + 1];
                        *(u16 *)(OBJ_VRAM + i * 64 + TILE_SLOTS[object->slot].offset + column * 2) =
                            REVEAL_BUFFER[i * 64 + column * 2] | (REVEAL_BUFFER[i * 64 + column * 2 + 1] << 8);
                    }
                }
                scale136[1] = scale136[0] = (155 << 12) - (frame << 11);
                pos200[3] = 0;
                pos200[1] = 0xff0000;
                pos200[0] = 184 << 15;
                pos200[2] = 144 << 15;
                Func_08020010(work->objects[13], pos200, scale136, 0);
                {
                    u16 *target = Resource_GetTableEntry((s32)&Value_0000009d);
                    u16 *color = PLTT + 0x1c0;

                    for (i = 0; i != 32; i++) {
                        s32 r = (*color >> 10) & 31;
                        s32 g = (*color >> 5) & 31;
                        s32 b = *color & 31;
                        s32 tr = (*target >> 10) & 31;
                        s32 tg = (*target >> 5) & 31;
                        s32 tb = *target & 31;

                        if (r < tr) {
                            r++;
                        }
                        if (g < tg) {
                            g++;
                        }
                        if (b < tb) {
                            b++;
                        }
                        *color = (r << 10) | (g << 5) | b;
                        target++;
                        color++;
                    }
                }
            }
            if (frame > 301) {
                if (frame == 302) {
                    CpuFill(canvas, 0x3c00, 0x3f3f3f3f);
                    CpuFill(PLTT + 0x1c0, 64, 0x7fff7fff);
                    CpuCopy(PLTT + 0x60, Resource_GetTableEntry((s32)&Value_00000075), 0x100);
                    source = (u8 *)Resource_GetTableEntry((s32)&Value_000000bd);
                    CpuCopy(PLTT, source, 128);
                    Func_08157cf4((s32)&Value_000000c2, work->buffer + 0x1000, 0, 0);
                    Func_0815b410();
                }
                if (frame > 303) {
                    u16 *target = Resource_GetTableEntry((s32)&Value_00000075);
                    u16 *color = PLTT + 0x60;

                    for (i = 0; i != 128; i++) {
                        s32 r = (*color >> 10) & 31;
                        s32 g = (*color >> 5) & 31;
                        s32 b = *color & 31;
                        s32 tr = ((*target >> 10) & 31) - 20;
                        s32 tg = ((*target >> 5) & 31) - 20;
                        s32 tb = (*target & 31) + 8;

                        if (tr < 0) {
                            tr = 0;
                        }
                        if (tg < 0) {
                            tg = 0;
                        }
                        if (tb > 31) {
                            tb = 31;
                        }
                        if (r > tr) {
                            r--;
                        }
                        if (g > tg) {
                            g--;
                        }
                        if (b > tb) {
                            b--;
                        }
                        *color = (r << 10) | (g << 5) | b;
                        target++;
                        color++;
                    }
                }
                if (frame > 301) {
                    s32 k;
                    s32 x;
                    s32 depth2;
                    s32 size;

                    texture = Func_08014dac(32);
                    model = Func_081969f8(1);
                    desc128.a = 7;
                    desc128.b = 7;
                    desc128.tiles = work->buffer + 0x1000;
                    model->kind = 7;
                    model->desc = &desc128;
                    model->mesh = Data_08199364;
                    model->texture = texture;
                    for (k = 0; k != 3; k++) {
                        x = frame - 302 - k * 8;
                        if (x >= 0) {
                            size = 140000 - (x * 6192 - 16);
                            depth2 = x * 4 - 16;
                            if (size < 0) {
                                size = 0;
                            }
                            if (depth2 > 0) {
                                depth2 = 0;
                            }
                            model->depth = depth2;
                            Func_08014de4();
                            Func_08015160(-0x80000, -0x80000, 0);
                            Func_080151e4(size / 2, size, size / 2);
                            if (k == 0) {
                                Func_080150e4(0xf000);
                            } else if (k == 1) {
                                Func_080150e4(0x2000);
                            } else {
                                Func_080150e4(0xe000);
                            }
                            Func_08015024(0x800);
                            Func_08196958(Data_08199210, texture, 4);
                            Func_08196a7c(model);
                        }
                    }
                    Func_08013164(model);
                    Func_08013164(texture);
                }
            }
            if (frame == 330) {
                CpuFill(PLTT + 0x1c0, 64, 0x7c1f7c1f);
            }
            if (frame >= 330 && frame < 351) {
                s32 scale120[2] = { 0x10000, 0x10000 };
                s32 dx = 0;
                s32 dz = 0;

                if (frame > 346) {
                    dx = 347 - frame;
                    dz = (frame - 347) / 4;
                }
                scale120[1] = scale120[0] = (frame << 11) - 0xa5000;
                pos184[3] = 0;
                pos184[1] = 0xff0000;
                pos184[0] = (dx << 16) + (184 << 15);
                pos184[2] = (dz << 16) + (144 << 15);
                Func_08020010(work->objects[13], pos184, scale120, 0);
            }
            if (frame == 347) {
                work->transfer_mode = 1;
                work->transfer_value = 0;
                REG_BLDCNT = 0;
                for (i = 0; i != 63; i++) {
                    s32 r = 31 - i;
                    s32 b = r;

                    if (r < 0) {
                        r = 0;
                    }
                    if (b < 0) {
                        b = 0;
                    }
                    PLTT[1 + i] = (b << 10) | r;
                }
            }
            if (frame > 346) {
                s32 height = 54;

                t = frame * 4 - 1380;
                if (frame > 349) {
                    height = ((-(Func_08002096((frame << 10) - 0x57800) * 54)) >> 16) + 54;
                }
                if (frame == 360) {
                    work->transfer_mode = 0;
                }
                if (frame <= 371) {
                    Func_0815b434(work->buffer + 0x1000, t, 0x10000);
                } else {
                    t = 104;
                }
                Func_0801314c(188);
                Func_0818caa8(work->buffer + 0x1000, height, 58, t);
                Func_081963ec(188, 3);
                blit[1] = SYSTEM_WORK->blit_bc;
            }
            for (j = 0; j != 2; j++) {
                t = frame - 357 - j * 19;
                if (t == 0) {
                    CpuFill(PLTT + 0x60, 0x100, 0x7fff7fff);
                }
                if (t >= 0 && t < 160) {
                    u16 *target = Resource_GetTableEntry((s32)&Value_00000075);
                    u16 *color = PLTT + 0x60;

                    for (i = 0; i != 128; i++) {
                        s32 r = (*color >> 10) & 31;
                        s32 g = (*color >> 5) & 31;
                        s32 b = *color & 31;
                        s32 tr;
                        s32 tg;
                        s32 tb;

                        if (t < 8) {
                            tr = (*target >> 10) & 31;
                            tg = (*target >> 5) & 31;
                            tb = *target & 31;
                        } else {
                            tr = ((*target >> 10) & 31) - 16;
                            tg = ((*target >> 5) & 31) - 16;
                            tb = (*target & 31) + 8;
                        }
                        if (tr < 0) {
                            tr = 0;
                        }
                        if (tg < 0) {
                            tg = 0;
                        }
                        if (tb > 31) {
                            tb = 31;
                        }
                        if (r > tr + 1) {
                            r -= 2;
                        }
                        if (g > tg + 1) {
                            g -= 2;
                        }
                        if (b > tb + 1) {
                            b -= 2;
                        }
                        *color = (r << 10) | (g << 5) | b;
                        target++;
                        color++;
                    }
                }
            }
        }
        if (t56 >= 0 && t56 < 24) {
            s32 radius = frame * 2 - 104;
            s32 row = (frame * 2 - 114) / 6;
            s32 x;
            s32 y;
            s32 w = 4;
            s32 h = 8;

            if (row < 0) {
                row = 0;
            }
            for (i = 0; i != 256; i++) {
                x = ((Func_08002096(i << 8) * radius * 2) >> 16) + 66;
                y = ((Func_08002090(i << 8) * radius) >> 16) + 68;
                blit[0](canvas, glyphs + row * 32, x - w / 2, y, w, h);
            }
        }
        if (frame >= 108 && frame < 132) {
            u16 *color = PLTT + 0x60;

            for (i = 0; i != 128; i++) {
                s32 r = *color & 31;
                s32 g = ((*color >> 5) & 31) - 1;
                s32 b = (*color >> 10) & 31;

                if (g < 0) {
                    r--;
                }
                if (r > 31) {
                    r = 31;
                } else if (r < 0) {
                    r = 0;
                }
                if (g > 31) {
                    g = 31;
                } else if (g < 0) {
                    g = 0;
                }
                if (b > 31) {
                    b = 31;
                } else if (b < 0) {
                    b = 0;
                }
                *color = (b << 10) | (g << 5) | r;
                color++;
            }
        }
        if (t56 >= 0 && t56 < 36) {
            if (frame == 56) {
                s32 x;
                s32 y;

                work->transfer_mode = 3;
                work->transfer_value = 0x04040404;
                source = (u8 *)Resource_GetTableEntry((s32)&Value_000000b5);
                Func_0801587c(source + 32, work->buffer + 0x2400);
                for (y = 0; y != 80; y++) {
                    for (x = 0; x != 184; x += 2) {
                        u16 *dst = (u16 *)(BG_VRAM_A900 + ((((y / 8) * 32 + x / 8) * 8 + (y & 7)) * 8 + (x & 7)));
                        u8 hi = *dst >> 8;
                        u8 lo = *dst;

                        if (work->buffer[0x2400 + y * 184 + x + 1] != 0) {
                            hi = work->buffer[0x2400 + y * 184 + x + 1] - 32;
                        }
                        if (work->buffer[0x2400 + y * 184 + x] != 0) {
                            lo = work->buffer[0x2400 + y * 184 + x] - 32;
                        }
                        *dst = (hi << 8) | lo;
                    }
                }
            }
            if (frame < 88) {
                u16 *color = Resource_GetTableEntry((s32)&Value_000000b5);

                for (i = 0; i != 16; i++) {
                    s32 add = ((Func_08002090(t56 << 9) * 12) >> 16) + 16;
                    s32 r = (*color & 31) + add;
                    s32 g = ((*color >> 5) & 31) + add;
                    s32 b = ((*color >> 10) & 31) + add;

                    if (r > 31) {
                        r = 31;
                    }
                    if (g > 31) {
                        g = 31;
                    }
                    if (b > 31) {
                        b = 31;
                    }
                    PLTT[0xe0 + i] = (b << 10) | (g << 5) | r;
                    color++;
                }
            }
        }
        if (frame == 54) {
            CpuFill(canvas, 0x3c00, 0x20202020);
        }
        if (frame == 55) {
            CpuFill(canvas, 0x3c00, 0x3f3f3f3f);
        }
        if (frame < 132) {
            s32 scale112;

            texture = Func_08014dac(32);
            model = Func_081969f8(1);
            work->transfer_mode = 2;
            work->transfer_value = 50;
            depth = frame * 2 - 64;
            scale112 = 100000 - frame * 1524;
            if (frame > 55) {
                scale112 = frame * 1524 - 52576;
            }
            if (depth >= -8) {
                depth = -8;
            }
            if (frame > 95) {
                depth = 184 - frame * 2;
                model->depth = depth;
            }
            desc112.a = 6;
            desc112.b = 6;
            desc112.tiles = work->buffer;
            model->kind = 7;
            model->mesh = Data_08199340;
            model->desc = &desc112;
            model->texture = texture;
            if (depth > -48) {
                model->depth = depth;
                Func_08014de4();
                Func_08015160(0x40000, 0, 0);
                Func_080151e4(scale112, scale112, scale112);
                Func_08015024(0x2000);
                if (frame > 55) {
                    Func_08015068(frame << 10);
                } else {
                    Func_08015068(-frame << 10);
                }
                Func_08196958(Data_08199210, texture, 4);
                Func_08196a7c(model);
            }
            if (frame > 95) {
                depth = 568 - frame * 6;
                model->depth = depth;
            }
            if (depth > -48) {
                Func_08014de4();
                Func_08015160(0x40000, -0x100000, 0);
                Func_0801521c(-0xa710);
                Func_08015024(0x2000);
                Func_08015068(-frame << 9);
                Func_08196958(Data_08199210, texture, 4);
                Func_08196a7c(model);
            }
            if (frame > 55) {
                s32 d = frame * 2 - 208;

                desc112.tiles = work->buffer + 0x1000;
                if (d >= -32) {
                    d = -32;
                }
                model->depth = d;
                Func_08014de4();
                Func_08015160(0x20000, -0x200000, 0);
                Func_0801521c(0x58f0);
                Func_08015024(0x2000);
                Func_08015068(frame << 10);
                Func_08196958(Data_08199210, texture, 4);
                Func_08196a7c(model);
                if (frame > -48) {
                    Func_08014de4();
                    Func_08015160(0x20000, -0x300000, 0);
                    Func_0801521c(-0x31e0);
                    Func_08015024(0x2000);
                    Func_08015068(-frame << 10);
                    Func_08196958(Data_08199210, texture, 4);
                    Func_08196a7c(model);
                }
            }
            Func_08013164(model);
            Func_08013164(texture);
            while (work->frame_sync <= 1) {
            }
        }
        work->frame_done = 1;
        Func_08013560(1);
    }

    Func_081c0010(162);
    if (frame < 132) {
        Func_08020048(work->objects[0]);
    } else {
        for (i = 0; i != 14; i++) {
            Func_08020048(work->objects[i]);
        }
    }
    for (i = 0; i != 48; i++) {
        Func_08020048(SPRITES[i]);
    }
    Func_0801314c(188);
    Func_0801314c(104);
    Func_08014c4c();
    REG_DISPCNT = 0x141;
    Func_08013560(1);
    scene->unk_867 = 0;
    Func_08118040(1, scene->unk_670, 24);
    CpuFill(canvas, 0x4000, 0);
    CpuFill((void *)0x06004000, 0x4000, 0);
    DISPLAY_11E0->width = 120;
    unit30 = SYSTEM_WORK->unit30;
    shake = 0;
    saved_unit30 = unit30->unk_36;
    unit30->unk_36 = shake;
    work->transfer_mode = 1;
    work->transfer_value = 0;
    work->transfer_mode = 2;
    work->transfer_value = 50;
    REG_BLDALPHA = 0x410;
    REG_BG2X = 0;
    REG_BG2CNT = 0x786;
    for (i = 1; i != 64; i++) {
        s32 q = i * 2 / 3;
        s32 r = 31 - q;
        s32 g = 8 - i / 2;
        s32 b = 27 - q;

        if (r < 0) {
            r = 0;
        }
        if (g < 0) {
            g = 0;
        }
        if (b < 0) {
            b = 0;
        }
        PLTT[i] = (b << 10) | (g << 5) | r;
    }
    arg->actors[arg->count] = (u32)&Value_000000ff;
    Func_08118010((struct Local264 *)arg->actors, 0);
    Func_0815b434(work->buffer, 56, 0x10000);
    Func_0815b434(work->buffer + 0x1000, 16, 0x10000);
    CpuFill(work->buffer + 0x2000, 0x1000, 0x3f3f3f3f);
    Func_08157cf4((s32)&Value_000000c3, work->buffer + 0x3000, 0, 0);
    Func_08157cf4((s32)&Value_00000134, glyphs, 0, 0);
    for (i = 0; i != 0x302; i += 35) {
    }
    for (i = 0; i != 0x1000; i++) {
        if (work->buffer[0x3000 + i] != 0) {
            work->buffer[0x3000 + i] = 63;
        }
    }
    {
        u16 ime = REG_IME;

        REG_IME = 0x208;
        if (REGISTER_QUEUE->count < 32) {
            struct RegisterQueueEntry *entry = &REGISTER_QUEUE->entries[REGISTER_QUEUE->count++];

            entry->value = 0x7741;
            entry->address = 0x04000000;
            entry->mode = 0x20000;
        }
        REG_IME = ime;
    }
    for (i = 0; i != 16; i++) {
        work->drops[i].timer = -i;
    }
    for (i = 0; i != 128; i++) {
        PARTICLES[i].timer = -1;
    }
    for (i = 0; i != arg->count; i++) {
        struct ActorPos *actor = *Func_08118098(arg->actors[i]);

        work->homes[i].x = actor->x;
        work->homes[i].y = actor->y;
        work->homes[i].z = actor->z;
    }

    frame = 0;
    do {
        if ((INPUT->pressed & 3) && frame < 120) {
            frame = 120;
        }
        if (frame == 0) {
            s32 x;

            Func_081c0010(162);
            work->wave = 160;
            shake = 0;
            for (i = 0; i != arg->count; i++) {
                work->members[i].timer = -1;
            }
            x = -0x2000000;
            for (i = 0; i != arg->count; i++) {
                struct ActorPos *actor = *Func_08118098(arg->actors[i]);

                Func_0814cd48(arg->actors[i], 0, 5, -1, -1);
                actor->x = x;
                actor->y = 0x140000;
                actor->z = 0;
                actor->unk_48 = 0;
                x -= 0x800000;
            }
        }
        if (frame == 120) {
            work->transfer_mode = 1;
            work->transfer_value = 0;
            Func_081180f0(scene->palette, PLTT + 0x60, 0x99a0, 128);
            for (i = 0; i != arg->count; i++) {
                struct ActorPos *actor = *Func_08118098(arg->actors[i]);

                actor->x = 0;
                actor->y = 0x7c0000;
                actor->z = 0;
                actor->unk_48 = 0;
            }
        }
        if (frame == 160) {
            Func_081180e8(134);
        }
        if (frame >= 140) {
            if (frame == 140) {
                for (i = 0; i != 32; i++) {
                    s32 speed = Func_08014878() & 511;
                    s32 angle = Func_08014878();

                    PARTICLES[96 + i].x = 240 << 14;
                    PARTICLES[96 + i].y = 240 << 14;
                    speed += 32;
                    PARTICLES[96 + i].vx = (Func_08002096(angle) * speed) >> 5;
                    PARTICLES[96 + i].vy = (Func_08002090(angle) * speed) >> 5;
                    PARTICLES[96 + i].timer = 0;
                }
                for (i = 0; i != 128; i++) {
                    s32 speed = Func_08014878() & 255;
                    s32 angle = Func_08014878();

                    PARTICLES[128 + i].x = 240 << 14;
                    PARTICLES[128 + i].y = 240 << 14;
                    speed += 32;
                    PARTICLES[128 + i].vx = (Func_08002096(angle) * speed) >> 6;
                    PARTICLES[128 + i].vy = (Func_08002090(angle) * speed) >> 6;
                    PARTICLES[128 + i].timer = (Func_08014878() & 15) + 16;
                }
                Func_08157cf4((s32)&Value_0000013e, SPRITE_BUFFER, 1, 0);
                Func_081c0010(144);
                CpuFill(canvas, 0x3c00, 0x3f3f3f3f);
                REG_BG2CNT = 0x784;
                work->transfer_mode = 2;
                work->transfer_value = 50;
                for (i = 0; i != arg->count; i++) {
                    struct ActorPos *actor = *Func_08118098(arg->actors[i]);

                    actor->x = 0;
                    actor->y = 0x200000;
                    actor->z = work->homes[i].z;
                }
                for (i = 0; i != arg->count; i++) {
                    s32 spin = Func_08014878();

                    Func_0815f000(arg->actors[i], 1, 0x40000, 0x80000, spin * 2 + 0x20000,
                        (Func_08014878() & 31) + 120);
                    Func_0814cd48(arg->actors[i], 7, 5, i, 32);
                }
                work->unk_77a8 = 3;
            }
            Func_081963ec(104, 31);
            blit[0] = SYSTEM_WORK->blit;
            for (i = 0; i != 16; i++) {
                p = &PARTICLES[96 + i];
                if (p->timer >= 0 && p->timer < 24) {
                    blit[0](canvas, SPRITE_BUFFER + (p->timer / 4) * 2048,
                        (p->x >> 16) - 16, (p->y >> 16) - 32, 32, 64);
                    Func_08138086(p, 60, -0x2000);
                }
                p->timer++;
            }
            for (i = 0; i != 128; i++) {
                p = &PARTICLES[128 + i];
                if (p->timer >= 0) {
                    s32 size = (p->timer >> 3) + 2;
                    s32 k;

                    for (k = 0; k != 4; k++) {
                        blit[0](canvas, glyphs + Data_08197410[size - 1],
                            (p->x >> 16) - size / 2, (p->y >> 16) - size, size, size * 2);
                        Func_08138086(p, 63, 0);
                        if (p->y > 224 << 15) {
                            p->vy = -p->vy;
                        }
                    }
                    p->timer--;
                }
            }
            Func_0801314c(104);
        }
        if (frame < 120) {
            for (i = 0; i != 3; i++) {
                if (frame == Data_08198c67[i]) {
                    CpuFill(PLTT + 0x60, 0x100, 0x7fff7fff);
                }
                if (frame >= Data_08198c67[i] + 1 && frame < Data_08198c67[i] + 16) {
                    u16 *target = SYSTEM_WORK->scene->palette;

                    for (j = 0; j != 128; j++) {
                        s32 tr = (target[j] & 31) - 10;
                        s32 tg = ((target[j] >> 5) & 31) - 20;
                        s32 tb = ((target[j] >> 10) & 31) - 10;
                        s32 r = PLTT[0x60 + j] & 31;
                        s32 g = (PLTT[0x60 + j] >> 5) & 31;
                        s32 b = (PLTT[0x60 + j] >> 10) & 31;

                        if (tr < 0) {
                            tr = 0;
                        }
                        if (tg < 0) {
                            tg = 0;
                        }
                        if (tb < 0) {
                            tb = 0;
                        }
                        if (tr < r - 1) {
                            r -= 2;
                        }
                        if (tg < g - 1) {
                            g -= 2;
                        }
                        if (tb < b - 1) {
                            b -= 2;
                        }
                        PLTT[0x60 + j] = (b << 10) | (g << 5) | r;
                    }
                }
            }
        }
        if (frame > 120) {
            if (frame == 121) {
                DISPLAY_1120->unk_4 = saved_width;
                unit88->unk_10 = 0;
                unit30->unk_36 = saved_unit30;
            }
            if (frame < 148) {
                s32 wave = work->wave;

                work->wave = wave - (8 - (frame - 120) / 6);
                if (wave > 0) {
                    Func_0815b434(work->buffer, wave, 0x10000);
                    Func_0818caa8(work->buffer, 60, 60, wave);
                }
            }
        } else {
            if (frame >= 4 && frame < 26) {
                shake += 4;
            }
            if (frame > 107) {
                shake -= 8;
                if (shake < 0) {
                    shake = 0;
                }
            }
            for (i = 0; i != arg->count; i++) {
                struct ActorPos *actor = *Func_08118098(arg->actors[i]);
                struct Particle *member = &work->members[i];
                s32 landed = 0;

                if (actor->y < 0x640000) {
                    if (member->timer == -1) {
                        actor->x += 0xa0000;
                    } else {
                        actor->x += 0x20000;
                    }
                    if (member->timer == -1 && actor->x > 0x300000) {
                        landed = 1;
                    }
                    if (landed == 1) {
                        member->timer = 0;
                        Func_0815e20c(arg->actors[i], member);
                        Func_081c0010(103);
                        for (j = 0; j != 16; j++) {
                            s32 angle = Func_08014878() & 0x7fff;
                            s32 speed = Func_08014878() & 255;

                            p = &PARTICLES[i * 16 + j];
                            p->x = member->x << 15;
                            p->y = member->y << 16;
                            speed += 256;
                            p->vx = -(Func_08002096(angle) * speed) >> 7;
                            p->vy = (Func_08002090(angle) * speed) >> 6;
                            p->timer = (Func_08014878() & 15) + 16;
                        }
                    }
                }
                if (member->timer == 2 || member->timer == 7) {
                    Func_0814cd48(arg->actors[i], 7, 5, i, 32);
                }
                if (member->timer == 6) {
                    Func_0814cd48(arg->actors[i], 15, -1, i, 32);
                }
                if (member->timer == 8) {
                    actor->x = 0;
                    actor->y = 148 << 16;
                    actor->z = 0;
                }
            }
            if (frame >= 0) {
                DISPLAY_1120->unk_4 += 0xfff4;
                Func_081963ec(104, 27);
                for (i = 0; i != 4; i++) {
                    struct Particle *column = &work->columns[i];

                    column->x = shake + i * 8 + ((Func_08002096((frame << 10) + i * 0x2000) * 4) >> 16);
                    column->y = i * 30;
                    if (i == 3) {
                        column->x -= 8;
                        column->y = i * 30 - 26;
                        Func_0801314c(104);
                        Func_081963ec(104, 19);
                    }
                    SYSTEM_WORK->blit(canvas, work->buffer, column->x - 28, column->y, 28, 56);
                }
                Func_0801314c(104);
                for (i = 0; i != 12; i++) {
                    struct Particle *drop = &work->drops[i];

                    if (++drop->timer == 4) {
                        drop->timer = 0;
                    }
                    if (drop->timer == 0) {
                        drop->x = (Func_08014878() % 24 + shake - i * 2) << 16;
                        drop->y = (160 << 15) + i * (192 << 10);
                        drop->vx = -(Func_08014878() & 0xffff) - 0x4000;
                        drop->vy = -0x10000;
                    }
                    if (drop->timer >= 0) {
                        Func_0818caa8(work->buffer + 0x1000, drop->x >> 16, drop->y >> 16, 16);
                        drop->x += drop->vx;
                        drop->y += drop->vy;
                    }
                }
                Func_081963ec(104, 31);
                blit[0] = SYSTEM_WORK->blit;
                for (i = 0; i != 4; i++) {
                    s32 width = 120 - work->columns[i].x;

                    if (width > 0) {
                        blit[0](canvas, work->buffer + 0x2000, work->columns[i].x, i * 30, width, 30);
                    }
                }
                Func_0801314c(104);
                Func_081963ec(104, 47);
                blit[0] = SYSTEM_WORK->blit;
                for (i = 0; i != 96; i++) {
                    p = &PARTICLES[i];
                    if (p->timer >= 0) {
                        s32 size = (p->timer >> 3) + 2;

                        blit[0](canvas, glyphs + Data_08197410[size - 1],
                            (p->x >> 16) - size / 2, (p->y >> 16) - size, size, size * 2);
                        Func_08138086(p, 60, 0);
                        p->timer--;
                    }
                }
                Func_0801314c(104);
            }
            texture = Func_08014dac(32);
            model = Func_081969f8(1);
            desc104.a = 6;
            desc104.b = 6;
            desc104.tiles = work->buffer + 0x3000;
            model->kind = 9;
            model->mesh = Data_08199340;
            model->desc = &desc104;
            model->texture = texture;
            for (i = 0; i != arg->count; i++) {
                struct Particle *member = &work->members[i];

                if (member->timer >= 0 && member->timer < 18) {
                    s32 size = member->timer * 3 * 1016 + 0x800;

                    model->depth = member->timer * 3 - 64;
                    member->timer++;
                    Func_08014de4();
                    Func_08015160((member->x / 2 - 56) << 16, (member->y - 56) << 16, 0);
                    Func_080151e4(size / 3, size, size);
                    Func_08015024(0xc000);
                    Func_08196958(Data_08199210, texture, 4);
                    Func_08196a7c(model);
                }
            }
            Func_08013164(model);
            Func_08013164(texture);
        }
        Func_08158d68(8);
        Func_081434f8();
        work->frame_done = 1;
        Func_08013560(1);
        frame++;
    } while (frame != 180);
    for (i = 0; i != arg->count; i++) {
    }
    Func_0801314c(188);
    Func_08014644(Data_08143001);
    Func_08143bb8();
}
