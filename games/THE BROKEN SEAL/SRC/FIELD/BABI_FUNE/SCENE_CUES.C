#include "TYPES.H"
#include "FIELD_EVENT.H"

#define PALETTE ((volatile u16 *)0x05000000)

extern u8 BabiFune_SceneTableB[];
extern u8 BabiFune_SceneTableC[];
extern u8 BabiFune_SceneTableD[];

void DisplayScroll_InitializeHBlankDma(s32 mode, s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void SceneEffect_LoadTablesAndStopDma0(void);

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
