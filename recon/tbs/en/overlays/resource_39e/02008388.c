/* resource_39e:02008388..02008414 (140 bytes with pool), still linked from
 * the listing. Remaining difference: the area test loads 0x3c from the
 * literal pool, a link-time value; an integer compares with an immediate
 * (132 bytes, 99 differ). */
#include "TYPES.H"
#include "FIELD_EVENT.H"

void Event_RegisterAreaEventsFar(void *events);

union GameStateRows {
    u8 bytes[512][2];
    s16 halves[512][1];
    s32 words[256];
};

struct AreaEvent {
    u16 kind;
    u16 flag;
    s32 unknown_04;
    s32 x;
    s32 unknown_0c;
    s32 z;
    s32 unknown_14;
};

extern union GameStateRows Data_02000240_t;
extern struct AreaEvent ShianJiin_AreaEventsA[];
extern struct AreaEvent ShianJiin_AreaEventsB[];
extern struct AreaEvent ShianJiin_AreaEventsC[];

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

/* Choose the area's event table by area and entrance; once flag 0x895 is set, the third table's gated events take that flag and one event moves. */
struct AreaEvent *ShianJiin_SelectAreaEvents(void)
{
    if (Data_02000240_t.halves[224][0] == 0x3c) {
        return ShianJiin_AreaEventsA;
    }
    if (Data_02000240_t.halves[225][0] == 3) {
        return ShianJiin_AreaEventsB;
    }
    if (Value1(Engine_GameFlagIsSet, 0x895)) {
        ShianJiin_AreaEventsC[5].flag = 0x895;
        ShianJiin_AreaEventsC[7].flag = 0x895;
        ShianJiin_AreaEventsC[8].x = 0x1200000;
        ShianJiin_AreaEventsC[8].z = 0xf80000;
        ShianJiin_AreaEventsC[11].flag = 0x895;
        ShianJiin_AreaEventsC[12].flag = 0x895;
    }
    Event_RegisterAreaEventsFar(ShianJiin_AreaEventsC);
    return ShianJiin_AreaEventsC;
}
