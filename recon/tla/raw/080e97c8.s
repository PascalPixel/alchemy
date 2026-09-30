.syntax unified
	.thumb
	.global Func_080e97c8
	.thumb_func
Func_080e97c8:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r1, #201
	lsls r1, r1, #5
	movs r0, #92
	bl Runtime_AllocateHeapBlock
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #224
	ldr r5, [r3]
	bl BattleEffect_InitializeSharedScene
	ldr r0, [r5, #16]
	movs r1, #0
	bl Func_080e1420
	movs r0, #2
	bl Func_080e89e4
	movs r0, #10
	bl WaitFrames
	movs r6, #128
	movs r3, #0
	mov r8, r3
	lsls r6, r6, #10
	movs r5, #0
	movs r7, #0
.L_080e9804:
	mov r1, r8
	movs r3, #1
	movs r0, #2
	adds r2, r6, #0
	add r8, r3
	bl Func_080e8b44
	cmp r5, #1
	beq .L_080e9840
	cmp r5, #1
	bgt .L_080e9820
	cmp r5, #0
	beq .L_080e982a
	b .L_080e986c
.L_080e9820:
	cmp r5, #2
	beq .L_080e984c
	cmp r5, #3
	beq .L_080e9860
	b .L_080e986c
.L_080e982a:
	movs r3, #152
	lsls r3, r3, #6
	adds r3, #102
	adds r6, r6, r3
	ldr r3, .L_080e9894
	cmp r6, r3
	ble .L_080e986c
	movs r7, #1
	movs r5, #1
	negs r7, r7
	b .L_080e986c
.L_080e9840:
	cmp r7, #10
	bne .L_080e986c
	movs r7, #1
	movs r5, #2
	negs r7, r7
	b .L_080e986c
.L_080e984c:
	ldr r3, .L_080e9898
	adds r6, r6, r3
	movs r3, #128
	lsls r3, r3, #10
	cmp r6, r3
	bgt .L_080e986c
	movs r7, #1
	movs r5, #3
	negs r7, r7
	b .L_080e986c
.L_080e9860:
	movs r6, #0
	cmp r7, #0
	bne .L_080e986c
	movs r5, #186
	lsls r5, r5, #2
	adds r5, #255
.L_080e986c:
	movs r0, #1
	bl WaitFrames
	movs r3, #186
	lsls r3, r3, #2
	adds r3, #255
	adds r7, #1
	cmp r5, r3
	bne .L_080e9804
	bl Func_080e8c9c
	bl BattleFx_PrepareBufferInterpolation
	movs r0, #92
	bl Runtime_ReleaseHeapBlock
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080e9894:
	.4byte 0x0005ffff
.L_080e9898:
	.4byte 0xffffd99a
