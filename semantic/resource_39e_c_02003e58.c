#include "types.h"

/*
 * resource_39e owner at 0x02003e58, 644 bytes: THE OVERLAY'S ENTRY
 * DRIVER, and the reason this overlay's closure was false -- the
 * function the loader actually calls was owned by nobody while the
 * rows it leads to were drafted.
 *
 * It is the target of this overlay's header veneer (the word at image
 * offset 4; `overlay overlay_driver resource_39e`), so nothing
 * inside the image calls it.
 *
 * Top split is on the s16 scene id at 0x02000240+448:
 *
 * - scene 61 (0x3d), the overlay's own scene: write 521 to the
 *   workspace's +448 s32, then branch on the sub-selector s16 at
 *   0x02000240+450 --
 *     1: flag 0x88f set -> set actor 8 to state 6 and finish; clear ->
 *        set actor 8 to state 5, then run the conditional scene beat only
 *        if 0xf14 is set and 0x893 and 0x109 are both clear;
 *     2 or 4 (shared arm): raise flag 0x12f, then stage record 19
 *        (clear +85, +12 and +60 = 0xc0000, +24 = 0xcccc,
 *        +28 = 0x8000, and 0x8000 into +30 of the record its +80
 *        points at).  Flag 0x895 clear takes the placement path --
 *        with 0x89a set and 0x89b clear it places actors 18 and 16 and
 *        hangs the callback constant 0x02008325 on records
 *        18/13/14/15/16 at +108; 0x895 set instead sets bit 3 of +89
 *        and pops panel (14, 11, 1, 1, 14, 10).  Both meet at the same
 *        tail: configure the scene view with
 *        (0x1300000, 0x180000, 0xe00000, 223), then set actors 10 and 11
 *        to state 5;
 *     3: raise flag 0x12f; with 0x895 clear run the exact 5000-byte roof
 *        repair scene in this overlay's former gap, otherwise,
 *        if 0x8b2 is clear, park actors 8 and 9 at the origin.
 *
 * - any other scene: open fallback scene 170, set bit 4 of record 9's +89,
 *   then two independent panel pops -- one when the sub-selector is 3
 *   with 0xf14 set and 0x894 clear, one when 0x892 is set (which also
 *   places and halts actor 9).
 *
 * Complete owner: `push {r5, lr}` at 0x02003e58 with `sub sp, #8`
 * through `movs r0, #0 / add sp, #8 / pop {r5} / pop {r1} / bx r1` at
 * 0x02004092-0x0200409a, then the sixteen-word literal pool
 * 0x0200409c-0x020040db; next owner (resource_39e_c_020040dc.c,
 * already drafted) exactly at 0x020040dc.  Returns a literal 0.
 *
 * The +448 / +450 split in the 0x02000240 global is the same one
 * resource_3a4's driver uses: +448 selects the scene, +450
 * sub-selects within it.  Both are recorded by offset.
 *
 * Uncertainty: the flag ids are literal; the record fields (+12, +24,
 * +28, +60, +85, +89, +108, and +30 of the record at +80) are
 * transcribed by offset with roles open; 0x02008325 is an in-image
 * code pointer (image offset 0x324 | 1) installed as a callback, not
 * called here.
 */

extern s16 Data_02000240[];
extern u8 *Data_03001ebc;
extern u8 Value_0000003d;

/*
 * The overlay's BL sites call local relocation veneers, not the logical
 * 0x080xxxxx imports printed by overlay_show.  These aliases are named from
 * the raw Thumb targets in the released overlay.  A suffix distinguishes two
 * signatures that share one relocation address.
 */
extern s32 Func_0200824c();
extern void Func_020082f0();
extern void Func_020082fa();
extern s32 Func_02008268();
extern s32 Func_02008274();
extern s32 Func_02008280();
extern void Func_020066ba();
extern void Func_020082aa();
extern s32 Func_020082a0();
extern u8 *Func_020082f4();
extern s32 Func_020082ce();
extern void Func_02008370();
extern s32 Func_020082e6();
extern void Func_02008388();
extern u8 *Func_02008346();
extern u8 *Func_02008350();
extern u8 *Func_02008358();
extern u8 *Func_02008360();
extern u8 *Func_02008368();
extern u8 *Func_02008372();
extern void Func_0200834c();
extern void *Func_0200400a();
extern void Func_0200841e();
extern void Func_02008426();
extern void Func_020083aa();
extern s32 Func_020083a0();
extern void Func_02006abc();
extern s32 Func_020083b0();
extern void Func_0200844e();
extern void Func_02008458();
extern void Func_02008578();
extern u8 *Func_0200841e_b();
extern s32 Func_020083f4();
extern s32 Func_020083fe();
extern void Func_020083f6();
extern s32 Func_0200841c();
extern void Func_020084be(s32 actor, s32 x, s32 y);
extern void Func_02008540();
extern void Func_0200842c();

/*
 * Scene vocabulary over the raw per-call relocation witnesses above.  Several
 * logical engine calls intentionally have distinct aliases because this
 * overlay reaches them through different veneers; keeping that distinction in
 * the declarations preserves the linked call targets without leaking address
 * names into the scene logic.
 */
#define IsFlag088fSet                 Func_0200824c
#define SetActor8State6               Func_020082f0
#define SetActor8State5               Func_020082fa
#define IsFlag0f14Set                 Func_02008268
#define IsFlag0893Set                 Func_02008274
#define IsFlag0109Set                 Func_02008280
#define RunConditionalSceneBeat       Func_020066ba
#define SetFlag012fForPlacement       Func_020082aa
#define IsFlag0895SetForPlacement     Func_020082a0
#define GetActor19ForPlacement        Func_020082f4
#define IsFlag089aSet                 Func_020082ce
#define PlaceActor18                  Func_02008370
#define IsFlag089bSet                 Func_020082e6
#define PlaceActor16                  Func_02008388
#define GetActor18ForCallback         Func_02008346
#define GetActor13ForCallback         Func_02008350
#define GetActor14ForCallback         Func_02008358
#define GetActor15ForCallback         Func_02008360
#define GetActor16ForCallback         Func_02008368
#define GetActor19ForPanel            Func_02008372
#define ShowPlacementPanel            Func_0200834c
#define ConfigureSceneView            Func_0200400a
#define SetActor10State5              Func_0200841e
#define SetActor11State5              Func_02008426
#define SetFlag012fForRoofScene       Func_020083aa
#define IsFlag0895SetForRoofScene     Func_020083a0
#define RunRoofRepairScene            Func_02006abc
#define IsFlag08b2Set                 Func_020083b0
#define PlaceActor8                   Func_0200844e
#define PlaceActor9AtOrigin           Func_02008458
#define OpenFallbackScene             Func_02008578
#define GetFallbackActor9             Func_0200841e_b
#define IsFallbackFlag0f14Set         Func_020083f4
#define IsFlag0894Set                 Func_020083fe
#define ShowFallbackPanel84           Func_020083f6
#define IsFlag0892Set                 Func_0200841c
#define PlaceFallbackActor9           Func_020084be
#define HaltFallbackActor9            Func_02008540
#define ShowFallbackPanel26           Func_0200842c
#define SceneState                    Data_02000240
#define SceneWorkspace                Data_03001ebc
#define RunResource39eSceneDriver     Func_02003e58

s32 RunResource39eSceneDriver(void)
{
    u8 *record;
    if (SceneState[224] == (s32)&Value_0000003d) {
        *(s32 *)(SceneWorkspace + 448) = 521;
        if (SceneState[225] == 1) {
            if (IsFlag088fSet(0x88f) != 0) {
                SetActor8State6(8, 6);
            } else {
                SetActor8State5(8, 5);
                if (IsFlag0f14Set(0xf14) != 0
                    && IsFlag0893Set(0x893) == 0
                    && IsFlag0109Set(0x109) == 0) {
                    RunConditionalSceneBeat();
                }
            }
        } else if (SceneState[225] == 2 || SceneState[225] == 4) {
            SetFlag012fForPlacement(0x12f);
            if (IsFlag0895SetForPlacement(0x895) == 0) {
                record = GetActor19ForPlacement(19);
                record[85] = 0;
                *(s32 *)(record + 12) = 0xc0000;
                *(s32 *)(record + 60) = 0xc0000;
                *(s32 *)(record + 24) = 0xcccc;
                *(s32 *)(record + 28) = 0x8000;
                *(u16 *)(*(u8 **)(record + 80) + 30) = 0x8000;
                if (IsFlag089aSet(0x89a) != 0) {
                    PlaceActor18(18, 0xf80000, 0xd00000);
                    if (IsFlag089bSet(0x89b) == 0) {
                        PlaceActor16(16, 0x1000000, 0xf00000);
                        *(s32 *)(GetActor18ForCallback(18) + 108) = 0x02008325;
                        *(s32 *)(GetActor13ForCallback(13) + 108) = 0x02008325;
                        *(s32 *)(GetActor14ForCallback(14) + 108) = 0x02008325;
                        *(s32 *)(GetActor15ForCallback(15) + 108) = 0x02008325;
                        *(s32 *)(GetActor16ForCallback(16) + 108) = 0x02008325;
                    }
                }
            } else {
                record = GetActor19ForPanel(19);
                record[85] = 0;
                *(s32 *)(record + 12) = 0xc0000;
                *(s32 *)(record + 60) = 0xc0000;
                *(s32 *)(record + 24) = 0xcccc;
                *(s32 *)(record + 28) = 0x8000;
                {
                    volatile u8 *flag = record + 89;
                    u8 old = *flag;
                    u8 bits = 8;
                    *flag = bits | old;
                }
                *(u16 *)(*(u8 **)(record + 80) + 30) = 0x8000;
                {
                    s32 stack_arg0 = 14;
                    s32 stack_arg1 = 10;
                    ShowPlacementPanel(14, 11, 1, 1, stack_arg0, stack_arg1);
                }
            }
            ConfigureSceneView(0x1300000, 0x180000, 0xe00000, 223);
            SetActor10State5(10, 5);
            SetActor11State5(11, 5);
        } else if (SceneState[225] == 3) {
            SetFlag012fForRoofScene(0x12f);
            if (IsFlag0895SetForRoofScene(0x895) == 0) {
                RunRoofRepairScene();
            } else if (IsFlag08b2Set(0x8b2) == 0) {
                PlaceActor8(8, 0, 0);
                PlaceActor9AtOrigin(9, 0, 0);
            }
        }
        return 0;
    }

    OpenFallbackScene(170);
    {
        volatile u8 *flag = GetFallbackActor9(9) + 89;
        u8 old = flag[0];
        u8 bits = 16;
        flag[0] = bits | old;
    }
    if (SceneState[225] == 3
        && IsFallbackFlag0f14Set(0xf14) != 0
        && IsFlag0894Set(0x894) == 0) {
        {
            s32 stack_arg0 = 10;
            s32 stack_arg1 = 24;
            ShowFallbackPanel84(10, 84, 1, 1, stack_arg0, stack_arg1);
        }
    }
    if (IsFlag0892Set(0x892) != 0) {
        {
            s32 actor = 9;
            s32 x;
            s32 y;
            x = 0x98;
            y = 0xc4;
            x <<= 16;
            y <<= 17;
            PlaceFallbackActor9(actor, x, y);
        }
        HaltFallbackActor9(9, 0, 0);
        {
            s32 stack_arg0 = 10;
            s32 stack_arg1 = 22;
            ShowFallbackPanel26(10, 26, 1, 1, stack_arg0, stack_arg1);
        }
    }
    return 0;
}
