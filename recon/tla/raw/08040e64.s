.syntax unified
	.thumb
	.global Func_08040e64
	.thumb_func
Func_08040e64:
	push {lr}
	movs r1, #243
	movs r0, #4
	bl Inventory_AddItemFar
	movs r1, #244
	movs r0, #4
	bl Inventory_AddItemFar
	ldr r0, .L_08040e80
	movs r1, #4
	bl Event_SetPairWork1c0Far
	pop {pc}
.L_08040e80:
	.4byte 0x00000101
