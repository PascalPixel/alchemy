.syntax unified
	.thumb
	.global Func_0810a670
	.thumb_func
Func_0810a670:
	push {r5, r6, lr}
	movs r3, #128
	adds r5, r0, #0
	lsls r3, r3, #3
	adds r6, r5, r3
	adds r0, r6, #0
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0810a6b2
	adds r0, r6, #0
	bl GameFlag_SetBit
	lsls r3, r5, #5
	ldr r2, .L_0810a6b4
	adds r3, r3, r5
	lsls r3, r3, #1
	adds r3, r3, r2
	adds r5, r3, #0
	movs r6, #0
	adds r5, #48
.L_0810a69a:
	ldrh r3, [r5]
	adds r5, #2
	lsls r3, r3, #16
	asrs r0, r3, #16
	cmp r0, #0
	beq .L_0810a6ac
	movs r1, #1
	bl Item_AdjustCounterFar
.L_0810a6ac:
	adds r6, #1
	cmp r6, #7
	ble .L_0810a69a
.L_0810a6b2:
	pop {r5, r6, pc}
.L_0810a6b4:
	.4byte Data_0810c3f4
