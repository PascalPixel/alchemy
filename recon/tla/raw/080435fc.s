.syntax unified
	.thumb
	.global Func_080435fc
	.thumb_func
Func_080435fc:
	push {r5, r6, lr}
	adds r5, r0, #0
	movs r6, #0
	cmp r5, #2
	bls .L_08043608
	movs r5, #2
.L_08043608:
	bl Func_0801596c
	cmp r0, #0
	beq .L_08043618
	ldr r0, .L_08043678
	movs r1, #1
	movs r6, #9
	b .L_0804365e
.L_08043618:
	bl Func_08016054
	ldr r3, .L_0804367c
	movs r0, #85
	strh r5, [r3]
	bl Audio_PlayCue
	movs r1, #13
	ldr r0, .L_08043680
	bl UiText_ShowPositionedMessageAndWait
	bl Func_08043358
	bl Func_080c8628
	adds r0, r5, #0
	ldr r1, .L_08043684
	bl Func_08042f10
	adds r5, r0, #0
	b .L_08043648
.L_08043642:
	movs r0, #1
	bl WaitFrames
.L_08043648:
	bl UiWork_IsComplete
	cmp r0, #0
	beq .L_08043642
	bl Func_0803ce1c
	cmp r5, #0
	beq .L_08043666
	ldr r0, .L_08043688
	movs r1, #1
	movs r6, #3
.L_0804365e:
	bl UiText_ShowPositionedMessageAndWait
	negs r6, r6
	b .L_0804366e
.L_08043666:
	ldr r0, .L_0804368c
	movs r1, #9
	bl UiText_ShowPositionedMessageAndWait
.L_0804366e:
	bl Func_0801613c
	adds r0, r6, #0
	pop {r5, r6, pc}
	.2byte 0x0000
.L_08043678:
	.4byte 0x0000000b
.L_0804367c:
	.4byte Data_020036d0
.L_08043680:
	.4byte 0x0000001d
.L_08043684:
	.4byte Data_02000000
.L_08043688:
	.4byte 0x0000000c
.L_0804368c:
	.4byte 0x00000019
