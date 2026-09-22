.syntax unified
	.thumb
	.set sub_08002096, 0x08002096
	.set sub_08014d00, 0x08014d00
	.global Overlay_08014de4
Overlay_08014de4:
	push	{r5, lr}
	movs	r1, #48
	movs	r0, #8
	ldr	r5, [pc, #36]
	bl	sub_08014d00
	ldr	r2, [pc, #32]
	movs	r3, #0
	str	r3, [r2, #0]
	str	r0, [r5, #0]
	ldr	r3, [pc, #28]
	adds	r0, r3, #0
	movs	r1, #128
	lsls	r1, r1, #9
	movs	r2, #0
	movs	r3, #0
	movs	r4, #0
	stmia	r0!, {r1, r2, r3, r4}
	stmia	r0!, {r1, r2, r3, r4}
	stmia	r0!, {r1, r2, r3, r4}
	pop	{r5, pc}
	movs	r0, r0
	.4byte 0x03001220
	.4byte 0x030011cc
	.2byte 0x0478
	.2byte 0x0300
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [pc, #12]
	ldr	r3, [r3, #8]
	str	r3, [r2, #0]
	ldr	r2, [pc, #12]
	movs	r3, #0
	str	r3, [r2, #0]
	bx	lr
	movs	r0, r0
	.4byte 0x03001220
	.2byte 0x11cc
	.2byte 0x0300
	push	{r5, lr}
	ldr	r5, [pc, #44]
	ldr	r3, [r5, #0]
	cmp	r3, #0
	bgt.n	.L_08014e64
	ldr	r4, [pc, #40]
	movs	r3, #128
	movs	r2, #132
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r3, #212
	ldr	r0, [pc, #32]
	ldr	r1, [r4, #0]
	adds	r2, #12
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r3, [r5, #0]
	adds	r3, #1
	str	r3, [r5, #0]
	ldr	r3, [r4, #0]
	adds	r3, #48
	str	r3, [r4, #0]
.L_08014e64:
	pop	{r5, pc}
	movs	r0, r0
	.4byte 0x030011cc
	.4byte 0x03001220
	.2byte 0x0478
	.2byte 0x0300
	movs	r3, #128
	movs	r2, #132
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r1, r0, #0
	adds	r3, #212
	ldr	r0, [pc, #8]
	adds	r2, #12
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	bx	lr
	movs	r0, r0
	.2byte 0x0478
	.2byte 0x0300
	movs	r3, #128
	movs	r2, #132
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r3, #212
	ldr	r1, [pc, #8]
	adds	r2, #12
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	bx	lr
	.2byte 0x0478
	.2byte 0x0300
	push	{lr}
	ldr	r2, [pc, #40]
	ldr	r3, [r2, #0]
	cmp	r3, #0
	ble.n	.L_08014ed0
	subs	r3, #1
	str	r3, [r2, #0]
	ldr	r3, [pc, #32]
	movs	r2, #132
	ldr	r0, [r3, #0]
	lsls	r2, r2, #24
	subs	r0, #48
	str	r0, [r3, #0]
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #212
	ldr	r1, [pc, #16]
	adds	r2, #12
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
.L_08014ed0:
	pop	{pc}
	movs	r0, r0
	.4byte 0x030011cc
	.4byte 0x03001220
	.2byte 0x0478
	.2byte 0x0300
	ldr	r3, [pc, #20]
	adds	r0, r3, #0
	movs	r1, #128
	lsls	r1, r1, #9
	movs	r2, #0
	movs	r3, #0
	movs	r4, #0
	stmia	r0!, {r1, r2, r3, r4}
	stmia	r0!, {r1, r2, r3, r4}
	stmia	r0!, {r1, r2, r3, r4}
	bx	lr
	movs	r0, r0
	.2byte 0x0478
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	adds	r5, r0, #0
	ldr	r0, [r5, #0]
	sub	sp, #56
	bl	sub_08002096
	adds	r7, r0, #0
	ldr	r0, [r5, #0]
	movs	r6, #128
	lsls	r6, r6, #7
	adds	r0, r0, r6
	bl	sub_08002096
	str	r0, [sp, #4]
	ldr	r0, [r5, #4]
	bl	sub_08002096
	mov	fp, r0
	ldr	r0, [r5, #4]
	adds	r0, r0, r6
	bl	sub_08002096
	str	r0, [sp, #0]
	ldr	r0, [r5, #8]
	bl	sub_08002096
	mov	r9, r0
	ldr	r0, [r5, #8]
	adds	r0, r0, r6
	bl	sub_08002096
	mov	sl, r0
	ldr	r6, [pc, #208]
	ldr	r0, [sp, #0]
	mov	r1, sl
	mov	lr, r6
	.2byte 0xf800
	.2byte 0xaa02
	str	r0, [r2, #0]
	ldr	r0, [sp, #0]
	mov	r1, r9
	mov	r8, r2
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x4643
	mov	r2, fp
	str	r0, [r3, #4]
	negs	r3, r2
	mov	r2, r8
	str	r3, [r2, #8]
	mov	r1, fp
	adds	r0, r7, #0
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x4651
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x4649
	adds	r5, r0, #0
	ldr	r0, [sp, #4]
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x4643
	subs	r5, r5, r0
	str	r5, [r3, #12]
	mov	r1, fp
	adds	r0, r7, #0
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x4649
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x4651
	adds	r5, r0, #0
	ldr	r0, [sp, #4]
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x4642
	adds	r5, r5, r0
	str	r5, [r2, #16]
	ldr	r1, [sp, #0]
	adds	r0, r7, #0
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x4643
	str	r0, [r3, #20]
	ldr	r0, [sp, #4]
	mov	r1, fp
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x4651
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x4649
	adds	r5, r0, #0
	adds	r0, r7, #0
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x4642
	adds	r5, r5, r0
	str	r5, [r2, #24]
	ldr	r0, [sp, #4]
	mov	r1, fp
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x4649
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x4651
	adds	r5, r0, #0
	adds	r0, r7, #0
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x4643
	subs	r5, r5, r0
	str	r5, [r3, #28]
	ldr	r1, [sp, #0]
	ldr	r0, [sp, #4]
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x4642
	movs	r3, #0
	str	r0, [r2, #32]
	str	r3, [r2, #36]
	str	r3, [r2, #40]
	str	r3, [r2, #44]
	mov	r0, r8
	ldr	r3, [pc, #24]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0xb00e
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x0300021c
	.2byte 0x03c8
	.2byte 0x0300
	push	{r5, r6, lr}
	sub	sp, #48
	adds	r6, r0, #0
	mov	r5, sp
	adds	r0, r5, #0
	movs	r1, #128
	lsls	r1, r1, #9
	movs	r2, #0
	movs	r3, #0
	movs	r4, #0
	stmia	r0!, {r1, r2, r3, r4}
	stmia	r0!, {r1, r2, r3, r4}
	stmia	r0!, {r1, r2, r3, r4}
	movs	r3, #128
	lsls	r3, r3, #7
	adds	r0, r6, r3
	bl	sub_08002096
	str	r0, [r5, #16]
	str	r0, [r5, #32]
	adds	r0, r6, #0
	bl	sub_08002096
	str	r0, [r5, #20]
	negs	r0, r0
	str	r0, [r5, #28]
	ldr	r3, [pc, #8]
	adds	r0, r5, #0
	mov	lr, r3
	.2byte 0xf800
	.2byte 0xb00c
	pop	{r5, r6, pc}
	.2byte 0x03c8
	.2byte 0x0300
	push	{r5, r6, lr}
	sub	sp, #48
	adds	r6, r0, #0
	mov	r5, sp
	adds	r0, r5, #0
	movs	r1, #128
	lsls	r1, r1, #9
	movs	r2, #0
	movs	r3, #0
	movs	r4, #0
	stmia	r0!, {r1, r2, r3, r4}
	stmia	r0!, {r1, r2, r3, r4}
	stmia	r0!, {r1, r2, r3, r4}
	movs	r3, #128
	lsls	r3, r3, #7
	adds	r0, r6, r3
	bl	sub_08002096
	str	r0, [r5, #0]
	str	r0, [r5, #32]
	adds	r0, r6, #0
	bl	sub_08002096
	negs	r3, r0
	str	r3, [r5, #8]
	str	r0, [r5, #24]
	ldr	r3, [pc, #8]
	adds	r0, r5, #0
	mov	lr, r3
	.2byte 0xf800
	.2byte 0xb00c
	pop	{r5, r6, pc}
	.2byte 0x03c8
	.2byte 0x0300
	push	{r5, r6, lr}
	adds	r6, r0, #0
	ldr	r5, [pc, #44]
	adds	r0, r5, #0
	movs	r1, #128
	lsls	r1, r1, #9
	movs	r2, #0
	movs	r3, #0
	movs	r4, #0
	stmia	r0!, {r1, r2, r3, r4}
	stmia	r0!, {r1, r2, r3, r4}
	stmia	r0!, {r1, r2, r3, r4}
	movs	r3, #128
	lsls	r3, r3, #7
	adds	r0, r6, r3
	bl	sub_08002096
	str	r0, [r5, #0]
	str	r0, [r5, #32]
	adds	r0, r6, #0
	bl	sub_08002096
	negs	r3, r0
	str	r3, [r5, #8]
	str	r0, [r5, #24]
	pop	{r5, r6, pc}
	.2byte 0x0478
	.2byte 0x0300
	push	{r5, r6, lr}
	sub	sp, #48
	adds	r6, r0, #0
	mov	r5, sp
	adds	r0, r5, #0
	movs	r1, #128
	lsls	r1, r1, #9
	movs	r2, #0
	movs	r3, #0
	movs	r4, #0
	stmia	r0!, {r1, r2, r3, r4}
	stmia	r0!, {r1, r2, r3, r4}
	stmia	r0!, {r1, r2, r3, r4}
	movs	r3, #128
	lsls	r3, r3, #7
	adds	r0, r6, r3
	bl	sub_08002096
	str	r0, [r5, #0]
	str	r0, [r5, #16]
	adds	r0, r6, #0
	bl	sub_08002096
	str	r0, [r5, #4]
	negs	r0, r0
	str	r0, [r5, #12]
	ldr	r3, [pc, #8]
	adds	r0, r5, #0
	mov	lr, r3
	.2byte 0xf800
	.2byte 0xb00c
	pop	{r5, r6, pc}
	.2byte 0x03c8
	.2byte 0x0300
	push	{r5, r6, lr}
	sub	sp, #48
	adds	r6, r0, #0
	mov	r5, sp
	adds	r0, r5, #0
	movs	r1, #128
	lsls	r1, r1, #9
	movs	r2, #0
	movs	r3, #0
	movs	r4, #0
	stmia	r0!, {r1, r2, r3, r4}
	stmia	r0!, {r1, r2, r3, r4}
	stmia	r0!, {r1, r2, r3, r4}
	ldr	r3, [r6, #0]
	adds	r0, r5, #0
	str	r3, [r5, #36]
	ldr	r3, [r6, #4]
	str	r3, [r5, #40]
	ldr	r3, [r6, #8]
	str	r3, [r5, #44]
	ldr	r3, [pc, #8]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0xb00c
	pop	{r5, r6, pc}
	movs	r0, r0
	.2byte 0x03c8
	.2byte 0x0300
	push	{r5, r6, lr}
	sub	sp, #48
	adds	r6, r0, #0
	mov	ip, r1
	mov	lr, r2
	mov	r5, sp
	adds	r0, r5, #0
	movs	r1, #128
	lsls	r1, r1, #9
	movs	r2, #0
	movs	r3, #0
	movs	r4, #0
	stmia	r0!, {r1, r2, r3, r4}
	stmia	r0!, {r1, r2, r3, r4}
	stmia	r0!, {r1, r2, r3, r4}
	mov	r3, ip
	str	r3, [r5, #40]
	mov	r3, lr
	str	r3, [r5, #44]
	str	r6, [r5, #36]
	ldr	r3, [pc, #8]
	adds	r0, r5, #0
	mov	lr, r3
	.2byte 0xf800
	.2byte 0xb00c
	pop	{r5, r6, pc}
	.2byte 0x03c8
	.2byte 0x0300
	ldr	r3, [r0, #0]
	ldr	r2, [pc, #12]
	str	r3, [r2, #36]
	ldr	r3, [r0, #4]
	str	r3, [r2, #40]
	ldr	r3, [r0, #8]
	str	r3, [r2, #44]
	bx	lr
	.2byte 0x0478
	.2byte 0x0300
	push	{r5, r6, lr}
	sub	sp, #48
	adds	r6, r0, #0
	mov	r5, sp
	adds	r0, r5, #0
	movs	r1, #128
	lsls	r1, r1, #9
	movs	r2, #0
	movs	r3, #0
	movs	r4, #0
	stmia	r0!, {r1, r2, r3, r4}
	stmia	r0!, {r1, r2, r3, r4}
	stmia	r0!, {r1, r2, r3, r4}
	ldr	r3, [r6, #0]
	adds	r0, r5, #0
	str	r3, [r5, #0]
	ldr	r3, [r6, #4]
	str	r3, [r5, #16]
	ldr	r3, [r6, #8]
	str	r3, [r5, #32]
	ldr	r3, [pc, #8]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0xb00c
	pop	{r5, r6, pc}
	movs	r0, r0
	.2byte 0x03c8
	.2byte 0x0300
	push	{r5, r6, lr}
	sub	sp, #48
	adds	r6, r0, #0
	mov	ip, r1
	mov	lr, r2
	mov	r5, sp
	adds	r0, r5, #0
	movs	r1, #128
	lsls	r1, r1, #9
	movs	r2, #0
	movs	r3, #0
	movs	r4, #0
	stmia	r0!, {r1, r2, r3, r4}
	stmia	r0!, {r1, r2, r3, r4}
	stmia	r0!, {r1, r2, r3, r4}
	mov	r3, ip
	str	r3, [r5, #16]
	mov	r3, lr
	str	r3, [r5, #32]
	str	r6, [r5, #0]
	ldr	r3, [pc, #8]
	adds	r0, r5, #0
	mov	lr, r3
	.2byte 0xf800
	.2byte 0xb00c
	pop	{r5, r6, pc}
	.2byte 0x03c8
	.2byte 0x0300
	push	{r5, r6, lr}
	sub	sp, #48
	adds	r5, r0, #0
	mov	r6, sp
	adds	r0, r6, #0
	movs	r1, #128
	lsls	r1, r1, #9
	movs	r2, #0
	movs	r3, #0
	movs	r4, #0
	stmia	r0!, {r1, r2, r3, r4}
	stmia	r0!, {r1, r2, r3, r4}
	stmia	r0!, {r1, r2, r3, r4}
	str	r5, [r6, #0]
	str	r5, [r6, #16]
	str	r5, [r6, #32]
	ldr	r3, [pc, #8]
	adds	r0, r6, #0
	mov	lr, r3
	.2byte 0xf800
	.2byte 0xb00c
	pop	{r5, r6, pc}
	.2byte 0x03c8
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #60
	str	r1, [sp, #8]
	adds	r5, r0, #0
	ldr	r0, [r5, #0]
	bl	sub_08002096
	adds	r7, r0, #0
	ldr	r0, [r5, #0]
	movs	r6, #128
	lsls	r6, r6, #7
	adds	r0, r0, r6
	bl	sub_08002096
	str	r0, [sp, #4]
	ldr	r0, [r5, #4]
	bl	sub_08002096
	mov	fp, r0
	ldr	r0, [r5, #4]
	adds	r0, r0, r6
	bl	sub_08002096
	str	r0, [sp, #0]
	ldr	r0, [r5, #8]
	bl	sub_08002096
	mov	r9, r0
	ldr	r0, [r5, #8]
	adds	r0, r0, r6
	bl	sub_08002096
	mov	sl, r0
	ldr	r6, [pc, #224]
	ldr	r0, [sp, #0]
	mov	r1, sl
	mov	lr, r6
	.2byte 0xf800
	.2byte 0xaa03
	str	r0, [r2, #0]
	ldr	r0, [sp, #0]
	mov	r1, r9
	mov	r8, r2
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x4643
	mov	r2, fp
	str	r0, [r3, #4]
	negs	r3, r2
	mov	r2, r8
	str	r3, [r2, #8]
	mov	r1, fp
	adds	r0, r7, #0
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x4651
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x4649
	adds	r5, r0, #0
	ldr	r0, [sp, #4]
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x4643
	subs	r5, r5, r0
	str	r5, [r3, #12]
	mov	r1, fp
	adds	r0, r7, #0
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x4649
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x4651
	adds	r5, r0, #0
	ldr	r0, [sp, #4]
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x4642
	adds	r5, r5, r0
	str	r5, [r2, #16]
	ldr	r1, [sp, #0]
	adds	r0, r7, #0
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x4643
	str	r0, [r3, #20]
	ldr	r0, [sp, #4]
	mov	r1, fp
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x4651
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x4649
	adds	r5, r0, #0
	adds	r0, r7, #0
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x4642
	adds	r5, r5, r0
	str	r5, [r2, #24]
	ldr	r0, [sp, #4]
	mov	r1, fp
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x4649
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x4651
	adds	r5, r0, #0
	adds	r0, r7, #0
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x4643
	subs	r5, r5, r0
	str	r5, [r3, #28]
	ldr	r1, [sp, #0]
	ldr	r0, [sp, #4]
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x4642
	str	r0, [r2, #32]
	ldr	r2, [sp, #8]
	mov	r0, r8
	ldr	r3, [r2, #0]
	mov	r2, r8
	str	r3, [r2, #36]
	ldr	r2, [sp, #8]
	ldr	r3, [r2, #4]
	mov	r2, r8
	str	r3, [r2, #40]
	ldr	r2, [sp, #8]
	ldr	r3, [r2, #8]
	mov	r2, r8
	str	r3, [r2, #44]
	ldr	r3, [pc, #20]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0xb00f
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x0300021c
	.2byte 0x03c8
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #68
	str	r1, [sp, #16]
	str	r2, [sp, #12]
	adds	r5, r0, #0
	ldr	r0, [r5, #0]
	bl	sub_08002096
	str	r0, [sp, #8]
	movs	r6, #128
	ldr	r0, [r5, #0]
	lsls	r6, r6, #7
	adds	r0, r0, r6
	bl	sub_08002096
	str	r0, [sp, #4]
	ldr	r0, [r5, #4]
	bl	sub_08002096
	mov	fp, r0
	ldr	r0, [r5, #4]
	adds	r0, r0, r6
	bl	sub_08002096
	str	r0, [sp, #0]
	ldr	r0, [r5, #8]
	bl	sub_08002096
	mov	r9, r0
	ldr	r0, [r5, #8]
	adds	r0, r0, r6
	bl	sub_08002096
	ldr	r2, [sp, #12]
	mov	sl, r0
	ldr	r6, [pc, #304]
	ldr	r0, [sp, #0]
	mov	r1, sl
	ldr	r7, [r2, #0]
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x1c01
	adds	r0, r7, #0
	mov	lr, r6
	.2byte 0xf800
	.2byte 0xab05
	str	r0, [r3, #0]
	ldr	r0, [sp, #0]
	mov	r1, r9
	mov	r8, r3
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x1c01
	adds	r0, r7, #0
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x4642
	mov	r3, fp
	str	r0, [r2, #4]
	negs	r1, r3
	adds	r0, r7, #0
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x4642
	str	r0, [r2, #8]
	ldr	r3, [sp, #12]
	ldr	r0, [sp, #8]
	mov	r1, fp
	ldr	r7, [r3, #4]
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x4651
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x4649
	adds	r5, r0, #0
	ldr	r0, [sp, #4]
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x1a2d
	adds	r1, r5, #0
	adds	r0, r7, #0
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x4642
	str	r0, [r2, #12]
	ldr	r0, [sp, #8]
	mov	r1, fp
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x4649
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x4651
	adds	r5, r0, #0
	ldr	r0, [sp, #4]
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x182d
	adds	r1, r5, #0
	adds	r0, r7, #0
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x4643
	str	r0, [r3, #16]
	ldr	r1, [sp, #0]
	ldr	r0, [sp, #8]
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x1c01
	adds	r0, r7, #0
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x4642
	str	r0, [r2, #20]
	ldr	r3, [sp, #12]
	ldr	r0, [sp, #4]
	mov	r1, fp
	ldr	r7, [r3, #8]
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x4651
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x4649
	adds	r5, r0, #0
	ldr	r0, [sp, #8]
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x182d
	adds	r1, r5, #0
	adds	r0, r7, #0
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x4642
	str	r0, [r2, #24]
	ldr	r0, [sp, #4]
	mov	r1, fp
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x4649
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x4651
	adds	r5, r0, #0
	ldr	r0, [sp, #8]
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x1a2d
	adds	r1, r5, #0
	adds	r0, r7, #0
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x4643
	str	r0, [r3, #28]
	ldr	r1, [sp, #0]
	ldr	r0, [sp, #4]
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x1c01
	adds	r0, r7, #0
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x4642
	str	r0, [r2, #32]
	ldr	r2, [sp, #16]
	mov	r0, r8
	ldr	r3, [r2, #0]
	mov	r2, r8
	str	r3, [r2, #36]
	ldr	r2, [sp, #16]
	ldr	r3, [r2, #4]
	mov	r2, r8
	str	r3, [r2, #40]
	ldr	r2, [sp, #16]
	ldr	r3, [r2, #8]
	mov	r2, r8
	str	r3, [r2, #44]
	ldr	r3, [pc, #20]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0xb011
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x0300021c
	.2byte 0x03c8
	.2byte 0x0300
.L_08015510:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	adds	r4, r1, #0
	ldr	r1, [pc, #436]
	sub	sp, #68
	str	r0, [sp, #32]
	str	r2, [sp, #28]
	ldr	r0, [pc, #432]
	add	r2, sp, #36
	mov	sl, r2
	mov	r9, r1
	movs	r3, #128
	adds	r1, r2, #0
	movs	r2, #132
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	mov	fp, r0
	adds	r3, #212
	ldr	r0, [pc, #416]
	adds	r2, #8
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r0, [sp, #32]
	ldr	r2, [r4, #0]
	ldr	r3, [r0, #0]
	subs	r2, r2, r3
	str	r2, [sp, #24]
	ldr	r2, [r4, #4]
	ldr	r3, [r0, #4]
	subs	r2, r2, r3
	str	r2, [sp, #20]
	ldr	r2, [r4, #8]
	ldr	r3, [r0, #8]
	ldr	r0, [sp, #20]
	subs	r2, r2, r3
	str	r2, [sp, #16]
	asrs	r3, r0, #8
	ldr	r2, [sp, #24]
	ldr	r0, [sp, #16]
	asrs	r1, r2, #8
	asrs	r2, r0, #8
	str	r2, [sp, #0]
	str	r2, [sp, #4]
	adds	r0, r1, #0
	adds	r2, r3, #0
	mov	lr, sl
	.2byte 0xf800
	.2byte 0x495a
	mov	lr, r1
	.2byte 0xf800
	.2byte 0x1c01
	movs	r0, #128
	lsls	r0, r0, #24
	mov	lr, r9
	.2byte 0xf800
	.2byte 0x0bc5
	negs	r5, r5
	ldr	r0, [sp, #24]
	adds	r1, r5, #0
	mov	lr, fp
	.2byte 0xf800
	.2byte 0x1c29
	str	r0, [sp, #24]
	ldr	r0, [sp, #20]
	mov	lr, fp
	.2byte 0xf800
	.2byte 0x1c29
	str	r0, [sp, #20]
	ldr	r0, [sp, #16]
	mov	lr, fp
	.2byte 0xf800
	.2byte 0x9a06
	str	r0, [sp, #16]
	negs	r2, r2
	str	r2, [sp, #8]
	ldr	r0, [sp, #20]
	adds	r1, r0, #0
	mov	lr, fp
	.2byte 0xf800
	.2byte 0x2380
	lsls	r3, r3, #9
	subs	r0, r3, r0
	adds	r5, r3, #0
	cmp	r0, #0
	ble.n	.L_080155d8
	ldr	r3, [pc, #284]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x1c01
	movs	r0, #128
	lsls	r1, r1, #8
	lsls	r0, r0, #24
	mov	lr, r9
	.2byte 0xf800
	.2byte 0x0045
.L_080155d8:
	ldr	r0, [sp, #16]
	adds	r1, r5, #0
	mov	lr, fp
	.2byte 0xf800
	.2byte 0x1c29
	str	r0, [sp, #12]
	ldr	r0, [sp, #8]
	mov	lr, fp
	.2byte 0xf800
	.2byte 0x9002
	ldr	r1, [sp, #8]
	ldr	r0, [sp, #20]
	mov	lr, fp
	.2byte 0xf800
	.2byte 0x9903
	mov	r8, r0
	ldr	r0, [sp, #16]
	mov	lr, fp
	.2byte 0xf800
	.2byte 0x9902
	adds	r5, r0, #0
	ldr	r0, [sp, #24]
	mov	lr, fp
	.2byte 0xf800
	.2byte 0x9903
	subs	r6, r5, r0
	ldr	r0, [sp, #20]
	mov	lr, fp
	.2byte 0xf800
	.2byte 0x4247
	str	r7, [sp, #0]
	str	r7, [sp, #4]
	adds	r2, r6, #0
	adds	r3, r6, #0
	mov	r1, r8
	mov	r0, r8
	mov	lr, sl
	.2byte 0xf800
	.2byte 0x492f
	mov	lr, r1
	.2byte 0xf800
	.2byte 0x1c01
	movs	r0, #128
	lsls	r1, r1, #8
	lsls	r0, r0, #24
	mov	lr, r9
	.2byte 0xf800
	.2byte 0x0045
	adds	r1, r5, #0
	mov	r0, r8
	mov	lr, fp
	.2byte 0xf800
	.2byte 0x1c29
	mov	r8, r0
	adds	r0, r6, #0
	mov	lr, fp
	.2byte 0xf800
	.2byte 0x1c29
	adds	r6, r0, #0
	adds	r0, r7, #0
	mov	lr, fp
	.2byte 0xf800
	.2byte 0x9a08
	ldr	r3, [sp, #32]
	ldr	r2, [r2, #0]
	ldr	r3, [r3, #4]
	ldr	r1, [sp, #12]
	mov	fp, r2
	adds	r7, r0, #0
	ldr	r2, [sp, #28]
	ldr	r0, [sp, #32]
	mov	r9, r3
	movs	r3, #0
	ldr	r5, [r0, #8]
	str	r1, [r2, #0]
	str	r3, [r2, #12]
	ldr	r0, [sp, #8]
	str	r0, [r2, #24]
	ldr	r1, [sp, #12]
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	adds	r2, r5, #0
	ldr	r3, [sp, #8]
	mov	r0, fp
	mov	lr, sl
	.2byte 0xf800
	.2byte 0x9907
	mov	r2, r8
	negs	r0, r0
	str	r0, [r1, #36]
	str	r2, [r1, #4]
	str	r6, [r1, #16]
	str	r7, [r1, #28]
	mov	r2, r9
	str	r5, [sp, #0]
	str	r7, [sp, #4]
	mov	r1, r8
	adds	r3, r6, #0
	mov	r0, fp
	mov	lr, sl
	.2byte 0xf800
	.2byte 0x9b07
	negs	r0, r0
	str	r0, [r3, #40]
	ldr	r0, [sp, #24]
	str	r0, [r3, #8]
	ldr	r1, [sp, #20]
	mov	r0, fp
	str	r1, [r3, #20]
	ldr	r2, [sp, #16]
	str	r2, [r3, #32]
	ldr	r1, [sp, #24]
	str	r5, [sp, #0]
	str	r2, [sp, #4]
	ldr	r3, [sp, #20]
	mov	r2, r9
	mov	lr, sl
	.2byte 0xf800
	.2byte 0x9b07
	negs	r0, r0
	str	r0, [r3, #44]
	add	sp, #68
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x03000534
	.4byte 0x0300021c
	.4byte 0x08017ce8
	.2byte 0x02d4
	.2byte 0x0300
	push	{lr}
	ldr	r2, [pc, #8]
	bl	.L_08015510
	pop	{pc}
	movs	r0, r0
	.2byte 0x0478
	.2byte 0x0300
	push	{r5, lr}
	sub	sp, #48
	mov	r5, sp
	adds	r2, r5, #0
	bl	.L_08015510
	ldr	r3, [pc, #8]
	adds	r0, r5, #0
	mov	lr, r3
	.2byte 0xf800
	.2byte 0xb00c
	pop	{r5, pc}
	.2byte 0x03c8
	.2byte 0x0300
	push	{r5, r6, lr}
	mov	r6, sl
	mov	r5, r8
	push	{r5, r6}
	adds	r5, r0, #0
	lsrs	r3, r5, #31
	adds	r5, r5, r3
	asrs	r5, r5, #1
	adds	r0, r5, #0
	mov	r8, r1
	mov	sl, r2
	bl	sub_08002096
	movs	r2, #128
	lsls	r2, r2, #7
	adds	r5, r5, r2
	adds	r6, r0, #0
	adds	r0, r5, #0
	bl	sub_08002096
	lsls	r1, r0, #2
	adds	r1, r1, r0
	lsls	r1, r1, #4
	ldr	r3, [pc, #28]
	adds	r0, r6, #0
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x4b06
	mov	r2, r8
	str	r2, [r3, #4]
	mov	r2, sl
	str	r0, [r3, #0]
	str	r2, [r3, #8]
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, pc}
	movs	r0, r0
	.4byte 0x03000230
	.4byte 0x030011e0
