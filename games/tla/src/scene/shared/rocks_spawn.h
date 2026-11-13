/// Throws `count` rocks out from a point.
void Rocks_Spawn(s32 x, s32 y, s32 z, s32 count) {
  struct RockWork *work;
  s32 i;

  /* FAKEMATCH(barrier): an empty asm before the work is read keeps the
   * spills of x and y at the entry. */
  asm("");
  work = Ram_HeapSlots->menuRuntime;
  /* A goto loop carries no loop notes, so the offsets are rebuilt on every
   */
  /* pass, as the bytes have them; a for loop hoists them. */
  if (count > 0) {
    i = count;
  loop:;
    {
      s32 angle = Random16();
      struct Rock *rock = &work->rocks[work->next];

      rock->timer = 0;
      rock->position[0] = x;
      rock->position[1] = y;
      rock->position[2] = z;
      FieldVector_AddPolarOffset((Random16() << 3) + 0x80000, angle,
                                 rock->position);
      rock->velocity[0] = 0;
      rock->velocity[1] = 0x50000;
      rock->velocity[2] = 0;
      FieldVector_AddPolarOffset((Random16() << 1) + 0x20000, angle,
                                 rock->velocity);
      work->next++;
      work->next &= 63;
    }
    i--;
    if (i != 0)
      goto loop;
  }
}
