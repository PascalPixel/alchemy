.syntax unified
	.thumb
	.global Func_080d5bec
	.thumb_func
Func_080d5bec:
	push {r5, r6, lr}
	adds r5, r1, #0
	bl ObjectTable_Get
	adds r4, r0, #0
	cmp r4, #0
	bne .L_080d5c00
	movs r0, #1
	negs r0, r0
	b .L_080d5c6a
.L_080d5c00:
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	ldr r1, .L_080d5c6c
	adds r3, #228
	ldr r0, [r3]
	ldr r2, [r3, #4]
	ldr r3, [r4, #16]
	ands r2, r1
	ands r0, r1
	ldr r1, [r4, #8]
	subs r3, r3, r2
	ldr r2, [r4, #12]
	subs r1, r1, r0
	subs r6, r3, r2
	adds r2, r5, #0
	adds r5, #4
	cmp r1, #0
	bge .L_080d5c2e
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	adds r1, r1, r3
.L_080d5c2e:
	asrs r3, r1, #16
	str r3, [r2]
	adds r3, r6, #0
	cmp r3, #0
	bge .L_080d5c40
	movs r2, #255
	lsls r2, r2, #8
	adds r2, #255
	adds r3, r3, r2
.L_080d5c40:
	asrs r3, r3, #16
	str r3, [r5]
	adds r3, r4, #0
	adds r3, #84
	ldrb r2, [r3]
	movs r3, #15
	ands r3, r2
	cmp r3, #1
	bne .L_080d5c68
	ldr r3, [r4, #80]
	ldr r3, [r3, #40]
	movs r2, #0
	ldrsh r0, [r3, r2]
	bl Func_08020000
	ldr r3, [r5]
	movs r2, #8
	ldrsb r2, [r0, r2]
	subs r3, r3, r2
	str r3, [r5]
.L_080d5c68:
	movs r0, #0
.L_080d5c6a:
	pop {r5, r6, pc}
.L_080d5c6c:
	.4byte 0xffff0000
