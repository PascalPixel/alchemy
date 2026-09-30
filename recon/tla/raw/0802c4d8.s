.syntax unified
	.thumb
	.global Func_0802c4d8
	.thumb_func
Func_0802c4d8:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	sub sp, #16
	movs r1, #0
	str r3, [sp, #12]
	str r1, [sp, #8]
	str r1, [sp, #4]
	ldr r3, [r3]
	cmp r3, #0
	beq .L_0802c504
	ldmia r3!, {r2}
	str r2, [sp, #8]
	ldr r3, [r3, #4]
	str r3, [sp, #4]
.L_0802c504:
	ldr r1, [sp, #8]
	movs r2, #255
	lsls r2, r2, #24
	adds r3, r1, r2
	ldr r1, [sp, #4]
	ldr r2, .L_0802c588
	asrs r3, r3, #25
	str r3, [sp, #8]
	adds r3, r1, r2
	asrs r3, r3, #25
	str r3, [sp, #4]
	movs r3, #0
	mov r10, r3
.L_0802c51e:
	mov r2, r10
	ldr r6, [sp, #4]
	movs r1, #0
	lsls r2, r2, #10
	mov r9, r1
	mov r11, r2
.L_0802c52a:
	adds r3, r6, #0
	movs r1, #31
	ands r3, r1
	movs r5, #0
	mov r8, r6
	lsls r7, r3, #5
.L_0802c536:
	ldr r2, [sp, #8]
	mov r0, r10
	adds r1, r2, r5
	adds r3, r1, #0
	movs r2, #31
	ands r3, r2
	adds r4, r7, r3
	movs r2, #164
	lsls r3, r4, #1
	lsls r2, r2, #1
	adds r3, r3, r2
	ldr r2, [sp, #12]
	adds r5, #1
	ldrh r4, [r2, r3]
	movs r3, #1
	add r4, r11
	str r3, [sp, #0]
	mov r2, r8
	adds r3, r4, #0
	bl Map_WriteLayerCellTile
	cmp r5, #1
	bls .L_0802c536
	movs r3, #1
	add r9, r3
	mov r1, r9
	adds r6, #1
	cmp r1, #1
	bls .L_0802c52a
	add r10, r3
	mov r2, r10
	cmp r2, #1
	bls .L_0802c51e
	add sp, #16
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0802c588:
	.4byte 0xfec00000
