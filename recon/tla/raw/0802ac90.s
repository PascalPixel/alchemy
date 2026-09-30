.syntax unified
	.thumb
	.global Func_0802ac90
	.thumb_func
Func_0802ac90:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r3, .L_0802ad74
	adds r7, r1, #0
	mov r8, r3
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #32]
	lsls r3, r0, #3
	subs r3, r3, r0
	lsls r3, r3, #3
	adds r1, r1, r3
	ldr r3, .L_0802ad78
	lsls r0, r0, #11
	adds r3, r3, r0
	movs r6, #132
	lsls r6, r6, #1
	mov r9, r3
	lsrs r3, r2, #31
	adds r1, r1, r6
	adds r3, r2, r3
	movs r0, #127
	ldrh r6, [r1, #42]
	asrs r3, r3, #1
	ands r3, r0
	mov r10, r0
	lsls r4, r3, #7
	movs r0, #254
	movs r3, #30
	mov lr, r6
	ands r2, r3
	lsls r0, r0, #6
	ldrh r6, [r1, #46]
	mov r12, r3
	lsls r5, r2, #5
	cmp lr, r0
	beq .L_0802acf2
	subs r4, r4, r6
	lsls r3, r6, #2
	ldr r6, .L_0802ad74
	mov r2, lr
	adds r6, r6, r3
	ands r4, r2
	mov r8, r6
.L_0802acf2:
	lsrs r3, r7, #31
	adds r3, r7, r3
	ldrh r2, [r1, #40]
	asrs r0, r3, #1
	mov r11, r7
	mov r3, r10
	ands r0, r3
	mov r6, r11
	mov r3, r12
	ands r6, r3
	ldrh r1, [r1, #44]
	mov r11, r6
	ands r0, r2
	cmp r2, #127
	beq .L_0802ad18
	subs r0, r0, r1
	lsls r3, r1, #2
	ands r0, r2
	add r8, r3
.L_0802ad18:
	movs r6, #1
	mov r12, r6
	mov r2, r12
	movs r3, #240
	ands r2, r7
	lsls r3, r3, #2
	mov r12, r2
	movs r7, #0
	mov r10, r3
.L_0802ad2a:
	adds r3, r4, r0
	lsls r3, r3, #2
	mov r6, r8
	ldr r1, [r3, r6]
	ldr r3, .L_0802ad7c
	lsls r1, r1, #21
	lsrs r1, r1, #19
	add r1, r12
	lsls r1, r1, #1
	mov r6, r11
	adds r2, r1, r3
	adds r3, r5, r6
	ldrh r2, [r2]
	add r3, r12
	lsls r3, r3, #1
	add r3, r9
	ldr r6, .L_0802ad80
	strh r2, [r3]
	adds r2, r1, r6
	ldrh r2, [r2]
	adds r3, #64
	strh r2, [r3]
	adds r4, #128
	mov r2, lr
	adds r5, #64
	mov r3, r10
	adds r7, #1
	ands r4, r2
	ands r5, r3
	cmp r7, #10
	bls .L_0802ad2a
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0802ad74:
	.4byte gMapCellBuffer
.L_0802ad78:
	.4byte 0x06002800
.L_0802ad7c:
	.4byte gMapBlocks
.L_0802ad80:
	.4byte Data_02020004
