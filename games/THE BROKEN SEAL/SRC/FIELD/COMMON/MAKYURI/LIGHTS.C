#include "RUNTIME_MEM.H"
#include "DMA.H"
#include "RAM_BUFFER.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "OBJECT_RUNTIME.H"
#include "ANIMSPR.H"
#include "MAP.H"

/* The light script's timer and anchor are a distinct motion-mode tail. */
struct MakyuriObject {
    u8 unknown_00[0x64];
    u16 timer;
    u8 unknown_66[2];
    struct ObjectRuntime *owner;
};

struct MakyuriLights {
    s32 lit;
    s32 region;
    s32 pad08[3];
    struct FieldActor *lower;
    struct FieldActor *upper;
};

extern const s32 Makyuri_PillarScript[];
extern const s32 Makyuri_RampScript[];
s32 Engine_GameFlagIsSet(s32 flag);
void *ObjectTable_Get();
s32 AnimationObjects_SelectAnimation(struct AnimationObject *anim, s32 animation);

/* Mercury Lighthouse: remember the light state in heap block 35. Before
 * flag 0x109 the state is cleared and keeps only its region; after it, the
 * lower light rises above the party leader while lit, and the upper light
 * appears when the leader stands in the state's region. */
void Makyuri_SpawnLightObjects(s32 region, struct MakyuriLights *st)
{
    struct ObjectRuntime *actor;
    struct FieldActor *obj;
    struct FieldSprite *anim;
    struct MapCell *cell;
    s32 z;
    s32 flag;
    volatile u32 cleared;

    *(struct MakyuriLights **)Runtime_AllocateBlock(35, sizeof(st)) = st;
    flag = Engine_GameFlagIsSet(0x109);
    if (flag == 0) {
        cleared = flag;
        Dma_Set((const void *)&cleared, st, 0x85000007, (volatile u32 *)0x040000d4);
        st->region = region;
        return;
    }
    actor = ObjectTable_Get(gGameState.selected_actor);
    z = actor->z;
    cell = (struct MapCell *)Ram_MapCellBuffer + ((z / 0x100000) << 7) + actor->x / 0x100000;
    if (st->lit != 0 && st->lower != 0) {
        obj = Engine_ObjectCreate(26, actor->x, actor->y + 0x180000, z);
        if (obj == 0)
            goto upper;
        ((struct ObjectRuntime *)obj)->terrain_height = actor->terrain_height;
        anim = obj->sprite;
        Engine_ObjectSetScript(obj, Makyuri_PillarScript);
        /* FAKEMATCH: the typed tail store moves past motion_flags and y in
           the 394-byte body; retain its existing address-word alias lane. */
        *(s32 *)((u8 *)obj + (u32)&((struct MakyuriObject *)0)->owner) = (s32)actor;
        obj->motion_flags = 4;
        obj->y.fixed += -0x8000;
        if (anim != 0) {
            AnimationObjects_SelectAnimation((struct AnimationObject *)anim, 6 - st->lit);
            anim->flags = 0;
            anim->priority = 1;
        }
        st->lower = obj;
    } else {
        st->lower = 0;
    }
upper:
    if (cell->collision_code == region && st->upper != 0) {
        obj = Engine_ObjectCreate(26, actor->x, actor->y, actor->z);
        if (obj == 0)
            return;
        ((struct ObjectRuntime *)obj)->terrain_height = actor->terrain_height;
        anim = obj->sprite;
        Engine_ObjectSetScript(obj, Makyuri_RampScript);
        obj->motion_flags = 0;
        ((struct MakyuriObject *)obj)->timer = 0;
        obj->priority_flags = 2;
        obj->speed = 0x40000;
        if (anim != 0) {
            AnimationObjects_SelectAnimation((struct AnimationObject *)anim, 6);
            /* FAKEMATCH: a one-halfword aggregate holds the zero frame, a
             * halfword pool constant whose short pool range dumps the
             * literal pool before the tail. */
            {
                struct Half {
                    u16 v;
                } zero;

                zero.v = 0;
                anim->flags = zero.v;
            }
        }
        st->upper = obj;
    } else {
        st->upper = 0;
    }
}
