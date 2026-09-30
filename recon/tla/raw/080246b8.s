.syntax unified
	.thumb
	.global Func_080246b8
	.thumb_func
Func_080246b8:
	push {r5, r6, r7, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #20]
	movs r7, #63
.L_080246c2:
	ldr r3, [r1]
	cmp r3, #0
	beq .L_08024712
	adds r3, r1, #0
	adds r3, #35
	ldrb r2, [r3]
	movs r3, #64
	ands r3, r2
	ldr r5, [r1, #8]
	ldr r4, [r1, #12]
	ldr r6, [r1, #16]
	cmp r3, #0
	beq .L_0802470c
	ldr r0, [r1, #124]
	cmp r0, #0
	beq .L_0802470c
	ldr r2, [r0, #116]
	cmp r2, #0
	beq .L_08024704
	ldr r3, [r1, #20]
	cmp r4, r3
	bne .L_080246fc
	ldr r3, [r0, #12]
	movs r2, #128
	lsls r2, r2, #13
	adds r3, r3, r2
	str r3, [r1, #20]
	adds r4, r3, #0
	b .L_08024704
.L_080246fc:
	adds r3, r3, r2
	str r3, [r1, #20]
	ldr r3, [r0, #116]
	adds r4, r4, r3
.L_08024704:
	ldr r3, [r0, #112]
	adds r5, r5, r3
	ldr r3, [r0, #120]
	adds r6, r6, r3
.L_0802470c:
	str r5, [r1, #8]
	str r4, [r1, #12]
	str r6, [r1, #16]
.L_08024712:
	subs r7, #1
	adds r1, #128
	cmp r7, #0
	bge .L_080246c2
	pop {r5, r6, r7, pc}
