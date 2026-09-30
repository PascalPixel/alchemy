.syntax unified
	.thumb
	.global Func_0810a2ac
	.thumb_func
Func_0810a2ac:
	push {r5, lr}
	adds r5, r0, #0
	bl Item_Get
	ldrh r3, [r0]
	lsrs r0, r3, #2
	movs r3, #128
	lsls r3, r3, #3
	ands r3, r5
	cmp r3, #0
	bne .L_0810a2c4
	movs r0, #0
.L_0810a2c4:
	pop {r5, pc}
	.2byte 0x0000
