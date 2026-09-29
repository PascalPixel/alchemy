/* Draft of Makyuri_SpawnLightObjects, resource_39c at 0x0200cfcc, for
 * FIELD/MAKYURI_HEYA (it compiles with that directory's PROBE.H). Linking it
 * needs the import veneer at 0x0200dc14 labelled Engine_ActorLookup and the
 * script at 0x0200de20 labelled Makyuri_GrowScript.
 * Remaining difference: 4 halfwords where the cell address is formed. The
 * game adds the index to the buffer's address in a low register, (plus base
 * index), and copies the sum to sl. Written as one sum, cse knows the base
 * is a constant and puts it second, (plus index base) (5 halfwords, the
 * state pointer then reloads into r2 instead of r0); set first and then
 * advanced, as here, the base comes first but the cell is kept in sl and
 * advanced there. A typed or two-dimensional array, a byte-offset sum,
 * reordered terms, a pointer local, an inline helper and the inline engine
 * services all keep one of the two. The pool zero for the upper light's
 * frame comes out of the plain store. */
#include "PROBE.H"
#include "DMA.H"

extern u8 gMapCellBuffer[];

/* The lower light follows the party leader; the upper light grows in place. */
extern const s32 Makyuri_PillarScript[];
extern const s32 Makyuri_GrowScript[];

struct LightAnim {
    u8 unknown_00[9];
    u8 low : 2;
    u8 mode : 2;
    u8 high : 4;
    u8 unknown_0a[28];
    u8 frame;
};

struct LightActor {
    u8 unknown_00[8];
    s32 x;
    s32 y;
    s32 z;
    s32 layer;
};

struct LightObject {
    u8 unknown_00[12];
    s32 y;
    u8 unknown_10[4];
    s32 layer;
    u8 unknown_18[11];
    u8 shape;
    u8 unknown_24[12];
    s32 scale;
    u8 unknown_34[28];
    struct LightAnim *anim;
    u8 unknown_54;
    u8 state;
    u8 unknown_56[14];
    u16 timer;
    u8 unknown_66[2];
    struct LightActor *owner;
};

struct MakyuriLights {
    s32 lit;
    s32 region;
    s32 unknown_08[3];
    struct LightObject *lower;
    struct LightObject *upper;
};

struct MapCell {
    u8 unknown_00[2];
    u8 region;
    u8 unknown_03;
};

/* Remember the light state in heap block 35. Before flag 0x109 the state is
 * cleared and keeps only its region; after it, the lower light rises above
 * the party leader while lit, and the upper light appears when the leader
 * stands in the state's region. */
void Makyuri_SpawnLightObjects(s32 region, struct MakyuriLights *st)
{
    struct LightActor *actor;
    struct LightObject *obj;
    struct LightAnim *anim;
    struct MapCell *cell;
    s32 z;
    s32 flag;
    s32 idx;
    volatile u32 cleared;

    *Runtime_AllocateBlock(35, 4) = (u8 *)st;
    flag = Engine_GameFlagIsSet(0x109);
    if (flag == 0) {
        cleared = flag;
        Dma_Set((const void *)&cleared, st, 0x85000007, (volatile u32 *)0x040000d4);
        st->region = region;
        return;
    }
    actor = (struct LightActor *)Engine_ActorLookup(gGameState.selected_actor);
    z = actor->z;
    idx = ((z / 0x100000) << 7) + actor->x / 0x100000;
    cell = (struct MapCell *)gMapCellBuffer;
    cell += idx;
    if (st->lit != 0 && st->lower != 0) {
        obj = (struct LightObject *)Engine_ObjectCreate(26, actor->x, actor->y + 0x180000, z);
        if (obj == 0)
            goto upper;
        obj->layer = actor->layer;
        anim = obj->anim;
        Engine_ObjectSetScript((struct FieldActor *)obj, Makyuri_PillarScript);
        obj->owner = actor;
        obj->state = 4;
        obj->y += -0x8000;
        if (anim != 0) {
            AnimationObjects_SelectAnimation((u8 *)anim, 6 - st->lit);
            anim->frame = 0;
            anim->mode = 1;
        }
        st->lower = obj;
    } else {
        st->lower = 0;
    }
upper:
    if (cell->region == region && st->upper != 0) {
        obj = (struct LightObject *)Engine_ObjectCreate(26, actor->x, actor->y, actor->z);
        if (obj == 0)
            return;
        obj->layer = actor->layer;
        anim = obj->anim;
        Engine_ObjectSetScript((struct FieldActor *)obj, Makyuri_GrowScript);
        obj->state = 0;
        obj->timer = 0;
        obj->shape = 2;
        obj->scale = 0x40000;
        if (anim != 0) {
            AnimationObjects_SelectAnimation((u8 *)anim, 6);
            anim->frame = 0;
        }
        st->upper = obj;
    } else {
        st->upper = 0;
    }
}
