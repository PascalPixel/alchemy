#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "GLOBAL_CELLS.H"
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "DMA.H"
#include "IO_REG.H"
#include "IWRAM_CALL.H"
#include "FIXED_POINT_POSITION.H"


#define PALETTE ((volatile u16 *)0x05000000)

s32 Math_DivideUnsigned();



u8 *BabiFune_GetEntrances(void)
{
    return BabiFune_SceneTableA;
}

s32 BabiFune_GetDefaultResult(void)
{
    return 0;
}

u8 *BabiFune_GetExits(void)
{
    return BabiFune_SceneTableB;
}

u8 *BabiFune_GetPlacements(void)
{
    return BabiFune_SceneTableC;
}

void PlayWorkspaceCueAndClearPaletteZero(void)
{
    Engine_EventRequestExit(gEventWork->touched_trigger);
    /* FAKEMATCH: removing this one-pass block changes instruction scheduling. */
    do {
        u16 color = PALETTE == 0;
        register volatile u16 *palette = PALETTE;

        palette[color] = color;
    } while (0);
}

void BabiFune_LoadTablesAndStopDma(void)
{
    SceneEffect_LoadTablesAndStopDma0();
}

void FieldScene_ConfigureFixedPointValues(void)
{
    DisplayScroll_InitializeHBlankDma(0, 0x40000, 0x10000, 0x2000, 0x10000, 0x8000, 0x4000);
}

u8 *BabiFune_GetEvents(void) { return BabiFune_SceneTableD; }

void BabiFune_CyclePalette(void)
{
    u16 *frame;
    s32 v;

    frame = &BabiFune_PaletteStep;
    v = Math_DivideUnsigned(*frame, 6);
    Dma_Set((const void *)(((u32)(v << 16) >> 15) + (u32)BabiFune_PaletteFrames), (void *)0x050000e8, DMA_ENABLE32 | 6, REG_DMA3);
    {
        /* FAKEMATCH: n is a fresh scope so the store precedes the test. */
        s32 n = *frame + 1;

        *frame = n;
        if ((u32)(n << 16) > 0x230000)
            *frame = 0;
    }
}

void BabiFune_UpdateDriftingObject(u8 *obj)
{
    u16 *wait = &((struct FieldActor *)obj)->timer;
    s32 left = *wait;

    if (*(s16 *)wait != 0) {
        s32 drift;

        *wait = left - 1;
        drift = Engine_RandomNext();
        drift -= Engine_RandomNext();
        ((struct FieldActor *)obj)->x.fixed += drift;
        ((struct FieldActor *)obj)->y.fixed += 0xcccc;
    } else if (*(s16 *)&((struct FieldActor *)obj)->work != 0) {
        *(s16 *)&((struct FieldActor *)obj)->work = 0;
        Object_SetMode(obj, 1);
        {
            u16 *p = (u16 *)(obj + 94);
            s32 delay = 20;

            *p = delay;
        }
        Engine_ObjectSetScript(obj, BabiFune_DriftScript);
    }
}

void SceneState_CountDownEveryFortyTicks(void)
{
    s32 n = BabiFune_CountTicks + 1;

    BabiFune_CountTicks = n;
    if (n == 40) {
        if (BabiFune_Count > 4) {
            BabiFune_Count -= 1;
            BabiFune_CountTicks = 0;
        }
    }
}

struct WaveMapLayer {
    s32 unknown_00[3];
    s32 y;
    s32 unknown_10[8];
};

struct WaveMap {
    u8 unknown_00[20];
    struct WaveMapLayer layers[8];
};



void BabiFune_UpdateWaves(void)
{
    volatile u16 blend;
    struct FixedPointPosition place;
    struct FixedPointPosition *pos;
    struct WaveMap *map;
    struct FieldActor *actor;
    s32 swell;
    s32 x, z;

    map = gMapWork[0];
    if (BabiFune_ShimmerActive != 0) {
        swell = Iwram_MulQ16(Engine_MathSin(BabiFune_ShimmerPhase << 9), 3);
        blend = BabiFune_Count + ((swell + 8) << 8);
        {
            u32 value = blend;

            /* FAKEMATCH: outside a one-pass block the register's address is formed before the halfword is read back. */
            do {
                REG_BLDALPHA = value;
            } while (0);
        }
        BabiFune_ShimmerPhase++;
    }
    if (BabiFune_SwellActive != 0) {
        swell = Iwram_MulQ16(Engine_MathSin(BabiFune_SwellPhase << 9), 2) << 16;
        map->layers[6].y = BabiFune_SwellLayerSixY + swell;
        map->layers[7].y = BabiFune_SwellLayerSevenY + swell;
        if (BabiFune_StoredSlot0 != -0x10000) {
            actor = Object_GetById(0);
            actor->y.fixed = BabiFune_StoredSlot0 + swell;
            actor->terrain_height = BabiFune_StoredSlot0 + swell;
            actor->motion_flags = 0;
        }
        if (BabiFune_StoredRecord1 != -0x10000) {
            actor = Object_GetById(1);
            actor->y.fixed = BabiFune_StoredRecord1 + swell;
            actor->terrain_height = BabiFune_StoredSlot0 + swell;
            actor->motion_flags = 0;
        }
        if (BabiFune_StoredSlot3 != -0x10000) {
            actor = Object_GetById(3);
            actor->y.fixed = BabiFune_StoredSlot3 + swell;
            actor->terrain_height = BabiFune_StoredSlot0 + swell;
            actor->motion_flags = 0;
        }
        if (BabiFune_StoredRecord2 != -0x10000) {
            actor = Object_GetById(2);
            actor->y.fixed = BabiFune_StoredRecord2 + swell;
            actor->terrain_height = BabiFune_StoredSlot0 + swell;
            actor->motion_flags = 0;
        }
        BabiFune_SwellPhase++;
    }
    if (BabiFune_DriftActive != 0 && (gFrameCount & 1) != 0) {
        x = map->layers[4].unknown_10[0] & -0x10000;
        z = map->layers[4].unknown_10[1] & -0x10000;
        x += Random_Next() * 240;
        pos = &place;
        pos->x = x;
        pos->y = 0;
        z += Random_Next() * 160;
        z += 0x1e0000;
        pos->z = z;
        actor = Object_Create(0x1f7, pos->x, pos->y, pos->z);
        if (actor != 0) {
            actor->update = (void (*)(union FieldObject *))BabiFune_UpdateDriftingObject;
            actor->timer = 60;
            actor->work = 1;
            actor->motion_flags = 0;
            actor->priority_flags = 2;
            actor->sprite->priority = 2;
            Engine_ActorSetSpriteFlags(actor, OBJECT_BLEND_NORMAL);
            Object_SetMode(actor, 0);
        }
    }
}
