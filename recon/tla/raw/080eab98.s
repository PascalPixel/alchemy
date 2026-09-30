.syntax unified
	.thumb
	.global Func_080eab98
	.thumb_func
Func_080eab98:
	push {r5, lr}
	ldr r4, .L_080eabcc
	lsls r2, r2, #24
	lsrs r5, r2, #24
	movs r0, #0
	asrs r2, r2, #24
.L_080eaba4:
	ldrb r3, [r4]
	cmp r3, r1
	bne .L_080eabb8
	adds r4, #1
	movs r3, #0
	ldrsb r3, [r4, r3]
	subs r4, #1
	cmp r3, r2
	bne .L_080eabc2
	b .L_080eabc8
.L_080eabb8:
	cmp r3, #255
	bne .L_080eabc2
	strb r1, [r4]
	strb r5, [r4, #1]
	b .L_080eabc8
.L_080eabc2:
	adds r4, #4
	adds r0, #1
	b .L_080eaba4
.L_080eabc8:
	pop {r5, pc}
	.2byte 0x0000
.L_080eabcc:
	.4byte gMapCollision
