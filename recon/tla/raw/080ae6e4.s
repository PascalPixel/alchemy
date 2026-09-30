.syntax unified
	.thumb
	.global Func_080ae6e4
	.thumb_func
Func_080ae6e4:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r2, #0
	sub sp, #16
	mov r8, r2
.L_080ae6f2:
	lsls r3, r2, #2
	adds r3, r3, r2
	lsls r3, r3, #2
	movs r5, #128
	adds r6, r3, #0
	lsls r5, r5, #4
	movs r7, #0
	adds r6, #48
	add r5, r8
.L_080ae704:
	adds r0, r5, #0
	str r2, [sp, #0]
	bl GameFlag_Test
	ldr r2, [sp, #0]
	cmp r0, #0
	beq .L_080ae71a
	adds r0, r6, #0
	bl GameFlag_SetBit
	ldr r2, [sp, #0]
.L_080ae71a:
	adds r7, #1
	adds r6, #1
	adds r5, #1
	cmp r7, #6
	ble .L_080ae704
	movs r1, #7
	adds r2, #1
	add r8, r1
	cmp r2, #3
	ble .L_080ae6f2
	movs r0, #222
	bl PartyInventory_Remove
	movs r0, #0
	bl Func_080afdd8
	movs r0, #1
	bl Func_080afdd8
	movs r0, #2
	bl Func_080afdd8
	movs r0, #3
	bl Func_080afdd8
	movs r0, #0
	bl Func_080ae358
	bl Func_080b1004
	movs r2, #0
.L_080ae758:
	adds r0, r2, #0
	str r2, [sp, #0]
	bl Owner_RecalculateStats
	ldr r2, [sp, #0]
	adds r2, #1
	cmp r2, #7
	ble .L_080ae758
	movs r0, #1
	bl Func_080ae16c
	movs r0, #34
	bl GameFlag_SetBit
	add r3, sp, #8
	mov r8, r3
	movs r1, #200
	ldr r3, .L_080ae828
	lsls r1, r1, #5
	adds r1, #80
	adds r1, r1, r3
	movs r2, #0
	mov r10, r1
.L_080ae786:
	lsls r3, r2, #2
	adds r3, r3, r2
	lsls r3, r3, #2
	adds r6, r3, #0
	movs r1, #0
	movs r7, #0
	adds r6, #48
	mov r5, r8
.L_080ae796:
	adds r0, r6, #0
	str r1, [sp, #4]
	str r2, [sp, #0]
	bl GameFlag_Test
	ldr r1, [sp, #4]
	ldr r2, [sp, #0]
	cmp r0, #0
	bne .L_080ae7ae
	strb r7, [r5]
	adds r1, #1
	adds r5, #1
.L_080ae7ae:
	adds r7, #1
	adds r6, #1
	cmp r7, #6
	ble .L_080ae796
	cmp r1, #0
	beq .L_080ae7d4
	str r1, [sp, #4]
	str r2, [sp, #0]
	bl Random16
	ldr r1, [sp, #4]
	ldr r2, [sp, #0]
	adds r3, r1, #0
	muls r3, r0
	mov r1, r8
	lsrs r3, r3, #16
	ldrsb r3, [r1, r3]
	adds r7, r3, #1
	b .L_080ae7d6
.L_080ae7d4:
	movs r7, #0
.L_080ae7d6:
	mov r3, r10
	movs r1, #1
	adds r2, #1
	strb r7, [r3]
	add r10, r1
	cmp r2, #3
	ble .L_080ae786
	movs r0, #202
	movs r1, #3
	bl Func_080ae6c8
	movs r0, #201
	movs r1, #3
	bl Func_080ae6c8
	movs r0, #203
	movs r1, #1
	bl Func_080ae6c8
	movs r0, #206
	movs r1, #0
	bl Func_080ae6c8
	movs r0, #207
	movs r1, #2
	bl Func_080ae6c8
	ldr r1, .L_080ae82c
	movs r3, #163
	lsls r3, r3, #2
	adds r2, r1, r3
	ldr r2, [r2]
	ldr r3, [r1, #16]
	add sp, #16
	adds r3, r3, r2
	str r3, [r1, #16]
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080ae828:
	.4byte Data_02001000
.L_080ae82c:
	.4byte gPartyState
