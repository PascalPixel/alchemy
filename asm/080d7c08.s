@ 呼出しグラフから到達した領域の再構築サム逆アセンブル。
@ （コードとデータが混在）。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.set sub_080022fc, 0x080022fc
	.set sub_080030f8, 0x080030f8
	.set sub_08004458, 0x08004458
	.set sub_080072f4, 0x080072f4
	.set sub_08009038, 0x08009038
	.set sub_080d67dc, 0x080d67dc
	.set sub_080d7814, 0x080d7814
	.set sub_080d7bba, 0x080d7bba
	.set sub_080d7ea0, 0x080d7ea0
	.set sub_080f9010, 0x080f9010
	.global Region_080d7c08
Region_080d7c08:
	movs r4, #3
	mov r0, r8
	ands r4, r0
	ldr r2, [pc, #592]
	ldr r0, [pc, #592]
	lsls r3, r4, #1
	ldrh r1, [r2, r3]
	ldrb r0, [r0, r4]
	ldr r2, [sp, #44]
	adds r1, r2, r1
	movs r3, #2
	ldrsh r2, [r5, r3]
	movs r7, #6
	ldrsh r3, [r5, r7]
	str r0, [sp, #0]
	ldr r0, [pc, #576]
	ldrb r0, [r0, r4]
	ldr r4, [sp, #32]
	str r0, [sp, #4]
	ldr r0, [sp, #48]
	bl sub_080072f4
	ldr r2, [r5, #0]
	ldr r3, [r5, #12]
	adds r2, r2, r3
	str r2, [r5, #0]
	ldr r3, [r5, #4]
	ldr r2, [r5, #16]
	movs r7, #128
	lsls r7, r7, #6
	adds r3, r3, r2
	adds r2, r2, r7
	str r3, [r5, #4]
	str r2, [r5, #16]
	movs r0, #1
	add r8, r0
	mov r1, r8
	adds r5, #28
	cmp r1, #64
	bne sub_080d7bba
	ldr r2, [sp, #40]
	cmp r2, #223
	bgt .L_080d7c60
	b .L_080d7dc8
.L_080d7c60:
	cmp r2, #224
	bne .L_080d7ca4
	movs r3, #0
	ldr r5, [pc, #516]
	mov r8, r3
	movs r6, #127
.L_080d7c6c:
	movs r3, #144
	lsls r3, r3, #15
	str r3, [r5, #0]
	movs r3, #224
	lsls r3, r3, #14
	str r3, [r5, #4]
	bl sub_08004458
	ands r0, r6
	negs r0, r0
	subs r0, #64
	lsls r0, r0, #11
	str r0, [r5, #12]
	bl sub_08004458
	ands r0, r6
	adds r0, #16
	lsls r0, r0, #11
	str r0, [r5, #16]
	bl sub_08004458
	movs r4, #1
	add r8, r4
	mov r7, r8
	str r0, [r5, #24]
	adds r5, #28
	cmp r7, #128
	bne .L_080d7c6c
.L_080d7ca4:
	movs r0, #0
	ldr r5, [pc, #452]
	mov r8, r0
.L_080d7caa:
	mov r3, r8
	cmp r3, #0
	bge .L_080d7cb2
	adds r3, #3
.L_080d7cb2:
	asrs r3, r3, #2
	ldr r1, [sp, #40]
	adds r3, #224
	cmp r1, r3
	blt .L_080d7d28
	mov r0, r8
	movs r1, #3
	bl sub_080022fc
	movs r3, #1
	mov r2, r8
	ands r3, r2
	adds r4, r0, #0
	cmp r3, #0
	bne .L_080d7cf6
	ldr r2, [pc, #412]
	lsls r3, r4, #1
	ldrh r1, [r2, r3]
	ldr r3, [sp, #44]
	adds r1, r3, r1
	movs r0, #6
	ldrsh r3, [r5, r0]
	ldr r0, [pc, #404]
	ldrb r0, [r0, r4]
	movs r7, #2
	ldrsh r2, [r5, r7]
	str r0, [sp, #0]
	ldr r0, [pc, #396]
	ldrb r0, [r0, r4]
	ldr r4, [sp, #36]
	str r0, [sp, #4]
	ldr r0, [sp, #48]
	bl sub_080072f4
.L_080d7cf6:
	ldr r2, [r5, #0]
	ldr r3, [r5, #12]
	ldr r1, [r5, #4]
	adds r2, r2, r3
	ldr r3, [r5, #16]
	movs r7, #16
	str r2, [r5, #0]
	adds r1, r1, r3
	asrs r2, r2, #16
	negs r7, r7
	str r1, [r5, #4]
	cmp r2, r7
	blt .L_080d7d16
	asrs r3, r1, #16
	cmp r3, #120
	ble .L_080d7d22
.L_080d7d16:
	movs r3, #144
	lsls r3, r3, #15
	str r3, [r5, #0]
	movs r3, #224
	lsls r3, r3, #14
	str r3, [r5, #4]
.L_080d7d22:
	ldr r3, [r5, #24]
	adds r3, #1
	str r3, [r5, #24]
.L_080d7d28:
	movs r0, #1
	add r8, r0
	mov r1, r8
	adds r5, #28
	cmp r1, #128
	bne .L_080d7caa
	ldr r2, [sp, #40]
	cmp r2, #228
	bne .L_080d7d5c
	movs r3, #0
	ldr r2, [pc, #316]
	ldr r1, [pc, #300]
	mov r8, r3
	movs r0, #0
.L_080d7d44:
	ldr r3, [r1, #0]
	str r3, [r2, #0]
	movs r4, #1
	ldr r3, [r1, #4]
	add r8, r4
	mov r5, r8
	str r3, [r2, #4]
	str r0, [r2, #24]
	adds r1, #28
	adds r2, #28
	cmp r5, #128
	bne .L_080d7d44
.L_080d7d5c:
	movs r7, #0
	ldr r5, [pc, #284]
	ldr r6, [pc, #264]
	mov r8, r7
.L_080d7d64:
	mov r3, r8
	ldr r0, [sp, #40]
	adds r3, #228
	cmp r0, r3
	blt .L_080d7dba
	ldr r0, [r5, #24]
	lsrs r3, r0, #31
	adds r0, r0, r3
	movs r1, #9
	asrs r0, r0, #1
	bl sub_080022fc
	ldr r3, [pc, #256]
	ldrb r4, [r3, r0]
	ldr r3, [pc, #256]
	lsls r0, r0, #1
	ldrh r1, [r3, r0]
	ldr r2, [sp, #28]
	adds r1, r2, r1
	movs r3, #2
	ldrsh r2, [r5, r3]
	movs r7, #6
	ldrsh r3, [r5, r7]
	lsrs r0, r4, #1
	subs r3, r3, r0
	subs r2, r2, r0
	str r4, [sp, #0]
	str r4, [sp, #4]
	ldr r0, [sp, #48]
	ldr r4, [sp, #36]
	bl sub_080072f4
	ldr r3, [r5, #24]
	adds r3, #1
	str r3, [r5, #24]
	cmp r3, #18
	bne .L_080d7dba
	ldr r3, [r6, #0]
	str r3, [r5, #0]
	ldr r3, [r6, #4]
	str r3, [r5, #4]
	movs r3, #0
	str r3, [r5, #24]
.L_080d7dba:
	movs r7, #1
	add r8, r7
	mov r0, r8
	adds r5, #28
	adds r6, #28
	cmp r0, #128
	bne .L_080d7d64
.L_080d7dc8:
	ldr r1, [sp, #44]
	ldr r3, [pc, #188]
	adds r2, r1, r3
	movs r3, #1
	str r3, [r2, #0]
	movs r0, #1
	bl sub_080030f8
	ldr r4, [sp, #40]
	movs r5, #144
	adds r4, #1
	lsls r5, r5, #1
	str r4, [sp, #40]
	cmp r4, r5
	beq .L_080d7dfe
	ldr r3, [pc, #164]
	ldr r3, [r3, #0]
	movs r2, #3
	movs r7, #16
	ands r3, r2
	mov r10, r7
	cmp r3, #0
	bne .L_080d7df8
	b sub_080d7814
.L_080d7df8:
	cmp r4, #16
	bgt .L_080d7dfe
	b sub_080d7814
.L_080d7dfe:
	ldr r1, [sp, #44]
	ldr r2, [pc, #140]
	movs r0, #0
	mov r8, r0
	adds r5, r1, r2
.L_080d7e08:
	ldmia r5!, {r0}
	bl sub_08009038
	movs r3, #1
	add r8, r3
	mov r4, r8
	cmp r4, #9
	bne .L_080d7e08
	bl sub_080d67dc
	ldr r3, [pc, #116]
	ldr r2, [pc, #60]
	ldr r0, [pc, #116]
	strh r2, [r3, #0]
	bl sub_080f9010
	movs r1, #225
	ldr r0, [sp, #44]
	movs r5, #0
	lsls r1, r1, #7
	mov r8, r5
	movs r7, #127
	movs r6, #63
	adds r5, r0, r1
	bl sub_08004458
	ands r0, r7
	ldr r3, [pc, #92]
	adds r0, #64
	lsls r0, r0, #16
	str r3, [r5, #8]
	str r0, [r5, #0]
	bl sub_08004458
	ands r0, r7
	negs r0, r0
	subs r0, #64
	lsls r0, r0, #16
	str r0, [r5, #4]
	bl sub_08004458
	b sub_080d7ea0
