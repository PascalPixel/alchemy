.syntax unified
	.thumb
	.global Func_080cf350
	.thumb_func
Func_080cf350:
	push {r5, r6, lr}
	adds r6, r0, #0
	cmp r6, #0
	beq .L_080cf3ac
	bl EventRuntime_GetControlledOwner
	bl ObjectTable_Get
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r6, #52]
	movs r3, #128
	lsls r3, r3, #10
	adds r2, r6, #0
	str r3, [r6, #48]
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	adds r5, r0, #0
	ldr r2, [r5, #12]
	movs r3, #144
	lsls r3, r3, #14
	adds r2, r2, r3
	ldr r1, [r5, #8]
	ldr r3, [r5, #16]
	adds r0, r6, #0
	bl Object_SetPosition
	movs r1, #1
	adds r0, r6, #0
	bl Func_08020290
	movs r0, #3
	bl WaitFrames
	adds r0, r5, #0
	movs r1, #28
	bl Object_SetMode
	ldr r1, .L_080cf3b0
	adds r0, r6, #0
	bl ObjectDispatch_InitializeFar
	movs r3, #128
	lsls r3, r3, #7
	strh r3, [r5, #6]
.L_080cf3ac:
	pop {r5, r6, pc}
	.2byte 0x0000
.L_080cf3b0:
	.4byte Data_080f0074
