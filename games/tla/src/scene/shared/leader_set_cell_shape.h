/* Sets the shape of the cell under the leader `level` steps above its ground.
 */
void fdLeaderSetCellShapeAt(s32 level, s32 layer) {
	fdActorSetCellShape(level, layer, MainRoutineStateWork.leaderActorId);
}
