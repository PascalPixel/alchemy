/* Vault's inn, before and after the thieves are caught: an item lying in the
   room drawn as its menu icon (the Bone on the barrel, at (568, 16, 672)). */

/* Places the item's icon, standing still, at the 16.16 position. */
void scVaultPlaceItemIcon(Sint32 item, FIXED x, FIXED y, FIXED z)
{
	ACTOR		  *actor;
	SPR			  *spr;
	ICON_TRANSFER *icon;
	Uint8		   clear = 0;

	actor = fdActorCreate(SPRITE_BLANK, x, y, z);
	if(actor != NULL) {
		spr = actor->spr;
		spr->flg = clear;
		spr->cnt = clear;
		spr->image.fullCol = 0;
		spr->image.pal = 0;
		actor->motionFlg = clear;
		actor->retainSprCache = 1;
		icon = syHeapSlotAlloc(HEAP_SLOT_ICON_TRANSFER, sizeof(ICON_TRANSFER));
		mnIconDrawItem(item);
		syVRAMCacheLoad(spr->slot, ICON_TILE_BYTES, icon->tiles);
		syHeapSlotRelease(HEAP_SLOT_ICON_TRANSFER);
	}
}
