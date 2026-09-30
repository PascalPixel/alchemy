.syntax unified
	.thumb
	.global Func_081c16ec
	.thumb_func
Func_081c16ec:
	push {r4, r5, lr}
	adds r5, r1, #0
	ldr r4, [r5, #32]
	cmp r4, #0
	beq .L_081c1710
.L_081c16f6:
	ldrb r1, [r4]
	movs r0, #199
	tst r0, r1
	beq .L_081c1704
	movs r0, #64
	orrs r1, r0
	strb r1, [r4]
.L_081c1704:
	adds r0, r4, #0
	bl Func_081c16cc
	ldr r4, [r4, #52]
	cmp r4, #0
	bne .L_081c16f6
.L_081c1710:
	movs r0, #0
	strb r0, [r5]
	pop {r4, r5}
	pop {r0}
	bx r0
	.2byte 0x0000
