// Draft, differ 6 of 214: translated from its listing; names and structure
// still to do.
#include "types.h"
#include "lib/call.h"
#include "object_runtime.h"
#include "party_state.h"
#include "ram_buffer.h"
#include "text/msg_ids.h"

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

void *Object_GetById(s32 id);
void GameFlag_SetBit(s32 flag);
s32 Func_02000b30();
s32 Func_02002e44();
s32 Func_02002edc();
void FieldEvent_WaitFrames(s32 frames);
s32 Func_02002eec();
s32 Func_02002df4();
void Camera_MoveWithinMapBounds(s32, s32, s32, s32);
s32 Func_02002e24();
s32 Func_02000b78();
s32 Func_02002e4c();

s32 Func_02000e60(s32 a0, s32 a1) {
  s32 r5, v0, r7, r6, r0, r3, v1, r10, r8, v2, v3;
  s32 stack[2];

  r5 = a1;
  /* sub sp, #8 */
  v0 = Object_GetById(r5);
  r7 = (FIELD_AT(v0, s32, 8)) >> 20;
  r6 = (FIELD_AT(v0, s32, 16)) >> 20;
  r0 = v0;
  r3 = FIELD_AT(v0, s32, 16);
  if (r5 == 8)
    goto L_02008e80;
  goto L_02009040;
L_02008e80:
  if (r7 == 12)
    goto L_02008e86;
  goto L_02009040;
L_02008e86:
  if (r6 == 5)
    goto L_02008e8c;
  goto L_02009040;
L_02008e8c:
  Call1((void (*)())GameFlag_SetBit, 0x9dc);
  v1 = Func_02000b30(8, 181);
  Func_02002e44();
  Call1((void (*)())Func_02002edc, 0);
  Call1((void (*)())FieldEvent_WaitFrames, 15);
  Call1((void (*)())Func_02002eec, 202);
  Call6((void (*)())Func_02002df4, 29, 70, 3, 1, r7, 71);
  Call4((void (*)())Camera_MoveWithinMapBounds, 0xd80000, -1, 0x780000, 1);
  Call3((void (*)())Func_02002e24, 0x20000, 0x20000, 0x10000);
  Call1((void (*)())FieldEvent_WaitFrames, 60);
  Call1((void (*)())Func_02002eec, 0x23f);
  stack[1] = 7;
  r5 = 10;
  r10 = 7;
  Call6((void (*)())Func_02002df4, 35, 6, 7, 2, r5, 7);
  Call6((void (*)())Func_02002df4, 35, 70, 7, 2, r5, 71);
  r8 = -40;
  Call6((void (*)())Func_02000b78, 6, 10, 200, r8, 124, 3);
  Call6((void (*)())Func_02002df4, 35, 6, 7, 3, r5, r10);
  Call6((void (*)())Func_02002df4, 35, 70, 7, 3, r5, 71);
  Call6((void (*)())Func_02000b78, 3, 10, 184, r8, 140, r6);
  Call6((void (*)())Func_02002df4, 35, 6, 7, 4, r5, r10);
  Call6((void (*)())Func_02002df4, 35, 70, 7, 4, r5, 71);
  Call6((void (*)())Func_02000b78, 3, 10, 184, r8, 156, r6);
  Call6((void (*)())Func_02002df4, 35, 6, 7, 5, r5, r10);
  Call6((void (*)())Func_02002df4, 35, 70, 7, 5, r5, 71);
  Call6((void (*)())Func_02000b78, 3, 10, 184, r8, 172, r6);
  Call3((void (*)())Func_02002e24, -1, -1, 0xe666);
  Call6((void (*)())Func_02002df4, 35, 6, 7, 6, r5, r10);
  Call6((void (*)())Func_02002df4, 35, 70, 7, 6, r5, 71);
  Call1((void (*)())FieldEvent_WaitFrames, 15);
  Call6((void (*)())Func_02002df4, 35, 6, 7, 16, r5, r10);
  v2 = Func_02002df4(35, 70, 7, 16, r5, 71);
  v3 = Func_02002e4c();
  r0 = v3;
L_02009040:
  /* add sp, #8 */
  return r0;
}
