.syntax unified
	.thumb
	.include "games/tla/src/gamepak/far_call/veneer.s"
	.global Func_080f8058
	.thumb_func
Func_080f8058:
	overlay_veneer Func_08104ef8
	.global Func_080f8060
	.thumb_func
Func_080f8060:
	overlay_veneer Menu_PlaceOwnerEntryObjects
	.global Menu_ReleaseEntryObjectsFar
	.thumb_func
Menu_ReleaseEntryObjectsFar:
	overlay_veneer Menu_ReleaseEntryObjects
	.global Func_080f8070
	.thumb_func
Func_080f8070:
	overlay_veneer Func_081051a8
	.global Func_080f8078
	.thumb_func
Func_080f8078:
	overlay_veneer MenuIcon_ReleaseObjects
	.global Func_080f8080
	.thumb_func
Func_080f8080:
	overlay_veneer Menu_SetEntryObjectsHidden
	.global Func_080f8088
	.thumb_func
Func_080f8088:
	overlay_veneer MenuIcon_ReplaceObject
