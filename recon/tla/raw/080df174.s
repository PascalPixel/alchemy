.syntax unified
	.thumb
	.global Func_080df174
	.thumb_func
Func_080df174:
	push {r5, r6, lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #224
	ldr r3, [r3]
	sub sp, #12
	ldr r5, [r3, #20]
	mov r6, sp
	ldr r3, [r5, #8]
	str r3, [r6]
	bl Random16
	ldr r3, [r5, #12]
	lsls r0, r0, #4
	movs r2, #192
	lsls r2, r2, #13
	subs r3, r3, r0
	adds r3, r3, r2
	str r3, [r6, #4]
	ldr r3, [r5, #16]
	str r3, [r6, #8]
	bl Random16
	lsls r5, r0, #1
	adds r5, r5, r0
	bl Random16
	lsls r5, r5, #4
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
	beq .L_080df1f2
	adds r2, r5, #0
	adds r2, #85
	movs r3, #2
	strb r3, [r2]
	movs r3, #200
	lsls r3, r3, #5
	adds r3, #153
	str r3, [r5, #72]
	movs r1, #0
	bl Object_SetMode
	adds r2, r5, #0
	adds r2, #94
	movs r3, #12
	strh r3, [r2]
	ldr r1, .L_080df1f8
	adds r0, r5, #0
	bl ObjectDispatch_InitializeFar
.L_080df1f2:
	add sp, #12
	pop {r5, r6, pc}
	.2byte 0x0000
.L_080df1f8:
	.4byte BattleFx_CommonParticleScript
