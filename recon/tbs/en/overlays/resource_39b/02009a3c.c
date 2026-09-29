/* NONMATCHING (address-bound): Makyuri_SpawnLightObjects, resource_39b at
 * 0x02009a3c (396 bytes with its pool and alignment); twin
 * resource_39c:0x0200cfcc. Formerly FIELD/COMMON/MAKYURI/LIGHT_OBJECTS.C,
 * which no script linked.
 *
 * Remaining difference: 394 of 394 code bytes, 5 differing halfwords, all
 * in the cell address sum and the state pointer's scratch copy: the
 * reference adds the index to the base (adds r2, r2, r3) and copies the
 * state into r0, this source the base to the index into r3 with the state
 * copy in r2. Spelled with the map cell buffer as the integer 0x02010000
 * the source is byte-identical to both copies: agscc then folds the
 * address into the add as a constant, while the symbol gMapCellBuffer comes
 * from the constant pool and CSE orders it second. Declaring the buffer as
 * a typed array, a local base pointer, or the base assigned before the add
 * (4 halfwords: the sum then forms in sl) did not reproduce it. It links
 * once the map cell buffer has an honest link-time number. */
#include "DMA.H"
extern u8 gMapCellBuffer[];

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
extern u8 Makyuri_LowerLightScript[];
extern u8 Makyuri_UpperLightScript[];
struct MakyuriLights **Runtime_AllocateBlockFar(s32 slot, s32 size);
s32 GameFlag_TestFar(s32 flag);
struct MakyuriActor *ObjectTable_GetFar(s32 id);
struct MakyuriObject *Object_CreateFar(s32 kind, s32 x, s32 y, s32 z);
void Makyuri_ObjectSetScript(struct MakyuriObject *obj, void *script);
void AnimationObjects_SelectAnimationFar(struct MakyuriAnim *anim, s32 animation);

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

    *Runtime_AllocateBlockFar(35, 4) = st;
    flag = GameFlag_TestFar(0x109);
    if (flag == 0) {
        cleared = flag;
        Dma_Set((const void *)&cleared, st, 0x85000007, (volatile u32 *)0x040000d4);
        st->region = region;
        return;
    }
    actor = ObjectTable_GetFar(gGameState[125]);
    z = actor->z;
    cell = (struct MakyuriCell *)gMapCellBuffer + ((z / 0x100000) << 7) + actor->x / 0x100000;
    if (st->lit != 0 && st->lower != 0) {
        obj = Object_CreateFar(26, actor->x, actor->y + 0x180000, z);
        if (obj == 0)
            goto upper;
        obj->layer = actor->layer;
        anim = obj->anim;
        Makyuri_ObjectSetScript(obj, Makyuri_LowerLightScript);
        obj->owner = actor;
        obj->state = 4;
        obj->y += -0x8000;
        if (anim != 0) {
            AnimationObjects_SelectAnimationFar(anim, 6 - st->lit);
            anim->frame = 0;
            anim->mode = 1;
        }
        st->lower = obj;
    } else {
        st->lower = 0;
    }
upper:
    if (cell->region == region && st->upper != 0) {
        obj = Object_CreateFar(26, actor->x, actor->y, actor->z);
        if (obj == 0)
            return;
        obj->layer = actor->layer;
        anim = obj->anim;
        Makyuri_ObjectSetScript(obj, Makyuri_UpperLightScript);
        obj->state = 0;
        obj->timer = 0;
        obj->shape = 2;
        obj->scale = 0x40000;
        if (anim != 0) {
            AnimationObjects_SelectAnimationFar(anim, 6);
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
