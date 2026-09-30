.syntax unified
	.thumb
	.global Func_080ae5fc
	.thumb_func
Func_080ae5fc:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r1, #144
	movs r7, #128
	lsls r1, r1, #2
	movs r3, #164
	lsls r7, r7, #3
	adds r1, r1, r0
	lsls r3, r3, #3
	adds r7, #101
	ldr r2, .L_080ae6c4
	adds r6, r0, r3
	mov r8, r1
	adds r3, r0, r7
	movs r1, #147
	ldrb r3, [r3]
	lsls r1, r1, #1
	adds r1, #255
	adds r5, r2, r1
	strb r3, [r5]
	adds r7, #1
	adds r3, r0, r7
	ldrb r3, [r3]
	adds r1, #1
	adds r4, r2, r1
	strb r3, [r4]
	adds r7, #6
	adds r3, r0, r7
	ldrb r1, [r3]
	movs r7, #139
	lsls r7, r7, #2
	adds r3, r2, r7
	strb r1, [r3]
	movs r1, #128
	lsls r1, r1, #3
	adds r1, #106
	adds r3, r0, r1
	ldrb r1, [r3]
	subs r7, #2
	adds r3, r2, r7
	strb r1, [r3]
	movs r1, #128
	lsls r1, r1, #3
	adds r1, #138
	adds r0, r0, r1
	ldrb r3, [r0]
	adds r7, #32
	adds r2, r2, r7
	strb r3, [r2]
	sub sp, #16
	ldrb r0, [r5]
	ldrb r1, [r4]
	bl Func_08038348
	movs r7, #8
	movs r5, #0
.L_080ae66e:
	adds r0, r5, #0
	bl Owner_GetState
	movs r3, #166
	lsls r3, r3, #1
	adds r1, r5, #0
	muls r1, r3
	adds r1, r6, r1
	ldrh r2, [r1, #16]
	strh r2, [r0, #16]
	ldrh r3, [r1, #18]
	strh r2, [r0, #20]
	strh r3, [r0, #18]
	strh r3, [r0, #22]
	movs r2, #146
	ldrh r3, [r1, #24]
	lsls r2, r2, #1
	strh r3, [r0, #24]
	ldrh r3, [r1, #26]
	strh r3, [r0, #26]
	ldrh r3, [r1, #28]
	strh r3, [r0, #28]
	ldrb r3, [r1, #30]
	strb r3, [r0, #30]
	ldrb r3, [r1, #15]
	strb r3, [r0, #15]
	ldr r3, [r1, r2]
	str r3, [r0, r2]
	adds r0, r5, #0
	adds r5, #1
	bl Owner_RecalculateStats
	cmp r5, r7
	blt .L_080ae66e
	mov r1, r8
	ldr r2, .L_080ae6c4
	ldr r3, [r1, #16]
	add sp, #16
	str r3, [r2, #16]
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080ae6c4:
	.4byte gPartyState
