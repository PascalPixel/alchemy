#include "GLOBAL_CELLS.H"
#include "TYPES.H"
#include "GAME_STATE.H"
#include "RAM_BUFFER.H"

struct Vec {
    s32 x;
    s32 y;
    s32 z;
};

struct Object {
    u8 pad0[6];
    u16 angle;
    struct Vec pos;
    u8 pad1[14];
    u8 kind;
};

void Vector_AddPolarOffset(s32, s32, struct Vec *);
s32 Map_GetTerrainHeightFar(s32, s32, s32);
struct Object *ObjectTable_Get(s32);
s32 BattleFx_FindDescriptor(s32, s32);

struct EntranceView {
    s16 entrance;                   /* 0x00; -1 ends the table */
    s16 flag;                       /* 0x02; -1 when unconditional */
    s16 x;                          /* 0x04 */
    s16 y;                          /* 0x06 */
    s16 z;                          /* 0x08 */
    u16 heading;                    /* 0x0a */
    s16 unknown_0c;
    s16 left;                       /* 0x0e; -1 keeps the current limit */
    s16 top;                        /* 0x10 */
    s16 right;                      /* 0x12 */
    s16 bottom;                     /* 0x14 */
    s16 unknown_16;
};

struct ViewServices {
    u8 unknown_00[0x0c];
    struct EntranceView *(*entrance_views)(void);
};

struct MapScrollWork {
    u8 unknown_000[0xec];
    s32 left;                       /* 0x0ec */
    s32 top;                        /* 0x0f0 */
    s32 right;                      /* 0x0f4 */
    s32 bottom;                     /* 0x0f8 */
};

extern struct ViewServices gOverlayArea;
s32 GameFlag_TestFar(s32 flag);
extern volatile u32 gKeysRepeat;
extern volatile u32 Data_03001ae8;
extern volatile u32 Data_03001e40;

/* The compressed swatch tiles, one filled with each colour index. */
extern const u8 Debug_PaletteSwatchTiles[];
void Resource_DecompressHalfwords(const void *source, void *destination);
void WaitFrames(s32 count);
void Ui_LoadWindowGraphics(void);
void Bg0_ClearTilemap(void);
#define PALETTE ((u16 *)0x05000000)
#define EDITOR_MAP ((u16 *)0x0600205a)
#define HEX_DIGIT 0xf0e0

/* One 24-byte descriptor; a sprite of -1 ends each table. */
struct ActionDescriptor {
    u16 sprite;
    s16 condition;
    s32 behavior;
    s32 x;
    s32 y;
    s32 z;
    u16 facing;
    u8 talk_facing;
    u8 flags;
};

struct ActionDescriptorTables {
    struct ActionDescriptor *tables[4];
};

extern struct ActionDescriptorTables *gEventWork;

s32 Object_GetTriggerTileAheadOfCurrent(void)
{
    u8 *state;
    u8 *map;
    struct Object *obj;
    struct Vec pos;
    u8 *cell;
    u8 *base;
    s32 kind;
    s32 height;
    s32 result;

    result = 0;
    obj = ObjectTable_Get(gGameState.selected_actor);
    state = *(u8 **)Ram_EventWork;
    map = *(u8 **)Ram_MapWork;
    if (obj != 0) {
        pos.x = obj->pos.x;
        pos.y = obj->pos.y;
        pos.z = obj->pos.z;
        Vector_AddPolarOffset(0x100000, obj->angle, &pos);
        if (*(s16 *)(state + 0x19e) == 3) {
            cell = Ram_MapBlocks + ((((pos.x / 0x200000) & 31) + (((pos.z / 0x200000) & 31) << 5)) << 2);
        } else {
            base = *(u8 **)(map + 0x130);
            cell = base + (((pos.x / 0x100000) + ((pos.z / 0x100000) << 7)) << 2);
        }
        kind = cell[2];
        if ((u32)(kind - 242) <= 5) {
            height = Map_GetTerrainHeightFar(obj->kind, pos.x, pos.z);
            if (height >= obj->pos.y && height <= obj->pos.y + 0x400000) {
                result = kind;
            }
        } else if (BattleFx_FindDescriptor(3, kind) != 0) {
            result = kind;
        }
    }
    return result;
}

/*
 * Places the battle view for the current entrance: finds the entrance's
 * entry in the overlay's view table (the first entry whose flag is clear
 * when none matches), sets the view position and heading from it unless
 * flag 0x109 is set, then applies the entry's scroll limits and keeps the
 * view window at least one screen (240 by 160) inside them.
 */
void BattleMap_ApplyEntranceView(void)
{
    struct MapScrollWork *work = gMapWork[0];
    s32 entrance = gGameState.entrance;
    struct EntranceView *view = gOverlayArea.entrance_views();
    s32 found = 0;

    while (view->entrance != -1) {
        if (view->entrance == entrance
            && (view->flag == -1 || GameFlag_TestFar(view->flag))) {
            found = 1;
            break;
        }
        view++;
    }
    if (!found)
        view = gOverlayArea.entrance_views();

    if (!GameFlag_TestFar(0x109)) {
        gGameState.x = view->x << 16;
        gGameState.y = view->y << 16;
        gGameState.z = view->z << 16;
        gGameState.heading = view->heading;
        gGameState.turn = 0;
    }

    if (view->left != -1)
        work->left = view->left << 16;
    if (view->top != -1)
        work->top = view->top << 16;
    if (view->right != -1)
        work->right = view->right << 16;
    if (view->bottom != -1)
        work->bottom = view->bottom << 16;

    if (work->left + (240 << 16) > work->right)
        work->left = work->right - (240 << 16);
    if (work->top + (160 << 16) > work->bottom)
        work->top = work->bottom - (160 << 16);
}

/*
 * A developer palette editor over the background palettes. Shows the chosen
 * palette's fifteen colours as swatches with their red, green and blue
 * levels in hex digits; up and down pick the channel, left and right the
 * colour, L and R the palette, A and B raise and lower the level, holding
 * Start blinks the colour and Select leaves.
 */
void Debug_RunPaletteEditor(void)
{
    s32 palette = 0;
    s32 channel = 1;
    s32 color = 1;
    u16 *map;
    u16 *p;
    u16 *colors;
    u32 tile;
    u32 value;
    u32 red;
    u32 green;
    u32 blue;
    u32 i;

    Resource_DecompressHalfwords(Debug_PaletteSwatchTiles, (void *)0x06001a00);
redraw:
    tile = (palette << 12) + 0xd1;
    colors = &PALETTE[palette * 16 + 1];
    map = EDITOR_MAP;
    p = map;
    *p = HEX_DIGIT + palette;
    p += 32;
    *p = 0xf000 | 'R';
    p += 32;
    *p = 0xf000 | 'G';
    p += 32;
    *p = 0xf000 | 'B';
    map++;
    for (i = 1; i < 16; i++) {
        p = map;
        *p = tile++;
        value = *colors++;
        p += 32;
        *p = (value & 31) + HEX_DIGIT;
        p += 32;
        *p = ((value >> 5) & 31) + HEX_DIGIT;
        p += 32;
        *p = ((value >> 10) & 31) + HEX_DIGIT;
        map++;
    }
    WaitFrames(1);
    for (;;) {
        if (gKeysRepeat & 0x40) {
            channel--;
            if (channel <= 0)
                channel = 3;
        }
        if (gKeysRepeat & 0x80) {
            channel++;
            if (channel > 3)
                channel = 1;
        }
        if (gKeysRepeat & 0x20) {
            color--;
            if (color <= 0)
                color = 15;
        }
        if (gKeysRepeat & 0x10) {
            color++;
            if (color > 15)
                color = 1;
        }
        if (gKeysRepeat & 0x200) {
            palette--;
            if (palette < 0)
                palette = 13;
            goto redraw;
        }
        if (gKeysRepeat & 0x100) {
            palette++;
            if (palette > 13)
                palette = 0;
            goto redraw;
        }
        if (gKeysRepeat & 1) {
            colors = &PALETTE[palette * 16 + color];
            value = *colors;
            red = value & 31;
            green = (value >> 5) & 31;
            blue = (value >> 10) & 31;
            if (channel == 1 && red < 31)
                red++;
            if (channel == 2 && green < 31)
                green++;
            if (channel == 3 && blue < 31)
                blue++;
            *colors = (blue << 10) | (green << 5) | red;
            goto redraw;
        }
        if (gKeysRepeat & 2) {
            colors = &PALETTE[palette * 16 + color];
            value = *colors;
            red = value & 31;
            green = (value >> 5) & 31;
            blue = (value >> 10) & 31;
            if (channel == 1 && red != 0)
                red--;
            if (channel == 2 && green != 0)
                green--;
            if (channel == 3 && blue != 0)
                blue--;
            *colors = (blue << 10) | (green << 5) | red;
            goto redraw;
        }
        if (gKeysRepeat & 8) {
            colors = &PALETTE[palette * 16 + color];
            value = *colors;
            red = 0;
            for (;;) {
                WaitFrames(1);
                if (!(Data_03001ae8 & 8))
                    break;
                if (red == 0)
                    *colors = 0x7fff;
                if (red == 10)
                    *colors = value;
                if (red == 20)
                    *colors = 0;
                if (red == 30)
                    *colors = value;
                if (++red >= 40)
                    red = 0;
            }
            *colors = value;
        }
        if (gKeysRepeat & 4)
            break;
        Data_03001e40;
        WaitFrames(1);
    }
    Ui_LoadWindowGraphics();
    Bg0_ClearTilemap();
}

/*
 * Finds the descriptor for a battle action across the event work's four
 * tables. Ids up to 7 name a descriptor by its sprite; larger ids count,
 * from 8, the descriptors whose sprite is above 7. Returns NULL when the
 * id is not found.
 */
struct ActionDescriptor *BattleAction_FindDescriptor(s32 id)
{
    struct ActionDescriptor *entry;
    s32 i;
    s32 group = 8;

    for (i = 0; i < 4; i++) {
        entry = gEventWork->tables[i];
        if (entry == NULL)
            continue;
        if (id <= 7) {
            for (; (s16)entry->sprite != -1; entry++) {
                if ((s16)entry->sprite == id)
                    goto found;
            }
        } else {
            for (; (s16)entry->sprite != -1; entry++) {
                if ((s16)entry->sprite > 7) {
                    if (group == id)
                        goto found;
                    group++;
                }
            }
        }
    }
found:
    if ((s16)entry->sprite == -1)
        return NULL;
    return entry;
}
