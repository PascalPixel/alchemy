.syntax unified
	.thumb
	.global MenuSelection_SetupEntry
	.thumb_func
MenuSelection_SetupEntry:
	push {r5, r6, r7, lr}
	adds r7, r0, #0
	adds r6, r1, #0
	sub sp, #12
	adds r5, r2, #0
	adds r1, r3, #0
	cmp r7, #2
	beq .L_0803f320
	cmp r7, #2
	bhi .L_0803f2fe
	cmp r7, #1
	beq .L_0803f306
	b .L_0803f356
.L_0803f2fe:
	cmp r7, #4
	beq .L_0803f33a
	cmp r7, #6
	bne .L_0803f356
.L_0803f306:
	cmp r1, #0
	beq .L_0803f30e
	ldrh r3, [r5, #12]
	str r3, [sp, #8]
.L_0803f30e:
	add r3, sp, #4
	str r1, [sp, #0]
	add r2, sp, #8
	adds r0, r6, #0
	movs r1, #0
	bl Ui_BuildPairedPatternsToSlot
	ldr r3, .L_0803f3b8
	b .L_0803f352
.L_0803f320:
	cmp r1, #0
	beq .L_0803f328
	ldrh r3, [r5, #12]
	str r3, [sp, #8]
.L_0803f328:
	add r3, sp, #4
	str r1, [sp, #0]
	add r2, sp, #8
	adds r0, r6, #0
	movs r1, #1
	bl Func_0803d5c4
	ldr r3, .L_0803f3bc
	b .L_0803f352
.L_0803f33a:
	cmp r1, #0
	beq .L_0803f342
	ldrh r3, [r5, #12]
	str r3, [sp, #8]
.L_0803f342:
	add r3, sp, #4
	str r1, [sp, #0]
	add r2, sp, #8
	adds r0, r6, #0
	movs r1, #1
	bl Ability_LoadGlyph
	ldr r3, .L_0803f3c0
.L_0803f352:
	adds r3, r6, r3
	strh r3, [r5, #32]
.L_0803f356:
	ldr r3, [sp, #8]
	strh r6, [r5, #8]
	ldr r6, [sp, #4]
	strh r3, [r5, #12]
	movs r3, #128
	lsls r3, r3, #1
	strh r6, [r5, #14]
	strh r7, [r5, #10]
	strh r3, [r5, #34]
	strh r3, [r5, #38]
	adds r0, r5, #0
	adds r0, #40
	ldrb r3, [r0, #5]
	movs r5, #13
	negs r5, r5
	adds r2, r5, #0
	ands r2, r3
	movs r3, #33
	negs r3, r3
	ldrb r1, [r0, #7]
	ands r2, r3
	adds r3, #16
	movs r4, #63
	ands r2, r3
	ands r2, r4
	adds r3, r4, #0
	ands r3, r1
	strb r2, [r0, #5]
	movs r1, #64
	ldrb r2, [r0, #9]
	orrs r3, r1
	strb r3, [r0, #7]
	movs r3, #15
	ands r3, r2
	strb r3, [r0, #9]
	movs r3, #192
	lsls r3, r3, #2
	adds r3, #255
	ldrh r2, [r0, #8]
	ands r6, r3
	ldr r3, .L_0803f3c4
	add sp, #12
	ands r3, r2
	orrs r3, r6
	strh r3, [r0, #8]
	ldrb r3, [r0, #9]
	ands r5, r3
	strb r5, [r0, #9]
	pop {r5, r6, r7, pc}
.L_0803f3b8:
	.4byte 0x0000003a
.L_0803f3bc:
	.4byte 0x0000025f
.L_0803f3c0:
	.4byte 0x000005a7
.L_0803f3c4:
	.4byte 0xfffffc00
