/* Draft, not exact (2026-09-24): 143 differing halfwords, 284 of 292 bytes.
   Layout is settled: 52-byte sides at +8, each with a sprite at +32 whose
   OAM starts at +4; count, first, second and other at 0x394..0x39c. The
   OAM bitfield writes merge into one read-modify-write per byte as in the
   ROM. Residual: the reference keeps the graphics pointer in fp from its
   zero initialiser (storing side->more from it) and the sprite pointer in
   sl, which this C folds into constants and recomputes.
   2026-09-29 (alchemy permute scorer): the draft scored 2395; with the
   right-marker tiles as the linked Menu_CursorObjectTiles and the left ones
   as Menu_CursorLeftObjectTiles (the last 0x400 bytes of the UiText_Glyphs scaffold
   block, still unlabelled, one fixed operand) it scores 2530. A 300-second
   search (36,000 candidates) reached this body at 1100 (11 register-only,
   3 operand, 6 reordered, 5 inserted, 1 deleted); the fp/sl pointers above
   are still the difference. The literal tile addresses are gone.
 */
#include "TYPES.H"

extern const u8 Menu_CursorObjectTiles[];
extern const u8 Menu_CursorLeftObjectTiles[];

struct SelectionOam {
    u16 y:8;
    u16 affine:2;
    u16 mode:2;
    u16 mosaic:1;
    u16 color:1;
    u16 shape:2;
    u16 x:9;
    u16 matrix:5;
    u16 size:2;
    u16 tile:10;
    u16 priority:2;
    u16 palette:4;
    u16 unused;
};

struct SelectionSprite {
    u32 link;
    struct SelectionOam oam;
};

struct SelectionSide {
    u16 frame;
    u16 more;
    u16 entry;
    u16 vram;
    u16 x;
    s16 y;
    u8 pad0c[20];
    struct SelectionSprite sprite;
    u8 pad2c[8];
};

struct SelectionMenu {
    u8 pad000[8];
    struct SelectionSide sides[2];
    u8 pad070[0x394 - 0x70];
    u16 count;
    u16 first;
    u16 second;
    u16 pad39a;
    u16 other;
};

s32 Resource_FindFreeEntry(void);
s32 VramBlock_LoadCached(s32 slot, s32 size, const void *source);

void Menu_SetupSelectionSide(struct SelectionMenu *menu, s32 index)
{
    struct SelectionSide *side = &menu->sides[index];
    const void *gfx = 0;
    struct SelectionSprite *sprite = &menu->sides[index].sprite;
    u32 count;

    side->more = (u32)gfx;
    if (0 != index) {
        count = menu->count;
        if (menu->other != 0)
            count -= menu->other;
        if (5 < count) {
            side->more = 1;
            count = (u32)5;
        }
        menu->sides[1].x = menu->first + 16 * (count - 1) + 17;
        gfx = Menu_CursorObjectTiles;
    } else {
        s32 tmp2;
        u16 tmp;
        menu->sides[0].x = menu->first - 9;
        tmp = menu->other;
        gfx = Menu_CursorLeftObjectTiles;
        tmp2 = tmp != 0;
        if (tmp2)
            menu->sides[0].more = 1;
    }
    if (!menu->sides[index].y) {
        menu[0].sides[index].entry = Resource_FindFreeEntry();
        menu->sides[index].vram = VramBlock_LoadCached(menu->sides[index].entry, 128, gfx);
        menu->sides[index].y = menu->second;
        menu->sides[index].frame = (s32)0;
        sprite->oam.mode = 0;
        sprite->oam.mosaic = 0;
        sprite->oam.color = 1;
        sprite->oam.affine = 0;
        sprite->oam.matrix = 0;
        sprite->oam.size = 0;
        sprite->oam.shape = 2;
        sprite->oam.priority = 0;
    }
}
