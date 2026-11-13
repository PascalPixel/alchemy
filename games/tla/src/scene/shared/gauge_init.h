/// Sets up the gauge.
void Gauge_Init(void) {
  Decompress_TaggedLz(gWorldMapPackedTilesC, gData_02006f20);
  gData_02006e0e = VramCache_FindFreeEntry();
  Vram_Load(gData_02006e0e, 0x100, 0);
  gData_02006e0c = -1;
  gData_02006e10 = 10;
  Task_AddFrame(Func_020048f4, TASK_PASS_VBLANK - 10);
}
