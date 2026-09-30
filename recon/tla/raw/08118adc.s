.syntax unified
	.thumb
	.global Func_08118adc
	.thumb_func
Func_08118adc:
	push {r5, r6, r7, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #36]
	adds r2, r3, #0
	ldr r5, [r3, #48]
	adds r3, r1, #0
	adds r3, #68
	ldrb r3, [r3]
	adds r2, #176
	sub sp, #16
	ldr r7, [r2]
	cmp r3, #0
	beq .L_08118b42
	ldr r3, .L_08118bc0
	ldrh r2, [r3]
	movs r3, #3
	ands r3, r2
	cmp r3, #3
	beq .L_08118b20
	adds r2, r1, #0
	adds r2, #81
	ldrb r3, [r2]
	adds r3, #1
	strb r3, [r2]
	movs r2, #192
	lsls r3, r3, #24
	lsls r2, r2, #21
	cmp r3, r2
	bls .L_08118b42
	adds r2, r1, #0
	adds r2, #82
	movs r3, #1
	b .L_08118b40
.L_08118b20:
	ldr r3, .L_08118bc4
	adds r2, r1, #0
	ldr r3, [r3]
	adds r2, #80
	ldrb r2, [r2]
	lsls r3, r3, #26
	lsrs r3, r3, #30
	cmp r2, r3
	beq .L_08118b3a
	adds r2, r1, #0
	adds r2, #82
	movs r3, #1
	strb r3, [r2]
.L_08118b3a:
	adds r2, r1, #0
	adds r2, #81
	movs r3, #0
.L_08118b40:
	strb r3, [r2]
.L_08118b42:
	ldr r3, [r7, #4]
	cmp r3, #0
	beq .L_08118b66
	ldr r3, [r7]
	ldrh r1, [r5, #54]
	subs r3, r3, r1
	lsls r3, r3, #16
	asrs r3, r3, #16
	adds r2, r3, #0
	cmp r3, #0
	bge .L_08118b5a
	adds r2, #15
.L_08118b5a:
	asrs r3, r2, #4
	adds r3, r1, r3
	strh r3, [r5, #54]
	ldr r3, [r7, #4]
	subs r3, #1
	str r3, [r7, #4]
.L_08118b66:
	ldr r3, [r5, #28]
	adds r6, r5, #0
	adds r6, #12
	cmp r3, #0
	beq .L_08118b72
	adds r6, r3, #0
.L_08118b72:
	bl Func_08014e1c
	movs r3, #54
	ldrsh r0, [r5, r3]
	bl Func_080150ac
	movs r2, #52
	ldrsh r0, [r5, r2]
	bl SceneTransform_ApplyPitch
	adds r0, r6, #0
	bl Func_08015198
	movs r3, #0
	add r0, sp, #4
	str r3, [r0]
	str r3, [r0, #4]
	adds r1, r5, #0
	ldr r3, [r5, #32]
	str r3, [r0, #8]
	ldr r3, .L_08118bc8
	mov lr, r3
	.2byte 0xf800
	ldr r3, [r7, #20]
	cmp r3, #0
	bne .L_08118bba
	movs r1, #240
	movs r3, #128
	lsls r3, r3, #9
	lsls r1, r1, #15
	str r3, [sp, #0]
	adds r0, r1, #0
	movs r2, #0
	movs r3, #0
	bl BattleCamera_SetRange
.L_08118bba:
	add sp, #16
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_08118bc0:
	.4byte gLinkStatus
.L_08118bc4:
	.4byte 0x04000128
.L_08118bc8:
	.4byte IwramTransformVector
