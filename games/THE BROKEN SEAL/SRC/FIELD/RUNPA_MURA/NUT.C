#include "VILLAGE.H"

/* The Nut circles its resting point, bobbing and swaying. */
s32 FloatingNut_Update(union FieldObject *object)
{
    struct FloatingNut *nut = (struct FloatingNut *)object;
    struct FieldSprite *sprite = nut->sprite;
    s32 bob;
    s32 first;
    s32 second;

    bob = Math_Sin(nut->angle) * 2;
    if (bob > 0) {
        bob = -bob;
    }
    nut->x = nut->rest_x + Math_Cos(nut->angle) * 2;
    nut->y = nut->rest_y + bob;
    sprite->rotation = Math_Cos(nut->angle + 0x8000) / 8;
    first = Random_Next();
    second = Random_Next();
    nut->angle = nut->angle + (((u32)first << 9 >> 16) + ((u32)second << 9 >> 16)) + 0x400;
    return 0;
}

/* The Nut shows its item icon and hovers out of the party's reach. */
void FloatingNut_Initialize(s32 actor)
{
    struct FloatingNut *nut;
    struct FieldSprite *sprite;
    u8 *icon;

    nut = (struct FloatingNut *)Actor_Get(actor);
    sprite = nut->sprite;
    sprite->priority = 1;
    sprite->full_color = 0;
    sprite->palette = 0;
    sprite->part_count = 0;
    Actor_SetSpriteFlags((struct FieldActor *)nut, 0);
    nut->ready = 0;
    nut->motion_flags = 0;
    if (GameFlag_IsSet(FLAG_KEEP_PARTY_POSITION) == 0) {
        nut->y += PIXELS(32);
    }
    nut->priority_flags &= ~ACTOR_PRIORITY_AUTOMATIC;
    nut->free_motion = 1;
    icon = Heap_Allocate(HEAP_ITEM_ICON, ITEM_ICON_BUFFER_SIZE);
    Item_LoadIcon(ITEM_NUT);
    Vram_Load(sprite->vram_block, ITEM_ICON_TILE_BYTES, &icon[ITEM_ICON_TILES]);
    Heap_Release(HEAP_ITEM_ICON);
    nut->rest_x = nut->x;
    nut->angle = 0;
    nut->rest_y = nut->y;
    nut->ready = 1;
    nut->update = FloatingNut_Update;
    nut->status = 0;
}
