.syntax unified
.text
	.arm
	.global Func_080158e8
Func_080158e8:
	push	{r5, r6, r7, r8, r9, sl, lr}
	mov	sl, r1
	mov	r1, #0
.L_080158f4:
	mov	r7, #0
	mov	lr, r7
	mov	r8, r7
	mov	r2, r7
	mov	ip, r2
	mov	r4, r2
	mov	r5, r2
	mov	r6, r2
	add	r0, r0, #2
	add	r9, r1, #1
	mov	r1, r2
.L_08015920:
	ldrh	r3, [sl], #2
	cmp	r3, #0
	beq .L_08015a8c
	cmp	r5, #0
	bne .L_080159f8
	sub	r3, r3, #61440
	sub	r3, r3, #7
	cmp	r3, #19
	ldrls	pc, [pc, r3, lsl #2]
	b .L_080159f4
	stmdaeq	r1, {r3, r4, r6, r7, r8, fp, ip, lr}
	stmdaeq	r1, {r2, r3, r7, r9, fp, ip, lr}
	stmdaeq	r1, {r2, r3, r7, r9, fp, ip, lr}
	stmdaeq	r1, {r2, r3, r7, r9, fp, ip, lr}
	stmdaeq	r1, {r2, r3, r7, r9, fp, ip, lr}
	stmdaeq	r1, {r2, r3, r7, r9, fp, ip, lr}
	stmdaeq	r1, {r2, r3, r7, r9, fp, ip, lr}
	stmdaeq	r1, {r2, r3, r7, r9, fp, ip, lr}
	stmdaeq	r1, {r2, r3, r7, r9, fp, ip, lr}
	stmdaeq	r1, {r3, r4, r7, r8, fp, ip, lr}
	stmdaeq	r1, {r2, r3, r7, r9, fp, ip, lr}
	stmdaeq	r1, {r2, r3, r7, r9, fp, ip, lr}
	stmdaeq	r1, {r3, r4, r5, r7, r8, fp, ip, lr}
	stmdaeq	r1, {r2, r3, r7, r9, fp, ip, lr}
	stmdaeq	r1, {r2, r3, r7, r9, fp, ip, lr}
	stmdaeq	r1, {r3, r4, r6, r7, r8, fp, ip, lr}
	stmdaeq	r1, {r2, r3, r7, r9, fp, ip, lr}
	stmdaeq	r1, {r2, r3, r7, r9, fp, ip, lr}
	stmdaeq	r1, {r2, r3, r7, r9, fp, ip, lr}
	stmdaeq	r1, {r3, r4, r6, r7, r8, fp, ip, lr}
	lsl	lr, r6, #3
	add	ip, lr, #8
	add	r8, lr, #1
	mov	r7, lr
	add	r4, r7, #7
	mov	r2, ip
	add	r5, r5, #1
	b .L_08015a8c
	lsl	lr, r6, #3
	add	ip, lr, #8
	mov	r8, lr
	add	r7, lr, #1
	mov	r4, ip
	add	r2, lr, #7
	add	r5, r5, #1
	b .L_08015a8c
	lsl	lr, r6, #3
	add	ip, lr, #8
	mov	r7, lr
	mov	r8, r7
	mov	r2, ip
	mov	r4, r2
	add	r5, r5, #1
.L_080159f4:
	b .L_08015a8c
.L_080159f8:
	sub	r3, r3, #61440
	sub	r3, r3, #7
	cmp	r3, #20
	ldrls	pc, [pc, r3, lsl #2]
	b .L_08015a8c
	stmdaeq	r1, {r7, r9, fp, ip, lr}
	stmdaeq	r1, {r2, r3, r7, r9, fp, ip, lr}
	stmdaeq	r1, {r2, r3, r7, r9, fp, ip, lr}
	stmdaeq	r1, {r2, r3, r7, r9, fp, ip, lr}
	stmdaeq	r1, {r2, r3, r7, r9, fp, ip, lr}
	stmdaeq	r1, {r2, r3, r7, r9, fp, ip, lr}
	stmdaeq	r1, {r2, r3, r7, r9, fp, ip, lr}
	stmdaeq	r1, {r2, r3, r7, r9, fp, ip, lr}
	stmdaeq	r1, {r2, r3, r7, r9, fp, ip, lr}
	stmdaeq	r1, {r2, r3, r7, r9, fp, ip, lr}
	stmdaeq	r1, {r2, r3, r7, r9, fp, ip, lr}
	stmdaeq	r1, {r5, r6, r9, fp, ip, lr}
	stmdaeq	r1, {r2, r3, r7, r9, fp, ip, lr}
	stmdaeq	r1, {r2, r3, r7, r9, fp, ip, lr}
	stmdaeq	r1, {r4, r5, r6, r9, fp, ip, lr}
	stmdaeq	r1, {r2, r3, r7, r9, fp, ip, lr}
	stmdaeq	r1, {r7, r9, fp, ip, lr}
	stmdaeq	r1, {r2, r3, r7, r9, fp, ip, lr}
	stmdaeq	r1, {r2, r3, r7, r9, fp, ip, lr}
	stmdaeq	r1, {r2, r3, r7, r9, fp, ip, lr}
	stmdaeq	r1, {r7, r9, fp, ip, lr}
	add	ip, r1, #8
	add	r4, r1, #7
	mov	r2, ip
	b .L_08015a8c
	add	ip, r1, #8
	mov	r4, ip
	add	r2, r1, #7
	b .L_08015a8c
	add	r2, r1, #8
	mov	ip, r2
	mov	r4, r2
.L_08015a8c:
	add	r1, r1, #8
	add	r6, r6, #1
	cmp	r6, #29
	bls .L_08015920
	add	r3, r4, r8, lsl #8
	strh	r3, [r0], #2
	add	r3, ip, lr, lsl #8
	add	r0, r0, #2
	strh	r3, [r0], #2
	add	r0, r0, #2
	strh	r3, [r0], #2
	add	r0, r0, #2
	strh	r3, [r0], #2
	add	sl, sl, #4
	add	r0, r0, #2
	strh	r3, [r0], #2
	mov	r1, r9
	add	r0, r0, #2
	strh	r3, [r0], #2
	cmp	r1, #19
	add	r0, r0, #2
	strh	r3, [r0], #2
	add	r3, r2, r7, lsl #8
	add	r0, r0, #2
	strh	r3, [r0], #2
	bls .L_080158f4
	pop	{r5, r6, r7, r8, r9, sl, lr}
	bx	lr
