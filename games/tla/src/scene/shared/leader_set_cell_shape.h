/// Sets the shape of the cell under the leader `level` steps above its ground.
void Leader_SetCellShapeAt(s32 level, s32 layer) {
  Actor_SetCellShape(level, layer, gMainRoutineWork.currentOwner);
}
