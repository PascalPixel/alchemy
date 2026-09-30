.syntax unified
	.thumb
	.global Func_0812764c
	.thumb_func
Func_0812764c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r2, #0
	mov r11, r0
	movs r0, #128
	sub sp, #16
	mov r9, r2
	bl Runtime_BumpAllocateAlternatePool
	mov r5, sp
	movs r3, #0
	mov r10, r0
	adds r0, r5, #0
	mov r8, r3
	bl Func_0811a038
	adds r7, r0, #0
	cmp r7, #0
	ble .L_08127692
	adds r6, r5, #0
	adds r5, r7, #0
.L_08127680:
	ldrh r0, [r6]
	bl Owner_GetState
	ldrb r3, [r0, #15]
	subs r5, #1
	adds r6, #2
	add r8, r3
	cmp r5, #0
	bne .L_08127680
.L_08127692:
	adds r1, r7, #0
	mov r0, r8
	bl __divsi3
	mov r8, r0
	movs r0, #254
	lsls r0, r0, #2
	bl GameFlag_GetByte
	lsls r0, r0, #24
	asrs r0, r0, #24
	add r8, r0
	mov r7, r8
	cmp r7, #0
	bgt .L_081276b4
	movs r2, #1
	mov r8, r2
.L_081276b4:
	mov r3, r8
	cmp r3, #99
	ble .L_081276be
	movs r7, #99
	mov r8, r7
.L_081276be:
	ldr r1, .L_081276ec
	mov r2, r10
	movs r5, #31
.L_081276c4:
	ldrh r3, [r2, #2]
	subs r5, #1
	orrs r3, r1
	strh r3, [r2, #2]
	adds r2, #4
	cmp r5, #0
	bge .L_081276c4
	movs r0, #192
	lsls r0, r0, #3
	adds r0, #91
	bl GameFlag_Test
	cmp r0, #0
	beq .L_081276f0
	movs r0, #172
	lsls r0, r0, #3
	adds r0, #255
	bl GameFlag_SetBit
	b .L_081276f0
.L_081276ec:
	.4byte 0x0000ffff
.L_081276f0:
	movs r0, #192
	lsls r0, r0, #3
	adds r0, #92
	bl GameFlag_Test
	cmp r0, #0
	beq .L_08127708
	movs r0, #172
	lsls r0, r0, #3
	adds r0, #255
	bl GameFlag_SetBit
.L_08127708:
	movs r0, #192
	lsls r0, r0, #3
	adds r0, #93
	bl GameFlag_Test
	cmp r0, #0
	beq .L_08127720
	movs r0, #172
	lsls r0, r0, #3
	adds r0, #255
	bl GameFlag_SetBit
.L_08127720:
	movs r0, #192
	lsls r0, r0, #3
	adds r0, #94
	bl GameFlag_Test
	cmp r0, #0
	beq .L_08127738
	movs r0, #172
	lsls r0, r0, #3
	adds r0, #255
	bl GameFlag_SetBit
.L_08127738:
	movs r0, #192
	lsls r0, r0, #3
	adds r0, #107
	bl GameFlag_Test
	cmp r0, #0
	beq .L_08127750
	movs r0, #192
	lsls r0, r0, #3
	adds r0, #108
	bl GameFlag_SetBit
.L_08127750:
	movs r0, #205
	lsls r0, r0, #3
	bl GameFlag_Test
	cmp r0, #0
	beq .L_08127766
	movs r0, #192
	lsls r0, r0, #3
	adds r0, #106
	bl GameFlag_SetBit
.L_08127766:
	movs r0, #192
	lsls r0, r0, #3
	adds r0, #105
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0812777e
	movs r0, #192
	lsls r0, r0, #3
	adds r0, #106
	bl GameFlag_SetBit
.L_0812777e:
	movs r5, #224
	lsls r5, r5, #3
	adds r5, #113
	adds r0, r5, #0
	bl GameFlag_ClearBit
	movs r0, #239
	lsls r0, r0, #3
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0812779c
	adds r0, r5, #0
	bl GameFlag_SetBit
.L_0812779c:
	ldr r6, .L_08127868
	movs r5, #0
.L_081277a0:
	ldrh r0, [r6]
	movs r2, #192
	lsls r2, r2, #3
	adds r0, r0, r2
	adds r5, #1
	adds r6, #2
	bl GameFlag_ClearBit
	cmp r5, #86
	bls .L_081277a0
	movs r0, #71
	bl GameFlag_SetBit
	movs r0, #230
	bl GameFlag_SetBit
	movs r0, #117
	bl GameFlag_SetBit
	movs r5, #0
.L_081277c8:
	adds r0, r5, #0
	bl Func_08127588
	cmp r0, #0
	blt .L_0812780c
	mov r3, r8
	adds r3, #3
	cmp r0, r3
	bgt .L_0812780c
	movs r3, #186
	movs r6, #1
	lsls r3, r3, #2
	negs r6, r6
	adds r3, #255
	movs r4, #0
	mov r1, r10
.L_081277e8:
	movs r7, #2
	ldrsh r2, [r1, r7]
	cmp r2, r3
	bge .L_081277f4
	adds r3, r2, #0
	adds r6, r4, #0
.L_081277f4:
	adds r4, #1
	adds r1, #4
	cmp r4, #31
	ble .L_081277e8
	cmp r6, #0
	blt .L_0812780c
	lsls r3, r6, #2
	add r3, r10
	strh r0, [r3, #2]
	strh r5, [r3]
	movs r2, #1
	add r9, r2
.L_0812780c:
	movs r3, #202
	lsls r3, r3, #1
	adds r5, #1
	adds r3, #255
	cmp r5, r3
	bls .L_081277c8
	mov r7, r9
	cmp r7, #32
	ble .L_08127822
	movs r2, #32
	mov r9, r2
.L_08127822:
	mov r3, r9
	cmp r3, #0
	beq .L_08127848
	bl Random16
	mov r3, r9
	muls r3, r0
	lsrs r3, r3, #16
	lsls r3, r3, #2
	add r3, r10
	movs r7, #0
	ldrsh r5, [r3, r7]
	movs r2, #2
	ldrsh r3, [r3, r2]
	mov r7, r8
	subs r3, r7, r3
	mov r2, r11
	str r3, [r2]
	b .L_08127850
.L_08127848:
	mov r3, r9
	mov r7, r11
	str r3, [r7]
	movs r5, #1
.L_08127850:
	mov r0, r10
	bl Sys_Free
	adds r0, r5, #0
	add sp, #16
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_08127868:
	.4byte Data_08130c5c
