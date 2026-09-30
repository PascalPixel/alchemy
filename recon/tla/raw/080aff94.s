.syntax unified
	.thumb
	.global Func_080aff94
	.thumb_func
Func_080aff94:
	push {r5, lr}
	adds r5, r0, #0
	movs r0, #0
	bl Trade_GetOfferState
	ldr r3, [r0]
	movs r2, #1
	lsls r2, r5
	orrs r3, r2
	str r3, [r0]
	pop {r5, pc}
	.2byte 0x0000
