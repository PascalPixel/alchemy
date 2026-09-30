.syntax unified
	.thumb
	.global Item_AdjustCounter
	.thumb_func
Item_AdjustCounter:
	push {lr}
	movs r3, #128
	lsls r3, r3, #1
	adds r3, #255
	ldr r2, .L_080af39c
	ands r3, r0
	lsls r3, r3, #1
	ldrh r0, [r2, r3]
	movs r4, #0
	cmp r0, #0
	beq .L_080af396
	subs r0, #1
	bl ItemCounter_Adjust
	adds r4, r0, #0
.L_080af396:
	adds r0, r4, #0
	pop {pc}
	.2byte 0x0000
.L_080af39c:
	.4byte Data_080b1f40
