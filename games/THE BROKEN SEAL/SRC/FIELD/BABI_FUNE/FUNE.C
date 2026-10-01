#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "DMA.H"

extern u8 BabiFune_SceneTableA[];

#define PALETTE ((volatile u16 *)0x05000000)
extern u8 BabiFune_SceneTableB[];
extern u8 BabiFune_SceneTableC[];
extern u8 BabiFune_SceneTableD[];
void DisplayScroll_InitializeHBlankDma(s32 mode, s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void SceneEffect_LoadTablesAndStopDma0(void);

s32 Math_DivideUnsigned();
extern u16 BabiFune_PaletteStep;
extern const u16 BabiFune_PaletteFrames[];

s32 Engine_RandomNext();
void Object_SetMode();
void Engine_ObjectSetScript();
extern const s32 BabiFune_DriftScript[];

extern s32 BabiFune_CountTicks;
extern s32 BabiFune_Count;

/*
 * Overlay resource_3ca. Exported getter for an in-image table, published
 * from the overlay header.
 */

/*
 * The eight-byte owner at 0x02000030 includes its one pool word, which holds
 * the returned address; the table is returned without being dereferenced.
 */
u8 *SceneData_GetTable96c8(void)
{
    return BabiFune_SceneTableA;
}

/*
 * Babi Fune: the no-hit result the overlay's entry table publishes; the body
 * just returns zero.
 */
s32 BabiFune_GetDefaultResult(void)
{
    return 0;
}

/*
 * Overlay resource_3ca. Exported getters for in-image tables, published
 * from the overlay header. Each eight-byte owner includes its one pool word,
 * which holds the returned address; the table is returned without being
 * dereferenced.
 */
u8 *SceneData_GetTable9710(void)
{
    return BabiFune_SceneTableB;
}

u8 *SceneData_GetTable971c(void)
{
    return BabiFune_SceneTableC;
}

void PlayWorkspaceCueAndClearPaletteZero(void)
{
    Event_RequestExit(gEventWork->touched_trigger);
    do {
        u16 color = PALETTE == 0;
        register volatile u16 *palette = PALETTE;

        palette[color] = color;
    } while (0);
}

void FieldScene_Forward11fc(void)
{
    SceneEffect_LoadTablesAndStopDma0();
}

void FieldScene_ConfigureFixedPointValues(void)
{
    DisplayScroll_InitializeHBlankDma(0, 0x40000, 0x10000, 0x2000, 0x10000, 0x8000, 0x4000);
}

u8 *SceneData_GetTable97AC(void) { return BabiFune_SceneTableD; }

/* Babi Fune: copy six colours of the cycling palette for the current step
 * into palette colours 116..121 and advance the step, wrapping after 35. */
void BabiFune_CyclePalette(void)
{
    u16 *frame;
    s32 v;

    frame = &BabiFune_PaletteStep;
    v = Math_DivideUnsigned(*frame, 6);
    Dma_Set((const void *)(((u32)(v << 16) >> 15) + (u32)BabiFune_PaletteFrames), (void *)0x050000e8, 0x80000006, (volatile u32 *)0x040000d4);
    {
        /* FAKEMATCH: n is a fresh scope so the store precedes the test. */
        s32 n = *frame + 1;

        *frame = n;
        if ((u32)(n << 16) > 0x230000)
            *frame = 0;
    }
}

/* Lemurian ship: while the wait lasts, drift the object randomly sideways
 * and upwards; when it ends and a trigger is pending, start its script. */
void BabiFune_UpdateDriftingObject(u8 *obj)
{
    u16 *wait = (u16 *)(obj + 100);
    s32 left = *wait;

    if (*(s16 *)wait != 0) {
        s32 drift;

        *wait = left - 1;
        drift = Engine_RandomNext();
        drift -= Engine_RandomNext();
        *(s32 *)(obj + 8) += drift;
        *(s32 *)(obj + 12) += 0xcccc;
    } else if (*(s16 *)(obj + 102) != 0) {
        *(s16 *)(obj + 102) = 0;
        Object_SetMode(obj, 1);
        {
            u16 *p = (u16 *)(obj + 94);
            s32 delay = 20;

            *p = delay;
        }
        Engine_ObjectSetScript(obj, BabiFune_DriftScript);
    }
}

/* Every forty ticks, count down while the count is above four. */
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
