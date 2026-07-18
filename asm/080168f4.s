@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080168f4
	.thumb_func
Func_080168f4:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #344]
	ldr	r1, [pc, #344]
	ldr	r3, [r3, #0]
	adds	r6, r0, #0
	mov	r8, r3
	movs	r0, #131
	ldr	r3, [r1, #0]
	ldr	r3, [pc, #336]
	lsls	r0, r0, #2
	adds	r3, r3, r0
	ldrb	r3, [r3, #0]
	ldr	r2, [pc, #332]
	ldrb	r2, [r2, r3]
	sub	sp, #52
	ldr	r3, [pc, #332]
	str	r2, [sp, #32]
	add	r3, r8
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L0
	ldr	r3, [pc, #324]
	ldrh	r3, [r3, #0]
	adds	r2, r3, #0
	cmp	r2, #0
	bge.n	.L1
	movs	r2, #0
.L1:
	cmp	r2, #2
	ble.n	.L2
	movs	r2, #2
.L2:
	lsls	r3, r2, #2
	adds	r3, r3, r2
	adds	r3, #3
	str	r3, [sp, #32]
.L0:
	ldrh	r3, [r6, #28]
	cmp	r3, #0
	beq.n	.L3
	movs	r0, #1
	bl	Func_080167e0
	ldrh	r3, [r6, #28]
	subs	r3, #1
	strh	r3, [r6, #28]
	b.n	.L4
.L3:
	ldr	r3, [r1, #0]
	cmp	r3, #0
	bne.n	.L5
	ldrh	r2, [r6, #34]
	adds	r3, r2, #0
	cmp	r3, #0
	beq.n	.L5
	ldr	r1, [pc, #268]
	adds	r3, r2, r1
	strh	r3, [r6, #34]
	b.n	.L4
.L30:
	movs	r0, #9
	b.n	.L6
.L5:
	ldrh	r3, [r6, #32]
	movs	r7, #0
	cmp	r3, #0
	bne.n	.L7
	ldrh	r3, [r6, #18]
	movs	r2, #235
	lsls	r3, r3, #1
	lsls	r2, r2, #4
	adds	r3, r3, r2
	mov	r4, r8
	ldrh	r7, [r4, r3]
.L7:
	cmp	r7, #30
	bls.n	.L8
	b.n	.L9
.L8:
	ldr	r2, [pc, #232]
	lsls	r3, r7, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	movs	r0, r0
	.4byte 0x08016d58
	.4byte 0x08016a80
	.4byte 0x08016c2c
	.4byte 0x08016a14
	.4byte 0x08016c7c
	.4byte 0x08016c58
	.4byte 0x08016c62
	.4byte 0x08016d18
	.4byte 0x08016c90
	.4byte 0x08016cd4
	.4byte 0x08016cfc
	.4byte 0x08016d64
	.4byte 0x08016d64
	.4byte 0x08016d64
	.4byte 0x08016d64
	.4byte 0x08016d30
	.4byte 0x08016d64
	.4byte 0x08016d64
	.4byte 0x08016d64
	.4byte 0x08016d64
	.4byte 0x08016d64
	.4byte 0x08016d64
	.4byte 0x08016d64
	.4byte 0x08016d64
	.4byte 0x08016d64
	.4byte 0x08016d64
	.4byte 0x08016d64
	.4byte 0x08016d64
	.4byte 0x08016d64
	.4byte 0x08016d64
	.4byte 0x08016d58
	ldrh	r3, [r6, #30]
	strh	r3, [r6, #4]
	ldr	r3, [r6, #0]
	ldrh	r2, [r3, #22]
	movs	r3, #8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L10
	ldrh	r2, [r6, #6]
	ldr	r3, [pc, #84]
	cmp	r2, r3
	bls.n	.L11
	adds	r0, r6, #0
	bl	Func_080167d8
	movs	r0, #1
	str	r0, [sp, #32]
	b.n	.L12
.L11:
	movs	r1, #208
	lsls	r1, r1, #4
	adds	r3, r2, r1
	strh	r3, [r6, #6]
	b.n	.L12
.L10:
	ldrh	r3, [r6, #6]
	movs	r2, #240
	lsls	r2, r2, #4
	adds	r3, r3, r2
	ldrh	r2, [r6, #16]
	strh	r3, [r6, #6]
	adds	r3, r2, #0
	cmp	r3, #2
	bls.n	.L13
	b.n	.L12
.L13:
	adds	r3, r2, #1
	strh	r3, [r6, #16]
	b.n	.L12
	.4byte 0x03001e8c
	.4byte 0x03001ae8
	.4byte 0x02000240
	.4byte 0x0807380b
	.4byte 0x00000ea5
	.4byte 0x03001cd0
	.4byte 0x0000ffff
	.4byte 0x08016998
	.4byte 0x00000cff
	ldr	r3, [pc, #76]
	add	r3, r8
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L14
	movs	r4, #225
	ldrh	r3, [r6, #20]
	lsls	r4, r4, #2
	cmp	r3, r4
	bcs.n	.L14
	ldr	r2, [pc, #60]
	movs	r3, #0
	str	r3, [r2, #0]
.L14:
	ldr	r3, [pc, #60]
	adds	r0, r6, #0
	ldr	r7, [pc, #44]
	strh	r3, [r6, #20]
	bl	Func_080199ec
	cmp	r0, #0
	bne.n	.L15
	ldr	r0, [r6, #0]
	ldrh	r3, [r0, #8]
	cmp	r3, #0
	bne.n	.L16
	b.n	.L12
.L16:
	ldrh	r3, [r0, #10]
	cmp	r3, #0
	bne.n	.L17
	b.n	.L12
.L17:
	ldr	r7, [pc, #28]
	add	r7, r8
	ldrb	r3, [r7, #0]
	cmp	r3, #0
	beq.n	.L18
	b.n	.L12
.L18:
	b.n	.L19
	movs	r0, r0
	.4byte 0x00000000
	.4byte 0x00000ea4
	.4byte 0x03001af8
	.4byte 0x00000397
	.4byte 0x000012f8
.L19:
	ldrh	r2, [r0, #8]
	ldrh	r3, [r0, #10]
	lsls	r2, r2, #2
	lsls	r3, r3, #3
	movs	r5, #1
	subs	r2, #8
	subs	r3, #16
	movs	r1, #1
	str	r5, [sp, #0]
	bl	Func_08018cac
	strb	r5, [r7, #0]
	b.n	.L12
.L15:
	ldr	r5, [r6, #0]
	ldrh	r3, [r5, #12]
	ldrh	r0, [r5, #8]
	str	r3, [sp, #48]
	ldrh	r3, [r5, #14]
	str	r0, [sp, #28]
	str	r3, [sp, #44]
	ldr	r3, [pc, #428]
	ldrh	r1, [r5, #10]
	ldrh	r4, [r6, #18]
	add	r3, r8
	str	r1, [sp, #24]
	adds	r0, r5, #0
	strb	r7, [r3, #0]
	str	r4, [sp, #12]
	bl	Func_08016478
	ldrh	r3, [r6, #36]
	ldr	r4, [sp, #12]
	cmp	r3, #0
	bne.n	.L20
	ldrh	r2, [r5, #8]
	ldrh	r3, [r5, #10]
	orrs	r3, r2
	cmp	r3, #0
	beq.n	.L20
	ldrh	r0, [r5, #12]
	ldrh	r1, [r5, #14]
	ldrh	r2, [r5, #8]
	ldrh	r3, [r5, #10]
	bl	Func_08016178
	ldr	r4, [sp, #12]
.L20:
	ldr	r3, [pc, #380]
	adds	r4, #1
	ands	r4, r3
	movs	r2, #235
	lsls	r3, r4, #1
	lsls	r2, r2, #4
	adds	r3, r3, r2
	mov	r0, r8
	ldrh	r3, [r0, r3]
	cmp	r3, #0
	beq.n	.L21
	ldrh	r2, [r5, #8]
	ldrh	r3, [r5, #10]
	orrs	r3, r2
	cmp	r3, #0
	beq.n	.L21
	ldrh	r7, [r6, #36]
	cmp	r7, #0
	beq.n	.L22
	ldrh	r0, [r5, #12]
	ldrh	r1, [r5, #14]
	ldrh	r2, [r5, #8]
	ldrh	r3, [r5, #10]
	bl	Func_08016178
	b.n	.L23
.L22:
	add	r1, sp, #48
	mov	fp, r1
	movs	r0, #36
	movs	r1, #8
	add	r0, sp
	adds	r1, r1, r6
	mov	r2, sp
	mov	r3, sp
	adds	r3, #40
	adds	r2, #44
	str	r0, [sp, #0]
	str	r1, [sp, #4]
	mov	r9, r0
	mov	sl, r1
	adds	r0, r4, #0
	mov	r1, fp
	str	r3, [sp, #16]
	str	r4, [sp, #12]
	str	r2, [sp, #20]
	str	r7, [sp, #8]
	bl	Func_0801868c
	ldrh	r1, [r5, #22]
	movs	r3, #128
	ands	r3, r1
	ldr	r4, [sp, #12]
	cmp	r3, #0
	beq.n	.L24
	ldr	r2, [sp, #36]
	ldr	r3, [sp, #24]
	cmp	r3, r2
	beq.n	.L25
	subs	r2, r2, r3
	ldr	r3, [sp, #44]
	subs	r3, r3, r2
	str	r3, [sp, #44]
.L25:
	ldr	r3, [sp, #44]
	cmp	r3, #0
	bge.n	.L24
	str	r7, [sp, #44]
.L24:
	movs	r3, #128
	lsls	r3, r3, #1
	ands	r3, r1
	cmp	r3, #0
	bne.n	.L26
	ldr	r3, [sp, #40]
	ldr	r0, [sp, #28]
	subs	r3, r0, r3
	cmp	r3, #0
	bge.n	.L27
	adds	r3, #3
.L27:
	ldr	r2, [sp, #48]
	asrs	r3, r3, #2
	adds	r2, r2, r3
	mov	r1, r9
	movs	r3, #2
	str	r2, [sp, #48]
	mov	r2, sl
	str	r1, [sp, #0]
	str	r2, [sp, #4]
	str	r3, [sp, #8]
	adds	r0, r4, #0
	mov	r1, fp
	ldr	r2, [sp, #20]
	ldr	r3, [sp, #16]
	bl	Func_0801868c
.L26:
	ldr	r3, [sp, #48]
	strh	r3, [r5, #12]
	ldr	r3, [sp, #44]
	strh	r3, [r5, #14]
	ldr	r3, [sp, #40]
	strh	r3, [r5, #8]
	ldr	r3, [sp, #36]
	strh	r3, [r5, #10]
.L23:
	ldrh	r0, [r5, #12]
	ldrh	r1, [r5, #14]
	ldrh	r2, [r5, #8]
	ldrh	r3, [r5, #10]
	bl	Func_080170f8
.L21:
	ldrh	r3, [r6, #30]
	movs	r2, #0
	ldr	r5, [pc, #168]
	strh	r3, [r6, #4]
	strh	r2, [r6, #6]
	strh	r2, [r6, #16]
	add	r5, r8
	ldrh	r0, [r5, #0]
	bl	Func_08003f3c
	movs	r3, #99
	strh	r3, [r5, #0]
	b.n	.L12
	ldr	r3, [pc, #148]
	add	r3, r8
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L28
	movs	r4, #225
	ldrh	r3, [r6, #20]
	lsls	r4, r4, #2
	cmp	r3, r4
	bcs.n	.L28
	ldr	r2, [pc, #132]
	movs	r3, #0
	str	r3, [r2, #0]
.L28:
	adds	r0, r6, #0
	bl	Func_080199ec
	cmp	r0, #0
	beq.n	.L29
	b.n	.L30
.L29:
	ldr	r3, [pc, #120]
	strh	r3, [r6, #20]
	b.n	.L12
	ldrh	r3, [r6, #20]
	cmp	r3, #0
	bne.n	.L31
	movs	r3, #20
	b.n	.L32
	ldrh	r3, [r6, #20]
	cmp	r3, #0
	bne.n	.L31
	movs	r3, #120
.L32:
	strh	r3, [r6, #20]
.L31:
	ldr	r2, [pc, #96]
	movs	r3, #0
	add	r2, r8
	strh	r3, [r2, #0]
	adds	r0, r6, #0
	bl	Func_0801999c
	b.n	.L12
	ldrh	r3, [r6, #20]
	cmp	r3, #0
	bne.n	.L33
	movs	r3, #60
	strh	r3, [r6, #20]
.L33:
	ldr	r2, [pc, #72]
	movs	r3, #0
	add	r2, r8
	strh	r3, [r2, #0]
	b.n	.L12
	ldrh	r3, [r6, #18]
	ldr	r2, [pc, #32]
	adds	r3, #1
	ands	r3, r2
	strh	r3, [r6, #18]
	ldrh	r3, [r6, #18]
	movs	r0, #235
	lsls	r0, r0, #4
	lsls	r3, r3, #1
	adds	r3, r3, r0
	mov	r1, r8
	ldrh	r3, [r1, r3]
	adds	r0, r6, #0
	strh	r3, [r6, #22]
	bl	Func_080167ac
	b.n	.L12
	movs	r0, r0
	.4byte 0x000001ff
	.4byte 0x000012f8
	.4byte 0x000001ff
	.4byte 0x000012b6
	.4byte 0x00000ea4
	.4byte 0x03001af8
	.4byte 0x00000397
	.4byte 0x000012f6
	ldrh	r3, [r6, #18]
	ldr	r2, [pc, #32]
	adds	r3, #1
	ands	r3, r2
	strh	r3, [r6, #18]
	ldrh	r3, [r6, #18]
	movs	r2, #235
	lsls	r3, r3, #1
	lsls	r2, r2, #4
	adds	r3, r3, r2
	mov	r4, r8
	ldrh	r3, [r4, r3]
	adds	r0, r6, #0
	strh	r3, [r6, #24]
	bl	Func_080167ac
	b.n	.L12
	movs	r0, r0
	.4byte 0x000001ff
	ldrh	r3, [r6, #18]
	ldr	r2, [pc, #44]
	adds	r3, #1
	ands	r3, r2
	strh	r3, [r6, #18]
	ldrh	r3, [r6, #18]
	movs	r0, #235
	lsls	r0, r0, #4
	lsls	r3, r3, #1
	adds	r3, r3, r0
	mov	r1, r8
	ldrh	r3, [r1, r3]
	adds	r0, r6, #0
	b.n	.L34
	movs	r3, #0
	movs	r2, #15
	strh	r3, [r6, #24]
	adds	r0, r6, #0
	movs	r3, #10
	strh	r2, [r6, #22]
.L34:
	strh	r3, [r6, #26]
	bl	Func_080167ac
	b.n	.L12
	.4byte 0x000001ff
	ldrh	r3, [r6, #18]
	ldr	r0, [pc, #44]
	adds	r3, #1
	ands	r3, r0
	strh	r3, [r6, #18]
	ldrh	r2, [r6, #18]
	movs	r4, #235
	lsls	r3, r2, #1
	lsls	r4, r4, #4
	adds	r3, r3, r4
	mov	r4, r8
	ldrh	r3, [r4, r3]
	ldr	r1, [r6, #0]
	adds	r2, #1
	strh	r3, [r1, #18]
	ands	r2, r0
	movs	r3, #10
	strh	r3, [r6, #20]
	strh	r2, [r6, #18]
	b.n	.L12
	movs	r3, #1
	strh	r3, [r6, #32]
	movs	r0, #8
	b.n	.L6
	.4byte 0x000001ff
.L12:
	ldr	r3, [pc, #108]
	add	r3, r8
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L35
	b.n	.L36
.L35:
	movs	r0, #1
	str	r0, [sp, #32]
	b.n	.L36
.L9:
	ldrh	r3, [r6, #4]
	adds	r2, r3, #0
	adds	r2, #128
	cmp	r2, #0
	bge.n	.L37
	ldr	r1, [pc, #84]
	adds	r2, r3, r1
.L37:
	asrs	r5, r2, #8
	ldrh	r2, [r6, #6]
	adds	r3, r2, #0
	adds	r3, #128
	cmp	r3, #0
	bge.n	.L38
	ldr	r4, [pc, #68]
	adds	r3, r2, r4
.L38:
	asrs	r3, r3, #8
	mov	ip, r3
	movs	r0, #131
	ldr	r3, [pc, #64]
	lsls	r0, r0, #2
	adds	r3, r3, r0
	ldrb	r3, [r3, #0]
	ldr	r2, [pc, #60]
	ldrb	r2, [r2, r3]
	ldr	r3, [pc, #60]
	add	r3, r8
	ldrb	r3, [r3, #0]
	mov	sl, r2
	ldrh	r2, [r6, #18]
	cmp	r3, #0
	beq.n	.L39
	adds	r5, #8
.L39:
	adds	r3, r2, #1
	ldr	r2, [pc, #44]
	movs	r1, #235
	ands	r3, r2
	lsls	r3, r3, #1
	lsls	r1, r1, #4
	adds	r3, r3, r1
	mov	r0, r8
	ldrh	r4, [r0, r3]
	cmp	r4, #222
	bne.n	.L40
	movs	r3, #128
	lsls	r3, r3, #7
	b.n	.L41
	movs	r0, r0
	.4byte 0x00000ea5
	.4byte 0x0000017f
	.4byte 0x02000240
	.4byte 0x0807380e
	.4byte 0x00000ea4
	.4byte 0x000001ff
.L40:
	cmp	r4, #223
	bne.n	.L42
	movs	r3, #128
	lsls	r3, r3, #8
.L41:
	orrs	r7, r3
	ldrh	r3, [r6, #18]
	adds	r3, #1
	ands	r3, r2
	strh	r3, [r6, #18]
.L42:
	ldr	r0, [r6, #0]
	ldrh	r2, [r0, #22]
	movs	r3, #8
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L43
	cmp	r7, #32
	bls.n	.L43
	cmp	r4, #32
	bls.n	.L43
	adds	r3, r7, #0
	adds	r2, r4, #0
	ldr	r1, [pc, #88]
	subs	r3, #32
	subs	r2, #32
	lsls	r3, r3, #5
	lsls	r2, r2, #5
	ldrh	r3, [r1, r3]
	ldrh	r2, [r1, r2]
	movs	r1, #240
	adds	r3, r3, r2
	lsls	r3, r3, #16
	lsls	r1, r1, #12
	cmp	r3, r1
	bhi.n	.L43
	lsls	r3, r4, #8
	orrs	r7, r3
	ldrh	r3, [r6, #18]
	ldr	r2, [pc, #52]
	adds	r3, #1
	ands	r3, r2
	strh	r3, [r6, #18]
.L43:
	movs	r3, #0
	str	r3, [sp, #0]
	adds	r2, r5, #0
	mov	r3, ip
	adds	r1, r7, #0
	bl	Func_08018cac
	ldr	r3, [pc, #36]
	adds	r4, r0, #0
	movs	r0, #131
	lsls	r0, r0, #2
	adds	r3, r3, r0
	ldr	r2, [pc, #32]
	ldrb	r3, [r3, #0]
	ldrb	r3, [r2, r3]
	strh	r3, [r6, #34]
	cmp	r4, #0
	beq.n	.L44
	ldr	r1, [pc, #24]
	add	r1, r8
	ldrh	r3, [r1, #0]
	b.n	.L45
	movs	r0, r0
	.4byte 0x000001ff
	.4byte 0x08032224
	.4byte 0x02000240
	.4byte 0x08073808
	.4byte 0x000012f4
.L45:
	cmp	r3, #0
	beq.n	.L46
	ldr	r5, [pc, #36]
	add	r5, r8
	ldrh	r2, [r5, #0]
	adds	r3, r2, #0
	cmp	r3, #0
	bne.n	.L47
	cmp	r7, #32
	beq.n	.L46
	ldrh	r0, [r1, #0]
	movs	r3, #3
	ands	r3, r7
	adds	r0, r0, r3
	str	r4, [sp, #12]
	bl	Func_080f9010
	mov	r1, sl
	strh	r1, [r5, #0]
	ldr	r4, [sp, #12]
	b.n	.L46
	movs	r0, r0
	.4byte 0x000012f6
.L47:
	ldr	r0, [pc, #92]
	adds	r3, r2, r0
	strh	r3, [r5, #0]
.L46:
	lsls	r0, r4, #8
	cmp	r7, #32
	bne.n	.L48
	ldrh	r3, [r6, #16]
	lsls	r3, r3, #1
	adds	r3, #8
	ldrh	r3, [r6, r3]
	adds	r0, r0, r3
.L48:
	ldrh	r3, [r6, #4]
	adds	r3, r3, r0
	strh	r3, [r6, #4]
.L44:
	cmp	r7, #32
	bne.n	.L36
	ldr	r3, [pc, #64]
	add	r3, r8
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L36
	movs	r1, #1
	str	r1, [sp, #32]
.L36:
	ldrh	r2, [r6, #20]
	adds	r3, r2, #0
	cmp	r3, #0
	beq.n	.L49
	ldr	r4, [pc, #40]
	adds	r3, r2, r4
	strh	r3, [r6, #20]
	lsls	r3, r3, #16
	cmp	r3, #0
	bne.n	.L50
.L49:
	ldrh	r3, [r6, #18]
	ldr	r2, [pc, #20]
	adds	r3, #1
	ands	r3, r2
	strh	r3, [r6, #18]
.L50:
	ldr	r0, [sp, #32]
	subs	r0, #1
	str	r0, [sp, #32]
	cmp	r0, #0
	beq.n	.L51
	b.n	.L5
.L51:
	b.n	.L4
	movs	r0, r0
	.4byte 0x000001ff
	.4byte 0x0000ffff
	.4byte 0x00000ea5
.L4:
	movs	r0, #0
.L6:
	add	sp, #52
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
