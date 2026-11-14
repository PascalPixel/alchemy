/// Takes the gauge down.
void Gauge_End(void) {
  Decompress_TaggedLz(gWorldMapPackedTilesC, gData_02006f20);
  VramCache_FreeEntry(gData_02006e0e);
  gData_02006e0c = -1;
  FieldTask_Remove(Func_020048f4);
}
