.syntax unified
.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.set sub_020003e2, 0x020003e2
	.set sub_02000782, 0x02000782
	.set sub_020007a4, 0x020007a4
	.set sub_020007ac, 0x020007ac
	.set sub_020007ce, 0x020007ce
	.set sub_020007d0, 0x020007d0
	.set sub_020007e0, 0x020007e0
	.set sub_020007e6, 0x020007e6
	.set sub_020007ea, 0x020007ea
	.set sub_020007fa, 0x020007fa
	.set sub_02000800, 0x02000800
	.set sub_0200081a, 0x0200081a
	.set sub_0200081c, 0x0200081c
	.set sub_02000824, 0x02000824
	.set sub_0200082c, 0x0200082c
	.set sub_02000834, 0x02000834
	.set sub_0200083a, 0x0200083a
	.set sub_0200083c, 0x0200083c
	.set sub_02000846, 0x02000846
	.set sub_02000848, 0x02000848
	.set sub_02000852, 0x02000852
	.set sub_02000856, 0x02000856
	.set sub_0200087c, 0x0200087c
	.set sub_02000890, 0x02000890
	.set sub_02000898, 0x02000898
	.set sub_020008b4, 0x020008b4
	.set sub_020008c6, 0x020008c6
	.set sub_020008e6, 0x020008e6
	.set sub_020008f4, 0x020008f4
	.set sub_02000918, 0x02000918
	.set sub_02000940, 0x02000940
	.set sub_0200094c, 0x0200094c
	.set sub_02000950, 0x02000950
	.set sub_02000952, 0x02000952
	.set sub_02000958, 0x02000958
	.set sub_0200095e, 0x0200095e
	.set sub_0200096e, 0x0200096e
	.set sub_0200097e, 0x0200097e
	.set sub_0200098e, 0x0200098e
	.set sub_020009a8, 0x020009a8
	.set sub_020009ba, 0x020009ba
	.set sub_020009d8, 0x020009d8
	.set sub_020009e6, 0x020009e6
	.set sub_020009f0, 0x020009f0
	.set sub_020009f4, 0x020009f4
	.set sub_02000a04, 0x02000a04
	.set sub_02000a08, 0x02000a08
	.set sub_02000a10, 0x02000a10
	.set sub_02000a20, 0x02000a20
	.set sub_02000a44, 0x02000a44
	.set sub_02000a4e, 0x02000a4e
	.set sub_02000a52, 0x02000a52
	.set sub_02000a54, 0x02000a54
	.set sub_02000aa6, 0x02000aa6
	.set sub_02000af6, 0x02000af6
	.set sub_02000afc, 0x02000afc
	.set sub_02000b2c, 0x02000b2c
	.set sub_02000b32, 0x02000b32
	.set sub_02000b3e, 0x02000b3e
	.set sub_02000b58, 0x02000b58
	.set sub_02000b5a, 0x02000b5a
	.set sub_02000b66, 0x02000b66
	.set sub_02000b72, 0x02000b72
	.set sub_02000b86, 0x02000b86
	.set sub_02000b8a, 0x02000b8a
	.set sub_02000b8e, 0x02000b8e
	.set sub_02000b92, 0x02000b92
	.set sub_02000ba4, 0x02000ba4
	.set sub_02000bb8, 0x02000bb8
	.set sub_02000bbe, 0x02000bbe
	.set sub_02000bc8, 0x02000bc8
	.set sub_02000bdc, 0x02000bdc
	.set sub_02000cb0, 0x02000cb0
	.set sub_02000cb2, 0x02000cb2
	.set sub_02000cbc, 0x02000cbc
	.set sub_02000cc0, 0x02000cc0
	.set sub_02000ccc, 0x02000ccc
	.set sub_02000cdc, 0x02000cdc
	.set sub_02000cee, 0x02000cee
	.set sub_02000cf6, 0x02000cf6
	.set sub_02000d02, 0x02000d02
	.set sub_02000d04, 0x02000d04
	.set sub_02000d0e, 0x02000d0e
	.global Overlay_02000000
Overlay_02000000:
	.irp EntryTarget, 0x02008591, 0x02008039, 0x02008045, 0x0200804d, 0x02008139, 0x02008041, 0x02008621
	overlay_veneer \EntryTarget
	.endr
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0x8780
	.2byte 0x0200
	movs	r0, #0
	bx	lr
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0x8828
	.2byte 0x0200
	push	{lr}
	ldr	r3, [pc, #48]
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #99
	bne.n	.L_02000062
	ldr	r0, [pc, #36]
	b.n	.L_0200007c
.L_02000062:
	cmp	r3, #98
	bne.n	.L_0200006a
	ldr	r0, [pc, #32]
	b.n	.L_0200007c
.L_0200006a:
	cmp	r3, #97
	bne.n	.L_02000072
	ldr	r0, [pc, #28]
	b.n	.L_0200007c
.L_02000072:
	cmp	r3, #96
	bne.n	.L_0200007a
	ldr	r0, [pc, #24]
	b.n	.L_0200007c
.L_0200007a:
	ldr	r0, [pc, #24]
.L_0200007c:
	pop	{pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x020088a4
	.4byte 0x02008a0c
	.4byte 0x02008b74
	.4byte 0x02008cf4
	.2byte 0x882c
	.2byte 0x0200
	push	{r5, lr}
	adds	r5, r0, #0
	bl	sub_02000782
	movs	r0, #0
	bl	sub_020007d0
	ldr	r3, [pc, #64]
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #99
	bne.n	.L_020000ba
	ldr	r0, [pc, #52]
	b.n	.L_020000c8
.L_020000ba:
	cmp	r3, #98
	bne.n	.L_020000c2
	ldr	r0, [pc, #48]
	b.n	.L_020000c8
.L_020000c2:
	cmp	r3, #97
	bne.n	.L_020000d0
	ldr	r0, [pc, #44]
.L_020000c8:
	adds	r0, r5, r0
	bl	sub_020007e0
	b.n	.L_020000d8
.L_020000d0:
	ldr	r0, [pc, #36]
	adds	r0, r5, r0
	bl	sub_020007ea
.L_020000d8:
	adds	r0, r5, #0
	movs	r1, #0
	bl	sub_020007fa
	bl	sub_020007ce
	pop	{r5, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0000133a
	.4byte 0x00001348
	.4byte 0x0000135e
	.2byte 0x1365
	.2byte 0x0000
	push	{r5, lr}
	adds	r5, r0, #0
	bl	sub_020007e6
	movs	r0, #0
	bl	sub_02000834
	ldr	r0, [pc, #40]
	adds	r0, r5, r0
	bl	sub_02000824
	adds	r0, r5, #0
	movs	r1, #1
	bl	sub_020007a4
	adds	r0, r5, #0
	movs	r1, #0
	bl	sub_0200083c
	cmp	r5, #1
	bne.n	.L_0200012e
	movs	r0, #0
	movs	r1, #0
	bl	sub_02000848
.L_0200012e:
	bl	sub_0200081c
	pop	{r5, pc}
	.2byte 0x135f
	.2byte 0x0000
	push	{lr}
	ldr	r3, [pc, #24]
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #94
	ble.n	.L_0200014e
	ldr	r0, [pc, #12]
	b.n	.L_02000150
.L_0200014e:
	ldr	r0, [pc, #12]
.L_02000150:
	pop	{pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x02008f10
	.2byte 0x8ebc
	.2byte 0x0200
	push	{r5, r6, lr}
	adds	r5, r0, #0
	adds	r6, r1, #0
	bl	sub_020007ac
	ldrb	r1, [r0, #15]
	adds	r0, r5, #0
	adds	r1, r1, r6
	bl	sub_02000846
	adds	r0, r5, #0
	bl	sub_0200082c
	pop	{r5, r6, pc}
	push	{r5, r6, r7, lr}
	sub	sp, #32
	adds	r7, r0, #0
	mov	r0, sp
	bl	sub_02000852
	movs	r5, #0
	adds	r6, r0, #0
	cmp	r5, r6
	bge.n	.L_020001a2
.L_02000190:
	lsls	r2, r5, #1
	mov	r3, sp
	ldrh	r0, [r3, r2]
	adds	r1, r7, #0
	adds	r5, #1
	bl	.L_020002fc
	cmp	r5, r6
	blt.n	.L_02000190
.L_020001a2:
	add	sp, #32
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #268]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	sub	sp, #4
	bl	sub_02000800
	movs	r3, #2
	str	r3, [sp, #0]
	movs	r1, #0
	movs	r2, #30
	movs	r3, #9
	mov	r8, r0
	movs	r0, #0
	bl	sub_0200081a
	ldr	r5, [pc, #236]
	adds	r6, r0, #0
	adds	r1, r6, #0
	adds	r0, r5, #0
	movs	r2, #0
	movs	r3, #0
	bl	sub_0200083a
	adds	r0, r5, #1
	adds	r1, r6, #0
	movs	r2, #0
	movs	r3, #16
	adds	r5, #2
	bl	sub_02000848
	adds	r0, r5, #0
	adds	r1, r6, #0
	movs	r2, #0
	movs	r3, #32
	movs	r7, #1
	bl	sub_02000856
.L_020001fc:
	cmp	r7, #0
	beq.n	.L_02000232
	adds	r0, r6, #0
	bl	sub_02000898
	mov	r0, r8
	adds	r1, r6, #0
	movs	r2, #0
	movs	r3, #48
	bl	sub_0200087c
	ldr	r0, [pc, #176]
	adds	r1, r6, #0
	movs	r2, #48
	movs	r3, #48
	bl	sub_02000890
	mov	r3, r8
	ldrb	r0, [r3, #15]
	movs	r3, #48
	str	r3, [sp, #0]
	movs	r1, #0
	adds	r2, r6, #0
	movs	r3, #72
	movs	r7, #0
	bl	sub_020008b4
.L_02000232:
	ldr	r1, [pc, #148]
	movs	r2, #8
	ldr	r3, [r1, #4]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_02000248
	ldr	r3, [r1, #4]
	movs	r2, #4
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02000256
.L_02000248:
	movs	r0, #5
	bl	.L_020003c8
	movs	r0, #93
	bl	sub_0200098e
	movs	r7, #1
.L_02000256:
	ldr	r5, [pc, #112]
	movs	r2, #1
	ldr	r3, [r5, #4]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02000270
	movs	r0, #1
	bl	sub_020003e2
	movs	r0, #91
	bl	sub_020009a8
	movs	r7, #1
.L_02000270:
	ldr	r3, [r5, #4]
	movs	r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_020002b4
	movs	r0, #113
	bl	sub_020009ba
	adds	r0, r6, #0
	bl	sub_02000918
	movs	r0, #1
	bl	sub_020008c6
	movs	r1, #1
	adds	r0, r6, #0
	bl	sub_020008e6
	movs	r0, #0
	bl	sub_0200094c
	movs	r0, #1
	bl	sub_02000952
	movs	r0, #3
	bl	sub_02000958
	movs	r0, #2
	bl	sub_0200095e
	add	sp, #4
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
.L_020002b4:
	movs	r0, #1
	bl	sub_020008f4
	b.n	.L_020001fc
	.4byte 0x02000240
	.4byte 0x00001151
	.4byte 0x02008744
	.2byte 0x1150
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r2, #0
	movs	r0, #112
	sub	sp, #4
	mov	sl, r2
	bl	sub_02000a20
	movs	r5, #2
	movs	r1, #0
	movs	r2, #30
	movs	r3, #7
	movs	r0, #0
	str	r5, [sp, #0]
	bl	sub_02000940
	movs	r1, #8
	adds	r7, r0, #0
	movs	r2, #13
.L_020002fc:
	movs	r3, #10
	movs	r0, #0
	str	r5, [sp, #0]
	bl	sub_02000950
	movs	r3, #128
	movs	r2, #128
	movs	r6, #1
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	mov	fp, r0
	mov	r9, r6
	adds	r3, #212
	ldr	r0, [pc, #564]
	ldr	r1, [pc, #564]
	adds	r2, #16
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r2, #128
	lsls	r2, r2, #24
	ldr	r0, [pc, #556]
	adds	r1, #28
	adds	r2, #1
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r0, #1
	bl	sub_0200096e
.L_02000334:
	mov	r3, r9
	cmp	r3, #0
	beq.n	.L_020003d2
	movs	r3, #128
	lsls	r3, r3, #2
	movs	r1, #128
	movs	r2, #0
	adds	r0, r6, r3
	lsls	r1, r1, #2
	mov	r9, r2
	bl	sub_0200097e
	adds	r6, r0, #0
	adds	r0, r7, #0
	bl	sub_020009e6
	adds	r0, r7, #0
	bl	sub_020009f4
	ldr	r0, [pc, #508]
	adds	r1, r7, #0
	movs	r2, #0
	movs	r3, #0
	bl	sub_020009d8
	mov	r2, r9
	str	r2, [sp, #0]
	adds	r0, r6, #0
	movs	r1, #0
	adds	r2, r7, #0
	movs	r3, #80
	bl	sub_020009f0
	bl	sub_02000a54
	cmp	r0, #0
	beq.n	.L_020003c6
	movs	r5, #128
	lsls	r5, r5, #1
	adds	r5, #255
	adds	r1, r7, #0
	movs	r2, #0
	movs	r3, #32
	ands	r5, r6
	ldr	r0, [pc, #460]
	bl	sub_02000a04
	adds	r0, r5, #0
	bl	sub_02000a52
	ldr	r0, [pc, #452]
	adds	r1, r7, #0
	adds	r0, r5, r0
	movs	r2, #120
	movs	r3, #0
	bl	sub_02000a08
	ldr	r3, [pc, #444]
	adds	r1, r7, #0
	adds	r5, r5, r3
	adds	r0, r5, #0
	movs	r2, #0
	movs	r3, #16
	bl	sub_02000a10
	mov	r0, fp
	bl	sub_02000a4e
	mov	r0, fp
	adds	r1, r6, #0
	bl	sub_02000af6
	b.n	.L_020003d2
.L_020003c6:
	ldr	r0, [pc, #416]
.L_020003c8:
	adds	r1, r7, #0
	movs	r2, #0
	movs	r3, #32
	bl	sub_02000a44
.L_020003d2:
	ldr	r3, [pc, #408]
	movs	r2, #1
	ldr	r3, [r3, #4]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_020003f2
	adds	r0, r6, #0
	bl	sub_02000aa6
	movs	r3, #1
	negs	r3, r3
	cmp	r0, r3
	beq.n	.L_020003fe
	movs	r0, #175
	bl	sub_02000b2c
.L_020003f2:
	ldr	r5, [pc, #376]
	movs	r2, #2
	ldr	r3, [r5, #4]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02000406
.L_020003fe:
	movs	r0, #113
	bl	sub_02000b3e
	b.n	.L_02000520
.L_02000406:
	ldr	r3, [r5, #12]
	movs	r2, #64
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02000420
	movs	r2, #255
	movs	r3, #1
	movs	r0, #111
	mov	sl, r2
	subs	r6, #1
	mov	r9, r3
	bl	sub_02000b5a
.L_02000420:
	ldr	r3, [r5, #12]
	movs	r2, #128
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02000438
	movs	r2, #1
	movs	r0, #111
	mov	sl, r2
	adds	r6, #1
	mov	r9, r2
	bl	sub_02000b72
.L_02000438:
	ldr	r3, [r5, #12]
	movs	r2, #16
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02000450
	movs	r3, #1
	movs	r0, #111
	mov	sl, r3
	adds	r6, #10
	mov	r9, r3
	bl	sub_02000b8a
.L_02000450:
	ldr	r3, [r5, #12]
	movs	r2, #32
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0200046a
	movs	r2, #255
	movs	r3, #1
	movs	r0, #111
	mov	sl, r2
	subs	r6, #10
	mov	r9, r3
	bl	sub_02000ba4
.L_0200046a:
	ldr	r3, [r5, #12]
	movs	r2, #128
	lsls	r2, r2, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02000484
	movs	r2, #1
	movs	r0, #111
	mov	sl, r2
	adds	r6, #30
	mov	r9, r2
	bl	sub_02000bbe
.L_02000484:
	ldr	r3, [r5, #12]
	movs	r2, #128
	lsls	r2, r2, #2
	ands	r3, r2
	mov	r8, r2
	cmp	r3, #0
	beq.n	.L_020004a2
	movs	r3, #255
	movs	r2, #1
	movs	r0, #111
	mov	sl, r3
	subs	r6, #30
	mov	r9, r2
	bl	sub_02000bdc
.L_020004a2:
	mov	r3, sl
	lsls	r5, r3, #24
	movs	r2, #1
	asrs	r3, r5, #24
	negs	r2, r2
	cmp	r3, r2
	bne.n	.L_020004de
	movs	r3, #128
	lsls	r3, r3, #2
	adds	r0, r6, r3
	mov	r1, r8
	b.n	.L_020004c6
.L_020004ba:
	movs	r2, #128
	lsls	r2, r2, #1
	adds	r2, #255
	movs	r1, #128
	adds	r0, r6, r2
	lsls	r1, r1, #2
.L_020004c6:
	bl	sub_02000afc
	adds	r6, r0, #0
	movs	r0, #128
	lsls	r0, r0, #1
	adds	r0, #255
	ands	r0, r6
	bl	sub_02000b92
	ldrh	r3, [r0, #6]
	cmp	r3, #0
	beq.n	.L_020004ba
.L_020004de:
	movs	r3, #128
	lsls	r3, r3, #17
	cmp	r5, r3
	bne.n	.L_02000514
	movs	r2, #128
	lsls	r2, r2, #2
	movs	r1, #128
	adds	r0, r6, r2
	b.n	.L_020004fa
.L_020004f0:
	movs	r3, #129
	lsls	r3, r3, #1
	adds	r3, #255
	movs	r1, #128
	adds	r0, r6, r3
.L_020004fa:
	lsls	r1, r1, #2
	bl	sub_02000b32
	adds	r6, r0, #0
	movs	r0, #128
	lsls	r0, r0, #1
	adds	r0, #255
	ands	r0, r6
	bl	sub_02000bc8
	ldrh	r3, [r0, #6]
	cmp	r3, #0
	beq.n	.L_020004f0
.L_02000514:
	movs	r2, #0
	movs	r0, #1
	mov	sl, r2
	bl	sub_02000b58
	b.n	.L_02000334
.L_02000520:
	adds	r0, r7, #0
	bl	sub_02000bb8
	movs	r0, #1
	bl	sub_02000b66
	adds	r0, r7, #0
	movs	r1, #1
	bl	sub_02000b86
	mov	r0, fp
	movs	r1, #1
	bl	sub_02000b8e
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x05000200
	.4byte 0x050001c0
	.4byte 0x050001e8
	.4byte 0x02008748
	.4byte 0x02008754
	.4byte 0x0000025f
	.4byte 0x00000092
	.4byte 0x0200876c
	.2byte 0x1150
	.2byte 0x0300
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r1, [r3, #108]
	movs	r3, #214
	lsls	r3, r3, #1
	adds	r2, r1, r3
	adds	r3, #85
	str	r3, [r2, #0]
	subs	r3, #77
	adds	r2, r1, r3
	movs	r3, #24
	str	r3, [r2, #0]
	bl	sub_02000cb0
	pop	{pc}
	push	{r5, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r1, [r3, #108]
	movs	r3, #214
	lsls	r3, r3, #1
	adds	r2, r1, r3
	adds	r3, #88
	str	r3, [r2, #0]
	subs	r3, #80
	adds	r2, r1, r3
	movs	r3, #24
	str	r3, [r2, #0]
	ldr	r3, [pc, #36]
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #89
	bgt.n	.L_020005cc
	movs	r0, #11
	bl	sub_02000cb2
	ldr	r5, [pc, #16]
	str	r5, [r0, #28]
	movs	r0, #11
	bl	sub_02000cbc
	str	r5, [r0, #24]
.L_020005cc:
	movs	r0, #0
	pop	{r5, pc}
	.4byte 0x02000240
	.2byte 0xb333
	.2byte 0x0001
	push	{lr}
	bl	sub_02000cc0
	movs	r0, #0
	bl	sub_02000d0e
	movs	r0, #10
	movs	r1, #2
	bl	sub_02000cee
	movs	r1, #4
	movs	r0, #11
	bl	sub_02000cf6
	movs	r0, #10
	bl	sub_02000d04
	movs	r1, #156
	movs	r2, #208
	movs	r0, #11
	lsls	r1, r1, #17
	lsls	r2, r2, #17
	bl	sub_02000d02
	bl	sub_02000cf6
	pop	{pc}
	.2byte 0x0000
	.4byte 0x049b23c0
	.4byte 0x681b33e0
	.4byte 0x33352201
	.2byte 0x701a
	.2byte 0x4770
	movs	r0, #0
	bx	lr
	push	{lr}
	bl	sub_02000ccc
	pop	{pc}
	push	{lr}
	bl	sub_02000cdc
	pop	{pc}
	.irp EntryTarget, 0x03000508, 0x080000c1, 0x080003c1, 0x08038011, 0x08038019, 0x08038079, 0x08038081, 0x08038091, 0x08038099, 0x080380a9, 0x080380b1, 0x08038121, 0x08038261, 0x08038269, 0x08038319, 0x08038321, 0x080ad009, 0x080ad011, 0x080ad029, 0x080ad101, 0x080ad199, 0x080ad1f9, 0x080c8019, 0x080c8021, 0x080c8089, 0x080c80f9, 0x080c8119, 0x080c8131, 0x080c8181, 0x080c81a1, 0x080c8279, 0x080c84e1, 0x080f8039, 0x081c0011
	overlay_veneer \EntryTarget
	.endr
	.4byte 0x0000764c
	.4byte 0x6d657449
	.4byte 0x3a6f4e20
	.4byte 0x00000000
	.4byte 0x65473a41
	.4byte 0x74492074
	.4byte 0x20206d65
	.4byte 0x65523a42
	.4byte 0x6e727574
	.4byte 0x00000000
	.4byte 0x4d455449
	.4byte 0x4c554620
	.4byte 0x2e2e2e4c
	.4byte 0x2e2e2e2e
	.4byte 0x0000002e
	.4byte 0xffff0000
	.4byte 0x00000050
	.4byte 0x40000048
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000a
	.4byte 0x00000050
	.4byte 0xc0000110
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0060
	.4byte 0x000000c0
	.4byte 0xc00000f0
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0061
	.4byte 0x000000c0
	.4byte 0xc00000f0
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0062
	.4byte 0x000000b0
	.4byte 0xc00000f0
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0063
	.4byte 0x000000b0
	.4byte 0xc00000f0
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x000001ff
	.4byte 0xffff0150
	.4byte 0x00000001
	.4byte 0x00300000
	.4byte 0x00000000
	.4byte 0x01100000
	.4byte 0x00002000
	.4byte 0xffff00fc
	.4byte 0x00000001
	.4byte 0x00300000
	.4byte 0x00000000
	.4byte 0x00480000
	.4byte 0x00002000
	.4byte 0xffff00fd
	.4byte 0x00000001
	.4byte 0x00300000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00002000
	.4byte 0xffff00d8
	.4byte 0x00000001
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x01100000
	.4byte 0x00002000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte 0x00000001
	.4byte 0x00a00000
	.4byte 0x00000000
	.4byte 0x00a00000
	.4byte 0x00022000
	.4byte 0xffff0012
	.4byte 0x00000001
	.4byte 0x00a00000
	.4byte 0x00000000
	.4byte 0x00c00000
	.4byte 0x00022000
	.4byte 0xffff0011
	.4byte 0x00000001
	.4byte 0x00a00000
	.4byte 0x00000000
	.4byte 0x00e00000
	.4byte 0x00022000
	.4byte 0xffff0014
	.4byte 0x00000001
	.4byte 0x00a00000
	.4byte 0x00000000
	.4byte 0x01000000
	.4byte 0x00022000
	.4byte 0xffff0013
	.4byte 0x00000001
	.4byte 0x00a00000
	.4byte 0x00000000
	.4byte 0x01200000
	.4byte 0x00022000
	.4byte 0xffff0018
	.4byte 0x00000001
	.4byte 0x00a00000
	.4byte 0x00000000
	.4byte 0x01400000
	.4byte 0x00022000
	.4byte 0xffff0019
	.4byte 0x00000001
	.4byte 0x00a00000
	.4byte 0x00000000
	.4byte 0x01600000
	.4byte 0x00022000
	.4byte 0xffff001a
	.4byte 0x00000001
	.4byte 0x00d00000
	.4byte 0x00000000
	.4byte 0x00a00000
	.4byte 0x00022000
	.4byte 0xffff0029
	.4byte 0x00000001
	.4byte 0x00d00000
	.4byte 0x00000000
	.4byte 0x00c00000
	.4byte 0x00022000
	.4byte 0xffff002a
	.4byte 0x00000001
	.4byte 0x00d00000
	.4byte 0x00000000
	.4byte 0x00e00000
	.4byte 0x00022000
	.4byte 0xffff0010
	.4byte 0x00000001
	.4byte 0x00d00000
	.4byte 0x00000000
	.4byte 0x01000000
	.4byte 0x00022000
	.4byte 0xffff0025
	.4byte 0x00000001
	.4byte 0x00d00000
	.4byte 0x00000000
	.4byte 0x01200000
	.4byte 0x00022000
	.4byte 0xffff0026
	.4byte 0x00000001
	.4byte 0x00d00000
	.4byte 0x00000000
	.4byte 0x01400000
	.4byte 0x00022000
	.4byte 0xffff0024
	.4byte 0x00000001
	.4byte 0x00d00000
	.4byte 0x00000000
	.4byte 0x01600000
	.4byte 0x00022000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff001d
	.4byte 0x00000001
	.4byte 0x01000000
	.4byte 0x00000000
	.4byte 0x00a00000
	.4byte 0x00022000
	.4byte 0xffff001e
	.4byte 0x00000001
	.4byte 0x01000000
	.4byte 0x00000000
	.4byte 0x00c00000
	.4byte 0x00022000
	.4byte 0xffff0020
	.4byte 0x00000001
	.4byte 0x01000000
	.4byte 0x00000000
	.4byte 0x00e00000
	.4byte 0x00022000
	.4byte 0xffff001f
	.4byte 0x00000001
	.4byte 0x01000000
	.4byte 0x00000000
	.4byte 0x01000000
	.4byte 0x00022000
	.4byte 0xffff0022
	.4byte 0x00000001
	.4byte 0x01000000
	.4byte 0x00000000
	.4byte 0x01200000
	.4byte 0x00022000
	.4byte 0xffff0017
	.4byte 0x00000001
	.4byte 0x01000000
	.4byte 0x00000000
	.4byte 0x01400000
	.4byte 0x00022000
	.4byte 0xffff0021
	.4byte 0x00000001
	.4byte 0x01000000
	.4byte 0x00000000
	.4byte 0x01600000
	.4byte 0x00022000
	.4byte 0xffff003c
	.4byte 0x00000001
	.4byte 0x00d00000
	.4byte 0x00000000
	.4byte 0x00a00000
	.4byte 0x00022000
	.4byte 0xffff003d
	.4byte 0x00000001
	.4byte 0x00d00000
	.4byte 0x00000000
	.4byte 0x00c00000
	.4byte 0x00022000
	.4byte 0xffff003e
	.4byte 0x00000001
	.4byte 0x00d00000
	.4byte 0x00000000
	.4byte 0x00e00000
	.4byte 0x00022000
	.4byte 0xffff002b
	.4byte 0x00000001
	.4byte 0x00d00000
	.4byte 0x00000000
	.4byte 0x01000000
	.4byte 0x00022000
	.4byte 0xffff002c
	.4byte 0x00000001
	.4byte 0x00d00000
	.4byte 0x00000000
	.4byte 0x01200000
	.4byte 0x00022000
	.4byte 0xffff002e
	.4byte 0x00000001
	.4byte 0x00d00000
	.4byte 0x00000000
	.4byte 0x01400000
	.4byte 0x00022000
	.4byte 0xffff002d
	.4byte 0x00000001
	.4byte 0x00d00000
	.4byte 0x00000000
	.4byte 0x01600000
	.4byte 0x00022000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0000
	.4byte 0x00000001
	.4byte 0x00a00000
	.4byte 0x00000000
	.4byte 0x00a00000
	.4byte 0x00022000
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00a00000
	.4byte 0x00000000
	.4byte 0x00c00000
	.4byte 0x00022000
	.4byte 0xffff0002
	.4byte 0x00000001
	.4byte 0x00a00000
	.4byte 0x00000000
	.4byte 0x00e00000
	.4byte 0x00022000
	.4byte 0xffff0003
	.4byte 0x00000001
	.4byte 0x00a00000
	.4byte 0x00000000
	.4byte 0x01000000
	.4byte 0x00022000
	.4byte 0xffff0005
	.4byte 0x00000001
	.4byte 0x00a00000
	.4byte 0x00000000
	.4byte 0x01200000
	.4byte 0x00022000
	.4byte 0xffff0006
	.4byte 0x00000001
	.4byte 0x00a00000
	.4byte 0x00000000
	.4byte 0x01400000
	.4byte 0x00022000
	.4byte 0xffff0007
	.4byte 0x00000001
	.4byte 0x00a00000
	.4byte 0x00000000
	.4byte 0x01600000
	.4byte 0x00022000
	.4byte 0xffff0039
	.4byte 0x00000001
	.4byte 0x00d00000
	.4byte 0x00000000
	.4byte 0x00a00000
	.4byte 0x00022000
	.4byte 0xffff0038
	.4byte 0x00000001
	.4byte 0x00d00000
	.4byte 0x00000000
	.4byte 0x00c00000
	.4byte 0x00022000
	.4byte 0xffff003b
	.4byte 0x00000001
	.4byte 0x00d00000
	.4byte 0x00000000
	.4byte 0x00e00000
	.4byte 0x00022000
	.4byte 0xffff00c9
	.4byte 0x00000001
	.4byte 0x00d00000
	.4byte 0x00000000
	.4byte 0x01000000
	.4byte 0x00022000
	.4byte 0xffff00ca
	.4byte 0x00000001
	.4byte 0x00d00000
	.4byte 0x00000000
	.4byte 0x01200000
	.4byte 0x00022000
	.4byte 0xffff0040
	.4byte 0x00000001
	.4byte 0x00d00000
	.4byte 0x00000000
	.4byte 0x01500000
	.4byte 0x00022000
	.4byte 0xffff0041
	.4byte 0x00000001
	.4byte 0x00d00000
	.4byte 0x00000000
	.4byte 0x01700000
	.4byte 0x00022000
	.4byte 0xffff003f
	.4byte 0x00000001
	.4byte 0x00d00000
	.4byte 0x00000000
	.4byte 0x01900000
	.4byte 0x00022000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00cd
	.4byte 0x00000001
	.4byte 0x00800000
	.4byte 0x00000000
	.4byte 0x00a00000
	.4byte 0x00022000
	.4byte 0xffff00ce
	.4byte 0x00000001
	.4byte 0x00800000
	.4byte 0x00000000
	.4byte 0x00c00000
	.4byte 0x00022000
	.4byte 0xffff00c6
	.4byte 0x00000001
	.4byte 0x00800000
	.4byte 0x00000000
	.4byte 0x00e00000
	.4byte 0x00022000
	.4byte 0xffff0079
	.4byte 0x00000001
	.4byte 0x00800000
	.4byte 0x00000000
	.4byte 0x01000000
	.4byte 0x00022000
	.4byte 0xffff0077
	.4byte 0x00000001
	.4byte 0x00800000
	.4byte 0x00000000
	.4byte 0x01200000
	.4byte 0x00022000
	.4byte 0xffff007b
	.4byte 0x00000001
	.4byte 0x00800000
	.4byte 0x00000000
	.4byte 0x01400000
	.4byte 0x00022000
	.4byte 0xffff0068
	.4byte 0x00000001
	.4byte 0x00b00000
	.4byte 0x00000000
	.4byte 0x00a00000
	.4byte 0x00022000
	.4byte 0xffff0069
	.4byte 0x00000001
	.4byte 0x00b00000
	.4byte 0x00000000
	.4byte 0x00c00000
	.4byte 0x00022000
	.4byte 0xffff0081
	.4byte 0x00000001
	.4byte 0x00b00000
	.4byte 0x00000000
	.4byte 0x00e00000
	.4byte 0x00022000
	.4byte 0xffff0082
	.4byte 0x00000001
	.4byte 0x00b00000
	.4byte 0x00000000
	.4byte 0x01000000
	.4byte 0x00022000
	.4byte 0xffff00c7
	.4byte 0x00000001
	.4byte 0x00b00000
	.4byte 0x00000000
	.4byte 0x01200000
	.4byte 0x00022000
	.4byte 0xffff00d1
	.4byte 0x00000001
	.4byte 0x00b00000
	.4byte 0x00000000
	.4byte 0x01400000
	.4byte 0x00022000
	.4byte 0xffff006c
	.4byte 0x00000001
	.4byte 0x00e00000
	.4byte 0x00000000
	.4byte 0x00a00000
	.4byte 0x00022000
	.4byte 0xffff006d
	.4byte 0x00000001
	.4byte 0x00e00000
	.4byte 0x00000000
	.4byte 0x00c00000
	.4byte 0x00022000
	.4byte 0xffff006e
	.4byte 0x00000001
	.4byte 0x00e00000
	.4byte 0x00000000
	.4byte 0x00e00000
	.4byte 0x00022000
	.4byte 0xffff006f
	.4byte 0x00000001
	.4byte 0x00e00000
	.4byte 0x00000000
	.4byte 0x01000000
	.4byte 0x00022000
	.4byte 0xffff0086
	.4byte 0x00000001
	.4byte 0x00e00000
	.4byte 0x00000000
	.4byte 0x01200000
	.4byte 0x00022000
	.4byte 0xffff0087
	.4byte 0x00000001
	.4byte 0x00e00000
	.4byte 0x00000000
	.4byte 0x01400000
	.4byte 0x00022000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x020082cd
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x02008625
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x0200862d
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x020081a9
	.4byte 0x00008e15
	.4byte 0xffff000a
	.4byte 0x020085d9
	.4byte 0x10008e15
	.4byte 0xffff000a
	.4byte 0x02008611
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0000
	.4byte 0x020080fd
	.4byte 0x00000000
	.4byte 0xffff0001
	.4byte 0x020080fd
	.4byte 0x00000000
	.4byte 0xffff0002
	.4byte 0x020080fd
	.4byte 0x00000000
	.4byte 0xffff0003
	.4byte 0x020080fd
	.4byte 0x00000000
	.4byte 0xffff0005
	.4byte 0x020080fd
	.4byte 0x00000000
	.4byte 0xffff0006
	.4byte 0x020080fd
	.4byte 0x00000000
	.4byte 0xffff0007
	.4byte 0x020080fd
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x02008099
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x02008099
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x02008099
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x02008099
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x02008099
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x02008099
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x02008099
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x02008099
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x02008099
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x02008099
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x02008099
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte 0x02008099
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte 0x02008099
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte 0x02008099
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte 0x02008099
	.4byte 0x00000000
	.4byte 0xffff0017
	.4byte 0x02008099
	.4byte 0x00000000
	.4byte 0xffff0018
	.4byte 0x02008099
	.4byte 0x00000000
	.4byte 0xffff0019
	.4byte 0x02008099
	.4byte 0x00000000
	.4byte 0xffff001a
	.4byte 0x02008099
	.4byte 0x00000000
	.4byte 0xffff001b
	.4byte 0x02008099
	.4byte 0x00000000
	.4byte 0xffff001c
	.4byte 0x02008099
	.4byte 0x00000000
	.4byte 0xffff001d
	.4byte 0x02008099
	.4byte 0x00000000
	.4byte 0xffff001e
	.4byte 0x02008099
	.4byte 0x00000000
	.4byte 0xffff001f
	.4byte 0x02008099
	.4byte 0x00000000
	.4byte 0xffff0020
	.4byte 0x02008099
	.4byte 0x00008e15
	.4byte 0xffff000a
	.4byte 0x020085d9
	.4byte 0x10008e15
	.4byte 0xffff000a
	.4byte 0x02008611
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
