/* The Lemurian ship's script call as it is placed on the sea: drops its
   sprite's shadow and puts it on the middle layer. */
Sint32 scShipDropShadow(ACTOR *actor)
{
	((FIELD_SPR *)actor->spr)->flg = 0;
	actor->layer = MAPGRID_LAYER_MIDDLE;
	return 1;
}
