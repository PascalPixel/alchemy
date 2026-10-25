// Draft, differ 269 of 286: translated from its listing; names and structure still
// to do.
#include "iwram_call.h"
#include "lib/call.h"
#include "object_runtime.h"
#include "party_state.h"
#include "ram_buffer.h"
#include "text/msg_ids.h"
#include "types.h"

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

extern u8 gInput[];
extern u8 gData_0200173c[];
extern u8 gData_02001700[];
void Func_02001380();
void Func_020013f8();
s32 PartyInventory_CountItem();
void Func_020013c8();
void UiText_OpenMessageAtObject();
s32 UiWindow_Create();
void UiText_DrawCharacterAtOffset();
void UiText_DrawNumberInWindow();
s32 Menu_SelectCoinsOrMedals();
void UiWork_Finalize();
void UiWork_FinalizePendingCore();
s32 Func_020013d8();
void WaitFrames();
void Func_02001408();
s32 PartyInventory_CountFreeSlots();
s32 Event_AskYesNoWithActorAnimation();
s32 Party_AdjustCoins();
s32 PartyInventory_Remove();
s32 Func_02001184();
void Func_02001328();
void FieldEvent_WaitFrames();
s32 GameFlag_Test();
void GameFlag_SetBit();
s32 Func_0200049c();
s32 Random16Far();
void Func_02001388();

s32 Func_020001e4(s32 a0, s32 a1) {
  s32 r7, r9, v0, r10, r8, v1, r5, r6, v2, v3, r2, r0, r3, r1, v4, v5, v6, v7,
      v8, v9, v10, v11, v12, v13;
  s32 stack[16];

  /* sub (s32)stack, #4 */
  Func_02001380();
  r7 = 0;
  Call1((void (*)())Func_020013f8, 0);
L_020081fc:
  r9 = FIELD_AT((s32)&gPartyState, s32, 16);
  v0 = PartyInventory_CountItem(229);
  r10 = v0;
  r8 = (s32)MsgWhatWillYou;
  Func_020013c8(r8);
  Value2((s32 (*)())UiText_OpenMessageAtObject, -1, 0);
  v1 = UiWindow_Create(0, 0, 17, 4, 2);
  r5 = (s32)MsgCoins2;
  r6 = v1;
  Call4((void (*)())UiText_DrawCharacterAtOffset, r5, r6, 0, 0);
  UiText_DrawNumberInWindow(r9, 6, r6, 72, 0);
  Call4((void (*)())UiText_DrawCharacterAtOffset, r5 + 1, r6, 0, 8);
  UiText_DrawNumberInWindow(r10, 6, r6, 72, 8);
  v2 = Menu_SelectCoinsOrMedals(r7);
  r7 = v2;
  Call2((void (*)())UiWork_Finalize, r6, 2);
  UiWork_FinalizePendingCore();
  r2 = -1;
  r0 = v3;
  if (r7 != -1)
    goto L_02008288;
  goto L_02008468;
L_02008288:
  if (r7 == 0) {
    r3 = r9;
    if (r9 != 0)
      goto L_020082ee;
    r0 = r8 + 1;
  } else {
    if (r7 != 1)
      goto L_020082ee;
    r1 = r10;
    if (r10 != 0)
      goto L_020082c6;
    r0 = r8 + 2;
  }
  Func_020013c8(r0);
  Call2((void (*)())Func_020013d8, -1, 0);
  Call1((void (*)())WaitFrames, 1);
  goto L_020081fc;
L_020082bc:
  Value1((s32 (*)())Func_02001408, 112);
  r5 = 0;
  goto L_0200834e;
L_020082c6:
  v4 = PartyInventory_CountFreeSlots();
  if (v4 != 0)
    goto L_020082ee;
  Func_020013c8(r8 + 4);
  Value2((s32 (*)())UiText_OpenMessageAtObject, -1, 0);
  v5 = Event_AskYesNoWithActorAnimation(0, 0);
  r0 = v5;
  if (v5 == 0)
    goto L_020082ee;
  goto L_02008468;
L_020082ee:
  v6 = UiWindow_Create(20, 15, 9, 4, 2);
  r5 = (s32)MsgToss2;
  r6 = v6;
  Call4((void (*)())UiText_DrawCharacterAtOffset, r5, r6, 0, 0);
  Call4((void (*)())UiText_DrawCharacterAtOffset, r5 + 1, r6, 0, 8);
  Call1((void (*)())WaitFrames, 5);
  Value1((s32 (*)())Func_02001408, 116);
  goto L_0200832e;
L_02008328:
  Call1((void (*)())WaitFrames, 1);
L_0200832e:
  r1 = (s32)gInput;
  r2 = 1;
  r3 = (FIELD_AT((s32)gInput, s32, 4)) & 1;
  if (((FIELD_AT((s32)gInput, s32, 4)) & 1) != 0)
    goto L_020082bc;
  r2 = 2;
  r3 = (FIELD_AT(r1, s32, 4)) & 2;
  if (((FIELD_AT(r1, s32, 4)) & 2) == 0)
    goto L_02008328;
  Value1((s32 (*)())Func_02001408, 113);
  r5 = 1;
  r5 = -r5;
L_0200834e:
  Call2((void (*)())UiWork_Finalize, r6, 2);
  r2 = -1;
  r0 = v7;
  if (r5 != -1)
    goto L_02008360;
  goto L_02008468;
L_02008360:
  if (r7 != 0)
    goto L_0200836e;
  Call1((void (*)())Party_AdjustCoins, -1);
  goto L_02008378;
L_0200836e:
  if (r7 != 1)
    goto L_02008378;
  Call1((void (*)())PartyInventory_Remove, 229);
L_02008378:
  v8 = Func_02001184(r7);
  r5 = v8;
  if (r7 != 0)
    goto L_020083c0;
  if (r5 == 4)
    goto L_020083b2;
  r6 = (s32)gData_0200173c;
  r5 = r5 << 1;
  Call1((void (*)())Party_AdjustCoins, *(u16 *)(r6 + r5));
  Value1((s32 (*)())Func_02001408, 91);
  Call2((void (*)())Func_02001328, *(u16 *)(r6 + r5), 5);
  Func_020013c8((s32)MsgYouWonCoin);
  v9 = Func_020013d8(-1, 0);
  r0 = v9;
  goto L_02008468;
L_020083b2:
  Value1((s32 (*)())Func_02001408, 113);
  Call1((void (*)())FieldEvent_WaitFrames, 10);
  r0 = v10;
  goto L_02008468;
L_020083c0:
  r7 = 0;
  if (r5 != 0)
    goto L_020083e8;
  v11 = GameFlag_Test(0x90b);
  if (v11 != 0)
    goto L_020083e8;
  Call1((void (*)())GameFlag_SetBit, 0x90b);
  v12 = Func_0200049c(-1);
  r0 = v12;
  goto L_02008468;
L_020083e8:
  r6 = 0;
  r0 = (r5 << 1) + r5;
  if (r6 >= (((r5 << 1) + r5) + 3))
    goto L_0200840a;
  r1 = (s32)&gPartyState;
L_020083f6:
  r6 = r6 + 1;
  r7 = r7 + (*(s8 *)((r1 + 1) + (r6 + 316)));
  if (r6 < (r0 + 3))
    goto L_020083f6;
L_0200840a:
  v13 = Random16Far();
  r6 = 0;
  r1 = ((u32)(r7 * v13) >> 16) - (*(s8 *)(((s32)(s32)&gPartyState + 1) + 316));
  r0 = (s32)&gPartyState;
  if ((((u32)(r7 * v13) >> 16) -
       (*(s8 *)(((s32)(s32)&gPartyState + 1) + 316))) < 0)
    goto L_0200843c;
L_02008426:
  r6 = r6 + 1;
  if (r6 > 14)
    goto L_0200843c;
  r1 = r1 - (*(s8 *)((r0 + 1) + (r6 + 316)));
  if ((r1 - (*(s8 *)((r0 + 1) + (r6 + 316)))) >= 0)
    goto L_02008426;
L_0200843c:
  if (r6 == 15) {
    r6 = 14;
  }
  Func_0200049c(*(s32 *)(((s32)gData_02001700) + (r6 << 2)));
  r3 = (s32)&gPartyState;
  r0 = r3 + 1;
  r1 = ((*(u8 *)(((s32)(s32)&gPartyState + 1) + (r6 + 316))) << 24) >> 24;
  r2 = r6 + 316;
  r3 = (*(u8 *)(((s32)(s32)&gPartyState + 1) + (r6 + 316))) << 24;
  if ((((*(u8 *)(((s32)(s32)&gPartyState + 1) + (r6 + 316))) << 24) >> 24) <= 1)
    goto L_02008468;
  *(u8 *)(r0 + r2) = (r1 + ((u32)r3 >> 31)) >> 1;
  r3 = (r1 + ((u32)r3 >> 31)) >> 1;
L_02008468:
  Func_02001388();
  /* add (s32)stack, #4 */
  return 0;
}
