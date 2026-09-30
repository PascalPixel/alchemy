.syntax unified
	.thumb
	.global Func_080eb594
	.thumb_func
Func_080eb594:
	push {r5, lr}
	movs r2, #192
	lsls r2, r2, #18
	adds r3, r2, #0
	adds r3, #240
	ldr r3, [r3]
	ldr r4, [r2, #96]
	mov r12, r3
	adds r3, #188
	ldrh r3, [r3]
	ldr r2, .L_080eb68c
	lsls r3, r3, #5
	adds r1, r3, r2
	mov r2, r12
	adds r2, #164
	ldr r3, [r2]
	mov r5, r12
	adds r3, #1
	str r3, [r2]
	adds r5, #193
	movs r3, #0
	ldrsb r3, [r5, r3]
	subs r3, #1
	cmp r3, #10
	bhi .L_080eb686
	ldr r2, .L_080eb690
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
	.2byte 0x0000
.L_080eb5d0:
	.4byte .L_080eb608
	.4byte .L_080eb5fc
	.4byte .L_080eb626
	.4byte .L_080eb640
	.4byte .L_080eb65a
	.4byte .L_080eb686
	.4byte .L_080eb686
	.4byte .L_080eb686
	.4byte .L_080eb686
	.4byte .L_080eb686
	.4byte .L_080eb674
.L_080eb5fc:
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #212
	adds r0, r4, #0
	ldr r2, .L_080eb694
	b .L_080eb682
.L_080eb608:
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #212
	adds r0, r4, #0
	ldr r2, .L_080eb694
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r1, #128
	ldr r3, .L_080eb698
	adds r0, r4, #0
	lsls r1, r1, #5
	movs r2, #0
	mov lr, r3
	.2byte 0xf800
	b .L_080eb686
.L_080eb626:
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #212
	adds r0, r4, #0
	ldr r2, .L_080eb694
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r1, #128
	lsls r1, r1, #5
	adds r0, r4, #0
	bl Func_080eb51c
	b .L_080eb686
.L_080eb640:
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #212
	adds r0, r4, #0
	ldr r2, .L_080eb694
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r1, #128
	lsls r1, r1, #5
	adds r0, r4, #0
	bl Func_080eb544
	b .L_080eb686
.L_080eb65a:
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #212
	adds r0, r4, #0
	ldr r2, .L_080eb694
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r1, #128
	lsls r1, r1, #5
	adds r0, r4, #0
	bl Func_080eb56c
	b .L_080eb686
.L_080eb674:
	movs r3, #128
	lsls r3, r3, #19
	movs r1, #192
	adds r3, #212
	adds r0, r4, #0
	lsls r1, r1, #19
	ldr r2, .L_080eb69c
.L_080eb682:
	stmia r3!, {r0, r1, r2}
	subs r3, #12
.L_080eb686:
	movs r3, #0
	strb r3, [r5]
	pop {r5, pc}
.L_080eb68c:
	.4byte 0x06010000
.L_080eb690:
	.4byte .L_080eb5d0
.L_080eb694:
	.4byte 0x84000400
.L_080eb698:
	.4byte IwramFillWords
.L_080eb69c:
	.4byte 0x84000800
