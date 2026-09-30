.syntax unified
	.thumb
	.global Func_080cef84
	.thumb_func
Func_080cef84:
	push {r5, lr}
	bl ObjectTable_Get
	adds r5, r0, #0
	movs r0, #18
	bl WaitFrames
	adds r0, r5, #0
	movs r1, #7
	bl Object_SetMode
	movs r0, #146
	bl Audio_PlayCue
	cmp r5, #0
	beq .L_080cefb2
	movs r3, #128
	lsls r3, r3, #12
	str r3, [r5, #40]
	adds r0, r5, #0
	movs r1, #1
	bl ObjectDispatch_SetSingleChildField26Far
.L_080cefb2:
	pop {r5, pc}
