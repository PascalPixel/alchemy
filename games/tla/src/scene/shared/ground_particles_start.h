/// Starts the rain about a doused stone: its tiles decoded into VRAM and every
/// drop at the map's origin with a staggered timer, drawn semi-transparent
/// (GroundParticles_Draw); the overlay's copy of
/// FieldEffect_InitializeSparkles.
void GroundParticles_Start(void) {
  struct GroundParticleWork *work =
      HeapSlot_AllocEwram(HEAP_SLOT_WEATHER, sizeof(struct GroundParticleWork));
  struct GroundParticle *p = work->particles;
  u8 *buf;
  u32 i;
  u32 size;
  volatile u32 zero;

  size = 0;
  zero = size;
  Dma_Set((const void *)&zero, work,
          DMA_FILL_WORDS(sizeof(struct GroundParticleWork) / 4), REG_DMA3);
  buf = HeapSlot_AllocEwram(HEAP_SLOT_TILE_SCRATCH, 1024);
  Decompress_ByteLz(gField_GroundTiles, buf);
  work->entry = VramCache_FindFreeEntry();
  work->tileBase = Vram_Load(work->entry, 768, buf);
  HeapSlot_Release(HEAP_SLOT_TILE_SCRATCH);
  work->leader = (s32 *)size;
  i = 0;
  do {
    size = (u32)p;
    *(u32 *)size = 0;
    size += 4;
    *(u32 *)size = OBJ_SIZE_16X16 | OBJ_MODE(OBJ_MODE_SEMI_TRANSPARENT);
    size += 4;
    *(u32 *)size = OBJ_PRIORITY(1) | OBJ_PALETTE(11);
    p->posX = 0;
    p->posZ = 0;
    p->posY = toFIXED(Map_GetTerrainHeight(0, 0, 0));
    p->timer = (i & (GROUND_PARTICLE_FRAMES - 1)) + 1;
    i++;
    p++;
  } while (i < ARRAY_COUNT(work->particles));
  Task_AddFrame(GroundParticles_Draw, TASK_PASS_FRAME);
}
