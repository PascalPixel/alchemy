#include "types.h"
#include "scene.h"
#include "abi/overlays/scene/story/command_list/command_list_scene.h"

extern s16 gCell[];
extern volatile s32 gIw;
extern u8 Value_00000c9b;
extern u8 Value_00000cc6;
extern u8 Value_00000cf1;
extern u8 Value_00000d21;
extern u8 Value_00000d4c;
extern u8 Value_00000d77;
extern u8 Value_00000da2;
extern u8 *gIw2[];
extern u8 gOv[];
extern u8 gOv2[];
extern u8 gWork[];

/* Scene state, dialogue and command-table steps for resource_3ce. */

/*
 * Table getter at 0x02000030. The eight-byte owner includes its one pool word
 * at 0x02000034, which holds 0x020093c8; the pc-relative load reads it. The
 * word is an address, returned without being dereferenced.
 */

/*
 * Table getter at 0x0200003c. The eight-byte owner includes its one pool word
 * at 0x02000040, which holds 0x020093f8; the pc-relative load reads it. The
 * word is an address, returned without being dereferenced.
 */

/*
 * Table getter at 0x02000044. The eight-byte owner includes its one pool word
 * at 0x02000048, which holds 0x020093fc; the pc-relative load reads it. The
 * word is an address, returned without being dereferenced.
 */

/* Step by one until the query returns non-zero. The test sits at the bottom of
 * the loop and entry jumps to it. */

/* Each symbol here is the per-site call word the overlay image holds -- one
 * word can serve two sites with different targets -- and the macro keeps the
 * site's own calling form. The names themselves are provisional. */

/* Runs one setup call, then a long table of two-argument calls each passing
 * a slot index (0-3) and an associated code value, and finishes with a few
 * index-only calls. */

u8 *SceneData_GetTable93c8(void)
{
    return (u8 *)0x020093c8;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

u8 *SceneData_GetTable93f8(void)
{
    return (u8 *)0x020093f8;
}

u8 *SceneData_GetTable93fc(void)
{
    return (u8 *)0x020093fc;
}

void Dialogue_ShowMessageAndWait(s32 arg0)
{
    Story_Run4();
    Story_SetMode(arg0, 5, 0, 0x22);
    while (Story_Check10() == 0) {
        Story_Do(1);
    }
    Story_Do2(1);
}

void CommandTable_RunDirectionalInput(s32 x, s32 cnt)
{
    s16 *tbl = gCell;
    volatile s32 *key;
    s32 token;
    s32 i;

    *(u8 *)&tbl[262] = 2;
    token = Story_SetMode2(125, 0, 0, 0);
    for (i = 0; i < cnt; i++) {
        key = &gIw;
        Story_Apply(1, 1);
        Story_Apply2(141, 2);
        Story_Apply3(0x1e240, 5);
        Story_Do3(x);
        goto test;
retry:
        if (*key != 0) {
            goto next;
        }
        Story_Do4(1);
test:
        if ((*key & 2) != 0) {
            goto end;
        }
        if ((*key & 1) != 0) {
            goto inc;
        }
        if ((*key & 0x80) == 0) {
            goto other;
        }
inc:
        x++;
        goto next;
other:
        if ((*key & 0x40) != 0) {
            x--;
            goto next;
        }
        goto retry;
next:;
    }
end:
    Story_Run5();
    Story_Apply4(token, 2);
}

void State_ApplyBlockC9b(void)
{
    Story_Check11((s32)&Value_00000c9b, (s32)&Value_00000cc6 - (s32)&Value_00000c9b);
}

void State_ApplyBlockCc6(void)
{
    Story_Check12((s32)&Value_00000cc6, (s32)&Value_00000cc6 - (s32)&Value_00000c9b);
}

void State_ApplyBlockCf1(void)
{
    Story_Check13((s32)&Value_00000cf1, (s32)&Value_00000cc6 - (s32)&Value_00000c9b);
}

void State_ApplyBlockD21(void)
{
    Story_Check14((s32)&Value_00000d21, (s32)&Value_00000d4c - (s32)&Value_00000d21);
}

void State_ApplyBlockD4c(void)
{
    Story_Check15((s32)&Value_00000d4c, (s32)&Value_00000cc6 - (s32)&Value_00000c9b);
}

void State_ApplyBlockD77(void)
{
    Story_Check16((s32)&Value_00000d77, (s32)&Value_00000cc6 - (s32)&Value_00000c9b);
}

void State_ApplyBlockDa2(void)
{
    Story_Check17((s32)&Value_00000da2, (s32)&Value_00000cc6 - (s32)&Value_00000c9b);
}

void State_ApplyOne(void)
{
    Story_Check18(1);
}

void State_NoOp(void)
{
}

void State_QueryTwoValues(void)
{
    s32 a;
    s32 b;
    Story_Check19(&a, &b);
}

void State_ApplyZero(void)
{
    Story_Check20(0);
}

void CommandTable_NoOpCallback(void)
{
}

void State_SetRecordFlag53(void)
{
    gIw2[0][0x35] = 1;
}

s32 SceneData_GetTable9564(void)
{
    return (s32)gOv;
}

void Scene_ApplyTable9684ValueToFourSlots(void)
{
    u32 i;
    s32 record;
    s32 p;

    Story_Run(0xc1a, 1);
    p = (s32)gOv2;
    Story_Run6(0, *(volatile s32 *)p);
    Story_Check(1, *(volatile s32 *)p);
    Story_Check2(3, *(volatile s32 *)p);
    Story_Run7(2, *(volatile s32 *)p);
    *(volatile s32 *)p += 10;
    Story_Run8(0);
    Story_Run9(1);
    Story_Run10(3);
    Story_Run11(2);
}

void Scene_GrantItemListToSlots(void)
{
    u32 tmp;
    s32 slot;

    UiText_DrawMessage_1(0xc1e, 1);
    Inventory_AddForOwner_1(0, 187);
    Inventory_AddForOwner_2(0, 187);
    Inventory_AddForOwner_3(0, 187);
    Inventory_AddForOwner_4(0, 187);
    Inventory_AddForOwner_5(0, 187);
    Inventory_AddForOwner_6(0, 187);
    Inventory_AddForOwner_7(0, 187);
    Inventory_AddForOwner_8(0, 187);
    Inventory_AddForOwner_9(0, 187);
    Inventory_AddForOwner_10(0, 187);
    Inventory_AddForOwner_11(0, 187);
    Inventory_AddForOwner_12(0, 187);
    Inventory_AddForOwner_13(0, 187);
    Inventory_AddForOwner_14(0, 187);
    Inventory_AddForOwner_15(0, 187);
    Inventory_AddForOwner_16(0, 187);
    Inventory_AddForOwner_17(0, 187);
    Inventory_AddForOwner_18(0, 180);
    Inventory_AddForOwner_19(0, 180);
    Inventory_AddForOwner_20(0, 180);
    Inventory_AddForOwner_21(0, 180);
    Inventory_AddForOwner_22(0, 180);
    Inventory_AddForOwner_23(0, 180);
    Inventory_AddForOwner_24(0, 180);
    Inventory_AddForOwner_25(0, 180);
    Inventory_AddForOwner_26(0, 180);
    Inventory_AddForOwner_27(0, 180);
    Inventory_AddForOwner_28(0, 180);
    Inventory_AddForOwner_29(0, 180);
    Inventory_AddForOwner_30(0, 180);
    Inventory_AddForOwner_31(0, 180);
    Inventory_AddForOwner_32(0, 181);
    Inventory_AddForOwner_33(0, 181);
    Inventory_AddForOwner_34(0, 181);
    Inventory_AddForOwner_35(0, 181);
    Inventory_AddForOwner_36(0, 181);
    Inventory_AddForOwner_37(0, 181);
    Inventory_AddForOwner_38(0, 181);
    Inventory_AddForOwner_39(0, 181);
    Inventory_AddForOwner_40(0, 181);
    Inventory_AddForOwner_41(0, 181);
    Inventory_AddForOwner_42(0, 181);
    Inventory_AddForOwner_43(0, 182);
    Inventory_AddForOwner_44(0, 182);
    Inventory_AddForOwner_45(0, 182);
    Inventory_AddForOwner_46(0, 182);
    Inventory_AddForOwner_47(0, 182);
    Inventory_AddForOwner_48(0, 182);
    Inventory_AddForOwner_49(0, 182);
    Inventory_AddForOwner_50(0, 182);
    Inventory_AddForOwner_51(0, 182);
    Inventory_AddForOwner_52(0, 182);
    Inventory_AddForOwner_53(0, 182);
    Inventory_AddForOwner_54(0, 182);
    Inventory_AddForOwner_55(0, 182);
    Inventory_AddForOwner_56(0, 183);
    Inventory_AddForOwner_57(0, 183);
    Inventory_AddForOwner_58(0, 183);
    Inventory_AddForOwner_59(0, 183);
    Inventory_AddForOwner_60(0, 183);
    Inventory_AddForOwner_61(0, 183);
    Inventory_AddForOwner_62(0, 183);
    Inventory_AddForOwner_63(0, 183);
    Inventory_AddForOwner_64(0, 183);
    Inventory_AddForOwner_65(1, 186);
    Inventory_AddForOwner_66(1, 186);
    Inventory_AddForOwner_67(1, 186);
    Inventory_AddForOwner_68(1, 186);
    Inventory_AddForOwner_69(1, 186);
    Inventory_AddForOwner_70(1, 186);
    Inventory_AddForOwner_71(1, 186);
    Inventory_AddForOwner_72(1, 186);
    Inventory_AddForOwner_73(1, 186);
    Inventory_AddForOwner_74(1, 186);
    Inventory_AddForOwner_75(1, 186);
    Inventory_AddForOwner_76(1, 186);
    Inventory_AddForOwner_77(1, 186);
    Inventory_AddForOwner_78(1, 187);
    Inventory_AddForOwner_79(1, 187);
    Inventory_AddForOwner_80(1, 187);
    Inventory_AddForOwner_81(1, 187);
    Inventory_AddForOwner_82(1, 187);
    Inventory_AddForOwner_83(1, 187);
    Inventory_AddForOwner_84(1, 187);
    Inventory_AddForOwner_85(1, 187);
    Inventory_AddForOwner_86(1, 187);
    Inventory_AddForOwner_87(1, 187);
    Inventory_AddForOwner_88(1, 187);
    Inventory_AddForOwner_89(1, 187);
    Inventory_AddForOwner_90(2, 188);
    Inventory_AddForOwner_91(2, 188);
    Inventory_AddForOwner_92(2, 188);
    Inventory_AddForOwner_93(2, 188);
    Inventory_AddForOwner_94(2, 188);
    Inventory_AddForOwner_95(2, 188);
    Inventory_AddForOwner_96(2, 188);
    Inventory_AddForOwner_97(2, 188);
    Inventory_AddForOwner_98(2, 188);
    Inventory_AddForOwner_99(2, 188);
    Inventory_AddForOwner_100(2, 188);
    Inventory_AddForOwner_101(2, 188);
    Inventory_AddForOwner_102(2, 188);
    Inventory_AddForOwner_103(2, 188);
    Inventory_AddForOwner_104(2, 189);
    Inventory_AddForOwner_105(2, 189);
    Inventory_AddForOwner_106(2, 189);
    Inventory_AddForOwner_107(2, 189);
    Inventory_AddForOwner_108(2, 189);
    Inventory_AddForOwner_109(2, 189);
    Inventory_AddForOwner_110(2, 189);
    Inventory_AddForOwner_111(2, 189);
    Inventory_AddForOwner_112(2, 189);
    Inventory_AddForOwner_113(2, 189);
    Inventory_AddForOwner_114(2, 189);
    Inventory_AddForOwner_115(2, 236);
    Inventory_AddForOwner_116(2, 236);
    Inventory_AddForOwner_117(2, 236);
    Inventory_AddForOwner_118(2, 236);
    Inventory_AddForOwner_119(2, 236);
    Inventory_AddForOwner_120(2, 236);
    Inventory_AddForOwner_121(2, 236);
    Inventory_AddForOwner_122(2, 236);
    Inventory_AddForOwner_123(2, 236);
    Inventory_AddForOwner_124(2, 236);
    Inventory_AddForOwner_125(2, 236);
    Inventory_AddForOwner_126(2, 236);
    Inventory_AddForOwner_127(2, 236);
    Inventory_AddForOwner_128(2, 236);
    Inventory_AddForOwner_129(2, 236);
    Inventory_AddForOwner_130(2, 236);
    Inventory_AddForOwner_131(3, 191);
    Inventory_AddForOwner_132(3, 191);
    Inventory_AddForOwner_133(3, 191);
    Inventory_AddForOwner_134(3, 191);
    Inventory_AddForOwner_135(3, 191);
    Inventory_AddForOwner_136(3, 191);
    Inventory_AddForOwner_137(3, 191);
    Inventory_AddForOwner_138(3, 191);
    Inventory_AddForOwner_139(3, 191);
    Inventory_AddForOwner_140(3, 191);
    Inventory_AddForOwner_141(3, 191);
    Inventory_AddForOwner_142(3, 192);
    Inventory_AddForOwner_143(3, 192);
    Inventory_AddForOwner_144(3, 192);
    Inventory_AddForOwner_145(3, 192);
    Inventory_AddForOwner_146(3, 192);
    Inventory_AddForOwner_147(3, 192);
    Inventory_AddForOwner_148(3, 192);
    Inventory_AddForOwner_149(3, 192);
    Inventory_AddForOwner_150(3, 192);
    Inventory_AddForOwner_151(3, 192);
    Inventory_AddForOwner_152(3, 192);
    Inventory_AddForOwner_153(3, 193);
    Inventory_AddForOwner_154(3, 193);
    Inventory_AddForOwner_155(3, 193);
    Inventory_AddForOwner_156(3, 193);
    Inventory_AddForOwner_157(3, 193);
    Inventory_AddForOwner_158(3, 193);
    Inventory_AddForOwner_159(3, 193);
    Inventory_AddForOwner_160(3, 193);
    Inventory_AddForOwner_161(3, 193);
    Inventory_AddForOwner_162(3, 193);
    Inventory_AddForOwner_163(3, 193);
    Inventory_AddForOwner_164(3, 194);
    Inventory_AddForOwner_165(3, 194);
    Inventory_AddForOwner_166(3, 194);
    Inventory_AddForOwner_167(3, 194);
    Inventory_AddForOwner_168(3, 194);
    Inventory_AddForOwner_169(3, 194);
    Inventory_AddForOwner_170(3, 194);
    Inventory_AddForOwner_171(3, 195);
    Inventory_AddForOwner_172(3, 195);
    Inventory_AddForOwner_173(3, 195);
    Inventory_AddForOwner_174(3, 195);
    Inventory_AddForOwner_175(3, 195);
    Inventory_AddForOwner_176(3, 195);
    Inventory_AddForOwner_177(3, 195);
    Inventory_AddForOwner_178(3, 195);
    Inventory_AddForOwner_179(3, 195);
    Inventory_AddForOwner_180(3, 195);
    Inventory_AddForOwner_181(3, 195);
    Inventory_AddForOwner_182(3, 195);
    Inventory_AddForOwner_183(3, 195);
    Inventory_AddForOwner_184(3, 196);
    Inventory_AddForOwner_185(3, 196);
    Inventory_AddForOwner_186(3, 196);
    Inventory_AddForOwner_187(3, 196);
    Inventory_AddForOwner_188(3, 196);
    Inventory_AddForOwner_189(3, 196);
    Inventory_AddForOwner_190(3, 196);
    Inventory_AddForOwner_191(3, 196);
    BattleUnit_Recalculate_1(0);
    BattleUnit_Recalculate_2(1);
    BattleUnit_Recalculate_3(3);
    BattleUnit_Recalculate_4(2);
}

void CommandTable_ConfigureCommandGroups(void)
{
    u8 buf[256];
    Story_Run12(0xc1d, 1);
    Story_Check21(0, 0, 0);
    Story_Check22(0, 0, 1);
    Story_Check23(0, 0, 2);
    Story_Check24(0, 0, 3);
    Story_Check25(0, 0, 4);
    Story_Check26(0, 0, 5);
    Story_Check27(0, 0, 6);
    Story_Check28(0, 0, 0);
    Story_Check29(0, 0, 1);
    Story_Check30(0, 0, 2);
    Story_Check31(0, 0, 3);
    Story_Check32(0, 0, 4);
    Story_Check33(0, 0, 5);
    Story_Check34(0, 0, 6);
    Story_Check35(1, 2, 0);
    Story_Check36(1, 2, 1);
    Story_Check37(1, 2, 2);
    Story_Check38(1, 2, 3);
    Story_Check39(1, 2, 4);
    Story_Check40(1, 2, 5);
    Story_Run13(1, 2, 6);
    Story_Check41(1, 2, 0);
    Story_Check42(1, 2, 1);
    Story_Check43(1, 2, 2);
    Story_Check44(1, 2, 3);
    Story_Check45(1, 2, 4);
    Story_Check46(1, 2, 5);
    Story_Check47(1, 2, 6);
    Story_Check48(3, 1, 0);
    Story_Check49(3, 1, 1);
    Story_Check50(3, 1, 2);
    Story_Check51(3, 1, 3);
    Story_Check52(3, 1, 4);
    Story_Check53(3, 1, 5);
    Story_Check54(3, 1, 6);
    Story_Check55(3, 1, 0);
    Story_Check56(3, 1, 1);
    Story_Check57(3, 1, 2);
    Story_Check58(3, 1, 3);
    Story_Check59(3, 1, 4);
    Story_Check60(3, 1, 5);
    Story_Check61(3, 1, 6);
    Story_Check62(2, 3, 0);
    Story_Check63(2, 3, 1);
    Story_Check64(2, 3, 2);
    Story_Check65(2, 3, 3);
    Story_Check66(2, 3, 4);
    Story_Check67(2, 3, 5);
    Story_Check68(2, 3, 0);
    Story_Check69(2, 3, 1);
    Story_Check70(2, 3, 2);
    Story_Check71(2, 3, 3);
    Story_Check72(2, 3, 4);
    Story_Check73(2, 3, 5);
    Story_Check74(0);
    Story_Check75(1);
    Story_Check76(3);
    Story_Check77(2);
}

void Scene_ApplySlotOffsetsAndFlags(void)
{
    u32 i;
    s32 record;

    Story_Run2(0xc1b, 1);
    Story_Check3(0, -100);
    Story_Check4(1, -100);
    Story_Check5(2, -33);
    Story_Check6(3, -100);
    Story_Check7(0, -50);
    Story_Check8(1, -40);
    Story_Check9(2, -35);
    Story_Run3(3, -20);
    record = Story_Check78(0);
    *(u8 *)(record + 0x131) = 1;
    record = record + 0x140;
    *(u8 *)record = 1;
    record = Story_Check79(1);
    *(u8 *)((record + 0x130)) = 1;
    *(u8 *)(record + 0x131) = 2;
    Story_Run14(0);
    Story_Run15(1);
    Story_Run16(3);
    Story_Run17(2);
}

void Scene_AssignCodeSetAToSlots(void)
{
    Story_Run18(0xc1f, 1);
    Story_Check80(0, 85);
    Story_Check81(0, 84);
    Story_Check82(0, 124);
    Story_Check83(0, 123);
    Story_Check84(0, 9);
    Story_Check85(0, 11);
    Story_Check86(0, 27);
    Story_Check87(0, 26);
    Story_Check88(1, 38);
    Story_Check89(1, 37);
    Story_Check90(1, 50);
    Story_Check91(1, 49);
    Story_Check92(1, 83);
    Story_Check93(1, 82);
    Story_Check94(1, 134);
    Story_Check95(1, 133);
    Story_Check96(1, 152);
    Story_Check97(2, 64);
    Story_Check98(2, 65);
    Story_Check99(2, 98);
    Story_Check100(2, 97);
    Story_Check101(2, 124);
    Story_Check102(2, 131);
    Story_Check103(2, 141);
    Story_Check104(2, 163);
    Story_Check105(3, 61);
    Story_Check106(3, 63);
    Story_Check107(3, 96);
    Story_Check108(3, 95);
    Story_Check109(3, 113);
    Story_Check110(3, 112);
    Story_Check111(3, 130);
    Story_Check112(3, 142);
    Story_Check113(3, 171);
    Story_Check114(0);
    Story_Check115(1);
    Story_Check116(3);
    Story_Check117(2);
}

void State_RunCall1c00(void)
{
    void Story_Run19(void);

    Story_Run19();
}

void Scene_AssignCodeSetBToSlots(void)
{
    Story_Run20(0xc1c, 1);
    Story_Check118(0, 0xb8);
    Story_Check119(0, 0xcc);
    Story_Check120(0, 0xdc);
    Story_Check121(0, 0xdd);
    Story_Check122(0, 0xde);
    Story_Check123(0, 0xdf);
    Story_Check124(0, 0xe0);
    Story_Check125(1, 0xe2);
    Story_Check126(1, 0xe3);
    Story_Check127(1, 0xe6);
    Story_Check128(1, 0xe4);
    Story_Check129(1, 0xe4);
    Story_Check130(1, 0xe4);
    Story_Check131(1, 0xe4);
    Story_Check132(1, 0xe4);
    Story_Check133(1, 0xe4);
    Story_Check134(1, 0xe4);
    Story_Check135(1, 0xe4);
    Story_Check136(1, 0xe4);
    Story_Check137(1, 0xe4);
    Story_Check138(1, 0xe4);
    Story_Check139(1, 0xe5);
    Story_Check140(1, 0xe5);
    Story_Check141(1, 0xe5);
    Story_Check142(1, 0xe5);
    Story_Check143(1, 0xe5);
    Story_Check144(1, 0xe5);
    Story_Check145(1, 0xe5);
    Story_Check146(1, 0xe5);
    Story_Check147(1, 0xe8);
    Story_Check148(1, 0xe7);
    Story_Check149(1, 0xed);
    Story_Check150(2, 0xf2);
    Story_Check151(2, 0x102);
    Story_Check152(2, 0x10b);
    Story_Check153(2, 0x109);
    Story_Check154(2, 0xfc);
    Story_Check155(3, 0xbd);
    Story_Check156(3, 0xc8);
    Story_Check157(3, 0xc9);
    Story_Check158(3, 0xca);
    Story_Check159(3, 0xcb);
    Story_Check160(3, 0xcc);
    Story_Check161(3, 0xcf);
    Story_Check162(0);
    Story_Check163(1);
    Story_Check164(3);
    Story_Check165(2);
}

s32 CommandTable_ConfigureCommandList(void)
{
    Story_Check166(5);
    Story_Check167(1);
    Story_Check168(3);
    Story_Check169(2);
    Story_Check170(5, 1);
    Story_Check171(5, 1);
    Story_Check172(5, 1);
    Story_Check173(6, 1);
    Story_Check174(6, 1);
    Story_Check175(7, 1);
    Story_Check176(106, 1);
    Story_Check177(108, 1);
    Story_Check178(109, 1);
    Story_Check179(113, 1);
    Story_Check180(123, 1);
    Story_Check181(130, 1);
    Story_Check182(140, 1);
    Story_Check183(151, 1);
    Story_Apply5(0, 50);
    Story_Apply6(1, 30);
    Story_Apply7(3, 30);
    Story_Check184(2, 30);
    Story_Check185(0);
    Story_Check186(1);
    Story_Check187(3);
    Story_Check188(2);
    return 0;
}

s32 State_GetFarResult2384(void)
{
    return Story_Check189();
}

s32 State_GetFarResult2418(void)
{
    return Story_Check190();
}
