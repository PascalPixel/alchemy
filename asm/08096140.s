@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
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
	bl	Func_08092054
	mov	sl, r0
	adds	r0, r5, #0
	bl	Func_08092054
	adds	r6, r0, #0
	cmp	r6, #0
	bne.n	.L0
	b.n	.L1
.L0:
	bl	Func_080958a8
	ldr	r3, [pc, #336]
	ldr	r3, [r3, #0]
	ldr	r0, [pc, #336]
	mov	r9, r3
	bl	Func_080b0048
	movs	r0, #30
	bl	Func_080030f8
	adds	r2, r6, #0
	movs	r3, #0
	adds	r2, #91
	strb	r3, [r2, #0]
	movs	r0, #152
	bl	Func_080f9010
	adds	r0, r5, #0
	movs	r1, #4
	movs	r2, #15
	bl	Func_08092560
	movs	r0, #152
	bl	Func_080f9010
	movs	r1, #4
	movs	r2, #15
	adds	r0, r5, #0
	bl	Func_08092560
	movs	r0, #30
	bl	Func_080030f8
	ldr	r3, [pc, #280]
	movs	r0, #153
	str	r3, [r6, #108]
	bl	Func_080f9010
	adds	r0, r5, #0
	movs	r1, #8
	movs	r2, #22
	bl	Func_08092560
	movs	r0, #140
	bl	Func_080f9010
	movs	r5, #128
	ldr	r1, [pc, #256]
	lsls	r5, r5, #9
	adds	r2, r5, #0
	adds	r0, r1, #0
	bl	Func_080091f0
	ldr	r3, [pc, #248]
	adds	r0, r6, #0
	str	r3, [r6, #108]
	movs	r1, #3
	bl	Func_08009080
	movs	r0, #90
	bl	Func_080030f8
	movs	r1, #128
	movs	r2, #0
	lsls	r1, r1, #7
	ldr	r0, [r7, #0]
	bl	Func_08092adc
	movs	r0, #20
	bl	Func_080030f8
	ldr	r0, [r7, #0]
	bl	Func_08092054
	movs	r1, #28
	bl	Func_08009080
	movs	r0, #30
	bl	Func_080030f8
	ldr	r1, [pc, #200]
	adds	r2, r5, #0
	adds	r0, r1, #0
	bl	Func_080091f0
	ldr	r3, [r6, #8]
	str	r3, [sp, #0]
	ldr	r3, [r6, #12]
	str	r3, [sp, #4]
	mov	r8, sp
	ldr	r3, [r6, #16]
	mov	r0, r8
	str	r3, [sp, #8]
	bl	Func_080974d8
	mov	r5, r9
	adds	r5, #88
	movs	r7, #23
	mov	r6, r8
.L2:
	movs	r1, #142
	ldr	r2, [r6, #0]
	ldr	r3, [r6, #8]
	adds	r0, r5, #0
	lsls	r1, r1, #1
	bl	Func_0809ba90
	adds	r0, r5, #0
	ldr	r1, [pc, #148]
	bl	Func_0809ba7c
	adds	r0, r5, #0
	movs	r1, #7
	bl	Func_0809ba70
	ldr	r0, [r5, #0]
	movs	r1, #11
	bl	Func_08009248
	movs	r3, #128
	lsls	r3, r3, #8
	str	r3, [r5, #40]
	bl	Func_08004458
	movs	r2, #192
	lsls	r2, r2, #9
	adds	r0, r0, r2
	str	r0, [r5, #44]
	subs	r7, #1
	movs	r0, #1
	bl	Func_080030f8
	adds	r5, #72
	cmp	r7, #0
	bge.n	.L2
	movs	r0, #140
	bl	Func_080030f8
	mov	r2, r9
	movs	r1, #2
	adds	r2, #152
	movs	r7, #23
.L4:
	movs	r3, #5
	ldrsb	r3, [r2, r3]
	cmp	r3, #0
	beq.n	.L3
	strb	r1, [r2, #0]
.L3:
	subs	r7, #1
	adds	r2, #72
	cmp	r7, #0
	bge.n	.L4
	movs	r0, #20
	bl	Func_080030f8
	movs	r0, #1
	movs	r1, #1
	movs	r2, #1
	bl	Func_080091f0
	movs	r0, #30
	bl	Func_080030f8
	ldr	r3, [pc, #8]
	movs	r7, #0
	mov	r5, r8
	mov	r9, r3
	b.n	.L5
	movs	r0, r0
	.4byte 0x00000000
	.4byte 0x02000240
	.4byte 0x03001f30
	.4byte 0x00201204
	.4byte 0x0809592d
	.4byte 0x00014ccc
	.4byte 0x08095f9d
	.4byte 0x00019999
	.4byte 0x08096049
.L5:
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
	bl	Func_08096c80
	adds	r6, r0, #0
	cmp	r6, #0
	beq.n	.L6
	bl	Func_08004458
	movs	r1, #3
	bl	Func_080022f4
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
	bl	Func_080022ec
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
	bl	Func_08009080
	adds	r0, r6, #0
	movs	r1, #11
	bl	Func_08009240
.L6:
	adds	r7, #1
	cmp	r7, #23
	ble.n	.L5
	movs	r0, #100
	bl	Func_080030f8
	movs	r0, #144
	lsls	r0, r0, #1
	bl	Func_080f9010
	movs	r0, #1
	bl	Func_080030f8
	movs	r0, #151
	bl	Func_080f9010
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
	b.n	.L7
.L9:
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
	bl	Func_08004458
	ldr	r3, [pc, #108]
	adds	r0, r0, r3
	str	r0, [r6, #48]
	bl	Func_08004458
	movs	r1, #128
	adds	r2, r0, #0
	lsls	r1, r1, #14
	adds	r0, r6, #0
	bl	Func_08096bec
	adds	r0, r6, #0
	movs	r1, #11
	bl	Func_08009240
	adds	r2, r6, #0
	adds	r2, #94
	movs	r3, #8
	strh	r3, [r2, #0]
	adds	r0, r6, #0
	ldr	r1, [pc, #72]
	bl	Func_08009098
	adds	r7, #1
.L7:
	cmp	r7, #7
	bgt.n	.L8
	movs	r0, #142
	ldr	r1, [r5, #0]
	ldr	r2, [r5, #4]
	ldr	r3, [r5, #8]
	lsls	r0, r0, #1
	bl	Func_08096c80
	adds	r6, r0, #0
	cmp	r6, #0
	bne.n	.L9
.L8:
	movs	r0, #15
	bl	Func_080030f8
	bl	Func_080b0050
	bl	Func_080958e4
.L1:
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
