#include "RUNTIME_MEM.H"
#include "MAPCOPY.H"
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "SCENE.H"
#include "CALL.H"
#include "DMA.H"
#include "MAP.H"

struct TileRun {
    s16 id;
    s16 x;
    s16 y;
    s16 vertical;
    s16 unused08;
    s16 unused0a;
};

struct Cell {
    u8 unk0;
    u8 unk1;
    u8 kind;
    u8 type;
};

extern s16 *gOv;
extern s16 *gOv2;
extern u16 *gOv3;
extern u8 gUnk[];
u32 Random16Far(void);
s32 Math_DivideSigned();

/* The board's records and its layout in the scene state, where the board
   setup points; the layout is 0 or 1. */
extern s16 *gKorimaMagariRecords;
extern s16 *gKorimaMagariLayout;
void State_StampRecordCells(s16 *records, s32 value);
void State_ApplyRectByLayoutSelector(void);
extern u8 gBgScroll[];
extern u32 KorimaMagari_ShakeScroll[];
extern u32 KorimaMagari_ShakeChance;
extern u8 MsgFieldFlippedSwitch[];
void State_CopyPresetA0d0WithOffsetB0(void);
void State_UpdateScrollRegistersWithPreset(void);
void KorimaMagari_DrawPanel();
void Scene_RepaintBoardRecords();
void Engine_TaskWait();
void Engine_EventBegin();
void Engine_MessageShowCentered();
void Engine_MapCopyCells();
void Engine_CameraSetSpeed();
void Engine_CameraMoveTo();
void Engine_CameraWaitForMove();
void Engine_ActorSetPosition();
void Engine_AudioPlayCue();
void Engine_ActorSetAnimation();
void Runtime_SetIrqHandler();
void Engine_MapRedraw();
void Engine_EventEnd();
extern u16 *gKorimaMagariReturned;
void Engine_EventWait();
void Engine_ActorSetSpeed();
void Engine_ActorSetDestination();
void Engine_ActorSetDestinationOffset();
void Engine_ActorWaitForMove();
void Engine_MapCopyCells(s32 src_x, s32 src_y, s32 width, s32 height, s32 dest_x, s32 dest_y);
void KorimaMagari_PlaceObjects();
void Scene_RepaintBoardRecords(void);
void Effect_AdjustPaletteColors(s32 amount);
extern u16 KorimaMagari_DefaultRecords[];
s32 Map_GetTerrainHeightFar(s32 layer, s32 x, s32 z);

/* An object placed at a cell, turned along one axis or the other. */
struct IcePlacement {
    s16 type;
    s16 x;
    s16 z;
    s16 turned;
    struct FieldActor *actor;
};

/* The map's cells, 128 to a row. */
extern struct MapCell gMapCellBuffer[];

/* A block on the ice: the four cells it covers from (x, z), across or down,
   and the object that draws it. */
struct TileRun2 {
    s16 id;
    s16 x;
    s16 z;
    s16 vertical;
    struct FieldActor *object;
};

/* By the leader's facing, in quarter turns: the push animation and the
   leader's step after the block. */
extern const u8 KorimaMagari_PushAnimations[];
extern const s8 KorimaMagari_PushStepX[];
extern const s8 KorimaMagari_PushStepZ[];
s32 State_CheckFourCellRun(s32 x, s32 z, s32 mode);
void Vector_AddPolarOffset(s32 distance, s32 angle, s32 *point);
void ObjectDispatch_InitFromTable4WithArgument(s32 table, struct FieldActor *object);
void Object_SetPosition(struct FieldActor *object, s32 x, s32 y, s32 z);
void Object_CommitPosition(struct FieldActor *object);
void KorimaPalette_SaveFirst(void);
void KorimaPalette_SaveSecond(void);
void KorimaPalette_Capture(void);
u16 Effect_AdjustColorChannels(u16 color, s32 adj);

const struct TileRun *SceneData_FindTileRunAt( const struct TileRun *run, s32 x, s32 y);

/* Old-style declarations: interfaces vary by call site across this overlay. */

  /* Place a fixture, first bank: (x, y, w, h, sx, sy). */

  /* Place a fixture, second bank. */

  /* Set object motion state. */

/*
 * One symbol per call site, named at the site's pc-relative-decoded address.
 * All three reach the same ARM-mode IWRAM helper that scales a channel by the
 * adjustment, and each still needs its own name.
 */

/* 0x02000efa */

/* 0x02000f08 */
void State_ApplyRectByLayoutSelector(void)
{
    if (**(s16 **)0x020092c4 == 1) {
        s32 fifth = 4;
        s32 sixth = 9;
        Map_CopyCellAttributeRect(0, 0, 1, 4, fifth, sixth);
    } else {
        s32 fifth = 6;
        s32 sixth = 9;
        Map_CopyCellAttributeRect(0, 0, 1, 4, fifth, sixth);
    }
}

/* Repaint the board for its layout: its cells and attributes, then each
   record's piece, raised with its marker drawn in layout 1 and lowered in
   the other, and the record cells stamped.
   FAKEMATCH: forced temporaries; each copy's destination is set in a block
   of its own so it is built after the other arguments, and the zero the
   pieces take is held from before the loop. */
void Scene_RepaintBoardRecords(void)
{
    s32 zero;
    s16 *record = gKorimaMagariRecords;

    if (*gKorimaMagariLayout != 0) {
        { s32 f1 = 79; s32 g1 = 29; Engine_MapCopyCells(65, 53, 2, 1, f1, g1); }
        { s32 f2 = 15; s32 g2 = 28; Engine_MapCopyCells(65, 40, 2, 4, f2, g2); }
    } else {
        { s32 f3 = 79; s32 g3 = 25; Engine_MapCopyCells(65, 50, 2, 5, f3, g3); }
    }

    if (*gKorimaMagariLayout != 0) {
        { s32 f4 = 32; s32 g4 = 0; Engine_MapCopyCells(0, 32, 32, 32, f4, g4); }
        { s32 f5 = 64; s32 g5 = 0; Engine_MapCopyCells(32, 32, 32, 32, f5, g5); }
        { s32 f6 = 0; s32 g6 = 0; Map_CopyCellAttributeRect(0, 32, 32, 32, f6, g6); }
    } else {
        { s32 f7 = 32; s32 g7 = 0; Engine_MapCopyCells(0, 64, 32, 32, f7, g7); }
        { s32 f8 = 64; s32 g8 = 0; Engine_MapCopyCells(32, 64, 32, 32, f8, g8); }
        { s32 f9 = 0; s32 g9 = 0; Map_CopyCellAttributeRect(0, 64, 32, 32, f9, g9); }
    }

    if (record[0] != -1) {
        zero = 0;
        do {
            u8 *piece = *(u8 **)(record + 4);

            if (*gKorimaMagariLayout == 1) {
                Object_SetMode((struct FieldActor *)piece, 4);
                piece[35] = 3;
                piece[85] = zero;
                *(s32 *)(piece + 12) = 0x1a0000;

                if (record[3] != 0) {
                    s32 col = record[1];
                    s32 row = record[2];
                    Engine_MapCopyCells(68, 40, 1, 4, col + 32, row);
                } else {
                    s32 col = record[1];
                    s32 row = record[2];
                    Engine_MapCopyCells(70, 40, 4, 1, col + 32, row);
                }
            } else {
                Object_SetMode((struct FieldActor *)piece, 1);
                piece[35] = 1;
                piece[85] = 2;
                *(s32 *)(piece + 12) = zero;
            }
            record += 6;
        } while (record[0] != -1);
    }

    { s32 f10 = 10; s32 g10 = 50; Engine_MapCopyCells(70, 42, 1, 1, f10, g10); }

    if (*gKorimaMagariLayout == 1) {
        { s32 f11 = 0; s32 g11 = 0; Map_CopyCellAttributeRect(0, 32, 32, 32, f11, g11); }
        State_StampRecordCells(gKorimaMagariRecords, 254);
    } else {
        { s32 f12 = 0; s32 g12 = 0; Map_CopyCellAttributeRect(0, 64, 32, 32, f12, g12); }
        State_StampRecordCells(gKorimaMagariRecords, 255);
    }

    State_ApplyRectByLayoutSelector();
}

/* Old-style declarations: interfaces vary by call site across this overlay. */

  /* Place a fixture, first bank: (x, y, w, h, sx, sy). */

  /* Place a fixture, second bank. */

  /* Set object motion state. */

/*
 * One symbol per call site, named at the site's pc-relative-decoded address.
 * All three reach the same ARM-mode IWRAM helper that scales a channel by the
 * adjustment, and each still needs its own name.
 */

/* 0x02000efa */

/* 0x02000f08 */
void State_UpdateScrollRegistersWithPreset(void)
{
    u16 line;
    u32 *source;
    volatile u32 *destination;

    line = *(volatile u16 *)0x04000006;
    source = (u32 *)(gBgScroll + 4);
    destination = (volatile u32 *)0x04000014;

    if (line == 227 || line <= 52) {
        if (((Random16Far() * 100) >> 16) < KorimaMagari_ShakeChance) {
            source = KorimaMagari_ShakeScroll;
        }
    }

    *destination = *source++;
    destination = (volatile u32 *)0x04000018;
    *destination++ = *source++;
    *destination = *source;
}

void State_CopyPresetA0d0WithOffsetB0(void)
{
    u32 *dst;
    const u32 *src;
    u16 *p;

    src = (const u32 *)(gBgScroll + 4);
    dst = KorimaMagari_ShakeScroll;
    *dst++ = *src++;
    *dst++ = *src++;
    *dst = *src;
    p = (u16 *)KorimaMagari_ShakeScroll;
    p[1] += 0xb0;
    p[3] += 0xb0;
    p[5] += 0xb0;
}

void Scene_RunKorimaMagariSequence(void)
{
    u32 i;
    s32 record;
    s32 v5;
    s32 base5_200a0dc;
    s32 v3;

    Engine_EventBegin();
    Engine_CameraSetSpeed(0x10000, 0x2000);
    Engine_CameraMoveTo(0x1080000, -1, 0x1c00000, 1);
    Engine_CameraWaitForMove();
    Engine_MessageShowCentered((s32)MsgFieldFlippedSwitch, 1);
    Engine_AudioPlayCue(232);
    if (*gKorimaMagariLayout != 0) {
    } else {
        Call3(Engine_ActorSetPosition, 9, 0x1000000, 0x1ce0000);
        Engine_MapCopyCells(77, 34, 1, 2, 83, 25);
        Engine_TaskWait(3);
        Engine_MapCopyCells(78, 34, 1, 2, 83, 25);
        Engine_TaskWait(3);
        Engine_MapCopyCells(79, 34, 1, 2, 83, 25);
        v5 = 79;
        Engine_TaskWait(30);
        Engine_MapCopyCells(67, 34, 2, 5, v5, 25);
        Engine_TaskWait(6);
        Engine_MapCopyCells(69, 34, 2, 5, v5, 25);
        Engine_ActorSetAnimation(9, 1);
        Engine_AudioPlayCue(240);
        Engine_TaskWait(6);
        Engine_MapCopyCells(71, 34, 2, 5, v5, 25);
        Engine_TaskWait(6);
        Engine_MapCopyCells(73, 34, 2, 5, v5, 25);
        Engine_MapCopyCells(75, 38, 2, 1, v5, 29);
        Engine_TaskWait(4);
        Engine_MapCopyCells(77, 38, 2, 1, v5, 29);
        Engine_TaskWait(6);
        Engine_MapCopyCells(79, 38, 2, 1, v5, 29);
        Engine_TaskWait(8);
        Engine_MapCopyCells(65, 53, 2, 1, v5, 29);
        Call6(Engine_MapCopyCells, 65, 40, 2, 4, 15, 28);
        goto L_02000626;
    }
    Call3(Engine_ActorSetPosition, 9, 0x1000000, 0x1e00000);
    Engine_MapCopyCells(78, 34, 1, 2, 83, 25);
    Engine_TaskWait(3);
    Engine_MapCopyCells(77, 34, 1, 2, 83, 25);
    Engine_TaskWait(3);
    Engine_MapCopyCells(76, 34, 1, 2, 83, 25);
    Engine_TaskWait(30);
    Call6(Engine_MapCopyCells, 65, 45, 2, 4, 15, 28);
    Engine_MapCopyCells(71, 50, 2, 5, 79, 25);
    Engine_ActorSetAnimation(9, 2);
    Engine_AudioPlayCue(230);
    Engine_TaskWait(6);
    Engine_MapCopyCells(69, 50, 2, 5, 79, 25);
    Engine_TaskWait(6);
    Engine_MapCopyCells(67, 50, 2, 5, 79, 25);
    Engine_TaskWait(6);
    Engine_MapCopyCells(65, 50, 2, 5, 79, 25);
    Engine_TaskWait(30);
    L_02000626:;
    if (*gKorimaMagariLayout == 0) {
        KorimaMagari_DrawPanel(9, 19, 16, 5, *gKorimaMagariLayout, 9, 30);
        KorimaMagari_DrawPanel(9, 51, 16, 5, 1, 9, 30);
        KorimaMagari_DrawPanel(41, 51, 16, 5, 2, 9, 30);
    } else {
        KorimaMagari_DrawPanel(9, 19, 16, 5, 0, 9, 30);
        KorimaMagari_DrawPanel(9, 83, 16, 5, 1, 9, 30);
        KorimaMagari_DrawPanel(41, 83, 16, 5, 2, 9, 30);
    }

    KorimaMagari_ShakeChance = 0;
    ((void (*)())Engine_TaskAddCallback)((s32)State_CopyPresetA0d0WithOffsetB0, 0xc80);
    Engine_TaskWait(1);
    Runtime_SetIrqHandler(1, 0, State_UpdateScrollRegistersWithPreset);
    /* FAKEMATCH: the do/while (0) wrap keeps the flag and the counter
     * address in r6 and r5. */
    do {
        Engine_AudioPlayCue(231);
    } while (0);
    KorimaMagari_ShakeChance = 0;
    do {
        Engine_TaskWait(1);
        v3 = (KorimaMagari_ShakeChance + 1);
        KorimaMagari_ShakeChance += 1;
    } while (v3 <= 100);
    Engine_AudioPlayCue(0x121);
    if (*gKorimaMagariLayout == 0) {
        KorimaMagari_DrawPanel(9, 19, 16, 5, *gKorimaMagariLayout, 9, 19);
        KorimaMagari_DrawPanel(9, 51, 16, 5, 1, 9, 19);
        KorimaMagari_DrawPanel(41, 51, 16, 5, 2, 9, 19);
    } else {
        KorimaMagari_DrawPanel(9, 19, 16, 5, 0, 9, 19);
        KorimaMagari_DrawPanel(9, 83, 16, 5, 1, 9, 19);
        KorimaMagari_DrawPanel(41, 83, 16, 5, 2, 9, 19);
    }
    Engine_TaskWait(1);
    Runtime_SetIrqHandler(1, 0, 0);
    Engine_TaskWait(1);
    ((void (*)())Engine_TaskRemoveCallback)((s32)State_CopyPresetA0d0WithOffsetB0);
    *(u16 *)gKorimaMagariLayout ^= 1;
    Scene_RepaintBoardRecords();
    Engine_MapRedraw();
    Engine_EventEnd();
}

void KorimaMagari_RunReturnSequence(void)
{
    s32 v5;

    Engine_EventBegin();
    Engine_ActorSetAnimation(0, 8);
    Engine_EventWait(6);
    Engine_AudioPlayCue(239);
    Call3(Engine_ActorSetSpeed, 8, 0x8000, 0x3333);
    Engine_ActorSetAnimation(8, 2);
    Engine_ActorSetDestination(8, 72, 176);
    Engine_EventWait(6);
    Engine_ActorSetAnimation(0, 2);
    Call3(Engine_ActorSetSpeed, 0, 0x4ccc, 0x3333);
    Engine_ActorSetDestinationOffset(0, -8, 0);
    Engine_EventWait(24);
    Engine_ActorSetAnimation(0, 1);
    Engine_ActorWaitForMove(8);
    Engine_ActorSetAnimation(8, 1);
    Engine_AudioPlayCue(0x120);
    v5 = 9;
    Engine_AudioPlayCue(213);
    Map_CopyCellAttributeRect(5, 9, 1, 4, 6, v5);
    Map_CopyCellAttributeRect(0, 0, 1, 4, 4, v5);
    *gKorimaMagariReturned = 1;
    Engine_EventEnd();
}

void KorimaMagari_RunDepartSequence(void)
{
    s32 v5;

    Engine_EventBegin();
    Engine_ActorSetAnimation(0, 8);
    Engine_EventWait(6);
    Engine_AudioPlayCue(239);
    Call3(Engine_ActorSetSpeed, 8, 0x8000, 0x3333);
    Engine_ActorSetAnimation(8, 2);
    Engine_ActorSetDestination(8, 104, 176);
    Engine_EventWait(6);
    Engine_ActorSetAnimation(0, 2);
    Call3(Engine_ActorSetSpeed, 0, 0x4ccc, 0x3333);
    Engine_ActorSetDestinationOffset(0, 8, 0);
    Engine_EventWait(24);
    Engine_ActorSetAnimation(0, 1);
    Engine_ActorWaitForMove(8);
    Engine_ActorSetAnimation(8, 1);
    Engine_AudioPlayCue(0x120);
    v5 = 9;
    Engine_AudioPlayCue(213);
    Map_CopyCellAttributeRect(5, 9, 1, 4, 4, v5);
    Map_CopyCellAttributeRect(0, 0, 1, 4, 6, v5);
    *gKorimaMagariReturned = 0;
    Engine_EventEnd();
}

/* Set up the board: point the three state cells into the scene state, copy
 * the map cells, reset the board records unless flag 0x109 is set, place the
 * objects and actors, and fade the palette unless flag 0x845 is set. */
s32 KorimaMagari_SetupBoard(void)
{
    struct FieldActor *actor;

    /* FAKEMATCH: the one-pass loop keeps the third cell store ahead of the
     * shared zero. */
    do {
        gKorimaMagariReturned = (u16 *)gSceneState;
        (*(u16 * *)&gKorimaMagariLayout) = (u16 *)gSceneState + 1;
        (*(u16 * *)&gKorimaMagariRecords) = (u16 *)gSceneState + 2;
    } while (0);
    Engine_MapCopyCells(32, 0, 64, 32, 0, 64);
    Map_CopyCellAttributeRect(0, 0, 32, 32, 0, 64);
    Map_CopyCellAttributeRect(32, 0, 32, 32, 0, 32);
    if (Engine_GameFlagIsSet(0x109) == 0) {
        Dma_Set(KorimaMagari_DefaultRecords, ((u16 *)gKorimaMagariRecords), 0x84000012, (volatile u32 *)0x040000d4);
        *gKorimaMagariReturned = 0;
        *(*(u16 * *)&gKorimaMagariLayout) = 1;
    }
    KorimaMagari_PlaceObjects(((u16 *)gKorimaMagariRecords));
    ((s32 (*)(u16 *, s32))State_StampRecordCells)(KorimaMagari_DefaultRecords, 255);
    Scene_RepaintBoardRecords();
    Engine_ActorSetAnimation(9, 0);
    Object_GetById(9)->motion_flags = 0;
    actor = Object_GetById(10);
    actor->radius = 8;
    actor->scale_x = 0xc000;
    actor->scale_y = 0xc000;
    gEventWork->start_transition = 0x204;
    if (Engine_GameFlagIsSet(0x845) == 0)
        Effect_AdjustPaletteColors(4);
    return 0;
}

/* Create each listed object centred on its cell and standing on the terrain;
 * the list ends at type -1. */
void KorimaMagari_PlaceObjects(struct IcePlacement *entry)
{
    struct FieldActor *actor;
    s32 height;
    s32 x;
    s32 z;

    for (; entry->type != -1; entry++) {
        if (entry->turned == 0) {
            x = (entry->x << 20) + 0x200000;
            z = (entry->z << 20) + 0x80000;
        } else {
            x = (entry->x << 20) + 0x80000;
            z = (entry->z << 20) + 0x200000;
        }
        actor = Engine_ObjectCreate(entry->type, x, 0, z);
        if (actor == NULL)
            return;
        entry->actor = actor;
        Object_SetMode(actor, 1);
        Engine_ActorSetSpriteFlags(actor, 0);
        actor->collision_flags = 0;
        actor->radius = 32;
        height = Map_GetTerrainHeightFar(0, actor->x.part.pixel, actor->z.part.pixel) << 16;
        /* FAKEMATCH: y is updated through an s32 lvalue; the coordinate
         * union has an s16 member, which would order the next entry load
         * after this store. */
        *(s32 *)&actor->y += height;
        *(s32 *)actor->unknown_14 = height;
    }
}

/* Old-style declarations: interfaces vary by call site across this overlay. */

  /* Place a fixture, first bank: (x, y, w, h, sx, sy). */

  /* Place a fixture, second bank. */

  /* Set object motion state. */

/*
 * One symbol per call site, named at the site's pc-relative-decoded address.
 * All three reach the same ARM-mode IWRAM helper that scales a channel by the
 * adjustment, and each still needs its own name.
 */

/* 0x02000efa */

/* 0x02000f08 */
void State_StampRecordCells(s16 *records, s32 value)
{

    s16 *record = records;
    if (record[0] == -1) return;
    do {
        s32 column = record[1];
        s32 row = record[2];
        s32 along = record[3];
        s32 i;
        for (i = 3; i >= 0; i--) {
            u8 *cell;
            cell = ((u8 *)gMapCellBuffer) + ((column + (row << 7)) << 2);
            cell[2] = (u8)value;
            if (along == 0) column++;
            else row++;
        }
        record += 6;
    } while (record[0] != -1);
}

const struct TileRun *SceneData_FindTileRunAt(
    const struct TileRun *run,
    s32 x,
    s32 y)
{
    while (run->id != -1) {
        s32 x0 = run->x;
        s32 x1 = x0;
        s32 y0 = run->y;
        s32 y1 = y0;

        if (run->vertical == 0)
            x1 += 3;
        else
            y1 += 3;

        if (x >= x0 && x <= x1 &&
            y >= y0 && y <= y1)
            return run;
        run++;
    }
    return 0;
}

/* Whether the four cells from (x, z) onward, rightward or in any other mode
   downward, are all open: none is unwalkable and none has an attribute
   whose collision entry is set. */
s32 State_CheckFourCellRun(s32 x, s32 z, s32 mode)
{
    s32 i;

    for (i = 0; i <= 3; i++) {
        struct MapCell *cell = &gMapCellBuffer[x + (z << 7)];

        if (cell->collision_code == 0xff
            || *(u8 *)((cell->attribute_b << 2) + (s32)gMapCollision) != 0) {
            return -1;
        }
        if (mode == 0) {
            x++;
        } else {
            z++;
        }
    }
    return 0;
}

/* Push the block in front of the leader: slide it cell by cell, up to eleven
   cells, while the four cells beyond it are open, then play the push with
   the block gliding to where it stopped. The actor is first the leader,
   then the block. */
void Scene_PushBlockAlongRun(struct TileRun2 *runs)
{
    struct FieldActor *actor;
    struct TileRun2 *run;
    s32 point[3];
    s32 facing;
    s32 moved;
    s32 i;
    s32 x;
    s32 z;
    s32 quarter;

    moved = 0;
    actor = Object_GetById(0);
    facing = (actor->facing + 0x2000) & 0xc000;
    point[0] = (actor->x.fixed & 0xfff00000) + 0x80000;
    point[1] = actor->y.fixed;
    point[2] = (actor->z.fixed & 0xfff00000) + 0x80000;
    Vector_AddPolarOffset(0x100000, facing, point);
    run = SceneData_FindTileRunAt(runs, point[0] / 0x100000, point[2] / 0x100000);
    if (run == 0)
        return;
    for (i = 0; i <= 10; i++) {
        point[0] = run->x << 20;
        point[2] = run->z << 20;
        Vector_AddPolarOffset(0x100000, facing, point);
        if (State_CheckFourCellRun(point[0] / 0x100000, point[2] / 0x100000, run->vertical) != 0)
            break;
        moved = 1;
        if (run->vertical == 0) {
            x = point[0] + 0x200000;
            z = point[2] + 0x80000;
        } else {
            x = point[0] + 0x80000;
            z = point[2] + 0x200000;
        }
        run->x = point[0] / 0x100000;
        run->z = point[2] / 0x100000;
    }
    if (moved == 0)
        return;
    point[0] = (actor->x.fixed & 0xfff00000) + 0x80000;
    point[1] = actor->y.fixed;
    point[2] = (actor->z.fixed & 0xfff00000) + 0x80000;
    Vector_AddPolarOffset(0x80000, facing, point);
    actor = run->object;
    quarter = facing / 0x4000;
    Engine_EventBegin();
    Engine_ActorSetAnimation(0, 8);
    Engine_EventWait(6);
    actor->speed = 0x8000;
    actor->acceleration = 0x3333;
    Audio_PlayCue(239);
    Object_SetMode(actor, KorimaMagari_PushAnimations[quarter]);
    Object_SetPosition(actor, x, 0, z);
    Engine_EventWait(6);
    Engine_ActorSetAnimation(0, 2);
    ObjectDispatch_InitFromTable4WithArgument(*(s32 *)(Runtime_AllocateBlock(27, 0xccc) + 0x1e0), actor);
    Actor_SetSpeed(0, 0x4ccc, 0x3333);
    Actor_SetDestinationOffset(0, KorimaMagari_PushStepX[quarter], KorimaMagari_PushStepZ[quarter]);
    Engine_EventWait(24);
    Engine_ActorSetAnimation(0, 1);
    Object_CommitPosition(actor);
    Object_SetMode(actor, 1);
    Audio_PlayCue(0x120);
    Audio_PlayCue(213);
    Engine_EventWait(15);
    Engine_EventEnd();
}

/* Adjust every palette colour but for colours 17 to 23 and 193 to 200 by
   the amount given, keeping the palette before and after, then blend
   towards it. */
void Effect_AdjustPaletteColors(s32 amount)
{
    u32 x;

    KorimaPalette_SaveFirst();
    x = 0;
    do {
        u32 idx = x >> 16;
        if (x + 0xffef0000 > 0x60000 && (idx + 0xff3f) << 16 > 0x70000) {
            u16 *pal = (u16 *)(0x5000000 + idx * 2);
            *pal = Effect_AdjustColorChannels(*pal, amount);
        }
        /* FAKEMATCH: forced temporary; the step is kept in its own
           register and tested before it replaces x, which the plain
           loop condition does not do. */
        {
            u32 nx = x + 0x10000;
            x = nx;
            if (nx > 0xdf0000) {
                break;
            }
        }
    } while (1);
    KorimaPalette_Capture();
    KorimaPalette_SaveSecond();
    Engine_ColorBufferApplyTarget(0x10000, 0);
}

/* Old-style declarations: interfaces vary by call site across this overlay. */

  /* Place a fixture, first bank: (x, y, w, h, sx, sy). */

  /* Place a fixture, second bank. */

  /* Set object motion state. */

/*
 * One symbol per call site, named at the site's pc-relative-decoded address.
 * All three reach the same ARM-mode IWRAM helper that scales a channel by the
 * adjustment, and each still needs its own name.
 */

/* 0x02000eec */

/* 0x02000efa */

/* 0x02000f08 */
u16 Effect_AdjustColorChannels(u16 color, s32 adj)
{
    s16 green = (s16)((color >> 5) & 31);
    s16 red = (s16)(color & 31);
    s16 blue = (s16)((color >> 10) & 31);
    u32 packed;

    red = (s16)(red + Math_DivideSigned(
        red,
        (s32)((u32)adj << 2)
    ));
    green = (s16)(green - Math_DivideSigned(green, adj));
    blue = (s16)(blue - Math_DivideSigned(blue, adj));

    /* Only the increasing channel is explicitly saturated by this owner. */
    if (red > 31)
        red = 31;

    packed = (u32)(s32)red;
    packed |= ((u32)(s32)blue << 10) | ((u32)(s32)green << 5);
    return (u16)packed;
}
