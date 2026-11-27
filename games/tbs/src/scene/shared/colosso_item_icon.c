/* @file */
/* The three Colosseum finals matches, and three of Crossbone Isle Cave's side
 */
/* rooms (CrossboneIsleKeys, CrossboneIsleStatues and CrossboneIsleBane): an */
/* actor drawn as an item's menu icon, such as the two prizes at a */
/* finals match's arena or the keys in CrossboneIsleKeys. */

/* Draws actor id, when it has a sprite of its own, as the menu icon of item, */
/* held still in place of its animation. */
void scColossoShowItemIcon(s32 id, s32 item) {
	ACTOR *actor;
	SPR *sprite;
	ICON_TRANSFER *icon;
	s32 tile;

	actor = fdActorGetById(id);
	if(actor != NULL && actor->sprStorageKind == ACTOR_SPRITE_ONE) {
		sprite = actor->spr;
		icon = syHeapSlotAlloc(HEAP_SLOT_ICON_TRANSFER, sizeof(ICON_TRANSFER));
		btDmaFill32(0, icon->tiles, ICON_TILE_BYTES);
		mnIconDrawItem(item);
		tile = syVramCacheLoad(sprite->slot, ICON_TILE_BYTES, icon->tiles);
		syHeapSlotRelease(HEAP_SLOT_ICON_TRANSFER);
		actor->retainSprCache = 1;
		fdSprAnimationClear(sprite->entries[0]);
		sprite->entries[0] = NULL;
		sprite->cnt = 0;
		sprite->image.fullCol = 0;
		sprite->image.tile = tile;
		sprite->dirty = 0;
		sprite->flg = 0;
	}
}
