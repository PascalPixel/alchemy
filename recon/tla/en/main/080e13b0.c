

void SceneActor_UpdateObjectByCounterBits(u8 *obj)
{
    if ((gFrameCount & 2) != 0) {
        Object_SetPartPalettes(obj, 7);
    } else {
        Object_SetPartPalettes(obj, 0);
    }
    if ((gFrameCount & 15) == 0) {
        WorldMap_CreateLinkedEffects(obj);
    }
}
