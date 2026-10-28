.syntax unified
	.thumb
	.include "games/tla/src/gamepak/far_call/veneer.s"
	.global WindowSprite_GetPointerTilesFar
	.thumb_func
WindowSprite_GetPointerTilesFar:
	overlay_veneer WindowSprite_GetPointerTiles
	.global Func_080383d8
	.thumb_func
Func_080383d8:
	overlay_veneer Message_OpenDecoded
	.global Func_080383e0
	.thumb_func
Func_080383e0:
	overlay_veneer Save_CountFilesExcludingSent
