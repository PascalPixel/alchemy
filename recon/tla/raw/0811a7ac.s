.syntax unified
	.thumb
	.global BattlePresentation_SpawnActorObject
	.thumb_func
BattlePresentation_SpawnActorObject:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #20
	str r2, [sp, #16]
	adds r7, r0, #0
	lsls r2, r2, #16
	movs r0, #240
	str r3, [sp, #12]
	str r2, [sp, #8]
	lsls r3, r3, #16
	mov r9, r1
	lsls r0, r0, #8
	adds r1, r2, #0
	movs r2, #0
	str r3, [sp, #4]
	bl Func_080200c0
	mov r8, r0
	mov r0, r9
	bl Owner_GetState
	movs r2, #0
	mov r10, r0
	mov r0, r9
	mov r11, r2
	bl Func_0811a4e0
	movs r3, #42
	adds r3, #255
	add r3, r10
	ldrb r3, [r3]
	adds r6, r0, #0
	cmp r3, #0
	bne .L_0811a81e
	movs r5, #165
	lsls r5, r5, #1
	add r5, r10
	ldrh r0, [r5]
	bl Func_081280bc
	adds r1, r0, #0
	cmp r6, #0
	bne .L_0811a81a
	ldrh r0, [r5]
	str r1, [sp, #0]
	bl Func_081280d8
	mov r11, r0
	ldr r1, [sp, #0]
	b .L_0811a88c
.L_0811a81a:
	adds r1, r6, #0
	b .L_0811a88c
.L_0811a81e:
	movs r3, #165
	lsls r3, r3, #1
	add r3, r10
	ldrh r3, [r3]
	cmp r3, #7
	bhi .L_0811a87c
	ldr r2, .L_0811a878
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
	.2byte 0x0000
.L_0811a834:
	.4byte .L_0811a854
	.4byte .L_0811a85a
	.4byte .L_0811a864
	.4byte .L_0811a85e
	.4byte .L_0811a86a
	.4byte .L_0811a87c
	.4byte .L_0811a870
	.4byte .L_0811a874
.L_0811a854:
	movs r1, #245
	lsls r1, r1, #1
	b .L_0811a882
.L_0811a85a:
	movs r1, #240
	b .L_0811a880
.L_0811a85e:
	movs r1, #252
	lsls r1, r1, #1
	b .L_0811a882
.L_0811a864:
	movs r1, #250
	lsls r1, r1, #1
	b .L_0811a882
.L_0811a86a:
	movs r1, #254
	lsls r1, r1, #1
	b .L_0811a882
.L_0811a870:
	movs r1, #131
	b .L_0811a87e
.L_0811a874:
	movs r1, #133
	b .L_0811a87e
.L_0811a878:
	.4byte .L_0811a834
.L_0811a87c:
	movs r1, #129
.L_0811a87e:
	lsls r1, r1, #1
.L_0811a880:
	adds r1, #255
.L_0811a882:
	mov r3, r9
	cmp r3, #7
	bls .L_0811a88c
	movs r2, #1
	mov r11, r2
.L_0811a88c:
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r7, #24]
	movs r3, #128
	lsls r3, r3, #2
	adds r3, #174
	strh r3, [r7, #10]
	movs r3, #165
	lsls r3, r3, #1
	add r3, r10
	ldrh r3, [r3]
	movs r2, #193
	lsls r2, r2, #1
	cmp r3, r2
	bls .L_0811a8ae
	bl .L_0811b0a8
.L_0811a8ae:
	ldr r2, .L_0811a8b8
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	b .L_0811a8bc
	.2byte 0x0000
.L_0811a8b8:
	.4byte .L_0811a8c0
.L_0811a8bc:
	mov pc, r3
	.2byte 0x0000
.L_0811a8c0:
	.4byte .L_0811aecc
	.4byte .L_0811aed2
	.4byte .L_0811aede
	.4byte .L_0811aed8
	.4byte .L_0811aef4
	.4byte .L_0811aeec
	.4byte .L_0811aefa
	.4byte .L_0811aee4
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b010
	.4byte .L_0811b010
	.4byte .L_0811b010
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a0
	.4byte .L_0811b0a0
	.4byte .L_0811b0a0
	.4byte .L_0811b0a8
	.4byte .L_0811b04c
	.4byte .L_0811b04c
	.4byte .L_0811b066
	.4byte .L_0811b06c
	.4byte .L_0811b06c
	.4byte .L_0811b06c
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b02e
	.4byte .L_0811b034
	.4byte .L_0811b034
	.4byte .L_0811b024
	.4byte .L_0811b024
	.4byte .L_0811b024
	.4byte .L_0811b024
	.4byte .L_0811b024
	.4byte .L_0811b024
	.4byte .L_0811b024
	.4byte .L_0811b0a0
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b028
	.4byte .L_0811b028
	.4byte .L_0811b028
	.4byte .L_0811b052
	.4byte .L_0811b052
	.4byte .L_0811b052
	.4byte .L_0811b05e
	.4byte .L_0811b05e
	.4byte .L_0811b05e
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b038
	.4byte .L_0811b03c
	.4byte .L_0811b040
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b038
	.4byte .L_0811b03c
	.4byte .L_0811b040
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b07c
	.4byte .L_0811b07c
	.4byte .L_0811b07c
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b046
	.4byte .L_0811b046
	.4byte .L_0811b046
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b070
	.4byte .L_0811b076
	.4byte .L_0811b0a8
	.4byte .L_0811b06c
	.4byte .L_0811b06c
	.4byte .L_0811b06c
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b06c
	.4byte .L_0811b06c
	.4byte .L_0811b06c
	.4byte .L_0811b020
	.4byte .L_0811b020
	.4byte .L_0811b020
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b018
	.4byte .L_0811b018
	.4byte .L_0811b018
	.4byte .L_0811b018
	.4byte .L_0811b018
	.4byte .L_0811b018
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811af00
	.4byte .L_0811af06
	.4byte .L_0811af0c
	.4byte .L_0811af12
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811af42
	.4byte .L_0811af48
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811af9a
	.4byte .L_0811af9e
	.4byte .L_0811b0a0
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811affe
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b00a
	.4byte .L_0811b00a
	.4byte .L_0811b00a
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811af18
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811afa2
	.4byte .L_0811afa8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811af1c
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811afae
	.4byte .L_0811b0a8
	.4byte .L_0811afb4
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811afba
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b004
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811afc2
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a0
	.4byte .L_0811b0a0
	.4byte .L_0811af2c
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811afe6
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811af24
	.4byte .L_0811af28
	.4byte .L_0811afc6
	.4byte .L_0811afce
	.4byte .L_0811afd6
	.4byte .L_0811afda
	.4byte .L_0811afde
	.4byte .L_0811afe2
	.4byte .L_0811af32
	.4byte .L_0811af5a
	.4byte .L_0811afec
	.4byte .L_0811aff0
	.4byte .L_0811aff6
	.4byte .L_0811b00a
	.4byte .L_0811b00a
	.4byte .L_0811b00a
	.4byte .L_0811b00a
	.4byte .L_0811b00a
	.4byte .L_0811af5e
	.4byte .L_0811af3a
	.4byte .L_0811af64
	.4byte .L_0811af6a
	.4byte .L_0811af70
	.4byte .L_0811af76
	.4byte .L_0811af7c
	.4byte .L_0811af82
	.4byte .L_0811af88
	.4byte .L_0811af8e
	.4byte .L_0811af94
	.4byte .L_0811b0a8
	.4byte .L_0811b0a0
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a0
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a0
	.4byte .L_0811b0a8
	.4byte .L_0811b084
	.4byte .L_0811b084
	.4byte .L_0811b084
	.4byte .L_0811b084
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811b0a8
	.4byte .L_0811af4e
	.4byte .L_0811af54
.L_0811aecc:
	movs r3, #128
	lsls r3, r3, #9
	b .L_0811b0a6
.L_0811aed2:
	movs r3, #128
	lsls r3, r3, #9
	b .L_0811b0a6
.L_0811aed8:
	movs r3, #128
	lsls r3, r3, #9
	b .L_0811b0a6
.L_0811aede:
	movs r3, #128
	lsls r3, r3, #9
	b .L_0811b0a6
.L_0811aee4:
	movs r3, #134
	lsls r3, r3, #9
	adds r3, #204
	b .L_0811b0a6
.L_0811aeec:
	movs r3, #243
	lsls r3, r3, #8
	adds r3, #51
	b .L_0811b0a6
.L_0811aef4:
	movs r3, #128
	lsls r3, r3, #9
	b .L_0811b0a6
.L_0811aefa:
	movs r3, #230
	lsls r3, r3, #8
	b .L_0811b0a4
.L_0811af00:
	movs r3, #230
	lsls r3, r3, #8
	b .L_0811b0a4
.L_0811af06:
	movs r3, #230
	lsls r3, r3, #8
	b .L_0811b0a4
.L_0811af0c:
	movs r3, #230
	lsls r3, r3, #8
	b .L_0811b0a4
.L_0811af12:
	movs r3, #230
	lsls r3, r3, #8
	b .L_0811b0a4
.L_0811af18:
	ldr r3, .L_0811b08c
	b .L_0811b0a6
.L_0811af1c:
	movs r3, #198
	lsls r3, r3, #9
	adds r3, #204
	b .L_0811b0a6
.L_0811af24:
	ldr r3, .L_0811b090
	b .L_0811b0a6
.L_0811af28:
	ldr r3, .L_0811b08c
	b .L_0811b0a6
.L_0811af2c:
	movs r3, #192
	lsls r3, r3, #9
	b .L_0811b0a6
.L_0811af32:
	movs r3, #230
	lsls r3, r3, #9
	adds r3, #204
	b .L_0811b0a6
.L_0811af3a:
	movs r3, #230
	lsls r3, r3, #9
	adds r3, #204
	b .L_0811b0a6
.L_0811af42:
	movs r3, #128
	lsls r3, r3, #9
	b .L_0811b0a6
.L_0811af48:
	movs r3, #230
	lsls r3, r3, #8
	b .L_0811b0a4
.L_0811af4e:
	movs r3, #128
	lsls r3, r3, #9
	b .L_0811b0a6
.L_0811af54:
	movs r3, #230
	lsls r3, r3, #8
	b .L_0811b0a4
.L_0811af5a:
	ldr r3, .L_0811b094
	b .L_0811b0a6
.L_0811af5e:
	movs r3, #192
	lsls r3, r3, #9
	b .L_0811b0a6
.L_0811af64:
	movs r3, #192
	lsls r3, r3, #9
	b .L_0811b0a6
.L_0811af6a:
	movs r3, #192
	lsls r3, r3, #9
	b .L_0811b0a6
.L_0811af70:
	movs r3, #192
	lsls r3, r3, #9
	b .L_0811b0a6
.L_0811af76:
	movs r3, #192
	lsls r3, r3, #9
	b .L_0811b0a6
.L_0811af7c:
	movs r3, #192
	lsls r3, r3, #9
	b .L_0811b0a6
.L_0811af82:
	movs r3, #192
	lsls r3, r3, #9
	b .L_0811b0a6
.L_0811af88:
	movs r3, #192
	lsls r3, r3, #9
	b .L_0811b0a6
.L_0811af8e:
	movs r3, #192
	lsls r3, r3, #9
	b .L_0811b0a6
.L_0811af94:
	movs r3, #192
	lsls r3, r3, #9
	b .L_0811b0a6
.L_0811af9a:
	ldr r3, .L_0811b090
	b .L_0811b0a6
.L_0811af9e:
	ldr r3, .L_0811b090
	b .L_0811b0a6
.L_0811afa2:
	movs r3, #128
	lsls r3, r3, #9
	b .L_0811b0a6
.L_0811afa8:
	movs r3, #160
	lsls r3, r3, #9
	b .L_0811b0a6
.L_0811afae:
	movs r3, #230
	lsls r3, r3, #8
	b .L_0811b0a4
.L_0811afb4:
	movs r3, #192
	lsls r3, r3, #9
	b .L_0811b0a6
.L_0811afba:
	movs r3, #166
	lsls r3, r3, #9
	adds r3, #204
	b .L_0811b0a6
.L_0811afc2:
	ldr r3, .L_0811b090
	b .L_0811b0a6
.L_0811afc6:
	movs r3, #134
	lsls r3, r3, #9
	adds r3, #204
	b .L_0811b0a6
.L_0811afce:
	movs r3, #134
	lsls r3, r3, #9
	adds r3, #204
	b .L_0811b0a6
.L_0811afd6:
	ldr r3, .L_0811b098
	b .L_0811b0a6
.L_0811afda:
	ldr r3, .L_0811b098
	b .L_0811b0a6
.L_0811afde:
	ldr r3, .L_0811b090
	b .L_0811b0a6
.L_0811afe2:
	ldr r3, .L_0811b090
	b .L_0811b0a6
.L_0811afe6:
	movs r3, #192
	lsls r3, r3, #9
	b .L_0811b0a6
.L_0811afec:
	ldr r3, .L_0811b090
	b .L_0811b0a6
.L_0811aff0:
	movs r3, #192
	lsls r3, r3, #9
	b .L_0811b0a6
.L_0811aff6:
	movs r3, #198
	lsls r3, r3, #9
	adds r3, #204
	b .L_0811b0a6
.L_0811affe:
	movs r3, #160
	lsls r3, r3, #9
	b .L_0811b0a6
.L_0811b004:
	movs r3, #160
	lsls r3, r3, #9
	b .L_0811b0a6
.L_0811b00a:
	movs r3, #230
	lsls r3, r3, #8
	b .L_0811b0a4
.L_0811b010:
	movs r3, #243
	lsls r3, r3, #8
	adds r3, #51
	b .L_0811b0a6
.L_0811b018:
	movs r3, #166
	lsls r3, r3, #9
	adds r3, #204
	b .L_0811b0a6
.L_0811b020:
	ldr r3, .L_0811b09c
	b .L_0811b0a6
.L_0811b024:
	ldr r3, .L_0811b098
	b .L_0811b0a6
.L_0811b028:
	movs r3, #192
	lsls r3, r3, #9
	b .L_0811b0a6
.L_0811b02e:
	movs r3, #192
	lsls r3, r3, #9
	b .L_0811b0a6
.L_0811b034:
	ldr r3, .L_0811b098
	b .L_0811b0a6
.L_0811b038:
	movs r3, #147
	b .L_0811b0a2
.L_0811b03c:
	ldr r3, .L_0811b090
	b .L_0811b0a6
.L_0811b040:
	movs r3, #160
	lsls r3, r3, #9
	b .L_0811b0a6
.L_0811b046:
	movs r3, #160
	lsls r3, r3, #9
	b .L_0811b0a6
.L_0811b04c:
	movs r3, #192
	lsls r3, r3, #8
	b .L_0811b0a6
.L_0811b052:
	movs r3, #215
	lsls r3, r3, #1
	adds r3, #255
	strh r3, [r7, #10]
	movs r3, #147
	b .L_0811b0a2
.L_0811b05e:
	movs r3, #129
	lsls r3, r3, #9
	adds r3, #143
	b .L_0811b0a6
.L_0811b066:
	movs r3, #160
	lsls r3, r3, #9
	b .L_0811b0a6
.L_0811b06c:
	ldr r3, .L_0811b098
	b .L_0811b0a6
.L_0811b070:
	movs r3, #160
	lsls r3, r3, #9
	b .L_0811b0a6
.L_0811b076:
	movs r3, #128
	lsls r3, r3, #9
	b .L_0811b0a6
.L_0811b07c:
	movs r3, #204
	lsls r3, r3, #8
	adds r3, #204
	b .L_0811b0a6
.L_0811b084:
	movs r3, #204
	lsls r3, r3, #8
	adds r3, #204
	b .L_0811b0a6
.L_0811b08c:
	.4byte 0x00019999
.L_0811b090:
	.4byte 0x00013333
.L_0811b094:
	.4byte 0x0001b333
.L_0811b098:
	.4byte 0x00011999
.L_0811b09c:
	.4byte 0x00017333
.L_0811b0a0:
	movs r3, #179
.L_0811b0a2:
	lsls r3, r3, #9
.L_0811b0a4:
	adds r3, #102
.L_0811b0a6:
	str r3, [r7, #24]
.L_0811b0a8:
	mov r3, r8
	str r3, [r7]
	ldr r2, [sp, #8]
	mov r0, r9
	str r2, [r7, #12]
	ldr r3, [sp, #4]
	mov r2, r11
	str r3, [r7, #16]
	str r2, [r7, #20]
	strh r1, [r7, #4]
	bl Func_0811a674
	movs r5, #0
	ldr r6, .L_0811b0f4
	strh r5, [r7, #8]
	str r5, [r7, #32]
	str r5, [r7, #36]
	strh r5, [r7, #40]
	adds r3, r7, #0
	movs r5, #165
	adds r3, #42
	lsls r5, r5, #1
	strh r0, [r7, #6]
	add r5, r10
	strb r6, [r3]
	adds r3, #1
	strb r6, [r3]
	ldrh r3, [r5]
	cmp r3, #1
	bhi .L_0811b10e
	mov r0, r10
	movs r1, #1
	bl Inventory_GetEquippedItemFar
	cmp r0, #15
	bne .L_0811b10e
	b .L_0811b0f8
	.2byte 0x0000
.L_0811b0f4:
	.4byte 0x00000000
.L_0811b0f8:
	ldrh r3, [r5]
	cmp r3, #0
	bne .L_0811b102
	movs r1, #190
	b .L_0811b104
.L_0811b102:
	movs r1, #191
.L_0811b104:
	lsls r1, r1, #1
	adds r1, #255
	strh r1, [r7, #4]
	movs r3, #0
	strh r3, [r7, #6]
.L_0811b10e:
	ldr r3, [sp, #12]
	cmp r3, #0
	bge .L_0811b116
	adds r3, #7
.L_0811b116:
	asrs r0, r3, #3
	ldr r1, [sp, #16]
	bl ArcTan2
	movs r3, #128
	lsls r3, r3, #8
	adds r0, r0, r3
	mov r2, r8
	movs r3, #3
	strh r0, [r2, #6]
	adds r2, #89
	strb r3, [r2]
	movs r3, #2
	subs r2, #4
	strb r3, [r2]
	movs r3, #42
	adds r3, #255
	add r3, r10
	ldrb r3, [r3]
	cmp r3, #0
	bne .L_0811b148
	movs r3, #166
	lsls r3, r3, #9
	adds r3, #204
	b .L_0811b14c
.L_0811b148:
	movs r3, #128
	lsls r3, r3, #9
.L_0811b14c:
	mov r2, r8
	str r3, [r2, #24]
	str r3, [r2, #28]
	ldr r1, .L_0811b168
	mov r0, r8
	bl ObjectDispatch_InitializeFar
	add sp, #20
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0811b168:
	.4byte Data_0812cacc
