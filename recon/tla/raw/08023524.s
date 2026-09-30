.syntax unified
	.thumb
	.global Func_08023524
	.thumb_func
Func_08023524:
	push {r5, lr}
	ldr r3, .L_080235b8
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #18
	adds r3, r3, r2
	ldrb r3, [r3]
	adds r5, r0, #0
	cmp r3, #12
	bhi .L_080235ae
	ldr r2, .L_080235bc
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
.L_08023540:
	.4byte .L_08023574
	.4byte .L_08023578
	.4byte .L_0802357c
	.4byte .L_08023580
	.4byte .L_08023584
	.4byte .L_08023588
	.4byte .L_0802358c
	.4byte .L_08023590
	.4byte .L_08023594
	.4byte .L_08023598
	.4byte .L_080235a2
	.4byte .L_080235a6
	.4byte .L_080235aa
.L_08023574:
	ldr r1, .L_080235c0
	b .L_080235b0
.L_08023578:
	ldr r1, .L_080235c4
	b .L_080235b0
.L_0802357c:
	ldr r1, .L_080235c8
	b .L_080235b0
.L_08023580:
	ldr r1, .L_080235cc
	b .L_080235b0
.L_08023584:
	ldr r1, .L_080235d0
	b .L_080235b0
.L_08023588:
	ldr r1, .L_080235d4
	b .L_080235b0
.L_0802358c:
	ldr r1, .L_080235d8
	b .L_080235b0
.L_08023590:
	ldr r1, .L_080235dc
	b .L_080235b0
.L_08023594:
	ldr r1, .L_080235e0
	b .L_080235b0
.L_08023598:
	movs r0, #1
	bl WaitFrames
	ldr r1, .L_080235e4
	b .L_080235b0
.L_080235a2:
	ldr r1, .L_080235e8
	b .L_080235b0
.L_080235a6:
	ldr r1, .L_080235ec
	b .L_080235b0
.L_080235aa:
	ldr r1, .L_080235f0
	b .L_080235b0
.L_080235ae:
	ldr r1, .L_080235f4
.L_080235b0:
	adds r0, r5, #0
	bl ObjectDispatch_Initialize
	pop {r5, pc}
.L_080235b8:
	.4byte gPartyState
.L_080235bc:
	.4byte .L_08023540
.L_080235c0:
	.4byte Data_0802f140
.L_080235c4:
	.4byte Data_0802f158
.L_080235c8:
	.4byte Data_0802f0b0
.L_080235cc:
	.4byte Data_0802f0c8
.L_080235d0:
	.4byte Data_0802f0e0
.L_080235d4:
	.4byte Data_0802f0f8
.L_080235d8:
	.4byte Data_0802f170
.L_080235dc:
	.4byte Data_0802f188
.L_080235e0:
	.4byte Data_0802f1a0
.L_080235e4:
	.4byte Data_0802f1b8
.L_080235e8:
	.4byte Data_0802f110
.L_080235ec:
	.4byte Data_0802f128
.L_080235f0:
	.4byte ObjectDispatch_Table6
.L_080235f4:
	.4byte Data_0802f098
