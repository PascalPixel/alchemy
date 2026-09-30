.syntax unified
	.thumb
	.global Func_0812628c
	.thumb_func
Func_0812628c:
	push {r5, r6, r7, lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #176
	ldr r6, [r3]
	adds r7, r0, #0
	ldr r3, [r6, #8]
	adds r5, r2, #0
	cmp r3, #0
	bne .L_081262ac
	movs r1, #192
	lsls r1, r1, #4
	ldr r0, .L_08126370
	adds r1, #255
	bl Func_080145a8
.L_081262ac:
	str r7, [r6, #8]
	cmp r7, #1
	bne .L_081262e8
	ldr r1, .L_08126374
	ldr r0, .L_08126378
	ldrh r3, [r0]
	adds r4, r3, #0
	strh r0, [r0]
	ldrh r2, [r1]
	cmp r2, #31
	bgt .L_081262e6
	lsls r3, r2, #1
	adds r3, r3, r2
	adds r2, #1
	lsls r3, r3, #2
	strh r2, [r1]
	movs r2, #248
	adds r3, r3, r1
	lsls r2, r2, #5
	adds r3, #4
	adds r2, #131
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #10
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #10
	str r2, [r3]
.L_081262e6:
	strh r4, [r0]
.L_081262e8:
	movs r3, #128
	movs r1, #160
	movs r2, #128
	lsls r3, r3, #19
	lsls r1, r1, #19
	lsls r2, r2, #24
	adds r3, #212
	ldr r0, .L_0812637c
	adds r1, #160
	adds r2, #16
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r2, .L_08126380
	movs r3, #160
	ldrh r2, [r2]
	lsls r3, r3, #19
	adds r3, #188
	strh r2, [r3]
	cmp r5, #128
	bne .L_08126332
	movs r3, #192
	lsls r3, r3, #18
	ldr r0, [r3, #36]
	movs r2, #160
	lsls r2, r2, #3
	adds r2, #108
	adds r0, r0, r2
	movs r3, #128
	movs r2, #128
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	adds r1, #32
	adds r2, #128
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	b .L_081263ae
.L_08126332:
	cmp r5, #0
	beq .L_081263ae
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #36]
	movs r2, #160
	lsls r2, r2, #3
	adds r2, #108
	movs r4, #160
	ldr r7, .L_0812636c
	adds r2, r2, r3
	lsls r4, r4, #19
	mov r12, r2
	movs r6, #0
	adds r4, #192
	movs r0, #0
.L_08126352:
	mov r2, r12
	ldrh r3, [r0, r2]
	movs r1, #31
	ands r1, r3
	lsls r3, r3, #16
	lsrs r2, r3, #21
	lsrs r3, r3, #26
	ands r2, r7
	ands r3, r7
	cmp r1, r5
	ble .L_08126384
	subs r1, r1, r5
	b .L_08126386
.L_0812636c:
	.4byte 0x0000001f
.L_08126370:
	.4byte Func_08125c0c
.L_08126374:
	.4byte Data_020038e0
.L_08126378:
	.4byte 0x04000208
.L_0812637c:
	.4byte 0x05000200
.L_08126380:
	.4byte 0x050001e8
.L_08126384:
	movs r1, #0
.L_08126386:
	cmp r2, r5
	ble .L_0812638e
	subs r2, r2, r5
	b .L_08126390
.L_0812638e:
	movs r2, #0
.L_08126390:
	cmp r3, r5
	ble .L_08126398
	subs r3, r3, r5
	b .L_0812639a
.L_08126398:
	movs r3, #0
.L_0812639a:
	lsls r3, r3, #10
	lsls r2, r2, #5
	orrs r3, r2
	orrs r3, r1
	adds r6, #1
	strh r3, [r4]
	adds r0, #2
	adds r4, #2
	cmp r6, #128
	bne .L_08126352
.L_081263ae:
	ldr r0, .L_081263bc
	bl Graphics_BuildSequentialTileTable
	ldr r0, .L_081263c0
	bl Func_08125bb8
	pop {r5, r6, r7, pc}
.L_081263bc:
	.4byte 0x06003800
.L_081263c0:
	.4byte 0x0600f800
