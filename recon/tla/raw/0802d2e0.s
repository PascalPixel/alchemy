.syntax unified
	.thumb
	.global Func_0802d2e0
	.thumb_func
Func_0802d2e0:
	push {r5, r6, lr}
	movs r3, #0
	ldrsb r3, [r0, r3]
	adds r0, #1
	lsls r6, r3, #19
	movs r3, #0
	ldrsb r3, [r0, r3]
	adds r1, r1, r2
	lsls r5, r3, #19
	movs r3, #1
	ldrsb r3, [r0, r3]
	adds r0, r5, #0
	lsls r3, r3, #19
	cmp r1, #15
	beq .L_0802d322
	cmp r1, #14
	bhi .L_0802d312
	subs r3, r5, r6
	adds r0, r1, #0
	muls r0, r3
	movs r1, #15
	bl __divsi3
	adds r0, r6, r0
	b .L_0802d322
.L_0802d312:
	subs r1, #15
	subs r3, r3, r5
	adds r0, r1, #0
	muls r0, r3
	movs r1, #15
	bl __divsi3
	adds r0, r5, r0
.L_0802d322:
	pop {r5, r6, pc}
