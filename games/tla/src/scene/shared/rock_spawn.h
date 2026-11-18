/// The falling rocks in flight at once, the ring a scene's rock work keeps.
#define ROCK_COUNT 64

/// Throws `count` rocks from a 16.16 point, as Burst breaks stone: each starts
/// 8 to 16 pixels out at a random angle and flies up at 5 pixels a frame and
/// out along that angle at 2 to 4.
void Rock_Spawn(s32 x, s32 y, s32 z, s32 count) {
  struct RockWork *work;
  s32 i;

  work = Ram_HeapSlots->menuWork;
  for (i = 0; i < count; i++) {
    s32 angle = Random16();
    struct Rock *rock = &work->rocks[work->next];

    rock->timer = 0;
    rock->position[0] = x;
    rock->position[1] = y;
    rock->position[2] = z;
    FieldVector_AddPolarOffsetFX((Random16() << 3) + toFIXED(8), angle,
                                 rock->position);
    rock->velocity[0] = 0;
    rock->velocity[1] = toFIXED(5);
    rock->velocity[2] = 0;
    FieldVector_AddPolarOffsetFX((Random16() << 1) + toFIXED(2), angle,
                                 rock->velocity);
    work->next++;
    work->next &= ROCK_COUNT - 1;
  }
}
