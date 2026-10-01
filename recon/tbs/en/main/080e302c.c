/* Draft, complete main:080e302c [080e302c,080e38b8), 2188 bytes, written
   fresh from the listing in plain C; 205 instructions off. The frame (72
   bytes), the set-up, the object and rock seeding, the mound and wall-row
   blocks and the dust block match. Remaining difference:
   1. The tile repack reads each strip width once in the byte loop
      preheader, which is what const tables give; with them const the frame
      loop then keeps table values in registers across the blitter calls
      where the ROM reads them again after every call.
   2. The palette loop: the ROM leaves the 32-bit 31 inside the loop and has
      the pointer in r5 and blue, green, red in r0, r1, r4. The second loop
      pass hoists the 31 here (30 against a loop of 29 instructions).
   3. The standing-wall block: the ROM reads the 48 back before each call
      (its frame loop hoists it: second loop pass, 15 x life 29 = 435
      against 439 instructions here) and so keeps the wall pointer in r6.
   4. The rock block: the ROM copies the unit scale as one struct, keeps
      the scale address in r7 and the slot index out of i, with -1 in r4
      saved around the placement call; trying that moved i from r7 to r6
      everywhere, so the two have to be solved together. */
#include "TYPES.H"
#include "RESOURCE_IDS.H"
#include "RESOURCE.H"
#include "BATTLE_EFX.H"
#include "BATTLE_EFFECT_WORK.H"
#include "EFFECT_STEP.H"
#include "CALLBACK_SCHEDULER.H"
#include "SYSTEM.H"
#include "B5_CONTEXT.H"
#include "IWRAM_CALL.H"
#include "RAM_BUFFER.H"

/* A scene object: two bits of its tenth byte pick its draw variant. */
typedef struct {
    u8 reserved_00[9];
    u8 flags09_0 : 2;
    u8 variant : 2;
    u8 flags09_4 : 4;
    u8 reserved_0a[28];
    u8 enabled;
} BattleEffectObject;

extern u8 gBattleFxWork[];
extern u8 gMapCellBuffer[];
extern DrawRectangle gWorkSlot[];

void BattlePresentation_ProcessPendingGraphicsTransfer(void);
void BattleFx_BeginCanvasLayer(s32 mode);
void BattleFx_EndCanvasLayer(void);
BattleEffectObject *GetBattleEffectObject(s32 kind);
void AnimationObjects_SelectAnimationFar(BattleEffectObject *object, s32 animation);
void Object_ApplyProjectedPlacementFar(BattleEffectObject *object, s32 *position, s32 *scale, s32 mode);
void ResourceObject_ReleaseFar(BattleEffectObject *object);
void Audio_PlayCue(s32 cue);
void BattleEventRuntime_BeginPhaseFar(s32 phase);
void BattleMotion_ApplyVariantMotionFar(s32 actor, s32 variant);
void Camera_ApplyShake(s32 x, s32 y);
void ObjectGroup_TickMemberTimers(void);

extern u16 ParticleStreams_CellOffsets[];
extern s32 Data_080edab0[];
extern u16 Data_080eed7e[];
extern u8 Data_080eed90[];
extern u16 Data_080eed9a[];
extern u8 Data_080eeda0[];
extern u8 Data_080eeda3[];
extern s8 Data_080eeda6[];
extern u8 Data_080eedac[];

/* The eleven scene objects the effect work keeps. */
#define OBJECTS ((BattleEffectObject **)((u8 *)work + 0x77d8))

/* The whole-pixel half of a 16.16 coordinate. */
#define HI(v) (((s16 *)&(v))[1])

/* Battle effect: an earth wall. Two mirrored halves of a mound rise, a
   column of wall rows scrolls up between them and later back down, the full
   wall stands for seventy-two frames while dust falls in front of it, and
   eleven rocks drop along it one after another, each respawning until the
   wall goes. Every affected unit reacts while the wall stands. */
void Unnamed_080e302c(struct BattleEffectArgument *effect)
{
    void **heap_cache;
    void **cursor;
    struct BattleEffectWork *work;
    void *canvas;
    DrawRectangle draw[2];
    void *sheet;
    s32 bias;
    s32 offset;
    s32 scale[2];
    s32 position[4];
    u8 *source;
    u16 *palette;
    s32 written;
    s32 offset_in_work;
    s32 frame;
    s32 i;
    s32 j;
    s32 k;

    heap_cache = (void **)gBattleFxWork;
    cursor = heap_cache;
    work = *cursor++;
    canvas = *cursor;
    sheet = heap_cache[2];
    work->effect = effect;
    BattleFx_BeginCanvasLayer(0);
    if (work->effect->variant == 0)
        *(volatile u16 *)0x04000020 = 204;
    else if (work->effect->variant == 1)
        *(volatile u16 *)0x04000020 = 170;
    if (work->effect->side == 1) {
        bias = 8;
        if (work->effect->variant == 0)
            offset = 40;
        else if (work->effect->variant == 1)
            offset = 36;
        else
            offset = 40;
    } else {
        bias = -16;
        offset = -12;
    }
    *(volatile u32 *)0x04000028 =
        Data_080eeda6[work->effect->side * 3 + work->effect->variant] << 8;
    Resource_LoadAndDecompress((s32)&ResourceId_EarthWallSheet, gMapCellBuffer, 1, 0);
    Iwram_CopyWords((void *)0x05000000,
        Resource_GetTableEntry((s32)&ResourceId_OrangePaletteA), 128);
    palette = (u16 *)0x05000002;
    for (i = 0; i != 63; i++) {
        s32 color = *palette;
        s32 blue = (((u16)color >> 10) & 31) - 8;
        s32 green = (((u16)color >> 5) & 31) - 8;
        s32 red = (31 & color) - 8;

        if (blue < 0)
            blue = 0;
        if (green < 0)
            green = 0;
        if (red < 0)
            red = 0;
        *palette++ = (blue << 10) | (green << 5) | red;
    }
    source = gMapCellBuffer;
    written = 0;
    for (j = 0; j != 9; j++) {
        for (i = 0; i != 32; i++) {
            for (k = 0; k != Data_080eed90[j]; k++)
                ((u8 *)work)[written++] = source[k];
        }
        source += Data_080eed90[j];
    }
    for (j = 0; j != 32; j++) {
        for (i = 0; i != 3; i++) {
            for (k = 0; k != 48; k++)
                ((u8 *)work)[written++] = source[k];
        }
        source += 48;
    }
    for (i = 0; i != 1008; i++)
        ((u8 *)work)[written++] = *source++;
    for (j = 0; j != 3; j++) {
        for (k = 0; k != Data_080eeda3[j] * Data_080eeda0[j]; k++)
            ((u8 *)work)[written++] = *source++;
    }
    Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
    offset_in_work = 0x77d8;
    for (i = 0; i != 11; i++) {
        BattleEffectObject *object = GetBattleEffectObject(390);

        *(BattleEffectObject **)((u8 *)work + offset_in_work) = object;
        if (object != 0) {
            object->enabled = 0;
            AnimationObjects_SelectAnimationFar(object, i / 4);
            (*(BattleEffectObject **)((u8 *)work + offset_in_work))->variant = 1;
        }
        offset_in_work += 4;
    }
    BattleEffect_LoadWork(46, 7, 7, 3, 2);
    draw[0] = gWorkSlot[46];
    BattleEffect_LoadWork(47, 7, 7, 7, 2);
    draw[1] = gWorkSlot[47];
    *(volatile u16 *)0x04000050 = 0x3f46;
    *(volatile u16 *)0x04000052 = 0x1010;
    work->transfer_mode = 2;
    work->transfer_value = 75;
    Scheduler_AddOrUpdateCallback((s32)BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
    for (i = 0; i != 11; i++) {
        struct EffectStep *rock = &work->particles[10 + i];

        rock->x = (Random16() & 15) + 88;
        rock->y = 128;
        rock->z = (Random16() & 15) + 2;
        rock->velocity_x = i;
        rock->velocity_y = 1;
        rock->velocity_z = 0x8000;
        rock->variant = 44 - i * 4;
    }
    for (i = 0; i != 6; i++)
        work->particles[16 + i].x = Data_080eedac[i];
    for (i = 0; i != 256; i++) {
        struct EffectStep *dust = &((struct EffectStep *)Ram_MapCellBuffer)[i];

        dust->x = ((Random16() & 63) + bias + 32) << 16;
        dust->y = ((Random16() & 7) + 96) << 16;
        dust->velocity_y = ((Random16() & 63) + 32) << 13;
        dust->variant = Random16() & 31;
    }
    *(volatile u16 *)0x0400000c = 0x785;
    work->shake_frames = 250;
    for (frame = 0; frame != 192; frame++) {
        if (frame == 0)
            Audio_PlayCue(212);
        if (frame == 40)
            Audio_PlayCue(141);
        if (frame == 96)
            Audio_PlayCue(145);
        if (frame == 120)
            BattleEventRuntime_BeginPhaseFar(134);
        if (frame <= 81) {
            s32 cell = frame / 4;

            if (cell > 2)
                cell = (cell & 1) + 1;
            draw[0](canvas, (u8 *)work + Data_080eed9a[cell], bias - Data_080eeda0[cell] + 64,
                116 - Data_080eeda3[cell], Data_080eeda0[cell], Data_080eeda3[cell]);
            draw[1](canvas, (u8 *)work + Data_080eed9a[cell], bias + 64,
                116 - Data_080eeda3[cell], Data_080eeda0[cell], Data_080eeda3[cell]);
        }
        if (frame >= 12 && frame <= 87) {
            s32 row = (frame - 64) / 3;
            s32 y;

            if (row < 0)
                row = 0;
            if (row > 7)
                row = 7;
            for (i = 0, y = -12; i != 4; i++, y += 32) {
                draw[0](canvas, (u8 *)work + Data_080eed7e[row], bias - Data_080eed90[row] + 64,
                    y, Data_080eed90[row], 32);
                draw[1](canvas, (u8 *)work + Data_080eed7e[row], bias + 64,
                    y, Data_080eed90[row], 32);
            }
        }
        if (frame >= 160 && frame <= 183) {
            s32 row = 7 - (frame - 160) / 3;
            s32 y;

            if (row < 0)
                row = 0;
            if (row > 7)
                row = 7;
            for (i = 0, y = -12; i != 4; i++, y += 32) {
                draw[0](canvas, (u8 *)work + Data_080eed7e[row], bias - Data_080eed90[row] + 64,
                    y, Data_080eed90[row], 32);
                draw[1](canvas, (u8 *)work + Data_080eed7e[row], bias + 64,
                    y, Data_080eed90[row], 32);
            }
        }
        if (frame >= 88 && frame <= 159) {
            u8 *wall = (u8 *)work + 0x13c0;

            draw[0](canvas, wall, bias + 16, 0, 48, 96);
            draw[1](canvas, wall, bias + 64, 0, 48, 96);
            wall = (u8 *)work + 0x25c0;
            draw[0](canvas, wall, bias + 16, 96, 48, 21);
            draw[1](canvas, wall, bias + 64, 96, 48, 21);
        }
        if (frame > 87) {
            for (i = 0; i != 64; i++) {
                struct EffectStep *dust = &((struct EffectStep *)Ram_MapCellBuffer)[i];

                if (dust->variant == 0) {
                    s32 size = (i & 3) + 5;

                    draw[0](canvas, (u8 *)sheet + ParticleStreams_CellOffsets[size - 1],
                        HI(dust->x) - size / 2, HI(dust->y) - size, size, size * 2);
                    dust->y -= dust->velocity_y;
                    if (dust->y < 0 && frame <= 159)
                        dust->y = 0x600000;
                } else {
                    dust->variant--;
                }
            }
        }
        if (frame > 4) {
            s32 rocks = 6;

            if (frame > 71)
                rocks = 11;
            for (k = 0; k != rocks; k++) {
                struct EffectStep *rock = &work->particles[10 + k];

                if (rock->variant == 0) {
                    scale[0] = Data_080edab0[0];
                    scale[1] = Data_080edab0[1];
                    if (frame > 71) {
                        scale[0] = (k << 12) + 0x8000;
                        scale[0] += work->effect->variant << 14;
                    } else {
                        scale[0] = 0x8000;
                    }
                    scale[1] = scale[0];
                    position[3] = 0;
                    position[0] = (rock->x + offset * 2) << 16;
                    position[2] = 0x2000000;
                    position[1] = 0x2000000 - (rock->y << 16);
                    i = (frame / 2 + k) % 11;
                    if (i != -1)
                        Object_ApplyProjectedPlacementFar(OBJECTS[i], position, scale, 0);
                    rock->y -= rock->z;
                    rock->velocity_x += rock->velocity_y;
                    if (rock->velocity_x > 12)
                        rock->velocity_x -= 12;
                    if (rock->y < 0) {
                        if (frame > 159) {
                            rock->variant = -1;
                        } else {
                            if (frame > 87) {
                                rock->z = (Random16() & 7) + 8;
                                if (work->effect->variant == 0)
                                    rock->x = Random16() % 96 + 42;
                                else if (work->effect->variant == 1)
                                    rock->x = Random16() % 112 + 34;
                                else
                                    rock->x = Random16() % 160 + 10;
                            }
                            rock->y = 128;
                            rock->variant = 8;
                        }
                    }
                } else {
                    rock->variant--;
                }
            }
        }
        if (frame <= 158) {
            for (i = 0; i != work->effect->count; i++) {
                if (frame > 85) {
                    if (frame % 12 == 0)
                        ObjectGroup_UpdateMembers(work->effect->actors[i], 7, 5, i, 6);
                    if ((frame & 3) == 0)
                        BattleMotion_ApplyVariantMotionFar(work->effect->actors[i], 5);
                }
            }
        }
        if (frame >= 90 && frame <= 160)
            Camera_ApplyShake(8, 8);
        else
            Camera_ApplyShake(2, 2);
        ObjectGroup_TickMemberTimers();
        work->transfer_pending = 1;
        WaitFrames(1);
    }
    Scheduler_RemoveCallback((u32)BattlePresentation_ProcessPendingGraphicsTransfer);
    Runtime_ReleaseHeapBlock(47);
    Runtime_ReleaseHeapBlock(46);
    for (i = 0; i != 11; i++)
        ResourceObject_ReleaseFar(OBJECTS[i]);
    BattleFx_EndCanvasLayer();
}
