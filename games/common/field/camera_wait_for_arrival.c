/* fdCameraWaitForArrival and fdCameraGetActor, the same in both games. Both
   games' field/script_commands_camera.c include it where the functions
   stand. */

/* The script command "camera wait for move": waits (at most 600 ticks) for the
   camera to arrive, then 2 ticks more. */
void fdCameraWaitForArrival(void)
{
	fdActorWaitForMoveStub(((FIELD_STATE *)syHeapSlotAllocEWRAM(
								HEAP_SLOT_FIELD, sizeof(FIELD_STATE)))
			->camera);
	fdScriptWait(2);
}

/* The script command "get camera": returns the camera's own actor, which the
   view centres on. */
ACTOR *fdCameraGetActor(void)
{
	return ((FIELD_STATE *)syHeapSlotAllocEWRAM(
				HEAP_SLOT_FIELD, sizeof(FIELD_STATE)))
		->camera;
}
