@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080ccc38
	.thumb_func
Func_080ccc38:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r6, [pc, #56]
	adds	r3, r6, #0
	ldmia	r3!, {r7}
	ldr	r2, [pc, #52]
	ldr	r3, [r3, #0]
	adds	r5, r7, r2
	str	r0, [r5, #0]
	movs	r0, #0
	mov	r9, r3
	sub	sp, #32
	mov	sl, r1
	bl	Func_080cd594
	ldr	r3, [pc, #24]
	ldr	r2, [pc, #36]
	strh	r3, [r2, #0]
	ldr	r3, [r5, #0]
	ldr	r3, [r3, #4]
	cmp	r3, #1
	bne.n	.L0
	str	r3, [sp, #0]
	movs	r0, #46
	movs	r1, #7
	movs	r2, #7
	movs	r3, #3
	b.n	.L1
	movs	r0, r0
	.4byte 0x00000100
	.4byte 0x03001eec
	.4byte 0x00007828
	.4byte 0x04000020
.L0:
	movs	r3, #1
	str	r3, [sp, #0]
	movs	r0, #46
	movs	r1, #7
	movs	r2, #7
	movs	r3, #7
.L1:
	bl	Func_080ed408
	ldr	r6, [r6, #28]
	str	r6, [sp, #12]
	ldr	r0, [pc, #468]
	adds	r1, r7, #0
	movs	r2, #1
	movs	r3, #1
	bl	Func_080e0524
	movs	r3, #0
	ldr	r0, [pc, #460]
	ldr	r1, [pc, #460]
	movs	r2, #1
	bl	Func_080e0524
	mov	r3, sl
	cmp	r3, #0
	bne.n	.L2
	ldr	r0, [pc, #452]
	bl	Func_08002f40
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #444]
	lsls	r0, r0, #19
	movs	r2, #128
	bl	Func_080072f0
.L2:
	movs	r4, #239
	lsls	r4, r4, #7
	ldr	r0, [pc, #436]
	adds	r2, r7, r4
	movs	r3, #2
	str	r3, [r2, #0]
	movs	r1, #144
	adds	r2, r7, r0
	movs	r3, #75
	str	r3, [r2, #0]
	lsls	r1, r1, #3
	ldr	r0, [pc, #420]
	bl	Func_080041d8
	ldr	r2, [pc, #420]
	adds	r5, r7, r2
	ldr	r3, [r5, #0]
	movs	r4, #36
	ldrsh	r0, [r3, r4]
	bl	Func_080b5098
	ldr	r3, [r5, #0]
	ldr	r0, [r0, #0]
	add	r6, sp, #20
	mov	r8, r0
	adds	r1, r6, #0
	movs	r2, #36
	ldrsh	r0, [r3, r2]
	bl	Func_080e396c
	ldr	r3, [r5, #0]
	ldr	r3, [r3, #4]
	cmp	r3, #0
	bne.n	.L3
	ldr	r2, [r6, #0]
	movs	r3, #16
	b.n	.L4
.L3:
	ldr	r2, [r6, #0]
	movs	r3, #112
.L4:
	ldr	r1, [pc, #372]
	subs	r3, r3, r2
	lsls	r3, r3, #8
	str	r3, [r1, #0]
	movs	r3, #74
	mov	r4, sl
	str	r3, [sp, #8]
	cmp	r4, #1
	beq.n	.L5
	movs	r0, #48
	str	r0, [sp, #8]
.L5:
	ldr	r2, [sp, #8]
	movs	r5, #0
	cmp	r2, #0
	bne.n	.L6
	b.n	.L7
.L6:
	ldr	r3, [pc, #336]
	ldr	r4, [pc, #344]
	adds	r6, r7, r3
	mov	fp, r4
.L16:
	adds	r3, r5, #0
	cmp	r5, #0
	bge.n	.L8
	adds	r3, r5, #3
.L8:
	asrs	r4, r3, #2
	cmp	r4, #5
	bgt.n	.L9
	cmp	r4, #3
	bgt.n	.L10
	lsls	r3, r4, #1
	mov	r0, fp
	ldrh	r1, [r0, r3]
	ldr	r3, [r6, #0]
	ldr	r2, [r3, #4]
	lsls	r3, r2, #1
	adds	r3, r3, r2
	ldr	r0, [pc, #308]
	lsls	r3, r3, #1
	adds	r3, r4, r3
	ldrb	r2, [r0, r3]
	ldr	r0, [pc, #304]
	ldr	r3, [pc, #308]
	ldrb	r0, [r0, r4]
	ldrsb	r3, [r3, r4]
	str	r0, [sp, #0]
	ldr	r0, [pc, #304]
	ldrb	r0, [r0, r4]
	adds	r1, r7, r1
	str	r0, [sp, #4]
	adds	r3, #32
	mov	r0, r9
	ldr	r4, [sp, #12]
	bl	Func_080072f4
	b.n	.L9
.L10:
	lsls	r3, r4, #1
	mov	r0, fp
	ldrh	r1, [r0, r3]
	ldr	r2, [pc, #232]
	ldr	r3, [r6, #0]
	adds	r1, r1, r2
	ldr	r2, [r3, #4]
	lsls	r3, r2, #1
	adds	r3, r3, r2
	ldr	r0, [pc, #252]
	lsls	r3, r3, #1
	adds	r3, r4, r3
	ldrb	r2, [r0, r3]
	ldr	r0, [pc, #248]
	ldr	r3, [pc, #252]
	ldrb	r0, [r0, r4]
	ldrsb	r3, [r3, r4]
	str	r0, [sp, #0]
	ldr	r0, [pc, #248]
	ldrb	r0, [r0, r4]
	adds	r3, #32
	str	r0, [sp, #4]
	ldr	r4, [sp, #12]
	mov	r0, r9
	bl	Func_080072f4
.L9:
	cmp	r5, #8
	bne.n	.L11
	mov	r0, sl
	cmp	r0, #0
	bne.n	.L12
	movs	r0, #133
	bl	Func_080b50e8
	ldr	r3, [r6, #0]
	movs	r1, #1
	movs	r2, #36
	ldrsh	r0, [r3, r2]
	bl	Func_080b5088
	b.n	.L13
.L12:
	movs	r0, #134
	bl	Func_080f9010
	ldr	r3, [r6, #0]
	movs	r4, #36
	ldrsh	r0, [r3, r4]
	movs	r3, #4
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r2, #5
	movs	r3, #0
	bl	Func_080d6888
.L13:
	ldr	r0, [pc, #180]
	movs	r3, #8
	adds	r2, r7, r0
	str	r3, [r2, #0]
.L11:
	mov	r2, sl
	cmp	r2, #1
	bne.n	.L14
	cmp	r5, #13
	bne.n	.L15
	movs	r3, #192
	mov	r4, r8
	lsls	r3, r3, #12
	str	r3, [r4, #40]
	ldr	r3, [pc, #156]
	str	r3, [r4, #72]
	movs	r3, #128
	lsls	r3, r3, #7
	str	r3, [r4, #68]
.L15:
	cmp	r5, #65
	bne.n	.L14
	ldr	r0, [pc, #140]
	movs	r3, #4
	adds	r2, r7, r0
	str	r3, [r2, #0]
	movs	r0, #134
	bl	Func_080b50e8
.L14:
	movs	r0, #8
	movs	r1, #8
	bl	Func_080e155c
	bl	Func_080cd52c
	ldr	r3, [pc, #120]
	adds	r2, r7, r3
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	Func_080030f8
	ldr	r4, [sp, #8]
	adds	r5, #1
	cmp	r5, r4
	beq.n	.L7
	b.n	.L16
.L7:
	ldr	r0, [pc, #56]
	bl	Func_08004278
	movs	r0, #46
	bl	Func_08002dd8
	bl	Func_080cdbc0
	add	sp, #32
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x00000071
	.4byte 0x00000072
	.4byte 0x02010000
	.4byte 0x000000a0
	.4byte 0x03001388
	.4byte 0x00007784
	.4byte 0x080cd261
	.4byte 0x00007828
	.4byte 0x04000028
	.4byte 0x080ee070
	.4byte 0x080ee07c
	.4byte 0x080ee064
	.4byte 0x080ee088
	.4byte 0x080ee06a
	.4byte 0x000077a8
	.4byte 0x00007851
	.4byte 0x00007824
