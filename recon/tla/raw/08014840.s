.syntax unified
	.thumb
	.set sub_08002074, 0x08002074
	.set sub_08002096, 0x08002096
	.global Func_08014840
	.thumb_func
Func_08014840:
	push	{r5, r6, r7, lr}
	ldr	r3, [pc, #44]
	adds	r7, r0, #0
	ldrb	r3, [r3, #0]
	ldr	r5, [pc, #40]
	asrs	r7, r7, #8
	cmp	r3, #1
	bne.n	.L_0801486e
	movs	r6, #25
	subs	r5, #8
.L_08014854:
	subs	r6, #1
	cmp	r6, #0
	beq.n	.L_0801486e
	adds	r5, #8
	ldrb	r3, [r5, #5]
	cmp	r3, #4
	beq.n	.L_0801486e
	cmp	r3, r7
	bne.n	.L_08014854
	ldr	r0, [r5, #0]
	mov	lr, r0
	.2byte 0xf800
	.2byte 0xe7f2
.L_0801486e:
	pop	{r5, r6, r7, pc}
	.4byte 0x03001228
	.2byte 0x3610
	.2byte 0x0200
	ldr	r1, [pc, #24]
	ldr	r3, [pc, #28]
	ldr	r2, [r1, #0]
	adds	r0, r2, #0
	muls	r0, r3
	movs	r3, #192
	lsls	r3, r3, #6
	adds	r3, #57
	adds	r0, r0, r3
	str	r0, [r1, #0]
	lsls	r0, r0, #8
	lsrs	r0, r0, #16
	bx	lr
	movs	r0, r0
	.4byte 0x030011bc
	.2byte 0x4e6d
	.2byte 0x41c6
	push	{r5, r6, lr}
	mov	r6, sl
	mov	r5, r8
	push	{r5, r6}
	mov	sl, r0
	movs	r0, #128
	mov	r8, r1
	lsls	r0, r0, #7
	add	r0, r8
	adds	r5, r2, #0
	bl	sub_08002096
	ldr	r6, [pc, #44]
	adds	r1, r0, #0
	mov	r0, sl
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x682b
	adds	r3, r3, r0
	stmia	r5!, {r3}
	mov	r0, r8
	bl	sub_08002096
	adds	r1, r0, #0
	mov	r0, sl
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x3504
	ldr	r3, [r5, #0]
	adds	r3, r3, r0
	str	r3, [r5, #0]
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, pc}
	movs	r0, r0
	.2byte 0x021c
	.2byte 0x0300
	push	{r5, r6, lr}
	adds	r6, r0, #0
	adds	r5, r1, #0
	movs	r4, #0
	cmp	r6, #0
	beq.n	.L_08014994
	movs	r4, #128
	lsls	r4, r4, #7
	cmp	r5, #0
	beq.n	.L_08014994
	cmp	r5, #0
	bge.n	.L_08014902
	negs	r1, r5
.L_08014902:
	adds	r0, r6, #0
	cmp	r6, #0
	bge.n	.L_0801490a
	negs	r0, r6
.L_0801490a:
	lsls	r0, r0, #8
	ldr	r3, [pc, #156]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x1c01
	movs	r0, #251
	lsls	r0, r0, #8
	movs	r4, #128
	adds	r0, #106
	lsls	r4, r4, #7
	cmp	r1, r0
	bgt.n	.L_08014994
	ldr	r3, [pc, #140]
	movs	r2, #128
	ldrh	r0, [r3, #0]
	movs	r4, #0
	lsls	r2, r2, #6
	subs	r3, #128
	cmp	r1, r0
	ble.n	.L_0801493a
	movs	r0, #128
	lsls	r0, r0, #1
	adds	r4, r2, #0
	adds	r3, r3, r0
.L_0801493a:
	ldrh	r0, [r3, #0]
	lsrs	r2, r2, #1
	subs	r3, #64
	cmp	r1, r0
	ble.n	.L_08014948
	adds	r4, r4, r2
	adds	r3, #128
.L_08014948:
	ldrh	r0, [r3, #0]
	lsrs	r2, r2, #1
	subs	r3, #32
	cmp	r1, r0
	ble.n	.L_08014956
	adds	r4, r4, r2
	adds	r3, #64
.L_08014956:
	ldrh	r0, [r3, #0]
	lsrs	r2, r2, #1
	subs	r3, #16
	cmp	r1, r0
	ble.n	.L_08014964
	adds	r4, r4, r2
	adds	r3, #32
.L_08014964:
	ldrh	r0, [r3, #0]
	lsrs	r2, r2, #1
	subs	r3, #8
	cmp	r1, r0
	ble.n	.L_08014972
	adds	r4, r4, r2
	adds	r3, #16
.L_08014972:
	ldrh	r0, [r3, #0]
	lsrs	r2, r2, #1
	subs	r3, #4
	cmp	r1, r0
	ble.n	.L_08014980
	adds	r4, r4, r2
	adds	r3, #8
.L_08014980:
	ldrh	r0, [r3, #0]
	subs	r3, #2
	cmp	r1, r0
	ble.n	.L_0801498c
	adds	r4, #128
	adds	r3, #4
.L_0801498c:
	ldrh	r0, [r3, #0]
	cmp	r1, r0
	ble.n	.L_08014994
	adds	r4, #64
.L_08014994:
	cmp	r5, #0
	bge.n	.L_0801499e
	movs	r3, #128
	lsls	r3, r3, #8
	subs	r4, r3, r4
.L_0801499e:
	cmp	r6, #0
	bge.n	.L_080149a4
	negs	r4, r4
.L_080149a4:
	lsls	r0, r4, #16
	lsrs	r0, r0, #16
	pop	{r5, r6, pc}
	movs	r0, r0
	.4byte 0x03000534
	.2byte 0x79ea
	.2byte 0x0801
	push	{r5, r6, lr}
	movs	r5, #0
	movs	r4, #15
	movs	r6, #1
.L_080149bc:
	adds	r3, r4, #1
	adds	r2, r5, #0
	lsls	r2, r3
	lsls	r1, r4, #1
	adds	r3, r6, #0
	lsls	r3, r1
	adds	r2, r2, r3
	cmp	r2, r0
	bgt.n	.L_080149d6
	adds	r3, r6, #0
	lsls	r3, r4
	orrs	r5, r3
	subs	r0, r0, r2
.L_080149d6:
	subs	r4, #1
	cmp	r4, #0
	bge.n	.L_080149bc
	adds	r0, r5, #0
	pop	{r5, r6, pc}
	push	{lr}
	ldr	r3, [pc, #8]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x0200
	pop	{pc}
	.2byte 0x02d4
	.2byte 0x0300
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0x7b10
	.2byte 0x0801
	push	{lr}
	ldr	r3, [pc, #32]
	ldr	r4, [pc, #32]
	movs	r1, #15
	adds	r2, r3, #7
	mov	ip, r3
.L_08014a04:
	adds	r3, r0, #0
	ands	r3, r1
	ldrb	r3, [r4, r3]
	lsrs	r0, r0, #4
	strb	r3, [r2, #0]
	subs	r2, #1
	cmp	r2, ip
	bge.n	.L_08014a04
	ldr	r2, [pc, #4]
	movs	r3, #0
	strb	r3, [r2, #8]
	pop	{pc}
	.4byte 0x03001250
	.2byte 0x7cd0
	.2byte 0x0801
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r3, #0
	adds	r5, r0, #0
	mov	r8, r3
	cmp	r5, #0
	bge.n	.L_08014a3a
	movs	r3, #1
	negs	r5, r5
	mov	r8, r3
.L_08014a3a:
	ldr	r2, [pc, #100]
	movs	r3, #0
	strb	r3, [r2, #11]
	movs	r6, #10
	cmp	r5, #0
	beq.n	.L_08014a6a
	adds	r7, r2, #0
	adds	r7, #10
.L_08014a4a:
	adds	r0, r5, #0
	ldr	r1, [pc, #84]
	bl	sub_08002074
	lsls	r3, r0, #2
	adds	r3, r3, r0
	lsls	r3, r3, #1
	subs	r5, r5, r3
	adds	r3, r5, #0
	adds	r3, #48
	adds	r5, r0, #0
	strb	r3, [r7, #0]
	subs	r6, #1
	subs	r7, #1
	cmp	r5, #0
	bne.n	.L_08014a4a
.L_08014a6a:
	cmp	r6, #10
	bne.n	.L_08014a76
	ldr	r2, [pc, #48]
	movs	r3, #48
	strb	r3, [r2, r6]
	movs	r6, #9
.L_08014a76:
	mov	r3, r8
	cmp	r3, #0
	beq.n	.L_08014a84
	ldr	r2, [pc, #32]
	movs	r3, #45
	strb	r3, [r2, r6]
	subs	r6, #1
.L_08014a84:
	cmp	r6, #0
	blt.n	.L_08014a98
	ldr	r3, [pc, #20]
	movs	r1, #32
	adds	r2, r6, r3
	mov	ip, r3
.L_08014a90:
	strb	r1, [r2, #0]
	subs	r2, #1
	cmp	r2, ip
	bge.n	.L_08014a90
.L_08014a98:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x03001250
	.2byte 0x999a
	.2byte 0x1999
	push	{lr}
	ldr	r3, [pc, #40]
	ldr	r1, [pc, #40]
	ldrb	r3, [r3, #0]
	ldr	r2, [r1, #0]
	cmp	r3, #0
	beq.n	.L_08014adc
	movs	r3, #0
	cmp	r3, r0
	bcs.n	.L_08014aca
	ldr	r1, [pc, #16]
.L_08014abe:
	adds	r3, #1
	strh	r1, [r2, #0]
	adds	r2, #2
	cmp	r3, r0
	bcc.n	.L_08014abe
	ldr	r1, [pc, #12]
.L_08014aca:
	str	r2, [r1, #0]
	b.n	.L_08014adc
	movs	r0, r0
	.4byte 0x0000f000
	.4byte 0x03001110
	.2byte 0x11c4
	.2byte 0x0300
.L_08014adc:
	pop	{pc}
	.align 2, 0
