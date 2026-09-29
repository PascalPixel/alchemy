#include "DMA.H"
#include "RAM_BUFFER.H"

struct MakyuriActor {
    u8 pad00[8];
    s32 x;
    s32 y;
    s32 z;
    s32 layer;
};

struct MakyuriAnim {
    u8 pad00[9];
    u8 low : 2;
    u8 mode : 2;
    u8 high : 4;
    u8 pad0a[28];
    u8 frame;
};

struct MakyuriObject {
    u8 pad00[12];
    s32 y;
    u8 pad10[4];
    s32 layer;
    u8 pad18[11];
    u8 shape;
    u8 pad24[12];
    s32 scale;
    u8 pad34[28];
    struct MakyuriAnim *anim;
    u8 pad54;
    u8 state;
    u8 pad56[14];
    u16 timer;
    u8 pad66[2];
    struct MakyuriActor *owner;
};

struct MakyuriLights {
    s32 lit;
    s32 region;
    s32 pad08[3];
    struct MakyuriObject *lower;
    struct MakyuriObject *upper;
};

struct MakyuriCell {
    u8 pad00[2];
    u8 region;
    u8 pad03;
};

extern s32 gGameState[];
extern u8 Makyuri_PillarScript[];
extern u8 Makyuri_RampScript[];
struct MakyuriLights **Runtime_AllocateBlock(s32 slot, s32 size);
s32 Engine_GameFlagIsSet(s32 flag);
struct MakyuriActor *ObjectTable_Get(s32 id);
struct MakyuriObject *Engine_ObjectCreate(s32 kind, s32 x, s32 y, s32 z);
void Engine_ObjectSetScript(struct MakyuriObject *obj, void *script);
void AnimationObjects_SelectAnimation(struct MakyuriAnim *anim, s32 animation);

/* Mercury Lighthouse: remember the light state in heap block 35. Before
 * flag 0x109 the state is cleared and keeps only its region; after it, the
 * lower light rises above the party leader while lit, and the upper light
 * appears when the leader stands in the state's region. */
void Makyuri_SpawnLightObjects(s32 region, struct MakyuriLights *st)
{
    struct MakyuriActor *actor;
    struct MakyuriObject *obj;
    struct MakyuriAnim *anim;
    struct MakyuriCell *cell;
    s32 z;
    s32 flag;
    volatile u32 cleared;

    *Runtime_AllocateBlock(35, 4) = st;
    flag = Engine_GameFlagIsSet(0x109);
    if (flag == 0) {
        cleared = flag;
        Dma_Set((const void *)&cleared, st, 0x85000007, (volatile u32 *)0x040000d4);
        st->region = region;
        return;
    }
    actor = ObjectTable_Get(gGameState[125]);
    z = actor->z;
    cell = (struct MakyuriCell *)Ram_MapCellBuffer + ((z / 0x100000) << 7) + actor->x / 0x100000;
    if (st->lit != 0 && st->lower != 0) {
        obj = Engine_ObjectCreate(26, actor->x, actor->y + 0x180000, z);
        if (obj == 0)
            goto upper;
        obj->layer = actor->layer;
        anim = obj->anim;
        Engine_ObjectSetScript(obj, Makyuri_PillarScript);
        obj->owner = actor;
        obj->state = 4;
        obj->y += -0x8000;
        if (anim != 0) {
            AnimationObjects_SelectAnimation(anim, 6 - st->lit);
            anim->frame = 0;
            anim->mode = 1;
        }
        st->lower = obj;
    } else {
        st->lower = 0;
    }
upper:
    if (cell->region == region && st->upper != 0) {
        obj = Engine_ObjectCreate(26, actor->x, actor->y, actor->z);
        if (obj == 0)
            return;
        obj->layer = actor->layer;
        anim = obj->anim;
        Engine_ObjectSetScript(obj, Makyuri_RampScript);
        obj->state = 0;
        obj->timer = 0;
        obj->shape = 2;
        obj->scale = 0x40000;
        if (anim != 0) {
            AnimationObjects_SelectAnimation(anim, 6);
            /* FAKEMATCH: a one-halfword aggregate holds the zero frame, a
             * halfword pool constant whose short pool range dumps the
             * literal pool before the tail. */
            {
                struct Half {
                    u16 v;
                } zero;

                zero.v = 0;
                anim->frame = zero.v;
            }
        }
        st->upper = obj;
    } else {
        st->upper = 0;
    }
}
