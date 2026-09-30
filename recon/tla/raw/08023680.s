.syntax unified
	.thumb
	.global Animation_SetStateFlags
	.thumb_func
Animation_SetStateFlags:
	push {lr}
	cmp r0, #0
	beq .L_080236a4
	adds r3, r0, #0
	adds r3, #84
	ldrb r3, [r3]
	cmp r3, #1
	bne .L_080236a4
	ldr r0, [r0, #80]
	movs r3, #3
	ldrb r2, [r0, #5]
	ands r1, r3
	movs r3, #13
	negs r3, r3
	lsls r1, r1, #2
	ands r3, r2
	orrs r3, r1
	strb r3, [r0, #5]
.L_080236a4:
	pop {pc}
	.2byte 0x0000
