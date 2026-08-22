@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08025200
	.thumb_func
Func_08025200:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #304
	str	r0, [sp, #80]
	str	r2, [sp, #72]
	str	r1, [sp, #76]
	ldr	r5, [pc, #164]
	movs	r2, #1
	ldr	r1, [r5, #0]
	negs	r2, r2
	movs	r0, #128
	str	r1, [sp, #68]
	str	r2, [sp, #64]
	str	r2, [sp, #60]
	bl	Func_080040b4
	movs	r3, #42
	str	r0, [sp, #56]
	str	r3, [sp, #0]
	movs	r1, #5
	movs	r2, #30
	movs	r3, #4
	movs	r0, #0
	bl	Func_080162d4
	movs	r3, #0
	str	r0, [sp, #52]
	str	r3, [sp, #48]
	adds	r5, #168
	ldr	r3, [r5, #0]
	ldr	r1, [r3, #52]
	ldr	r2, [r3, #48]
	ldr	r3, [r3, #56]
	mov	r9, r1
	mov	r8, r2
	str	r3, [sp, #44]
	movs	r3, #6
	str	r3, [sp, #0]
	movs	r1, #9
	movs	r3, #11
	movs	r0, #15
	movs	r2, #15
	bl	Func_080162d4
	mov	r3, sp
	adds	r3, #84
	ldr	r1, [pc, #88]
	movs	r6, #128
	str	r3, [sp, #16]
	mov	fp, r0
	movs	r7, #0
	mov	ip, r1
	adds	r4, r3, #0
	lsls	r6, r6, #23
	movs	r5, #0
.L0:
	lsls	r0, r7, #1
	str	r6, [r4, #4]
	str	r5, [r4, #8]
	mov	r3, fp
	ldrh	r2, [r3, #12]
	ldr	r3, [pc, #52]
	lsls	r2, r2, #3
	ldrh	r1, [r4, #6]
	adds	r2, #8
	ands	r2, r3
	mov	r3, ip
	ands	r3, r1
	orrs	r3, r2
	mov	r1, fp
	strh	r3, [r4, #6]
	ldrh	r3, [r1, #14]
	adds	r0, r0, r3
	lsls	r0, r0, #3
	adds	r0, #4
	adds	r7, #1
	strb	r0, [r4, #4]
	adds	r4, #12
	cmp	r7, #4
	ble.n	.L0
	mov	r2, sp
	adds	r2, #144
	ldr	r3, [pc, #20]
	str	r2, [sp, #28]
	ldr	r6, [sp, #16]
	str	r2, [sp, #8]
	movs	r5, #8
	b.n	.L1
	.4byte 0x000001ff
	.4byte 0x03001e8c
	.4byte 0xfffffe00
	.4byte 0xfffffc00
.L1:
	mov	sl, r3
	movs	r7, #4
.L2:
	movs	r0, #128
	bl	Func_080040b4
	ldr	r2, [sp, #8]
	stmia	r2!, {r0}
	adds	r1, r2, #0
	str	r1, [sp, #8]
	movs	r1, #1
	negs	r1, r1
	bl	Func_080040d0
	ldr	r3, [pc, #52]
	ands	r0, r3
	ldrh	r3, [r5, r6]
	mov	r1, sl
	ands	r3, r1
	orrs	r3, r0
	subs	r7, #1
	strh	r3, [r5, r6]
	adds	r5, #12
	cmp	r7, #0
	bge.n	.L2
	ldr	r5, [pc, #32]
	movs	r1, #128
	adds	r0, r5, #0
	lsls	r1, r1, #2
	bl	Func_080251d4
	adds	r0, r5, #0
	ldr	r1, [pc, #24]
	bl	Func_080251d4
	adds	r5, #1
	movs	r1, #132
	lsls	r1, r1, #2
	adds	r0, r5, #0
	b.n	.L3
	movs	r0, r0
	.4byte 0x000003ff
	.4byte 0x0000f018
	.4byte 0x00000201
.L3:
	bl	Func_080251d4
	ldr	r1, [pc, #148]
	adds	r0, r5, #0
	bl	Func_080251d4
	movs	r2, #146
	lsls	r2, r2, #1
	mov	r3, r8
	mov	r1, sp
	add	r2, sp
	lsls	r3, r3, #1
	adds	r1, #164
	str	r2, [sp, #24]
	str	r3, [sp, #20]
	str	r1, [sp, #32]
.L53:
	ldr	r2, [sp, #64]
	cmp	r9, r2
	bne.n	.L4
	ldr	r3, [sp, #60]
	cmp	r8, r3
	bne.n	.L4
	b.n	.L5
.L4:
	ldr	r1, [sp, #68]
	ldr	r3, [pc, #108]
	adds	r2, r1, r3
	movs	r3, #1
	strb	r3, [r2, #0]
	ldr	r2, [sp, #60]
	mov	r1, fp
	ldrh	r0, [r1, #12]
	ldrh	r1, [r1, #14]
	lsls	r3, r2, #1
	adds	r1, r1, r3
	mov	r3, fp
	ldrh	r2, [r3, #8]
	movs	r3, #15
	adds	r1, #1
	str	r3, [sp, #0]
	adds	r0, #1
	subs	r2, #2
	movs	r3, #1
	bl	Func_08022768
	bl	Func_08016738
	ldr	r1, [sp, #72]
	cmp	r1, #0
	beq.n	.L6
	mov	r3, r9
	add	r3, r8
	ldr	r2, [sp, #76]
	lsls	r3, r3, #1
	adds	r5, r3, r2
	ldrh	r1, [r5, #0]
	ldr	r0, [sp, #80]
	bl	Func_08025180
	cmp	r0, #2
	bne.n	.L7
	ldr	r5, [sp, #32]
	ldr	r0, [pc, #40]
	adds	r1, r5, #0
	b.n	.L8
.L7:
	ldrh	r3, [r5, #0]
	ldr	r0, [pc, #20]
	ldr	r5, [sp, #32]
	ands	r0, r3
	ldr	r3, [pc, #28]
	adds	r1, r5, #0
	adds	r0, r0, r3
.L8:
	movs	r2, #52
	bl	Func_0801965c
	b.n	.L9
	movs	r0, r0
	.4byte 0x000001ff
	.4byte 0x00000211
	.4byte 0x00000ea6
	.4byte 0x000008ee
	.4byte 0x00000075
.L6:
	ldr	r5, [sp, #32]
	ldr	r0, [pc, #100]
	adds	r1, r5, #0
	movs	r2, #52
	bl	Func_0801965c
.L9:
	ldr	r1, [sp, #52]
	movs	r3, #4
	adds	r0, r5, #0
	movs	r2, #0
	bl	Func_08017aa4
	ldr	r1, [sp, #64]
	mov	r3, r8
	str	r3, [sp, #60]
	cmp	r9, r1
	beq.n	.L10
	mov	r0, fp
	bl	Func_08016498
	mov	r2, r9
	ldr	r1, [sp, #76]
	lsls	r3, r2, #1
	adds	r3, r3, r1
	ldrh	r5, [r3, #0]
	movs	r7, #0
	cmp	r5, #0
	beq.n	.L11
	ldr	r1, [sp, #28]
	ldr	r2, [sp, #16]
	adds	r6, r3, #0
	movs	r3, #8
	str	r3, [sp, #12]
	str	r1, [sp, #4]
	mov	sl, r2
.L14:
	adds	r0, r5, #0
	bl	Func_08077018
	movs	r0, #15
	bl	Func_0801e71c
	adds	r1, r5, #0
	ldr	r0, [sp, #80]
	bl	Func_08025180
	cmp	r0, #0
	beq.n	.L12
	movs	r0, #4
	bl	Func_0801e71c
	b.n	.L13
	movs	r0, r0
	.4byte 0x000008e5
.L12:
	movs	r3, #128
	lsls	r3, r3, #3
	ands	r3, r5
	cmp	r3, #0
	beq.n	.L13
	movs	r0, #2
	bl	Func_0801e71c
.L13:
	ldr	r0, [pc, #84]
	ldr	r3, [pc, #88]
	ands	r0, r5
	adds	r0, r0, r3
	mov	r1, fp
	lsls	r3, r7, #4
	movs	r2, #16
	bl	Func_0801e7c0
	movs	r0, #15
	bl	Func_0801e71c
	ldr	r3, [sp, #4]
	ldmia	r3!, {r1}
	adds	r0, r5, #0
	adds	r2, r3, #0
	str	r2, [sp, #4]
	bl	Func_08021af0
	ldr	r3, [pc, #40]
	ldr	r1, [sp, #12]
	mov	r2, sl
	ands	r0, r3
	ldrh	r3, [r1, r2]
	ldr	r2, [pc, #32]
	ands	r3, r2
	orrs	r3, r0
	mov	r2, sl
	strh	r3, [r1, r2]
	adds	r7, #1
	adds	r1, #12
	str	r1, [sp, #12]
	cmp	r7, #4
	bgt.n	.L11
	adds	r6, #2
	ldrh	r5, [r6, #0]
	cmp	r5, #0
	bne.n	.L14
	b.n	.L11
	movs	r0, r0
	.4byte 0x000003ff
	.4byte 0xfffffc00
	.4byte 0x000001ff
	.4byte 0x00000182
.L11:
	mov	r3, r9
	str	r7, [sp, #48]
	str	r3, [sp, #64]
.L10:
	ldr	r1, [sp, #72]
	cmp	r1, #5
	ble.n	.L15
	movs	r7, #0
	adds	r1, #4
	mov	sl, r1
	b.n	.L16
.L18:
	ldr	r2, [pc, #120]
	mov	r0, r9
	movs	r1, #5
	adds	r6, r7, r2
	bl	Func_080022ec
	cmp	r7, r0
	bne.n	.L17
	ldr	r3, [pc, #108]
	adds	r6, r7, r3
.L17:
	mov	r1, fp
	ldrh	r2, [r1, #8]
	subs	r2, r2, r5
	adds	r2, r2, r7
	movs	r3, #0
	str	r3, [sp, #0]
	subs	r2, #2
	mov	r0, fp
	adds	r1, r6, #0
	subs	r3, #1
	bl	Func_08019000
	adds	r7, #1
.L16:
	mov	r0, sl
	movs	r1, #5
	bl	Func_080022ec
	adds	r5, r0, #0
	cmp	r7, r5
	blt.n	.L18
.L15:
	mov	r1, fp
	ldrh	r0, [r1, #12]
	ldr	r2, [sp, #20]
	ldrh	r1, [r1, #14]
	mov	r3, fp
	adds	r1, r1, r2
	ldrh	r2, [r3, #8]
	movs	r3, #14
	adds	r1, #1
	subs	r2, #2
	str	r3, [sp, #0]
	adds	r0, #1
	movs	r3, #1
	bl	Func_08022768
	ldr	r3, [pc, #40]
	ldr	r1, [sp, #68]
	adds	r2, r1, r3
	movs	r3, #1
	strb	r3, [r2, #0]
	ldr	r2, [pc, #32]
	adds	r3, r1, r2
	movs	r1, #0
	strb	r1, [r3, #0]
.L5:
	ldr	r2, [sp, #72]
	cmp	r2, #5
	ble.n	.L19
	movs	r7, #0
	adds	r2, #4
	mov	sl, r2
	b.n	.L20
	movs	r0, r0
	.4byte 0x0000f301
	.4byte 0x0000f30b
	.4byte 0x00000ea3
	.4byte 0x00000ea6
.L22:
	ldr	r3, [pc, #284]
	ldr	r1, [pc, #288]
	adds	r6, r7, r3
	ldr	r3, [r1, #0]
	movs	r2, #15
	ands	r3, r2
	cmp	r3, #11
	bhi.n	.L21
	mov	r0, r9
	movs	r1, #5
	bl	Func_080022ec
	cmp	r7, r0
	bne.n	.L21
	ldr	r2, [pc, #264]
	adds	r6, r7, r2
.L21:
	mov	r3, fp
	movs	r1, #5
	mov	r0, sl
	ldrh	r5, [r3, #8]
	bl	Func_080022ec
	subs	r5, r5, r0
	adds	r5, r5, r7
	movs	r1, #0
	subs	r5, #2
	movs	r3, #1
	str	r1, [sp, #0]
	mov	r0, fp
	adds	r1, r6, #0
	adds	r2, r5, #0
	negs	r3, r3
	bl	Func_08019000
	adds	r7, #1
.L20:
	mov	r0, sl
	movs	r1, #5
	bl	Func_080022ec
	cmp	r7, r0
	blt.n	.L22
	mov	r3, fp
	ldrh	r2, [r3, #8]
	movs	r5, #1
	negs	r5, r5
	subs	r2, r2, r0
	movs	r1, #0
	str	r1, [sp, #0]
	mov	r0, fp
	adds	r3, r5, #0
	subs	r2, #3
	ldr	r1, [pc, #196]
	bl	Func_08019000
	mov	r3, fp
	ldrh	r2, [r3, #8]
	movs	r1, #0
	str	r1, [sp, #0]
	subs	r2, #2
	mov	r0, fp
	ldr	r1, [pc, #180]
	adds	r3, r5, #0
	bl	Func_08019000
	ldr	r2, [sp, #68]
	ldr	r3, [pc, #176]
	adds	r1, r2, r3
	mov	r2, fp
	ldrh	r3, [r2, #14]
	subs	r3, #1
	lsrs	r3, r3, #2
	movs	r2, #2
	lsls	r2, r3
	ldrb	r3, [r1, #0]
	orrs	r2, r3
	strb	r2, [r1, #0]
.L19:
	ldr	r3, [sp, #48]
	cmp	r3, #0
	ble.n	.L23
	ldr	r5, [sp, #16]
	adds	r7, r3, #0
.L24:
	adds	r0, r5, #0
	movs	r1, #240
	subs	r7, #1
	bl	Func_08003dec
	adds	r5, #12
	cmp	r7, #0
	bne.n	.L24
.L23:
	mov	r1, fp
	ldrh	r3, [r1, #12]
	lsls	r3, r3, #3
	subs	r3, #2
	ldr	r2, [sp, #20]
	str	r3, [sp, #36]
	ldrh	r3, [r1, #14]
	adds	r3, r2, r3
	lsls	r3, r3, #3
	adds	r3, #20
	ldr	r1, [sp, #24]
	str	r3, [sp, #40]
	movs	r3, #128
	lsls	r3, r3, #23
	movs	r2, #0
	str	r3, [r1, #4]
	str	r2, [r1, #8]
	ldr	r0, [sp, #56]
	ldr	r1, [pc, #96]
	bl	Func_080040d0
	ldr	r3, [pc, #48]
	ldr	r1, [sp, #24]
	ands	r0, r3
	ldr	r2, [pc, #48]
	ldrh	r3, [r1, #8]
	ands	r3, r2
	orrs	r3, r0
	adds	r2, r1, #0
	strh	r3, [r2, #8]
	ldr	r3, [pc, #52]
	ldr	r2, [r3, #0]
	movs	r0, #4
	ldr	r1, [sp, #36]
	ands	r2, r0
	ldr	r3, [pc, #64]
	lsrs	r2, r2, #1
	adds	r2, r1, r2
	adds	r2, r2, r3
	ldr	r1, [sp, #24]
	ldr	r3, [pc, #20]
	ands	r2, r3
	ldrh	r3, [r1, #6]
	ldr	r1, [pc, #16]
	ands	r3, r1
	b.n	.L25
	movs	r0, r0
	.4byte 0x000003ff
	.4byte 0xfffffc00
	.4byte 0x000001ff
	.4byte 0xfffffe00
	.4byte 0x0000f301
	.4byte 0x03001e40
	.4byte 0x0000f30b
	.4byte 0x0000f334
	.4byte 0x0000f335
	.4byte 0x00000ea3
	.4byte 0x080310a4
	.4byte 0x0000fffc
.L25:
	orrs	r3, r2
	ldr	r1, [pc, #640]
	ldr	r2, [sp, #24]
	strh	r3, [r2, #6]
	ldr	r3, [r1, #0]
	ldr	r2, [sp, #40]
	ands	r3, r0
	lsrs	r3, r3, #2
	subs	r3, r2, r3
	ldr	r1, [sp, #24]
	adds	r3, #248
	strb	r3, [r1, #4]
	ldr	r2, [sp, #72]
	cmp	r2, #0
	beq.n	.L26
	ldr	r0, [sp, #24]
	movs	r1, #242
	bl	Func_08003dec
.L26:
	ldr	r3, [pc, #608]
	ldr	r1, [r3, #0]
	mov	r2, r8
	mov	r3, r9
	str	r3, [r1, #52]
	str	r2, [r1, #48]
	ldr	r3, [sp, #44]
	str	r3, [r1, #56]
	ldr	r0, [pc, #596]
	ldr	r3, [r0, #0]
	movs	r2, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L27
	ldr	r1, [sp, #72]
	cmp	r1, #0
	beq.n	.L28
	mov	r2, r9
	add	r2, r8
	ldr	r1, [sp, #76]
	lsls	r3, r2, #1
	adds	r5, r3, r1
	ldrh	r0, [r5, #0]
	movs	r7, #128
	mov	sl, r2
	lsls	r7, r7, #3
	bl	Func_08077018
	ldrh	r2, [r5, #0]
	adds	r3, r7, #0
	ands	r3, r2
	movs	r6, #0
	cmp	r3, #0
	bne.n	.L29
	ldrh	r1, [r5, #0]
	ldr	r0, [sp, #80]
	bl	Func_08025180
	adds	r6, r0, #0
	cmp	r6, #0
	bne.n	.L29
	b.n	.L30
.L29:
	movs	r0, #114
	bl	Func_080f9010
	cmp	r6, #2
	bne.n	.L31
	ldr	r5, [sp, #32]
	ldr	r0, [pc, #520]
	b.n	.L32
.L31:
	ldrh	r2, [r5, #0]
	adds	r3, r7, #0
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L33
	ldr	r5, [sp, #32]
	ldr	r0, [pc, #508]
.L32:
	adds	r1, r5, #0
	movs	r2, #52
	bl	Func_0801965c
	b.n	.L34
.L33:
	ldr	r5, [sp, #32]
	ldr	r0, [pc, #496]
	adds	r1, r5, #0
	movs	r2, #52
	bl	Func_0801965c
.L34:
	bl	Func_08016738
	adds	r0, r5, #0
	ldr	r1, [sp, #52]
	movs	r2, #0
	movs	r3, #4
	bl	Func_08017aa4
	b.n	.L35
.L28:
	movs	r2, #1
	negs	r2, r2
	mov	sl, r2
	b.n	.L30
.L27:
	ldr	r3, [r1, #76]
	cmp	r3, #0
	beq.n	.L36
	ldr	r3, [r0, #0]
	movs	r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L35
.L36:
	movs	r0, #113
	bl	Func_080f9010
	movs	r3, #1
	negs	r3, r3
	mov	sl, r3
	b.n	.L30
.L35:
	ldr	r1, [sp, #72]
	cmp	r1, #0
	bne.n	.L37
	b.n	.L38
.L37:
	ldr	r1, [pc, #428]
	ldr	r3, [r1, #0]
	movs	r2, #128
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L39
	movs	r0, #111
	bl	Func_080f9010
	movs	r2, #1
	add	r8, r2
	mov	r3, r8
	cmp	r3, #5
	beq.n	.L40
	mov	r3, r9
	ldr	r1, [sp, #72]
	add	r3, r8
	cmp	r3, r1
	bne.n	.L41
.L40:
	movs	r2, #0
	mov	r8, r2
.L41:
	mov	r1, r8
	mov	r3, r8
	lsls	r1, r1, #1
	str	r3, [sp, #44]
	str	r1, [sp, #20]
	b.n	.L38
.L39:
	ldr	r3, [r1, #0]
	movs	r2, #64
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L42
	movs	r0, #111
	bl	Func_080f9010
	movs	r2, #1
	negs	r2, r2
	add	r8, r2
	mov	r3, r8
	cmp	r3, #0
	bge.n	.L43
	ldr	r0, [sp, #72]
	movs	r1, #5
	subs	r0, #1
	bl	Func_080022ec
	lsls	r3, r0, #2
	adds	r3, r3, r0
	cmp	r9, r3
	bne.n	.L44
	ldr	r1, [sp, #72]
	mov	r2, r9
	subs	r3, r1, r2
	subs	r3, #1
	b.n	.L45
.L44:
	movs	r3, #4
.L45:
	mov	r8, r3
.L43:
	mov	r2, r8
	mov	r1, r8
	lsls	r2, r2, #1
	str	r1, [sp, #44]
	b.n	.L46
.L42:
	ldr	r3, [r1, #0]
	movs	r2, #16
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L47
	movs	r0, #111
	bl	Func_080f9010
	bl	Func_0800352c
	mov	r3, r9
	ldr	r1, [sp, #72]
	adds	r3, #5
	cmp	r3, r1
	blt.n	.L48
	mov	r2, r9
	cmp	r2, #0
	beq.n	.L38
	ldr	r1, [sp, #44]
	mov	r8, r1
	mov	r2, r8
	movs	r3, #0
	lsls	r2, r2, #1
	mov	r9, r3
	b.n	.L46
.L48:
	ldr	r0, [sp, #72]
	mov	r9, r3
	ldr	r3, [sp, #44]
	subs	r0, #1
	movs	r1, #5
	mov	r8, r3
	bl	Func_080022ec
	lsls	r3, r0, #2
	adds	r3, r3, r0
	cmp	r9, r3
	bne.n	.L49
	ldr	r1, [sp, #72]
	mov	r2, r9
	subs	r3, r1, r2
	subs	r3, #1
	mov	r8, r3
	ldr	r3, [sp, #44]
	cmp	r8, r3
	ble.n	.L50
	mov	r8, r3
	b.n	.L51
.L47:
	ldr	r3, [r1, #0]
	movs	r2, #32
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L38
	movs	r0, #111
	bl	Func_080f9010
	bl	Func_0800352c
	mov	r2, r9
	cmp	r2, #0
	beq.n	.L52
	ldr	r1, [sp, #44]
	mov	r8, r1
	movs	r3, #5
	mov	r2, r8
	negs	r3, r3
	lsls	r2, r2, #1
	add	r9, r3
	b.n	.L46
.L52:
	ldr	r0, [sp, #72]
	movs	r1, #5
	subs	r0, #1
	bl	Func_080022ec
	lsls	r3, r0, #2
	adds	r3, r3, r0
	mov	r9, r3
	ldr	r3, [sp, #44]
	mov	r1, r9
	mov	r8, r3
	cmp	r1, #0
	beq.n	.L51
	ldr	r2, [sp, #72]
	subs	r3, r2, r1
	subs	r3, #1
	mov	r8, r3
	ldr	r3, [sp, #44]
	cmp	r8, r3
	ble.n	.L49
	mov	r8, r3
	b.n	.L51
.L50:
	mov	r3, r8
	lsls	r3, r3, #1
	str	r3, [sp, #20]
	b.n	.L38
.L51:
	mov	r1, r8
	lsls	r1, r1, #1
	str	r1, [sp, #20]
	b.n	.L38
.L49:
	mov	r2, r8
	lsls	r2, r2, #1
.L46:
	str	r2, [sp, #20]
.L38:
	movs	r0, #1
	bl	Func_080030f8
	b.n	.L53
.L30:
	ldr	r0, [sp, #52]
	movs	r1, #1
	bl	Func_08016418
	movs	r1, #1
	mov	r0, fp
	bl	Func_08016418
	movs	r0, #1
	bl	Func_080030f8
	ldr	r0, [sp, #56]
	bl	Func_08003f3c
	ldr	r5, [sp, #28]
	movs	r7, #4
.L54:
	ldmia	r5!, {r0}
	subs	r7, #1
	bl	Func_08003f3c
	cmp	r7, #0
	bge.n	.L54
	movs	r0, #1
	bl	Func_080030f8
	mov	r0, sl
	add	sp, #304
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x03001e40
	.4byte 0x03001f34
	.4byte 0x03001c94
	.4byte 0x000008ee
	.4byte 0x000008ec
	.4byte 0x000008eb
	.4byte 0x03001b04
