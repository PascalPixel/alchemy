#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "MEM.H"
#include "DMA.H"
#include "RAM_BUFFER.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "OBJECT_RUNTIME.H"
#include "ANIMSPR.H"
#include "MAP.H"
#include "IO_REG.H"

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

s32 AnimationObjects_SelectAnimation(struct AnimationObject *anim, s32 animation);

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
        Dma_Set((const void *)&cleared, st, DMA_ENABLE32 | DMA_32BIT | DMA_SRC_FIXED32 | 7, REG_DMA3);
        st->region = region;
        return;
    }
    actor = Engine_ActorLookup(gGameState.selected_actor);
    z = actor->z;
    cell = (struct MapCell *)Ram_MapCellBuffer + ((z / 0x100000) << 7) + actor->x / 0x100000;
    if (st->lit != 0 && st->lower != 0) {
        obj = Engine_ObjectCreate(26, actor->x, actor->y + 0x180000, z);
        if (obj == 0)
            goto upper;
        obj->terrain_height = actor->terrain_height;
        anim = obj->sprite;
        Engine_ObjectSetScript(obj, Makyuri_PillarScript);
        /* FAKEMATCH: needed to match: address-word aliasing keeps the tail store before motion_flags and y. */
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
        obj->terrain_height = actor->terrain_height;
        anim = obj->sprite;
        Engine_ObjectSetScript(obj, Makyuri_RampScript);
        obj->motion_flags = 0;
        obj->timer = 0;
        obj->priority_flags = 2;
        obj->speed = 0x40000;
        if (anim != 0) {
            AnimationObjects_SelectAnimation((struct AnimationObject *)anim, 6);
            /* FAKEMATCH: needed to match: the halfword zero aggregate keeps the short-range literal pool before the tail. */
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
