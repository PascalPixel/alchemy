.syntax unified
	.thumb
	.set sub_08016cfc, 0x08016cfc
	.set sub_08020098, 0x08020098
	.set sub_080cad84, 0x080cad84
	.set sub_080d170c, 0x080d170c
	.set sub_080d17ac, 0x080d17ac
	.global Func_080dc164
	.thumb_func
Func_080dc164:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	adds	r7, r1, #0
	ldr	r1, [pc, #60]
	mov	r8, r0
	adds	r6, r2, #0
	movs	r5, #0
	mov	sl, r1
.L_080dc178:
	adds	r0, r5, #0
	bl	sub_080cad84
	mov	r1, sl
	movs	r2, #0
	ldrsh	r3, [r1, r2]
	cmp	r5, r3
	beq.n	.L_080dc19c
	cmp	r0, #0
	beq.n	.L_080dc19c
	cmp	r0, r8
	beq.n	.L_080dc19c
	adds	r3, r0, #0
	adds	r3, #91
	strb	r7, [r3, #0]
	adds	r1, r6, #0
	bl	sub_08020098
.L_080dc19c:
	adds	r5, #1
	cmp	r5, #80
	ble.n	.L_080dc178
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.2byte 0x04aa
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r5, [r3, #108]
	adds	r3, #128
	ldr	r4, [r3, #0]
	movs	r2, #154
	lsls	r2, r2, #5
	adds	r7, r0, #0
	adds	r0, r4, r2
	movs	r2, #224
	movs	r3, #128
	lsls	r2, r2, #3
	adds	r2, #98
	lsls	r3, r3, #19
	adds	r6, r1, #0
	adds	r3, #212
	adds	r1, r5, r2
	ldr	r2, [pc, #100]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r2, #192
	lsls	r2, r2, #4
	adds	r2, #164
	adds	r3, r5, r2
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	bne.n	.L_080dc206
	movs	r2, #224
	lsls	r2, r2, #4
	adds	r0, r4, r2
	movs	r2, #128
	movs	r3, #128
	lsls	r2, r2, #2
	adds	r2, #34
	lsls	r3, r3, #19
	adds	r1, r5, r2
	adds	r3, #212
	ldr	r2, [pc, #56]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
.L_080dc206:
	movs	r2, #224
	lsls	r2, r2, #4
	adds	r0, r4, r2
	movs	r3, #128
	movs	r2, #224
	lsls	r2, r2, #2
	lsls	r3, r3, #19
	adds	r1, r4, r2
	adds	r3, #212
	ldr	r2, [pc, #36]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r0, #128
	lsls	r0, r0, #14
	orrs	r0, r6
	movs	r1, #1
	bl	sub_080d170c
	movs	r0, #132
	lsls	r0, r0, #1
	bl	sub_08016cfc
	adds	r0, r7, #0
	bl	sub_080d17ac
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x84000150
	.4byte 0x840002a0
