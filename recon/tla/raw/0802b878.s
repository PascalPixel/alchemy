.syntax unified
	.thumb
	.global Map_WriteLayerCellTile
	.thumb_func
Map_WriteLayerCellTile:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	adds r5, r3, #0
	movs r3, #192
	lsls r3, r3, #18
	mov r8, r2
	ldr r2, [r3, #32]
	mov r10, r1
	movs r1, #138
	lsls r1, r1, #1
	adds r3, r2, r1
	ldr r3, [r3]
	mov r1, r10
	mov r11, r3
	movs r3, #1
	ands r1, r3
	mov r10, r1
	lsls r0, r0, #1
	mov r1, r8
	ands r1, r3
	mov r9, r0
	mov r8, r1
	mov r3, r9
	add r3, r8
	lsls r3, r3, #1
	add r3, r10
	lsls r3, r3, #1
	adds r2, r2, r3
	ldr r1, [sp, #32]
	movs r3, #144
	lsls r3, r3, #4
	adds r3, #72
	adds r2, r2, r3
	cmp r1, #0
	bne .L_0802b8d0
	ldrh r3, [r2]
	movs r0, #0
	cmp r5, r3
	beq .L_0802b978
.L_0802b8d0:
	movs r6, #128
	lsls r6, r6, #3
	strh r5, [r2]
	movs r0, #56
	adds r1, r6, #0
	bl Runtime_AllocateHeapBlock
	lsls r3, r5, #2
	mov r2, r11
	adds r7, r0, #0
	ldr r0, [r3, r2]
	cmp r0, #0
	beq .L_0802b8f4
	add r0, r11
	adds r1, r7, #0
	bl Func_0801591c
	b .L_0802b8fe
.L_0802b8f4:
	ldr r3, .L_0802b984
	adds r0, r7, #0
	adds r1, r6, #0
	mov lr, r3
	.2byte 0xf800
.L_0802b8fe:
	mov r3, r9
	add r3, r8
	lsls r3, r3, #5
	ldr r1, .L_0802b988
	add r3, r10
	lsls r3, r3, #6
	adds r4, r7, #0
	adds r5, r3, r1
	movs r6, #0
.L_0802b910:
	movs r3, #128
	movs r2, #132
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	adds r0, r4, #0
	adds r1, r5, #0
	adds r2, #16
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	adds r6, #1
	adds r4, #64
	adds r5, #128
	cmp r6, #15
	bls .L_0802b910
	ldr r2, [sp, #32]
	cmp r2, #0
	beq .L_0802b970
	mov r3, r9
	add r3, r8
	lsls r3, r3, #6
	ldr r2, .L_0802b98c
	add r3, r10
	ldr r5, .L_0802b990
	lsls r3, r3, #5
	adds r1, r3, r2
	adds r4, r7, #0
	movs r6, #0
.L_0802b948:
	movs r0, #0
.L_0802b94a:
	ldrh r3, [r4]
	adds r0, #1
	lsls r3, r3, #2
	ldrh r2, [r3, r5]
	adds r4, #4
	strh r2, [r1]
	ldr r2, .L_0802b994
	adds r3, r3, r2
	ldrh r3, [r3]
	adds r2, r1, #0
	adds r2, #64
	strh r3, [r2]
	adds r1, #2
	cmp r0, #15
	bls .L_0802b94a
	adds r6, #1
	adds r1, #96
	cmp r6, #15
	bls .L_0802b948
.L_0802b970:
	movs r0, #56
	bl Runtime_ReleaseHeapBlock
	movs r0, #1
.L_0802b978:
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0802b984:
	.4byte IwramClearWords
.L_0802b988:
	.4byte gMapBlocks
.L_0802b98c:
	.4byte 0x06004000
.L_0802b990:
	.4byte gMapCellBuffer
.L_0802b994:
	.4byte Data_02010002
