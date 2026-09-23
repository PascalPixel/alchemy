.syntax unified
	.thumb
	.set sub_080022ec, 0x080022ec
	.set sub_080022f4, 0x080022f4
	.set sub_080030f8, 0x080030f8
	.set sub_08004458, 0x08004458
	.set sub_08009080, 0x08009080
	.set sub_08009098, 0x08009098
	.set sub_080091f0, 0x080091f0
	.set sub_08009240, 0x08009240
	.set sub_08009248, 0x08009248
	.set sub_08092054, 0x08092054
	.set sub_08092560, 0x08092560
	.set sub_08092adc, 0x08092adc
	.set sub_080958a8, 0x080958a8
	.set sub_080958e4, 0x080958e4
	.set sub_08096bec, 0x08096bec
	.set sub_08096c80, 0x08096c80
	.set sub_080974d8, 0x080974d8
	.set sub_0809ba70, 0x0809ba70
	.set sub_0809ba7c, 0x0809ba7c
	.set sub_0809ba90, 0x0809ba90
	.set sub_080b0048, 0x080b0048
	.set sub_080b0050, 0x080b0050
	.set sub_080f9010, 0x080f9010
	.global Func_08096140
	.thumb_func
Func_08096140:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	ldr	r3, [pc, #372]
	adds	r5, r0, #0
	movs	r0, #250
	lsls	r0, r0, #1
	adds	r7, r3, r0
	ldr	r0, [r7, #0]
	sub	sp, #12
	bl	sub_08092054
	mov	sl, r0
	adds	r0, r5, #0
	bl	sub_08092054
	adds	r6, r0, #0
	cmp	r6, #0
	bne.n	.L_0809616c
	b.n	.L_080963fa
.L_0809616c:
	bl	sub_080958a8
	ldr	r3, [pc, #336]
	ldr	r3, [r3, #0]
	ldr	r0, [pc, #336]
	mov	r9, r3
	bl	sub_080b0048
	movs	r0, #30
	bl	sub_080030f8
	adds	r2, r6, #0
	movs	r3, #0
	adds	r2, #91
	strb	r3, [r2, #0]
	movs	r0, #152
	bl	sub_080f9010
	adds	r0, r5, #0
	movs	r1, #4
	movs	r2, #15
	bl	sub_08092560
	movs	r0, #152
	bl	sub_080f9010
	movs	r1, #4
	movs	r2, #15
	adds	r0, r5, #0
	bl	sub_08092560
	movs	r0, #30
	bl	sub_080030f8
	ldr	r3, [pc, #280]
	movs	r0, #153
	str	r3, [r6, #108]
	bl	sub_080f9010
	adds	r0, r5, #0
	movs	r1, #8
	movs	r2, #22
	bl	sub_08092560
	movs	r0, #140
	bl	sub_080f9010
	movs	r5, #128
	ldr	r1, [pc, #256]
	lsls	r5, r5, #9
	adds	r2, r5, #0
	adds	r0, r1, #0
	bl	sub_080091f0
	ldr	r3, [pc, #248]
	adds	r0, r6, #0
	str	r3, [r6, #108]
	movs	r1, #3
	bl	sub_08009080
	movs	r0, #90
	bl	sub_080030f8
	movs	r1, #128
	movs	r2, #0
	lsls	r1, r1, #7
	ldr	r0, [r7, #0]
	bl	sub_08092adc
	movs	r0, #20
	bl	sub_080030f8
	ldr	r0, [r7, #0]
	bl	sub_08092054
	movs	r1, #28
	bl	sub_08009080
	movs	r0, #30
	bl	sub_080030f8
	ldr	r1, [pc, #200]
	adds	r2, r5, #0
	adds	r0, r1, #0
	bl	sub_080091f0
	ldr	r3, [r6, #8]
	str	r3, [sp, #0]
	ldr	r3, [r6, #12]
	str	r3, [sp, #4]
	mov	r8, sp
	ldr	r3, [r6, #16]
	mov	r0, r8
	str	r3, [sp, #8]
	bl	sub_080974d8
	mov	r5, r9
	adds	r5, #88
	movs	r7, #23
	mov	r6, r8
.L_08096234:
	movs	r1, #142
	ldr	r2, [r6, #0]
	ldr	r3, [r6, #8]
	adds	r0, r5, #0
	lsls	r1, r1, #1
	bl	sub_0809ba90
	adds	r0, r5, #0
	ldr	r1, [pc, #148]
	bl	sub_0809ba7c
	adds	r0, r5, #0
	movs	r1, #7
	bl	sub_0809ba70
	ldr	r0, [r5, #0]
	movs	r1, #11
	bl	sub_08009248
	movs	r3, #128
	lsls	r3, r3, #8
	str	r3, [r5, #40]
	bl	sub_08004458
	movs	r2, #192
	lsls	r2, r2, #9
	adds	r0, r0, r2
	str	r0, [r5, #44]
	subs	r7, #1
	movs	r0, #1
	bl	sub_080030f8
	adds	r5, #72
	cmp	r7, #0
	bge.n	.L_08096234
	movs	r0, #140
	bl	sub_080030f8
	mov	r2, r9
	movs	r1, #2
	adds	r2, #152
	movs	r7, #23
.L_08096288:
	movs	r3, #5
	ldrsb	r3, [r2, r3]
	cmp	r3, #0
	beq.n	.L_08096292
	strb	r1, [r2, #0]
.L_08096292:
	subs	r7, #1
	adds	r2, #72
	cmp	r7, #0
	bge.n	.L_08096288
	movs	r0, #20
	bl	sub_080030f8
	movs	r0, #1
	movs	r1, #1
	movs	r2, #1
	bl	sub_080091f0
	movs	r0, #30
	bl	sub_080030f8
	ldr	r3, [pc, #8]
	movs	r7, #0
	mov	r5, r8
	mov	r9, r3
	b.n	.L_080962e0
	movs	r0, r0
	.4byte 0x00000000
	.4byte 0x02000240
	.4byte 0x03001f30
	.4byte 0x00201204
	.4byte 0x0809592d
	.4byte 0x00014ccc
	.4byte 0x08095f9d
	.4byte 0x00019999
	.2byte 0x6049
	.2byte 0x0809
.L_080962e0:
	mov	r0, sl
	ldr	r1, [r0, #8]
	str	r1, [r5, #0]
	movs	r3, #240
	ldr	r2, [r0, #12]
	lsls	r3, r3, #15
	adds	r2, r2, r3
	str	r2, [r5, #4]
	ldr	r3, [r0, #16]
	movs	r0, #142
	str	r3, [r5, #8]
	lsls	r0, r0, #1
	bl	sub_08096c80
	adds	r6, r0, #0
	cmp	r6, #0
	beq.n	.L_08096348
	bl	sub_08004458
	movs	r1, #3
	bl	sub_080022f4
	movs	r2, #128
	lsls	r2, r2, #9
	adds	r0, r0, r2
	adds	r2, r6, #0
	adds	r2, #100
	movs	r3, #100
	str	r0, [r6, #28]
	str	r0, [r6, #24]
	movs	r1, #24
	strh	r3, [r2, #0]
	lsls	r0, r7, #16
	bl	sub_080022ec
	adds	r3, r6, #0
	adds	r3, #102
	strh	r0, [r3, #0]
	ldr	r3, [pc, #220]
	str	r3, [r6, #108]
	adds	r3, r6, #0
	adds	r3, #85
	mov	r0, r9
	strb	r0, [r3, #0]
	movs	r1, #7
	adds	r0, r6, #0
	bl	sub_08009080
	adds	r0, r6, #0
	movs	r1, #11
	bl	sub_08009240
.L_08096348:
	adds	r7, #1
	cmp	r7, #23
	ble.n	.L_080962e0
	movs	r0, #100
	bl	sub_080030f8
	movs	r0, #144
	lsls	r0, r0, #1
	bl	sub_080f9010
	movs	r0, #1
	bl	sub_080030f8
	movs	r0, #151
	bl	sub_080f9010
	mov	r2, sl
	ldr	r3, [r2, #8]
	mov	r0, r8
	str	r3, [r0, #0]
	ldr	r3, [r2, #12]
	movs	r2, #144
	lsls	r2, r2, #13
	adds	r3, r3, r2
	str	r3, [r0, #4]
	mov	r0, sl
	ldr	r3, [r0, #16]
	mov	r2, r8
	movs	r7, #0
	mov	r5, r8
	str	r3, [r2, #8]
	b.n	.L_080963d4
.L_08096388:
	ldr	r3, [pc, #132]
	adds	r2, r6, #0
	adds	r2, #85
	str	r3, [r6, #28]
	str	r3, [r6, #24]
	movs	r3, #2
	strb	r3, [r2, #0]
	movs	r3, #160
	lsls	r3, r3, #11
	str	r3, [r6, #40]
	ldr	r3, [r6, #12]
	str	r3, [r6, #20]
	bl	sub_08004458
	ldr	r3, [pc, #108]
	adds	r0, r0, r3
	str	r0, [r6, #48]
	bl	sub_08004458
	movs	r1, #128
	adds	r2, r0, #0
	lsls	r1, r1, #14
	adds	r0, r6, #0
	bl	sub_08096bec
	adds	r0, r6, #0
	movs	r1, #11
	bl	sub_08009240
	adds	r2, r6, #0
	adds	r2, #94
	movs	r3, #8
	strh	r3, [r2, #0]
	adds	r0, r6, #0
	ldr	r1, [pc, #72]
	bl	sub_08009098
	adds	r7, #1
.L_080963d4:
	cmp	r7, #7
	bgt.n	.L_080963ec
	movs	r0, #142
	ldr	r1, [r5, #0]
	ldr	r2, [r5, #4]
	ldr	r3, [r5, #8]
	lsls	r0, r0, #1
	bl	sub_08096c80
	adds	r6, r0, #0
	cmp	r6, #0
	bne.n	.L_08096388
.L_080963ec:
	movs	r0, #15
	bl	sub_080030f8
	bl	sub_080b0050
	bl	sub_080958e4
.L_080963fa:
	add	sp, #12
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x08095fcd
	.4byte 0x00009999
	.4byte 0x00016666
	.4byte 0x0809f0b0
