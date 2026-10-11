#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "mapcopy.h"
#include "types.h"
#include "field_event.h"
#include "scene.h"
#include "mem.h"
#include "call.h"
#include "dma.h"
#include "map.h"
#include "io_reg.h"
#include "irq.h"
#include "text/msg_ids.h"

void KorimaMagari_PlaceObjects();
void Scene_RepaintBoardRecords();

#define Scene_PushBlockAlongRun ((void (*)(s16*))Scene_PushBlockAlongRun)

s32 Math_DivideSigned();

u8 *KorimaMagari_GetEntrances(void)
{
    return gKorimaMagariEntrances;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

u8 *KorimaMagari_GetExits(void)
{
    return gKorimaMagariExits;
}

u8 *KorimaMagari_GetPlacements(void)
{
    return gKorimaMagariPlacements;
}

u8 *KorimaMagari_GetEvents(void)
{
    return gKorimaMagariEvents;
}

void KorimaMagari_RedrawBoard(void)
{
    Scene_PushBlockAlongRun(gKorimaMagariRecords);
    Engine_MapCopyCellAttributes(0, 0x40, 0x20, 0x20, 0, 0);
    ((s32 (*)())State_StampRecordCells)((s32)gKorimaMagariRecords, 0xff);
    State_ApplyRectByLayoutSelector();
}

void KorimaMagari_TryJumpForward(void)
{
    Field_TryJumpForward();
}

#undef Scene_PushBlockAlongRun

#define ObjectDispatch_InitFromTable4WithArgument ((void (*)(s32,struct FieldActor*))ObjectDispatch_InitFromTable4WithArgument)

extern u8 gBgScroll[];
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);

struct IcePlacement {
    s16 type;
    s16 x;
    s16 z;
    s16 turned;
    struct FieldActor *actor;
};

extern struct MapCell gMapCellBuffer[];

struct TileRun2 {
    s16 id;
    s16 x;
    s16 z;
    s16 vertical;
    struct FieldActor *object;
};

void Vector_AddPolarOffset(s32 distance, s32 angle, s32 *point);
void Engine_ObjectSetPosition(struct FieldActor *object, s32 x, s32 y, s32 z);
void Object_CommitPosition(struct FieldActor *object);
void KorimaPalette_SaveFirst(void);
void KorimaPalette_SaveSecond(void);
void KorimaPalette_Capture(void);

// ここに共通ソースが入る（ROM の並びのとおり）
#include "../common/map/panel.c"

void State_ApplyRectByLayoutSelector(void)
{
    if (*(s16 *)gKorimaMagariReturned == 1) {
        Map_CopyCellAttributes(0, 0, 1, 4, 4, 9);
    } else {
        Map_CopyCellAttributes(0, 0, 1, 4, 6, 9);
    }
}

void Scene_RepaintBoardRecords(void)
{
    struct FieldActor *piece;
    s16 *record = gKorimaMagariRecords;

    if (*gKorimaMagariLayout != 0) {
        Map_CopyCells(65, 53, 2, 1, 79, 29);
        Map_CopyCells(65, 40, 2, 4, 15, 28);
    } else {
        Map_CopyCells(65, 50, 2, 5, 79, 25);
    }

    if (*gKorimaMagariLayout != 0) {
        Map_CopyCells(0, 32, 32, 32, 32, 0);
        Map_CopyCells(32, 32, 32, 32, 64, 0);
        Map_CopyCellAttributes(0, 32, 32, 32, 0, 0);
    } else {
        Map_CopyCells(0, 64, 32, 32, 32, 0);
        Map_CopyCells(32, 64, 32, 32, 64, 0);
        Map_CopyCellAttributes(0, 64, 32, 32, 0, 0);
    }

    while (record[0] != -1) {
        piece = *(struct FieldActor **)(record + 4);

        if (*gKorimaMagariLayout == 1) {
            Object_SetMode(piece, 4);
            piece->priority_flags = 3;
            piece->motion_flags = 0;
            piece->y.fixed = 0x1a0000;

            if (record[3] != 0) {
                Map_CopyCells(68, 40, 1, 4, record[1] + 32, record[2]);
            } else {
                Map_CopyCells(70, 40, 4, 1, record[1] + 32, record[2]);
            }
        } else {
            Object_SetMode(piece, 1);
            piece->priority_flags = 1;
            piece->motion_flags = 2;
            piece->y.fixed = 0;
        }
        record += 6;
    }

    Map_CopyCells(70, 42, 1, 1, 10, 50);

    if (*gKorimaMagariLayout == 1) {
        Map_CopyCellAttributes(0, 32, 32, 32, 0, 0);
        State_StampRecordCells(gKorimaMagariRecords, 254);
    } else {
        Map_CopyCellAttributes(0, 64, 32, 32, 0, 0);
        State_StampRecordCells(gKorimaMagariRecords, 255);
    }

    State_ApplyRectByLayoutSelector();
}

void State_UpdateScrollRegistersWithPreset(void)
{
    u16 line;
    u32 *source;
    volatile u32 *destination;

    line = REG_VCOUNT;
    source = (u32 *)(gBgScroll + 4);
    destination = (volatile u32 *)0x04000014;

    if (line == 227 || line <= 52) {
        if (((((u32 (*)(void))Engine_RandomNext)() * 100) >> 16) < KorimaMagari_ShakeChance) {
            source = KorimaMagari_ShakeScroll;
        }
    }

    *destination = *source++;
    destination = (volatile u32 *)0x04000018;
    *destination++ = *source++;
    *destination = *source;
}

void KorimaMagari_CaptureShakeScroll(void)
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
        Actor_SetPosition(9, 0x1000000, 0x1ce0000);
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
        Map_CopyCells(65, 40, 2, 4, 15, 28);
        goto L_02000626;
    }
    Actor_SetPosition(9, 0x1000000, 0x1e00000);
    Engine_MapCopyCells(78, 34, 1, 2, 83, 25);
    Engine_TaskWait(3);
    Engine_MapCopyCells(77, 34, 1, 2, 83, 25);
    Engine_TaskWait(3);
    Engine_MapCopyCells(76, 34, 1, 2, 83, 25);
    Engine_TaskWait(30);
    Map_CopyCells(65, 45, 2, 4, 15, 28);
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
        Map_DrawCellPanel(9, 19, 16, 5, *gKorimaMagariLayout, 9, 30);
        Map_DrawCellPanel(9, 51, 16, 5, 1, 9, 30);
        Map_DrawCellPanel(41, 51, 16, 5, 2, 9, 30);
    } else {
        Map_DrawCellPanel(9, 19, 16, 5, 0, 9, 30);
        Map_DrawCellPanel(9, 83, 16, 5, 1, 9, 30);
        Map_DrawCellPanel(41, 83, 16, 5, 2, 9, 30);
    }

    KorimaMagari_ShakeChance = 0;
    ((void (*)())Engine_TaskAddCallback)((s32)KorimaMagari_CaptureShakeScroll, 0xc80);
    Engine_TaskWait(1);
    Runtime_SetIrqHandler(1, 0, State_UpdateScrollRegistersWithPreset);
    /* FAKEMATCH: the do/while (0) wrap keeps the flag and the counter address in r6 and r5. */
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
        Map_DrawCellPanel(9, 19, 16, 5, *gKorimaMagariLayout, 9, 19);
        Map_DrawCellPanel(9, 51, 16, 5, 1, 9, 19);
        Map_DrawCellPanel(41, 51, 16, 5, 2, 9, 19);
    } else {
        Map_DrawCellPanel(9, 19, 16, 5, 0, 9, 19);
        Map_DrawCellPanel(9, 83, 16, 5, 1, 9, 19);
        Map_DrawCellPanel(41, 83, 16, 5, 2, 9, 19);
    }
    Engine_TaskWait(1);
    Runtime_SetIrqHandler(1, 0, 0);
    Engine_TaskWait(1);
    ((void (*)())Engine_TaskRemoveCallback)((s32)KorimaMagari_CaptureShakeScroll);
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
    Actor_SetSpeed(8, 0x8000, 0x3333);
    Engine_ActorSetAnimation(8, 2);
    Engine_ActorSetDestination(8, 72, 176);
    Engine_EventWait(6);
    Engine_ActorSetAnimation(0, 2);
    Actor_SetSpeed(0, 0x4ccc, 0x3333);
    Engine_ActorSetDestinationOffset(0, -8, 0);
    Engine_EventWait(24);
    Engine_ActorSetAnimation(0, 1);
    Engine_ActorWaitForMove(8);
    Engine_ActorSetAnimation(8, 1);
    Engine_AudioPlayCue(0x120);
    v5 = 9;
    Engine_AudioPlayCue(213);
    Engine_MapCopyCellAttributes(5, 9, 1, 4, 6, v5);
    Engine_MapCopyCellAttributes(0, 0, 1, 4, 4, v5);
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
    Actor_SetSpeed(8, 0x8000, 0x3333);
    Engine_ActorSetAnimation(8, 2);
    Engine_ActorSetDestination(8, 104, 176);
    Engine_EventWait(6);
    Engine_ActorSetAnimation(0, 2);
    Actor_SetSpeed(0, 0x4ccc, 0x3333);
    Engine_ActorSetDestinationOffset(0, 8, 0);
    Engine_EventWait(24);
    Engine_ActorSetAnimation(0, 1);
    Engine_ActorWaitForMove(8);
    Engine_ActorSetAnimation(8, 1);
    Engine_AudioPlayCue(0x120);
    v5 = 9;
    Engine_AudioPlayCue(213);
    Engine_MapCopyCellAttributes(5, 9, 1, 4, 4, v5);
    Engine_MapCopyCellAttributes(0, 0, 1, 4, 6, v5);
    *gKorimaMagariReturned = 0;
    Engine_EventEnd();
}

s32 KorimaMagari_SetupBoard(void)
{
    struct FieldActor *actor;

    /* FAKEMATCH: the one-pass loop keeps the third cell store ahead of the shared zero. */
    do {
        gKorimaMagariReturned = (u16 *)gSceneState;
        (*(u16 * *)&gKorimaMagariLayout) = (u16 *)gSceneState + 1;
        (*(u16 * *)&gKorimaMagariRecords) = (u16 *)gSceneState + 2;
    } while (0);
    Engine_MapCopyCells(32, 0, 64, 32, 0, 64);
    Engine_MapCopyCellAttributes(0, 0, 32, 32, 0, 64);
    Engine_MapCopyCellAttributes(32, 0, 32, 32, 0, 32);
    if (Engine_GameFlagIsSet(0x109) == 0) {
        Dma_Set(KorimaMagari_DefaultRecords, ((u16 *)gKorimaMagariRecords), DMA_ENABLE32 | DMA_32BIT | 0x12, REG_DMA3);
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
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
    if (Engine_GameFlagIsSet(0x845) == 0)
        Effect_AdjustPaletteColors(4);
    return 0;
}

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
        height = Map_GetTerrainHeight(0, actor->x.part.pixel, actor->z.part.pixel) << 16;
        /* FAKEMATCH: y is updated through an s32 lvalue; the coordinate union has an s16 member, which would order the next entry load after this store. */
        *(s32 *)&actor->y += height;
        actor->terrain_height = height;
    }
}

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
    Event_PlayCue(239);
    Object_SetMode(actor, KorimaMagari_PushAnimations[quarter]);
    Engine_ObjectSetPosition(actor, x, 0, z);
    Engine_EventWait(6);
    Engine_ActorSetAnimation(0, 2);
    ObjectDispatch_InitFromTable4WithArgument(*(s32 *)(Runtime_AllocateBlock(27, 0xccc) + 0x1e0), actor);
    Actor_SetSpeed(0, 0x4ccc, 0x3333);
    Actor_SetDestinationOffset(0, KorimaMagari_PushStepX[quarter], KorimaMagari_PushStepZ[quarter]);
    Engine_EventWait(24);
    Engine_ActorSetAnimation(0, 1);
    Object_CommitPosition(actor);
    Object_SetMode(actor, 1);
    Event_PlayCue(0x120);
    Event_PlayCue(213);
    Engine_EventWait(15);
    Engine_EventEnd();
}

void Effect_AdjustPaletteColors(s32 amount)
{
    u16 i;

    KorimaPalette_SaveFirst();
    for (i = 0; i < 224; i++) {
        if ((i < 17 || i > 23) && (i < 193 || i > 200)) {
            u16 *pal = (u16 *)(0x5000000 + i * 2);
            *pal = Effect_AdjustColorChannels(*pal, amount);
        }
    }
    KorimaPalette_Capture();
    KorimaPalette_SaveSecond();
    Engine_ColorBufferApplyTarget(0x10000, 0);
}

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

    if (red > 31)
        red = 31;

    packed = (u32)(s32)red;
    packed |= ((u32)(s32)blue << 10) | ((u32)(s32)green << 5);
    return (u16)packed;
}
