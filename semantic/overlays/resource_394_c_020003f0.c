typedef signed short s16;
typedef signed int s32;
typedef unsigned short u16;

/*
 * Resource 394 scene setup.
 *
 * The entry at 0x020003f0 has its own saved-register/stack prologue and the
 * matching outer return at 0x020007dc.  Several called service addresses fall
 * numerically inside that interval because overlay-local code and resident
 * services share the 0x02000000 address namespace.  They remain calls here:
 * treating their apparent local labels as standalone C owners would invent
 * hidden-context ABIs.
 *
 * The helpers are deliberately left with old-style declarations until their
 * shared interfaces are established from their own implementations.  Calls
 * below preserve every observed argument, including the two or three stack
 * arguments used by the six/seven-argument placement helpers.
 */
void Func_020006de();
void Func_020006f2();
void Func_02000706();
void Func_02000748();
void Func_020007a8();
void Func_020007bc();
void Func_020007d0();
void Func_02000952();
void Func_02001458();
void Func_0200146e();
void Func_02001484();
void Func_02001492();
void Func_0200149c();
void Func_020014a4();
void Func_020014c0();
void Func_020014c2();
void Func_020014d6();
void Func_020014d8();
void Func_020014e6();
void Func_020014ee();
void Func_020014fe();
void Func_02001500();
void Func_02001506();
void Func_0200150c();
void Func_02001514();
void Func_02001516();
void Func_0200151c();
void Func_0200152a();
void Func_0200153a();
void Func_02001540();
void Func_02001556();
void Func_02001568();
void Func_02001578();
void Func_0200157e();
void Func_0200158e();
void Func_02001594();
void Func_020015a4();
void Func_020015aa();
void Func_020015be();
void Func_020015d2();
void Func_020015de();
void Func_020015e2();
void Func_020015f4();
void Func_020015f8();
void Func_0200160a();
void Func_0200160e();
void Func_02001620();
void Func_02001628();
void Func_02001636();
void Func_0200163a();
void Func_0200165e();
void Func_02001674();
void Func_0200168a();
void Func_020016b2();
void Func_020016c2();
void Func_020016c4();
void Func_020016da();
void Func_020016f0();
void Func_020016f4();
void Func_02001794();
void Func_020017a4();
void Func_020017ba();
void Func_020017c6();
void Func_020017ea();
void Func_02001802();
void Func_0200180e();
void Func_0200186a();

/* Resident service entry points whose addresses overlap this overlay image. */
void Func_02000720();
void Func_02000734();
void Func_02000802();
void Func_02000816();
void Func_0200082a();

static s16 scene_variant(void)
{
    volatile s16 *state = *(volatile s16 **)0x020092c8;
    return state[0];
}

static void place(void (*helper)(), s32 id, s32 y, s32 width, s32 kind,
                  s32 height, s32 depth)
{
    helper(id, y, width, kind, height, depth);
}

static void place_panel(void (*helper)(), s32 x, s32 y, s32 width, s32 kind,
                        s32 panel, s32 margin, s32 height)
{
    helper(x, y, width, kind, panel, margin, height);
}

void Func_020003f0(void)
{
    volatile s32 *timer = (volatile s32 *)0x0200a0dc;
    volatile u16 *displayWord;
    s32 variant;

    Func_02001492();
    Func_020014e6(0x10000, 0x4000);
    Func_02001500(0x1080000, -1, 0x1c00000, 1);
    Func_0200150c();
    Func_020014a4(0x1528, 1);
    Func_0200153a(232);

    variant = scene_variant();
    if (variant == 0) {
        Func_02001516(9, 0x1000000, 0x1ce0000);
        place(Func_020014c2, 77, 34, 1, 2, 83, 25);
        Func_02001458(3);
        place(Func_020014d8, 78, 34, 1, 2, 83, 25);
        Func_0200146e(3);
        place(Func_020014ee, 79, 34, 1, 2, 83, 25);
        Func_02001484(30);
        place(Func_02001506, 67, 34, 2, 5, 79, 25);
        Func_0200149c(6);
        place(Func_0200151c, 69, 34, 2, 5, 79, 25);
        Func_02001594(9, 1);
        Func_020015d2(240);
        Func_020014c0(6);
        place(Func_02001540, 71, 34, 2, 5, 79, 25);
        Func_020014d6(6);
        place(Func_02001556, 73, 34, 2, 5, 79, 25);
        place(Func_02001568, 75, 38, 2, 1, 79, 29);
        Func_020014fe(4);
        place(Func_0200157e, 77, 38, 2, 1, 79, 29);
        Func_02001514(6);
        place(Func_02001594, 79, 38, 2, 1, 79, 29);
        Func_0200152a(8);
        place(Func_020015aa, 65, 53, 2, 1, 79, 29);
        place(Func_020015be, 65, 40, 2, 4, 15, 28);
    } else {
        Func_02001636(9, 0x1000000, 0x1e00000);
        place(Func_020015e2, 78, 34, 1, 2, 83, 25);
        Func_02001578(3);
        place(Func_020015f8, 77, 34, 1, 2, 83, 25);
        Func_0200158e(3);
        place(Func_0200160e, 76, 34, 1, 2, 83, 25);
        Func_020015a4(30);
        place(Func_02001628, 65, 45, 2, 4, 15, 28);
        place(Func_0200163a, 71, 50, 2, 5, 79, 25);
        Func_020016b2(9, 2);
        Func_020016f0(230);
        Func_020015de(6);
        place(Func_0200165e, 69, 50, 2, 5, 79, 25);
        Func_020015f4(6);
        place(Func_02001674, 67, 50, 2, 5, 79, 25);
        Func_0200160a(6);
        place(Func_0200168a, 65, 50, 2, 5, 79, 25);
        Func_02001620(30);
    }

    /*
     * These three calls choose the resource-specific placement family.  Their
     * numeric targets overlap this overlay but are resident service entries;
     * no local-label function is asserted here.
     */
    if (variant == 0) {
        place_panel(Func_020006de, 9, 19, 16, 5, 0, 9, 30);
        place_panel(Func_020006f2, 9, 51, 16, 5, 1, 9, 30);
        place_panel(Func_02000706, 41, 51, 16, 5, 2, 9, 30);
    } else {
        place_panel(Func_02000720, 9, 19, 16, 5, 0, 9, 30);
        place_panel(Func_02000734, 9, 83, 16, 5, 1, 9, 30);
        place_panel(Func_02000748, 41, 83, 16, 5, 2, 9, 30);
    }

    *timer = 0;
    Func_020016c4((void *)0x020083c1, 2000);
    Func_020016c2(1);
    Func_020016f4(1, 0, (void *)0x0200836d);
    Func_020017ea(231);
    *timer = 0;
    do {
        Func_020016da(1);
        ++*timer;
    } while (*timer <= 100);
    Func_02001802(0x121);

    variant = scene_variant();
    if (variant == 0) {
        place_panel(Func_020007a8, 9, 19, 16, 5, 0, 9, 19);
        place_panel(Func_020007bc, 9, 51, 16, 5, 1, 9, 19);
        place_panel(Func_020007d0, 41, 51, 16, 5, 2, 9, 19);
    } else {
        place_panel(Func_02000802, 9, 19, 16, 5, 0, 9, 19);
        place_panel(Func_02000816, 9, 83, 16, 5, 1, 9, 19);
        place_panel(Func_0200082a, 41, 83, 16, 5, 2, 9, 19);
    }

    Func_02001794(1);
    Func_020017c6(1, 0, 0);
    Func_020017a4(1);
    Func_020017ba((void *)0x020083c1);

    displayWord = *(volatile u16 **)0x020092c8;
    *displayWord ^= 1;
    Func_02000952();
    Func_0200180e();
    Func_0200186a();
}
