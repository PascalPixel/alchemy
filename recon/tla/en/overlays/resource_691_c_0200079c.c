// Draft, differ 78 of 110: translated from its listing; names and structure still
// to do.
#include "iwram_call.h"
#include "lib/call.h"
#include "actor.h"
#include "sequence_parent_work.h"
#include "ram_buffer.h"
#include "text/msg_ids.h"
#include "types.h"

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

s32 Actor_GetById();
s32 Map_GetTerrainHeight();
s32 MapCell_GetAttributes();
void Actor_SetSingleChildFlags();
void Func_020005cc();

s32 Func_0200079c(s32 a0, s32 a1, s32 a2, s32 a3) {
  s32 r8, r0, r3, v0, r7, r10, v1, r6, v2, r5, v3, r4;

  r8 = a0;
  r0 = a0;
  r3 = FIELD_AT(a0, u16, 0);
  if ((FIELD_AT(a0, u16, 0)) == 0xffff)
    goto L_0200887c;
L_020087b2:
  v0 = Actor_GetById(FIELD_AT(r8, u16, 0));
  r7 = v0;
  r10 = 34 + r7;
  v1 = Map_GetTerrainHeight(FIELD_AT(r7, u8, 34), FIELD_AT(r7, s32, 8),
                            FIELD_AT(r7, s32, 16));
  r6 = v1;
  v2 = MapCell_GetAttributes(0, (FIELD_AT(r7, s32, 8)) >> 20,
                             (FIELD_AT(r7, s32, 16)) >> 20);
  FIELD_AT(r7, s32, 76) = 255 & v2;
  r5 = 0;
  FIELD_AT(r7, u8, 85) = r5;
  FIELD_AT((r7 + 85), u8, 4) = r5;
  Call2((void (*)())Actor_SetSingleChildFlags, r7, 0);
  r6 = r6 >> 19;
  r6 = r6 + 4;
  r6 = r7;
  r6 = r6 + 99;
  Func_020005cc(FIELD_AT(r10, u8, 0), (FIELD_AT(r7, s32, 8)) >> 20,
                (FIELD_AT(r7, s32, 16)) >> 20, r6);
  FIELD_AT(r6, u8, 0) = r5;
  v3 = MapCell_GetAttributes(0, (FIELD_AT(r7, s32, 8)) >> 20,
                             (FIELD_AT(r7, s32, 16)) >> 20);
  r0 = v3 & 64;
  if ((v3 & 64) != 0) {
    r0 = (s32)Ram_HeapSlots->mapWork;
    r3 = 0x3000000;
    if (((s32)Ram_HeapSlots->mapWork) == 0)
      goto L_0200887c;
    r4 = FIELD_AT(r0, s32, 424);
    r4 = r4 +
         ((((FIELD_AT(r7, s32, 8)) >> 20) +
           (((((FIELD_AT(r7, s32, 16)) - (FIELD_AT(r7, s32, 12))) >> 20) + 1)
            << 7))
          << 2);
    FIELD_AT(r4, u8, 2) = 255;
    r4 = FIELD_AT(r0, s32, 424);
    r4 = r4 +
         ((((FIELD_AT(r7, s32, 8)) >> 20) +
           (((((FIELD_AT(r7, s32, 16)) - (FIELD_AT(r7, s32, 12))) >> 20) - 1)
            << 7))
          << 2);
    FIELD_AT(r4, u8, 2) = -1;
    FIELD_AT(r6, u8, 0) = 1;
    r0 = r0 + 424;
  }
  r8 = r8 + 2;
  r3 = FIELD_AT(r8, u16, 0);
  if ((FIELD_AT(r8, u16, 0)) != 0xffff)
    goto L_020087b2;
L_0200887c:
  return r0;
}
