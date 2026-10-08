#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "TYPES.H"
#include "METADATA_LOOKUP.H"
#include "ANIMSPR.H"
#include "HEAP_STATE.H"
#include "IO_REG.H"

extern volatile u32 gKeysHeld;
extern s32 gFrameTick;

void Vector_AddPolarOffset(s32 radius, s32 angle, s32 *point);

void Map_BuildProbeRing(s32 x, s32 z, struct AnimationPosition *out)
{
    struct AnimationPosition *pair;
    s32 angle;
    s32 i;
    u16 turn;
    u8 kind;

    angle = 0;
    kind = gSpriteObjects->entries[0]->kind;
    if ((gKeysHeld & KEY_B) != 0)
        angle = (s16)(gFrameTick << 8);
    switch ((u32)kind) {
    case 3:
        for (i = 0; i < 6; i++) {
            angle <<= 16;
            out->x = x << 16;
            angle = (u32)angle >> 16;
            out->y = 0;
            out->z = z << 16;
            Vector_AddPolarOffset(0x380000, (u16)angle, &out->x);
            out++;
            angle = (s16)(angle + 0x2aaa);
        }
        for (; i < 10; i++)
            out++;
        break;
    case 5:
    case 8:
    case 44:
    case 88:
        for (i = 0; i < 8; i++) {
            angle <<= 16;
            out->x = x << 16;
            angle = (u32)angle >> 16;
            out->y = 0;
            out->z = z << 16;
            Vector_AddPolarOffset(0x380000, (u16)angle, &out->x);
            out++;
            angle = (s16)(angle + 0x2000);
        }
        for (; i < 10; i++)
            out++;
        break;
    case 4:
    case 6:
        for (i = 0; i < 10; i++) {
            angle <<= 16;
            out->x = x << 16;
            angle = (u32)angle >> 16;
            out->y = 0;
            out->z = z << 16;
            Vector_AddPolarOffset(0x380000, (u16)angle, &out->x);
            out++;
            angle = (s16)(angle + 0x1999);
        }
        break;
    case 20:
        turn = angle;
        angle = (s16)(turn + 0x4000);
        i = 0;
        pair = out;
        while (i < 2) {
            pair[i * 2].x = x << 16;
            pair[i * 2].y = 0;
            pair[i * 2].z = z << 16;
            angle <<= 16;
            angle = (u32)angle >> 16;
            Vector_AddPolarOffset(0x280000, angle, &out[0].x);
            pair[i * 2 + 1].x = x << 16;
            {
                s32 *point = &out[1].x;
                pair[i * 2 + 1].y = 0;
                pair[i * 2 + 1].z = z << 16;
                Vector_AddPolarOffset(0x280000, (u16)angle, point);
            }
            angle = (angle + 0x8000) << 16;
            i++;
            out += 2;
            angle >>= 16;
        }
        break;
    default:
        angle = ((angle << 16) + 0x20000000) >> 16;
        for (i = 0; i < 4; i++) {
            angle <<= 16;
            angle = (u32)angle >> 16;
            out->x = x << 16;
            out->y = 0;
            out->z = z << 16;
            Vector_AddPolarOffset(0x380000, (u16)angle, &out->x);
            out++;
            angle = (s16)(angle + 0x4000);
        }
        for (i = 4; i < 10; i++)
            out++;
        break;
    }
}

struct Vec2 {
    s32 x;
    s32 y;
};

void Ui_SetGridColumnParameter(s32 slot, s32 value)
{
    struct AnimationObject *object = gSpriteObjects;
    s32 i;

    slot &= 3;
    for (i = 0; i < 10; i++) {
        object->entries[slot]->param = value;
        object++;
    }
}

void Ui_SetGridColumnPriority(s32 slot, s32 value)
{
    struct AnimationObject *object = gSpriteObjects;
    s32 i;

    slot &= 3;
    for (i = 0; i < 10; i++) {
        object->entries[slot]->priority = value;
        object++;
    }
}

void Ui_FillGridColumnFromMetadata(s32 slot, s32 value)
{
    s32 index;
    s32 count;
    s32 offset;
    struct AnimationMetadata *metadata;
    struct AnimationEntry *entry;
    struct AnimationObject *object;

    object = gSpriteObjects;
    count = 0;
    /* FAKEMATCH: needed to match: the precomputed slot lane keeps the offset addition outside the loop; typed indexing moves it inside. */
    offset = (3 & slot) * sizeof(void *) + (u32)&((struct AnimationObject *)0)->entries;
    index = 0;
    do {
        entry = *(struct AnimationEntry **)((u8 *)object + offset);
        if (entry->scripts != 0) {
            metadata = Resource_GetMetadataRecordFar(entry->anim_id);
            if (value < metadata->animation_count) {
                entry->kind = metadata->draw_kind;
                entry->script = entry->scripts[value];
                entry->timer = count * 0x10;
                entry->step = 0x10;
                entry->pos = index;
                entry->frame_base = index;
                entry->frame = 0xff;
            }
            object->offset_y = metadata->adjust_y;
            object->rotation = index;
        }
        count++;
        object++;
    } while (count <= 9);
}

void Ui_SetGridColumnNumber(s32 slot, s32 no)
{
    struct AnimationObject *object = gSpriteObjects;
    s32 i;

    Resource_GetMetadataRecordFar(no);
    slot &= 3;
    for (i = 0; i < 10; i++) {
        struct AnimationEntry *entry = object->entries[slot];

        entry->anim_id = no;
        Animation_InitWorkFromMetadata(entry);
        object++;
    }
}

void Battle_PlaceActorsByFormationKind(void)
{
    struct AnimationObject *actor = gSpriteObjects;
    u32 kind = actor->entries[0]->kind;
    struct Vec2 scale;
    struct AnimationPosition *table;
    u16 angle;
    u16 step;
    u16 odd = 0;
    u32 count;
    u32 i;

    scale = Battle_FormationPlacementScale;
    /* FAKEMATCH: needed to match: loading heap slot 9 relative to slot 4 avoids a second RAM-base load and eight bytes. */
    table = *(struct AnimationPosition **)((u8 *)&gSpriteObjects + (9 - 4) * sizeof(((union HeapState *)0)->slots[0]));

    switch (kind) {
    case 3:
        angle = 0;
        step = 0x2aaa;
        count = 6;
        break;
    case 5:
    case 8:
    case 44:
    case 88:
        angle = 0;
        step = 0x2000;
        count = 8;
        break;
    case 4:
    case 6:
        angle = 0;
        step = 0x1999;
        count = 10;
        break;
    case 20:
        angle = 0;
        step = 0;
        odd = 0x8000;
        count = 4;
        break;
    default:
        angle = 0x2000;
        step = 0x4000;
        count = 4;
        break;
    }

    for (i = 0; i < count; i++) {
        Render_ApplyProjectedPlacement(actor, &table[i].x, (s32 *)&scale, angle);
        actor++;
        angle += step;
        if (i & 1)
            angle += odd;
    }
}
