#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"

extern const u8 gData_02003f44[];
extern const u8 gData_02003f52[];
extern const u8 gData_02003f80[];
extern const u8 gData_02003f92[];
extern const u8 gData_02003f98[];
extern const u8 gData_02003fa0[];
extern const u8 gData_02003fa8[];
extern const u8 gData_02003fb0[];
extern const u8 gData_02003fb8[];
extern const u8 gData_02003fc0[];
extern const u8 gData_02003fc4[];

s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
void wait(s32 frames);
struct Actor *Actor_GetById(s32 id);
void Func_020000a4(const void *data, s32 a, s32 b);
void Func_020002f8(const void *data);
void Func_02000708(const void *data);
void Func_02001474(s32 show);
void Func_02002180(void);
void Map_CopyCellAttributeRect(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_02003c28(void);
void Func_02003c30(void);
void Event_ResolvePendingActions(s32 skip);
void Func_02003dc0(const void *data);
void Func_02003dc8(void);
void Sound_Play(s32 cue);
void Actor_SetHorizontalPositionWithTerrain(s32 id, s32 x, s32 z);
void Func_02003de8(void);
void Map_CopyCellPictureIndicesRect(s32 srcX, s32 srcY, s32 destX, s32 destY, s32 width,
                   s32 height);
void Func_020015b8(struct Actor *object);
void Func_0200172c(void);
void Func_02001804(void);

struct CellRecord {
  u16 present;
  u16 x;
  u16 y;
  u16 unk06[3];
};

extern const u16 gData_02003fd2[];
extern const struct CellRecord gData_02004066[];

void Actor_SetHorizontalPositionAndAngleWithTerrain(s32 id, s32 x, s32 z, s32 angle);
void Func_080d47b4(s32 id, s32 a, s32 b);
void Func_02003cf0(void);
struct Actor *Func_080d4568(void);
void Func_02003d50(s32 a, s32 b);
void Func_02003d58(s32 a, s32 b);
void Func_02003d60(void);
void Func_02003d68(void);
void Func_02003d70(void);
void Actor_ArmCallbackAndRefresh(s32 id, s32 value);
void Func_02003cc0(s32 id);
void Func_080d489c(s32 id, s32 value);
void Func_02003d40(struct Actor *object, s32 mode);
void Func_02003bb8(struct Actor *object);
void Motion_SetVarCbAndRefresh(u32 objectId, s32 variant);
void PartyInventory_GiveItem(s32 item, s32 mode);
void Func_02002b68(void);
void Func_02002f08(void);

struct EffectWork7c4 {
  u8 unk000[0x7c4];
  s16 active;
  s16 timer;
};
void Camera_MoveWithinMapBounds(s32 a, s32 b, s32 c, s32 d);
void Actor_SetAngleToward(s32 id, s32 target, s32 mode);
void Actor_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void Actor_SetPositionAndReset(s32 id, s32 x, s32 z);
void Actor_Jump(s32 id, s32 a, s32 b);
void Actor_ArmCallback(s32 id, s32 a, s32 b);
void Actor_WatchAndSetScript(s32 id, s32 target);
void Event_Wait(s32 frames);
void Task_Add(void (*func)(void), s32 priority);
void Task_RemoveFar(void (*func)(void));
void Field_DispatchTypeHandler(s32 type);

void Func_02002bcc(void) {
  Func_02003d50(134, 1);
  Task_Add(Func_02002b68, 0x480);
  Func_02003d58(9, gMainRoutineWork.currentOwner);
  Func_02003d70();
  Field_DispatchTypeHandler(1);
  Func_02003d60();
  Task_RemoveFar(Func_02002b68);
  Func_02003d68();
}
