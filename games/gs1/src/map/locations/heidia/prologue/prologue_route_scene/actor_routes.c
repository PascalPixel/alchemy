#include "types.h"

#define CheckActorPathSouth Func_02001b14
#define CheckActorPathNorth Func_02001b40
#define CheckActorPathWest Func_02001b68
#define CheckActorPathEast Func_02001b94
#define UpdateEscapeRouteForActorPositions Func_02001bbc
#define UpdateActor8ReturnRoute Func_02001c84
#define UpdateActor10RetreatRoute Func_02001d2c
#define UpdateActor10AdvanceRoute Func_02001de0
#define UpdateActor11WestRoute Func_02001e5c
#define UpdateActor11EastRoute Func_02001ef4
#define UpdateActor12WestRoute Func_02001f78
#define UpdateActor12EastRoute Func_02002004
#define UpdateActor13WestRoute Func_02002080
#define UpdateActor13EastRoute Func_0200216c
#define UpdateActor15WestRoute Func_02002200
#define UpdateActor15EastRoute Func_020022c8
#define UpdateActor17SouthRoute Func_020023c4
#define UpdateActor17NorthRoute Func_02002450
#define AdvanceActor18AlongEscapeRoute Func_020025f0
#define AdvanceActor9AlongEscapeRoute Func_02002848
#define RetreatActor14AlongEscapeRoute Func_02002b80
#define AdvanceActor14AlongEscapeRoute Func_02002c4c
#define RetreatActor16AlongEscapeRoute Func_02002d0c
#define AdvanceActor16AlongEscapeRoute Func_02002dd0
struct S { s32 pad[2]; s32 f08; s32 f0c; s32 f10; };

struct V { s32 a; s32 b; s32 c; };

struct S_02001bbc {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

struct S_02001c84 {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

struct S_02001d2c {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

struct S_02001de0 {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

struct S_02001e5c {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

struct S_02001ef4 {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

struct S_02001f78 {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

struct S_02002004 {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

struct S_02002080 {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

struct S_0200216c {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

struct S_02002200 {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

struct S_020022c8 {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

struct S_020023c4 {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

struct S_02002450 {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

struct S *Func_02004ba4(s32 arg0);
s32 Func_02003576(struct S *arg0, struct V *arg1);
struct S *Func_02004bd0(s32 arg0);
s32 Func_020035a4(struct S *arg0, struct V *arg1);
struct S *Func_02004bf8(s32 arg0);
s32 Func_020035ca(struct S *arg0, struct V *arg1);
struct S *Func_02004c24(s32 arg0);
s32 Func_020035f8(struct S *arg0, struct V *arg1);
struct S *Func_02004c4c(s32 arg0);
struct S *Func_02004c56(s32 arg0);
struct S *Func_02004c60(s32 arg0);
struct S *Func_02004c6a(s32 arg0);
struct S *Func_02004cda(s32 arg0);
void Func_02003376(s32 arg0, s32 arg1, s32 arg2);
void Func_02003386(s32 arg0, s32 arg1, s32 arg2);
void Func_020033b2(s32 arg0, s32 arg1, s32 arg2);
void Func_02003758(void);
void Func_02004c04(s32 arg0);
void Func_02004c90(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
void Func_02004ca0(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
struct S *Func_02004d14(s32 arg0);
struct S *Func_02004d1e(s32 arg0);
struct S *Func_02004d28(s32 arg0);
struct S *Func_02004d84(s32 arg0);
void Func_0200342a(s32 arg0, s32 arg1, s32 arg2);
void Func_02003436(s32 arg0, s32 arg1, s32 arg2);
void Func_02003440(s32 arg0, s32 arg1, s32 arg2);
void Func_02003454(s32 arg0, s32 arg1, s32 arg2);
void Func_02003464(s32 arg0, s32 arg1, s32 arg2);
void Func_02004cae(s32 arg0);
void Func_02004d3a(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
void Func_02004d4a(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
struct S *Func_02004dbc(s32 arg0);
struct S *Func_02004dc6(s32 arg0);
struct S *Func_02004dd0(s32 arg0);
struct S *Func_02004dda(s32 arg0);
struct S *Func_02004e38(s32 arg0);
void Func_020034f0(s32 arg0, s32 arg1, s32 arg2);
void Func_02003512(s32 arg0, s32 arg1, s32 arg2);
void Func_02004d62(s32 arg0);
void Func_02004dee(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
void Func_02004dfe(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
struct S *Func_02004e70(s32 arg0);
struct S *Func_02004e7a(s32 arg0);
struct S *Func_02004eb2(s32 arg0);
void Func_0200357c(s32 arg0, s32 arg1, s32 arg2);
void Func_02003588(s32 arg0, s32 arg1, s32 arg2);
void Func_02003592(s32 arg0, s32 arg1, s32 arg2);
void Func_02004ddc(s32 arg0);
void Func_02004e68(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
void Func_02004e78(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
struct S *Func_02004eec(s32 arg0);
struct S *Func_02004ef6(s32 arg0);
struct S *Func_02004f08(s32 arg0);
struct S *Func_02004f1e(s32 arg0);
struct S *Func_02004f4c(s32 arg0);
void Func_0200361e(s32 arg0, s32 arg1, s32 arg2);
void Func_0200362c(s32 arg0, s32 arg1, s32 arg2);
void Func_02004e76(s32 arg0);
void Func_02004f02(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
void Func_02004f12(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
struct S *Func_02004f84_a(s32 arg0);
struct S *Func_02004f8e(s32 arg0);
struct S *Func_02004fa0(s32 arg0);
struct S *Func_02004fce(s32 arg0);
void Func_0200369e(s32 arg0, s32 arg1, s32 arg2);
void Func_020036ae(s32 arg0, s32 arg1, s32 arg2);
void Func_02004ef8(s32 arg0);
void Func_02004f84_b(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
void Func_02004f94(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
struct S *Func_02005008(s32 arg0);
struct S *Func_02005012_a(s32 arg0);
struct S *Func_0200505c(s32 arg0);
void Func_02003714(s32 arg0, s32 arg1, s32 arg2);
void Func_0200372a(s32 arg0, s32 arg1, s32 arg2);
void Func_02003736(s32 arg0, s32 arg1, s32 arg2);
void Func_02004f86(s32 arg0);
void Func_02005012_b(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
void Func_02005022(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
struct S *Func_02005094(s32 arg0);
struct S *Func_0200509e(s32 arg0);
struct S *Func_020050d6(s32 arg0);
void Func_0200379c(s32 arg0, s32 arg1, s32 arg2);
void Func_020037b0(s32 arg0, s32 arg1, s32 arg2);
void Func_02005000(s32 arg0);
void Func_0200508c(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
void Func_0200509c(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
struct S *Func_02005110(s32 arg0);
struct S *Func_0200511a(s32 arg0);
struct S *Func_02005124(s32 arg0);
struct S *Func_0200512e(s32 arg0);
struct S *Func_020051c4(s32 arg0);
void Func_02003846(s32 arg0, s32 arg1, s32 arg2);
void Func_02003870(s32 arg0, s32 arg1, s32 arg2);
void Func_0200389e(s32 arg0, s32 arg1, s32 arg2);
void Func_020050ee(s32 arg0);
void Func_0200517a(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
void Func_0200518a(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
struct S *Func_020051fc(s32 arg0);
struct S *Func_02005206(s32 arg0);
void Func_02005210(s32 arg0);
struct S *Func_02005258(s32 arg0);
void Func_0200390a(s32 arg0, s32 arg1, s32 arg2);
void Func_02003932(s32 arg0, s32 arg1, s32 arg2);
void Func_02005182(s32 arg0);
void Func_0200520e(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
void Func_0200521e(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
struct S *Func_02005290(s32 arg0);
struct S *Func_0200529a(s32 arg0);
struct S *Func_020052a4(s32 arg0);
struct S *Func_020052ae(s32 arg0);
struct S *Func_02005320(s32 arg0);
void Func_020039be(s32 arg0, s32 arg1, s32 arg2);
void Func_020039d6(s32 arg0, s32 arg1, s32 arg2);
void Func_020039fa(s32 arg0, s32 arg1, s32 arg2);
void Func_0200524a(s32 arg0);
void Func_020052d6(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
void Func_020052e6(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
struct S *Func_02005358(s32 arg0);
struct S *Func_02005362(s32 arg0);
struct S *Func_0200536c(s32 arg0);
struct S *Func_02005376(s32 arg0);
struct S *Func_0200541c(s32 arg0);
void Func_02003a86(s32 arg0, s32 arg1, s32 arg2);
void Func_02003a9a(s32 arg0, s32 arg1, s32 arg2);
void Func_02003aaa(s32 arg0, s32 arg1, s32 arg2);
void Func_02003af6(s32 arg0, s32 arg1, s32 arg2);
void Func_02005346(s32 arg0);
void Func_020053d2(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
void Func_020053e2(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
struct S *Func_02005454(s32 arg0);
struct S *Func_0200545e(s32 arg0);
struct S *Func_02005468(s32 arg0);
struct S *Func_020054a6(s32 arg0);
void Func_02003b80(s32 arg0, s32 arg1, s32 arg2);
void Func_020053d0(s32 arg0);
void Func_0200545c(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
void Func_0200546c(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
struct S *Func_020054e0(s32 arg0);
struct S *Func_020054ea(s32 arg0);
struct S *Func_0200551e(s32 arg0);
void Func_02003be8(s32 arg0, s32 arg1, s32 arg2);
void Func_02003bf8(s32 arg0, s32 arg1, s32 arg2);
void Func_02005448(s32 arg0);
void Func_020054d4(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
void Func_020054e4(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
u8 *Func_0200568a();
u8 *Func_02005694();
u8 *Func_0200569e();
u8 *Func_02005758();
void Func_0200571e();
void Func_02003da8();
void Func_02003db2();
void Func_02003dce();
void Func_02003dda();
void Func_02003df0();
void Func_02003dfc();
void Func_02003e12();
void Func_02003e22();
void Func_02003e32();
u8 *Func_020058e8();
u8 *Func_020058f2();
u8 *Func_020058fc();
u8 *Func_02005906();
u8 *Func_020059d4();
void Func_0200599c();
void Func_0200401a();
void Func_02004024();
void Func_02004044();
void Func_02004050();
void Func_02004070();
void Func_0200407c();
void Func_02004094();
void Func_020040a4();
void Func_020040b4();
u8 *Func_02005c1a();
u8 *Func_02005c24();
u8 *Func_02005c2e();
u8 *Func_02005ca4();
void Func_02005c6a();
void Func_0200437e();
u8 *Func_02005ce6();
u8 *Func_02005cf0();
u8 *Func_02005cfa();
u8 *Func_02005d62();
void Func_02005d28();
void Func_0200443c();
u8 *Func_02005da6();
u8 *Func_02005db0();
u8 *Func_02005dba();
u8 *Func_02005e28();
void Func_02005dee();
void Func_02004502();
u8 *Func_02005e6a();
u8 *Func_02005e74();
u8 *Func_02005ec8();
void Func_02005e8e();
void Func_020045a2();

/* Advance actor 18 and its companions along the Z-axis escape route.
   Shared branches preserve the transition call sites used by multiple rows. */

/* Old-style declarations: overlay imports vary in arity between call sites.
   One import name per call site: bl displacements are per-site. */

/* This overlay's transition starter at 0x02001774, one name per site. */

/* Advance actor 9 and its companions along the Z-axis escape route.
   Shared branches preserve the transition call sites used by multiple rows. */

/* Return leg of the slot-14 transition beat: negated ids. */

/* This overlay's transition starter at 0x02001774. */

/* Outbound leg of the slot-14 transition beat. */

/* Return leg of the slot-16 transition beat: negated ids. */

/* Outbound leg of the slot-16 transition beat. */

u8 *Func_02005680();                    /* scene entity by selector */

void Func_02005682();                   /* wait N frames */

void Func_0200570e();                   /* collision repaint, six arguments */

u8 *Func_020058dc();                    /* scene entity by selector */

void Func_020058fe();                   /* wait N frames */

void Func_0200598c();                   /* collision repaint, six arguments */

u8 *Func_02005c10();                    /* scene entity by selector */

void Func_02005bce();                   /* wait N frames */

void Func_02005c5a();                   /* collision repaint, six arguments */

u8 *Func_02005cdc();                    /* scene entity by selector */

void Func_02005c8c();                   /* wait N frames */

void Func_02005d18();                   /* collision repaint, six arguments */

u8 *Func_02005d9c();                    /* scene entity by selector */

void Func_02005d52();                   /* wait N frames */

void Func_02005dde();                   /* collision repaint, six arguments */

u8 *Func_02005e60();                    /* scene entity by selector */

void Func_02005df2();                   /* wait N frames */

void Func_02005e7e();                   /* collision repaint, six arguments */

s32 CheckActorPathSouth(void)
{
    struct S *actor = Func_02004ba4(0);
    struct V destination;

    destination.a = actor->f08;
    destination.b = actor->f0c;
    destination.c = actor->f10 + -0x200000;
    return Func_02003576(actor, &destination);
}

s32 CheckActorPathNorth(void)
{
    struct S *actor = Func_02004bd0(0);
    struct V destination;

    destination.a = actor->f08;
    destination.b = actor->f0c;
    destination.c = actor->f10 + 0x200000;
    return Func_020035a4(actor, &destination);
}

s32 CheckActorPathWest(void)
{
    struct S *actor = Func_02004bf8(0);
    struct V destination;

    destination.a = actor->f08 + -0x200000;
    destination.b = actor->f0c;
    destination.c = actor->f10;
    return Func_020035ca(actor, &destination);
}

s32 CheckActorPathEast(void)
{
    struct S *actor = Func_02004c24(0);
    struct V destination;

    destination.a = actor->f08 + 0x200000;
    destination.b = actor->f0c;
    destination.c = actor->f10;
    return Func_020035f8(actor, &destination);
}

void UpdateEscapeRouteForActorPositions(void)
{
    s32 actor_x = Func_02004c4c(8)->f08 >> 20;
    s32 actor_z = Func_02004c56(8)->f10 >> 20;
    s32 actor_12_x = Func_02004c60(12)->f08 >> 20;
    s32 actor_15_x = Func_02004c6a(15)->f08 >> 20;

    if (actor_z == 19) {
        if (actor_12_x == 24) {
            Func_020033b2(8, 0, -80);
        } else if (actor_15_x == 24) {
            Func_02003376(8, 0, -112);
            Func_020033b2(8, 0, -32);
        } else {
            Func_02003386(8, 0, -80);
            Func_020033b2(8, 0, -112);
        }
    } else if (actor_z == 14) {
        if (actor_12_x == 24) {
            return;
        }
        if (actor_15_x == 24) {
            Func_020033b2(8, 0, -64);
        } else {
            Func_020033b2(8, 0, -112);
        }
    } else if (actor_z == 10) {
        if (actor_15_x == 24) {
            return;
        }
        Func_020033b2(8, 0, -48);
    } else {
        Func_02003758();
        return;
    }
    Func_02004c04(2);
    {
        s32 route_end_z = Func_02004cda(8)->f10 >> 20;
        s32 route_x = actor_x - 1;

        Func_02004c90(route_x, actor_z, 3, 1, route_x, route_end_z);
    }
    Func_02004ca0(0, 0, 3, 1, actor_x - 1, actor_z);
}

void UpdateActor8ReturnRoute(void)
{
    s32 x = Func_02004d14(8)->f08 >> 20;
    s32 y = Func_02004d1e(8)->f10 >> 20;
    s32 z = Func_02004d28(12)->f08 >> 20;

    if (y == 7) {
        if (z == 24) {
            Func_0200342a(8, 0, 48);
        } else {
            Func_02003436(8, 0, 80);
            Func_02003440(8, 0, 112);
        }
    } else if (y == 10) {
        if (z == 24) {
            return;
        }
        Func_02003454(8, 0, 144);
    } else if (y == 14) {
        Func_02003464(8, 0, 80);
    } else {
        return;
    }
    Func_02004cae(2);
    {
        s32 k = Func_02004d84(8)->f10 >> 20;
        s32 m = x - 1;

        Func_02004d3a(m, y, 3, 1, m, k);
    }
    Func_02004d4a(0, 0, 3, 1, x - 1, y);
}

void UpdateActor10RetreatRoute(void)
{
    s32 x = Func_02004dbc(10)->f08 >> 20;
    s32 y = Func_02004dc6(10)->f10 >> 20;
    s32 z = Func_02004dd0(13)->f08 >> 20;
    s32 w = Func_02004dda(15)->f08 >> 20;

    if (y == 18) {
        if (w >= 31 && w <= 33) {
            Func_02003512(10, 0, -128);
        } else if (z >= 31 && z <= 33) {
            Func_02003512(10, 0, -128);
        } else {
            Func_020034f0(10, 0, -112);
            Func_02003512(10, 0, -64);
        }
    } else if (y == 10) {
        if (w >= 31 && w <= 33) {
            return;
        }
        if (z >= 31 && z <= 33) {
            return;
        }
        Func_02003512(10, 0, -48);
    } else if (y == 7) {
        return;
    }
    Func_02004d62(2);
    {
        s32 k = Func_02004e38(10)->f10 >> 20;
        s32 m = x - 1;

        Func_02004dee(m, y, 3, 1, m, k);
    }
    Func_02004dfe(0, 0, 3, 1, x - 1, y);
}

void UpdateActor10AdvanceRoute(void)
{
    s32 x = Func_02004e70(10)->f08 >> 20;
    s32 y = Func_02004e7a(10)->f10 >> 20;

    if (y != 18) {
        if (y == 10) {
            Func_0200357c(10, 0, 128);
        } else {
            Func_02003588(10, 0, 112);
            Func_02003592(10, 0, 64);
        }
        Func_02004ddc(2);
        {
            s32 k = Func_02004eb2(10)->f10 >> 20;
            s32 m = x - 1;

            Func_02004e68(m, y, 3, 1, m, k);
        }
        Func_02004e78(0, 0, 3, 1, x - 1, y);
    }
}

void UpdateActor11WestRoute(void)
{
    s32 x = Func_02004eec(11)->f08 >> 20;
    s32 y = Func_02004ef6(11)->f10 >> 20;

    if (x != 30) {
        if (x == 34) {
            if ((Func_02004f08(10)->f10 >> 20) == 18) {
                return;
            }
            Func_0200361e(11, -64, 0);
        } else if (x == 36) {
            if ((Func_02004f1e(10)->f10 >> 20) == 18) {
                Func_0200361e(11, -32, 0);
            } else {
                Func_0200362c(11, -96, 0);
            }
        }
        Func_02004e76(2);
        {
            s32 k = Func_02004f4c(11)->f08 >> 20;
            s32 m = y - 1;

            Func_02004f02(x, m, 1, 3, k, m);
        }
        Func_02004f12(0, 0, 1, 3, x, y - 1);
    }
}

void UpdateActor11EastRoute(void)
{
    s32 x = Func_02004f84_a(11)->f08 >> 20;
    s32 y = Func_02004f8e(11)->f10 >> 20;

    if (x != 36) {
        if (x == 30) {
            if ((Func_02004fa0(10)->f10 >> 20) == 18) {
                return;
            }
            Func_0200369e(11, 96, 0);
        } else if (x == 34) {
            Func_020036ae(11, 32, 0);
        }
        Func_02004ef8(2);
        {
            s32 k = Func_02004fce(11)->f08 >> 20;
            s32 m = y - 1;

            Func_02004f84_b(x, m, 1, 3, k, m);
        }
        Func_02004f94(0, 0, 1, 3, x, y - 1);
    }
}

void UpdateActor12WestRoute(void)
{
    s32 x = Func_02005008(12)->f08 >> 20;
    s32 y = Func_02005012_a(12)->f10 >> 20;

    if (x == 36) {
        Func_02003714(12, -96, 0);
        Func_02003736(12, -96, 0);
    } else if (x == 34) {
        Func_0200372a(12, -96, 0);
        Func_02003736(12, -64, 0);
    } else if (x == 24) {
        return;
    }
    Func_02004f86(2);
    {
        s32 k = Func_0200505c(12)->f08 >> 20;
        s32 m = y - 1;

        Func_02005012_b(x, m, 1, 3, k, m);
    }
    Func_02005022(0, 0, 1, 3, x, y - 1);
}

void UpdateActor12EastRoute(void)
{
    s32 x = Func_02005094(12)->f08 >> 20;
    s32 y = Func_0200509e(12)->f10 >> 20;

    if (x == 24) {
        Func_0200379c(12, 96, 0);
        Func_020037b0(12, 96, 0);
    } else if (x == 34) {
        Func_020037b0(12, 32, 0);
    } else if (x == 36) {
        return;
    }
    Func_02005000(2);
    {
        s32 k = Func_020050d6(12)->f08 >> 20;
        s32 m = y - 1;

        Func_0200508c(x, m, 1, 3, k, m);
    }
    Func_0200509c(0, 0, 1, 3, x, y - 1);
}

void UpdateActor13WestRoute(void)
{
    s32 x = Func_02005110(13)->f08 >> 20;
    s32 y = Func_0200511a(13)->f10 >> 20;
    s32 z = Func_02005124(10)->f10 >> 20;
    s32 w = Func_0200512e(15)->f08 >> 20;

    if (x == 36) {
        if (w == 34) {
            Func_0200389e(13, -16, 0);
        } else if (z == 7) {
            Func_0200389e(13, -32, 0);
        } else if (w == 30) {
            Func_0200389e(13, -80, 0);
        } else {
            Func_02003846(13, -96, 0);
            Func_0200389e(13, -80, 0);
        }
    } else if (x == 35) {
        if (w == 34) {
            return;
        } else if (z == 7) {
            Func_0200389e(13, -16, 0);
        } else if (w == 30) {
            Func_0200389e(13, -64, 0);
        } else {
            Func_02003870(13, -80, 0);
            Func_0200389e(13, -80, 0);
        }
    } else if (x == 34) {
        if (z == 7) {
            return;
        }
        if (w == 30) {
            Func_0200389e(13, -48, 0);
        } else {
            Func_0200389e(13, -144, 0);
        }
    } else if (x == 31) {
        if (w == 30) {
            return;
        }
        Func_0200389e(13, -96, 0);
    } else if (x == 25) {
        return;
    }
    Func_020050ee(2);
    {
        s32 k = Func_020051c4(13)->f08 >> 20;
        s32 m = y - 1;

        Func_0200517a(x, m, 1, 3, k, m);
    }
    Func_0200518a(0, 0, 1, 3, x, y - 1);
}

void UpdateActor13EastRoute(void)
{
    s32 x = Func_020051fc(13)->f08 >> 20;
    s32 y = Func_02005206(13)->f10 >> 20;

    Func_02005210(15);
    if (x == 25) {
        Func_0200390a(13, 96, 0);
        Func_02003932(13, 80, 0);
    } else if (x == 31) {
        Func_02003932(13, 80, 0);
    } else if (x == 34) {
        Func_02003932(13, 32, 0);
    } else if (x == 35) {
        Func_02003932(13, 16, 0);
    } else if (x == 36) {
        return;
    }
    Func_02005182(2);
    {
        s32 k = Func_02005258(13)->f08 >> 20;
        s32 m = y - 1;

        Func_0200520e(x, m, 1, 3, k, m);
    }
    Func_0200521e(0, 0, 1, 3, x, y - 1);
}

void UpdateActor15WestRoute(void)
{
    s32 x = Func_02005290(15)->f08 >> 20;
    s32 y = Func_0200529a(15)->f10 >> 20;
    s32 z = Func_020052a4(8)->f10 >> 20;
    s32 w = Func_020052ae(10)->f10 >> 20;

    if (x == 35) {
        if (w == 7) {
            Func_020039fa(15, -16, 0);
        } else if (z == 7) {
            Func_020039fa(15, -112, 0);
        } else {
            Func_020039be(15, -96, 0);
            Func_020039fa(15, -80, 0);
        }
    } else if (x == 34) {
        if (w == 7) {
            return;
        }
        Func_020039d6(15, -96, 0);
        Func_020039fa(15, -64, 0);
    } else if (x == 33) {
        Func_020039fa(15, -144, 0);
    } else if (x == 31) {
        Func_020039fa(15, -80, 0);
    } else if (x == 30) {
        Func_020039fa(15, -96, 0);
    } else if (x == 24) {
        return;
    }
    Func_0200524a(2);
    {
        s32 k = Func_02005320(15)->f08 >> 20;
        s32 m = y - 1;

        Func_020052d6(x, m, 1, 3, k, m);
    }
    Func_020052e6(0, 0, 1, 3, x, y - 1);
}

void UpdateActor15EastRoute(void)
{
    s32 x = Func_02005358(15)->f08 >> 20;
    s32 y = Func_02005362(15)->f10 >> 20;
    s32 z = Func_0200536c(10)->f10 >> 20;
    s32 w = Func_02005376(13)->f08 >> 20;

    if (x == 24) {
        if (z == 7 || w == 31) {
            Func_02003af6(15, 96, 0);
        } else if (w == 34) {
            Func_02003a86(15, 64, 0);
            Func_02003af6(15, 80, 0);
        } else if (w == 35) {
            Func_02003a9a(15, 80, 0);
            Func_02003af6(15, 80, 0);
        } else {
            Func_02003aaa(15, 80, 0);
            Func_02003af6(15, 96, 0);
        }
    } else if (x == 30 || w == 31) {
        if (z == 7) {
            return;
        }
        if (w == 34) {
            Func_02003af6(15, 48, 0);
        } else if (w == 35) {
            Func_02003af6(15, 64, 0);
        } else {
            Func_02003af6(15, 80, 0);
        }
    } else if (x == 33) {
        if (w == 34) {
            return;
        }
        if (w == 35) {
            Func_02003af6(15, 16, 0);
        } else {
            Func_02003af6(15, 32, 0);
        }
    } else if (x == 34) {
        Func_02003af6(15, 16, 0);
    } else if (x == 35) {
        return;
    }
    Func_02005346(2);
    {
        s32 k = Func_0200541c(15)->f08 >> 20;
        s32 m = y - 1;

        Func_020053d2(x, m, 1, 3, k, m);
    }
    Func_020053e2(0, 0, 1, 3, x, y - 1);
}

void UpdateActor17SouthRoute(void)
{
    s32 x = Func_02005454(17)->f08 >> 20;
    s32 y = Func_0200545e(17)->f10 >> 20;
    s32 z = Func_02005468(19)->f08 >> 20;

    if (y == 19) {
        if (z >= 3 && z <= 5) {
            Func_02003b80(17, 0, -16);
        } else {
            Func_02003b80(17, 0, -64);
        }
    } else if (y == 18) {
        if (z >= 3 && z <= 5) {
            return;
        }
        Func_02003b80(17, 0, -48);
    } else if (y == 15) {
        return;
    }
    Func_020053d0(2);
    {
        s32 k = Func_020054a6(17)->f10 >> 20;
        s32 m = x - 1;

        Func_0200545c(m, y, 3, 1, m, k);
    }
    Func_0200546c(0, 0, 3, 1, x - 1, y);
}

void UpdateActor17NorthRoute(void)
{
    s32 x = Func_020054e0(17)->f08 >> 20;
    s32 y = Func_020054ea(17)->f10 >> 20;

    if (y == 15) {
        Func_02003be8(17, 0, 64);
    } else if (y == 18) {
        Func_02003bf8(17, 0, 16);
    } else if (y == 19) {
        return;
    }
    Func_02005448(2);
    {
        s32 k = Func_0200551e(17)->f10 >> 20;

        s32 m = x - 1;

        Func_020054d4(m, y, 3, 1, m, k);
    }
    Func_020054e4(0, 0, 3, 1, x - 1, y);
}

void AdvanceActor18AlongEscapeRoute(void)
{
    s32 column;
    s32 row;
    s32 companion19Column;
    s32 companion14Column;

    s32 permuted_5;
    permuted_5 = *(s32 *)(Func_02005680(18) + 8) >> 20;
    row = *(s32 *)(Func_0200568a(18) + 16) >> 20;
    column  = permuted_5;
    companion19Column = *(s32 *)(Func_02005694(19) + 8) >> 20;
    companion14Column = *(s32 *)(Func_0200569e(14) + 8) >> 20;

    if (row == 9) {
        if ((u32)(companion14Column - 6) <= 2) {
            goto transition32;
        }
        if ((u32)(companion19Column - 6) <= 2) {
            goto transition80;
        }
        /* This arm runs two transitions back to back. */
        Func_02003da8(18, 0, 64);
        Func_02003db2(18, 0, 96);
    } else if (row == 11) {
        if ((u32)(companion14Column - 6) <= 2) {
            return;
        }
        if ((u32)(companion19Column - 6) <= 2) {
            Func_02003dce(18, 0, 48);
        } else {
            Func_02003dda(18, 0, 128);
        }
    } else if (row == 12) {
        if ((u32)(companion19Column - 6) <= 2) {
transition32:
            Func_02003df0(18, 0, 32);
        } else {
            Func_02003dfc(18, 0, 112);
        }
    } else if (row == 14) {
        if ((u32)(companion19Column - 6) <= 2) {
            return;
        }
transition80:
        Func_02003e12(18, 0, 80);
    } else if (row == 15) {
        Func_02003e22(18, 0, 64);
    } else if (row == 18) {
        Func_02003e32(18, 0, 16);
    } else if (row == 19) {
        return;
    }

    Func_02005682(2);

    column -= 1;
    Func_0200570e(column, row, 3, 1,
                  column, *(s32 *)(Func_02005758(18) + 16) >> 20);
    Func_0200571e(0, 0, 3, 1, column, row);
}

void AdvanceActor9AlongEscapeRoute(void)
{
    s32 column;
    s32 row;
    s32 companion19Column;
    s32 companion14Column;
    s32 companion16Column;

    s32 permuted_6;
    permuted_6 = *(s32 *)(Func_020058dc(9) + 8) >> 20;
    row = *(s32 *)(Func_020058e8(9) + 16) >> 20;
    column  = permuted_6;
    companion19Column = *(s32 *)(Func_020058f2(19) + 8) >> 20;
    companion14Column = *(s32 *)(Func_020058fc(14) + 8) >> 20;
    companion16Column = *(s32 *)(Func_02005906(16) + 8) >> 20;

    if (row == 8) {
        if ((u32)(companion16Column - 9) <= 2) {
            return;
        }
        if ((u32)(companion14Column - 9) <= 2) {
            goto transition48;
        }
        if ((u32)(companion19Column - 9) > 2) {
            Func_0200401a(9, 0, 80);
        }
        /* Falls through into the id-96 site from both paths. */
        Func_02004024(9, 0, 96);
    } else if (row == 11) {
        if ((u32)(companion14Column - 9) <= 2) {
            return;
        }
        if ((u32)(companion19Column - 9) <= 2) {
transition48:
            Func_02004044(9, 0, 48);
        } else {
            Func_02004050(9, 0, 128);
        }
    } else if (row == 12) {
        if ((u32)(companion14Column - 9) <= 2) {
            return;
        }
        if ((u32)(companion19Column - 9) <= 2) {
            Func_02004070(9, 0, 32);
        } else {
            Func_0200407c(9, 0, 112);
        }
    } else if (row == 14) {
        if ((u32)(companion19Column - 9) <= 2) {
            return;
        }
        Func_02004094(9, 0, 80);
    } else if (row == 15) {
        Func_020040a4(9, 0, 64);
    } else if (row == 18) {
        Func_020040b4(9, 0, 16);
    }

    Func_020058fe(2);

    column -= 1;
    Func_0200598c(column, row, 3, 1,
                  column, *(s32 *)(Func_020059d4(9) + 16) >> 20);
    Func_0200599c(0, 0, 3, 1, column, row);
}

void RetreatActor14AlongEscapeRoute(void)
{
    s32 column;
    s32 row;
    s32 companion18Row;
    s32 companion9Row;
    s32 rowM1;

    s32 permuted_7;
    column = *(s32 *)(Func_02005c10(14) + 8) >> 20;
    permuted_7 = *(s32 *)(Func_02005c1a(14) + 16) >> 20;
    companion18Row = *(s32 *)(Func_02005c24(18) + 16) >> 20;
    row  = permuted_7;
    companion9Row = *(s32 *)(Func_02005c2e(9) + 16) >> 20;

    if (column == 13) {
        if ((u32)(companion9Row - 12) <= 2) {
            Func_0200437e(14, -16, 0);
        } else if ((u32)(companion18Row - 12) <= 2) {
            Func_0200437e(14, -64, 0);
        } else {
            Func_0200437e(14, -112, 0);
        }
    } else if (column == 12) {
        if ((u32)(companion9Row - 12) <= 2) {
            return;
        }
        if ((u32)(companion18Row - 12) <= 2) {
            Func_0200437e(14, -48, 0);
        } else {
            Func_0200437e(14, -96, 0);
        }
    } else if (column == 9) {
        if ((u32)(companion18Row - 12) <= 2) {
            return;
        }
        Func_0200437e(14, -48, 0);
    } else if (column == 8) {
        if ((u32)(companion18Row - 12) <= 2) {
            return;
        }
        Func_0200437e(14, -32, 0);
    } else if (column == 6) {
        return;
    }

    Func_02005bce(2);

    rowM1 = row - 1;
    Func_02005c5a(column, rowM1, 1, 3,
                  *(s32 *)(Func_02005ca4(14) + 8) >> 20, rowM1);
    Func_02005c6a(0, 0, 1, 3, column, rowM1);
}

void AdvanceActor14AlongEscapeRoute(void)
{
    s32 column;
    s32 row;
    s32 companion18Row;
    s32 companion9Row;
    s32 rowM1;

    s32 permuted_7;
    column = *(s32 *)(Func_02005cdc(14) + 8) >> 20;
    permuted_7 = *(s32 *)(Func_02005ce6(14) + 16) >> 20;
    companion18Row = *(s32 *)(Func_02005cf0(18) + 16) >> 20;
    row  = permuted_7;
    companion9Row = *(s32 *)(Func_02005cfa(9) + 16) >> 20;

    if (column == 6) {
        if ((u32)(companion9Row - 12) <= 2) {
            Func_0200443c(14, 32, 0);
        } else if ((u32)(companion18Row - 12) <= 2) {
            Func_0200443c(14, 64, 0);
        } else {
            Func_0200443c(14, 112, 0);
        }
    } else if (column == 8) {
        if ((u32)(companion9Row - 12) <= 2) {
            return;
        }
        Func_0200443c(14, 80, 0);
    } else if (column == 9) {
        if ((u32)(companion9Row - 12) <= 2) {
            return;
        }
        Func_0200443c(14, 64, 0);
    } else if (column == 12) {
        Func_0200443c(14, 16, 0);
    } else if (column == 13) {
        return;
    }

    Func_02005c8c(2);

    rowM1 = row - 1;
    Func_02005d18(column, rowM1, 1, 3,
                  *(s32 *)(Func_02005d62(14) + 8) >> 20, rowM1);
    Func_02005d28(0, 0, 1, 3, column, rowM1);
}

void RetreatActor16AlongEscapeRoute(void)
{
    s32 column;
    s32 row;
    s32 companion18Row;
    s32 companion9Row;
    s32 rowM1;

    s32 permuted_7;
    column = *(s32 *)(Func_02005d9c(16) + 8) >> 20;
    permuted_7 = *(s32 *)(Func_02005da6(16) + 16) >> 20;
    companion18Row = *(s32 *)(Func_02005db0(18) + 16) >> 20;
    row  = permuted_7;
    companion9Row = *(s32 *)(Func_02005dba(9) + 16) >> 20;

    if (column == 13) {
        if ((u32)(companion9Row - 9) <= 2) {
            Func_02004502(16, -16, 0);
        } else if ((u32)(companion18Row - 9) <= 2) {
            Func_02004502(16, -64, 0);
        } else {
            Func_02004502(16, -112, 0);
        }
    } else if (column == 12) {
        if ((u32)(companion9Row - 9) <= 2) {
            return;
        }
        if ((u32)(companion18Row - 9) <= 2) {
            Func_02004502(16, -48, 0);
        } else {
            Func_02004502(16, -96, 0);
        }
    } else if (column == 9) {
        if ((u32)(companion18Row - 9) <= 2) {
            return;
        }
        Func_02004502(16, -48, 0);
    } else if (column == 8) {
        Func_02004502(16, -32, 0);
    } else if (column == 6) {
        return;
    }

    Func_02005d52(2);

    rowM1 = row - 1;
    Func_02005dde(column, rowM1, 1, 3,
                  *(s32 *)(Func_02005e28(16) + 8) >> 20, rowM1);
    Func_02005dee(0, 0, 1, 3, column, rowM1);
}

void AdvanceActor16AlongEscapeRoute(void)
{
    s32 column;
    s32 row;
    s32 companionRow;

    s32 permuted_6;
    column = *(s32 *)(Func_02005e60(16) + 8) >> 20;
    permuted_6 = *(s32 *)(Func_02005e6a(16) + 16) >> 20;
    companionRow = *(s32 *)(Func_02005e74(9) + 16) >> 20;
    row  = permuted_6;

    if (column == 6) {
        if ((u32)(companionRow - 9) <= 2) {
            Func_020045a2(16, 32, 0);
        } else {
            Func_020045a2(16, 112, 0);
        }
    } else if (column == 8) {
        if ((u32)(companionRow - 9) <= 2) {
            return;
        }
        Func_020045a2(16, 80, 0);
    } else if (column == 9) {
        Func_020045a2(16, 64, 0);
    } else if (column == 12) {
        Func_020045a2(16, 16, 0);
    } else if (column == 13) {
        return;
    }

    Func_02005df2(2);

    row -= 1;
    Func_02005e7e(column, row, 1, 3,
                  *(s32 *)(Func_02005ec8(16) + 8) >> 20, row);
    Func_02005e8e(0, 0, 1, 3, column, row);
}
