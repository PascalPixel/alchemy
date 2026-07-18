@ 呼出しグラフから到達した領域の再構築サム逆アセンブル。
@ （コードとデータが混在）。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.set sub_08015118, 0x08015118
	.set sub_08015120, 0x08015120
	.set sub_080151c8, 0x080151c8
	.set sub_08077008, 0x08077008
	.set sub_080770c0, 0x080770c0
	.set sub_08077118, 0x08077118
	.set sub_080771a0, 0x080771a0
	.set sub_080b9a44, 0x080b9a44
	.set sub_080bb938, 0x080bb938
	.set sub_080bbabc, 0x080bbabc
	.set sub_080bdfec, 0x080bdfec
	.set sub_080bec5c, 0x080bec5c
	.set sub_080bec8a, 0x080bec8a
	.set sub_080bee00, 0x080bee00
	.global Region_080be378
Region_080be378:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #48
	mov r3, sp
	add r2, sp, #32
	adds r3, #44
	str r0, [r2, #0]
	str r3, [sp, #8]
	str r1, [r3, #0]
	ldr r3, [r2, #0]
	movs r4, #0
	ldrsh r0, [r3, r4]
	mov r10, r2
	bl sub_08077008
	mov r1, sp
	adds r1, #28
	str r1, [sp, #12]
	ldr r3, [pc, #912]
	ldr r3, [r3, #0]
	add r7, sp, #36
	mov r2, r10
	str r3, [r7, #0]
	ldr r3, [r2, #0]
	str r0, [r1, #0]
	movs r4, #10
	ldrsh r0, [r3, r4]
	bl sub_080b9a44
	str r0, [sp, #40]
	bl sub_080bdfec
	mov r4, r10
	ldr r0, [sp, #8]
	ldr r3, [r4, #0]
	ldr r1, [r0, #0]
	ldrh r3, [r3, #0]
	movs r2, #0
	movs r5, #4
	strb r3, [r1, #0]
	str r2, [r1, #96]
	strb r2, [r1, #1]
	str r2, [r1, #88]
	str r2, [r1, #92]
	str r5, [r1, #80]
	bl sub_08015118
	ldr r0, [sp, #12]
	ldr r3, [r0, #0]
	movs r1, #56
	ldrsh r3, [r3, r1]
	cmp r3, #0
	bne .L_080be3f0
	bl sub_080bec5c
.L_080be3f0:
	ldr r3, [pc, #840]
	ldrb r3, [r3, #0]
	cmp r3, #0
	beq .L_080be46a
	ldr r0, [pc, #836]
	bl sub_080770c0
	cmp r0, #0
	beq .L_080be46a
	ldr r1, [pc, #832]
	movs r2, #128
	ldr r3, [r1, #0]
	lsls r2, r2, #1
	ands r3, r2
	cmp r3, #0
	beq .L_080be46a
	ldr r3, [r1, #0]
	movs r2, #1
	ands r3, r5
	mov r8, r2
	cmp r3, #0
	beq .L_080be420
	movs r3, #0
	mov r8, r3
.L_080be420:
	movs r6, #100
	b .L_080be448
.L_080be424:
	cmp r5, #254
	beq .L_080be446
	movs r1, #192
	adds r0, r5, #0
	lsls r1, r1, #24
	bl sub_08077118
	cmp r0, #0
	bne .L_080be446
	adds r1, r5, #0
	movs r0, #8
	bl sub_080bbabc
	movs r0, #9
	adds r1, r5, #0
	bl sub_080bbabc
.L_080be446:
	adds r6, #2
.L_080be448:
	mov r4, r8
	cmp r4, #0
	beq .L_080be456
	ldr r3, [r7, #0]
	adds r3, #2
	ldrsh r5, [r3, r6]
	b .L_080be45e
.L_080be456:
	ldr r2, [r7, #0]
	adds r3, r6, #0
	subs r3, #12
	ldrsh r5, [r2, r3]
.L_080be45e:
	cmp r5, #255
	bne .L_080be424
	bl sub_080bb938
	bl sub_080bec5c
.L_080be46a:
	bl sub_08015118
	ldr r3, [sp, #12]
	ldr r4, [pc, #724]
	ldr r2, [r3, #0]
	adds r1, r2, r4
	ldrb r3, [r1, #0]
	cmp r3, #0
	beq .L_080be498
	movs r3, #0
	mov r0, r10
	strb r3, [r1, #0]
	ldr r3, [r0, #0]
	movs r1, #0
	ldrsh r0, [r3, r1]
	movs r1, #1
	bl sub_08015120
	ldr r0, [pc, #700]
	bl sub_080151c8
	bl sub_080bec8a
.L_080be498:
	movs r4, #158
	lsls r4, r4, #1
	adds r3, r2, r4
	ldrb r3, [r3, #0]
	cmp r3, #0
	beq .L_080be4bc
	mov r0, r10
	ldr r3, [r0, #0]
	movs r1, #0
	ldrsh r0, [r3, r1]
	movs r1, #1
	bl sub_08015120
	ldr r0, [pc, #668]
	bl sub_080151c8
	bl sub_080bec8a
.L_080be4bc:
	ldr r4, [pc, #660]
	adds r3, r2, r4
	ldrb r3, [r3, #0]
	cmp r3, #0
	beq .L_080be4dc
	mov r0, r10
	ldr r3, [r0, #0]
	movs r1, #0
	ldrsh r0, [r3, r1]
	movs r1, #1
	bl sub_08015120
	ldr r0, [pc, #640]
	bl sub_080151c8
	b sub_080bec8a
.L_080be4dc:
	movs r4, #152
	lsls r4, r4, #1
	adds r3, r2, r4
	ldrb r2, [r3, #0]
	movs r3, #1
	ands r3, r2
	cmp r3, #0
	beq .L_080be51a
	mov r0, r10
	ldr r3, [r0, #0]
	movs r1, #6
	ldrsh r3, [r3, r1]
	cmp r3, #3
	beq .L_080be51a
	bl sub_080771a0
	movs r3, #3
	ands r0, r3
	cmp r0, #0
	bne .L_080be51a
	mov r2, r10
	ldr r3, [r2, #0]
	movs r1, #1
	movs r4, #0
	ldrsh r0, [r3, r4]
	bl sub_08015120
	ldr r0, [pc, #584]
	bl sub_080151c8
	b sub_080bec8a
.L_080be51a:
	mov r0, r10
	ldr r3, [r0, #0]
	movs r1, #6
	ldrsh r3, [r3, r1]
	cmp r3, #8
	bne .L_080be528
	b sub_080bec5c
.L_080be528:
	ldr r4, [sp, #8]
	ldr r3, [r4, #0]
	movs r2, #1
	mov r11, r2
	movs r1, #0
	adds r3, #44
	movs r2, #13
.L_080be536:
	subs r2, #1
	strb r1, [r3, #0]
	adds r3, #1
	cmp r2, #0
	bge .L_080be536
	ldr r0, [sp, #8]
	movs r2, #1
	ldr r3, [r0, #0]
	negs r2, r2
	adds r1, r2, #0
	adds r3, #58
	movs r2, #13
.L_080be54e:
	subs r2, #1
	strb r1, [r3, #0]
	adds r3, #1
	cmp r2, #0
	bge .L_080be54e
	mov r4, r10
	ldr r3, [r4, #0]
	movs r0, #6
	ldrsh r3, [r3, r0]
	cmp r3, #99
	bls .L_080be568
	bl sub_080bee00
.L_080be568:
	ldr r2, [pc, #500]
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
