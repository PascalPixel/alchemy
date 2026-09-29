/* Actor 10 in the palace asks whether the party will go to Kolima Forest. */
#include "KYUDEN.H"
extern u8 MsgBiribinoYouWillingGoKolimaForest[];

void Kyuden_AskAboutKolima(void)
{
    Event_Begin();
    Event_SetMessage((s32)MsgBiribinoYouWillingGoKolimaForest);
    Event_AskYesNo(10, 0);
    Event_End();
}
