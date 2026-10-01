.syntax unified
	.thumb
	.global Func_080de8d0
	.thumb_func
Func_080de8d0:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r3, #192
	adds r7, r0, #0
	lsls r3, r3, #18
	movs r2, #100
	adds r2, r2, r7
	adds r3, #224
	ldr r6, [r3]
	movs r3, #0
	ldrsh r0, [r2, r3]
	mov r8, r2
	movs r2, #1
	negs r2, r2
	sub sp, #12
	cmp r0, r2
	beq .L_080de940
	lsls r0, r0, #10
	bl Trig_Sin
	movs r5, #192
	lsls r5, r5, #11
	adds r1, r0, #0
	ldr r3, .L_080de9dc
	adds r0, r5, #0
	mov lr, r3
	.2byte 0xf800
	ldr r3, [r6, #4]
	movs r2, #128
	adds r3, r3, r0
	str r3, [r7, #8]
	lsls r2, r2, #13
	ldr r3, [r6, #8]
	adds r3, r3, r2
	str r3, [r7, #12]
	mov r2, r8
	ldr r3, [r6, #12]
	str r3, [r7, #16]
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	lsls r3, r3, #16
	asrs r1, r3, #16
	adds r2, r1, #0
	adds r2, #64
	adds r3, r2, #0
	cmp r2, #0
	bge .L_080de936
	adds r3, r1, #0
	adds r3, #127
.L_080de936:
	asrs r3, r3, #6
	lsls r3, r3, #6
	subs r3, r2, r3
	mov r2, r8
	strh r3, [r2]
.L_080de940:
	ldr r3, .L_080de9e0
	movs r1, #3
	ldr r0, [r3]
	bl __umodsi3
	cmp r0, #0
	bne .L_080de9d4
	ldr r3, [r7, #8]
	mov r6, sp
	str r3, [r6]
	ldr r3, [r7, #12]
	movs r2, #128
	lsls r2, r2, #10
	adds r3, r3, r2
	str r3, [r6, #4]
	ldr r3, [r7, #16]
	str r3, [r6, #8]
	bl Random16
	lsls r5, r0, #1
	adds r5, r5, r0
	bl Random16
	lsls r5, r5, #1
	adds r1, r0, #0
	adds r2, r6, #0
	adds r0, r5, #0
	bl Vector_AddPolarOffset
	movs r0, #209
	lsls r0, r0, #1
	adds r0, #255
	ldr r1, [r6]
	ldr r2, [r6, #4]
	ldr r3, [r6, #8]
	bl Object_Spawn
	adds r5, r0, #0
	cmp r5, #0
	beq .L_080de9d4
	ldr r3, .L_080de9e4
	adds r2, r5, #0
	str r3, [r5, #108]
	movs r3, #153
	lsls r3, r3, #8
	adds r3, #153
	adds r2, #85
	str r3, [r5, #28]
	str r3, [r5, #24]
	movs r3, #2
	strb r3, [r2]
	movs r3, #229
	lsls r3, r3, #1
	str r3, [r5, #72]
	bl Random16
	adds r3, r5, #0
	lsrs r0, r0, #9
	adds r3, #100
	strh r0, [r3]
	movs r1, #9
	ldr r3, [r5, #8]
	adds r0, r5, #0
	str r3, [r5, #56]
	bl Animation_ApplyChildValuesFar
	adds r2, r5, #0
	adds r2, #94
	movs r3, #72
	strh r3, [r2]
	ldr r1, .L_080de9e8
	adds r0, r5, #0
	bl Object_SetCallback
.L_080de9d4:
	add sp, #12
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_080de9dc:
	.4byte IwramMulQ16
.L_080de9e0:
	.4byte Data_0300122c
.L_080de9e4:
	.4byte Func_080de818
.L_080de9e8:
	.4byte BattleFx_CommonParticleScript
