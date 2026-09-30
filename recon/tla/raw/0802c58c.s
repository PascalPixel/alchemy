.syntax unified
	.thumb
	.global Func_0802c58c
	.thumb_func
Func_0802c58c:
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
	sub sp, #24
	movs r0, #0
	str r3, [sp, #20]
	str r0, [sp, #16]
	str r0, [sp, #12]
	ldr r3, [r3]
	cmp r3, #0
	beq .L_0802c5b8
	ldmia r3!, {r1}
	str r1, [sp, #16]
	ldr r3, [r3, #4]
	str r3, [sp, #12]
.L_0802c5b8:
	ldr r2, [sp, #16]
	movs r0, #255
	lsls r0, r0, #24
	adds r3, r2, r0
	ldr r1, [sp, #12]
	ldr r2, .L_0802c81c
	asrs r3, r3, #25
	str r3, [sp, #8]
	adds r3, r1, r2
	asrs r3, r3, #25
	str r3, [sp, #4]
	movs r3, #0
	mov r10, r3
.L_0802c5d2:
	mov r1, r10
	ldr r5, [sp, #4]
	movs r0, #0
	lsls r1, r1, #10
	mov r9, r0
	mov r11, r1
.L_0802c5de:
	adds r3, r5, #0
	movs r2, #31
	ands r3, r2
	movs r6, #0
	mov r8, r5
	lsls r7, r3, #5
.L_0802c5ea:
	ldr r3, [sp, #8]
	movs r0, #31
	adds r1, r3, r6
	adds r3, r1, #0
	ands r3, r0
	adds r4, r7, r3
	ldr r0, [sp, #20]
	movs r2, #164
	lsls r3, r4, #1
	lsls r2, r2, #1
	adds r3, r3, r2
	ldrh r4, [r0, r3]
	movs r3, #0
	add r4, r11
	str r3, [sp, #0]
	mov r0, r10
	mov r2, r8
	adds r3, r4, #0
	bl Func_0802b878
	cmp r0, #0
	beq .L_0802c618
	b .L_0802c80c
.L_0802c618:
	adds r6, #1
	cmp r6, #1
	ble .L_0802c5ea
	movs r1, #1
	add r9, r1
	mov r2, r9
	adds r5, #1
	cmp r2, #1
	ble .L_0802c5de
	add r10, r1
	mov r3, r10
	cmp r3, #1
	ble .L_0802c5d2
	ldr r3, [sp, #16]
	cmp r3, #0
	bge .L_0802c63c
	ldr r0, .L_0802c820
	adds r3, r3, r0
.L_0802c63c:
	asrs r2, r3, #21
	ldr r3, [sp, #12]
	movs r1, #31
	ands r2, r1
	cmp r3, #0
	bge .L_0802c64c
	ldr r0, .L_0802c820
	adds r3, r3, r0
.L_0802c64c:
	asrs r5, r3, #21
	ands r5, r1
	lsls r5, r5, #5
	ldr r1, .L_0802c824
	adds r5, r2, r5
	lsls r5, r5, #2
	adds r5, r5, r1
	ldr r0, [r5]
	movs r1, #1
	lsls r0, r0, #2
	lsrs r0, r0, #26
	bl Func_0802d088
	ldr r3, [r5]
	movs r1, #10
	lsls r3, r3, #2
	lsrs r0, r3, #26
	bl Math_Div
	movs r2, #3
	cmp r0, #3
	beq .L_0802c68c
	cmp r0, #3
	bgt .L_0802c684
	movs r2, #2
	cmp r0, #2
	beq .L_0802c68c
	b .L_0802c68a
.L_0802c684:
	movs r2, #1
	cmp r0, #4
	beq .L_0802c68c
.L_0802c68a:
	movs r2, #0
.L_0802c68c:
	lsls r3, r2, #3
	adds r3, r3, r2
	ldr r2, .L_0802c828
	lsls r3, r3, #2
	adds r7, r3, r2
	ldr r3, [sp, #20]
	movs r0, #144
	lsls r0, r0, #1
	adds r2, r3, r0
	ldr r3, [r2]
	cmp r7, r3
	beq .L_0802c6b4
	str r7, [r2]
	ldr r1, [sp, #20]
	movs r3, #144
	lsls r3, r3, #4
	adds r3, #88
	adds r2, r1, r3
	movs r3, #12
	str r3, [r2]
.L_0802c6b4:
	bl Func_080c8950
	movs r2, #144
	ldr r1, [sp, #20]
	lsls r2, r2, #4
	adds r2, #88
	adds r5, r1, r2
	ldr r3, [r5]
	cmp r3, #0
	bne .L_0802c6ca
	b .L_0802c80c
.L_0802c6ca:
	cmp r0, #0
	beq .L_0802c6d0
	b .L_0802c80c
.L_0802c6d0:
	movs r0, #128
	lsls r0, r0, #2
	bl Runtime_BumpAllocate
	ldr r3, [r5]
	mov r8, r0
	subs r3, #1
	cmp r3, #11
	bls .L_0802c6e4
	b .L_0802c7f6
.L_0802c6e4:
	ldr r2, .L_0802c82c
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
.L_0802c6ec:
	.4byte .L_0802c7ea
	.4byte .L_0802c7f6
	.4byte .L_0802c7dc
	.4byte .L_0802c7ca
	.4byte .L_0802c7bc
	.4byte .L_0802c7b0
	.4byte .L_0802c7a2
	.4byte .L_0802c796
	.4byte .L_0802c788
	.4byte .L_0802c77c
	.4byte .L_0802c76e
	.4byte .L_0802c71c
.L_0802c71c:
	movs r5, #160
	lsls r5, r5, #19
	ldr r0, [r7]
	movs r3, #0
	ldrsh r6, [r5, r3]
	bl Resource_GetTableEntry
	mov r1, r8
	bl Func_0801591c
	movs r3, #128
	movs r2, #132
	mov r0, r8
	lsls r3, r3, #19
	lsls r2, r2, #24
	strh r6, [r0]
	adds r3, #212
	adds r1, r5, #0
	adds r2, #112
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r5, .L_0802c830
	mov r0, r8
	bl Func_080c8970
	ldr r0, [r7, #4]
	bl Resource_GetTableEntry
	adds r1, r5, #0
	bl Func_0801587c
	adds r0, r5, #0
	bl Func_0802cc9c
	ldr r0, [r7, #8]
	bl Resource_GetTableEntry
	ldr r1, .L_0802c834
	bl Func_0801587c
	b .L_0802c7f6
.L_0802c76e:
	ldr r0, [r7, #12]
	bl Resource_GetTableEntry
	ldr r1, .L_0802c838
	bl Func_0801587c
	b .L_0802c7f6
.L_0802c77c:
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #212
	ldr r0, .L_0802c838
	ldr r1, .L_0802c83c
	b .L_0802c7d4
.L_0802c788:
	ldr r0, [r7, #16]
	bl Resource_GetTableEntry
	ldr r1, .L_0802c840
	bl Func_0801587c
	b .L_0802c7f6
.L_0802c796:
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #212
	ldr r0, .L_0802c840
	ldr r1, .L_0802c844
	b .L_0802c7d4
.L_0802c7a2:
	ldr r0, [r7, #20]
	bl Resource_GetTableEntry
	ldr r1, .L_0802c848
	bl Func_0801587c
	b .L_0802c7f6
.L_0802c7b0:
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #212
	ldr r0, .L_0802c848
	ldr r1, .L_0802c84c
	b .L_0802c7d4
.L_0802c7bc:
	ldr r0, [r7, #24]
	bl Resource_GetTableEntry
	ldr r1, .L_0802c850
	bl Func_0801587c
	b .L_0802c7f6
.L_0802c7ca:
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #212
	ldr r0, .L_0802c850
	ldr r1, .L_0802c854
.L_0802c7d4:
	ldr r2, .L_0802c858
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	b .L_0802c7f6
.L_0802c7dc:
	ldr r0, [r7, #28]
	bl Resource_GetTableEntry
	ldr r1, .L_0802c85c
	bl Func_0801587c
	b .L_0802c7f6
.L_0802c7ea:
	ldr r0, [r7, #32]
	bl Resource_GetTableEntry
	ldr r1, .L_0802c860
	bl Func_0801587c
.L_0802c7f6:
	ldr r1, [sp, #20]
	movs r3, #144
	lsls r3, r3, #4
	adds r3, #88
	adds r2, r1, r3
	ldr r3, [r2]
	mov r0, r8
	subs r3, #1
	str r3, [r2]
	bl Sys_Free
.L_0802c80c:
	add sp, #24
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0802c81c:
	.4byte 0xfec00000
.L_0802c820:
	.4byte 0x001fffff
.L_0802c824:
	.4byte gMapBlocks
.L_0802c828:
	.4byte Data_0802ed34
.L_0802c82c:
	.4byte .L_0802c6ec
.L_0802c830:
	.4byte Data_0202d000
.L_0802c834:
	.4byte gMapCollision
.L_0802c838:
	.4byte Data_02038000
.L_0802c83c:
	.4byte 0x06008000
.L_0802c840:
	.4byte Data_0203a000
.L_0802c844:
	.4byte 0x0600a000
.L_0802c848:
	.4byte Data_0203c000
.L_0802c84c:
	.4byte 0x0600c000
.L_0802c850:
	.4byte Data_0203e000
.L_0802c854:
	.4byte 0x0600e000
.L_0802c858:
	.4byte 0x84000800
.L_0802c85c:
	.4byte Data_02028000
.L_0802c860:
	.4byte Data_0202a000
