.syntax unified
	.thumb
	.global Func_0810a108
	.thumb_func
Func_0810a108:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #20
	str r1, [sp, #16]
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r3, [r3]
	adds r7, r2, #0
	str r3, [sp, #12]
	mov r8, r0
	bl Owner_GetState
	ldr r2, [sp, #16]
	adds r6, r0, #0
	lsls r2, r2, #1
	str r2, [sp, #8]
	adds r5, r2, #0
	adds r5, #216
	ldrh r3, [r6, r5]
	movs r2, #128
	lsls r2, r2, #1
	adds r2, #255
	ands r2, r3
	mov r9, r2
	mov r0, r9
	bl Item_Get
	ldrb r2, [r0, #3]
	movs r3, #4
	ands r3, r2
	lsls r3, r3, #24
	lsrs r3, r3, #24
	movs r2, #1
	str r3, [sp, #4]
	negs r2, r2
	movs r3, #0
	mov r11, r0
	str r3, [sp, #0]
	cmp r7, r2
	bne .L_0810a16a
	movs r3, #1
	str r3, [sp, #0]
	movs r7, #1
.L_0810a16a:
	ldrh r0, [r6, r5]
	bl Shop_GetSellPrice
	adds r2, r7, #0
	muls r2, r0
	mov r10, r2
	cmp r2, #0
	bne .L_0810a18a
	mov r0, r9
	movs r1, #2
	bl UiText_DrawQuantity
	ldr r0, .L_0810a280
	bl Func_0810857c
	b .L_0810a270
.L_0810a18a:
	ldrh r2, [r6, r5]
	movs r3, #128
	lsls r3, r3, #2
	ands r3, r2
	cmp r3, #0
	beq .L_0810a1b2
	mov r3, r11
	ldrb r2, [r3, #3]
	movs r3, #2
	ands r3, r2
	cmp r3, #0
	beq .L_0810a1b2
	mov r0, r9
	movs r1, #2
	bl UiText_DrawQuantity
	ldr r0, .L_0810a284
	bl Func_0810857c
	b .L_0810a270
.L_0810a1b2:
	ldr r2, [sp, #0]
	cmp r2, #0
	beq .L_0810a1bc
	ldr r5, .L_0810a288
	b .L_0810a1e4
.L_0810a1bc:
	ldr r3, [sp, #8]
	adds r3, #216
	ldrh r2, [r6, r3]
	movs r3, #128
	lsls r3, r3, #3
	ands r3, r2
	cmp r3, #0
	beq .L_0810a1d0
	ldr r5, .L_0810a28c
	b .L_0810a1e4
.L_0810a1d0:
	cmp r7, #1
	ble .L_0810a1d8
	ldr r5, .L_0810a290
	b .L_0810a1e4
.L_0810a1d8:
	ldr r3, [sp, #4]
	cmp r3, #0
	beq .L_0810a1e2
	ldr r5, .L_0810a294
	b .L_0810a1e4
.L_0810a1e2:
	ldr r5, .L_0810a298
.L_0810a1e4:
	mov r0, r9
	movs r1, #2
	bl UiText_DrawQuantity
	mov r0, r10
	movs r1, #5
	bl UiText_DrawQuantity
	adds r0, r5, #0
	bl Func_0810857c
	movs r0, #0
	bl Func_08108630
	cmp r0, #0
	beq .L_0810a21e
	ldr r2, [sp, #4]
	cmp r2, #0
	bne .L_0810a210
	ldr r3, [sp, #0]
	cmp r3, #0
	beq .L_0810a214
.L_0810a210:
	ldr r5, .L_0810a29c
	b .L_0810a216
.L_0810a214:
	ldr r5, .L_0810a2a0
.L_0810a216:
	adds r0, r5, #0
	bl Func_0810857c
	b .L_0810a270
.L_0810a21e:
	movs r0, #102
	bl Audio_PlayCue
	cmp r7, #0
	ble .L_0810a238
	adds r5, r7, #0
.L_0810a22a:
	mov r0, r8
	ldr r1, [sp, #16]
	subs r5, #1
	bl Inventory_DiscardFar
	cmp r5, #0
	bne .L_0810a22a
.L_0810a238:
	mov r0, r8
	bl Owner_RefreshClassActionsFar
	mov r0, r8
	bl Owner_RecalculateStatsFar
	mov r0, r10
	bl Party_AdjustSixDigitCounterAFar
	bl Func_08109188
	ldr r2, [sp, #12]
	mov r1, r8
	ldr r0, [r2, #36]
	bl Func_0810a004
	ldr r3, [sp, #4]
	cmp r3, #0
	bne .L_0810a264
	ldr r2, [sp, #0]
	cmp r2, #0
	beq .L_0810a268
.L_0810a264:
	ldr r5, .L_0810a2a4
	b .L_0810a26a
.L_0810a268:
	ldr r5, .L_0810a2a8
.L_0810a26a:
	adds r0, r5, #0
	bl Func_0810857c
.L_0810a270:
	add sp, #20
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0810a280:
	.4byte 0x0000125d
.L_0810a284:
	.4byte 0x0000125c
.L_0810a288:
	.4byte 0x00001263
.L_0810a28c:
	.4byte 0x00001262
.L_0810a290:
	.4byte 0x00001261
.L_0810a294:
	.4byte 0x00001260
.L_0810a298:
	.4byte 0x0000125f
.L_0810a29c:
	.4byte 0x00001267
.L_0810a2a0:
	.4byte 0x00001265
.L_0810a2a4:
	.4byte 0x00001266
.L_0810a2a8:
	.4byte 0x00001264
