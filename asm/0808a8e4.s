@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0808a8e4
	.thumb_func
Func_0808a8e4:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #140]
	ldr	r1, [pc, #140]
	ldrb	r3, [r3, #0]
	mov	fp, r1
	cmp	r3, #0
	beq.n	.L0
	cmp	r0, #1
	bne.n	.L1
	ldr	r1, [pc, #132]
	movs	r4, #224
	ldr	r2, [pc, #132]
	lsls	r4, r4, #1
	adds	r3, r1, r4
	strh	r2, [r3, #0]
	movs	r2, #225
	lsls	r2, r2, #1
	adds	r3, r1, r2
	strh	r0, [r3, #0]
	b.n	.L2
.L1:
	cmp	r0, #2
	bne.n	.L0
	ldr	r1, [pc, #104]
	movs	r4, #224
	ldr	r3, [pc, #108]
	lsls	r4, r4, #1
	movs	r0, #225
	adds	r2, r1, r4
	lsls	r0, r0, #1
	strh	r3, [r2, #0]
	adds	r2, r1, r0
	movs	r3, #1
	b.n	.L3
.L0:
	bl	Func_08077098
	ldr	r1, [pc, #80]
	movs	r4, #224
	ldr	r3, [pc, #88]
	lsls	r4, r4, #1
	movs	r0, #225
	adds	r2, r1, r4
	lsls	r0, r0, #1
	strh	r3, [r2, #0]
	adds	r2, r1, r0
	movs	r3, #2
.L3:
	strh	r3, [r2, #0]
.L2:
	ldr	r3, [pc, #56]
	ldr	r1, [pc, #72]
	adds	r2, r3, r1
	ldrb	r0, [r2, #0]
	ldr	r2, [pc, #68]
	adds	r3, r3, r2
	ldrb	r1, [r3, #0]
	bl	Func_08015360
	bl	Func_0800403c
	bl	Func_080040e8
	bl	Func_080040e8
	ldr	r3, [pc, #52]
	mov	r9, r3
.L14:
	ldr	r0, [pc, #52]
	bl	Func_080770c0
	cmp	r0, #0
	beq.n	.L4
	ldr	r0, [pc, #40]
	bl	Func_080770d0
	b.n	.L5
	.4byte 0x03001f54
	.4byte 0x0809f1a8
	.4byte 0x02000240
	.4byte 0x00000005
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000205
	.4byte 0x00000206
	.4byte 0x050001c0
	.4byte 0x00000101
.L4:
	movs	r0, #144
	lsls	r0, r0, #1
	bl	Func_080f9010
.L5:
	ldr	r7, [pc, #92]
	movs	r4, #224
	lsls	r4, r4, #1
	adds	r4, r4, r7
	movs	r0, #0
	ldrsh	r3, [r4, r0]
	movs	r1, #225
	lsls	r3, r3, #3
	add	r3, fp
	lsls	r1, r1, #1
	mov	sl, r3
	adds	r3, r7, r1
	ldr	r1, [pc, #72]
	movs	r2, #0
	ldrsh	r6, [r3, r2]
	ldrh	r3, [r1, #10]
	ldr	r2, [pc, #52]
	ands	r3, r2
	strh	r3, [r1, #10]
	ldr	r2, [pc, #52]
	ldrh	r3, [r1, #10]
	ands	r3, r2
	strh	r3, [r1, #10]
	mov	r8, r4
	ldrh	r3, [r1, #10]
	bl	Func_080040e8
	movs	r0, #1
	movs	r1, #0
	movs	r2, #0
	bl	Func_0800307c
	movs	r0, #2
	movs	r1, #0
	movs	r2, #0
	bl	Func_0800307c
	bl	Func_08004858
	bl	Func_08004760
	bl	Func_0800403c
	b.n	.L6
	.4byte 0x0000c5ff
	.4byte 0x00007fff
	.4byte 0x02000240
	.4byte 0x040000b0
.L6:
	mov	r0, r8
	movs	r1, #253
	movs	r4, #0
	ldrsh	r3, [r0, r4]
	lsls	r1, r1, #1
	cmp	r3, r1
	ble.n	.L7
	movs	r2, #254
	lsls	r2, r2, #1
	cmp	r3, r2
	beq.n	.L8
	cmp	r3, r2
	bgt.n	.L9
	subs	r2, #1
	cmp	r3, r2
	beq.n	.L10
	b.n	.L11
.L9:
	ldr	r4, [pc, #240]
	cmp	r3, r4
	beq.n	.L12
	movs	r0, #255
	lsls	r0, r0, #1
	cmp	r3, r0
	bne.n	.L11
	adds	r0, r6, #0
	bl	Func_080b50a0
	adds	r6, r0, #0
	b.n	.L11
.L12:
	movs	r0, #64
	bl	Func_08004938
	adds	r5, r0, #0
	ldr	r3, [pc, #212]
	mov	r0, r9
	adds	r1, r5, #0
	ldr	r2, [pc, #208]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r0, r6, #0
	bl	Func_080f4000
	b.n	.L13
.L8:
	movs	r0, #64
	bl	Func_08004938
	adds	r5, r0, #0
	ldr	r3, [pc, #184]
	mov	r0, r9
	adds	r1, r5, #0
	ldr	r2, [pc, #180]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r0, r6, #0
	bl	Func_080f6000
.L13:
	ldr	r3, [pc, #164]
	adds	r6, r0, #0
	mov	r1, r9
	adds	r0, r5, #0
	ldr	r2, [pc, #160]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r0, r5, #0
	bl	Func_08002df0
	b.n	.L11
.L10:
	movs	r6, #0
.L11:
	adds	r0, r6, #0
	bl	Func_0808a6e4
	b.n	.L14
.L7:
	ldr	r5, [pc, #140]
	adds	r0, r5, #0
	bl	Func_080770c0
	mov	r3, r8
	adds	r1, r0, #0
	movs	r2, #0
	ldrsh	r0, [r3, r2]
	bl	Func_0808ab74
	bl	Func_0808b090
	adds	r0, r5, #0
	bl	Func_080770c0
	cmp	r0, #0
	bne.n	.L15
	movs	r0, #141
	lsls	r0, r0, #1
	bl	Func_080770c0
	cmp	r0, #0
	bne.n	.L16
	ldr	r0, [pc, #100]
	bl	Func_080770c0
	cmp	r0, #0
	bne.n	.L16
	bl	Func_0808acc4
	b.n	.L17
.L16:
	movs	r0, #141
	lsls	r0, r0, #1
	bl	Func_080770d0
	b.n	.L17
.L15:
	ldr	r4, [pc, #76]
	adds	r3, r7, r4
	movs	r2, #1
	movs	r1, #0
	ldrsh	r0, [r3, r1]
	negs	r2, r2
	cmp	r0, r2
	beq.n	.L18
	bl	Func_080f9010
	b.n	.L17
.L18:
	bl	Func_0808acc4
.L17:
	mov	r4, sl
	ldr	r3, [pc, #52]
	movs	r0, #237
	ldrh	r2, [r4, #4]
	lsls	r0, r0, #1
	adds	r3, r3, r0
	strh	r2, [r3, #0]
	movs	r0, #0
	bl	Func_0808ab48
	adds	r0, r6, #0
	bl	Func_0808c4f8
	bl	Func_0808a5f8
	b.n	.L14
	movs	r0, r0
	.4byte 0x000001fd
	.4byte 0x040000d4
	.4byte 0x84000010
	.4byte 0x00000109
	.4byte 0x0000011b
	.4byte 0x0000021e
	.4byte 0x02000240
