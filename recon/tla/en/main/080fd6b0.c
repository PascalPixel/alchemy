extern u8 Data_03001f2c[];

s32 UiMenu_CreateCursor(void *menu);
s32 PsynergyMenu_InitializeEntryObjects(s32, s32, s32, s32, s32);
struct PsynergyMenuIcon *UiIcon_CreateWithResourceVariant(s32, s32, s32);
void *SideObject_CreateFar(s32, s32, s32, s32, s32, s32);
struct PsynergyMenuIcon *RenderOutput_CreateFromResourceFar(s32, s32, s32, s32, s32);

void PsynergyMenu_DrawPsynergyIcons(u16 *psynergies)
{
    s32 remaining;
    struct PsynergyMenuIcon **icons;
    u16 *p;
    s32 psynergy_id;

    icons =
        (*(struct PsynergyMenuState **)((u32)&Data_03001f2c))->entry_icons;
    p = psynergies;
    remaining = 31;
    do {
        psynergy_id = *p++;
        if (psynergy_id != 0) {
            Resource_LoadByModeIntoSlotFar(
                4, psynergy_id, (*icons)->render_target, 0);
        }
        icons++;
        remaining--;
    } while (remaining >= 0);
    Menu_HideEmptyEntryIcons(psynergies);
}
