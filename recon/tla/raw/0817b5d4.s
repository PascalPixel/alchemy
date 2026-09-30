.syntax unified
	.thumb
	.global Func_0817b5d4
	.thumb_func
Func_0817b5d4:
	push {r5, r6, r7, lr}
	adds r5, r0, #0
	bl GetBattleObjectSlotFar
	adds r7, r0, #0
	adds r0, r5, #0
	ldr r6, [r7]
	bl Owner_GetState
	movs r2, #165
	lsls r2, r2, #1
	adds r3, r0, r2
	ldrh r0, [r3]
	movs r3, #102
	adds r3, #255
	cmp r0, r3
	beq .L_0817b64c
	adds r2, #39
	cmp r0, r2
	beq .L_0817b64c
	adds r3, #16
	cmp r0, r3
	beq .L_0817b64c
	adds r2, #7
	cmp r0, r2
	beq .L_0817b64c
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r6, #52]
	movs r3, #128
	lsls r3, r3, #12
	str r3, [r6, #48]
	ldr r3, [r6, #12]
	movs r2, #160
	lsls r2, r2, #12
	cmp r3, r2
	bge .L_0817b628
	ldr r3, [r6, #40]
	movs r2, #160
	lsls r2, r2, #10
	adds r3, r3, r2
	str r3, [r6, #40]
.L_0817b628:
	adds r0, r6, #0
	bl Object_ResetMotion
	ldr r3, [r7, #12]
	cmp r3, #0
	ble .L_0817b63c
	ldr r3, [r6, #8]
	movs r2, #240
	lsls r2, r2, #12
	b .L_0817b640
.L_0817b63c:
	ldr r3, [r6, #8]
	ldr r2, .L_0817b668
.L_0817b640:
	adds r1, r3, r2
	ldr r3, [r7, #16]
	adds r0, r6, #0
	movs r2, #0
	bl Object_SetPosition
.L_0817b64c:
	movs r3, #153
	lsls r3, r3, #8
	adds r3, #153
	adds r2, r6, #0
	str r3, [r6, #72]
	adds r2, #90
	movs r3, #0
	str r3, [r6, #68]
	adds r0, r6, #0
	strb r3, [r2]
	movs r1, #5
	bl Object_SetMode
	pop {r5, r6, r7, pc}
.L_0817b668:
	.4byte 0xfff10000
