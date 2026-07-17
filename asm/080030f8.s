@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080030f8
	.thumb_func
Func_080030f8:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #8
	str	r0, [sp, #4]
	mov	sl, sp
	ldr	r1, [pc, #288]
	cmp	sl, r1
	bhi.n	.L0
	ldr	r4, [pc, #288]
	ldr	r3, [pc, #288]
	mov	r1, sl
	subs	r2, r4, r1
	movs	r5, #132
	str	r2, [r3, #0]
	lsls	r5, r5, #24
	lsrs	r2, r2, #2
	ldr	r3, [pc, #280]
	mov	r0, sl
	ldr	r1, [pc, #280]
	orrs	r2, r5
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	mov	sp, r4
.L0:
	ldr	r3, [sp, #4]
	movs	r2, #0
	str	r2, [sp, #0]
	cmp	r2, r3
	bcc.n	.L1
	b.n	.L2
.L1:
	ldr	r2, [pc, #260]
	movs	r1, #1
	mov	fp, r1
	mov	r9, r2
.L33:
	ldr	r5, [pc, #256]
	mov	r3, fp
	movs	r0, #200
	strb	r3, [r5, #0]
	lsls	r0, r0, #4
	bl	Func_08004420
	movs	r3, #0
	movs	r1, #128
	strb	r3, [r5, #0]
	lsls	r1, r1, #3
	movs	r0, #52
	bl	Func_080048b0
	bl	Func_08003e10
	ldr	r3, [pc, #228]
	mov	r1, fp
	strb	r1, [r3, #0]
	ldr	r3, [pc, #228]
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L3
	ldr	r3, [pc, #224]
	ldrh	r1, [r3, #0]
	cmp	r1, #159
	bls.n	.L4
	subs	r1, #160
	b.n	.L5
.L4:
	adds	r1, #68
.L5:
	ldr	r3, [pc, #212]
	ldr	r0, [pc, #216]
	ldrh	r3, [r3, #0]
	ldr	r2, [r0, #0]
	subs	r3, #1
	lsls	r3, r3, #8
	adds	r1, r1, r3
	cmp	r2, #0
	bne.n	.L6
	ldr	r3, [pc, #204]
	str	r2, [r3, #0]
	b.n	.L7
.L6:
	subs	r3, r2, #1
	str	r3, [r0, #0]
.L7:
	ldr	r2, [pc, #192]
	ldr	r3, [r2, #0]
	cmp	r3, r1
	bcs.n	.L3
	str	r1, [r2, #0]
	ldr	r2, [pc, #180]
	movs	r3, #30
	str	r3, [r2, #0]
.L3:
	ldr	r3, [pc, #180]
	ldrb	r3, [r3, #0]
	adds	r2, r3, #0
	cmp	r2, #0
	bne.n	.L8
	ldr	r3, [pc, #176]
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L9
	ldr	r3, [pc, #172]
	ldr	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L10
	mov	r3, r9
	strh	r2, [r3, #0]
	b.n	.L9
.L10:
	mov	r1, r9
	ldrh	r3, [r1, #0]
	mov	r2, r9
	adds	r3, #1
	strh	r3, [r2, #0]
	ldr	r3, [pc, #152]
	ldrh	r2, [r2, #0]
	cmp	r2, r3
	bls.n	.L9
	ldr	r3, [pc, #148]
	mov	r1, fp
	strb	r1, [r3, #0]
.L9:
	ldr	r3, [pc, #132]
	movs	r2, #192
	ldr	r3, [r3, #0]
	lsls	r2, r2, #2
	cmp	r3, r2
	bne.n	.L11
	ldr	r2, [pc, #132]
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	ldrh	r3, [r2, #0]
	cmp	r3, #179
	bls.n	.L8
	ldr	r3, [pc, #44]
	strh	r3, [r2, #0]
	ldr	r3, [pc, #112]
	mov	r1, fp
	strb	r1, [r3, #0]
	b.n	.L8
.L11:
	ldr	r2, [pc, #108]
	ldr	r3, [pc, #28]
	strh	r3, [r2, #0]
.L8:
	ldr	r3, [pc, #104]
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L12
	b.n	.L13
.L12:
	ldr	r5, [pc, #100]
	movs	r2, #128
	ldrb	r3, [r5, #0]
	lsls	r2, r2, #20
	ldr	r7, [pc, #96]
	mov	r8, r2
	ldr	r6, [pc, #68]
	b.n	.L14
	movs	r0, r0
	.4byte 0x00000000
	.4byte 0x030079ff
	.4byte 0x03007a00
	.4byte 0x03001804
	.4byte 0x040000d4
	.4byte 0x020023b0
	.4byte 0x03001d24
	.4byte 0x03001a10
	.4byte 0x03001e44
	.4byte 0x03001f58
	.4byte 0x04000006
	.4byte 0x03001ccc
	.4byte 0x03001af0
	.4byte 0x03001ca4
	.4byte 0x03001ca0
	.4byte 0x03001d08
	.4byte 0x03001ae8
	.4byte 0x00002a30
	.4byte 0x03001cc8
	.4byte 0x03001f5c
	.4byte 0x03001f54
	.4byte 0x03001d20
	.4byte 0x03001d28
.L18:
	ldr	r3, [r6, #0]
	cmp	r3, #12
	bne.n	.L13
	mov	r3, fp
	strb	r3, [r5, #0]
.L19:
	ldrh	r3, [r7, #0]
	ldr	r2, [pc, #44]
	ands	r3, r2
	strh	r3, [r7, #0]
	ldr	r0, [pc, #44]
	movs	r1, #1
.L15:
	svc	2
	ldrh	r2, [r0, #0]
	adds	r3, r1, #0
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L15
	bl	Func_08003538
	ldr	r2, [pc, #28]
	ldrb	r3, [r2, #0]
	cmp	r3, #0
	beq.n	.L16
	movs	r3, #0
	strb	r3, [r2, #0]
	ldr	r2, [pc, #20]
	ldr	r3, [pc, #20]
	str	r3, [r2, #0]
	b.n	.L17
	movs	r0, r0
	.4byte 0x0000fffe
	.4byte 0x03001d28
	.4byte 0x03001cb8
	.4byte 0x03007800
	.4byte 0x19670704
.L17:
	movs	r2, #0
	ldr	r3, [pc, #108]
	strh	r2, [r3, #0]
	bl	Func_08007304
.L16:
	ldrb	r3, [r5, #0]
.L14:
	cmp	r3, #0
	beq.n	.L18
	ldr	r0, [pc, #96]
	ldr	r3, [r0, #0]
	movs	r2, #7
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L13
	ldr	r1, [r6, #0]
	movs	r3, #240
	ands	r1, r3
	cmp	r1, #0
	bne.n	.L13
	ldr	r3, [r0, #0]
	movs	r2, #8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L19
	strb	r1, [r5, #0]
.L13:
	ldr	r2, [pc, #68]
	ldr	r1, [pc, #68]
	ldrh	r3, [r2, #0]
	strh	r3, [r1, #0]
	ldr	r3, [pc, #44]
	ldr	r1, [pc, #64]
	strh	r3, [r2, #0]
	ldr	r2, [pc, #40]
	ldrh	r3, [r1, #0]
	ands	r3, r2
	strh	r3, [r1, #0]
	movs	r0, #1
.L20:
	svc	2
	ldrh	r2, [r1, #0]
	adds	r3, r0, #0
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L20
	movs	r0, #52
	bl	Func_08002dd8
	bl	Func_08003d04
	ldr	r2, [pc, #32]
	ldr	r3, [r2, #0]
	adds	r3, #1
	b.n	.L21
	.4byte 0x00000000
	.4byte 0x0000fffe
	.4byte 0x04000208
	.4byte 0x03001b04
	.4byte 0x03001ccc
	.4byte 0x03001cd0
	.4byte 0x03001d28
	.4byte 0x03001e40
.L21:
	str	r3, [r2, #0]
	ldr	r2, [pc, #188]
	ldr	r3, [r2, #0]
	adds	r3, #1
	str	r3, [r2, #0]
	bl	Func_08003538
	ldr	r3, [pc, #180]
	ldrh	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L22
	bl	Func_08005fcc
	ldr	r2, [pc, #172]
	ldrb	r3, [r2, #0]
	cmp	r3, #0
	beq.n	.L22
	movs	r3, #1
	strb	r3, [r2, #8]
.L22:
	ldr	r1, [pc, #164]
	ldrb	r3, [r1, #0]
	cmp	r3, #0
	bne.n	.L23
	b.n	.L24
.L23:
	ldr	r3, [pc, #156]
	ldrb	r3, [r3, #0]
	adds	r2, r3, #0
	cmp	r2, #0
	beq.n	.L25
	b.n	.L24
.L25:
	movs	r4, #128
	lsls	r4, r4, #19
	ldrh	r3, [r4, #0]
	movs	r0, #160
	lsls	r3, r3, #16
	lsls	r0, r0, #19
	asrs	r7, r3, #16
	ldrh	r3, [r0, #0]
	lsls	r3, r3, #16
	asrs	r3, r3, #16
	mov	r8, r3
	ldrb	r3, [r1, #0]
	cmp	r3, #1
	bne.n	.L26
	strh	r2, [r4, #0]
	ldr	r3, [pc, #120]
	strh	r3, [r0, #0]
	movs	r1, #0
	ldr	r0, [pc, #116]
	ldr	r5, [pc, #120]
	movs	r4, #1
.L28:
	ldrh	r2, [r0, #0]
	adds	r3, r5, #0
	ands	r3, r2
	strh	r3, [r0, #0]
.L27:
	svc	2
	ldrh	r2, [r0, #0]
	adds	r3, r4, #0
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L27
	adds	r1, #1
	cmp	r1, #59
	ble.n	.L28
	ldr	r6, [pc, #92]
	ldr	r3, [pc, #52]
	strh	r3, [r6, #0]
	movs	r3, #195
	ldr	r5, [pc, #88]
	lsls	r3, r3, #8
	strh	r3, [r5, #0]
	bl	Func_08006868
	svc	3
	bl	Func_08006870
	ldr	r3, [pc, #76]
	strh	r3, [r5, #0]
	ldr	r3, [pc, #28]
	strh	r3, [r6, #0]
	lsls	r3, r7, #16
	movs	r2, #128
	lsrs	r3, r3, #16
	lsls	r2, r2, #19
	strh	r3, [r2, #0]
	mov	r1, r8
	lsls	r3, r1, #16
	movs	r2, #160
	lsrs	r3, r3, #16
	lsls	r2, r2, #19
	strh	r3, [r2, #0]
	b.n	.L29
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x03001c9c
	.4byte 0x03001cb0
	.4byte 0x02002240
	.4byte 0x03001cc8
	.4byte 0x03001ca0
	.4byte 0x00007fff
	.4byte 0x03001d28
	.4byte 0x0000fffe
	.4byte 0x02002000
	.4byte 0x04000132
	.4byte 0x0000c00f
.L29:
	movs	r1, #0
	ldr	r0, [pc, #48]
	ldr	r5, [pc, #48]
	movs	r4, #1
.L31:
	ldrh	r2, [r0, #0]
	adds	r3, r5, #0
	ands	r3, r2
	strh	r3, [r0, #0]
.L30:
	svc	2
	ldrh	r2, [r0, #0]
	adds	r3, r4, #0
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L30
	adds	r1, #1
	cmp	r1, #9
	ble.n	.L31
	ldr	r3, [pc, #24]
	movs	r2, #0
	strb	r2, [r3, #0]
	ldr	r3, [pc, #4]
	mov	r2, r9
	strh	r3, [r2, #0]
	b.n	.L24
	.4byte 0x00000000
	.4byte 0x03001d28
	.4byte 0x0000fffe
	.4byte 0x03001cc8
.L26:
	ldrb	r3, [r1, #0]
	adds	r3, #255
	strb	r3, [r1, #0]
.L24:
	ldr	r2, [pc, #124]
	ldrb	r3, [r2, #0]
	cmp	r3, #0
	beq.n	.L32
	movs	r3, #0
	strb	r3, [r2, #0]
	movs	r0, #128
	ldr	r2, [pc, #112]
	ldr	r3, [pc, #116]
	lsls	r0, r0, #20
	str	r3, [r2, #0]
	movs	r2, #0
	ldr	r3, [pc, #112]
	strh	r2, [r3, #0]
	bl	Func_080072e4
.L32:
	ldr	r3, [sp, #0]
	ldr	r1, [sp, #4]
	adds	r3, #1
	str	r3, [sp, #0]
	cmp	r3, r1
	bcs.n	.L2
	b.n	.L33
.L2:
	ldr	r2, [pc, #92]
	ldr	r3, [r2, #0]
	cmp	r3, #0
	beq.n	.L34
	mov	sl, sp
	ldr	r3, [r2, #0]
	mov	r2, sl
	subs	r2, r2, r3
	mov	sl, r2
	mov	sp, sl
	ldr	r3, [pc, #72]
	ldr	r2, [r3, #0]
	movs	r4, #132
	lsls	r4, r4, #24
	lsrs	r2, r2, #2
	ldr	r3, [pc, #68]
	ldr	r0, [pc, #68]
	mov	r1, sl
	orrs	r2, r4
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r3, #128
	ldr	r1, [pc, #52]
	lsls	r3, r3, #24
.L35:
	ldr	r2, [r1, #8]
	ands	r2, r3
	cmp	r2, #0
	bne.n	.L35
	ldr	r3, [pc, #36]
	str	r2, [r3, #0]
.L34:
	add	sp, #8
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001cb8
	.4byte 0x03007800
	.4byte 0x19670704
	.4byte 0x04000208
	.4byte 0x03001804
	.4byte 0x040000d4
	.4byte 0x020023b0
