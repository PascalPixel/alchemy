.syntax unified
	.thumb
	.global Func_080ca514
	.thumb_func
Func_080ca514:
	push {lr}
	subs r0, #1
	cmp r0, #11
	bhi .L_080ca56c
	ldr r2, .L_080ca57c
	lsls r3, r0, #2
	ldr r3, [r3, r2]
	mov pc, r3
.L_080ca524:
	.4byte .L_080ca554
	.4byte .L_080ca558
	.4byte .L_080ca55c
	.4byte .L_080ca560
	.4byte .L_080ca564
	.4byte .L_080ca564
	.4byte .L_080ca560
	.4byte .L_080ca56c
	.4byte .L_080ca56c
	.4byte .L_080ca56c
	.4byte .L_080ca568
	.4byte .L_080ca568
.L_080ca554:
	ldr r2, .L_080ca580
	b .L_080ca56e
.L_080ca558:
	ldr r2, .L_080ca584
	b .L_080ca56e
.L_080ca55c:
	ldr r2, .L_080ca588
	b .L_080ca56e
.L_080ca560:
	ldr r2, .L_080ca58c
	b .L_080ca56e
.L_080ca564:
	ldr r2, .L_080ca590
	b .L_080ca56e
.L_080ca568:
	ldr r2, .L_080ca594
	b .L_080ca56e
.L_080ca56c:
	ldr r2, .L_080ca598
.L_080ca56e:
	ldr r3, .L_080ca59c
	movs r1, #251
	lsls r1, r1, #1
	adds r3, r3, r1
	strh r2, [r3]
	pop {pc}
	.2byte 0x0000
.L_080ca57c:
	.4byte .L_080ca524
.L_080ca580:
	.4byte 0x00000046
.L_080ca584:
	.4byte 0x00000048
.L_080ca588:
	.4byte 0x00000073
.L_080ca58c:
	.4byte 0x00000047
.L_080ca590:
	.4byte 0x0000006d
.L_080ca594:
	.4byte 0x0000006e
.L_080ca598:
	.4byte 0x00000053
.L_080ca59c:
	.4byte gPartyState
