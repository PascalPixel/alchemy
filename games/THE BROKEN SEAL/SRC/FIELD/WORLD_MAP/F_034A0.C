#include "TYPES.H"

u8 *Engine_ActorGet(s32 actor);
u8 *Engine_ObjectCreate(s32 kind, s32 x, s32 y, s32 z);
u32 Engine_RandomNext();
void Engine_ObjectSetPalette(u8 *obj, s32 palette);
void Engine_ObjectSetAnimation(u8 *obj, s32 anim);
void Engine_ObjectSetScript(u8 *obj, s32 script);

struct Sprite371 {
    u8 pad[9];
    u8 lo : 2;
    u8 layer : 2;
    u8 hi : 4;
};

struct Flags35 {
    u8 pad[35];
    u8 flags;
};

struct Flags38 {
    u8 pad[38];
    u8 flags;
};

struct Flags85 {
    u8 pad[85];
    u8 flags;
};

/* Every sixteenth frame, drop a puff beside actor 8 at a random offset. */
void WorldMap_Func020034a0(void)
{
    u8 *leader;
    u8 *obj;
    struct Sprite371 *spr;
    u32 value;

    if ((*(s32 *)0x03001e40 & 15) != 0)
        return;
    leader = Engine_ActorGet(8);
    obj = Engine_ObjectCreate(222, *(s32 *)(leader + 8) + -0x200000, *(s32 *)(leader + 12), *(s32 *)(leader + 16) + -0x100000);
    if (obj == 0)
        return;
    *(s32 *)(obj + 24) = 0x8000;
    *(s32 *)(obj + 28) = 0x8000;
    spr = *(struct Sprite371 **)(obj + 80);
    if ((u16)(Engine_RandomNext() * 2 >> 16)) {
        s32 back = ((Engine_RandomNext() * 48) >> 16) << 16;

        *(s32 *)(obj + 8) -= back >> 1;
        *(s32 *)(obj + 16) -= back;
    } else {
        value = ((Engine_RandomNext() << 5) >> 16) << 16;
        *(s32 *)(obj + 8) += value;
        value = (s32)value >> 1;
        *(s32 *)(obj + 16) += value;
    }
    ((struct Flags38 *)spr)->flags = 0;
    spr->layer = ((struct Sprite371 *)*(u8 **)(leader + 80))->layer;
    ((struct Flags35 *)obj)->flags |= 2;
    ((struct Flags85 *)obj)->flags = leader[85];
    Engine_ObjectSetPalette(obj, 9);
    Engine_ObjectSetAnimation(obj, 2);
    Engine_ObjectSetScript(obj, 0x200e73c);
}
