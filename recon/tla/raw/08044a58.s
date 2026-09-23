.syntax unified
	.thumb
	.set sub_0801314c, 0x0801314c
	.set sub_08013560, 0x08013560
	.set sub_08014128, 0x08014128
	.set sub_08014274, 0x08014274
	.set sub_080142d4, 0x080142d4
	.set sub_080143ac, 0x080143ac
	.set sub_08014d00, 0x08014d00
	.set sub_0801591c, 0x0801591c
	.set sub_08039260, 0x08039260
	.set sub_0803939c, 0x0803939c
	.set sub_0803954c, 0x0803954c
	.set sub_0803ccd0, 0x0803ccd0
	.set sub_0803cf60, 0x0803cf60
	.set sub_0803cf6c, 0x0803cf6c
	.set sub_0803d2f0, 0x0803d2f0
	.set sub_0803dab0, 0x0803dab0
	.set sub_08041c54, 0x08041c54
	.set sub_08042314, 0x08042314
	.set sub_080ad188, 0x080ad188
	.set sub_081c0010, 0x081c0010
	.set sub_081c0048, 0x081c0048
	.global Func_08044a58
	.thumb_func
Func_08044a58:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #32
	str	r2, [sp, #8]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r5, [r3, #60]
	ldr	r2, [pc, #252]
	adds	r6, r1, #0
	movs	r3, #3
	ands	r3, r6
	movs	r1, #0
	lsls	r3, r3, #1
	mov	r9, r1
	mov	sl, r1
	ldrsh	r7, [r2, r3]
	add	r2, sp, #20
	mov	r3, r9
	str	r3, [sp, #0]
	mov	fp, r0
	mov	r8, r2
	movs	r0, #2
	movs	r1, #1
	movs	r2, #26
	movs	r3, #5
	bl	sub_08039260
	mov	r9, r0
	cmp	r0, #0
	beq.n	.L_08044b60
	movs	r3, #4
	movs	r1, #4
	movs	r2, #0
	str	r3, [sp, #0]
	bl	sub_08041c54
	movs	r3, #1
	strb	r3, [r5, #3]
	adds	r0, r7, #0
	bl	sub_0803d2f0
	movs	r1, #14
	add	r2, sp, #16
	add	r3, sp, #12
	str	r1, [sp, #0]
	mov	r1, sl
	str	r1, [sp, #4]
	bl	sub_0803dab0
	mov	r2, sl
	mov	r3, r8
	str	r2, [r3, #0]
	ldr	r3, [pc, #168]
	movs	r2, #224
	str	r3, [sp, #24]
	ldr	r3, [sp, #12]
	movs	r1, #152
	lsls	r2, r2, #8
	lsls	r1, r1, #5
	orrs	r3, r2
	adds	r1, #132
	str	r3, [sp, #28]
	mov	r2, sl
	adds	r3, r5, r1
	adds	r1, #2
	strh	r2, [r3, #0]
	adds	r3, r5, r1
	strh	r2, [r3, #0]
	mov	r0, fp
	movs	r1, #1
	bl	sub_0803ccd0
	lsls	r0, r6, #2
	ldr	r3, [sp, #8]
	adds	r0, r0, r6
	lsls	r0, r0, #2
	movs	r1, #150
	adds	r0, r0, r3
	lsls	r1, r1, #1
	adds	r0, r0, r1
	movs	r1, #4
	bl	sub_0803ccd0
	ldr	r0, [pc, #108]
	movs	r5, #129
	adds	r0, r6, r0
	bl	sub_0803cf60
	mov	r2, sl
	adds	r1, r0, #0
	str	r2, [sp, #0]
	movs	r3, #2
	movs	r2, #36
	mov	r0, r9
	bl	sub_0803954c
	movs	r0, #81
	bl	sub_081c0010
	ldr	r6, [pc, #80]
	lsls	r5, r5, #2
	adds	r5, #255
.L_08044b2e:
	movs	r1, #250
	mov	r0, r8
	bl	sub_08014128
	movs	r0, #1
	bl	sub_08013560
	bl	sub_081c0048
	cmp	r0, #0
	beq.n	.L_08044b4c
	ldr	r3, [r6, #4]
	ands	r3, r5
	cmp	r3, #0
	beq.n	.L_08044b2e
.L_08044b4c:
	mov	r0, r9
	movs	r1, #2
	bl	sub_0803939c
	movs	r0, #1
	bl	sub_08013560
	ldr	r0, [sp, #16]
	bl	sub_08014274
.L_08044b60:
	add	sp, #32
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x0805f65e
	.4byte 0x8014000c
	.4byte 0x0000002e
	.2byte 0x1150
	.2byte 0x0300
.L_08044b80:
	push	{lr}
	cmp	r0, #8
	bls.n	.L_08044b8a
	movs	r0, #0
	b.n	.L_08044b90
.L_08044b8a:
	ldr	r3, [pc, #8]
	lsls	r2, r0, #1
	ldrsh	r0, [r3, r2]
.L_08044b90:
	pop	{pc}
	movs	r0, r0
	.2byte 0xf666
	.2byte 0x0805
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r3, #192
	sub	sp, #28
	lsls	r3, r3, #18
	movs	r2, #0
	ldr	r5, [r3, #60]
	mov	sl, r0
	str	r2, [sp, #0]
	movs	r0, #2
	movs	r1, #1
	movs	r2, #26
	movs	r3, #5
	add	r7, sp, #16
	bl	sub_08039260
	movs	r6, #0
	mov	r8, r0
	cmp	r0, #0
	beq.n	.L_08044c6a
	movs	r1, #4
	movs	r2, #0
	movs	r3, #4
	str	r3, [sp, #0]
	bl	sub_08041c54
	movs	r3, #1
	strb	r3, [r5, #3]
	mov	r0, sl
	bl	.L_08044b80
	bl	sub_0803d2f0
	movs	r1, #14
	add	r2, sp, #12
	add	r3, sp, #8
	str	r1, [sp, #0]
	movs	r1, #0
	str	r6, [sp, #4]
	bl	sub_0803dab0
	ldr	r3, [pc, #132]
	str	r6, [r7, #0]
	str	r3, [sp, #20]
	ldr	r3, [sp, #8]
	movs	r2, #224
	lsls	r2, r2, #8
	orrs	r3, r2
	movs	r2, #152
	lsls	r2, r2, #5
	adds	r2, #132
	str	r3, [sp, #24]
	adds	r3, r5, r2
	adds	r2, #2
	strh	r6, [r3, #0]
	adds	r3, r5, r2
	strh	r6, [r3, #0]
	movs	r1, #1
	mov	r0, sl
	bl	sub_0803ccd0
	ldr	r0, [pc, #96]
	bl	sub_0803cf60
	movs	r2, #36
	adds	r1, r0, #0
	movs	r3, #2
	mov	r0, r8
	str	r6, [sp, #0]
	bl	sub_0803954c
	movs	r0, #81
	bl	sub_081c0010
	movs	r5, #129
	ldr	r6, [pc, #72]
	lsls	r5, r5, #2
	adds	r5, #255
.L_08044c38:
	movs	r1, #250
	adds	r0, r7, #0
	bl	sub_08014128
	movs	r0, #1
	bl	sub_08013560
	bl	sub_081c0048
	cmp	r0, #0
	beq.n	.L_08044c56
	ldr	r3, [r6, #4]
	ands	r3, r5
	cmp	r3, #0
	beq.n	.L_08044c38
.L_08044c56:
	mov	r0, r8
	movs	r1, #2
	bl	sub_0803939c
	movs	r0, #1
	bl	sub_08013560
	ldr	r0, [sp, #12]
	bl	sub_08014274
.L_08044c6a:
	add	sp, #28
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.4byte 0x8014000c
	.4byte 0x0000002b
	.2byte 0x1150
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #48
	str	r1, [sp, #8]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #60]
	movs	r2, #0
	mov	r8, r3
	add	r3, sp, #24
	str	r2, [sp, #0]
	mov	fp, r0
	mov	sl, r3
	movs	r0, #1
	movs	r1, #1
	movs	r2, #28
	movs	r3, #5
	bl	sub_08039260
	movs	r6, #0
	mov	r9, r0
	cmp	r0, #0
	beq.n	.L_08044daa
	movs	r1, #8
	movs	r3, #4
	movs	r2, #0
	str	r3, [sp, #0]
	bl	sub_08041c54
	movs	r3, #1
	mov	r2, r8
	strb	r3, [r2, #3]
	mov	r0, fp
	bl	.L_08044b80
	bl	sub_0803d2f0
	movs	r3, #14
	add	r5, sp, #16
	add	r2, sp, #20
	str	r3, [sp, #0]
	movs	r1, #0
	adds	r3, r5, #0
	str	r6, [sp, #4]
	bl	sub_0803dab0
	mov	r3, sl
	str	r6, [r3, #0]
	ldr	r3, [pc, #204]
	movs	r2, #224
	str	r3, [sp, #28]
	ldr	r3, [sp, #16]
	lsls	r2, r2, #8
	ldr	r0, [sp, #8]
	orrs	r3, r2
	str	r3, [sp, #32]
	add	r7, sp, #36
	bl	.L_08044b80
	bl	sub_0803d2f0
	movs	r3, #15
	add	r2, sp, #12
	str	r3, [sp, #0]
	movs	r1, #0
	adds	r3, r5, #0
	str	r6, [sp, #4]
	bl	sub_0803dab0
	ldr	r3, [pc, #164]
	str	r6, [r7, #0]
	str	r3, [sp, #40]
	ldr	r3, [sp, #16]
	movs	r2, #240
	lsls	r2, r2, #8
	orrs	r3, r2
	str	r3, [sp, #44]
	movs	r3, #152
	lsls	r3, r3, #5
	adds	r3, #132
	add	r3, r8
	strh	r6, [r3, #0]
	movs	r3, #152
	lsls	r3, r3, #5
	adds	r3, #134
	add	r3, r8
	strh	r6, [r3, #0]
	mov	r0, fp
	movs	r1, #1
	bl	sub_0803ccd0
	movs	r1, #1
	ldr	r0, [sp, #8]
	bl	sub_0803ccd0
	ldr	r0, [pc, #116]
	bl	sub_0803cf60
	movs	r2, #68
	adds	r1, r0, #0
	movs	r3, #2
	mov	r0, r9
	str	r6, [sp, #0]
	bl	sub_0803954c
	movs	r0, #81
	bl	sub_081c0010
.L_08044d62:
	movs	r1, #250
	mov	r0, sl
	bl	sub_08014128
	movs	r1, #250
	adds	r0, r7, #0
	bl	sub_08014128
	movs	r0, #1
	bl	sub_08013560
	bl	sub_081c0048
	cmp	r0, #0
	beq.n	.L_08044d90
	ldr	r3, [pc, #64]
	movs	r2, #129
	ldr	r3, [r3, #4]
	lsls	r2, r2, #2
	adds	r2, #255
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08044d62
.L_08044d90:
	movs	r1, #2
	mov	r0, r9
	bl	sub_0803939c
	movs	r0, #1
	bl	sub_08013560
	ldr	r0, [sp, #20]
	bl	sub_08014274
	ldr	r0, [sp, #12]
	bl	sub_08014274
.L_08044daa:
	add	sp, #48
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x800c000c
	.4byte 0x802c000c
	.4byte 0x0000002d
	.2byte 0x1150
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	sub	sp, #52
	lsls	r3, r3, #18
	movs	r2, #0
	ldr	r5, [r3, #60]
	adds	r7, r0, #0
	str	r2, [sp, #0]
	movs	r0, #4
	movs	r1, #1
	movs	r2, #22
	movs	r3, #5
	bl	sub_08039260
	movs	r6, #0
	mov	r9, r0
	cmp	r0, #0
	bne.n	.L_08044dfa
	b.n	.L_08044f28
.L_08044dfa:
	movs	r2, #152
	lsls	r2, r2, #5
	movs	r3, #1
	adds	r2, #132
	strb	r3, [r5, #3]
	adds	r3, r5, r2
	adds	r2, #2
	strh	r6, [r3, #0]
	adds	r3, r5, r2
	strh	r6, [r3, #0]
	ldr	r2, [pc, #296]
	movs	r3, #31
	ands	r3, r7
	lsls	r3, r3, #1
	ldrh	r0, [r2, r3]
	movs	r1, #4
	bl	sub_0803ccd0
	ldr	r0, [pc, #284]
	bl	sub_0803cf60
	mov	sl, r0
	mov	r1, sl
	movs	r2, #0
	movs	r3, #2
	mov	r0, r9
	str	r6, [sp, #0]
	bl	sub_0803954c
	movs	r0, #81
	bl	sub_081c0010
	movs	r5, #129
	ldr	r6, [pc, #256]
	lsls	r5, r5, #2
	adds	r5, #255
.L_08044e42:
	movs	r0, #1
	bl	sub_08013560
	bl	sub_081c0048
	cmp	r0, #0
	beq.n	.L_08044e58
	ldr	r3, [r6, #4]
	ands	r3, r5
	cmp	r3, #0
	beq.n	.L_08044e42
.L_08044e58:
	mov	r0, r9
	movs	r1, #2
	bl	sub_0803939c
	movs	r0, #1
	bl	sub_08013560
	movs	r3, #0
	str	r3, [sp, #0]
	movs	r0, #3
	movs	r1, #1
	movs	r2, #24
	movs	r3, #7
	bl	sub_08039260
	mov	r9, r0
	cmp	r0, #0
	beq.n	.L_08044f28
	ldr	r2, [pc, #184]
	movs	r3, #31
	ands	r3, r7
	lsls	r3, r3, #1
	ldrh	r0, [r2, r3]
	movs	r1, #4
	bl	sub_0803ccd0
	ldr	r0, [pc, #180]
	bl	sub_0803cf60
	mov	sl, r0
	adds	r0, r7, #0
	bl	sub_080ad188
	add	r7, sp, #8
	adds	r2, r7, #0
	adds	r0, #4
	movs	r1, #3
.L_08044ea2:
	ldrb	r3, [r0, #0]
	subs	r1, #1
	adds	r0, #1
	stmia	r2!, {r3}
	cmp	r1, #0
	bge.n	.L_08044ea2
	movs	r3, #0
	mov	r8, r3
	mov	fp, r7
.L_08044eb4:
	movs	r5, #0
	movs	r0, #0
	movs	r1, #0
	mov	r2, fp
.L_08044ebc:
	ldmia	r2!, {r3}
	cmp	r0, r3
	bge.n	.L_08044ec6
	adds	r5, r1, #0
	adds	r0, r3, #0
.L_08044ec6:
	adds	r1, #1
	cmp	r1, #3
	ble.n	.L_08044ebc
	movs	r1, #5
	bl	sub_0803ccd0
	ldr	r6, [pc, #116]
	adds	r0, r5, r6
	bl	sub_0803cf6c
	movs	r2, #1
	lsls	r3, r5, #2
	add	r8, r2
	movs	r5, #0
	str	r5, [r7, r3]
	mov	r3, r8
	cmp	r3, #1
	ble.n	.L_08044eb4
	adds	r0, r6, #4
	bl	sub_0803cf6c
	mov	r0, r9
	mov	r1, sl
	movs	r2, #4
	movs	r3, #2
	str	r5, [sp, #0]
	bl	sub_0803954c
	movs	r5, #129
	ldr	r6, [pc, #60]
	lsls	r5, r5, #2
	adds	r5, #255
.L_08044f06:
	movs	r0, #1
	bl	sub_08013560
	ldr	r3, [r6, #4]
	ands	r3, r5
	cmp	r3, #0
	beq.n	.L_08044f06
	mov	r0, r9
	movs	r1, #2
	bl	sub_0803939c
	movs	r0, #1
	bl	sub_08013560
	ldr	r0, [sp, #4]
	bl	sub_08014274
.L_08044f28:
	add	sp, #52
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x0805f676
	.4byte 0x00000032
	.4byte 0x03001150
	.4byte 0x00000033
	.2byte 0x0034
	.2byte 0x0000
.L_08044f4c:
	push	{r5, r6, r7, lr}
	adds	r7, r1, #0
	movs	r1, #128
	adds	r5, r0, #0
	lsls	r1, r1, #3
	movs	r0, #56
	bl	sub_08014d00
	ldr	r3, [pc, #36]
	lsls	r5, r5, #2
	adds	r6, r0, #0
	ldr	r0, [r3, r5]
	cmp	r7, #95
	bgt.n	.L_08044f80
	adds	r1, r6, #0
	bl	sub_0801591c
	movs	r1, #128
	lsls	r1, r1, #2
	adds	r0, r7, #0
	adds	r2, r6, #0
	bl	sub_080142d4
	movs	r0, #56
	bl	sub_0801314c
.L_08044f80:
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.2byte 0x9978
	.2byte 0x0805
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #4
	adds	r5, r0, #0
	mov	r9, r1
	adds	r6, r2, #0
	mov	sl, r3
	bl	sub_080143ac
	adds	r7, r0, #0
	movs	r0, #0
	cmp	r7, #95
	bgt.n	.L_08045008
	adds	r0, r5, #0
	adds	r1, r7, #0
	bl	.L_08044f4c
	ldr	r2, [pc, #72]
	mov	r3, sl
	mov	fp, r2
	str	r3, [sp, #0]
	mov	r1, fp
	mov	r2, r9
	adds	r3, r6, #0
	adds	r0, r7, #0
	bl	sub_08042314
	movs	r5, #253
	mov	r2, sl
	adds	r3, r6, #0
	strb	r5, [r0, #15]
	mov	r8, r0
	adds	r3, #32
	str	r2, [sp, #0]
	mov	r1, fp
	mov	r2, r9
	adds	r0, r7, #0
	bl	sub_08042314
	ldrh	r1, [r0, #24]
	ldr	r3, [pc, #24]
	lsls	r2, r1, #22
	lsrs	r2, r2, #22
	adds	r2, #8
	ands	r2, r3
	ldr	r3, [pc, #20]
	strb	r5, [r0, #15]
	ands	r3, r1
	orrs	r3, r2
	strh	r3, [r0, #24]
	mov	r0, r8
	b.n	.L_08045008
	movs	r0, r0
	.4byte 0x000003ff
	.4byte 0x80004000
	.2byte 0xfc00
	.2byte 0xffff
.L_08045008:
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.align 2, 0
