// Sets the shape of the metatile under the leader `level` steps above its ground.
void fdLeaderSetMetatileShapeAt(Sint32 level, Sint32 layer)
{
    fdActorSetMetatileShape(level, layer, MainRoutineStateWork.leaderActorId);
}
