@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080d1714
	.thumb_func
Func_080d1714:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r5, [pc, #672]
	ldr	r1, [r5, #0]
	sub	sp, #356
	str	r1, [sp, #100]
	adds	r3, r5, #0
	subs	r3, #112
	ldr	r3, [r3, #0]
	str	r3, [sp, #96]
	subs	r3, r5, #4
	ldr	r3, [r3, #0]
	str	r3, [sp, #92]
	ldr	r4, [pc, #652]
	ldr	r2, [r5, #4]
	adds	r3, r3, r4
	str	r2, [sp, #76]
	str	r0, [r3, #0]
	movs	r0, #0
	mov	r8, r3
	bl	Func_080cd594
	movs	r6, #2
	movs	r1, #7
	movs	r2, #7
	movs	r3, #3
	movs	r0, #46
	str	r6, [sp, #0]
	bl	Func_080ed408
	ldr	r7, [r5, #24]
	movs	r3, #3
	movs	r1, #7
	movs	r2, #7
	movs	r0, #47
	str	r7, [sp, #84]
	str	r3, [sp, #0]
	bl	Func_080ed408
	ldr	r5, [r5, #28]
	ldr	r0, [pc, #604]
	str	r5, [sp, #88]
	bl	Func_08002f40
	adds	r5, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #596]
	movs	r2, #128
	adds	r1, r5, #0
	lsls	r0, r0, #19
	bl	Func_080072f0
	adds	r5, #128
	ldr	r1, [sp, #92]
	adds	r0, r5, #0
	bl	Func_08005340
	ldr	r0, [pc, #576]
	bl	Func_08002f40
	ldr	r1, [sp, #76]
	bl	Func_08005340
	movs	r1, #239
	ldr	r0, [sp, #92]
	lsls	r1, r1, #7
	adds	r3, r0, r1
	str	r6, [r3, #0]
	ldr	r3, [pc, #560]
	movs	r1, #144
	adds	r2, r0, r3
	movs	r3, #50
	str	r3, [r2, #0]
	lsls	r1, r1, #3
	ldr	r0, [pc, #552]
	bl	Func_080041d8
	mov	r1, r8
	ldr	r3, [r1, #0]
	movs	r4, #128
	movs	r5, #160
	ldr	r3, [r3, #20]
	movs	r0, #0
	lsls	r4, r4, #16
	lsls	r5, r5, #14
	movs	r7, #0
	str	r4, [sp, #68]
	str	r5, [sp, #72]
	str	r7, [sp, #60]
	str	r0, [sp, #64]
	mov	r9, r0
	cmp	r3, #0
	beq.n	.L0
	mov	r2, sp
	movs	r3, #162
	movs	r4, #146
	ldr	r5, [sp, #92]
	ldr	r0, [pc, #484]
	adds	r2, #164
	lsls	r3, r3, #1
	lsls	r4, r4, #1
	add	r4, sp
	add	r3, sp
	adds	r5, r5, r0
	str	r2, [sp, #40]
	mov	r8, r4
	mov	fp, r3
	add	r7, sp, #260
	mov	sl, r5
	movs	r4, #0
	add	r6, sp, #196
.L1:
	mov	r1, sl
	mov	r5, r9
	ldr	r2, [r1, #0]
	lsls	r3, r5, #1
	adds	r3, #36
	ldrsh	r0, [r2, r3]
	str	r4, [sp, #8]
	bl	Func_080b5098
	ldr	r5, [r0, #0]
	ldr	r3, [r5, #8]
	str	r3, [r6, #0]
	ldr	r3, [r5, #16]
	str	r3, [r6, #4]
	ldr	r4, [sp, #8]
	ldrh	r3, [r5, #6]
	ldr	r2, [sp, #40]
	str	r3, [r4, r2]
	ldr	r1, [r5, #16]
	ldr	r0, [r5, #8]
	bl	Func_080044d0
	ldr	r4, [sp, #8]
	lsls	r0, r0, #16
	lsrs	r0, r0, #16
	mov	r3, fp
	str	r0, [r4, r3]
	ldr	r3, [r5, #8]
	asrs	r3, r3, #8
	adds	r0, r3, #0
	muls	r0, r3
	ldr	r3, [r5, #16]
	asrs	r3, r3, #8
	adds	r1, r3, #0
	muls	r1, r3
	adds	r3, r1, #0
	adds	r0, r0, r3
	ldr	r3, [pc, #408]
	bl	Func_080072f0
	ldr	r4, [sp, #8]
	movs	r3, #0
	asrs	r0, r0, #7
	mov	r2, r8
	str	r0, [r4, r2]
	str	r3, [r4, r7]
	str	r3, [r5, #72]
	movs	r3, #1
	mov	r5, sl
	add	r9, r3
	ldr	r3, [r5, #0]
	ldr	r3, [r3, #20]
	adds	r4, #4
	adds	r6, #8
	cmp	r9, r3
	bne.n	.L1
.L0:
	ldr	r0, [sp, #92]
	movs	r1, #225
	movs	r7, #0
	lsls	r1, r1, #7
	mov	r9, r7
	movs	r6, #0
	adds	r5, r0, r1
.L2:
	movs	r3, #120
	str	r3, [r5, #8]
	str	r6, [r5, #4]
	bl	Func_08004458
	str	r6, [r5, #16]
	str	r6, [r5, #12]
	bl	Func_08004458
	movs	r3, #63
	movs	r2, #1
	ands	r3, r0
	add	r9, r2
	str	r3, [r5, #24]
	mov	r3, r9
	adds	r5, #28
	cmp	r3, #64
	bne.n	.L2
	ldr	r5, [sp, #96]
	movs	r4, #0
	adds	r5, #12
	str	r4, [sp, #80]
	str	r5, [sp, #36]
.L81:
	ldr	r3, [pc, #312]
	ldr	r3, [r3, #0]
	movs	r2, #3
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L3
	ldr	r7, [sp, #80]
	cmp	r7, #159
	bgt.n	.L4
	movs	r0, #160
	str	r0, [sp, #80]
	b.n	.L3
.L4:
	movs	r2, #197
	ldr	r1, [sp, #80]
	lsls	r2, r2, #1
	cmp	r1, r2
	bgt.n	.L3
	ldr	r3, [pc, #284]
	str	r3, [sp, #80]
.L3:
	bl	Func_080049ac
	ldr	r0, [sp, #96]
	ldr	r1, [sp, #36]
	bl	Func_080051d8
	ldr	r4, [sp, #80]
	cmp	r4, #16
	bne.n	.L5
	movs	r0, #141
	bl	Func_080f9010
.L5:
	movs	r7, #128
	ldr	r5, [sp, #80]
	lsls	r7, r7, #1
	cmp	r5, r7
	bne.n	.L6
	movs	r0, #140
	bl	Func_080f9010
.L6:
	movs	r1, #167
	ldr	r0, [sp, #80]
	lsls	r1, r1, #1
	cmp	r0, r1
	bne.n	.L7
	movs	r0, #212
	bl	Func_080f9010
.L7:
	ldr	r2, [sp, #80]
	ldr	r3, [pc, #224]
	cmp	r2, r3
	bne.n	.L8
	movs	r0, #212
	bl	Func_080f9010
.L8:
	ldr	r4, [sp, #80]
	ldr	r5, [pc, #216]
	cmp	r4, r5
	bne.n	.L9
	movs	r0, #212
	bl	Func_080f9010
.L9:
	movs	r0, #186
	ldr	r7, [sp, #80]
	lsls	r0, r0, #1
	cmp	r7, r0
	bne.n	.L10
	movs	r0, #212
	bl	Func_080f9010
.L10:
	ldr	r2, [pc, #148]
	ldr	r4, [sp, #92]
	ldr	r3, [r4, r2]
	ldr	r3, [r3, #20]
	movs	r1, #0
	mov	r9, r1
	cmp	r3, #0
	bne.n	.L11
	b.n	.L12
.L11:
	movs	r4, #0
.L25:
	mov	r5, r9
	ldr	r0, [sp, #80]
	lsls	r7, r5, #4
	cmp	r0, r7
	bgt.n	.L13
	b.n	.L14
.L13:
	ldr	r1, [sp, #92]
	lsls	r5, r5, #1
	ldr	r2, [r1, r2]
	adds	r3, r5, #0
	str	r5, [sp, #56]
	adds	r3, #36
	ldrsh	r0, [r2, r3]
	str	r4, [sp, #8]
	bl	Func_080b5098
	ldr	r4, [sp, #8]
	add	r6, sp, #324
	ldr	r5, [r0, #0]
	ldr	r0, [r6, r4]
	mov	sl, r4
	bl	Func_08002322
	movs	r1, #146
	ldr	r4, [sp, #8]
	lsls	r1, r1, #1
	add	r1, sp
	ldr	r3, [r1, r4]
	muls	r3, r0
	asrs	r3, r3, #1
	str	r3, [r5, #8]
	ldr	r0, [r6, r4]
	mov	r8, r1
	bl	Func_0800231c
	ldr	r4, [sp, #8]
	mov	r2, r8
	ldr	r3, [r2, r4]
	muls	r3, r0
	asrs	r3, r3, #1
	str	r3, [r5, #16]
	ldr	r3, [sp, #80]
	cmp	r3, #159
	bgt.n	.L15
	adds	r3, r7, #0
	ldr	r7, [sp, #80]
	adds	r3, #16
	cmp	r7, r3
	ble.n	.L16
	add	r7, sp, #260
	ldr	r3, [r7, r4]
	adds	r3, #48
	str	r3, [r7, r4]
.L16:
	mov	r0, r8
	ldr	r3, [r0, r4]
	cmp	r3, #31
	bgt.n	.L17
	ldr	r3, [r5, #12]
	movs	r1, #192
	lsls	r1, r1, #11
	adds	r3, r3, r1
	b.n	.L18
	movs	r0, r0
	.4byte 0x03001ef0
	.4byte 0x00007828
	.4byte 0x00000082
	.4byte 0x03001388
	.4byte 0x00000073
	.4byte 0x00007784
	.4byte 0x080cd261
	.4byte 0x030001d8
	.4byte 0x03001b04
	.4byte 0x0000018b
	.4byte 0x0000015b
	.4byte 0x00000167
.L17:
	ldr	r3, [r5, #12]
	movs	r2, #128
	lsls	r2, r2, #8
	adds	r3, r3, r2
.L18:
	str	r3, [r5, #12]
	movs	r2, #248
	ldr	r3, [r5, #12]
	lsls	r2, r2, #15
	cmp	r3, r2
	ble.n	.L15
	str	r2, [r5, #12]
.L15:
	movs	r7, #245
	ldr	r3, [sp, #80]
	lsls	r7, r7, #1
	cmp	r3, r7
	bgt.n	.L19
	add	r7, sp, #260
	ldr	r2, [r6, r4]
	ldr	r3, [r7, r4]
	movs	r0, #128
	adds	r2, r2, r3
	lsls	r0, r0, #9
	str	r2, [r6, r4]
	cmp	r2, r0
	ble.n	.L20
	ldr	r1, [pc, #576]
	adds	r3, r2, r1
	mov	r2, sl
	str	r3, [r6, r2]
.L20:
	mov	r3, sl
	ldr	r2, [r7, r3]
	cmp	r2, #0
	bge.n	.L21
	adds	r2, #3
.L21:
	ldrh	r3, [r5, #6]
	asrs	r2, r2, #2
	adds	r3, r3, r2
	strh	r3, [r5, #6]
.L19:
	ldr	r7, [sp, #80]
	ldr	r0, [pc, #552]
	cmp	r7, r0
	bne.n	.L22
	ldr	r3, [pc, #548]
	str	r3, [r5, #72]
.L22:
	movs	r1, #0
	ldr	r6, [pc, #548]
	mov	fp, r1
.L24:
	ldrh	r3, [r6, #0]
	ldr	r2, [sp, #80]
	adds	r6, #2
	cmp	r2, r3
	bne.n	.L23
	movs	r3, #0
	str	r3, [r5, #40]
	ldr	r7, [sp, #92]
	ldr	r0, [pc, #532]
	adds	r3, r7, r0
	ldr	r2, [r3, #0]
	ldr	r3, [sp, #56]
	adds	r3, #36
	ldrsh	r0, [r2, r3]
	movs	r3, #8
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r2, #5
	mov	r3, r9
	str	r4, [sp, #8]
	bl	Func_080d6888
	ldr	r4, [sp, #8]
.L23:
	movs	r2, #1
	add	fp, r2
	mov	r3, fp
	cmp	r3, #4
	bne.n	.L24
	add	r5, sp, #292
	ldr	r3, [r5, r4]
	cmp	r3, #16
	ble.n	.L14
	subs	r3, #2
	mov	r7, sl
	str	r3, [r5, r7]
.L14:
	ldr	r2, [pc, #476]
	ldr	r1, [sp, #92]
	ldr	r3, [r1, r2]
	movs	r0, #1
	ldr	r3, [r3, #20]
	add	r9, r0
	adds	r4, #4
	cmp	r9, r3
	beq.n	.L12
	b.n	.L25
.L12:
	ldr	r3, [sp, #80]
	subs	r3, #16
	cmp	r3, #143
	bhi.n	.L26
	ldr	r2, [sp, #80]
	lsls	r3, r2, #1
	adds	r5, r3, #0
	subs	r5, #32
	cmp	r5, #48
	ble.n	.L27
	movs	r5, #48
.L27:
	ldr	r0, [sp, #80]
	cmp	r0, #0
	bge.n	.L28
	adds	r0, #3
.L28:
	movs	r1, #3
	asrs	r0, r0, #2
	bl	Func_080022fc
	movs	r3, #48
	subs	r2, r3, r5
	lsls	r1, r0, #1
	lsls	r3, r2, #1
	adds	r1, r1, r0
	adds	r3, r3, r2
	lsls	r3, r3, #4
	ldr	r4, [sp, #92]
	lsls	r1, r1, #10
	adds	r1, r1, r3
	movs	r7, #48
	movs	r3, #112
	adds	r1, r4, r1
	subs	r3, r3, r5
	str	r7, [sp, #0]
	str	r5, [sp, #4]
	ldr	r0, [sp, #100]
	movs	r2, #32
	ldr	r4, [sp, #84]
	bl	Func_080072f4
.L26:
	ldr	r3, [sp, #80]
	subs	r3, #48
	cmp	r3, #111
	bhi.n	.L29
	ldr	r5, [sp, #80]
	lsls	r3, r5, #1
	adds	r5, r3, #0
	subs	r5, #96
	cmp	r5, #64
	ble.n	.L30
	movs	r5, #64
.L30:
	ldr	r0, [sp, #80]
	cmp	r0, #0
	bge.n	.L31
	adds	r0, #3
.L31:
	movs	r1, #3
	asrs	r0, r0, #2
	bl	Func_080022fc
	movs	r3, #64
	subs	r3, r3, r5
	lsls	r1, r0, #1
	lsls	r2, r3, #1
	adds	r1, r1, r0
	adds	r2, r2, r3
	lsls	r2, r2, #4
	lsls	r1, r1, #10
	ldr	r7, [sp, #92]
	adds	r1, r1, r2
	movs	r0, #48
	str	r0, [sp, #0]
	adds	r1, r7, r1
	str	r5, [sp, #4]
	ldr	r0, [sp, #100]
	movs	r2, #32
	ldr	r4, [sp, #84]
	bl	Func_080072f4
.L29:
	ldr	r5, [sp, #80]
	subs	r5, #160
	mov	sl, r5
	cmp	r5, #239
	bhi.n	.L32
	ldr	r0, [sp, #80]
	cmp	r0, #0
	bge.n	.L33
	adds	r0, #3
.L33:
	movs	r1, #3
	asrs	r0, r0, #2
	bl	Func_080022fc
	lsls	r5, r0, #1
	adds	r5, r5, r0
	ldr	r7, [sp, #92]
	ldr	r4, [sp, #84]
	lsls	r5, r5, #10
	adds	r5, r7, r5
	movs	r0, #48
	movs	r6, #64
	str	r0, [sp, #0]
	adds	r1, r5, #0
	movs	r2, #8
	movs	r3, #0
	str	r4, [sp, #8]
	str	r6, [sp, #4]
	ldr	r0, [sp, #100]
	bl	Func_080072f4
	movs	r1, #48
	str	r1, [sp, #0]
	str	r6, [sp, #4]
	ldr	r0, [sp, #100]
	adds	r1, r5, #0
	movs	r2, #8
	movs	r3, #64
	ldr	r4, [sp, #8]
	bl	Func_080072f4
.L32:
	ldr	r2, [sp, #80]
	cmp	r2, #159
	bgt.n	.L34
	movs	r4, #152
	ldr	r5, [sp, #92]
	movs	r7, #225
	movs	r3, #0
	add	r4, sp
	lsls	r7, r7, #7
	mov	r9, r3
	mov	r8, r4
	adds	r6, r5, r7
.L41:
	ldr	r4, [r6, #24]
	cmp	r4, #0
	bne.n	.L35
	ldr	r0, [r6, #16]
	str	r4, [sp, #8]
	bl	Func_08002322
	ldr	r3, [r6, #8]
	muls	r3, r0
	mov	r5, r8
	str	r3, [r5, #0]
	ldr	r3, [r6, #4]
	str	r3, [r5, #4]
	ldr	r0, [r6, #16]
	bl	Func_0800231c
	ldr	r3, [r6, #8]
	muls	r3, r0
	add	r7, sp, #140
	str	r3, [r5, #8]
	adds	r0, r5, #0
	adds	r1, r7, #0
	bl	Func_080e3944
	ldr	r3, [r7, #0]
	asrs	r2, r3, #1
	str	r2, [r7, #0]
	ldr	r0, [pc, #148]
	ldr	r3, [r6, #4]
	ldr	r4, [sp, #8]
	cmp	r3, r0
	bgt.n	.L36
	ldr	r3, [r7, #4]
	ldr	r5, [sp, #92]
	movs	r7, #144
	movs	r1, #16
	lsls	r7, r7, #6
	str	r1, [sp, #0]
	str	r1, [sp, #4]
	subs	r2, #12
	adds	r1, r5, r7
	subs	r3, #12
	ldr	r0, [sp, #100]
	ldr	r5, [sp, #84]
	bl	Func_080072f8
	ldr	r4, [sp, #8]
.L36:
	ldr	r3, [r6, #8]
	cmp	r3, #24
	ble.n	.L37
	subs	r3, #4
	str	r3, [r6, #8]
.L37:
	ldr	r3, [r6, #12]
	ldr	r2, [r6, #16]
	lsls	r3, r3, #1
	movs	r7, #128
	adds	r2, r2, r3
	lsls	r7, r7, #9
	str	r2, [r6, #16]
	cmp	r2, r7
	ble.n	.L38
	ldr	r0, [pc, #60]
	adds	r3, r2, r0
	str	r3, [r6, #16]
.L38:
	ldr	r3, [r6, #12]
	movs	r2, #128
	adds	r3, #50
	lsls	r2, r2, #5
	str	r3, [r6, #12]
	cmp	r3, r2
	ble.n	.L39
	str	r2, [r6, #12]
.L39:
	ldr	r3, [r6, #16]
	movs	r1, #128
	lsls	r1, r1, #4
	adds	r3, r3, r1
	ldr	r2, [r6, #4]
	str	r3, [r6, #16]
	lsls	r3, r3, #1
	adds	r2, r2, r3
	movs	r3, #192
	lsls	r3, r3, #14
	str	r2, [r6, #4]
	cmp	r2, r3
	ble.n	.L40
	movs	r3, #100
	str	r4, [r6, #4]
	str	r3, [r6, #8]
	str	r4, [r6, #16]
	str	r4, [r6, #12]
	b.n	.L40
	movs	r0, r0
	.4byte 0xffff0000
	.4byte 0x0000018b
	.4byte 0x0000ab85
	.4byte 0x080ee16c
	.4byte 0x00007828
	.4byte 0x003fffff
.L35:
	subs	r3, r4, #1
	str	r3, [r6, #24]
.L40:
	movs	r4, #1
	add	r9, r4
	mov	r5, r9
	adds	r6, #28
	cmp	r5, #24
	bne.n	.L41
.L34:
	ldr	r7, [sp, #80]
	cmp	r7, #160
	beq.n	.L42
	b.n	.L43
.L42:
	ldr	r5, [pc, #772]
	movs	r0, #1
	adds	r1, r5, #0
	movs	r2, #0
	bl	Func_080b5040
	adds	r1, r5, #0
	movs	r2, #8
	movs	r0, #1
	bl	Func_080b5038
	ldr	r1, [pc, #756]
	ldr	r0, [sp, #92]
	adds	r3, r0, r1
	ldr	r0, [r3, #0]
	bl	Func_080d6750
	movs	r1, #188
	lsls	r1, r1, #1
	movs	r2, #2
	movs	r0, #9
	bl	Func_080dbb24
	ldr	r0, [pc, #736]
	bl	Func_08002f40
	adds	r5, r0, #0
	movs	r0, #160
	adds	r1, r5, #0
	ldr	r3, [pc, #728]
	movs	r2, #128
	lsls	r0, r0, #19
	bl	Func_080072f0
	movs	r3, #216
	ldr	r2, [sp, #92]
	lsls	r3, r3, #6
	adds	r5, #128
	adds	r1, r2, r3
	adds	r0, r5, #0
	bl	Func_08005340
	movs	r4, #0
	movs	r2, #128
	ldr	r3, [pc, #700]
	mov	r9, r4
	movs	r1, #0
	lsls	r2, r2, #2
.L44:
	movs	r5, #1
	add	r9, r5
	str	r1, [r3, #0]
	adds	r3, #28
	cmp	r9, r2
	bne.n	.L44
	ldr	r3, [pc, #668]
	ldr	r0, [sp, #92]
	ldr	r3, [r0, r3]
	ldr	r3, [r3, #20]
	movs	r7, #0
	mov	r9, r7
	cmp	r3, #0
	beq.n	.L45
	ldr	r1, [pc, #652]
	movs	r7, #36
	adds	r6, r0, r1
.L46:
	ldr	r3, [r6, #0]
	ldrsh	r0, [r3, r7]
	bl	Func_080b5098
	ldr	r5, [r0, #0]
	bl	Func_08004458
	movs	r3, #15
	ands	r3, r0
	adds	r3, #8
	lsls	r3, r3, #16
	str	r3, [r5, #12]
	movs	r3, #1
	add	r9, r3
	ldr	r3, [r6, #0]
	ldr	r3, [r3, #20]
	adds	r7, #2
	cmp	r9, r3
	bne.n	.L46
.L45:
	ldr	r2, [pc, #624]
	movs	r3, #72
	str	r3, [r2, #12]
	ldr	r5, [sp, #92]
	movs	r0, #225
	movs	r4, #0
	lsls	r0, r0, #7
	mov	r9, r4
	adds	r7, r5, r0
.L47:
	bl	Func_08004458
	movs	r3, #127
	adds	r6, r0, #0
	ands	r6, r3
	bl	Func_08004458
	ldr	r5, [pc, #596]
	ldr	r1, [pc, #600]
	ands	r5, r0
	adds	r5, r5, r1
	adds	r0, r5, #0
	bl	Func_08002322
	adds	r3, r6, #0
	muls	r3, r0
	adds	r0, r5, #0
	str	r3, [r7, #0]
	bl	Func_0800231c
	adds	r3, r6, #0
	muls	r3, r0
	str	r3, [r7, #4]
	bl	Func_08004458
	movs	r1, #200
	bl	Func_08002304
	movs	r2, #1
	movs	r3, #0
	subs	r0, #100
	add	r9, r2
	str	r3, [r7, #24]
	lsls	r0, r0, #16
	mov	r3, r9
	str	r0, [r7, #8]
	adds	r7, #28
	cmp	r3, #32
	bne.n	.L47
	ldr	r3, [pc, #512]
	ldr	r5, [sp, #92]
	ldr	r3, [r5, r3]
	ldr	r3, [r3, #20]
	movs	r4, #0
	mov	r9, r4
	cmp	r3, #0
	beq.n	.L43
	ldr	r1, [pc, #496]
	ldr	r7, [sp, #92]
	adds	r3, r7, r1
	ldr	r5, [pc, #520]
	ldr	r1, [r3, #0]
	add	r4, sp, #292
	movs	r6, #16
	add	r0, sp, #260
	movs	r2, #0
.L48:
	str	r6, [r2, r4]
	str	r5, [r2, r0]
	movs	r3, #1
	add	r9, r3
	ldr	r3, [r1, #20]
	adds	r2, #4
	cmp	r9, r3
	bne.n	.L48
.L43:
	ldr	r4, [sp, #80]
	cmp	r4, #159
	bgt.n	.L49
	b.n	.L50
.L49:
	ldr	r3, [pc, #488]
	ldr	r4, [r3, #4]
	ldr	r3, [r3, #0]
	mov	r7, sl
	str	r3, [sp, #104]
	str	r4, [sp, #108]
	movs	r5, #16
	cmp	r7, #64
	bgt.n	.L51
	movs	r5, #32
.L51:
	mov	r1, sl
	lsls	r0, r1, #7
	bl	Func_08002322
	ldr	r2, [sp, #68]
	lsls	r0, r0, #5
	asrs	r0, r0, #6
	subs	r0, r2, r0
	mov	r3, sl
	str	r0, [sp, #68]
	lsls	r0, r3, #9
	bl	Func_08002322
	adds	r3, r5, #0
	muls	r3, r0
	ldr	r4, [sp, #72]
	ldr	r2, [sp, #60]
	asrs	r3, r3, #6
	adds	r3, r4, r3
	str	r3, [sp, #72]
	adds	r1, r3, #0
	lsrs	r3, r2, #31
	ldr	r4, [sp, #64]
	adds	r3, r3, r2
	asrs	r3, r3, #1
	ldr	r5, [sp, #60]
	str	r3, [sp, #60]
	lsrs	r3, r4, #31
	adds	r3, r3, r4
	asrs	r3, r3, #1
	ldr	r0, [sp, #64]
	str	r3, [sp, #64]
	movs	r3, #128
	lsls	r3, r3, #9
	add	r4, sp, #104
	ldr	r7, [sp, #68]
	str	r3, [sp, #104]
	add	r2, sp, #112
	str	r3, [r4, #4]
	movs	r3, #0
	adds	r0, r0, r1
	str	r3, [r2, #12]
	adds	r5, r5, r7
	str	r0, [sp, #72]
	ldr	r7, [sp, #92]
	ldr	r0, [pc, #380]
	str	r5, [sp, #68]
	adds	r5, r7, r0
	movs	r7, #255
	mov	r9, r3
	adds	r6, r2, #0
	lsls	r7, r7, #16
.L52:
	ldr	r3, [pc, #372]
	mov	r1, r9
	ldrb	r3, [r3, r1]
	ldr	r2, [sp, #68]
	lsls	r3, r3, #16
	movs	r0, #160
	adds	r3, r3, r2
	lsls	r0, r0, #14
	adds	r3, r3, r0
	str	r3, [r6, #0]
	ldr	r3, [pc, #356]
	ldrb	r3, [r3, r1]
	lsls	r2, r1, #6
	adds	r3, r3, r2
	ldr	r1, [sp, #72]
	lsls	r3, r3, #16
	adds	r3, r3, r1
	adds	r2, r4, #0
	str	r7, [r6, #4]
	str	r3, [r6, #8]
	ldmia	r5!, {r0}
	movs	r3, #0
	adds	r1, r6, #0
	str	r4, [sp, #8]
	bl	Func_08009008
	movs	r3, #1
	movs	r2, #128
	add	r9, r3
	lsls	r2, r2, #15
	mov	r0, r9
	adds	r7, r7, r2
	ldr	r4, [sp, #8]
	cmp	r0, #9
	bne.n	.L52
	ldr	r1, [sp, #80]
	cmp	r1, #255
	bgt.n	.L53
	b.n	.L50
.L53:
	movs	r3, #128
	add	r5, sp, #128
	movs	r2, #0
	lsls	r3, r3, #17
	str	r3, [r5, #8]
	str	r2, [r5, #0]
	str	r2, [r5, #4]
	mov	sl, r2
	bl	Func_080049ac
	adds	r0, r5, #0
	bl	Func_08004cb4
	ldr	r4, [pc, #276]
	ldr	r3, [sp, #80]
	cmp	r3, r4
	ble.n	.L54
	b.n	.L55
.L54:
	ldr	r7, [sp, #92]
	movs	r0, #225
	movs	r5, #0
	lsls	r0, r0, #7
	mov	r9, r5
	adds	r6, r7, r0
.L61:
	ldr	r3, [r6, #0]
	asrs	r3, r3, #8
	adds	r0, r3, #0
	muls	r0, r3
	ldr	r3, [r6, #4]
	asrs	r3, r3, #8
	adds	r2, r3, #0
	muls	r2, r3
	ldr	r3, [r6, #8]
	asrs	r3, r3, #8
	adds	r1, r3, #0
	muls	r1, r3
	adds	r0, r0, r2
	adds	r3, r1, #0
	adds	r0, r0, r3
	ldr	r3, [pc, #228]
	bl	Func_080072f0
	asrs	r0, r0, #9
	mov	r8, r0
	cmp	r0, #0
	beq.n	.L56
	add	r7, sp, #140
	adds	r1, r7, #0
	adds	r0, r6, #0
	bl	Func_080e3944
	ldr	r3, [r7, #0]
	ldr	r4, [sp, #68]
	asrs	r3, r3, #17
	asrs	r2, r4, #17
	adds	r3, r3, r2
	adds	r3, #32
	ldr	r0, [sp, #72]
	str	r3, [r7, #0]
	movs	r5, #6
	ldrsh	r3, [r7, r5]
	asrs	r2, r0, #16
	adds	r3, r3, r2
	subs	r3, #4
	str	r3, [r7, #4]
	movs	r1, #10
	ldrsh	r3, [r7, r1]
	str	r3, [r7, #8]
	cmp	r3, #169
	bgt.n	.L57
	movs	r3, #170
	str	r3, [r7, #8]
.L57:
	movs	r3, #175
	ldr	r0, [r7, #8]
	lsls	r3, r3, #1
	cmp	r0, r3
	ble.n	.L58
	str	r3, [r7, #8]
	adds	r0, r3, #0
.L58:
	movs	r1, #36
	subs	r0, #170
	bl	Func_080022ec
	movs	r3, #6
	subs	r4, r3, r0
	lsls	r0, r4, #1
	ldr	r2, [pc, #136]
	subs	r3, r0, #2
	ldrh	r1, [r2, r3]
	ldr	r2, [sp, #76]
	lsrs	r3, r4, #31
	adds	r1, r2, r1
	adds	r3, r4, r3
	ldr	r2, [r7, #0]
	asrs	r3, r3, #1
	subs	r2, r2, r3
	ldr	r3, [r7, #4]
	str	r4, [sp, #0]
	subs	r3, r3, r4
	str	r0, [sp, #4]
	ldr	r4, [sp, #88]
	ldr	r0, [sp, #100]
	bl	Func_080072f4
	ldr	r5, [r6, #0]
	mov	r1, r8
	adds	r0, r5, #0
	bl	Func_080022ec
	subs	r5, r5, r0
	str	r5, [r6, #0]
	ldr	r5, [r6, #4]
	mov	r1, r8
	adds	r0, r5, #0
	bl	Func_080022ec
	subs	r5, r5, r0
	str	r5, [r6, #4]
	ldr	r5, [r6, #8]
	mov	r1, r8
	adds	r0, r5, #0
	bl	Func_080022ec
	subs	r5, r5, r0
	str	r5, [r6, #8]
	b.n	.L59
	.4byte 0x0000003b
	.4byte 0x00007828
	.4byte 0x00000088
	.4byte 0x03001388
	.4byte 0x02010018
	.4byte 0x03001ce0
	.4byte 0x00007fff
	.4byte 0x00009fff
	.4byte 0x00001770
	.4byte 0x080eda78
	.4byte 0x000077d8
	.4byte 0x080ee15a
	.4byte 0x080ee163
	.4byte 0x00000149
	.4byte 0x030001d8
	.4byte 0x080ede48
.L56:
	movs	r5, #1
	add	sl, r5
.L59:
	movs	r7, #1
	add	r9, r7
	mov	r0, r9
	adds	r6, #28
	cmp	r0, #32
	beq.n	.L60
	b.n	.L61
.L60:
	mov	r1, sl
	cmp	r1, #0
	ble.n	.L55
	movs	r1, #10
	mov	r0, sl
	bl	Func_080022ec
	adds	r4, r0, #1
	lsls	r0, r4, #1
	ldr	r2, [pc, #928]
	subs	r3, r0, #2
	ldrh	r1, [r2, r3]
	ldr	r2, [sp, #76]
	ldr	r3, [sp, #68]
	adds	r1, r2, r1
	asrs	r2, r3, #17
	lsrs	r3, r4, #31
	adds	r3, r4, r3
	ldr	r5, [sp, #72]
	asrs	r3, r3, #1
	subs	r2, r2, r3
	asrs	r3, r5, #16
	subs	r3, r3, r4
	str	r0, [sp, #4]
	adds	r2, #32
	subs	r3, #4
	str	r4, [sp, #0]
	ldr	r0, [sp, #100]
	ldr	r7, [sp, #88]
	bl	Func_08007300
.L55:
	movs	r0, #0
	mov	fp, r0
	mov	sl, r0
.L70:
	ldr	r2, [pc, #884]
	mov	r1, fp
	lsls	r3, r1, #1
	ldrh	r3, [r2, r3]
	ldr	r4, [sp, #80]
	cmp	r4, r3
	bne.n	.L62
	ldr	r7, [pc, #872]
	movs	r5, #0
	mov	r9, r5
	add	r7, sl
	mov	r8, r5
.L63:
	bl	Func_08004458
	movs	r6, #127
	ands	r6, r0
	bl	Func_08004458
	ldr	r3, [pc, #856]
	adds	r5, r0, #0
	ands	r5, r3
	adds	r0, r5, #0
	bl	Func_08002322
	adds	r6, #16
	adds	r3, r6, #0
	muls	r3, r0
	asrs	r3, r3, #6
	str	r3, [r7, #12]
	adds	r0, r5, #0
	bl	Func_0800231c
	adds	r3, r6, #0
	muls	r3, r0
	negs	r3, r3
	asrs	r3, r3, #6
	str	r3, [r7, #16]
	bl	Func_08004458
	movs	r3, #255
	ands	r3, r0
	subs	r3, #128
	mov	r0, r8
	lsls	r3, r3, #10
	str	r3, [r7, #16]
	str	r0, [r7, #0]
	str	r0, [r7, #4]
	str	r0, [r7, #8]
	bl	Func_08004458
	movs	r3, #15
	movs	r1, #1
	ands	r3, r0
	add	r9, r1
	adds	r3, #64
	mov	r2, r9
	str	r3, [r7, #24]
	adds	r7, #28
	cmp	r2, #96
	bne.n	.L63
	ldr	r2, [pc, #764]
.L62:
	ldrh	r3, [r2, #0]
	ldr	r4, [sp, #80]
	cmp	r4, r3
	blt.n	.L64
	movs	r5, #0
	mov	r9, r5
	ldr	r5, [pc, #752]
.L68:
	ldr	r3, [r5, #24]
	cmp	r3, #0
	ble.n	.L65
	add	r7, sp, #140
	adds	r1, r7, #0
	adds	r0, r5, #0
	bl	Func_080e3944
	ldr	r3, [r7, #0]
	asrs	r3, r3, #17
	adds	r3, #32
	str	r3, [r7, #0]
	movs	r0, #6
	ldrsh	r3, [r7, r0]
	adds	r3, #56
	str	r3, [r7, #4]
	movs	r1, #10
	ldrsh	r3, [r7, r1]
	str	r3, [r7, #8]
	cmp	r3, #169
	bgt.n	.L66
	movs	r3, #170
	str	r3, [r7, #8]
.L66:
	movs	r3, #175
	ldr	r0, [r7, #8]
	lsls	r3, r3, #1
	cmp	r0, r3
	ble.n	.L67
	str	r3, [r7, #8]
	adds	r0, r3, #0
.L67:
	movs	r1, #90
	subs	r0, #170
	bl	Func_080022ec
	movs	r3, #3
	subs	r4, r3, r0
	lsls	r0, r4, #1
	ldr	r2, [pc, #668]
	subs	r3, r0, #2
	ldrh	r1, [r2, r3]
	ldr	r2, [sp, #76]
	lsrs	r3, r4, #31
	adds	r1, r2, r1
	adds	r3, r4, r3
	ldr	r2, [r7, #0]
	asrs	r3, r3, #1
	subs	r2, r2, r3
	ldr	r3, [r7, #4]
	str	r4, [sp, #0]
	subs	r3, r3, r4
	str	r0, [sp, #4]
	ldr	r4, [sp, #84]
	ldr	r0, [sp, #100]
	bl	Func_080072f4
	ldr	r2, [r5, #0]
	ldr	r3, [r5, #12]
	adds	r2, r2, r3
	str	r2, [r5, #0]
	ldr	r3, [r5, #4]
	ldr	r2, [r5, #16]
	adds	r3, r3, r2
	str	r3, [r5, #4]
	ldr	r3, [r5, #8]
	adds	r3, r3, r2
	str	r3, [r5, #8]
	ldr	r3, [r5, #24]
	subs	r3, #1
	str	r3, [r5, #24]
.L65:
	movs	r7, #1
	movs	r0, #128
	add	r9, r7
	lsls	r0, r0, #2
	adds	r5, #28
	cmp	r9, r0
	bne.n	.L68
.L64:
	movs	r2, #1
	movs	r1, #224
	add	fp, r2
	lsls	r1, r1, #4
	mov	r3, fp
	add	sl, r1
	cmp	r3, #4
	beq.n	.L69
	b.n	.L70
.L69:
	movs	r4, #0
	ldr	r3, [pc, #588]
	mov	r9, r4
	str	r4, [sp, #24]
	ldr	r2, [sp, #92]
	ldr	r4, [sp, #68]
	ldr	r0, [pc, #580]
	ldr	r7, [sp, #80]
	ldr	r5, [pc, #580]
	movs	r1, #165
	adds	r6, r2, r3
	asrs	r3, r4, #17
	adds	r0, r7, r0
	lsls	r1, r1, #1
	adds	r3, #32
	str	r5, [sp, #28]
	str	r0, [sp, #20]
	str	r1, [sp, #16]
	str	r3, [sp, #52]
.L79:
	ldr	r5, [sp, #80]
	ldr	r7, [sp, #16]
	cmp	r5, r7
	bne.n	.L71
	ldr	r1, [sp, #72]
	ldr	r0, [sp, #52]
	asrs	r3, r1, #16
	subs	r3, #4
	str	r3, [r6, #4]
	str	r3, [r6, #16]
	str	r0, [r6, #0]
	str	r0, [r6, #12]
	ldr	r4, [sp, #92]
	ldr	r5, [pc, #536]
	movs	r2, #0
	mov	fp, r2
	adds	r3, r4, r5
	movs	r2, #4
.L72:
	movs	r7, #1
	add	fp, r7
	mov	r0, fp
	str	r2, [r3, #0]
	adds	r3, #28
	cmp	r0, #28
	bne.n	.L72
	ldr	r3, [sp, #92]
	ldr	r2, [pc, #516]
	ldr	r4, [pc, #516]
	movs	r1, #128
	lsls	r1, r1, #12
	str	r2, [sp, #64]
	adds	r2, r3, r4
	movs	r3, #8
	str	r1, [sp, #60]
	str	r3, [r2, #0]
.L71:
	ldr	r5, [sp, #80]
	ldr	r7, [sp, #16]
	cmp	r5, r7
	bge.n	.L73
	b.n	.L74
.L73:
	ldr	r0, [sp, #20]
	lsrs	r3, r0, #31
	adds	r3, r0, r3
	asrs	r4, r3, #1
	cmp	r4, #2
	ble.n	.L75
	movs	r4, #2
.L75:
	ldr	r2, [pc, #480]
	lsls	r3, r4, #1
	ldrh	r1, [r2, r3]
	ldr	r3, [pc, #480]
	ldrb	r0, [r3, r4]
	ldr	r3, [r6, #4]
	subs	r3, r3, r0
	ldr	r0, [pc, #476]
	ldr	r2, [sp, #92]
	ldrb	r0, [r0, r4]
	adds	r1, r2, r1
	ldr	r2, [r6, #0]
	str	r0, [sp, #0]
	ldr	r0, [pc, #468]
	ldrb	r0, [r0, r4]
	ldr	r4, [sp, #88]
	str	r0, [sp, #4]
	ldr	r0, [sp, #100]
	bl	Func_080072f4
	ldr	r3, [r6, #0]
	subs	r3, #8
	str	r3, [r6, #0]
	ldr	r3, [r6, #4]
	adds	r3, #2
	str	r3, [r6, #4]
	ldr	r3, [sp, #16]
	ldr	r5, [sp, #80]
	adds	r3, #8
	cmp	r5, r3
	bge.n	.L74
	ldr	r0, [pc, #436]
	bl	Func_08004c6c
	ldr	r0, [pc, #432]
	bl	Func_08004c1c
	add	r0, sp, #152
	ldr	r1, [sp, #24]
	ldr	r2, [sp, #28]
	movs	r7, #0
	mov	r8, r0
	movs	r3, #0
	ldr	r4, [sp, #92]
	movs	r0, #232
	mov	fp, r7
	lsls	r0, r0, #7
	add	r7, sp, #140
	str	r1, [sp, #48]
	str	r2, [sp, #44]
	str	r3, [sp, #32]
	mov	sl, r7
	adds	r5, r4, r0
.L78:
	mov	r1, r8
	movs	r3, #0
	str	r3, [r1, #0]
	ldr	r0, [sp, #32]
	bl	Func_0800231c
	ldr	r3, [r5, #24]
	muls	r3, r0
	mov	r2, r8
	str	r3, [r2, #4]
	ldr	r0, [sp, #32]
	bl	Func_08002322
	ldr	r3, [r5, #24]
	muls	r3, r0
	mov	r4, r8
	str	r3, [r4, #8]
	ldr	r3, [r5, #24]
	adds	r3, #2
	str	r3, [r5, #24]
	mov	r1, sl
	mov	r0, r8
	bl	Func_080e3944
	mov	r0, sl
	ldr	r1, [sp, #48]
	ldr	r4, [pc, #348]
	ldr	r2, [r0, #0]
	ldr	r0, [sp, #92]
	adds	r3, r1, r4
	ldr	r3, [r0, r3]
	asrs	r2, r2, #17
	mov	r1, sl
	adds	r2, r2, r3
	str	r2, [r1, #0]
	ldr	r4, [sp, #44]
	movs	r3, #6
	ldrsh	r2, [r1, r3]
	ldr	r3, [r0, r4]
	adds	r2, r2, r3
	movs	r0, #10
	ldrsh	r3, [r1, r0]
	str	r2, [r1, #4]
	str	r3, [r1, #8]
	cmp	r3, #169
	bgt.n	.L76
	movs	r3, #170
	str	r3, [r7, #8]
.L76:
	movs	r3, #175
	ldr	r0, [r7, #8]
	lsls	r3, r3, #1
	cmp	r0, r3
	ble.n	.L77
	str	r3, [r7, #8]
	adds	r0, r3, #0
.L77:
	movs	r1, #90
	subs	r0, #170
	bl	Func_080022ec
	movs	r3, #3
	subs	r4, r3, r0
	lsls	r0, r4, #1
	ldr	r2, [pc, #212]
	subs	r3, r0, #2
	ldrh	r1, [r2, r3]
	ldr	r2, [sp, #76]
	lsrs	r3, r4, #31
	adds	r1, r2, r1
	adds	r3, r4, r3
	ldr	r2, [r7, #0]
	asrs	r3, r3, #1
	subs	r2, r2, r3
	ldr	r3, [r7, #4]
	str	r4, [sp, #0]
	subs	r3, r3, r4
	str	r0, [sp, #4]
	ldr	r4, [sp, #84]
	ldr	r0, [sp, #100]
	bl	Func_080072f4
	ldr	r1, [pc, #244]
	ldr	r0, [sp, #32]
	movs	r2, #1
	add	fp, r2
	adds	r0, r0, r1
	mov	r3, fp
	str	r0, [sp, #32]
	adds	r5, #28
	cmp	r3, #28
	bne.n	.L78
.L74:
	ldr	r4, [sp, #28]
	ldr	r5, [sp, #24]
	ldr	r7, [sp, #20]
	ldr	r0, [sp, #16]
	movs	r1, #1
	add	r9, r1
	adds	r4, #28
	adds	r5, #28
	subs	r7, #12
	adds	r0, #12
	mov	r2, r9
	str	r4, [sp, #28]
	str	r5, [sp, #24]
	str	r7, [sp, #20]
	str	r0, [sp, #16]
	adds	r6, #28
	cmp	r2, #4
	beq.n	.L50
	b.n	.L79
.L50:
	bl	Func_080cd52c
	ldr	r4, [pc, #184]
	ldr	r3, [sp, #92]
	adds	r2, r3, r4
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	Func_080030f8
	ldr	r5, [sp, #80]
	movs	r7, #200
	adds	r5, #1
	lsls	r7, r7, #1
	str	r5, [sp, #80]
	cmp	r5, r7
	beq.n	.L80
	bl	.L81
.L80:
	movs	r0, #134
	bl	Func_080b50e8
	ldr	r3, [pc, #148]
	ldr	r1, [sp, #92]
	adds	r2, r1, r3
	ldr	r3, [r2, #0]
	ldr	r3, [r3, #20]
	movs	r0, #0
	mov	r9, r0
	cmp	r3, #0
	beq.n	.L82
	adds	r6, r2, #0
	add	r7, sp, #164
	add	r5, sp, #196
	movs	r1, #36
.L83:
	ldr	r3, [r6, #0]
	ldrsh	r0, [r3, r1]
	str	r1, [sp, #12]
	bl	Func_080b5098
	ldr	r3, [r5, #0]
	ldr	r2, [r0, #0]
	str	r3, [r2, #8]
	ldr	r3, [r5, #4]
	str	r3, [r2, #16]
	ldmia	r7!, {r3}
	strh	r3, [r2, #6]
	ldr	r3, [r6, #0]
	ldr	r1, [sp, #12]
	movs	r0, #1
	ldr	r3, [r3, #20]
	add	r9, r0
	adds	r5, #8
	adds	r1, #2
	cmp	r9, r3
	bne.n	.L83
.L82:
	ldr	r2, [pc, #84]
	movs	r3, #120
	str	r3, [r2, #12]
	b.n	.L84
	.4byte 0x080ede48
	.4byte 0x080ee16c
	.4byte 0x02010000
	.4byte 0x0000ffff
	.4byte 0x00007710
	.4byte 0xfffffeb6
	.4byte 0x00007720
	.4byte 0x00007418
	.4byte 0xfffe0000
	.4byte 0x000077a8
	.4byte 0x080ee17e
	.4byte 0x080ee17a
	.4byte 0x080ee174
	.4byte 0x080ee177
	.4byte 0xfffff800
	.4byte 0xfffff000
	.4byte 0x0000771c
	.4byte 0x00000924
	.4byte 0x00007824
	.4byte 0x00007828
	.4byte 0x03001ce0
.L84:
	bl	Func_080d67dc
	ldr	r3, [pc, #64]
	ldr	r2, [sp, #92]
	movs	r1, #0
	mov	r9, r1
	adds	r5, r2, r3
.L85:
	ldmia	r5!, {r0}
	bl	Func_08009038
	movs	r4, #1
	add	r9, r4
	mov	r7, r9
	cmp	r7, #9
	bne.n	.L85
	ldr	r0, [pc, #44]
	bl	Func_08004278
	movs	r0, #47
	bl	Func_08002dd8
	movs	r0, #46
	bl	Func_08002dd8
	bl	Func_080cdbc0
	add	sp, #356
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x000077d8
	.4byte 0x080cd261
