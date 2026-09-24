.syntax unified
	.thumb
	.set sub_080030f8, 0x080030f8
	.set sub_080041d8, 0x080041d8
	.set sub_080072f8, 0x080072f8
	.set sub_080b5028, 0x080b5028
	.set sub_080b5038, 0x080b5038
	.set sub_080cd508, 0x080cd508
	.global BattleFx_OpenCanvasLayer
	.global Func_080cd86c
	.thumb_func
BattleFx_OpenCanvasLayer:
Func_080cd86c:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #100]
	ldr	r2, [r3, #0]
	mov	sl, r2
	adds	r2, r3, #0
	subs	r2, #120
	ldr	r2, [r2, #0]
	mov	r9, r2
	ldr	r2, [r3, #4]
	sub	sp, #4
	str	r2, [sp, #0]
	ldr	r3, [r3, #20]
	mov	fp, r3
	adds	r7, r0, #0
	bl	sub_080cd508
	mov	r2, fp
	movs	r3, #1
	str	r3, [r2, #12]
	movs	r0, #1
	bl	sub_080030f8
	ldr	r3, [pc, #52]
	ldr	r2, [pc, #60]
	ldr	r6, [pc, #60]
	strh	r3, [r2, #0]
	ldr	r5, [pc, #60]
	ldrh	r3, [r5, #0]
	adds	r1, r3, #0
	strh	r5, [r5, #0]
	ldrh	r2, [r6, #0]
	cmp	r2, #31
	bgt.n	.L_080cd8f4
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #2
	adds	r2, #1
	adds	r3, r3, r6
	strh	r2, [r6, #0]
	ldr	r2, [pc, #40]
	adds	r3, #4
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #19
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #10
	str	r2, [r3, #0]
	b.n	.L_080cd8f4
	movs	r0, r0
	.4byte 0x00000000
	.4byte 0x03001eec
	.4byte 0x04000050
	.4byte 0x02002090
	.4byte 0x04000208
	.2byte 0x1741
	.2byte 0x0000
.L_080cd8f4:
	strh	r1, [r5, #0]
	ldr	r2, [pc, #164]
	movs	r3, #0
	mov	r8, r3
	movs	r3, #32
	strh	r3, [r2, #6]
	movs	r0, #1
	bl	sub_080030f8
	movs	r3, #201
	lsls	r3, r3, #3
	add	r3, r9
	ldrh	r1, [r3, #0]
	movs	r0, #1
	movs	r2, #128
	bl	sub_080b5038
	ldr	r2, [pc, #136]
	movs	r3, #24
	add	r2, sl
	str	r3, [r2, #0]
	ldr	r3, [pc, #132]
	mov	r2, r8
	add	r3, sl
	movs	r1, #200
	str	r2, [r3, #0]
	lsls	r1, r1, #4
	ldr	r0, [pc, #124]
	bl	sub_080041d8
	ldrh	r3, [r5, #0]
	adds	r1, r3, #0
	strh	r5, [r5, #0]
	ldrh	r2, [r6, #0]
	cmp	r2, #31
	bgt.n	.L_080cd95a
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #2
	adds	r2, #1
	adds	r3, r3, r6
	strh	r2, [r6, #0]
	ldr	r2, [pc, #96]
	adds	r3, #4
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #19
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #10
	str	r2, [r3, #0]
.L_080cd95a:
	strh	r1, [r5, #0]
	movs	r0, #1
	bl	sub_080030f8
	ldr	r2, [pc, #52]
	ldr	r3, [pc, #72]
	orrs	r7, r2
	strh	r7, [r3, #0]
	ldrh	r3, [r5, #0]
	adds	r1, r3, #0
	strh	r5, [r5, #0]
	ldrh	r2, [r6, #0]
	cmp	r2, #31
	bgt.n	.L_080cd9b4
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #2
	adds	r2, #1
	adds	r3, r3, r6
	strh	r2, [r6, #0]
	ldr	r2, [pc, #40]
	adds	r3, #4
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #19
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #10
	str	r2, [r3, #0]
	b.n	.L_080cd9b4
	movs	r0, r0
	.4byte 0x00000784
	.4byte 0x03001ad0
	.4byte 0x000077b4
	.4byte 0x000077b8
	.4byte 0x080cd4b5
	.4byte 0x00001341
	.2byte 0x000c
	.2byte 0x0400
.L_080cd9b4:
	strh	r1, [r5, #0]
	movs	r0, #0
	movs	r1, #0
	movs	r2, #0
	movs	r3, #100
	bl	sub_080b5028
	mov	r3, r8
	mov	r2, fp
	str	r3, [r2, #12]
	movs	r0, #1
	bl	sub_080030f8
	ldr	r3, [pc, #52]
	ldr	r2, [pc, #68]
	strh	r3, [r2, #0]
	ldr	r3, [pc, #48]
	adds	r2, #2
	strh	r3, [r2, #0]
	ldr	r3, [pc, #64]
	mov	r2, r8
	str	r2, [r3, #0]
	ldr	r2, [pc, #60]
	ldr	r3, [pc, #64]
	str	r3, [r2, #0]
	ldr	r3, [pc, #36]
	subs	r2, #12
	strh	r3, [r2, #0]
	ldr	r3, [pc, #56]
	mov	r2, r8
	strh	r2, [r3, #0]
	adds	r3, #2
	strh	r2, [r3, #0]
	ldr	r2, [pc, #52]
	ldr	r3, [pc, #20]
	ldr	r1, [pc, #24]
	strh	r3, [r2, #0]
	ldr	r3, [pc, #48]
	b.n	.L_080cda34
	movs	r0, r0
	.4byte 0x00003f44
	.4byte 0x0000100e
	.4byte 0x00000080
	.4byte 0x00000100
	.4byte 0x000000f0
	.4byte 0x04000050
	.4byte 0x04000028
	.4byte 0x0400002c
	.4byte 0xfffff000
	.4byte 0x04000022
	.4byte 0x04000026
	.2byte 0x0040
	.2byte 0x0400
.L_080cda34:
	ldr	r2, [pc, #40]
	strh	r1, [r3, #0]
	adds	r3, #4
	strh	r2, [r3, #0]
	subs	r3, #2
	strh	r1, [r3, #0]
	adds	r3, #4
	strh	r2, [r3, #0]
	ldr	r2, [pc, #36]
	ldr	r3, [pc, #28]
	strh	r3, [r2, #0]
	ldr	r3, [pc, #28]
	adds	r2, #2
	strh	r3, [r2, #0]
	ldrh	r3, [r5, #0]
	adds	r1, r3, #0
	strh	r5, [r5, #0]
	ldrh	r2, [r6, #0]
	cmp	r2, #31
	bgt.n	.L_080cda8e
	b.n	.L_080cda70
	movs	r0, r0
	.4byte 0x00001088
	.4byte 0x00003537
	.4byte 0x00003f21
	.2byte 0x0048
	.2byte 0x0400
.L_080cda70:
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #2
	adds	r2, #1
	adds	r3, r3, r6
	strh	r2, [r6, #0]
	ldr	r2, [pc, #148]
	adds	r3, #4
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #19
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #10
	str	r2, [r3, #0]
.L_080cda8e:
	strh	r1, [r5, #0]
	movs	r0, #1
	bl	sub_080030f8
	ldr	r3, [pc, #128]
	movs	r2, #128
	mov	r8, r3
	movs	r3, #128
	movs	r5, #0
	lsls	r2, r2, #1
	lsls	r3, r3, #2
	mov	ip, r5
	movs	r7, #0
	mov	sl, r2
	mov	lr, r3
	movs	r6, #0
.L_080cdaae:
	mov	r2, sl
	movs	r4, #0
	adds	r0, r6, r2
	lsls	r1, r7, #1
.L_080cdab6:
	adds	r3, r0, #0
	orrs	r3, r1
	lsls	r3, r3, #16
	asrs	r3, r3, #16
	mov	r9, r3
	mov	r3, r8
	adds	r2, r5, r3
	adds	r4, #1
	mov	r3, r9
	strh	r3, [r2, #0]
	add	r0, lr
	adds	r1, #2
	adds	r5, #2
	cmp	r4, #8
	bne.n	.L_080cdab6
	movs	r2, #128
	movs	r3, #1
	lsls	r2, r2, #5
	add	ip, r3
	adds	r6, r6, r2
	mov	r2, ip
	adds	r7, #8
	cmp	r2, #16
	bne.n	.L_080cdaae
	movs	r1, #128
	ldr	r0, [sp, #0]
	ldr	r5, [pc, #48]
	lsls	r1, r1, #7
	bl	sub_080072f8
	movs	r1, #128
	lsls	r1, r1, #7
	ldr	r0, [pc, #40]
	bl	sub_080072f8
	movs	r0, #1
	bl	sub_080030f8
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x00007741
	.4byte 0x06003800
	.4byte 0x03000164
	.4byte 0x06004000
