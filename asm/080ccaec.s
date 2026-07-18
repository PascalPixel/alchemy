@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080ccaec
	.thumb_func
Func_080ccaec:
	push	{r5, r6, lr}
	ldr	r1, [pc, #96]
	adds	r6, r0, #0
	movs	r0, #39
	bl	Func_080048b0
	movs	r1, #128
	adds	r5, r0, #0
	lsls	r1, r1, #7
	movs	r0, #40
	bl	Func_080048b0
	movs	r0, #0
	bl	Func_080cd594
	ldr	r3, [pc, #72]
	adds	r2, r5, r3
	movs	r3, #24
	str	r3, [r2, #0]
	ldr	r2, [pc, #68]
	ldr	r3, [pc, #48]
	strh	r3, [r2, #0]
	ldr	r3, [pc, #48]
	adds	r2, #50
	strh	r3, [r2, #0]
	cmp	r6, #4
	bhi.n	.L0
	ldr	r2, [pc, #56]
	lsls	r3, r6, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	movs	r0, r0
	.4byte 0x080ccb40
	.4byte 0x080ccb44
	.4byte 0x080ccb68
	.4byte 0x080ccb6c
	.4byte 0x080ccb70
	ldr	r0, [pc, #28]
	b.n	.L1
	ldr	r0, [pc, #28]
	b.n	.L1
	.4byte 0x00000100
	.4byte 0x00001010
	.4byte 0x0000782c
	.4byte 0x000077b4
	.4byte 0x04000020
	.4byte 0x080ccb2c
	.4byte 0x000000c8
	.4byte 0x000000cf
	ldr	r0, [pc, #76]
	b.n	.L1
	ldr	r0, [pc, #76]
	b.n	.L1
.L0:
	ldr	r0, [pc, #76]
.L1:
	bl	Func_08002f40
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #72]
	movs	r2, #128
	lsls	r0, r0, #19
	bl	Func_080072f0
	ldr	r3, [pc, #64]
	adds	r2, r5, r3
	movs	r3, #0
	str	r3, [r2, #0]
	movs	r3, #239
	lsls	r3, r3, #7
	adds	r2, r5, r3
	movs	r3, #3
	str	r3, [r2, #0]
	ldr	r3, [pc, #52]
	adds	r2, r5, r3
	ldr	r3, [pc, #52]
	movs	r1, #200
	str	r3, [r2, #0]
	lsls	r1, r1, #4
	ldr	r0, [pc, #48]
	bl	Func_080041d8
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #40]
	bl	Func_080041d8
	pop	{r5, r6}
	pop	{r0}
	bx	r0
	.4byte 0x000000b4
	.4byte 0x000000cb
	.4byte 0x000000be
	.4byte 0x03001388
	.4byte 0x0000778c
	.4byte 0x00007784
	.4byte 0x06060606
	.4byte 0x080cc961
	.4byte 0x080cd261
