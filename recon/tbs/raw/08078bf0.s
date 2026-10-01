.syntax unified
	.thumb
	.global Owner_RefreshClassActions
	.thumb_func
Owner_RefreshClassActions:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #8
	bl Owner_GetState
	ldr r5, .L_08078c50
	mov r9, r0
	movs r0, #88
	add r5, r9
	add r0, r9
	mov r8, r0
	ldrb r0, [r5]
	bl Owner_GetRecordStride84
	str r0, [sp, #4]
	ldrb r3, [r5]
	movs r4, #128
	lsls r4, r4, #8
	ldr r1, .L_08078c4c
	mov r2, r8
	movs r5, #31
	movs r0, #0
	cmp r3, #0
	bne .L_08078c2c
	b .L_08078e16
.L_08078c2c:
	ldrh r3, [r2]
	ands r3, r4
	cmp r3, #0
	beq .L_08078c36
	strh r1, [r2]
.L_08078c36:
	subs r5, #1
	adds r2, #4
	cmp r5, #0
	bge .L_08078c2c
	movs r4, #128
	ldr r1, .L_08078c4c
	lsls r4, r4, #7
	mov r2, r8
	movs r5, #31
	b .L_08078c54
	.2byte 0x0000
.L_08078c4c:
	.4byte 0x00000000
.L_08078c50:
	.4byte 0x00000129
.L_08078c54:
	ldrh r3, [r2]
	ands r3, r4
	cmp r3, #0
	beq .L_08078c5e
	strh r1, [r2]
.L_08078c5e:
	subs r5, #1
	adds r2, #4
	cmp r5, #0
	bge .L_08078c54
	mov r1, r8
	movs r4, #31
	movs r5, #31
	adds r1, #124
.L_08078c6e:
	lsls r3, r4, #2
	mov r0, r8
	ldrh r2, [r3, r0]
	adds r3, r2, #0
	cmp r3, #0
	bne .L_08078c7e
	subs r4, #1
	b .L_08078c86
.L_08078c7e:
	strh r2, [r1]
	subs r4, #1
	subs r1, #4
	subs r5, #1
.L_08078c86:
	cmp r4, #0
	bge .L_08078c6e
	cmp r5, #0
	blt .L_08078ca4
	lsls r3, r5, #2
	ldr r2, .L_08078ca0
	add r3, r8
.L_08078c94:
	subs r5, #1
	strh r2, [r3]
	subs r3, #4
	cmp r5, #0
	bge .L_08078c94
	b .L_08078ca4
.L_08078ca0:
	.4byte 0x00000000
.L_08078ca4:
	ldr r2, [sp, #4]
	movs r1, #128
	adds r2, #16
	lsls r1, r1, #8
	mov lr, r2
	movs r3, #16
	movs r5, #0
	mov r11, r1
	mov r10, r3
	mov r7, lr
.L_08078cb8:
	ldrb r3, [r7]
	cmp r3, #0
	beq .L_08078d24
	mov r0, r9
	mov r1, lr
	ldrb r2, [r0, #15]
	ldrb r3, [r1, #1]
	cmp r2, r3
	bcc .L_08078d24
	mov r2, r8
	ldrh r6, [r2]
	ldrb r3, [r1]
	mov r12, r6
	movs r4, #0
	cmp r12, r3
	beq .L_08078cf0
	mov r12, r10
	mov r1, r8
.L_08078cdc:
	adds r4, #1
	cmp r4, #31
	bgt .L_08078cf0
	ldr r3, [sp, #4]
	adds r1, #4
	mov r0, r12
	ldrh r2, [r1]
	ldrb r3, [r3, r0]
	cmp r2, r3
	bne .L_08078cdc
.L_08078cf0:
	cmp r4, #32
	bne .L_08078d24
	adds r3, r6, #0
	movs r4, #0
	cmp r3, #0
	bne .L_08078d06
	ldrb r3, [r7]
	mov r1, r11
	orrs r3, r1
	mov r2, r8
	b .L_08078d1e
.L_08078d06:
	adds r4, #1
	cmp r4, #31
	bgt .L_08078d20
	lsls r3, r4, #2
	mov r0, r8
	adds r2, r3, r0
	ldrh r3, [r2]
	cmp r3, #0
	bne .L_08078d06
	ldrb r3, [r7]
	mov r1, r11
	orrs r3, r1
.L_08078d1e:
	strh r3, [r2]
.L_08078d20:
	cmp r4, #32
	beq .L_08078d32
.L_08078d24:
	movs r2, #4
	adds r5, #1
	add lr, r2
	adds r7, #4
	add r10, r2
	cmp r5, #15
	ble .L_08078cb8
.L_08078d32:
	movs r3, #216
	movs r5, #0
	mov r10, r3
.L_08078d38:
	mov r0, r10
	mov r1, r9
	ldrh r2, [r0, r1]
	adds r3, r2, #0
	cmp r3, #0
	beq .L_08078dc4
	ldr r3, .L_08078d68
	ands r3, r2
	cmp r3, #0
	beq .L_08078dc4
	ldrh r0, [r0, r1]
	bl Item_GetDirect
	ldrb r3, [r0, #12]
	cmp r3, #3
	bne .L_08078dc4
	mov r2, r8
	ldrh r6, [r2]
	ldr r3, .L_08078d6c
	ldrh r0, [r0, #40]
	ands r3, r6
	mov r12, r0
	movs r4, #0
	b .L_08078d70
.L_08078d68:
	.4byte 0x00000200
.L_08078d6c:
	.4byte 0x00003fff
.L_08078d70:
	cmp r3, r12
	beq .L_08078d8a
	ldr r7, .L_08078da4
	mov r1, r8
.L_08078d78:
	adds r4, #1
	cmp r4, #31
	bgt .L_08078d8a
	adds r1, #4
	ldrh r2, [r1]
	adds r3, r7, #0
	ands r3, r2
	cmp r3, r12
	bne .L_08078d78
.L_08078d8a:
	cmp r4, #32
	bne .L_08078dc4
	adds r3, r6, #0
	movs r4, #0
	cmp r3, #0
	bne .L_08078da8
	ldr r3, .L_08078da0
	orrs r3, r0
	mov r0, r8
	strh r3, [r0]
	b .L_08078dc0
.L_08078da0:
	.4byte 0x00004000
.L_08078da4:
	.4byte 0x00003fff
.L_08078da8:
	adds r4, #1
	cmp r4, #31
	bgt .L_08078dc0
	lsls r3, r4, #2
	mov r1, r8
	adds r2, r3, r1
	ldrh r3, [r2]
	cmp r3, #0
	bne .L_08078da8
	ldr r3, .L_08078de4
	orrs r3, r0
	strh r3, [r2]
.L_08078dc0:
	cmp r4, #32
	beq .L_08078dce
.L_08078dc4:
	movs r2, #2
	adds r5, #1
	add r10, r2
	cmp r5, #14
	ble .L_08078d38
.L_08078dce:
	movs r4, #0
	movs r5, #0
	mov r1, r8
.L_08078dd4:
	lsls r3, r4, #2
	mov r0, r8
	ldrh r2, [r3, r0]
	adds r3, r2, #0
	cmp r3, #0
	bne .L_08078de8
	adds r4, #1
	b .L_08078df0
.L_08078de4:
	.4byte 0x00004000
.L_08078de8:
	strh r2, [r1]
	adds r4, #1
	adds r1, #4
	adds r5, #1
.L_08078df0:
	cmp r4, #31
	ble .L_08078dd4
	cmp r5, #31
	bgt .L_08078e14
	lsls r3, r5, #2
	mov r0, r8
	adds r2, r3, r0
	ldr r1, .L_08078e10
	movs r3, #32
	subs r5, r3, r5
.L_08078e04:
	subs r5, #1
	strh r1, [r2]
	adds r2, #4
	cmp r5, #0
	bne .L_08078e04
	b .L_08078e14
.L_08078e10:
	.4byte 0x00000000
.L_08078e14:
	movs r0, #0
.L_08078e16:
	add sp, #8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r1}
	bx r1
