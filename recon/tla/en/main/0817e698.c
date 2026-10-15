/* Draft (tla-en): listing 386 instructions, draft 386, common 374, differ 24.
 * The shape matches, including the three mirrored copies as indexed loops
 * (written with walking pointers loop.c makes outer givs of them: differ
 * 206). Left: in each copy's inner loop the native steps the left pointer
 * before its second load (ldrb, add r4, strb, add r0, ldrb, add r2, strb,
 * sub r1); the draft loads first. After the calls the native loads the
 * pool word (IwramClearWords, IwramCopyWords, BG2PA's 0x100) before the
 * neighbouring constant, and the first "mov r3, #192" comes after the
 * argument's spill. u32 or u8 counters and a s8 canvas change nothing.
 * The copy loops' order is what sched2 gives when a load after a store to
 * the canvas waits two cycles (here one): the second ldrb is ready at
 * once and goes before the left pointer's add. Measured against it: an
 * empty asm or a volatile memory barrier between the copies (loop.c
 * then rebuilds the row pointers: differ 166), a one-pass block around
 * the second copy (differ 278), a volatile load or store (the address
 * givs no longer combine: 166 to 267), an asm with an unused output
 * (deleted), and walking pointers per row (206).
 * Resources 316, 328, 297 and 336 need their names in
 * system/resource/directory.s when this is adopted.
 */
#include "types.h"
#include "io_reg.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "effect_step.h"
#include "battle_effect_work.h"

struct FxWork {
    u8 canvas[0x7780];
    s32 transfer_mode;
    s32 transfer_value;
    u8 unknown_7788[0x20];
    s32 shake_frames;
    u8 unknown_77ac[0x78e8 - 0x77ac];
    s32 transfer_pending;
};

struct SpriteSource {
    u32 bits;
    void *pixels;
};

struct RenderModel {
    s32 kind;
    s32 unknown_04;
    const void *mesh;
    void *vertices;
    struct SpriteSource *source;
    s32 unknown_14;
};

extern char ResourceId_EarthWallSheet;
extern char ResourceId_OrangePaletteB;
extern char ResourceId_FireballSheet;
extern char ResourceId_MarsDjinnSheet;
extern const u8 Data_08199340[];
extern const s32 Data_081994e0[];
extern const s32 Data_081994d0[];
extern const u8 Data_081991f0[];

void BattleFx_BeginCanvasLayer(s32 bg_control);
void Resource_LoadAndDecompress(s32 resource_id, void *target, s32 unpack, s32 wait);
void *Resource_GetTableEntry(s32 resource_id);
void Scheduler_AddOrUpdateCallback(s32 callback, s32 priority);
s32 Scheduler_RemoveCallback(s32 callback);
void EffectPosition_ApplyAlternateStepAndYOffset(s32 id, struct EffectPosition *position);
void Render_ResetTransformState(void);
void Audio_PlayCue(s32 cue);
void Func_081180e8(s32 cue);
void Func_081963ec(s32 slot, s32 value);
void Func_0814cd48(s32 id, s32 a, s32 b, s32 c, s32 d);
void *Runtime_BumpAllocateAlternatePool(s32 size);
void Runtime_ReleaseHeapBlock(s32 slot);
struct RenderModel *Func_081969f8(s32 kind);
void Func_08015160(s32 x, s32 y, s32 z);
void Func_0801521c(s32 scale);
void Func_08196958(const void *mesh, void *vertices, s32 count);
void Func_08196a7c(struct RenderModel *model);
void MemFree(void *buffer);
void Func_08158ce0(s32 x, s32 y);
void Func_081434f8(void);
void WaitFrames(s32 frames);
void Func_08143000(void);
void Func_08143bb8(void);

void Func_0817e698(struct BattleEffectArgument *effect, s32 mode)
{
    struct FxWork *work = (struct FxWork *)Ram_HeapSlots->battle_fx_work[0];
    struct SpriteSource sprite;
    struct EffectPosition pos;
    struct RenderModel *model;
    void *vertices;
    s32 frame;
    s32 cell;
    s32 y;
    s32 x;
    u8 *src;
    u8 *left;
    u8 *right;

    BattleFx_BeginCanvasLayer(0);
    Resource_LoadAndDecompress((s32)&ResourceId_EarthWallSheet, work->canvas + 0x4700, 1, 1);
    Iwram_ClearWords(work->canvas + 0x700, 0x4000);
    for (y = 0; y != 49; y++) {
        for (x = 0; x != 17; x++) {
            work->canvas[0x700 + (y + 15) * 64 + 15 + x] = work->canvas[0x518e + y * 17 + x];
            work->canvas[0x700 + (y + 15) * 64 + 48 - x] = work->canvas[0x518e + y * 17 + x];
        }
    }
    for (y = 0; y != 55; y++) {
        for (x = 0; x != 19; x++) {
            work->canvas[0x1700 + (y + 9) * 64 + 13 + x] = work->canvas[0x54cf + y * 19 + x];
            work->canvas[0x1700 + (y + 9) * 64 + 50 - x] = work->canvas[0x54cf + y * 19 + x];
        }
    }
    for (y = 0; y != 64; y++) {
        for (x = 0; x != 20; x++) {
            work->canvas[0x2700 + y * 64 + 12 + x] = work->canvas[0x58e4 + y * 20 + x];
            work->canvas[0x2700 + y * 64 + 51 - x] = work->canvas[0x58e4 + y * 20 + x];
        }
    }
    if (mode == 3)
        Iwram_CopyWords((void *)0x05000000, Resource_GetTableEntry((s32)&ResourceId_OrangePaletteB), 128);
    else if (mode == 2)
        Iwram_CopyWords((void *)0x05000000, Resource_GetTableEntry((s32)&ResourceId_FireballSheet), 128);
    else
        Iwram_CopyWords((void *)0x05000000, Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSheet), 128);
    EffectPosition_ApplyAlternateStepAndYOffset(effect->actors[0], &pos);
    REG_BG2PA = 0x100;
    REG_BG2X = (64 - pos.x) << 8;
    Func_081963ec(104, 19);
    work->transfer_mode = 2;
    work->transfer_value = 50;
    Scheduler_AddOrUpdateCallback((s32)Func_08143000, 0xc80);
    if (mode == 2) {
        work->shake_frames = 0;
        Audio_PlayCue(212);
    } else if (mode == 1 || mode == 3) {
        work->shake_frames = 8;
        Audio_PlayCue(212);
    } else {
        work->shake_frames = 32;
    }
    for (frame = 0; frame != 48; frame++) {
        if (frame == 0) {
            if (mode == 2)
                Func_0814cd48(effect->actors[0], 7, -1, 0, 32);
            else
                Func_0814cd48(effect->actors[0], 10, -1, 0, 32);
        }
        if (frame == 24)
            Func_081180e8(0);
        if (frame == 8 && mode == 0)
            Audio_PlayCue(126);
        if (frame < 32) {
            cell = frame / 4;
            if (cell > 2)
                cell = (cell & 1) + 1;
            if (frame < 28) {
                vertices = Runtime_BumpAllocateAlternatePool(32);
                model = Func_081969f8(1);
                sprite.bits = (sprite.bits & ~0xff) | 6;
                sprite.bits = (sprite.bits & ~0xff00) | 0x600;
                sprite.pixels = work->canvas + (cell << 12) + 0x700;
                model->kind = 6;
                model->mesh = Data_08199340;
                model->source = &sprite;
                model->unknown_14 = Data_081994e0[mode];
                model->vertices = vertices;
                Render_ResetTransformState();
                Func_08015160(0, (pos.y - 58) << 16, 0);
                Func_0801521c(Data_081994d0[mode] << 1);
                Func_08196958(Data_081991f0, vertices, 4);
                Func_08196a7c(model);
                MemFree(model);
                MemFree(vertices);
            }
        }
        if (mode == 0)
            Func_08158ce0(4, 4);
        else
            Func_08158ce0(16, 16);
        Func_081434f8();
        work->transfer_pending = 1;
        WaitFrames(1);
    }
    Scheduler_RemoveCallback((s32)Func_08143000);
    Runtime_ReleaseHeapBlock(104);
    Func_08143bb8();
}
