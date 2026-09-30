.syntax unified
	.thumb
	.global Func_08046284
	.thumb_func
Func_08046284:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #16
	mov r8, r1
	movs r1, #0
	str r2, [sp, #12]
	str r3, [sp, #8]
	str r1, [sp, #4]
	str r1, [sp, #0]
	mov r2, r8
	ldrh r3, [r2]
	mov r10, r0
	mov r9, r1
	cmp r3, #0
	beq .L_0804632e
	movs r3, #252
	ldr r6, [sp, #12]
	lsls r3, r3, #6
	adds r3, #255
	mov r11, r3
	mov r5, r8
	subs r6, #2
.L_080462ba:
	ldrh r0, [r5]
	bl BattleAction_Get
	ldrb r2, [r0, #1]
	movs r3, #128
	ands r3, r2
	cmp r3, #0
	beq .L_0804631e
	ldrh r2, [r5]
	mov r3, r11
	ands r3, r2
	strh r3, [r6, #2]
	movs r1, #1
	add r9, r1
	mov r1, r10
	ldrh r2, [r1]
	ldrh r3, [r5]
	adds r6, #2
	eors r3, r2
	mov r2, r11
	ands r3, r2
	movs r0, #0
	cmp r3, #0
	beq .L_08046302
	ldr r7, .L_08046314
	adds r4, r5, #0
.L_080462ee:
	adds r0, #1
	cmp r0, #31
	bgt .L_08046302
	adds r1, #4
	ldrh r3, [r4]
	ldrh r2, [r1]
	eors r3, r2
	ands r3, r7
	cmp r3, #0
	bne .L_080462ee
.L_08046302:
	cmp r0, #32
	bne .L_0804631e
	ldr r3, [sp, #4]
	ldr r2, .L_08046318
	adds r3, #1
	str r3, [sp, #4]
	ldrh r3, [r6]
	orrs r3, r2
	b .L_0804631c
.L_08046314:
	.4byte 0x00003fff
.L_08046318:
	.4byte 0x00008000
.L_0804631c:
	strh r3, [r6]
.L_0804631e:
	mov r3, r8
	adds r5, #4
	adds r3, #124
	cmp r5, r3
	bgt .L_0804632e
	ldrh r3, [r5]
	cmp r3, #0
	bne .L_080462ba
.L_0804632e:
	mov r1, r10
	ldrh r3, [r1]
	cmp r3, #0
	beq .L_080463b8
	mov r2, r9
	lsls r3, r2, #1
	ldr r1, [sp, #12]
	movs r2, #252
	lsls r2, r2, #6
	adds r2, #255
	mov r5, r10
	adds r7, r3, r1
	mov r11, r2
.L_08046348:
	ldrh r0, [r5]
	bl BattleAction_Get
	ldrb r2, [r0, #1]
	movs r3, #128
	ands r3, r2
	cmp r3, #0
	beq .L_080463a8
	mov r1, r8
	ldrh r2, [r1]
	ldrh r3, [r5]
	movs r0, #0
	eors r3, r2
	mov r2, r11
	ands r3, r2
	cmp r3, #0
	beq .L_08046382
	ldr r6, .L_08046394
	adds r4, r5, #0
.L_0804636e:
	adds r0, #1
	cmp r0, #31
	bgt .L_08046382
	adds r1, #4
	ldrh r3, [r4]
	ldrh r2, [r1]
	eors r3, r2
	ands r3, r6
	cmp r3, #0
	bne .L_0804636e
.L_08046382:
	cmp r0, #32
	bne .L_080463a8
	ldr r3, [sp, #0]
	ldr r2, .L_08046398
	adds r3, #1
	str r3, [sp, #0]
	mov r1, r11
	ldrh r3, [r5]
	b .L_0804639c
.L_08046394:
	.4byte 0x00003fff
.L_08046398:
	.4byte 0x00004000
.L_0804639c:
	ands r3, r1
	orrs r3, r2
	movs r2, #1
	strh r3, [r7]
	add r9, r2
	adds r7, #2
.L_080463a8:
	mov r3, r10
	adds r5, #4
	adds r3, #124
	cmp r5, r3
	bgt .L_080463b8
	ldrh r3, [r5]
	cmp r3, #0
	bne .L_08046348
.L_080463b8:
	ldr r3, [sp, #4]
	ldr r1, [sp, #8]
	mov r0, r9
	str r3, [r1]
	ldr r3, [sp, #48]
	ldr r2, [sp, #0]
	add sp, #16
	str r2, [r3]
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
