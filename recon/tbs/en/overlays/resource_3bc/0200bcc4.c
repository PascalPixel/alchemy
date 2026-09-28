/* NONMATCHING: resource_3bc at 0x0200bcc4..0x0200bcf4 (48 bytes with their
 * pools), the balance-state pair, between FIELD/KOROSSEO_MARUTA/HEX.C and
 * SCENE_EFFECT.C, stay listing.
 *
 * Remaining difference: they read and write the word at 0x02001000, which the
 * main image does not name yet.
 */
#include "SITES.H"

void ColossoLogRollingStage_SetBalanceStateReady(void)
{
    u16 *p = (u16 *)0x02001000;
    u16 v = 9;
    *p = v;
}

void ColossoLogRollingStage_WaitForBalanceState(void)
{
    extern s16 Data_02001000;

    s16 *status = &Data_02001000;

    while (*status != 9) {
        Task_Wait(1);
    }
}
