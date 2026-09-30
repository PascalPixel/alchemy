.syntax unified
	.thumb
	.global Func_080dcf8c
	.thumb_func
Func_080dcf8c:
	push {r5, r6, lr}
	adds r5, r0, #0
	ldr r0, [r5, #104]
	sub sp, #12
	cmp r0, #0
	beq .L_080dcfd6
	ldr r2, [r0, #8]
	ldr r3, [r5, #8]
	subs r1, r2, r3
	ldr r2, [r0, #16]
	ldr r3, [r5, #16]
	subs r0, r2, r3
	cmp r1, #0
	bne .L_080dcfac
	cmp r0, #0
	beq .L_080dcfce
.L_080dcfac:
	bl ArcTan2
	ldrh r3, [r5, #6]
	movs r2, #128
	subs r0, r0, r3
	lsls r0, r0, #16
	asrs r0, r0, #16
	lsls r2, r2, #5
	cmp r0, r2
	ble .L_080dcfc2
	adds r0, r2, #0
.L_080dcfc2:
	ldr r2, .L_080dd048
	cmp r0, r2
	bge .L_080dcfca
	adds r0, r2, #0
.L_080dcfca:
	adds r3, r3, r0
	strh r3, [r5, #6]
.L_080dcfce:
	adds r2, r5, #0
	adds r2, #90
	movs r3, #0
	strb r3, [r2]
.L_080dcfd6:
	ldr r3, [r5, #8]
	mov r6, sp
	str r3, [r6]
	bl Random16
	ldr r3, [r5, #12]
	ldr r2, .L_080dd04c
	lsls r0, r0, #4
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
	beq .L_080dd044
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
	ldr r1, .L_080dd050
	adds r0, r5, #0
	bl Object_SetCallback
.L_080dd044:
	add sp, #12
	pop {r5, r6, pc}
.L_080dd048:
	.4byte 0xfffff000
.L_080dd04c:
	.4byte 0xfff80000
.L_080dd050:
	.4byte BattleFx_CommonParticleScript
