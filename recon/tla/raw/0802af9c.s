.syntax unified
	.thumb
	.global Func_0802af9c
	.thumb_func
Func_0802af9c:
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
	mov r8, r0
	adds r0, r1, #0
	movs r1, #132
	sub sp, #40
	lsls r1, r1, #1
	str r3, [sp, #24]
	adds r7, r3, r1
	ldr r3, .L_0802b178
	ldr r2, .L_0802b17c
	adds r0, r0, r3
	ldr r3, [sp, #24]
	add r8, r2
	adds r3, #236
	ldr r3, [r3]
	cmp r8, r3
	bge .L_0802afd2
	mov r8, r3
.L_0802afd2:
	ldr r3, [sp, #24]
	ldr r4, .L_0802b180
	adds r3, #244
	ldr r3, [r3]
	adds r3, r3, r4
	cmp r8, r3
	ble .L_0802afe2
	mov r8, r3
.L_0802afe2:
	ldr r3, [sp, #24]
	adds r3, #240
	ldr r3, [r3]
	cmp r0, r3
	bge .L_0802afee
	adds r0, r3, #0
.L_0802afee:
	ldr r3, [sp, #24]
	ldr r1, .L_0802b184
	adds r3, #248
	ldr r3, [r3]
	adds r3, r3, r1
	cmp r0, r3
	ble .L_0802affe
	adds r0, r3, #0
.L_0802affe:
	ldr r2, [sp, #24]
	mov r3, r8
	adds r2, #228
	str r2, [sp, #16]
	str r3, [r2]
	ldr r4, [sp, #24]
	movs r1, #0
	adds r4, #232
	str r4, [sp, #12]
	str r0, [r4]
	str r1, [sp, #36]
.L_0802b014:
	ldr r2, [sp, #36]
	movs r4, #130
	ldr r1, [sp, #24]
	lsls r4, r4, #1
	adds r3, r2, r4
	ldrb r3, [r1, r3]
	cmp r3, #0
	bne .L_0802b026
	b .L_0802b15e
.L_0802b026:
	ldr r2, [sp, #16]
	ldr r1, [r7, #16]
	ldr r0, [r2]
	ldr r3, .L_0802b188
	mov lr, r3
	.2byte 0xf800
	ldr r4, [sp, #12]
	ldr r2, .L_0802b188
	mov r8, r0
	ldr r1, [r7, #20]
	ldr r0, [r4]
	mov lr, r2
	.2byte 0xf800
	ldr r2, [r7, #24]
	cmp r2, #0
	beq .L_0802b04e
	ldr r3, [r7, #32]
	adds r3, r3, r2
	str r3, [r7, #32]
	add r8, r3
.L_0802b04e:
	ldr r2, [r7, #28]
	cmp r2, #0
	beq .L_0802b05c
	ldr r3, [r7, #36]
	adds r3, r3, r2
	str r3, [r7, #36]
	adds r0, r0, r3
.L_0802b05c:
	ldr r3, [r7, #8]
	add r8, r3
	ldr r3, [r7, #12]
	mov r1, r8
	adds r0, r0, r3
	cmp r1, #0
	bge .L_0802b06e
	ldr r1, .L_0802b18c
	add r1, r8
.L_0802b06e:
	asrs r3, r1, #19
	mov r8, r3
	adds r2, r0, #0
	cmp r0, #0
	bge .L_0802b07c
	ldr r4, .L_0802b18c
	adds r2, r0, r4
.L_0802b07c:
	ldr r4, [sp, #36]
	asrs r0, r2, #19
	lsls r3, r4, #11
	ldr r4, .L_0802b190
	adds r4, r3, r4
	str r4, [sp, #32]
	ldrh r3, [r7, #42]
	ldrh r4, [r7, #40]
	mov r10, r3
	ldrh r3, [r7, #44]
	mov lr, r4
	ldr r4, .L_0802b194
	ldrh r5, [r7, #46]
	str r3, [sp, #20]
	mov r3, lr
	str r4, [sp, #28]
	cmp r3, #127
	beq .L_0802b0aa
	ldr r4, [sp, #20]
	lsls r3, r4, #2
	ldr r4, .L_0802b194
	adds r4, r3, r4
	str r4, [sp, #28]
.L_0802b0aa:
	movs r3, #254
	lsls r3, r3, #6
	mov r12, r3
	cmp r10, r12
	beq .L_0802b0bc
	ldr r4, [sp, #28]
	lsls r3, r5, #2
	adds r4, r4, r3
	str r4, [sp, #28]
.L_0802b0bc:
	lsrs r3, r2, #31
	adds r3, r0, r3
	asrs r3, r3, #1
	movs r2, #127
	ands r3, r2
	lsls r4, r3, #7
	movs r3, #30
	ands r3, r0
	lsls r6, r3, #5
	cmp r10, r12
	beq .L_0802b0d8
	subs r4, r4, r5
	mov r2, r10
	ands r4, r2
.L_0802b0d8:
	movs r3, #0
	mov r9, r3
	lsrs r3, r1, #31
	add r3, r8
	asrs r3, r3, #1
	str r3, [sp, #8]
.L_0802b0e4:
	ldr r1, [sp, #8]
	movs r0, #127
	movs r5, #30
	mov r2, r8
	mov r3, lr
	ands r0, r1
	ands r5, r2
	cmp r3, #127
	beq .L_0802b0fc
	ldr r1, [sp, #20]
	subs r0, r0, r1
	ands r0, r3
.L_0802b0fc:
	movs r2, #0
	movs r3, #30
	mov r12, r2
	mov r11, r3
.L_0802b104:
	ldr r2, [sp, #28]
	adds r3, r4, r0
	lsls r3, r3, #2
	ldr r1, [r3, r2]
	ldr r2, .L_0802b198
	lsls r1, r1, #21
	lsrs r1, r1, #18
	str r1, [sp, #4]
	adds r3, r1, r2
	ldr r1, [sp, #32]
	adds r2, r6, r5
	lsls r2, r2, #1
	adds r2, r2, r1
	str r2, [sp, #0]
	ldr r1, .L_0802b19c
	ldr r3, [r3]
	adds r0, #1
	str r3, [r2]
	ldr r2, [sp, #4]
	adds r5, #2
	adds r3, r2, r1
	ldr r3, [r3]
	ldr r2, [sp, #0]
	mov r1, r11
	str r3, [r2, #64]
	movs r2, #1
	mov r3, lr
	add r12, r2
	ands r0, r3
	mov r3, r12
	ands r5, r1
	cmp r3, #15
	bls .L_0802b104
	movs r3, #240
	add r9, r2
	adds r4, #128
	mov r1, r10
	adds r6, #64
	lsls r3, r3, #2
	mov r2, r9
	ands r4, r1
	ands r6, r3
	cmp r2, #10
	bls .L_0802b0e4
	adds r7, #56
.L_0802b15e:
	ldr r3, [sp, #36]
	adds r3, #1
	str r3, [sp, #36]
	cmp r3, #2
	bhi .L_0802b16a
	b .L_0802b014
.L_0802b16a:
	add sp, #40
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0802b178:
	.4byte 0xffa00000
.L_0802b17c:
	.4byte 0xff880000
.L_0802b180:
	.4byte 0xff100000
.L_0802b184:
	.4byte 0xff600000
.L_0802b188:
	.4byte IwramMulQ16
.L_0802b18c:
	.4byte 0x0007ffff
.L_0802b190:
	.4byte 0x06002800
.L_0802b194:
	.4byte gMapCellBuffer
.L_0802b198:
	.4byte gMapBlocks
.L_0802b19c:
	.4byte Data_02020004
