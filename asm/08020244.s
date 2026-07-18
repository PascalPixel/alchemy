@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08020244
	.thumb_func
Func_08020244:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	mov	r9, r1
	movs	r1, #167
	mov	r8, r0
	lsls	r1, r1, #4
	movs	r0, #55
	sub	sp, #40
	bl	Func_080048b0
	ldr	r3, [pc, #792]
	movs	r1, #0
	movs	r2, #1
	ldr	r7, [r3, #0]
	str	r1, [sp, #32]
	str	r1, [sp, #28]
	str	r1, [sp, #24]
	str	r2, [sp, #12]
	subs	r3, #144
	ldr	r3, [r3, #0]
	adds	r5, r0, #0
	str	r3, [sp, #8]
	bl	Func_08077300
	mov	r3, r8
	str	r0, [sp, #4]
	cmp	r3, #0
	bge.n	.L0
	movs	r1, #0
	mov	r8, r1
.L0:
	mov	r2, r9
	cmp	r2, #1
	bne.n	.L1
	mov	r3, r8
	ldr	r1, [pc, #748]
	lsls	r2, r3, #6
	adds	r3, r2, r1
	ldrb	r3, [r7, r3]
	movs	r6, #0
	b.n	.L2
.L5:
	movs	r2, #1
	add	r8, r2
	mov	r3, r8
	cmp	r3, #3
	bne.n	.L3
	movs	r1, #0
	mov	r8, r1
.L3:
	adds	r6, #1
	cmp	r6, #2
	bgt.n	.L4
	mov	r3, r8
	ldr	r1, [pc, #712]
	lsls	r2, r3, #6
	adds	r3, r2, r1
	ldrb	r3, [r7, r3]
.L2:
	cmp	r3, #0
	beq.n	.L5
	adds	r1, #20
	adds	r3, r2, r1
	adds	r3, r7, r3
	ldrb	r3, [r3, #1]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	bne.n	.L5
	b.n	.L4
.L1:
	mov	r2, r9
	cmp	r2, #4
	bne.n	.L6
	mov	r3, r8
	ldr	r1, [pc, #676]
	lsls	r2, r3, #6
	adds	r3, r2, r1
	ldrb	r3, [r7, r3]
	movs	r6, #0
	b.n	.L7
.L9:
	movs	r2, #1
	add	r8, r2
	mov	r3, r8
	cmp	r3, #3
	bne.n	.L8
	movs	r1, #0
	mov	r8, r1
.L8:
	adds	r6, #1
	cmp	r6, #2
	bgt.n	.L4
	mov	r3, r8
	ldr	r1, [pc, #640]
	lsls	r2, r3, #6
	adds	r3, r2, r1
	ldrb	r3, [r7, r3]
.L7:
	cmp	r3, #0
	beq.n	.L9
	adds	r1, #20
	adds	r3, r2, r1
	adds	r3, r7, r3
	ldrb	r3, [r3, #2]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	beq.n	.L9
	b.n	.L4
.L6:
	mov	r2, r9
	cmp	r2, #5
	bne.n	.L10
	mov	r3, r8
	ldr	r1, [pc, #604]
	lsls	r2, r3, #6
	adds	r3, r2, r1
	ldrb	r3, [r7, r3]
	movs	r6, #0
	b.n	.L11
.L13:
	movs	r2, #1
	add	r8, r2
	mov	r3, r8
	cmp	r3, #3
	bne.n	.L12
	movs	r1, #0
	mov	r8, r1
.L12:
	adds	r6, #1
	cmp	r6, #2
	bgt.n	.L4
	mov	r3, r8
	ldr	r1, [pc, #568]
	lsls	r2, r3, #6
	adds	r3, r2, r1
	ldrb	r3, [r7, r3]
.L11:
	cmp	r3, #0
	beq.n	.L13
	adds	r1, #20
	adds	r3, r2, r1
	adds	r3, r7, r3
	ldrb	r3, [r3, #1]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	beq.n	.L13
	b.n	.L4
.L10:
	mov	r2, r9
	cmp	r2, #0
	beq.n	.L14
	mov	r3, r8
	ldr	r1, [pc, #532]
	lsls	r2, r3, #6
	adds	r3, r2, r1
	ldrb	r3, [r7, r3]
	movs	r6, #0
	cmp	r3, #0
	bne.n	.L4
	adds	r3, r2, r7
	adds	r2, r3, r1
.L16:
	movs	r3, #1
	add	r8, r3
	mov	r3, r8
	adds	r2, #64
	cmp	r3, #3
	bne.n	.L15
	movs	r3, #0
	adds	r2, r7, r1
	mov	r8, r3
.L15:
	adds	r6, #1
	cmp	r6, #2
	bgt.n	.L4
	ldrb	r3, [r2, #0]
	cmp	r3, #0
	beq.n	.L16
.L4:
	cmp	r6, #3
	bne.n	.L14
	movs	r5, #2
	negs	r5, r5
	b.n	.L17
.L49:
	movs	r0, #113
	movs	r5, #1
	bl	Func_080f9010
	negs	r5, r5
	b.n	.L18
.L14:
	add	r0, sp, #36
	movs	r3, #0
	str	r3, [r0, #0]
	adds	r1, r5, #0
	ldr	r3, [pc, #456]
	ldr	r2, [pc, #460]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	bl	Func_0801fd84
	movs	r3, #2
	str	r3, [sp, #0]
	movs	r1, #2
	movs	r2, #28
	movs	r3, #7
	movs	r0, #1
	bl	Func_080162d4
	movs	r1, #130
	lsls	r1, r1, #5
	adds	r1, r7, r1
	ldr	r3, [pc, #420]
	movs	r2, #0
	str	r1, [sp, #20]
	mov	sl, r0
	mov	fp, r2
	movs	r6, #2
	adds	r5, r7, r3
.L25:
	ldrb	r3, [r5, #0]
	cmp	r3, #0
	bne.n	.L19
	ldr	r0, [pc, #412]
	b.n	.L20
.L19:
	ldrh	r3, [r5, #26]
	ldr	r1, [sp, #4]
	cmp	r3, r1
	bcs.n	.L21
	ldr	r0, [pc, #404]
	b.n	.L20
.L21:
	ldr	r2, [r5, #4]
	ldr	r3, [r5, #28]
	cmp	r2, r3
	beq.n	.L22
	ldr	r0, [pc, #396]
	b.n	.L20
.L22:
	mov	r2, r9
	cmp	r2, #5
	bne.n	.L23
	movs	r3, #21
	ldrsb	r3, [r5, r3]
	cmp	r3, #0
	bne.n	.L23
	ldr	r0, [pc, #380]
.L20:
	mov	r1, sl
	movs	r2, #10
	mov	r3, fp
	bl	Func_0801e74c
	b.n	.L24
.L23:
	ldr	r0, [sp, #20]
	mov	r1, sl
	adds	r0, #16
	movs	r2, #12
	mov	r3, fp
	bl	Func_0801e858
	ldr	r3, [pc, #356]
	ldrh	r0, [r5, #2]
	mov	r1, sl
	adds	r0, r0, r3
	movs	r2, #62
	mov	r3, fp
	bl	Func_0801e74c
	ldr	r1, [pc, #344]
	ldr	r3, [sp, #8]
	adds	r2, r3, r1
	movs	r3, #1
	strb	r3, [r2, #0]
.L24:
	ldr	r3, [sp, #20]
	movs	r2, #16
	adds	r3, #64
	subs	r6, #1
	add	fp, r2
	adds	r5, #64
	str	r3, [sp, #20]
	cmp	r6, #0
	bge.n	.L25
	movs	r3, #2
	str	r3, [sp, #0]
	mov	r0, sl
	movs	r1, #0
	movs	r2, #2
	movs	r3, #27
	bl	Func_0801e41c
	movs	r3, #4
	str	r3, [sp, #0]
	mov	r0, sl
	movs	r1, #0
	movs	r2, #4
	movs	r3, #27
	bl	Func_0801e41c
	movs	r3, #24
	mov	r1, sl
	negs	r3, r3
	mov	r0, r9
	movs	r2, #72
	bl	Func_08021620
	movs	r1, #2
	str	r0, [sp, #16]
	mov	fp, r1
.L40:
	ldr	r2, [sp, #12]
	cmp	r2, #0
	bne.n	.L26
	b.n	.L27
.L26:
	movs	r3, #0
	mov	r1, r8
	ldr	r2, [pc, #220]
	str	r3, [sp, #12]
	lsls	r5, r1, #6
	adds	r3, r5, r2
	ldrb	r3, [r7, r3]
	cmp	r3, #0
	bne.n	.L28
	b.n	.L29
.L28:
	ldr	r1, [pc, #240]
	adds	r3, r5, r1
	ldrb	r0, [r7, r3]
	adds	r3, r7, r3
	ldrb	r1, [r3, #1]
	bl	Func_0801ccc0
	ldr	r2, [sp, #32]
	cmp	r2, #0
	bne.n	.L30
	mov	r3, fp
	str	r3, [sp, #0]
	movs	r0, #1
	movs	r1, #10
	movs	r2, #14
	movs	r3, #9
	bl	Func_080162d4
	str	r0, [sp, #32]
.L30:
	movs	r1, #130
	adds	r3, r7, r5
	lsls	r1, r1, #5
	adds	r6, r3, r1
	ldr	r0, [sp, #32]
	adds	r1, r6, #0
	bl	Func_08020198
	movs	r0, #1
	bl	Func_080030f8
	ldr	r2, [sp, #28]
	cmp	r2, #0
	bne.n	.L31
	mov	r3, fp
	str	r3, [sp, #0]
	movs	r0, #16
	movs	r1, #10
	movs	r2, #13
	movs	r3, #3
	bl	Func_080162d4
	str	r0, [sp, #28]
.L31:
	bl	Func_0801ff14
	movs	r1, #0
	ldr	r0, [sp, #28]
	movs	r2, #0
	adds	r3, r6, #0
	bl	Func_0801fe2c
	movs	r0, #1
	bl	Func_080030f8
	ldr	r1, [pc, #140]
	adds	r3, r5, r1
	adds	r1, #1
	ldrsb	r2, [r7, r3]
	adds	r3, r5, r1
	ldrsb	r3, [r7, r3]
	adds	r1, #1
	adds	r2, r2, r3
	adds	r3, r5, r1
	ldrsb	r3, [r7, r3]
	adds	r1, #1
	adds	r2, r2, r3
	adds	r3, r5, r1
	ldrsb	r3, [r7, r3]
	cmn	r2, r3
	beq.n	.L32
	ldr	r2, [sp, #24]
	cmp	r2, #0
	bne.n	.L33
	mov	r3, fp
	str	r3, [sp, #0]
	movs	r0, #16
	movs	r1, #14
	movs	r2, #13
	movs	r3, #5
	bl	Func_080162d4
	str	r0, [sp, #24]
.L33:
	ldr	r0, [sp, #24]
	adds	r1, r6, #0
	bl	Func_08020150
	bl	Func_08020088
	movs	r1, #0
	ldr	r0, [sp, #24]
	movs	r2, #0
	bl	Func_0801ffd8
	b.n	.L34
.L32:
	bl	Func_08020088
	movs	r1, #2
	ldr	r0, [sp, #24]
	bl	Func_08016418
	movs	r1, #0
	str	r1, [sp, #24]
	b.n	.L34
	.4byte 0x03001f1c
	.4byte 0x0000105c
	.4byte 0x040000d4
	.4byte 0x8500029c
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00000002
	.4byte 0x0000099b
	.4byte 0x00000ea3
	.4byte 0x00001074
	.4byte 0x00001068
.L29:
	ldr	r2, [pc, #504]
	ldr	r1, [pc, #508]
	adds	r3, r2, r1
	ldrb	r0, [r3, #0]
	ldr	r3, [pc, #504]
	adds	r2, r2, r3
	ldrb	r1, [r2, #0]
	bl	Func_0801ccc0
	bl	Func_08020088
	bl	Func_0801ff14
	movs	r1, #2
	ldr	r0, [sp, #24]
	bl	Func_08016418
	movs	r1, #2
	ldr	r0, [sp, #28]
	bl	Func_08016418
	movs	r1, #2
	ldr	r0, [sp, #32]
	bl	Func_08016418
	movs	r1, #0
	str	r1, [sp, #24]
	str	r1, [sp, #28]
	str	r1, [sp, #32]
.L34:
	mov	r0, sl
	bl	Func_08016498
	mov	r2, fp
	str	r2, [sp, #0]
	mov	r0, sl
	movs	r1, #0
	movs	r2, #2
	movs	r3, #27
	bl	Func_0801e41c
	movs	r3, #4
	str	r3, [sp, #0]
	mov	r0, sl
	movs	r1, #0
	movs	r2, #4
	movs	r3, #27
	bl	Func_0801e41c
	mov	r3, r8
	lsls	r2, r3, #1
	movs	r3, #1
	str	r3, [sp, #0]
	mov	r0, sl
	movs	r1, #0
	movs	r3, #26
	bl	Func_0801fda8
.L27:
	ldr	r0, [sp, #16]
	bl	Func_080216b4
	movs	r0, #1
	bl	Func_080030f8
	ldr	r1, [pc, #392]
	ldr	r2, [r1, #0]
	movs	r3, #64
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L35
	movs	r0, #111
	bl	Func_080f9010
	movs	r1, #1
	str	r1, [sp, #12]
	b.n	.L36
.L41:
	mov	r3, r8
	ldr	r1, [pc, #368]
	lsls	r2, r3, #6
	adds	r3, r2, r1
	ldrb	r3, [r7, r3]
	cmp	r3, #0
	beq.n	.L36
	mov	r3, r9
	cmp	r3, #1
	bne.n	.L37
	adds	r1, #20
	adds	r3, r2, r1
	adds	r3, r7, r3
	ldrb	r3, [r3, #1]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	bne.n	.L36
.L37:
	mov	r3, r9
	cmp	r3, #4
	bne.n	.L38
	ldr	r1, [pc, #332]
	adds	r3, r2, r1
	adds	r3, r7, r3
	ldrb	r3, [r3, #2]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	beq.n	.L36
.L38:
	mov	r3, r9
	cmp	r3, #5
	beq.n	.L39
	b.n	.L40
.L39:
	ldr	r1, [pc, #308]
	adds	r3, r2, r1
	adds	r3, r7, r3
	ldrb	r3, [r3, #1]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	beq.n	.L36
	b.n	.L40
.L36:
	mov	r0, r8
	adds	r0, #2
	movs	r1, #3
	bl	Func_080022fc
	mov	r2, r9
	mov	r8, r0
	cmp	r2, #0
	bne.n	.L41
	b.n	.L40
.L35:
	ldr	r2, [r1, #0]
	movs	r3, #128
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L42
	movs	r0, #111
	bl	Func_080f9010
	movs	r3, #1
	str	r3, [sp, #12]
	b.n	.L43
.L47:
	mov	r1, r8
	lsls	r2, r1, #6
	ldr	r1, [pc, #240]
	adds	r3, r2, r1
	ldrb	r3, [r7, r3]
	cmp	r3, #0
	beq.n	.L43
	mov	r3, r9
	cmp	r3, #1
	bne.n	.L44
	adds	r1, #20
	adds	r3, r2, r1
	adds	r3, r7, r3
	ldrb	r3, [r3, #1]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	bne.n	.L43
.L44:
	mov	r3, r9
	cmp	r3, #4
	bne.n	.L45
	ldr	r1, [pc, #208]
	adds	r3, r2, r1
	adds	r3, r7, r3
	ldrb	r3, [r3, #2]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	beq.n	.L43
.L45:
	mov	r3, r9
	cmp	r3, #5
	beq.n	.L46
	b.n	.L40
.L46:
	ldr	r1, [pc, #184]
	adds	r3, r2, r1
	adds	r3, r7, r3
	ldrb	r3, [r3, #1]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	beq.n	.L43
	b.n	.L40
.L43:
	mov	r0, r8
	adds	r0, #4
	movs	r1, #3
	bl	Func_080022fc
	mov	r2, r9
	mov	r8, r0
	cmp	r2, #0
	bne.n	.L47
	b.n	.L40
.L42:
	ldr	r2, [pc, #148]
	ldr	r3, [r2, #0]
	mov	r1, fp
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L48
	b.n	.L49
.L48:
	ldr	r3, [r2, #0]
	movs	r2, #1
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L50
	b.n	.L40
.L50:
	movs	r0, #112
	bl	Func_080f9010
	mov	r5, r8
.L18:
	bl	Func_08020088
	bl	Func_0801ff14
	movs	r1, #2
	ldr	r0, [sp, #24]
	bl	Func_08016418
	movs	r1, #2
	ldr	r0, [sp, #28]
	bl	Func_08016418
	movs	r1, #2
	ldr	r0, [sp, #32]
	bl	Func_08016418
	movs	r1, #2
	mov	r0, sl
	bl	Func_08016418
	bl	Func_0801fd98
	movs	r0, #55
	bl	Func_08002dd8
	ldr	r3, [pc, #40]
	ldr	r1, [pc, #44]
	adds	r2, r3, r1
	ldrb	r0, [r2, #0]
	ldr	r2, [pc, #40]
	adds	r3, r3, r2
	ldrb	r1, [r3, #0]
	bl	Func_0801ccc0
	movs	r0, #1
	bl	Func_080030f8
.L17:
	adds	r0, r5, #0
	add	sp, #40
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x02000240
	.4byte 0x00000205
	.4byte 0x00000206
	.4byte 0x03001b04
	.4byte 0x0000105c
	.4byte 0x00001070
	.4byte 0x03001c94
