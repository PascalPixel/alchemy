.syntax unified
	.thumb
	.global Func_08123648
	.thumb_func
Func_08123648:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #36
	mov r1, r9
	add r3, sp, #32
	str r1, [r3]
	mov r11, r1
	bl BattleAction_Get
	str r0, [sp, #28]
	movs r2, #0
	ldrb r5, [r0]
	str r2, [sp, #24]
	ldrb r0, [r0, #3]
	bl BattleFx_IsReviveFar
	cmp r0, #0
	beq .L_0812367a
	movs r3, #1
	str r3, [sp, #24]
.L_0812367a:
	cmp r5, #0
	beq .L_08123696
	cmp r5, #4
	beq .L_081236ae
	mov r4, r11
	subs r4, #12
	str r4, [sp, #8]
	movs r2, #0
	ldr r3, [r4]
	movs r7, #0
	movs r1, #12
	ldrsh r0, [r3, r1]
	str r2, [sp, #12]
	b .L_081237fe
.L_08123696:
	mov r3, r11
	subs r3, #4
	ldr r3, [r3]
	movs r1, #1
	strb r1, [r3, #1]
	strb r5, [r3, #17]
	mov r2, r11
	subs r2, #8
	ldr r2, [r2]
	strb r1, [r3, #31]
	strb r2, [r3, #3]
	b .L_08123806
.L_081236ae:
	mov r3, r11
	subs r3, #4
	ldr r2, [r3]
	movs r1, #1
	movs r3, #0
	strb r3, [r2, #17]
	strb r1, [r2, #1]
	mov r3, r11
	subs r3, #8
	ldr r3, [r3]
	strb r1, [r2, #31]
	strb r3, [r2, #3]
	b .L_08123806
.L_081236c8:
	movs r3, #16
	negs r3, r3
	add r3, r11
	ldr r2, [r3]
	mov r10, r3
	movs r3, #88
	ldrsh r3, [r2, r3]
	movs r6, #0
	cmp r3, #255
	beq .L_081236ea
	adds r2, #88
.L_081236de:
	adds r2, #2
	movs r1, #0
	ldrsh r3, [r2, r1]
	adds r6, #1
	cmp r3, #255
	bne .L_081236de
.L_081236ea:
	str r6, [sp, #20]
	mov r2, r10
	ldr r1, [r2]
	movs r3, #100
	adds r2, r1, #2
	ldrsh r3, [r2, r3]
	movs r6, #0
	cmp r3, #255
	beq .L_0812370a
	adds r2, #100
.L_081236fe:
	adds r2, #2
	movs r1, #0
	ldrsh r3, [r2, r1]
	adds r6, #1
	cmp r3, #255
	bne .L_081236fe
.L_0812370a:
	str r6, [sp, #16]
	ldr r2, [sp, #8]
	movs r4, #15
	ldr r3, [r2]
	mov r8, r4
	ldrh r3, [r3, #10]
	mov r1, r8
	ands r1, r3
	subs r2, r1, r0
	adds r3, r1, r0
	subs r3, #1
	adds r6, r2, #1
	mov r8, r1
	str r3, [sp, #4]
	cmp r6, r3
	bgt .L_081237ac
	movs r2, #4
	negs r2, r2
	lsls r3, r6, #1
	add r2, r11
	adds r4, r3, #0
	mov r9, r2
	adds r4, #100
.L_08123738:
	cmp r6, #0
	blt .L_081237a0
	ldr r0, [sp, #8]
	ldr r3, [r0]
	ldrh r2, [r3, #10]
	movs r3, #128
	ands r3, r2
	cmp r3, #0
	beq .L_0812375a
	ldr r1, [sp, #16]
	cmp r6, r1
	bge .L_081237a0
	mov r2, r10
	ldr r3, [r2]
	adds r3, #2
	ldrsh r5, [r3, r4]
	b .L_0812376a
.L_0812375a:
	ldr r1, [sp, #20]
	cmp r6, r1
	bge .L_081237a0
	mov r3, r10
	ldr r2, [r3]
	lsls r3, r6, #1
	adds r3, #88
	ldrsh r5, [r2, r3]
.L_0812376a:
	cmp r5, #254
	beq .L_081237a0
	ldr r1, [sp, #24]
	cmp r1, #0
	bne .L_08123786
	adds r0, r5, #0
	str r4, [sp, #0]
	bl Owner_GetState
	movs r2, #56
	ldrsh r3, [r0, r2]
	ldr r4, [sp, #0]
	cmp r3, #0
	beq .L_081237a0
.L_08123786:
	mov r3, r9
	ldr r2, [r3]
	adds r0, r7, #0
	adds r1, r2, #3
	adds r0, #28
	movs r3, #1
	strb r3, [r1, r0]
	mov r0, r8
	adds r2, r2, r7
	subs r3, r6, r0
	strb r3, [r2, #17]
	strb r5, [r1, r7]
	adds r7, #1
.L_081237a0:
	ldr r1, [sp, #4]
	adds r6, #1
	adds r4, #2
	cmp r6, r1
	ble .L_08123738
	b .L_081237b4
.L_081237ac:
	movs r2, #4
	negs r2, r2
	add r2, r11
	mov r9, r2
.L_081237b4:
	mov r4, r9
	ldr r3, [r4]
	strb r7, [r3, #1]
	cmp r7, #0
	bgt .L_081237ee
	ldr r0, [sp, #8]
	ldr r3, [r0]
	movs r1, #0
	ldrsh r0, [r3, r1]
	movs r1, #1
	bl UiText_DrawQuantity
	ldr r0, .L_08123818
	bl Func_080381c0 + 0x8
	mov r3, r11
	subs r3, #20
	ldr r3, [r3]
	movs r4, #44
	adds r4, #255
	adds r2, r3, r4
	ldrb r3, [r2]
	cmp r3, #0
	bne .L_081237e8
	movs r3, #1
	strb r3, [r2]
.L_081237e8:
	movs r0, #1
	negs r0, r0
	b .L_08123808
.L_081237ee:
	ldr r0, [sp, #28]
	ldrb r3, [r0, #3]
	cmp r3, #75
	bne .L_08123806
	ldr r1, [sp, #12]
	movs r0, #11
	adds r1, #1
	str r1, [sp, #12]
.L_081237fe:
	ldr r2, [sp, #12]
	cmp r2, #1
	bgt .L_08123806
	b .L_081236c8
.L_08123806:
	movs r0, #0
.L_08123808:
	add sp, #36
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_08123818:
	.4byte 0x00000c62
