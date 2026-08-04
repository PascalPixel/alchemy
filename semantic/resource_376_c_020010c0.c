typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;

/*
 * resource_376 owner at 0x020010c0, 132 bytes: periodically activate one of
 * actors 23, 24, and 25, place it relative to actor 8, and initialise its
 * short motion/state record.
 *
 * Complete owner: `push {r5, r6, lr}` at 0x020010c0 through the interworking
 * return at 0x02001130-0x02001135, one alignment halfword, and three
 * referenced pool words through 0x02001143.  The import veneer bank starts at
 * 0x02001144.
 *
 * Func_030003e0 is the established relocated remainder helper.  Applied to
 * the global frame counter with period 180, only remainders 10, 20, and 30
 * select actors 23, 24, and 25 respectively; every other frame returns.
 * A missing selected actor also returns.  If actor 8 exists, its +8 and +16
 * coordinates are passed to Func_0808a0f0 for the selected actor.  The
 * selected record then receives paired 0x6666 motion terms, a +0x180000
 * displacement mirrored into +0x3c, state halfword 25, duration halfword
 * 128, and the overlay-local descriptor at 0x02009440.
 *
 * All five static calls resolve independently through the overlay veneer
 * bank under the +2 branch rule.
 */

struct Actor_020010c0 {
    u8 reserved00[8];
    s32 x;
    s32 displaced_axis;
    s32 y;
    u8 reserved14[4];
    s32 motion_x;
    s32 motion_y;
    u8 reserved20[0x1c];
    s32 mirrored_axis;
    u8 reserved40[0x24];
    u16 state;
    u16 duration;
};

extern volatile s32 Data_03001e40;
extern const u8 Data_02009440[];

extern s32 Func_030003e0(s32 value, s32 divisor);
extern struct Actor_020010c0 *Func_0808a080(s32 actor_id);
extern void Func_0808a0f0(s32 actor_id, s32 x, s32 y);
extern void Func_0808a098(s32 actor_id, const void *descriptor);

void Func_020010c0(void)
{
    struct Actor_020010c0 *selected;
    struct Actor_020010c0 *anchor;
    s32 actor_id;

    switch (Func_030003e0(Data_03001e40, 180)) {
    case 10:
        actor_id = 23;
        break;
    case 20:
        actor_id = 24;
        break;
    case 30:
        actor_id = 25;
        break;
    default:
        return;
    }

    selected = Func_0808a080(actor_id);
    if (selected == 0) {
        return;
    }

    anchor = Func_0808a080(8);
    if (anchor != 0) {
        Func_0808a0f0(actor_id, anchor->x, anchor->y);
    }

    selected->motion_x = 0x6666;
    selected->motion_y = 0x6666;
    selected->displaced_axis += 0x180000;
    selected->mirrored_axis = selected->displaced_axis;
    selected->state = 25;
    selected->duration = 128;
    Func_0808a098(actor_id, Data_02009440);
}
