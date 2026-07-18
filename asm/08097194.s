@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08097194
	.thumb_func
Func_08097194:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #204]
	ldr	r1, [r3, #0]
	adds	r2, r3, #0
	subs	r2, #116
	subs	r3, #192
	mov	r8, r1
	ldr	r2, [r2, #0]
	ldr	r3, [r3, #0]
	mov	r7, r8
	mov	r6, r8
	mov	fp, r2
	mov	r9, r3
	adds	r7, #157
	adds	r6, #88
	movs	r5, #23
.L1:
	ldrb	r3, [r7, #0]
	lsls	r3, r3, #24
	adds	r7, #72
	cmp	r3, #0
	beq.n	.L0
	adds	r0, r6, #0
	bl	Func_0809bb34
.L0:
	subs	r5, #1
	adds	r6, #72
	cmp	r5, #0
	bge.n	.L1
	ldr	r3, [pc, #152]
	add	r3, fp
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	bne.n	.L2
	ldr	r2, [pc, #144]
	ldr	r7, [pc, #144]
	movs	r6, #0
	mov	sl, r2
.L7:
	ldr	r3, [pc, #144]
	ldr	r3, [r3, #0]
	movs	r1, #0
	movs	r5, #0
.L5:
	cmp	r5, #63
	bgt.n	.L3
	ldr	r2, [r3, #108]
	cmp	r2, sl
	beq.n	.L4
	adds	r5, #1
	adds	r3, #112
	cmp	r2, r7
	bne.n	.L5
.L4:
	movs	r1, #1
.L3:
	cmp	r1, #0
	beq.n	.L6
	movs	r0, #1
	adds	r6, #1
	bl	Func_080030f8
	cmp	r6, #29
	ble.n	.L7
.L6:
	ldr	r2, [pc, #104]
	movs	r3, #0
	add	r2, fp
	strb	r3, [r2, #0]
	ldr	r0, [pc, #100]
	bl	Func_08004278
	mov	r3, r8
	adds	r3, #70
	movs	r1, #0
	ldrsh	r0, [r3, r1]
	bl	Func_08003f3c
	mov	r2, r8
	ldr	r3, [r2, #76]
	mov	r1, r9
	str	r3, [r1, #4]
	ldr	r3, [r2, #80]
	str	r3, [r1, #8]
	ldr	r3, [r2, #84]
	str	r3, [r1, #12]
	movs	r1, #30
	ldrsh	r3, [r2, r1]
	cmp	r3, #8
	beq.n	.L8
	movs	r2, #204
	lsls	r2, r2, #4
	add	r2, fp
	movs	r3, #1
	strh	r3, [r2, #0]
.L8:
	bl	Func_0809202c
	movs	r0, #56
	bl	Func_08002dd8
.L2:
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03001f30
	.4byte 0x00000cc6
	.4byte 0x08096d85
	.4byte 0x08096d2d
	.4byte 0x03001e64
	.4byte 0x00000cc7
	.4byte 0x08096f8d
