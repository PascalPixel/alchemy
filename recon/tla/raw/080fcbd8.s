.syntax unified
	.thumb
	.global Func_080fcbd8
	.thumb_func
Func_080fcbd8:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #12
	movs r5, #0
	str r0, [sp, #8]
	str r2, [sp, #4]
	str r5, [sp, #0]
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r7, [r3]
	mov r11, r5
	b .L_080fcedc
.L_080fcbfc:
	cmp r5, #4
	bls .L_080fcc02
	b .L_080fced8
.L_080fcc02:
	ldr r2, .L_080fcd78
	lsls r3, r5, #2
	ldr r3, [r3, r2]
	mov pc, r3
	.2byte 0x0000
.L_080fcc0c:
	.4byte .L_080fcc20
	.4byte .L_080fcc50
	.4byte .L_080fcd52
	.4byte .L_080fcd2a
	.4byte .L_080fcdc2
.L_080fcc20:
	movs r3, #180
	lsls r3, r3, #1
	adds r2, r7, r3
	movs r3, #0
	strh r3, [r2]
	ldr r1, .L_080fcd7c
	movs r0, #0
	bl Func_080facb4
	movs r0, #0
	bl Func_080fcf5c
	movs r3, #1
	negs r3, r3
	cmp r0, r3
	bne .L_080fcc46
	movs r2, #1
	str r2, [sp, #0]
	mov r11, r3
.L_080fcc46:
	ldr r0, [r7, #48]
	bl RenderOutput_RedrawSavedRectFar
	movs r5, #1
	b .L_080fcedc
.L_080fcc50:
	movs r0, #1
	bl WaitFrames
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #22
	adds r3, r7, r2
	ldrb r0, [r3]
	bl Owner_GetState
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r7, r2
	ldrb r3, [r3]
	movs r5, #0
	cmp r3, #0
	bne .L_080fcc74
	b .L_080fcedc
.L_080fcc74:
	movs r2, #155
	lsls r2, r2, #2
	adds r3, r7, r2
	ldrb r3, [r3]
	cmp r3, #1
	beq .L_080fcc94
	cmp r3, #1
	bgt .L_080fcc8a
	cmp r3, #0
	beq .L_080fcc90
	b .L_080fcca6
.L_080fcc8a:
	cmp r3, #2
	beq .L_080fcc9e
	b .L_080fcca6
.L_080fcc90:
	ldr r1, .L_080fcd80
	b .L_080fcc96
.L_080fcc94:
	ldr r1, .L_080fcd84
.L_080fcc96:
	movs r0, #0
	bl Func_080facb4
	b .L_080fcca6
.L_080fcc9e:
	ldr r1, .L_080fcd88
	movs r0, #0
	bl Func_080facb4
.L_080fcca6:
	bl Func_08100700
	movs r3, #128
	lsls r3, r3, #2
	adds r3, #22
	adds r6, r7, r3
	ldrb r1, [r6]
	movs r2, #0
	ldr r0, [r7, #40]
	movs r3, #0
	bl Func_080f8170
	movs r0, #0
	bl Func_080fdad4
	movs r2, #1
	negs r2, r2
	adds r1, r0, #0
	mov r8, r2
	movs r5, #0
	cmp r1, r8
	bne .L_080fccd4
	b .L_080fcedc
.L_080fccd4:
	movs r2, #155
	lsls r2, r2, #2
	adds r3, r7, r2
	ldrb r3, [r3]
	movs r5, #2
	cmp r3, #0
	bne .L_080fcce4
	b .L_080fcedc
.L_080fcce4:
	cmp r3, #1
	bne .L_080fcd08
	movs r2, #0
	ldrb r0, [r6]
	bl Func_080fd52c
	ldr r0, [r7, #48]
	bl RenderOutput_ClearListFar
	ldr r0, .L_080fcd8c
	mov r1, r8
	mov r2, r8
	bl Func_080f8ce8
	ldr r0, [r7, #48]
	bl RenderOutput_RedrawSavedRectFar
	b .L_080fcd26
.L_080fcd08:
	movs r2, #1
	ldrb r0, [r6]
	bl Func_080fd52c
	ldr r0, [r7, #48]
	bl RenderOutput_ClearListFar
	ldr r0, .L_080fcd90
	mov r1, r8
	mov r2, r8
	bl Func_080f8ce8
	ldr r0, [r7, #48]
	bl RenderOutput_RedrawSavedRectFar
.L_080fcd26:
	movs r5, #0
	b .L_080fcedc
.L_080fcd2a:
	ldr r1, .L_080fcd94
	movs r0, #0
	bl Func_080facb4
	movs r0, #0
	bl Func_080fd2e8
	movs r3, #1
	mov r10, r0
	negs r3, r3
	movs r5, #4
	cmp r10, r3
	beq .L_080fcd46
	b .L_080fcedc
.L_080fcd46:
	movs r2, #135
	lsls r2, r2, #2
	adds r1, r7, r2
	ldrh r2, [r1]
	ldr r3, .L_080fcd74
	b .L_080fcec6
.L_080fcd52:
	bl Func_080fcf14
	cmp r0, #1
	bne .L_080fcd5e
.L_080fcd5a:
	movs r5, #3
	b .L_080fcedc
.L_080fcd5e:
	cmp r0, #2
	bne .L_080fcd98
	movs r3, #140
	lsls r3, r3, #1
	adds r3, #255
	adds r2, r7, r3
	movs r3, #9
	strb r3, [r2]
	movs r5, #4
	b .L_080fcedc
	.2byte 0x0000
.L_080fcd74:
	.4byte 0x00000001
.L_080fcd78:
	.4byte .L_080fcc0c
.L_080fcd7c:
	.4byte 0x00001018
.L_080fcd80:
	.4byte 0x00001019
.L_080fcd84:
	.4byte 0x00001020
.L_080fcd88:
	.4byte 0x0000101f
.L_080fcd8c:
	.4byte 0x00001011
.L_080fcd90:
	.4byte 0x00001012
.L_080fcd94:
	.4byte 0x0000101a
.L_080fcd98:
	movs r2, #1
	str r2, [sp, #0]
	mov r11, r2
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #22
	adds r3, r7, r2
	ldrb r3, [r3]
	ldr r2, [sp, #8]
	str r3, [r2]
	movs r2, #182
	lsls r2, r2, #1
	adds r3, r7, r2
	ldrh r2, [r3]
	movs r3, #252
	lsls r3, r3, #6
	adds r3, #255
	ands r3, r2
	ldr r2, [sp, #4]
	str r3, [r2]
	b .L_080fcedc
.L_080fcdc2:
	movs r2, #182
	lsls r2, r2, #1
	adds r2, r2, r7
	movs r3, #0
	ldrh r0, [r2]
	mov r10, r3
	mov r8, r2
	movs r3, #128
	movs r2, #140
	lsls r3, r3, #2
	lsls r2, r2, #1
	adds r3, #22
	adds r2, #255
	adds r5, r7, r3
	adds r6, r7, r2
	movs r3, #0
	ldrb r1, [r5]
	ldrb r2, [r6]
	bl Func_081007f8
	ldrb r3, [r6]
	mov r11, r0
	cmp r3, #9
	bne .L_080fcdfa
	ldrb r3, [r5]
	strb r3, [r6]
	movs r3, #9
	mov r10, r3
.L_080fcdfa:
	movs r2, #1
	negs r2, r2
	mov r9, r2
	cmp r11, r9
	beq .L_080fce20
	mov r2, r8
	ldrh r3, [r2]
	movs r0, #252
	lsls r0, r0, #6
	adds r0, #255
	ands r0, r3
	bl BattleAction_Get
	ldrb r3, [r5]
	ldrb r1, [r0, #9]
	adds r0, r3, #0
	negs r1, r1
	bl Owner_AdjustSecondValueFar
.L_080fce20:
	ldrb r0, [r5]
	bl BattleUnit_Recalculate
	cmp r11, r9
	beq .L_080fce66
	ldrb r1, [r6]
	ldr r0, [r7, #40]
	movs r2, #0
	movs r3, #0
	bl Func_080f8170
	mov r2, r8
	ldrh r3, [r2]
	movs r0, #252
	lsls r0, r0, #6
	adds r0, #255
	ands r0, r3
	bl Func_08100d58
	ldr r0, [r7, #48]
	bl RenderOutput_ClearListFar
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #94
	adds r3, r7, r2
	movs r2, #0
	ldrsh r0, [r3, r2]
	ldr r3, .L_080fced4
	movs r1, #0
	adds r0, r0, r3
	mov r2, r9
	bl Func_080f8ce8
	b .L_080fce8a
.L_080fce66:
	movs r0, #114
	bl Audio_PlayCue
	ldr r0, [r7, #48]
	bl RenderOutput_ClearListFar
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #94
	adds r3, r7, r2
	movs r2, #0
	ldrsh r0, [r3, r2]
	ldr r3, .L_080fced4
	mov r1, r11
	adds r0, r0, r3
	mov r2, r11
	bl Func_080f8ce8
.L_080fce8a:
	movs r3, #1
	negs r3, r3
	cmp r11, r3
	bne .L_080fceb8
	movs r3, #128
	lsls r3, r3, #2
	adds r3, #30
	adds r2, r7, r3
	movs r3, #1
	strh r3, [r2]
	mov r2, r10
	ldr r1, .L_080fced0
	cmp r2, #9
	beq .L_080fcea8
	b .L_080fcd5a
.L_080fcea8:
	movs r3, #135
	lsls r3, r3, #2
	adds r2, r7, r3
	ldrh r3, [r2]
	movs r5, #1
	orrs r3, r1
	strh r3, [r2]
	b .L_080fcedc
.L_080fceb8:
	movs r3, #135
	lsls r3, r3, #2
	adds r1, r7, r3
	movs r2, #1
	mov r11, r2
	ldr r3, .L_080fced0
	ldrh r2, [r1]
.L_080fcec6:
	movs r5, #1
	orrs r3, r2
	strh r3, [r1]
	b .L_080fcedc
	.2byte 0x0000
.L_080fced0:
	.4byte 0x00000001
.L_080fced4:
	.4byte 0x00001120
.L_080fced8:
	movs r2, #1
	str r2, [sp, #0]
.L_080fcedc:
	ldr r3, [sp, #0]
	cmp r3, #0
	bne .L_080fcef0
	movs r0, #168
	lsls r0, r0, #1
	bl GameFlag_Test
	cmp r0, #0
	bne .L_080fcef0
	b .L_080fcbfc
.L_080fcef0:
	movs r0, #168
	lsls r0, r0, #1
	bl GameFlag_Test
	cmp r0, #0
	beq .L_080fcf02
	movs r2, #1
	negs r2, r2
	mov r11, r2
.L_080fcf02:
	mov r0, r11
	add sp, #12
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
