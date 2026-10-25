// Draft, differ 129 of 187: translated from its listing; names and structure still
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

extern u8 gData_02003d98[];
extern u8 gData_02003dac[];
extern u8 gData_02003dd2[];
extern u8 gData_02003de4[];
extern u8 gData_02003de8[];
void Func_02003aec();
s32 Func_02003bcc();
void Func_02003c1c();
void Func_02003aa4();
void Func_020006cc();
void GameFlag_SetBit();
s32 Func_02003af4();

s32 Func_02000a48(void) {
  s32 r6, v0, r4, r1, r2, r5, r3, r0, ip, v1, v2, v3, v4, v5, v6, v7, v8;
  s32 stack[2];

  r6 = FIELD_AT(((s32)Ram_HeapSlots->eventWork), s32, 3260);
  /* sub (s32)stack, #8 */
  Func_02003aec();
  v0 = Func_02003bcc(0);
  Func_02003c1c();
  r4 = 0;
  r1 = (s32)&gPartyState;
  r2 = *(s16 *)(((s32)(s32)&gPartyState + 480) + r4);
  if ((*(s16 *)(((s32)(s32)&gPartyState + 480) + r4)) != (223))
    goto L_02008aaa;
  stack[1] = 21;
  r5 = 78;
  Call6((void (*)())Func_02003aa4, 98, 21, 4, 3, r5, 21);
  Call6((void (*)())Func_02003aa4, 98, 27, 4, 3, r5, 24);
  Call1((void (*)())Func_020006cc, (s32)gData_02003d98);
  goto L_02008b1e;
L_02008aaa:
  if (r2 != (224))
    goto L_02008b1e;
  r3 = FIELD_AT(r1, s16, 482);
  r2 = 0;
  if ((FIELD_AT(r1, s16, 482)) > 3)
    goto L_02008ac6;
  Call1((void (*)())Func_020006cc, (s32)gData_02003dac);
  goto L_02008b1e;
L_02008ac6:
  if (r3 > 8)
    goto L_02008ad2;
  Call1((void (*)())Func_020006cc, (s32)gData_02003dd2);
  goto L_02008b1e;
L_02008ad2:
  if (r3 > 12)
    goto L_02008af2;
  Call6((void (*)())Func_02003aa4, 79, 37, 2, 1, 79, 36);
  Call1((void (*)())Func_020006cc, (s32)gData_02003de4);
  goto L_02008b1e;
L_02008af2:
  stack[0] = 103;
  r5 = 38;
  Call6((void (*)())Func_02003aa4, 103, 39, 1, 1, 103, r5);
  Call6((void (*)())Func_02003aa4, 99, 42, 5, 1, 99, r5);
  Call1((void (*)())Func_020006cc, (s32)gData_02003de8);
L_02008b1e:
  r4 = 0;
  r0 = *(s16 *)((480 + (s32)&gPartyState) + r4);
  r1 = (FIELD_AT(r6, s32, 16)) >> 20;
  r2 = (FIELD_AT(r6, s32, 8)) >> 20;
  ip = (s32)&gPartyState;
  r3 = 223;
  if ((*(s16 *)((480 + (s32)&gPartyState) + r4)) != (223))
    goto L_02008b62;
  if (r2 != 15)
    goto L_02008b4e;
  if (r1 != 25)
    goto L_02008b4e;
  Call1((void (*)())GameFlag_SetBit, 0x8d6);
  r0 = v1;
  goto L_02008be0;
L_02008b4e:
  if (r2 != 16)
    goto L_02008be0;
  if (r1 != 22)
    goto L_02008be0;
  Call1((void (*)())GameFlag_SetBit, 0x8d5);
  r0 = v2;
  goto L_02008be0;
L_02008b62:
  r3 = 224;
  if (r0 != (224))
    goto L_02008be0;
  r0 = 0;
  r3 = FIELD_AT(ip, s16, 482);
  if ((FIELD_AT(ip, s16, 482)) > 3)
    goto L_02008b8a;
  if (r2 != 27)
    goto L_02008be0;
  if (r1 != 33)
    goto L_02008be0;
  Call1((void (*)())GameFlag_SetBit, 0x8d7);
  r0 = v3;
  goto L_02008be0;
L_02008b8a:
  if (r3 > 8)
    goto L_02008ba2;
  if (r2 != 55)
    goto L_02008be0;
  if (r1 != 35)
    goto L_02008be0;
  Call1((void (*)())GameFlag_SetBit, 0x8d8);
  r0 = v4;
  goto L_02008be0;
L_02008ba2:
  if (r3 > 12)
    goto L_02008bba;
  if (r2 != 16)
    goto L_02008be0;
  if (r1 != 36)
    goto L_02008be0;
  Call1((void (*)())GameFlag_SetBit, 0x8d9);
  r0 = v5;
  goto L_02008be0;
L_02008bba:
  if (r2 != 40)
    goto L_02008bce;
  if (r1 != 37)
    goto L_02008bce;
  Call1((void (*)())GameFlag_SetBit, 0x8da);
  r0 = v6;
  goto L_02008be0;
L_02008bce:
  if (r2 != 35)
    goto L_02008be0;
  if (r1 != 38)
    goto L_02008be0;
  Call1((void (*)())GameFlag_SetBit, 0x8db);
  r0 = v7;
L_02008be0:
  v8 = Func_02003af4(r0);
  /* add (s32)stack, #8 */
  return v8;
}
