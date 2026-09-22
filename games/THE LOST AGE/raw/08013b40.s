.syntax unified
	.thumb
	.set sub_08002054, 0x08002054
	.global Overlay_08013b40
Overlay_08013b40:
	push	{lr}
	ldr	r3, [pc, #24]
	ldrh	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L_08013b5a
	ldr	r3, [pc, #20]
	movs	r2, #195
	lsls	r2, r2, #8
	adds	r2, #255
	strh	r2, [r3, #0]
	ldr	r2, [pc, #12]
	movs	r3, #1
	strb	r3, [r2, #0]
.L_08013b5a:
	pop	{pc}
	.4byte 0x02003000
	.4byte 0x04000132
	.2byte 0x11c0
	.2byte 0x0300
	push	{r5, r6, lr}
	ldr	r4, [pc, #48]
	adds	r6, r0, #0
	adds	r0, r1, #0
	ldr	r1, [pc, #44]
	ldrh	r3, [r1, #0]
	adds	r5, r3, #0
	strh	r1, [r1, #0]
	ldrh	r2, [r4, #0]
	cmp	r2, #31
	bgt.n	.L_08013b96
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #2
	adds	r3, r3, r4
	adds	r3, #4
	stmia	r3!, {r0}
	adds	r2, #1
	stmia	r3!, {r6}
	strh	r2, [r4, #0]
	movs	r2, #128
	lsls	r2, r2, #9
	str	r2, [r3, #0]
.L_08013b96:
	strh	r5, [r1, #0]
	pop	{r5, r6, pc}
	movs	r0, r0
	.4byte 0x020038e0
	.2byte 0x0208
	.2byte 0x0400
	push	{r5, r6, lr}
	ldr	r4, [pc, #48]
	adds	r6, r0, #0
	adds	r0, r1, #0
	ldr	r1, [pc, #44]
	ldrh	r3, [r1, #0]
	adds	r5, r3, #0
	strh	r1, [r1, #0]
	ldrh	r2, [r4, #0]
	cmp	r2, #31
	bgt.n	.L_08013bd2
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #2
	adds	r3, r3, r4
	adds	r3, #4
	stmia	r3!, {r0}
	adds	r2, #1
	stmia	r3!, {r6}
	strh	r2, [r4, #0]
	movs	r2, #128
	lsls	r2, r2, #10
	str	r2, [r3, #0]
.L_08013bd2:
	strh	r5, [r1, #0]
	pop	{r5, r6, pc}
	movs	r0, r0
	.4byte 0x020038e0
	.2byte 0x0208
	.2byte 0x0400
	push	{r5, r6, lr}
	ldr	r4, [pc, #48]
	adds	r6, r0, #0
	adds	r0, r1, #0
	ldr	r1, [pc, #44]
	ldrh	r3, [r1, #0]
	adds	r5, r3, #0
	strh	r1, [r1, #0]
	ldrh	r2, [r4, #0]
	cmp	r2, #31
	bgt.n	.L_08013c0e
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #2
	adds	r3, r3, r4
	adds	r3, #4
	stmia	r3!, {r0}
	adds	r2, #1
	stmia	r3!, {r6}
	strh	r2, [r4, #0]
	movs	r2, #192
	lsls	r2, r2, #10
	str	r2, [r3, #0]
.L_08013c0e:
	strh	r5, [r1, #0]
	pop	{r5, r6, pc}
	movs	r0, r0
	.4byte 0x020038e0
	.2byte 0x0208
	.2byte 0x0400
	push	{r5, r6, lr}
	ldr	r4, [pc, #48]
	adds	r6, r0, #0
	adds	r0, r1, #0
	ldr	r1, [pc, #44]
	ldrh	r3, [r1, #0]
	adds	r5, r3, #0
	strh	r1, [r1, #0]
	ldrh	r2, [r4, #0]
	cmp	r2, #31
	bgt.n	.L_08013c4a
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #2
	adds	r3, r3, r4
	adds	r3, #4
	stmia	r3!, {r0}
	adds	r2, #1
	stmia	r3!, {r6}
	strh	r2, [r4, #0]
	movs	r2, #160
	lsls	r2, r2, #11
	str	r2, [r3, #0]
.L_08013c4a:
	strh	r5, [r1, #0]
	pop	{r5, r6, pc}
	movs	r0, r0
	.4byte 0x020038e0
	.2byte 0x0208
	.2byte 0x0400
	push	{r5, r6, lr}
	ldr	r4, [pc, #48]
	adds	r6, r0, #0
	adds	r0, r1, #0
	ldr	r1, [pc, #44]
	ldrh	r3, [r1, #0]
	adds	r5, r3, #0
	strh	r1, [r1, #0]
	ldrh	r2, [r4, #0]
	cmp	r2, #31
	bgt.n	.L_08013c86
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #2
	adds	r3, r3, r4
	adds	r3, #4
	stmia	r3!, {r0}
	adds	r2, #1
	stmia	r3!, {r6}
	strh	r2, [r4, #0]
	movs	r2, #192
	lsls	r2, r2, #11
	str	r2, [r3, #0]
.L_08013c86:
	strh	r5, [r1, #0]
	pop	{r5, r6, pc}
	movs	r0, r0
	.4byte 0x020038e0
	.2byte 0x0208
	.2byte 0x0400
	push	{r5, r6, lr}
	ldr	r4, [pc, #48]
	adds	r6, r0, #0
	adds	r0, r1, #0
	ldr	r1, [pc, #44]
	ldrh	r3, [r1, #0]
	adds	r5, r3, #0
	strh	r1, [r1, #0]
	ldrh	r2, [r4, #0]
	cmp	r2, #31
	bgt.n	.L_08013cc2
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #2
	adds	r3, r3, r4
	adds	r3, #4
	stmia	r3!, {r0}
	adds	r2, #1
	stmia	r3!, {r6}
	strh	r2, [r4, #0]
	movs	r2, #224
	lsls	r2, r2, #11
	str	r2, [r3, #0]
.L_08013cc2:
	strh	r5, [r1, #0]
	pop	{r5, r6, pc}
	movs	r0, r0
	.4byte 0x020038e0
	.2byte 0x0208
	.2byte 0x0400
	push	{r5, r6, lr}
	ldr	r4, [pc, #48]
	adds	r6, r0, #0
	adds	r0, r1, #0
	ldr	r1, [pc, #44]
	ldrh	r3, [r1, #0]
	adds	r5, r3, #0
	strh	r1, [r1, #0]
	ldrh	r2, [r4, #0]
	cmp	r2, #31
	bgt.n	.L_08013cfe
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #2
	adds	r3, r3, r4
	adds	r3, #4
	stmia	r3!, {r0}
	adds	r2, #1
	stmia	r3!, {r6}
	strh	r2, [r4, #0]
	movs	r2, #144
	lsls	r2, r2, #12
	str	r2, [r3, #0]
.L_08013cfe:
	strh	r5, [r1, #0]
	pop	{r5, r6, pc}
	movs	r0, r0
	.4byte 0x020038e0
	.2byte 0x0208
	.2byte 0x0400
	push	{r5, r6, lr}
	ldr	r4, [pc, #48]
	adds	r6, r0, #0
	adds	r0, r1, #0
	ldr	r1, [pc, #44]
	ldrh	r3, [r1, #0]
	adds	r5, r3, #0
	strh	r1, [r1, #0]
	ldrh	r2, [r4, #0]
	cmp	r2, #31
	bgt.n	.L_08013d3a
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #2
	adds	r3, r3, r4
	adds	r3, #4
	stmia	r3!, {r0}
	adds	r2, #1
	stmia	r3!, {r6}
	strh	r2, [r4, #0]
	movs	r2, #160
	lsls	r2, r2, #12
	str	r2, [r3, #0]
.L_08013d3a:
	strh	r5, [r1, #0]
	pop	{r5, r6, pc}
	movs	r0, r0
	.4byte 0x020038e0
	.2byte 0x0208
	.2byte 0x0400
	push	{r5, r6, lr}
	ldr	r4, [pc, #48]
	adds	r6, r0, #0
	adds	r0, r1, #0
	ldr	r1, [pc, #44]
	ldrh	r3, [r1, #0]
	adds	r5, r3, #0
	strh	r1, [r1, #0]
	ldrh	r2, [r4, #0]
	cmp	r2, #31
	bgt.n	.L_08013d76
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #2
	adds	r3, r3, r4
	adds	r3, #4
	stmia	r3!, {r0}
	adds	r2, #1
	stmia	r3!, {r6}
	strh	r2, [r4, #0]
	movs	r2, #176
	lsls	r2, r2, #12
	str	r2, [r3, #0]
.L_08013d76:
	strh	r5, [r1, #0]
	pop	{r5, r6, pc}
	movs	r0, r0
	.4byte 0x020038e0
	.2byte 0x0208
	.2byte 0x0400
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r6, [pc, #72]
	mov	r7, sp
	ldrh	r5, [r6, #0]
	cmp	r5, #0
	beq.n	.L_08013dce
	ldr	r3, [pc, #68]
	movs	r1, #0
	adds	r2, r3, #1
	adds	r3, #4
	lsrs	r3, r3, #2
	mov	sl, r1
	lsls	r3, r3, #2
	mov	r1, sp
	subs	r1, r1, r3
	movs	r4, #132
	movs	r3, #128
	lsls	r4, r4, #24
	lsrs	r2, r2, #2
	lsls	r3, r3, #19
	mov	r8, sp
	adds	r3, #212
	mov	sp, r1
	ldr	r0, [pc, #36]
	orrs	r2, r4
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r0, r6, #0
	adds	r1, r5, #0
	mov	lr, sp
	.2byte 0xf800
	.2byte 0x4652
	strh	r2, [r6, #0]
	mov	sp, r8
.L_08013dce:
	mov	sp, r7
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.4byte 0x020038e0
	.4byte 0x0000006b
	.2byte 0x3060
	.2byte 0x0801
	push	{r5, r6, r7, lr}
	ldr	r3, [pc, #28]
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_08013e6e
	ldr	r3, [pc, #24]
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_08013e10
	ldr	r3, [pc, #20]
	ldrh	r2, [r3, #0]
	ldr	r3, [pc, #4]
	b.n	.L_08013e16
	movs	r0, r0
	.4byte 0x00000080
	.4byte 0x03001178
	.4byte 0x030011dc
	.2byte 0x11f4
	.2byte 0x0300
.L_08013e10:
	ldr	r3, [pc, #64]
	ldrh	r2, [r3, #0]
	ldr	r3, [pc, #56]
.L_08013e16:
	orrs	r3, r2
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #80
	strh	r3, [r2, #0]
	ldr	r6, [pc, #52]
	ldrb	r3, [r6, #0]
	adds	r3, #255
	strb	r3, [r6, #0]
	ldr	r2, [pc, #48]
	ldr	r3, [pc, #52]
	ldrb	r5, [r2, #0]
	ldrb	r3, [r3, #0]
	ldrb	r2, [r2, #0]
	ldr	r7, [pc, #48]
	subs	r3, r3, r2
	ldrb	r2, [r6, #0]
	ldrb	r1, [r7, #0]
	adds	r0, r2, #0
	muls	r0, r3
	bl	sub_08002054
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r5, r5, r0
	adds	r3, #84
	strh	r5, [r3, #0]
	ldrb	r3, [r6, #0]
	b.n	.L_08013e68
	.4byte 0x000000c0
	.4byte 0x030011f4
	.4byte 0x0300110c
	.4byte 0x030011b0
	.4byte 0x0300113c
	.2byte 0x1178
	.2byte 0x0300
.L_08013e68:
	cmp	r3, #0
	bne.n	.L_08013e6e
	strb	r3, [r7, #0]
.L_08013e6e:
	pop	{r5, r6, r7, pc}
	ldr	r2, [pc, #36]
	movs	r3, #0
	strb	r3, [r2, #0]
	ldr	r2, [pc, #36]
	ldr	r3, [pc, #24]
	ldr	r1, [pc, #36]
	strh	r3, [r2, #0]
	ldr	r2, [pc, #36]
	ldrb	r3, [r2, #0]
	strb	r3, [r1, #0]
	movs	r3, #16
	strb	r3, [r2, #0]
	ldr	r3, [pc, #28]
	ldr	r2, [pc, #32]
	strb	r0, [r3, #0]
	ldrb	r3, [r3, #0]
	strb	r3, [r2, #0]
	b.n	.L_08013eb0
	.4byte 0x0000003e
	.4byte 0x030011dc
	.4byte 0x030011f4
	.4byte 0x0300113c
	.4byte 0x030011b0
	.4byte 0x03001178
	.2byte 0x110c
	.2byte 0x0300
.L_08013eb0:
	bx	lr
