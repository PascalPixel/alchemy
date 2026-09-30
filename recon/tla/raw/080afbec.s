.syntax unified
	.thumb
	.global Func_080afbec
	.thumb_func
Func_080afbec:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #48
	adds r6, r1, #0
	str r0, [sp, #12]
	str r1, [sp, #8]
	mov r8, r2
	subs r6, #8
	cmp r0, #127
	ble .L_080afc10
	ldr r0, [sp, #12]
	cmp r0, #134
	ble .L_080afc14
.L_080afc10:
	movs r0, #0
	b .L_080afd9c
.L_080afc14:
	movs r1, #248
	lsls r1, r1, #2
	movs r0, #0
	cmp r6, r1
	bls .L_080afc20
	b .L_080afd9c
.L_080afc20:
	ldr r0, [sp, #12]
	bl Owner_GetState
	movs r1, #166
	ldr r3, .L_080afdac
	lsls r1, r1, #1
	adds r5, r0, #0
	mov lr, r3
	.2byte 0xf800
	movs r2, #190
	lsls r2, r2, #1
	cmp r6, r2
	bcc .L_080afc3c
	movs r6, #0
.L_080afc3c:
	movs r3, #76
	adds r2, r6, #0
	muls r2, r3
	ldr r3, .L_080afdb0
	ldr r0, .L_080afdb4
	adds r4, r2, r3
	ldrb r3, [r4, #15]
	ldrb r2, [r4, #28]
	strb r3, [r5, #15]
	ldrh r3, [r4, #16]
	adds r0, r6, r0
	strh r3, [r5, #16]
	strh r3, [r5, #56]
	strh r3, [r5, #52]
	ldrh r3, [r4, #18]
	add r6, sp, #16
	strh r3, [r5, #18]
	strh r3, [r5, #58]
	strh r3, [r5, #54]
	movs r3, #128
	lsls r3, r3, #7
	strh r3, [r5, #20]
	strh r3, [r5, #22]
	ldrh r3, [r4, #20]
	adds r1, r6, #0
	strh r3, [r5, #24]
	ldrh r3, [r4, #22]
	str r4, [sp, #0]
	strh r3, [r5, #26]
	ldrh r3, [r4, #24]
	movs r7, #0
	strh r3, [r5, #28]
	ldrb r3, [r4, #26]
	strb r3, [r5, #30]
	ldrb r3, [r4, #27]
	strb r3, [r5, #31]
	adds r3, r5, #0
	adds r3, #32
	strb r2, [r3]
	ldrb r3, [r4, #29]
	adds r2, r5, #0
	adds r2, #33
	strb r3, [r2]
	movs r2, #15
	bl Ui_AdjustValueWithoutLimitFar + 0x8
	ldrh r3, [r6, r7]
	ldr r4, [sp, #0]
	cmp r3, #0
	beq .L_080afcba
	adds r0, r6, #0
	adds r1, r5, #0
	movs r2, #0
.L_080afca6:
	ldrh r3, [r2, r0]
	adds r7, #1
	strb r3, [r1]
	adds r2, #2
	adds r1, #1
	cmp r7, #13
	bgt .L_080afcba
	ldrh r3, [r2, r6]
	cmp r3, #0
	bne .L_080afca6
.L_080afcba:
	mov r3, r8
	cmp r3, #8
	bgt .L_080afcc6
	adds r3, #49
	strb r3, [r5, r7]
	adds r7, #1
.L_080afcc6:
	adds r0, r4, #2
	mov r10, r0
	movs r3, #0
	mov r12, r10
	strb r3, [r5, r7]
	mov r1, r12
	movs r3, #0
	strb r3, [r5, #14]
	movs r0, #28
	str r1, [sp, #4]
	adds r4, #30
	movs r1, #36
	mov r9, r3
	movs r7, #0
	mov r11, r0
	mov r8, r1
	mov lr, r4
	mov r12, r5
.L_080afcea:
	mov r2, lr
	ldrh r3, [r2]
	movs r1, #2
	ldr r0, [sp, #4]
	add lr, r1
	cmp r3, #0
	beq .L_080afd28
	mov r2, r8
	mov r1, r10
	ldrb r3, [r2, r1]
	movs r4, #0
	cmp r4, r3
	bge .L_080afd28
	mov r1, r12
	mov r6, r11
	adds r1, #216
.L_080afd0a:
	mov r3, r9
	cmp r3, #14
	bgt .L_080afd1e
	ldrh r3, [r0, r6]
	movs r0, #2
	strh r3, [r1]
	movs r3, #1
	adds r1, #2
	add r12, r0
	add r9, r3
.L_080afd1e:
	mov r0, r10
	ldrb r3, [r0, r2]
	adds r4, #1
	cmp r4, r3
	blt .L_080afd0a
.L_080afd28:
	movs r0, #2
	movs r1, #1
	adds r7, #1
	add r11, r0
	add r8, r1
	cmp r7, #3
	ble .L_080afcea
	movs r2, #144
	lsls r2, r2, #1
	movs r0, #42
	adds r3, r5, r2
	adds r0, #255
	movs r2, #0
	str r2, [r3]
	adds r3, r5, r0
	strb r2, [r3]
	add r2, sp, #8
	ldrh r2, [r2]
	movs r1, #165
	lsls r1, r1, #1
	adds r6, r5, r1
	strh r2, [r6]
	adds r1, r5, #0
	adds r1, #36
	ldr r0, [sp, #12]
	bl Func_080b0084
	ldr r0, [sp, #12]
	bl Owner_RecalculateStats
	movs r3, #149
	lsls r3, r3, #1
	adds r1, r5, r3
	movs r3, #1
	strb r3, [r1]
	movs r0, #255
	ldrh r2, [r6]
	lsls r0, r0, #8
	adds r0, #247
	adds r3, r2, r0
	movs r0, #228
	lsls r3, r3, #16
	lsls r0, r0, #14
	cmp r3, r0
	bhi .L_080afd88
	movs r3, #2
	strb r3, [r1]
	ldrh r2, [r6]
.L_080afd88:
	ldr r0, .L_080afdb8
	adds r3, r2, r0
	movs r2, #128
	lsls r3, r3, #16
	lsls r2, r2, #12
	cmp r3, r2
	bhi .L_080afd9a
	movs r3, #2
	strb r3, [r1]
.L_080afd9a:
	movs r0, #1
.L_080afd9c:
	add sp, #48
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080afdac:
	.4byte IwramClearWords
.L_080afdb0:
	.4byte Data_080b9e7c
.L_080afdb4:
	.4byte 0x0000042c
.L_080afdb8:
	.4byte 0xfffffe8f
