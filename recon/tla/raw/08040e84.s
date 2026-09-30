.syntax unified
	.thumb
	.global Func_08040e84
	.thumb_func
Func_08040e84:
	push {lr}
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #171
	bl GameFlag_SetBit
	movs r0, #245
	lsls r0, r0, #3
	adds r0, #255
	bl GameFlag_ClearBit
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #170
	bl GameFlag_SetBit
	movs r1, #202
	adds r1, #255
	movs r0, #4
	bl Inventory_AddItemFar
	ldr r3, .L_08040ec8
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #66
	adds r3, r3, r2
	movs r2, #141
	strh r2, [r3]
	ldr r0, .L_08040ecc
	movs r1, #1
	bl Func_080c8268
	pop {pc}
	.2byte 0x0000
.L_08040ec8:
	.4byte gPartyState
.L_08040ecc:
	.4byte 0x0000005f
	.4byte 0x00004770
