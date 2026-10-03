#include "RUNTIME_MEM.H"
#include "HEAP_STATE.H"
#include "CALLBACK_SCHEDULER.H"
#include "CANVAS.H"
#include "RESOURCE.H"
#include "TYPES.H"
#include "IWRAM_CALL.H"
#include "RESOURCE_IDS.H"
#include "EFFECT_STEP.H"
#include "BATTLE_EFX.H"
#include "BATTLE_EFFECT_WORK.H"

extern u8 gBattleFxWork[];


/* The earth wall's three cells: where each starts in the decoded sheet, and
   its width and height in pixels. */
extern u16 EarthWall_CellSourceOffsets[];
extern u8 EarthWall_CellWidths[];
extern u8 EarthWall_CellHeights[];

typedef s32 (*WordCopy)(void *, const void *, s32);

void AudioCommand_PlayFar(s32 cue);
void ObjectGroup_UpdateMembers(s32 member_id, s32 b, s32 c, s32 d, s32 e);
void BattleEventRuntime_BeginPhaseFar(s32 phase);
void Camera_ApplyShake(s32 x, s32 y);
void ObjectGroup_TickMemberTimers(void);
void WaitFrames(s32 frames);
void EffectPosition_ApplyAlternateStepAndYOffset(
    s32 id, struct EffectPosition *position);

void BattleFx_RunFortyEightFrameEffect(struct BattleEffectArgument *effect, s32 mode);

void BattleFx_FetchRectangleBlitters(s32 alternate,
    BattleEffectDrawRectangle *output)
{
    /* FAKEMATCH: merging the two genuine loading paths shortened this
       helper from 112 to 76 bytes and changed its registers. */
    if (alternate == 0) {
        u8 *state;
        u32 value;

        BattleEffect_LoadWork(alternate = HEAP_SLOT_BLITTER, 7, 7,
            GOUSEI_CLIP_X | GOUSEI_CLIP_Y, GOUSEI_HIKAKU);
        state = (u8 *)gWorkSlot;
        value = *(u32 *)(state + HEAP_SLOT_BLITTER * sizeof(void *));
        alternate = HEAP_SLOT_BLITTER_ALTERNATE;
        output[0] = (BattleEffectDrawRectangle)value;
        BattleEffect_LoadWork(alternate, 7, 7,
            GOUSEI_CLIP_X | GOUSEI_CLIP_Y, GOUSEI_KASAN);
        output[1] = (BattleEffectDrawRectangle)*(u32 *)(state +=
            HEAP_SLOT_BLITTER_ALTERNATE * sizeof(void *));
    } else {
        u8 *state;
        u32 value;

        BattleEffect_LoadWork(alternate = HEAP_SLOT_BLITTER, 7, 7,
            GOUSEI_CLIP_X | GOUSEI_CLIP_Y | GOUSEI_FLIP_X, GOUSEI_HIKAKU);
        state = (u8 *)gWorkSlot;
        value = *(u32 *)(state + HEAP_SLOT_BLITTER * sizeof(void *));
        alternate = HEAP_SLOT_BLITTER_ALTERNATE;
        output[0] = (BattleEffectDrawRectangle)value;
        BattleEffect_LoadWork(alternate, 7, 7,
            GOUSEI_CLIP_X | GOUSEI_CLIP_Y | GOUSEI_FLIP_X, GOUSEI_KASAN);
        output[1] = (BattleEffectDrawRectangle)*(u32 *)(state +=
            HEAP_SLOT_BLITTER_ALTERNATE * sizeof(void *));
    }
}

void BattleFx_RunFortyEightFrameMode1(struct BattleEffectArgument *arg0)
{
    BattleFx_RunFortyEightFrameEffect(arg0, 1);
}

void BattleFx_RunFortyEightFrameMode0(struct BattleEffectArgument *arg0)
{
    BattleFx_RunFortyEightFrameEffect(arg0, 0);
}

void BattleFx_RunFortyEightFrameMode2(struct BattleEffectArgument *arg0)
{
    BattleFx_RunFortyEightFrameEffect(arg0, 2);
}

static __inline__ void CopyPalette(WordCopy copy, void *destination, const void *source, s32 size)
{
    /* FAKEMATCH: the three palette copies share the routine's address but
       build the palette address again at each call, which only an inlined
       constant argument compiles to; a direct call keeps it in a register. */
    copy(destination, source, size);
}

/*
 * The earth wall: a pair of mirrored cells rises beside the target for 28 of
 * the effect's 48 frames while the screen shakes. Mode 0 draws it at full
 * scale over the plain sheet, modes 1 and 2 at 0xcc with a harder shake, and
 * mode 2 also takes the fireball palette and a different member motion.
 */
void BattleFx_RunFortyEightFrameEffect(struct BattleEffectArgument *effect, s32 mode)
{
    void **cursor;
    struct BattleEffectWork *work;
    void *canvas;
    u8 *palette;
    s32 x;
    struct EffectPosition pos;
    DrawRectangle draw_b;
    DrawRectangle draw_a;
    u8 *state;
    s32 frame;
    s32 cell;

    cursor = (void **)gBattleFxWork;
    work = *cursor++;
    canvas = *cursor;
    work->effect = effect;
    BattleFx_BeginCanvasLayer(0);

    palette = Resource_GetTableEntry((s32)&ResourceId_EarthWallSheet);
    CopyPalette(Iwram_CopyWords, (void *)0x05000000, palette, 128);
    palette += 128;
    Resource_DecodeType01(palette, work);
    palette = Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSheet);
    CopyPalette(Iwram_CopyWords, (void *)0x05000000, palette, 128);
    if (mode == 2) {
        palette = Resource_GetTableEntry((s32)&ResourceId_FireballSheet);
        CopyPalette(Iwram_CopyWords, (void *)0x05000000, palette, 128);
    }

    EffectPosition_ApplyAlternateStepAndYOffset(work->effect->actors[0], &pos);
    if (mode == 0) {
        *(s16 *)0x04000020 = 0x100;
        x = 64 - pos.x;
        *(s32 *)0x04000028 = x << 8;
    } else {
        *(s16 *)0x04000020 = 0xcc;
        x = (-pos.x * 4) / 5 + 64;
        *(s32 *)0x04000028 = x << 8;
    }

    BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 3, 2);
    state = (u8 *)gWorkSlot;
    draw_a = *(DrawRectangle *)(state + 184);
    BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 7, 2);
    work->transfer_mode = 2;
    /* FAKEMATCH: reading the second blitter through the slot pointer's own
       variable keeps it in that register between the two transfer stores;
       a direct assignment only orders three instructions differently. */
    state += 188;
    state = *(u8 **)state;
    work->transfer_value = 50;
    draw_b = (DrawRectangle)state;
    Scheduler_AddOrUpdateCallback((s32)BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);

    if (mode == 2) {
        work->shake_frames = 0;
        AudioCommand_PlayFar(212);
    } else if (mode == 1) {
        work->shake_frames = 8;
        AudioCommand_PlayFar(212);
    } else {
        work->shake_frames = 32;
    }

    for (frame = 0; frame != 48; frame++) {
        if (frame == 0) {
            if (mode == 2)
                ObjectGroup_UpdateMembers(work->effect->actors[0], 7, -1, 0, 32);
            else
                ObjectGroup_UpdateMembers(work->effect->actors[0], 10, -1, 0, 32);
        }
        if (frame == 24)
            BattleEventRuntime_BeginPhaseFar(0);
        if (frame == 8 && mode == 0)
            AudioCommand_PlayFar(126);
        if (frame <= 31) {
            cell = frame / 4;
            if (cell > 2)
                cell = (cell & 1) + 1;
            if (frame <= 27) {
                draw_a(canvas,
                    (u8 *)work + EarthWall_CellSourceOffsets[cell],
                    64 - EarthWall_CellWidths[cell],
                    (pos.y - EarthWall_CellHeights[cell]) + 8,
                    EarthWall_CellWidths[cell],
                    EarthWall_CellHeights[cell]);
                draw_b(canvas,
                    (u8 *)work + EarthWall_CellSourceOffsets[cell],
                    64,
                    (pos.y - EarthWall_CellHeights[cell]) + 8,
                    EarthWall_CellWidths[cell],
                    EarthWall_CellHeights[cell]);
            }
        }
        if (mode == 0)
            Camera_ApplyShake(2, 2);
        else
            Camera_ApplyShake(16, 16);
        ObjectGroup_TickMemberTimers();
        work->transfer_pending = 1;
        WaitFrames(1);
    }

    Scheduler_RemoveCallback((u32)BattlePresentation_ProcessPendingGraphicsTransfer);
    Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
    Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
    BattleFx_EndCanvasLayer();
}
