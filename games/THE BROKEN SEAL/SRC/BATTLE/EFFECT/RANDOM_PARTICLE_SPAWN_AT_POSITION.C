#include "TYPES.H"
#include "FIXED_MATH.H"
#include "SYSTEM.H"

extern const u8 BattleFx_ParticleScript[];

/* The European editions pause the random particles while the field's top
   menu is open: the menu sets the event work's menu-open byte and clears the
   particles already flying, and no new ones spawn until it closes. */
#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) || \
    defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
#define PARTICLES_PAUSE_FOR_MENU 1
#endif

#if defined(PARTICLES_PAUSE_FOR_MENU)
#define BATTLE_ACTIVE_OFS 0xcb8
#define MENU_OPEN_OFS     0xcca

struct ParticleSlot {
    s32 script;
    u8 pad[0x6c];
};

extern u8 *gEventWork;
extern void Object_Destroy(struct ParticleSlot *);

#define ParticlePool (*(struct ParticleSlot **)((u8 *)&gEventWork - 88))

void BattleFx_ClearRandomParticles(void)
{
    if (*(s16 *)(gEventWork + BATTLE_ACTIVE_OFS) != 0) {
        struct ParticleSlot *ent = ParticlePool;
        s32 n = 63;

        do {
            if (ent->script != 0) {
                if (ent->script == (s32)BattleFx_ParticleScript)
                    Object_Destroy(ent);
            }
            n--;
            ent++;
        } while (n >= 0);
    }
}
#endif

struct Values_0808f28c {
    u32 first;
    u32 second;
    u32 third;
};

struct Source_0808f28c {
    u8 padding[8];
    struct Values_0808f28c values;
};

struct Child_0808f28c {
    u8 padding[9];
    u8 flags;
};

struct Object_0808f28c {
    u8 padding[80];
    struct Child_0808f28c *child;
};

extern void Vector_AddPolarOffset(s32, s32, struct Values_0808f28c *);
extern struct Object_0808f28c *Object_Spawn(s32, u32, u32, u32);
extern void ObjectDispatch_InitializeFar(struct Object_0808f28c *, void *);
extern void Object_SetMode(struct Object_0808f28c *, s32);

void BattleFx_SpawnRandomParticleAtPosition(const struct Source_0808f28c *source)
{
    struct Values_0808f28c values;
    struct Object_0808f28c *object;
    u32 rnd;

#if defined(PARTICLES_PAUSE_FOR_MENU)
    if (*(s8 *)(gEventWork + MENU_OPEN_OFS) != 0)
        return;
#endif
    if ((100 * Random16() >> 16) > 9)
        return;

    values.first = source->values.first;
    values.second = source->values.second;
    values.third = source->values.third;
    rnd = Random16();
    Vector_AddPolarOffset(rnd << 4, Random16(), &values);
    object = Object_Spawn(
        0x11D, values.first, values.second, values.third);
    if (object != 0) {
        s32 mask;
        u8 flags;

        ObjectDispatch_InitializeFar(object, (void *)BattleFx_ParticleScript);
        Object_SetMode(object, 0);
        mask = 13;
        flags = object->child->flags;
        mask = -mask;
        mask &= flags;
        mask |= 4;
        object->child->flags = mask;
    }
}
