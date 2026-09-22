.syntax unified
	.thumb
	.set sub_08013560, 0x08013560
	.set sub_08013ba4, 0x08013ba4
	.set sub_08014644, 0x08014644
	.set sub_08118038, 0x08118038
	.set sub_08118048, 0x08118048
	.set sub_081434d8, 0x081434d8
	.set sub_081c0010, 0x081c0010
	.global Overlay_08143bb8
Overlay_08143bb8:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	movs	r0, #195
	lsls	r3, r3, #18
	lsls	r0, r0, #1
	ldr	r5, [r3, #92]
	ldr	r7, [r3, #36]
	bl	sub_081c0010
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #160
	adds	r3, r5, r2
	ldr	r6, [pc, #80]
	ldr	r3, [r3, #0]
	ldr	r2, [pc, #80]
	strh	r3, [r6, #4]
	movs	r3, #238
	lsls	r3, r3, #7
	adds	r3, #164
	adds	r5, r5, r3
	ldr	r3, [r5, #0]
	movs	r1, #128
	strh	r3, [r6, #6]
	movs	r3, #120
	str	r3, [r2, #12]
	str	r3, [r2, #16]
	ldr	r3, [pc, #48]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #12
	strh	r3, [r2, #0]
	lsls	r1, r1, #7
	ldr	r3, [pc, #48]
	ldr	r0, [pc, #48]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x480c
	bl	sub_08014644
	ldr	r1, [pc, #44]
	movs	r3, #32
	strh	r3, [r6, #6]
	ldr	r0, [pc, #44]
	ldrh	r3, [r0, #0]
	adds	r4, r3, #0
	strh	r0, [r0, #0]
	ldrh	r2, [r1, #0]
	cmp	r2, #31
	bgt.n	.L_08143c66
	b.n	.L_08143c44
	movs	r0, r0
	.4byte 0x00000787
	.4byte 0x03001120
	.4byte 0x030011e0
	.4byte 0x03000258
	.4byte 0x06004000
	.4byte 0x08143489
	.4byte 0x020038e0
	.2byte 0x0208
	.2byte 0x0400
.L_08143c44:
	lsls	r3, r2, #1
	adds	r3, r3, r2
	adds	r2, #1
	lsls	r3, r3, #2
	strh	r2, [r1, #0]
	movs	r2, #230
	adds	r3, r3, r1
	lsls	r2, r2, #7
	adds	r3, #4
	adds	r2, #65
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #19
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #10
	str	r2, [r3, #0]
.L_08143c66:
	strh	r4, [r0, #0]
	movs	r2, #128
	ldr	r3, [pc, #52]
	lsls	r2, r2, #19
	adds	r2, #80
	strh	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	movs	r2, #206
	lsls	r2, r2, #3
	adds	r3, r7, r2
	ldrh	r1, [r3, #0]
	movs	r2, #7
	movs	r0, #2
	bl	sub_08118038
	movs	r0, #1
	bl	sub_08013560
	movs	r3, #206
	lsls	r3, r3, #3
	movs	r2, #21
	movs	r6, #0
	adds	r7, r7, r3
	movs	r5, #0
	mov	r8, r2
	b.n	.L_08143ca4
	movs	r0, r0
	.2byte 0x0000
	.2byte 0x0000
.L_08143ca4:
	mov	r3, r8
	subs	r1, r3, r5
	ldrh	r0, [r7, #0]
	bl	sub_08118048
	adds	r6, #1
	movs	r0, #1
	bl	sub_08013560
	adds	r5, #3
	cmp	r6, #8
	bne.n	.L_08143ca4
	ldr	r1, [pc, #60]
	ldr	r0, [pc, #64]
	ldrh	r3, [r0, #0]
	adds	r4, r3, #0
	strh	r0, [r0, #0]
	ldrh	r2, [r1, #0]
	cmp	r2, #31
	bgt.n	.L_08143cee
	lsls	r3, r2, #1
	adds	r3, r3, r2
	adds	r2, #1
	lsls	r3, r3, #2
	strh	r2, [r1, #0]
	movs	r2, #234
	adds	r3, r3, r1
	lsls	r2, r2, #7
	adds	r3, #4
	adds	r2, #65
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #19
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #10
	str	r2, [r3, #0]
.L_08143cee:
	strh	r4, [r0, #0]
	movs	r0, #1
	bl	sub_08013560
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.4byte 0x020038e0
	.2byte 0x0208
	.2byte 0x0400
	push	{r5, lr}
	movs	r2, #192
	lsls	r2, r2, #18
	adds	r3, r2, #0
	adds	r3, #176
	ldr	r1, [r3, #0]
	movs	r3, #1
	str	r3, [r1, #12]
	movs	r1, #168
	movs	r0, #128
	lsls	r1, r1, #5
	adds	r1, #65
	lsls	r0, r0, #19
	ldr	r5, [r2, #36]
	bl	sub_08013ba4
	movs	r0, #1
	bl	sub_08013560
	movs	r3, #206
	lsls	r3, r3, #3
	adds	r5, r5, r3
	ldrh	r1, [r5, #0]
	movs	r0, #2
	movs	r2, #0
	bl	sub_08118038
	ldr	r1, [pc, #60]
	ldr	r0, [pc, #60]
	ldrh	r3, [r0, #0]
	adds	r4, r3, #0
	strh	r0, [r0, #0]
	ldrh	r2, [r1, #0]
	cmp	r2, #31
	bgt.n	.L_08143d6e
	lsls	r3, r2, #1
	adds	r3, r3, r2
	adds	r2, #1
	lsls	r3, r3, #2
	strh	r2, [r1, #0]
	movs	r2, #248
	adds	r3, r3, r1
	lsls	r2, r2, #5
	adds	r3, #4
	adds	r2, #131
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #10
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #10
	str	r2, [r3, #0]
.L_08143d6e:
	strh	r4, [r0, #0]
	movs	r0, #1
	bl	sub_08013560
	pop	{r5, pc}
	.4byte 0x020038e0
	.2byte 0x0208
	.2byte 0x0400
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #96]
	ldr	r5, [r3, #36]
	adds	r3, #176
	ldr	r6, [r3, #0]
	mov	r8, r2
	bl	sub_081434d8
	movs	r3, #206
	lsls	r3, r3, #3
	adds	r5, r5, r3
	ldrh	r1, [r5, #0]
	movs	r0, #2
	movs	r2, #0
	bl	sub_08118038
	ldr	r2, [pc, #68]
	movs	r1, #0
	movs	r3, #32
	strh	r3, [r2, #6]
	str	r1, [r6, #12]
	ldr	r3, [pc, #64]
	movs	r1, #64
	ldr	r0, [pc, #64]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x2180
	movs	r2, #1
	negs	r2, r2
	ldr	r3, [pc, #56]
	ldr	r0, [pc, #56]
	lsls	r1, r1, #2
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x4d0d
	movs	r2, #128
	ldr	r7, [pc, #24]
	lsls	r2, r2, #1
	movs	r0, #0
	movs	r6, #0
	mov	ip, r2
	movs	r4, #0
.L_08143ddc:
	mov	r3, ip
	movs	r1, #0
	adds	r2, r4, r3
.L_08143de2:
	cmp	r1, #15
	ble.n	.L_08143e08
	adds	r3, r0, r5
	strh	r7, [r3, #0]
	b.n	.L_08143e0c
	.4byte 0x000000ff
	.4byte 0x03001120
	.4byte 0x03000258
	.4byte 0x06003fc0
	.4byte 0x03000260
	.4byte 0x0600f900
	.2byte 0xfb00
	.2byte 0x0600
.L_08143e08:
	adds	r3, r0, r5
	strh	r2, [r3, #0]
.L_08143e0c:
	adds	r1, #1
	adds	r2, #1
	adds	r0, #2
	cmp	r1, #32
	bne.n	.L_08143de2
	adds	r6, #1
	adds	r4, #16
	cmp	r6, #16
	bne.n	.L_08143ddc
	ldr	r3, [pc, #52]
	movs	r2, #128
	lsls	r2, r2, #19
	strh	r3, [r2, #0]
	ldr	r3, [pc, #48]
	adds	r2, #10
	strh	r3, [r2, #0]
	ldr	r3, [pc, #44]
	adds	r2, #70
	strh	r3, [r2, #0]
	ldr	r1, [pc, #44]
	movs	r3, #128
	ldr	r2, [pc, #44]
	lsls	r3, r3, #19
	adds	r3, #64
	strh	r1, [r3, #0]
	adds	r3, #4
	strh	r2, [r3, #0]
	subs	r3, #2
	strh	r1, [r3, #0]
	adds	r3, #4
	strh	r2, [r3, #0]
	ldr	r3, [pc, #28]
	movs	r2, #128
	lsls	r2, r2, #19
	b.n	.L_08143e6c
	movs	r0, r0
	.4byte 0x00007741
	.4byte 0x00001f81
	.4byte 0x00003f42
	.4byte 0x000000f0
	.4byte 0x00001088
	.2byte 0x3537
	.2byte 0x0000
.L_08143e6c:
	adds	r2, #72
	strh	r3, [r2, #0]
	ldr	r3, [pc, #40]
	adds	r2, #2
	strh	r3, [r2, #0]
	ldr	r3, [pc, #40]
	adds	r2, #8
	movs	r1, #128
	strh	r3, [r2, #0]
	ldr	r5, [pc, #36]
	mov	r0, r8
	lsls	r1, r1, #7
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x2180
	lsls	r1, r1, #7
	ldr	r0, [pc, #24]
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x2001
	bl	sub_08013560
	b.n	.L_08143eac
	movs	r0, r0
	.4byte 0x00003f21
	.4byte 0x0000100e
	.4byte 0x03000258
	.2byte 0x4000
	.2byte 0x0600
.L_08143eac:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
