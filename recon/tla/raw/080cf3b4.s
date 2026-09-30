.syntax unified
	.thumb
	.global Func_080cf3b4
	.thumb_func
Func_080cf3b4:
	push {r5, r6, r7, lr}
	adds r5, r0, #0
	adds r6, r1, #0
	cmp r5, #0
	beq .L_080cf41a
	bl Func_080cdf5c
	bl ObjectTable_Get
	movs r3, #1
	ands r3, r6
	adds r7, r0, #0
	cmp r3, #0
	beq .L_080cf3f0
	adds r0, r5, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26Far
	ldr r1, .L_080cf41c
	adds r0, r5, #0
	bl Object_SetCallback
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r5, #40]
	movs r3, #128
	lsls r3, r3, #7
	str r3, [r5, #72]
	ldr r3, .L_080cf420
	str r3, [r5, #108]
.L_080cf3f0:
	cmp r6, #3
	bne .L_080cf3fa
	movs r0, #60
	bl WaitFrames
.L_080cf3fa:
	movs r3, #2
	ands r3, r6
	cmp r3, #0
	beq .L_080cf408
	adds r0, r5, #0
	bl Func_080cf350
.L_080cf408:
	cmp r6, #3
	bne .L_080cf412
	movs r0, #80
	bl WaitFrames
.L_080cf412:
	adds r0, r7, #0
	movs r1, #1
	bl Object_SetMode
.L_080cf41a:
	pop {r5, r6, r7, pc}
.L_080cf41c:
	.4byte Data_080effd8
.L_080cf420:
	.4byte Func_080cf0d0
