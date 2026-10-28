#include "mars_lighthouse_aerie.h"
#include "edition.h"
#include "scene_tables.h"

void Func_02004da0(void) {
  Func_0200560c();
  Func_02005784(0);
#if (LANGUAGE != LANGUAGE_JAPANESE)
  Func_02004d24(2);
#endif
  Actor_SetAngleToward(2, gMainRoutineWork.currentOwner, 0);
  Func_020056dc(MsgIUnderstandYour);
  Func_020056f4(2, 0);
#if (LANGUAGE != LANGUAGE_JAPANESE)
  Func_02005704(2, FACING_WEST);
  Func_02004d50(2);
#else
  MarsLighthouseAerie_ObjTurn(2, FACING_WEST);
#endif
  Func_02005614();
}

void Func_02004df0(void) {
  Func_0200560c();
  Func_02005784(0);
#if (LANGUAGE != LANGUAGE_JAPANESE)
  Func_02004d24(20);
#endif
  Func_020056dc(MsgUhhhhnnnICan);
  Func_020056f4(20, 0);
#if (LANGUAGE != LANGUAGE_JAPANESE)
  Func_02004d50(20);
#endif
  Func_02005614();
}

void Func_02004e20(void) {
  Func_0200560c();
  Func_02005784(0);
#if (LANGUAGE != LANGUAGE_JAPANESE)
  Func_02004d24(22);
#endif
  Func_020056dc(MsgKoffKoffYou);
  Func_020056f4(22, 0);
#if (LANGUAGE != LANGUAGE_JAPANESE)
  Func_02004d50(22);
#endif
  Func_02005614();
}

void Func_02004e50(void) {
  Func_0200560c();
  Func_02005784(0);
#if (LANGUAGE != LANGUAGE_JAPANESE)
  Func_02004d24(21);
#endif
  Func_020056dc(MsgUnnnnIAm);
  Func_020056f4(21, 0);
#if (LANGUAGE != LANGUAGE_JAPANESE)
  Func_02004d50(21);
#endif
  Func_02005614();
}
