/* Draft, not exact: 480 of 492 bytes, 37 halfword edits, 87 differing
   halfwords by position. A fallback label inside the entry-point branch
   shares home_x, but also merges its destination-address setup too early.
   The return tail is 12 bytes short; the heal loop additionally schedules
   the PP store after HP sign extension. An out-of-branch fallback label
   gives 488 bytes and 22 edits, but moves the defeat fallback past entry
   loads and reloads the state pointer before the both-unset home case. */
#include "TYPES.H"
#include "BATTLE_TYPES.H"

#define Party_SetReturnPoint Func_0808a6e4

/* Party-state rows 0x1c0-0x1d4 hold the field return point as x/y pairs;
   0x236 remembers the reason the party is being sent back. */
struct ReturnPointState {
    u8 unknown_000[0x1c0];
    s16 return_x;                   /* 0x1c0 */
    s16 return_y;                   /* 0x1c2 */
    s16 sanctum_x;                  /* 0x1c4 */
    s16 sanctum_y;                  /* 0x1c6 */
    s16 home_x;                     /* 0x1c8 */
    s16 home_y;                     /* 0x1ca */
    u8 unknown_1cc[2];
    s16 entry_x;                    /* 0x1ce */
    s16 entry_y;                    /* 0x1d0 */
    s16 defeat_x;                   /* 0x1d2 */
    s16 defeat_y;                   /* 0x1d4 */
    u8 unknown_1d6[0x1e];
    s32 current_owner;              /* 0x1f4 */
    u8 unknown_1f8[0x3e];
    s16 reason;                     /* 0x236 */
};

extern struct ReturnPointState Data_02000240;

s32 Math_Div(s32, s32);
struct BattleUnit *Owner_GetStateFar(s32);
s32 GameFlag_TestFar(s32);
void GameFlag_SetBitFar(s32);

#define GAUGE_ONE 0x4000

static __inline__ void Unit_UpdateGauges(struct BattleUnit *unit)
{
    s32 ratio;
    s32 gauge;
    s32 ratio2;
    s32 gauge2;

    ratio = Math_Div(unit->hp << 14, unit->max_hp);
    gauge = GAUGE_ONE;
    if (ratio <= GAUGE_ONE) {
        gauge = 0;
        if (ratio >= 0)
            gauge = ratio;
    }
    unit->hp_gauge = gauge;
    if ((s16)gauge == 0 && unit->hp != 0)
        unit->hp_gauge = 1;
    ratio2 = Math_Div(unit->pp << 14, unit->max_pp);
    gauge2 = GAUGE_ONE;
    if (ratio2 <= GAUGE_ONE) {
        gauge2 = 0;
        if (ratio2 >= 0)
            gauge2 = ratio2;
    }
    unit->pp_gauge = gauge2;
    if ((s16)gauge2 == 0 && unit->pp != 0)
        unit->pp_gauge = 1;
}

#define st (&Data_02000240)

void Party_SetReturnPoint(s32 reason)
{
    struct BattleUnit *unit;
    s32 i;
    s32 x;
    s32 y;

    st->reason = reason;
    if (reason == -1) {
        unit = Owner_GetStateFar(st->current_owner);
        if (unit->hp == 0) {
            unit->hp = 1;
            Unit_UpdateGauges(unit);
        }
        if (GameFlag_TestFar(32) != 0) {
            i = 0;
        heal:
            unit = Owner_GetStateFar(i);
            unit->hp = unit->max_hp;
            unit->pp = unit->max_pp;
            Unit_UpdateGauges(unit);
            if (++i <= 1)
                goto heal;
        }
        x = st->defeat_x;
        y = st->defeat_y;
        if (x == -1) {
            if (y == -1) {
                st->return_x = st->sanctum_x;
                st->return_y = st->sanctum_y;
                return;
            }
            goto default_x;
        } else {
            st->return_x = x;
        }
    } else {
        x = st->entry_x;
        y = st->entry_y;
        if (x == -1) {
            if (y == -1)
                goto home;
default_x:
            st->return_x = st->home_x;
        } else {
            st->return_x = x;
        }
    }
    if (y != -1)
        st->return_y = y;
    else
        st->return_y = st->home_y;
    return;
home:
    st->return_x = st->home_x;
    st->return_y = st->home_y;
    GameFlag_SetBitFar(0x109);
}
