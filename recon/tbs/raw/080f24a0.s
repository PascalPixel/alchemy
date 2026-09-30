.syntax unified
	.thumb
	.global Unnamed_080f24a0
	.thumb_func
Unnamed_080f24a0:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r3, .L_080f24e4
	ldr r6, .L_080f24e0
	ldr r7, [r3]
	movs r3, #128
	lsls r3, r3, #19
	strh r6, [r3]
	ldr r0, .L_080f24e8
	bl Resource_GetTableEntry
	ldr r3, .L_080f24ec
	adds r4, r0, #0
	ldr r1, .L_080f24f0
	ldr r2, .L_080f24f4
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r3, .L_080f24f0
	strh r6, [r3]
	movs r3, #128
	lsls r3, r3, #2
	mov r8, r3
	ldr r5, .L_080f24f8
	add r4, r8
	adds r1, r5, #0
	adds r0, r4, #0
	bl Resource_DecodeByteLz
	ldr r3, .L_080f24ec
	b .L_080f24fc
	.2byte 0x0000
.L_080f24e0:
	.4byte 0x00000000
.L_080f24e4:
	.4byte Data_03001efc
.L_080f24e8:
	.4byte 0x00000015
.L_080f24ec:
	.4byte 0x040000d4
.L_080f24f0:
	.4byte 0x05000200
.L_080f24f4:
	.4byte 0x84000080
.L_080f24f8:
	.4byte gMapCellBuffer
.L_080f24fc:
	adds r0, r5, #0
	ldr r1, .L_080f2634
	ldr r2, .L_080f2638
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r0, .L_080f263c
	bl Resource_GetTableEntry
	movs r1, #160
	adds r4, r0, #0
	ldr r3, .L_080f2640
	lsls r1, r1, #19
	ldr r2, .L_080f2644
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r3, #160
	lsls r3, r3, #19
	add r4, r8
	strh r6, [r3]
	adds r1, r5, #0
	adds r0, r4, #0
	bl Resource_DecodeByteLz
	movs r1, #192
	ldr r3, .L_080f2640
	ldr r0, .L_080f2648
	lsls r1, r1, #19
	ldr r2, .L_080f264c
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r0, .L_080f2650
	ldr r1, .L_080f2654
	ldr r2, .L_080f2658
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r5, .L_080f265c
	ldr r1, .L_080f2660
	ldr r3, .L_080f2664
	movs r4, #0
.L_080f254a:
	movs r0, #29
.L_080f254c:
	adds r2, r3, #0
	movs r6, #128
	lsls r3, r2, #16
	lsls r6, r6, #9
	adds r3, r3, r6
	subs r0, #1
	strh r2, [r1]
	asrs r3, r3, #16
	adds r1, #2
	cmp r0, #0
	bge .L_080f254c
	strh r5, [r1]
	adds r4, #1
	adds r1, #2
	strh r5, [r1]
	adds r1, #2
	cmp r4, #10
	ble .L_080f254a
	ldr r3, .L_080f2668
	movs r4, #11
.L_080f2574:
	movs r0, #29
.L_080f2576:
	adds r2, r3, #0
	movs r6, #128
	lsls r3, r2, #16
	lsls r6, r6, #9
	adds r3, r3, r6
	subs r0, #1
	strh r2, [r1]
	asrs r3, r3, #16
	adds r1, #2
	cmp r0, #0
	bge .L_080f2576
	strh r5, [r1]
	adds r4, #1
	adds r1, #2
	strh r5, [r1]
	adds r1, #2
	cmp r4, #31
	ble .L_080f2574
	movs r3, #150
	ldr r1, .L_080f266c
	lsls r3, r3, #1
	movs r4, #0
.L_080f25a2:
	movs r0, #29
.L_080f25a4:
	adds r2, r3, #0
	movs r6, #128
	lsls r3, r2, #16
	lsls r6, r6, #9
	adds r3, r3, r6
	subs r0, #1
	strh r2, [r1]
	asrs r3, r3, #16
	adds r1, #2
	cmp r0, #0
	bge .L_080f25a4
	strh r5, [r1]
	adds r4, #1
	adds r1, #2
	strh r5, [r1]
	adds r1, #2
	cmp r4, #10
	ble .L_080f25a2
	movs r3, #0
	movs r4, #11
.L_080f25cc:
	movs r0, #29
.L_080f25ce:
	adds r2, r3, #0
	movs r6, #128
	lsls r3, r2, #16
	lsls r6, r6, #9
	adds r3, r3, r6
	subs r0, #1
	strh r2, [r1]
	asrs r3, r3, #16
	adds r1, #2
	cmp r0, #0
	bge .L_080f25ce
	strh r5, [r1]
	adds r4, #1
	adds r1, #2
	strh r5, [r1]
	adds r1, #2
	cmp r4, #31
	ble .L_080f25cc
	ldr r2, .L_080f2670
	ldr r3, .L_080f2620
	strh r3, [r2]
	ldr r3, .L_080f2624
	adds r2, #2
	strh r3, [r2]
	ldr r1, .L_080f2628
	ldr r3, .L_080f2674
	ldr r2, .L_080f262c
	strh r1, [r3]
	adds r3, #4
	strh r2, [r3]
	subs r3, #2
	strh r1, [r3]
	adds r3, #4
	strh r2, [r3]
	ldr r2, .L_080f2678
	ldr r3, .L_080f2630
	strh r3, [r2]
	ldr r3, .L_080f267c
	movs r2, #0
	movs r4, #3
	b .L_080f2680
.L_080f2620:
	.4byte 0x00001f43
.L_080f2624:
	.4byte 0x00001e81
.L_080f2628:
	.4byte 0x000000f0
.L_080f262c:
	.4byte 0x0000009f
.L_080f2630:
	.4byte 0x00001616
.L_080f2634:
	.4byte 0x06010000
.L_080f2638:
	.4byte 0x80000f00
.L_080f263c:
	.4byte 0x00000017
.L_080f2640:
	.4byte 0x040000d4
.L_080f2644:
	.4byte 0x84000080
.L_080f2648:
	.4byte Data_02012940
.L_080f264c:
	.4byte 0x80002760
.L_080f2650:
	.4byte Data_0201a140
.L_080f2654:
	.4byte 0x06004ec0
.L_080f2658:
	.4byte 0x80004ec0
.L_080f265c:
	.4byte 0x000001ff
.L_080f2660:
	.4byte 0x0600f000
.L_080f2664:
	.4byte 0x00000267
.L_080f2668:
	.4byte 0x0000013b
.L_080f266c:
	.4byte 0x0600f800
.L_080f2670:
	.4byte 0x0400000a
.L_080f2674:
	.4byte 0x04000040
.L_080f2678:
	.4byte 0x04000048
.L_080f267c:
	.4byte gBgScroll
.L_080f2680:
	subs r4, #1
	strh r2, [r3, #2]
	strh r2, [r3]
	adds r3, #4
	cmp r4, #0
	bge .L_080f2680
	ldr r0, .L_080f26cc
	movs r3, #0
	movs r2, #96
	strh r2, [r0, #6]
	strh r2, [r0, #10]
	str r3, [r7, #8]
	str r3, [r7]
	str r3, [r7, #4]
	str r3, [r7, #12]
	str r3, [r7, #20]
	str r3, [r7, #16]
	ldr r1, .L_080f26d0
	ldr r3, .L_080f26d4
	ldr r2, .L_080f26d8
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r3, .L_080f26c0
	adds r1, #64
	strh r3, [r1]
	ldr r2, .L_080f26dc
	ldr r3, .L_080f26c4
	strh r3, [r2]
	ldr r3, .L_080f26c8
	strh r3, [r1]
	b .L_080f26e0
	.2byte 0x0000
.L_080f26c0:
	.4byte 0x00003fbf
.L_080f26c4:
	.4byte 0x00001010
.L_080f26c8:
	.4byte 0x00003f44
.L_080f26cc:
	.4byte gBgScroll
.L_080f26d0:
	.4byte 0x04000010
.L_080f26d4:
	.4byte 0x040000d4
.L_080f26d8:
	.4byte 0x84000004
.L_080f26dc:
	.4byte 0x04000052
.L_080f26e0:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
