.syntax unified
	.thumb
	.set sub_08002090, 0x08002090
	.set sub_08002096, 0x08002096
	.set sub_08013164, 0x08013164
	.set sub_0801401c, 0x0801401c
	.set sub_080140d8, 0x080140d8
	.set sub_080142d4, 0x080142d4
	.set sub_080143ac, 0x080143ac
	.set sub_080145a8, 0x080145a8
	.set sub_080148e8, 0x080148e8
	.set sub_08014d00, 0x08014d00
	.set sub_08014d78, 0x08014d78
	.set sub_0801587c, 0x0801587c
	.set sub_080cdf5c, 0x080cdf5c
	.set sub_080d2d84, 0x080d2d84
	.set sub_080da060, 0x080da060
	.global Func_080da968
	.thumb_func
Func_080da968:
	push	{lr}
	ldr	r2, [r0, #80]
	movs	r3, #253
	strb	r3, [r2, #22]
	movs	r3, #12
	strb	r3, [r2, #23]
	ldr	r3, [pc, #40]
	movs	r2, #128
	ldr	r3, [r3, #12]
	lsls	r2, r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080da988
	ldr	r2, [pc, #32]
	movs	r3, #1
	str	r3, [r2, #0]
.L_080da988:
	ldr	r3, [pc, #24]
	ldr	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_080da99c
	movs	r0, #0
	movs	r1, #12
	movs	r2, #13
	movs	r3, #0
	bl	sub_080da060
.L_080da99c:
	pop	{pc}
	movs	r0, r0
	.4byte 0x03001150
	.2byte 0x3950
	.2byte 0x080f
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r1, #128
	lsls	r1, r1, #5
	adds	r1, #228
	adds	r5, r0, #0
	movs	r0, #164
	sub	sp, #4
	bl	sub_08014d00
	movs	r3, #128
	lsls	r3, r3, #3
	mov	r8, r3
	adds	r7, r0, #0
	mov	r0, r8
	bl	sub_08014d78
	movs	r3, #0
	adds	r6, r0, #0
	mov	r0, sp
	str	r3, [r0, #0]
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #212
	adds	r1, r7, #0
	ldr	r2, [pc, #168]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	bl	sub_080143ac
	movs	r3, #186
	lsls	r3, r3, #1
	str	r0, [r7, #0]
	cmp	r5, r3
	bne.n	.L_080daa28
	adds	r1, r6, #0
	ldr	r0, [pc, #152]
	bl	sub_0801587c
	movs	r3, #128
	lsls	r3, r3, #1
	adds	r1, r6, r3
	ldr	r0, [pc, #144]
	bl	sub_0801587c
	movs	r3, #128
	lsls	r3, r3, #2
	adds	r1, r6, r3
	ldr	r0, [pc, #136]
	bl	sub_0801587c
	movs	r3, #192
	lsls	r3, r3, #2
	adds	r1, r6, r3
	ldr	r0, [pc, #128]
	bl	sub_0801587c
	ldr	r0, [r7, #0]
	mov	r1, r8
	adds	r2, r6, #0
	bl	sub_080142d4
	b.n	.L_080daa60
.L_080daa28:
	adds	r1, r6, #0
	ldr	r0, [pc, #112]
	bl	sub_0801587c
	ldr	r5, [pc, #108]
	movs	r3, #128
	lsls	r3, r3, #1
	adds	r1, r6, r3
	adds	r0, r5, #0
	bl	sub_0801587c
	movs	r3, #128
	lsls	r3, r3, #2
	adds	r1, r6, r3
	adds	r0, r5, #0
	bl	sub_0801587c
	movs	r3, #192
	lsls	r3, r3, #2
	adds	r1, r6, r3
	ldr	r0, [pc, #80]
	bl	sub_0801587c
	ldr	r0, [r7, #0]
	mov	r1, r8
	adds	r2, r6, #0
	bl	sub_080142d4
.L_080daa60:
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #64]
	bl	sub_080145a8
	movs	r1, #228
	lsls	r1, r1, #2
	adds	r1, #255
	ldr	r0, [pc, #56]
	bl	sub_080145a8
	adds	r0, r6, #0
	bl	sub_08013164
	adds	r0, r7, #0
	add	sp, #4
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x85000439
	.4byte 0x080f0c1c
	.4byte 0x080f0c63
	.4byte 0x080f0cb8
	.4byte 0x080f0d07
	.4byte 0x080f0d5e
	.4byte 0x080f0dbb
	.4byte 0x080f0e00
	.4byte 0x080daab1
	.2byte 0xacb9
	.2byte 0x080d
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r1, #192
	lsls	r1, r1, #18
	adds	r3, r1, #0
	sub	sp, #40
	movs	r2, #0
	adds	r3, #164
	ldr	r0, [r3, #0]
	str	r2, [sp, #24]
	mov	r8, r2
	ldr	r3, [r0, #0]
	ldr	r2, [pc, #456]
	lsls	r3, r3, #2
	adds	r3, r3, r2
	ldrh	r3, [r3, #2]
	adds	r0, #28
	lsrs	r3, r3, #5
	str	r3, [sp, #20]
	movs	r6, #0
	ldr	r1, [r1, #32]
	adds	r2, r1, #0
	adds	r2, #228
	ldr	r3, [r2, #0]
	str	r3, [sp, #16]
	ldr	r4, [sp, #16]
	ldr	r3, [pc, #432]
	ands	r4, r3
	str	r4, [sp, #16]
	ldr	r2, [r2, #4]
	ands	r2, r3
	str	r2, [sp, #12]
	ldr	r3, [r1, #0]
	ldr	r3, [r3, #4]
	str	r0, [sp, #0]
	movs	r0, #7
	str	r3, [sp, #8]
	str	r0, [sp, #28]
.L_080dab06:
	ldr	r1, [sp, #0]
	ldr	r5, [r1, #0]
	cmp	r5, #0
	bne.n	.L_080dab10
	b.n	.L_080dac6a
.L_080dab10:
	mov	r2, sp
	adds	r2, #32
	str	r2, [sp, #4]
.L_080dab16:
	ldr	r3, [r5, #4]
	ldr	r4, [r5, #8]
	mov	sl, r3
	ldr	r0, [r5, #12]
	movs	r3, #18
	ldrsb	r3, [r5, r3]
	mov	r9, r4
	mov	fp, r0
	cmp	r3, #1
	bgt.n	.L_080dab2c
	b.n	.L_080dac5a
.L_080dab2c:
	subs	r4, r6, r0
	adds	r2, r4, #0
	cmp	r4, #0
	bge.n	.L_080dab36
	subs	r2, r0, r6
.L_080dab36:
	mov	r3, r8
	mov	r7, r9
	subs	r1, r3, r7
	cmp	r1, #0
	blt.n	.L_080dab46
	cmp	r2, r1
	blt.n	.L_080dab50
	b.n	.L_080dab58
.L_080dab46:
	mov	r0, r9
	mov	r7, r8
	subs	r3, r0, r7
	cmp	r2, r3
	bge.n	.L_080dab58
.L_080dab50:
	ldr	r2, [sp, #24]
	mov	r3, sl
	subs	r0, r2, r3
	b.n	.L_080dab60
.L_080dab58:
	ldr	r7, [sp, #24]
	mov	r1, sl
	subs	r0, r7, r1
	adds	r1, r4, #0
.L_080dab60:
	bl	sub_080148e8
	lsls	r0, r0, #16
	lsrs	r7, r0, #16
	movs	r3, #18
	ldrsb	r3, [r5, r3]
	ldr	r2, [sp, #20]
	lsls	r3, r3, #3
	adds	r3, r2, r3
	adds	r4, r3, #0
	ldr	r3, [sp, #24]
	ldr	r1, [sp, #16]
	add	r3, sl
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	subs	r0, r3, r1
	mov	r3, r8
	add	r3, r9
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	ldr	r2, [sp, #8]
	asrs	r3, r3, #1
	mov	r1, fp
	subs	r2, r3, r2
	adds	r3, r6, r1
	mov	r8, r2
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	ldr	r2, [sp, #12]
	ldr	r1, [sp, #8]
	asrs	r3, r3, #1
	subs	r3, r3, r2
	subs	r6, r3, r1
	ldr	r1, [pc, #252]
	mov	r3, r8
	subs	r2, r6, r3
	adds	r3, r0, r1
	ldr	r1, [pc, #248]
	subs	r4, #16
	cmp	r3, r1
	bhi.n	.L_080dac5a
	ldr	r3, [pc, #244]
	cmp	r2, r3
	ble.n	.L_080dac5a
	ldr	r1, [pc, #244]
	cmp	r2, r1
	bgt.n	.L_080dac5a
	movs	r3, #128
	asrs	r1, r0, #16
	lsls	r3, r3, #1
	adds	r3, #255
	subs	r1, #8
	asrs	r2, r2, #16
	ands	r1, r3
	mov	r0, r8
	movs	r3, #255
	subs	r2, #8
	ands	r2, r3
	adds	r3, r0, r6
	asrs	r3, r3, #16
	adds	r3, #34
	adds	r6, r5, #0
	mov	r8, r3
	adds	r6, #20
	movs	r3, #0
	str	r3, [r6, #0]
	ldr	r3, [pc, #204]
	lsls	r1, r1, #16
	orrs	r2, r1
	orrs	r2, r3
	movs	r3, #128
	lsls	r3, r3, #4
	orrs	r4, r3
	str	r2, [r5, #24]
	str	r4, [r5, #28]
	cmp	r7, #0
	beq.n	.L_080dac50
	ldr	r4, [sp, #4]
	ldr	r1, [pc, #160]
	ldr	r3, [r4, #4]
	lsls	r2, r7, #16
	ands	r3, r1
	lsrs	r2, r2, #16
	orrs	r3, r2
	str	r3, [r4, #4]
	ldr	r3, [sp, #32]
	movs	r2, #128
	ands	r3, r1
	lsls	r2, r2, #1
	orrs	r3, r2
	movs	r2, #255
	lsls	r2, r2, #8
	adds	r2, #255
	ands	r3, r2
	movs	r2, #128
	lsls	r2, r2, #17
	orrs	r3, r2
	str	r3, [sp, #32]
	movs	r0, #4
	ldrb	r3, [r5, #25]
	negs	r0, r0
	adds	r2, r0, #0
	ands	r3, r2
	movs	r2, #1
	orrs	r3, r2
	strb	r3, [r5, #25]
	ldr	r0, [sp, #4]
	bl	sub_0801401c
	movs	r3, #31
	ands	r0, r3
	movs	r1, #63
	ldrb	r3, [r5, #27]
	negs	r1, r1
	adds	r2, r1, #0
	lsls	r0, r0, #1
	ands	r3, r2
	orrs	r3, r0
	strb	r3, [r5, #27]
.L_080dac50:
	strh	r7, [r5, #16]
	adds	r0, r6, #0
	mov	r1, r8
	bl	sub_080140d8
.L_080dac5a:
	mov	r2, sl
	str	r2, [sp, #24]
	mov	r8, r9
	ldr	r5, [r5, #0]
	mov	r6, fp
	cmp	r5, #0
	beq.n	.L_080dac6a
	b.n	.L_080dab16
.L_080dac6a:
	movs	r3, #0
	str	r3, [sp, #24]
	ldr	r4, [sp, #0]
	mov	r8, r3
	ldr	r5, [r4, #0]
	movs	r6, #0
	cmp	r5, #0
	beq.n	.L_080dac7c
	strh	r7, [r5, #16]
.L_080dac7c:
	ldr	r0, [sp, #0]
	ldr	r1, [sp, #28]
	adds	r0, #28
	subs	r1, #1
	str	r0, [sp, #0]
	str	r1, [sp, #28]
	cmp	r1, #0
	blt.n	.L_080dac8e
	b.n	.L_080dab06
.L_080dac8e:
	add	sp, #40
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x020036e0
	.4byte 0xffff0000
	.4byte 0x001fffff
	.4byte 0x012ffffe
	.4byte 0xffe00000
	.4byte 0x00dfffff
	.2byte 0x2000
	.2byte 0x4000
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #164
	movs	r0, #0
	sub	sp, #24
	ldr	r5, [r3, #0]
	mov	r9, r0
	bl	sub_080cdf5c
	bl	sub_080d2d84
	movs	r1, #128
	lsls	r1, r1, #1
	str	r0, [sp, #20]
	str	r1, [sp, #4]
	adds	r5, #4
	mov	r8, r5
.L_080dace8:
	mov	r3, r8
	ldr	r2, [r3, #24]
	cmp	r2, #0
	bne.n	.L_080dacf2
	b.n	.L_080dae08
.L_080dacf2:
	ldr	r3, [r3, #20]
	cmp	r3, #0
	bne.n	.L_080dacfa
	b.n	.L_080dae08
.L_080dacfa:
	mov	r0, r8
	ldr	r3, [r0, #16]
	movs	r1, #128
	ldr	r7, [r2, #0]
	lsls	r1, r1, #2
	adds	r3, r3, r1
	str	r3, [r0, #16]
	ldr	r2, [r7, #4]
	mov	r1, r8
	str	r2, [sp, #16]
	ldr	r3, [r7, #8]
	ldr	r2, [sp, #20]
	str	r3, [sp, #12]
	ldr	r0, [r7, #12]
	str	r0, [sp, #8]
	ldr	r0, [sp, #16]
	ldr	r3, [r2, #8]
	ldr	r1, [r1, #4]
	subs	r2, r0, r3
	mov	fp, r1
	cmp	r2, #0
	blt.n	.L_080dad2e
	ldr	r1, [pc, #256]
	cmp	r2, r1
	ble.n	.L_080dad38
	b.n	.L_080dad68
.L_080dad2e:
	ldr	r2, [sp, #16]
	ldr	r0, [pc, #244]
	subs	r3, r3, r2
	cmp	r3, r0
	bgt.n	.L_080dad68
.L_080dad38:
	ldr	r1, [sp, #20]
	ldr	r2, [sp, #8]
	ldr	r3, [r1, #16]
	ldr	r0, [pc, #236]
	ldr	r1, [pc, #236]
	subs	r3, r2, r3
	adds	r3, r3, r0
	cmp	r3, r1
	bhi.n	.L_080dad68
	ldr	r2, [sp, #12]
	ldr	r1, [sp, #20]
	movs	r0, #128
	lsls	r0, r0, #12
	adds	r3, r2, r0
	ldr	r2, [r1, #12]
	cmp	r3, r2
	ble.n	.L_080dad68
	ldr	r0, [sp, #12]
	ldr	r1, [pc, #212]
	adds	r3, r0, r1
	cmp	r3, r2
	bge.n	.L_080dad68
	movs	r2, #32
	str	r2, [sp, #4]
.L_080dad68:
	movs	r0, #200
	lsls	r0, r0, #2
	ldr	r7, [r7, #0]
	str	r0, [sp, #0]
	movs	r3, #1
	mov	r9, r3
	mov	r6, fp
.L_080dad76:
	mov	r1, r8
	ldr	r3, [r1, #8]
	ldr	r0, [r1, #16]
	mov	r2, r9
	muls	r2, r3
	ldr	r3, [sp, #0]
	mov	sl, r2
	subs	r0, r0, r3
	bl	sub_08002096
	lsrs	r1, r6, #31
	adds	r1, r6, r1
	asrs	r1, r1, #1
	ldr	r2, [pc, #164]
	add	r1, fp
	mov	lr, r2
	.2byte 0xf800
	.2byte 0x1c05
	bl	sub_08002096
	ldr	r3, [pc, #152]
	mov	r1, sl
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x9904
	add	r6, fp
	subs	r0, r1, r0
	str	r0, [r7, #4]
	adds	r0, r5, #0
	bl	sub_08002090
	mov	r1, sl
	ldr	r2, [pc, #128]
	mov	lr, r2
	.2byte 0xf800
	.2byte 0x9b03
	movs	r2, #200
	subs	r0, r3, r0
	str	r0, [r7, #8]
	ldr	r0, [sp, #8]
	movs	r3, #1
	str	r0, [r7, #12]
	ldr	r1, [sp, #0]
	lsls	r2, r2, #2
	add	r9, r3
	adds	r1, r1, r2
	mov	r0, r9
	ldr	r7, [r7, #0]
	str	r1, [sp, #0]
	cmp	r0, #5
	ble.n	.L_080dad76
	mov	r1, r8
	ldr	r0, [r1, #4]
	ldr	r2, [sp, #4]
	cmp	r0, r2
	ble.n	.L_080dadf8
	movs	r1, #253
	lsls	r1, r1, #8
	adds	r1, #112
	ldr	r3, [pc, #72]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x4641
	str	r0, [r1, #4]
	b.n	.L_080dae08
.L_080dadf8:
	movs	r1, #142
	lsls	r1, r1, #9
	adds	r1, #40
	ldr	r2, [pc, #56]
	mov	lr, r2
	.2byte 0xf800
	.2byte 0x4643
	str	r0, [r3, #4]
.L_080dae08:
	movs	r0, #1
	add	r9, r0
	movs	r1, #28
	mov	r2, r9
	add	r8, r1
	cmp	r2, #7
	bgt.n	.L_080dae18
	b.n	.L_080dace8
.L_080dae18:
	add	sp, #24
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x0007ffff
	.4byte 0x000fffff
	.4byte 0x000ffffe
	.4byte 0xffb00000
	.2byte 0x021c
	.2byte 0x0300
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #164
	ldr	r4, [r3, #0]
	movs	r0, #0
	ldr	r3, [r4, #28]
	adds	r1, r0, #0
	cmp	r3, #0
	bne.n	.L_080dae54
	adds	r0, r4, #4
	b.n	.L_080dae6e
.L_080dae54:
	adds	r1, #1
	cmp	r1, #7
	bgt.n	.L_080dae6e
	lsls	r3, r1, #3
	subs	r3, r3, r1
	lsls	r2, r3, #2
	adds	r3, r2, #0
	adds	r3, #28
	ldr	r3, [r4, r3]
	cmp	r3, #0
	bne.n	.L_080dae54
	adds	r3, r4, r2
	adds	r0, r3, #4
.L_080dae6e:
	pop	{pc}
	push	{r5, r6, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #164
	ldr	r4, [r3, #0]
	movs	r5, #0
	movs	r2, #4
	ldrsh	r3, [r4, r2]
	adds	r1, r5, #0
	cmp	r3, r0
	bne.n	.L_080dae8a
	adds	r5, r4, #4
	b.n	.L_080daea2
.L_080dae8a:
	adds	r1, #1
	cmp	r1, #7
	bgt.n	.L_080daea2
	lsls	r3, r1, #3
	subs	r3, r3, r1
	lsls	r2, r3, #2
	adds	r3, r2, #4
	ldrsh	r3, [r4, r3]
	cmp	r3, r0
	bne.n	.L_080dae8a
	adds	r3, r4, r2
	adds	r5, r3, #4
.L_080daea2:
	adds	r0, r5, #0
	pop	{r5, r6, pc}
	.2byte 0x0000
