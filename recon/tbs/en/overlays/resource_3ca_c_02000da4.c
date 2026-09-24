/* NONMATCHING: 394 of 396 bytes, 56 halfword edits (2026-09-24). Hand-written from the
 * resolved jump-table disassembly as a single-overlay unit binding Engine_* at
 * their import veneers. Remaining: the priority mask is 0xf3 where the reference derives -13 from the 2 in r5; the BG priorities 2 and 3 are pool constants whose short range puts the reference pool after the blend registers (the candidate pools at the end and derives 0x04000050 from 0x0400000a); the two layer offsets fold to +320/+368 where the reference keeps base +308/+356 with field +12 (56 edits). */
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

#define SET_PRIORITY(actor, n) (SPRITE_BYTES(actor)[n] = (s32)(SPRITE_BYTES(actor)[n] & ~0xc) | 4)

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
    cnt = (REG_BG3CNT & 0xfffc) | (u16)(u32)Data_00000002;
    REG_BG3CNT = cnt;
    cnt = (REG_BG2CNT & 0xfffc) | (u16)(u32)Data_00000003;
    REG_BG2CNT = cnt;
    cnt = (REG_BG1CNT & 0xfffc) | (u16)(u32)Data_00000003;
    REG_BG1CNT = cnt;
    REG_BLDCNT = 0x2648;
    REG_BLDALPHA = 0x810;
    map->layers[6].y += -0x5a0000;
    map->layers[7].y += -0x5a0000;
    Engine_MapRedraw();
    FieldScene_ConfigureFixedPointValues();
    return 0;
}
