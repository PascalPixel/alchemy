/* NONMATCHING: resource_3bc at 0x0200bddc (276 bytes with its pool),
 * ColossoLogRollingStage_PositionActiveActor, after
 * FIELD/KOROSSEO_MARUTA/SCENE_EFFECT.C, stays listing.
 *
 * Remaining difference: it reaches the resident pointer at 0x03001f3c, which
 * the main image does not name yet, and a script at 0x0200db24.
 */
#include "SITES.H"

s32 ColossoLogRollingStage_PositionActiveActor(s32 first_handle, s32 second_handle)
{
    extern u8 Data_02000240[];

    u8 *workspace = *(u8 **)0x03001f3c;
    u8 *shared;
    u8 *record;
    s32 flag;
    s32 x;
    s32 z;
    u16 *cuep;
    s16 *waitp;

    flag = GameFlag_IsSet(0x211);

    shared = Data_02000240;
    record = Func_02008818_a(*(s32 *)(shared + 500));

    if (*(s32 *)(workspace + 232) < *(s32 *)(record + 8)) {
        x = *(s32 *)(workspace + 232) + 0xc0000;
    } else {
        x = *(s32 *)(workspace + 232) - 0xc0000;
    }

    if (flag != 0) {
        z = *(s32 *)(workspace + 236) + 0x100000;
        cuep = (u16 *)(workspace + 228);
    } else {
        z = *(s32 *)(workspace + 236) - 0x100000;
        cuep = (u16 *)(workspace + 226);
    }

    waitp = (s16 *)(record + 100);
    *waitp = *cuep;
    *(s32 *)(record + 52) = 0x4000;
    *(s32 *)(record + 48) = 0x10000;

    Func_0200876a(record, x, 0, z);
    GameFlag_Set(0x211);
    Object_SetScript(record, (void *)0x0200db24);

    while (*waitp != 0) {
        Task_Wait(1);
    }

    if (flag == 0) {
        Func_02006ca2(0, first_handle);
        Func_0200880a(first_handle, 2);
    } else {
        Func_02006cb4(0, second_handle);
        Func_0200881c(second_handle, 2);
    }

    shared = Data_02000240;
    Func_0200882c_a(*(s32 *)(shared + 500), 1);
    Message_ShowCentered(MSG_ROBIN_GOT, 3);
    Func_020087ca(record);

    return flag;
}
