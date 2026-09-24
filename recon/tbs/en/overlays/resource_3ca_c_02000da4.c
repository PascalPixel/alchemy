/* NONMATCHING: 396 of 396 bytes, 7 halfword edits (2026-09-24). Hand-written
 * from the resolved disassembly as a single-overlay unit binding Engine_* at
 * their import veneers. Matched: the sprite priorities are the FieldSprite
 * bitfields read through an s32 view of actor->sprite (so the pointer reloads
 * after each byte store and the mask is -13 derived from the 2 in r5); the BG
 * priorities 2 and 3 are one-halfword structs (short pool reach puts the pool
 * after the blend registers, as in the ROM); the layers go through a struct
 * MapLayer pointer (base +308, field +12). Remaining: sched2 hoists the
 * BLDCNT address load above the BG1CNT store (the store is class 2 against
 * the cnt spill's output dependence); the reference stores BG1CNT first, then
 * loads 0x2648 before the address (7 edits). */
#include "TYPES.H"
#include "FIELD_EVENT.H"

void Main_08077028(s32 a, s32 b);
void FieldScene_ConfigureFixedPointValues(void);

union GameStateRows {
    u8 bytes[512][2];
    s16 halves[512][1];
    s32 words[256];
};

struct MapLayer {
    u8 unknown_00[12];
    s32 y;
    u8 unknown_10[32];
};

struct MapWork {
    u8 unknown_00[20];
    struct MapLayer layers[8];
};

/* The IWRAM field globals: the map work first, the event work at +0x4c. */
struct FieldGlobals {
    struct MapWork *map;
    u8 unknown_04[0x48];
    u8 *event;
};

extern union GameStateRows Data_02000240_t;
extern struct FieldGlobals Data_03001e70;
extern u8 Data_00000002[];
extern u8 Data_00000003[];

#define SPRITE_BYTES(actor) ((u8 *)(actor)->sprite)

#define SPRITE_OF(actor) ((struct FieldSprite *)*(s32 *)((u8 *)(actor) + 0x50))
#define SET_PRIORITY(actor, n) ((n) == 9 ? (SPRITE_OF(actor)->priority = 1) : (SPRITE_OF(actor)->second_priority = 1))

#define REG_BG1CNT (*(volatile u16 *)0x0400000a)
#define REG_BG2CNT (*(volatile u16 *)0x0400000c)
#define REG_BG3CNT (*(volatile u16 *)0x0400000e)
#define REG_BLDCNT (*(volatile u16 *)0x04000050)
#define REG_BLDALPHA (*(volatile u16 *)0x04000052)

s32 Local_02000da4(void)
{
    struct MapWork *map;
    volatile u16 cnt;

    map = Data_03001e70.map;
    if (Data_02000240_t.halves[225][0] == 99) {
        Main_08077028(0, 242);
    }
    *(s32 *)(Data_03001e70.event + 0x1c0) = 0x100;
    Engine_ActorGet(8)->collision_flags = 0;
    Engine_ActorGet(8)->priority_flags = 2;
    Engine_ActorGet(9)->collision_flags = 0;
    Engine_ActorGet(9)->priority_flags = 2;
    {
        struct FieldActor *actor = Engine_ActorGet(8);

        SET_PRIORITY(actor, 9);
    }
    {
        struct FieldActor *actor = Engine_ActorGet(9);

        SET_PRIORITY(actor, 9);
    }
    {
        struct FieldActor *actor = Engine_ActorGet(0);

        SET_PRIORITY(actor, 9);
        SET_PRIORITY(actor, 21);
    }
    {
        struct FieldActor *actor = Engine_ActorGet(1);

        SET_PRIORITY(actor, 9);
        SET_PRIORITY(actor, 21);
    }
    {
        struct FieldActor *actor = Engine_ActorGet(2);

        SET_PRIORITY(actor, 9);
        SET_PRIORITY(actor, 21);
    }
    {
        struct FieldActor *actor = Engine_ActorGet(3);

        SET_PRIORITY(actor, 9);
        SET_PRIORITY(actor, 21);
    }
    {
        struct Half { u16 v; } two, three;

        cnt = REG_BG3CNT & 0xfffc;
        two.v = 2;
        cnt |= two.v;
        REG_BG3CNT = cnt;
        cnt = REG_BG2CNT & 0xfffc;
        three.v = 3;
        cnt |= three.v;
        REG_BG2CNT = cnt;
        cnt = (REG_BG1CNT & 0xfffc) | three.v;
        REG_BG1CNT = cnt;
    }
    {
        s32 v = 0x2648;

        REG_BLDCNT = v;
        v = 0x810;
        REG_BLDALPHA = v;
    }
    {
        struct MapLayer *layer = map->layers + 6;

        layer->y += -0x5a0000;
        layer = map->layers + 7;
        layer->y += -0x5a0000;
    }
    Engine_MapRedraw();
    FieldScene_ConfigureFixedPointValues();
    return 0;
}
