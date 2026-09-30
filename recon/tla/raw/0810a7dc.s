.syntax unified
	.thumb
	.global Func_0810a7dc
	.thumb_func
Func_0810a7dc:
	push {r5, r6, lr}
	bl Owner_GetState
	adds r5, r0, #0
	movs r6, #0
	adds r5, #216
.L_0810a7e8:
	ldrh r0, [r5]
	adds r5, #2
	bl Func_0810a748
	cmp r0, #0
	beq .L_0810a7f8
	movs r0, #1
	b .L_0810a800
.L_0810a7f8:
	adds r6, #1
	cmp r6, #14
	ble .L_0810a7e8
	movs r0, #0
.L_0810a800:
	pop {r5, r6, pc}
	.2byte 0x0000
