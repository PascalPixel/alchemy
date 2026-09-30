.syntax unified
	.thumb
	.global Func_080438c8
	.thumb_func
Func_080438c8:
	push {r5, r6, r7, lr}
	movs r7, #0
	bl Func_0801596c
	cmp r0, #0
	beq .L_080438e0
	ldr r0, .L_08043968
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWait
	subs r7, #9
	b .L_08043960
.L_080438e0:
	bl Func_08016054
	movs r0, #0
	movs r1, #3
	bl Func_08043cd8
	movs r3, #1
	adds r5, r0, #0
	negs r3, r3
	cmp r5, r3
	bne .L_080438fa
	adds r7, r5, #0
	b .L_08043960
.L_080438fa:
	movs r1, #8
	movs r2, #1
	movs r3, #2
	ldr r0, .L_0804396c
	bl UiText_OpenMessageWindow
	b .L_0804390e
.L_08043908:
	movs r0, #1
	bl WaitFrames
.L_0804390e:
	bl UiWork_IsComplete
	cmp r0, #0
	beq .L_08043908
	movs r0, #1
	movs r1, #0
	movs r2, #3
	movs r3, #1
	bl Menu_RunConfirmSelection
	cmp r0, #0
	beq .L_0804392c
	bl Func_0803ce1c
	b .L_08043960
.L_0804392c:
	bl Func_0803ce1c
	movs r0, #1
	bl Func_08042e28
	adds r6, r0, #0
	adds r0, r5, #0
	bl Func_08042ff0
	adds r5, r0, #0
	adds r0, r6, #0
	bl Func_08042eec
	cmp r5, #0
	beq .L_08043958
	ldr r0, .L_08043970
	movs r1, #1
	movs r7, #4
	bl UiText_ShowPositionedMessageAndWait
	negs r7, r7
	b .L_08043960
.L_08043958:
	ldr r0, .L_08043974
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWait
.L_08043960:
	bl Func_0801613c
	adds r0, r7, #0
	pop {r5, r6, r7, pc}
.L_08043968:
	.4byte 0x0000000b
.L_0804396c:
	.4byte 0x00000017
.L_08043970:
	.4byte 0x0000000e
.L_08043974:
	.4byte 0x0000001a
