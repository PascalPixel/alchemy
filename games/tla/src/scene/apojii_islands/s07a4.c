#include "types.h"
#include "actor.h"
#include "io_write_queue.h"
#include "system/io_reg.h"
#include "task.h"

struct Actor *Actor_GetById(u32 id);
void Func_02001cc4(void);
void Func_02001dcc(s32 skip);
void Func_02000780(void);
void Func_02001e2c(s32 cue);
void WaitFrames(s32 frames);
void Func_02001ad8(void);
void Func_02001ba0(s32 a);
void Func_02001dec(s32 a);
u16 Random16Far(void);
s32 Func_02001da4(s32 a, s32 b);
void Func_02001dac(s32 frames);
void Func_02001de4(void);
void Task_RemoveFar(void (*func)(void));
s32 Task_Add(void (*func)(void), s32 priority);
void Func_02000540(void);
void Func_020006bc(void);
void Func_020017bc(void);
void Func_020017f8(u32 base, s32 a, s32 b);
void GameFlag_SetBit(s32 flag);
void Func_02001d7c(s32 a, s32 b);
void Camera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 mode);
s32 Func_02001d6c(s32 a, s32 b, s32 c);
void Func_02001d8c(void);
void Func_02001d94(s32 cue);

static inline void ApojiiIslands_Flash(s32 strength, s32 mode) {
  Func_02001da4(strength, mode);
}

static inline void ApojiiIslands_CamMove(s32 x, s32 y, s32 z, s32 mode) {
  Camera_MoveWithinMapBounds(x, y, z, mode);
}

/// An Apojii Islands scene inside an event: the actor takes Func_02000780 as
/// its update, cues 78 and 21 play, a sparkle setup (Func_02001ad8) runs and
/// four flashes of random strength follow; then the screen blends in over six
/// steps while the palette cycle gives way to Func_020006bc and the cell
/// markers are rebuilt (Func_020017f8). Flags 0x95f and 0x95e are set and the
/// camera pans through three points before cue 77.
void Func_020007a4(s32 unused, s32 id) {
  struct Actor *actor = Actor_GetById(id);
  s32 i;

  Func_02001cc4();
  Func_02001dcc(0);
  actor->update = Func_02000780;
  Func_02001e2c(78);
  WaitFrames(30);
  Func_02001e2c(21);
  Func_02001ad8();
  Func_02001ba0(17);
  Func_02001dec(170);
  i = 3;
  /* FAKEMATCH(wrapper): both flashes forwarded through ApojiiIslands_Flash,
   * which keeps each call's own constants; called directly, 10 of 188
   * instructions differ in tla-en. */
  /* FAKEMATCH(signature): Func_02001da4 declared returning s32, though its
   * target returns nothing; as void, 2 of 188 instructions differ in tla-en. */
flash:
  ApojiiIslands_Flash(Random16Far() & 0x7fff, 0);
  Func_02001dac(15);
  WaitFrames(10);
  ApojiiIslands_Flash(0x10000, 0);
  Func_02001dac(10);
  i--;
  WaitFrames(10);
  if (i >= 0)
    goto flash;
  Func_02001de4();
  IO_WRITE_QUEUE_PUSH(0x04000050, 0xf00, 0x20000);
  IO_WRITE_QUEUE_PUSH(0x04000052, 0x1000, 0x20000);
  WaitFrames(1);
  Task_RemoveFar(Func_02000540);
  Task_Add(Func_020006bc, TASK_PASS_VBLANK);
  Func_020017bc();
  Func_020017f8(33, 0, 0);
  i = 1;
blend:
  IO_WRITE_QUEUE_PUSH(0x04000052, 0x1000 | i, 0x20000);
  WaitFrames(4);
  i++;
  if (i <= 6)
    goto blend;
  GameFlag_SetBit(0x95f);
  GameFlag_SetBit(0x95e);
  Func_02001d7c(0x10000, 0x2000);
  /* FAKEMATCH(wrapper): the camera moves forwarded through
   * ApojiiIslands_CamMove, so each builds its own -1; called directly, 25 of
   * 188 instructions differ in tla-en. */
  ApojiiIslands_CamMove(0x1680000, -1, 0x1480000, 1);
  Func_02001d6c(16, 256, 20);
  Func_02001d8c();
  ApojiiIslands_CamMove(0x1b80000, -1, 0x1780000, 1);
  Func_02001d8c();
  ApojiiIslands_CamMove(0x1d80000, -1, 0x1980000, 1);
  Func_02001d94(77);
}
