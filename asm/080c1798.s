@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080c1798
	.thumb_func
Func_080c1798:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r7, [pc, #520]
	adds	r6, r3, #0
	adds	r3, r7, #0
	subs	r3, #12
	ldr	r3, [r3, #0]
	sub	sp, #240
	mov	sl, r0
	movs	r0, #1
	str	r3, [sp, #4]
	mov	fp, r1
	adds	r5, r2, #0
	bl	Func_080030f8
	movs	r1, #201
	ldr	r0, [sp, #4]
	lsls	r1, r1, #3
	adds	r3, r0, r1
	movs	r2, #0
	ldrh	r1, [r3, #0]
	movs	r0, #1
	bl	Func_080c0774
	movs	r1, #128
	ldr	r3, [pc, #476]
	lsls	r1, r1, #7
	ldr	r0, [pc, #476]
	bl	Func_080072f0
	movs	r0, #128
	lsls	r0, r0, #19
	ldr	r1, [pc, #472]
	bl	Func_0800387c
	ldr	r0, [pc, #468]
	ldr	r1, [pc, #472]
	bl	Func_0800387c
	ldr	r1, [pc, #468]
	ldr	r0, [pc, #472]
	bl	Func_0800387c
	movs	r0, #1
	bl	Func_080030f8
	movs	r2, #240
	ldr	r3, [pc, #460]
	strh	r2, [r3, #0]
	ldr	r2, [pc, #460]
	adds	r3, #4
	strh	r2, [r3, #0]
	movs	r2, #63
	adds	r3, #4
	strh	r2, [r3, #0]
	movs	r2, #17
	adds	r3, #2
	strh	r2, [r3, #0]
	cmp	r5, #0
	bne.n	.L0
	ldr	r0, [pc, #444]
	ldr	r1, [pc, #444]
	bl	Func_0800387c
	mov	r0, fp
	bl	Func_080c1470
	ldr	r4, [pc, #436]
	ldr	r3, [sp, #4]
	adds	r4, r3, r4
	movs	r2, #0
	str	r4, [sp, #0]
	ldr	r7, [pc, #432]
	mov	r8, r2
	add	r6, sp, #188
	mov	r9, r2
.L3:
	ldr	r3, [pc, #428]
	mov	r0, r8
	adds	r3, #156
	ldr	r5, [r3, #0]
	cmp	r0, #24
	bgt.n	.L1
	movs	r2, #128
	mov	r1, r9
	ldr	r3, [sp, #0]
	lsls	r2, r2, #9
	subs	r2, r2, r1
	str	r2, [r3, #0]
	ldr	r1, [pc, #408]
	ldr	r4, [sp, #4]
	movs	r3, #128
	adds	r0, r4, r1
	ldr	r1, [pc, #404]
	bl	Func_080c1724
.L1:
	adds	r1, r6, #0
	mov	r0, sl
	bl	Func_080b845c
	ldr	r3, [r6, #0]
	movs	r4, #64
	ldr	r2, [pc, #388]
	subs	r3, r4, r3
	adds	r0, r5, r2
	lsls	r3, r3, #8
	str	r3, [r0, #0]
	ldr	r1, [pc, #384]
	ldr	r3, [r6, #4]
	adds	r2, r5, r1
	subs	r3, r4, r3
	ldr	r1, [pc, #380]
	lsls	r3, r3, #8
	str	r3, [r2, #0]
	ldrh	r3, [r7, #0]
	adds	r4, r3, #0
	strh	r7, [r7, #0]
	ldrh	r2, [r1, #0]
	cmp	r2, #31
	bgt.n	.L2
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #2
	adds	r3, r3, r1
	adds	r3, #4
	adds	r2, #1
	stmia	r3!, {r0}
	strh	r2, [r1, #0]
	ldr	r2, [pc, #348]
	stmia	r3!, {r2}
	ldr	r2, [pc, #348]
	str	r2, [r3, #0]
.L2:
	strh	r4, [r7, #0]
	ldr	r3, [pc, #348]
	adds	r2, r5, r3
	movs	r3, #1
	movs	r0, #1
	str	r3, [r2, #0]
	bl	Func_080030f8
	movs	r0, #1
	ldr	r4, [pc, #336]
	add	r8, r0
	mov	r1, r8
	add	r9, r4
	cmp	r1, #44
	ble.n	.L3
	mov	r0, fp
	bl	Func_080c16d0
	b.n	.L4
.L0:
	cmp	r5, #1
	bne.n	.L5
	mov	r0, fp
	bl	Func_080c9038
	ldr	r2, [pc, #288]
	movs	r3, #39
	movs	r4, #64
	ldr	r6, [pc, #260]
	add	r7, sp, #176
	mov	r9, r2
	mov	r8, r3
	mov	fp, r4
.L7:
	ldr	r0, [pc, #292]
	adds	r1, r7, #0
	ldr	r5, [r0, #0]
	mov	r0, sl
	bl	Func_080b845c
	ldr	r3, [r7, #0]
	mov	r4, fp
	ldr	r2, [pc, #248]
	subs	r3, r4, r3
	adds	r1, r5, r2
	lsls	r3, r3, #8
	str	r3, [r1, #0]
	ldr	r3, [r7, #4]
	ldr	r0, [pc, #240]
	subs	r3, r4, r3
	adds	r2, r5, r0
	lsls	r3, r3, #8
	str	r3, [r2, #0]
	ldrh	r3, [r6, #0]
	adds	r0, r3, #0
	strh	r6, [r6, #0]
	mov	r3, r9
	ldrh	r2, [r3, #0]
	cmp	r2, #31
	bgt.n	.L6
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #2
	add	r3, r9
	adds	r3, #4
	adds	r2, #1
	mov	r4, r9
	stmia	r3!, {r1}
	strh	r2, [r4, #0]
	ldr	r2, [pc, #208]
	stmia	r3!, {r2}
	ldr	r2, [pc, #208]
	str	r2, [r3, #0]
.L6:
	strh	r0, [r6, #0]
	ldr	r0, [pc, #204]
	movs	r3, #1
	adds	r2, r5, r0
	str	r3, [r2, #0]
	movs	r0, #1
	bl	Func_080030f8
	movs	r1, #1
	negs	r1, r1
	add	r8, r1
	mov	r2, r8
	cmp	r2, #0
	bge.n	.L7
	bl	Func_080c9040
	b.n	.L4
.L5:
	cmp	r5, #2
	bne.n	.L8
	add	r0, sp, #92
	movs	r3, #0
	str	r3, [r0, #28]
	mov	r3, fp
	mov	r4, sl
	str	r3, [r0, #0]
	mov	r1, sl
	movs	r3, #1
	str	r6, [r0, #24]
	str	r4, [r0, #8]
	strh	r1, [r0, #36]
	str	r4, [r0, #12]
	str	r3, [r0, #20]
	str	r3, [r0, #16]
	bl	Func_080c9020
	b.n	.L4
.L8:
	add	r0, sp, #8
	movs	r3, #0
	str	r3, [r0, #28]
	str	r3, [r0, #24]
	mov	r3, sl
	mov	r2, fp
	str	r3, [r0, #8]
	mov	r4, sl
	str	r3, [r0, #12]
	movs	r3, #1
	str	r2, [r0, #0]
	strh	r4, [r0, #36]
	str	r3, [r0, #20]
	str	r3, [r0, #16]
	bl	Func_080c9030
.L4:
	add	sp, #240
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03001e80
	.4byte 0x03000164
	.4byte 0x06004000
	.4byte 0x00003741
	.4byte 0x0400000c
	.4byte 0x00000784
	.4byte 0x00003f44
	.4byte 0x04000050
	.4byte 0x04000040
	.4byte 0x00001088
	.4byte 0x04000052
	.4byte 0x0000100e
	.4byte 0x00000644
	.4byte 0x04000208
	.4byte 0x03001e50
	.4byte 0x00000544
	.4byte 0x050000c0
	.4byte 0x000013c4
	.4byte 0x000013c8
	.4byte 0x02002090
	.4byte 0x04000028
	.4byte 0x84000002
	.4byte 0x000013cc
	.4byte 0x00000444
	.4byte 0x03001eec
