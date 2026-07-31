typedef signed int s32;

void Func_02002740();           /* scripted-scene bracket: open */
s32 Func_02002726();            /* story-flag test; used in a condition */
void Func_02002818();           /* show a dialogue line by id */
s32 Func_02002738();
void Func_0200282a();
void Func_02002832();
s32 Func_02002750();
void Func_02002842();
void Func_0200285a();           /* wait for the slot's action to finish */
void Func_0200278e();           /* scripted-scene bracket: close */

void Func_020002cc(void)
{
    Func_02002740();

    if (Func_02002726(0x202) != 0) {
        Func_02002818(0x174c);
    } else if (Func_02002738(0x845) == 0) {
        Func_0200282a(0x1436);
    } else {
        Func_02002832(0x1434);
        if (Func_02002750(0x84e) != 0) {
            Func_02002842(0x176f);
        }
    }

    Func_0200285a(17, 0);
    Func_0200278e();
}
