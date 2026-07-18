@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080f03f0
	.thumb_func
Func_080f03f0:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #248]
	movs	r2, #0
	strb	r2, [r3, #0]
	ldr	r3, [pc, #244]
	strb	r2, [r3, #0]
	ldr	r3, [pc, #244]
	strb	r2, [r3, #0]
	ldr	r3, [pc, #244]
	strb	r2, [r3, #0]
	bl	Func_080040e8
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #236]
	bl	Func_080041d8
	movs	r3, #64
	movs	r5, #128
	lsls	r5, r5, #19
	strh	r3, [r5, #0]
	ldr	r0, [pc, #228]
	bl	Func_080f037c
	ldr	r0, [pc, #224]
	bl	Func_080f037c
	movs	r0, #0
	bl	Func_080f0254
	movs	r0, #1
	bl	Func_080f0254
	ldr	r2, [pc, #212]
	ldr	r3, [pc, #212]
	strh	r2, [r3, #0]
	ldr	r2, [pc, #212]
	adds	r3, #2
	strh	r2, [r3, #0]
	movs	r3, #226
	lsls	r3, r3, #5
	strh	r3, [r5, #0]
	ldr	r2, [pc, #204]
	ldr	r3, [pc, #208]
	strh	r2, [r3, #0]
	bl	Func_080f0678
	movs	r0, #150
	lsls	r0, r0, #1
	bl	Func_080030f8
	movs	r1, #0
	mov	r8, r1
	ldr	r3, [pc, #192]
	ldr	r1, [pc, #192]
	movs	r2, #1
	mov	fp, r2
	mov	sl, r3
	mov	r9, r1
.L3:
	mov	r3, fp
	mov	r5, r8
	ands	r5, r3
	mov	r2, r9
	adds	r1, r5, #0
	ldr	r0, [r2, #0]
	eors	r1, r3
	bl	Func_080f02b0
	adds	r7, r5, #0
	movs	r5, #240
	movs	r6, #1
	lsls	r5, r5, #4
.L2:
	cmp	r7, #0
	beq.n	.L0
	movs	r1, #16
	subs	r2, r1, r6
	lsls	r3, r6, #8
	orrs	r3, r2
	mov	r2, sl
	strh	r3, [r2, #0]
	b.n	.L1
.L0:
	adds	r3, r5, #0
	orrs	r3, r6
	mov	r1, sl
	strh	r3, [r1, #0]
.L1:
	movs	r0, #4
	bl	Func_080030f8
	ldr	r2, [pc, #128]
	adds	r6, #1
	adds	r5, r5, r2
	cmp	r6, #16
	ble.n	.L2
	ldr	r0, [pc, #124]
	bl	Func_080030f8
	movs	r1, #1
	add	r8, r1
	movs	r3, #4
	mov	r2, r8
	add	r9, r3
	cmp	r2, #32
	bls.n	.L3
	ldr	r3, [pc, #88]
	movs	r2, #0
	strh	r2, [r3, #0]
	movs	r2, #130
	lsls	r2, r2, #5
	subs	r3, #80
	strh	r2, [r3, #0]
	bl	Func_0800479c
	bl	Func_08004760
	ldr	r2, [pc, #20]
	movs	r3, #1
	strb	r3, [r2, #0]
	movs	r0, #0
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x03001d18
	.4byte 0x03001f58
	.4byte 0x03001ac4
	.4byte 0x03001d08
	.4byte 0x080f03c1
	.4byte 0x06007800
	.4byte 0x0600f800
	.4byte 0x00001f8a
	.4byte 0x0400000c
	.4byte 0x00000f83
	.4byte 0x00002844
	.4byte 0x04000050
	.4byte 0x04000052
	.4byte 0x080f0a5c
	.4byte 0xffffff00
	.4byte 0x0000010b
