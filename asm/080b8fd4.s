@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080b8fd4
	.thumb_func
Func_080b8fd4:
	push	{r5, r6, lr}
	mov	r6, sl
	mov	r5, r8
	push	{r5, r6}
	ldr	r3, [pc, #176]
	ldr	r5, [r3, #0]
	adds	r3, #128
	ldr	r3, [r3, #0]
	mov	r8, r3
	movs	r3, #160
	lsls	r3, r3, #11
	str	r3, [r5, #16]
	movs	r3, #128
	mov	r2, r8
	movs	r6, #0
	lsls	r3, r3, #7
	str	r6, [r5, #12]
	str	r6, [r5, #20]
	str	r3, [r2, #0]
	strh	r3, [r5, #54]
	movs	r3, #244
	lsls	r3, r3, #8
	strh	r3, [r5, #52]
	ldr	r3, [pc, #144]
	str	r6, [r5, #28]
	str	r3, [r5, #32]
	str	r6, [r5, #24]
	sub	sp, #16
	mov	sl, r0
	bl	Func_080049ac
	adds	r0, r5, #0
	adds	r0, #12
	bl	Func_08004cb4
	movs	r3, #54
	ldrsh	r0, [r5, r3]
	bl	Func_08004c1c
	movs	r2, #52
	ldrsh	r0, [r5, r2]
	bl	Func_08004bd4
	add	r0, sp, #4
	str	r6, [r0, #0]
	str	r6, [r0, #4]
	ldr	r3, [r5, #32]
	adds	r1, r5, #0
	str	r3, [r0, #8]
	ldr	r3, [pc, #96]
	bl	Func_080072f0
	movs	r1, #192
	ldr	r3, [pc, #92]
	lsls	r1, r1, #8
	ldr	r0, [pc, #92]
	bl	Func_080072f0
	ldr	r2, [pc, #88]
	adds	r1, r0, #0
	movs	r0, #0
	bl	Func_08005258
	ldr	r2, [pc, #84]
	mov	r3, sl
	adds	r3, #120
	str	r3, [r2, #16]
	movs	r1, #118
	mov	r2, sl
	movs	r5, #1
	mov	r3, r8
	subs	r1, r1, r2
	movs	r2, #128
	str	r5, [r3, #16]
	lsls	r2, r2, #10
	movs	r0, #240
	movs	r3, #128
	str	r2, [sp, #0]
	lsls	r0, r0, #15
	lsls	r1, r1, #16
	lsls	r3, r3, #4
	movs	r2, #0
	bl	Func_080c0a24
	mov	r3, r8
	str	r5, [r3, #20]
	str	r6, [r3, #16]
	add	sp, #16
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6}
	pop	{r0}
	bx	r0
	.4byte 0x03001e80
	.4byte 0x02ee0000
	.4byte 0x03000250
	.4byte 0x0300013c
	.4byte 0x03c90000
	.4byte 0x07920000
	.4byte 0x03001ce0
