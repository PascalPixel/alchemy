.syntax unified
.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.global Overlay_02000000
Overlay_02000000:
	.include "games/THE BROKEN SEAL/SRC/FIELD/SORU_FUNKA/ENTRY.INC"
	.global Func_02000030
	.thumb_func
Func_02000030:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x0200b6d4
	.global Func_02000038
	.thumb_func
Func_02000038:
	movs r0, #0
	bx lr
	.global Func_0200003c
	.thumb_func
Func_0200003c:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x0200b704
	.global Func_02000044
	.thumb_func
Func_02000044:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x0200b710
	.global Func_0200004c
	.thumb_func
Func_0200004c:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x0200b998
	.global Func_02000054
	.thumb_func
Func_02000054:
	push {r5, r6, r7, lr}
	ldr r5, [pc, #884]
	ldr r7, [r5]
	bl 0x0200b4d4
	ldr r2, [pc, #880]
	movs r3, #0
	adds r6, r7, r2
	str r3, [r6]
	movs r0, #141
	bl 0x0200b624
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r0, r0, #9
	lsls r1, r1, #9
	lsls r2, r2, #9
	bl 0x0200b494
	movs r1, #232
	movs r2, #156
	movs r0, #0
	lsls r1, r1, #16
	lsls r2, r2, #16
	bl 0x0200b534
	movs r1, #218
	movs r2, #172
	movs r0, #1
	lsls r1, r1, #16
	lsls r2, r2, #16
	bl 0x0200b534
	movs r1, #176
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200b59c
	movs r1, #208
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200b59c
	movs r2, #166
	movs r0, #5
	ldr r1, [pc, #796]
	lsls r2, r2, #17
	bl 0x0200b534
	movs r2, #166
	movs r0, #9
	ldr r1, [pc, #788]
	lsls r2, r2, #17
	bl 0x0200b534
	movs r2, #174
	movs r0, #11
	ldr r1, [pc, #780]
	lsls r2, r2, #17
	bl 0x0200b534
	movs r2, #174
	movs r0, #10
	ldr r1, [pc, #772]
	lsls r2, r2, #17
	bl 0x0200b534
	movs r2, #153
	movs r0, #13
	ldr r1, [pc, #764]
	lsls r2, r2, #17
	bl 0x0200b534
	movs r2, #181
	lsls r2, r2, #17
	movs r0, #14
	ldr r1, [pc, #756]
	bl 0x0200b534
	movs r0, #15
	movs r1, #1
	bl 0x0200b5a4
	movs r0, #232
	movs r1, #1
	movs r2, #156
	lsls r0, r0, #16
	negs r1, r1
	lsls r2, r2, #16
	movs r3, #0
	subs r5, #8
	bl 0x0200b5c4
	bl 0x0200b464
	ldr r3, [r5]
	movs r2, #228
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #8
	str r2, [r3]
	bl 0x0200b5fc
	bl 0x0200b604
	movs r1, #0
	ldr r0, [pc, #700]
	bl 0x0200b5ec
	movs r0, #4
	bl 0x0200b5f4
	movs r0, #4
	bl 0x0200b4cc
	movs r0, #128
	movs r1, #0
	lsls r0, r0, #9
	bl 0x0200b5ec
	movs r0, #4
	bl 0x0200b5f4
	movs r0, #4
	bl 0x0200b4cc
	bl 0x02008f64
	movs r1, #0
	ldr r0, [pc, #652]
	bl 0x0200b5ec
	movs r0, #4
	bl 0x0200b5f4
	movs r0, #16
	bl 0x0200b4cc
	movs r0, #144
	bl 0x0200b624
	movs r0, #128
	movs r1, #0
	lsls r0, r0, #9
	bl 0x0200b5ec
	movs r0, #4
	bl 0x0200b5f4
	movs r0, #4
	bl 0x0200b4cc
	movs r1, #0
	ldr r0, [pc, #604]
	bl 0x0200b5ec
	movs r0, #4
	bl 0x0200b5f4
	movs r0, #4
	bl 0x0200b4cc
	movs r0, #144
	bl 0x0200b624
	movs r0, #128
	movs r1, #0
	lsls r0, r0, #9
	bl 0x0200b5ec
	movs r0, #48
	bl 0x0200b5f4
	movs r0, #48
	bl 0x0200b4cc
	movs r0, #0
	movs r1, #6
	movs r2, #0
	bl 0x0200b54c
	movs r0, #1
	movs r1, #6
	movs r2, #20
	bl 0x0200b54c
	movs r0, #1
	movs r1, #20
	movs r2, #20
	bl 0x0200ad48
	movs r1, #20
	movs r2, #40
	movs r0, #0
	bl 0x0200ad48
	ldr r0, [pc, #524]
	bl 0x0200b57c
	movs r2, #20
	movs r0, #11
	movs r1, #0
	bl 0x0200b594
	movs r0, #10
	movs r1, #0
	bl 0x0200b58c
	movs r0, #1
	movs r1, #20
	movs r2, #0
	bl 0x0200ad48
	movs r0, #5
	movs r1, #0
	movs r2, #20
	bl 0x0200b594
	movs r0, #0
	movs r1, #20
	movs r2, #0
	bl 0x0200ad48
	movs r0, #14
	movs r1, #0
	movs r2, #20
	bl 0x0200b594
	movs r0, #9
	movs r1, #0
	movs r2, #20
	bl 0x0200b594
	movs r2, #0
	movs r1, #1
	movs r0, #0
	bl 0x0200b564
	movs r0, #40
	bl 0x0200b4cc
	movs r0, #0
	movs r1, #2
	bl 0x0200b554
	movs r1, #2
	movs r0, #1
	bl 0x0200b55c
	movs r0, #20
	bl 0x0200b4cc
	movs r1, #176
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200b59c
	movs r1, #208
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #30
	bl 0x0200b59c
	movs r2, #0
	movs r0, #1
	movs r1, #20
	bl 0x0200ad48
	movs r0, #0
	movs r1, #2
	bl 0x0200b554
	movs r1, #2
	movs r0, #1
	bl 0x0200b55c
	movs r0, #20
	bl 0x0200b4cc
	movs r2, #20
	movs r1, #20
	movs r0, #0
	bl 0x0200ad48
	movs r0, #15
	bl 0x0200b4ec
	movs r1, #0
	bl 0x0200b484
	movs r2, #151
	ldr r1, [pc, #336]
	lsls r2, r2, #17
	movs r0, #15
	bl 0x0200b534
	movs r0, #15
	bl 0x0200b4ec
	adds r2, r0, #0
	adds r2, #85
	movs r3, #5
	strb r3, [r2]
	movs r3, #1
	str r3, [r6]
	ldr r0, [pc, #312]
	bl 0x0200b624
	movs r0, #1
	movs r1, #1
	negs r1, r1
	ldr r2, [pc, #304]
	negs r0, r0
	bl 0x0200b494
	bl 0x0200b49c
	movs r0, #150
	bl 0x0200b4cc
	movs r2, #20
	movs r0, #11
	movs r1, #0
	bl 0x0200b594
	movs r0, #5
	movs r1, #0
	bl 0x0200b58c
	movs r0, #10
	movs r1, #0
	movs r2, #10
	bl 0x0200b594
	movs r0, #0
	movs r1, #1
	movs r2, #20
	bl 0x0200b564
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #6
	movs r2, #0
	bl 0x0200b59c
	movs r1, #192
	movs r0, #1
	lsls r1, r1, #6
	movs r2, #10
	bl 0x0200b59c
	movs r2, #166
	movs r0, #5
	ldr r1, [pc, #180]
	lsls r2, r2, #17
	bl 0x0200b534
	movs r2, #166
	movs r0, #9
	ldr r1, [pc, #172]
	lsls r2, r2, #17
	bl 0x0200b534
	movs r2, #174
	movs r0, #11
	ldr r1, [pc, #164]
	lsls r2, r2, #17
	bl 0x0200b534
	movs r2, #174
	movs r0, #10
	ldr r1, [pc, #156]
	lsls r2, r2, #17
	bl 0x0200b534
	movs r2, #153
	movs r0, #13
	ldr r1, [pc, #148]
	lsls r2, r2, #17
	bl 0x0200b534
	movs r2, #181
	lsls r2, r2, #17
	movs r0, #14
	ldr r1, [pc, #136]
	bl 0x0200b534
	ldr r0, [pc, #156]
	ldr r1, [pc, #156]
	bl 0x0200b5bc
	movs r0, #164
	movs r1, #1
	ldr r2, [pc, #152]
	movs r3, #1
	negs r1, r1
	lsls r0, r0, #17
	bl 0x0200b5c4
	bl 0x0200b5cc
	movs r0, #167
	bl 0x0200b624
	movs r1, #2
	ldr r0, [pc, #132]
	bl 0x0200b5ec
	movs r0, #20
	bl 0x0200b5f4
	movs r0, #20
	bl 0x0200b3e4
	movs r0, #128
	movs r1, #2
	lsls r0, r0, #9
	bl 0x0200b5ec
	movs r0, #20
	bl 0x0200b5f4
	movs r0, #200
	bl 0x0200b4cc
	movs r1, #0
	ldr r0, [pc, #92]
	bl 0x0200b584
	movs r0, #0
	movs r1, #0
	bl 0x0200b4e4
	cmp r0, #0
	bne .L_02000054_0
	ldr r0, [pc, #80]
	bl 0x0200b57c
	b .L_02000054_1
	.2byte 0x0000
	.4byte 0x03001ec4
	.4byte 0x0000040c
	.4byte 0x01db0000
	.4byte 0x01eb0000
	.4byte 0x01cb0000
	.4byte 0x01fb0000
	.4byte 0x01d70000
	.4byte 0x01df0000
	.4byte 0x00007fff
	.4byte 0x000010cd
	.4byte 0x01450000
	.4byte 0x00000121
	.4byte 0x0000e666
	.4byte 0x00066666
	.4byte 0x0000cccc
	.4byte 0x012b0000
	.4byte 0x00205294
	.4byte 0x00001001
	.4byte 0x000010d6
.L_02000054_0:
	ldr r0, [pc, #1016]
	bl 0x0200b57c
.L_02000054_1:
	movs r1, #0
	movs r2, #80
	ldr r0, [pc, #1012]
	bl 0x0200b594
	ldr r0, [pc, #1008]
	bl 0x0200b57c
	movs r0, #9
	movs r1, #0
	movs r2, #20
	bl 0x0200b594
	movs r1, #20
	movs r2, #0
	movs r0, #1
	bl 0x0200ad48
	ldr r3, [pc, #988]
	adds r5, r7, r3
	movs r3, #0
	str r3, [r5]
	movs r0, #141
	bl 0x0200b624
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #10
	lsls r2, r2, #9
	lsls r0, r0, #9
	bl 0x0200b494
	movs r0, #80
	bl 0x0200b4cc
	movs r3, #1
	str r3, [r5]
	ldr r0, [pc, #952]
	bl 0x0200b624
	movs r0, #1
	movs r1, #1
	negs r0, r0
	negs r1, r1
	ldr r2, [pc, #940]
	bl 0x0200b494
	bl 0x0200b49c
	movs r0, #0
	movs r1, #20
	movs r2, #60
	bl 0x0200ad48
	movs r0, #14
	movs r1, #0
	movs r2, #30
	bl 0x0200b594
	movs r1, #160
	movs r0, #15
	lsls r1, r1, #8
	movs r2, #40
	bl 0x0200b59c
	movs r0, #1
	movs r1, #20
	movs r2, #20
	bl 0x0200ad48
	movs r0, #0
	movs r1, #20
	movs r2, #20
	bl 0x0200ad48
	ldr r0, [pc, #864]
	movs r1, #0
	movs r2, #30
	bl 0x0200b594
	movs r1, #128
	movs r0, #15
	lsls r1, r1, #5
	movs r2, #40
	bl 0x0200b59c
	movs r0, #1
	movs r1, #20
	movs r2, #20
	bl 0x0200ad48
	movs r0, #0
	movs r1, #20
	movs r2, #20
	bl 0x0200ad48
	movs r2, #30
	movs r0, #5
	movs r1, #0
	bl 0x0200b594
	bl 0x02009084
	movs r1, #200
	lsls r1, r1, #4
	ldr r0, [pc, #824]
	bl 0x0200b3ec
	movs r1, #200
	lsls r1, r1, #4
	ldr r0, [pc, #816]
	bl 0x0200b3ec
	movs r0, #240
	bl 0x0200b4cc
	movs r0, #10
	movs r1, #0
	movs r2, #30
	bl 0x0200b594
	movs r2, #166
	movs r0, #5
	ldr r1, [pc, #796]
	lsls r2, r2, #17
	bl 0x0200b534
	movs r2, #166
	movs r0, #9
	ldr r1, [pc, #788]
	lsls r2, r2, #17
	bl 0x0200b534
	movs r2, #174
	movs r0, #11
	ldr r1, [pc, #780]
	lsls r2, r2, #17
	bl 0x0200b534
	movs r2, #174
	movs r0, #10
	ldr r1, [pc, #772]
	lsls r2, r2, #17
	bl 0x0200b534
	movs r2, #153
	movs r0, #13
	ldr r1, [pc, #764]
	lsls r2, r2, #17
	bl 0x0200b534
	movs r2, #181
	movs r0, #14
	ldr r1, [pc, #756]
	lsls r2, r2, #17
	bl 0x0200b534
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200b59c
	movs r1, #128
	movs r0, #9
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200b59c
	movs r1, #128
	movs r0, #11
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200b59c
	movs r1, #128
	movs r0, #10
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200b59c
	movs r1, #128
	movs r0, #13
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200b59c
	movs r1, #128
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #14
	bl 0x0200b59c
	movs r0, #5
	bl 0x0200b4ec
	adds r2, r0, #0
	movs r3, #10
	ldrsh r0, [r2, r3]
	movs r3, #18
	ldrsh r2, [r2, r3]
	movs r1, #1
	movs r3, #1
	lsls r2, r2, #16
	negs r1, r1
	lsls r0, r0, #16
	bl 0x0200b5c4
	bl 0x0200b5cc
	movs r0, #40
	bl 0x0200b4cc
	movs r1, #2
	movs r0, #13
	bl 0x0200b55c
	movs r0, #20
	bl 0x0200b4cc
	movs r0, #13
	movs r1, #0
	bl 0x0200b58c
	movs r0, #11
	movs r1, #2
	bl 0x0200b55c
	movs r2, #10
	movs r0, #11
	movs r1, #0
	bl 0x0200b594
	movs r0, #14
	movs r1, #4
	bl 0x0200b53c
	movs r0, #14
	movs r1, #0
	movs r2, #20
	bl 0x0200b594
	movs r2, #40
	movs r0, #10
	movs r1, #11
	bl 0x0200b564
	movs r0, #10
	movs r1, #4
	bl 0x0200b53c
	movs r0, #10
	movs r1, #0
	movs r2, #10
	bl 0x0200b594
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200b5ac
	movs r1, #192
	movs r0, #5
	lsls r1, r1, #6
	movs r2, #40
	bl 0x0200b59c
	ldr r0, [pc, #528]
	movs r1, #0
	movs r2, #10
	bl 0x0200b594
	movs r1, #176
	movs r0, #10
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200b59c
	movs r1, #208
	movs r0, #11
	lsls r1, r1, #8
	movs r2, #40
	bl 0x0200b59c
	movs r0, #10
	movs r1, #11
	movs r2, #40
	bl 0x0200b564
	movs r0, #10
	ldr r1, [pc, #488]
	movs r2, #40
	bl 0x0200b5ac
	movs r1, #176
	movs r0, #10
	lsls r1, r1, #8
	movs r2, #10
	bl 0x0200b59c
	movs r0, #10
	movs r1, #0
	movs r2, #10
	bl 0x0200b594
	movs r1, #128
	movs r2, #10
	movs r0, #10
	lsls r1, r1, #8
	bl 0x0200b59c
	movs r0, #11
	movs r1, #2
	bl 0x0200b55c
	movs r2, #10
	movs r0, #11
	movs r1, #0
	bl 0x0200b594
	movs r0, #10
	movs r1, #3
	bl 0x0200b53c
	movs r0, #10
	movs r1, #0
	movs r2, #40
	bl 0x0200b594
	movs r1, #176
	movs r2, #60
	movs r0, #14
	lsls r1, r1, #8
	bl 0x0200b59c
	movs r0, #14
	movs r1, #3
	bl 0x0200b544
	movs r0, #14
	movs r1, #0
	movs r2, #10
	bl 0x0200b594
	movs r1, #192
	movs r0, #13
	lsls r1, r1, #6
	movs r2, #0
	bl 0x0200b59c
	movs r1, #192
	movs r0, #5
	lsls r1, r1, #6
	movs r2, #0
	bl 0x0200b59c
	movs r1, #160
	movs r0, #9
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200b59c
	movs r1, #192
	movs r0, #11
	lsls r1, r1, #6
	movs r2, #0
	bl 0x0200b59c
	movs r1, #160
	movs r0, #10
	lsls r1, r1, #7
	movs r2, #20
	bl 0x0200b59c
	movs r1, #208
	movs r2, #20
	movs r0, #14
	lsls r1, r1, #8
	bl 0x0200b59c
	movs r0, #14
	movs r1, #4
	bl 0x0200b544
	movs r0, #14
	movs r1, #0
	movs r2, #10
	bl 0x0200b594
	movs r0, #13
	ldr r1, [pc, #292]
	movs r2, #0
	bl 0x0200b5ac
	movs r1, #128
	movs r2, #128
	movs r0, #13
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200b4fc
	movs r2, #157
	lsls r2, r2, #1
	movs r0, #13
	ldr r1, [pc, #268]
	bl 0x0200b524
	movs r0, #13
	movs r1, #3
	bl 0x0200b554
	movs r0, #13
	movs r1, #0
	movs r2, #10
	bl 0x0200b594
	movs r1, #176
	movs r2, #10
	movs r0, #14
	lsls r1, r1, #8
	bl 0x0200b59c
	movs r0, #14
	movs r1, #4
	bl 0x0200b53c
	movs r2, #10
	movs r0, #14
	movs r1, #0
	bl 0x0200b594
	movs r0, #13
	movs r1, #3
	bl 0x0200b554
	movs r2, #10
	movs r0, #13
	movs r1, #0
	bl 0x0200b594
	movs r0, #9
	movs r1, #2
	bl 0x0200b55c
	ldr r0, [pc, #192]
	movs r1, #0
	movs r2, #10
	bl 0x0200b594
	movs r1, #176
	movs r0, #10
	lsls r1, r1, #8
	movs r2, #20
	bl 0x0200b59c
	movs r0, #10
	movs r1, #0
	movs r2, #30
	bl 0x0200b594
	movs r2, #20
	movs r0, #11
	movs r1, #0
	bl 0x0200b59c
	movs r1, #2
	movs r0, #11
	bl 0x0200b55c
	movs r0, #30
	bl 0x0200b4cc
	movs r1, #3
	movs r0, #11
	bl 0x0200b544
	movs r0, #30
	bl 0x0200b4cc
	movs r1, #208
	movs r2, #30
	movs r0, #11
	lsls r1, r1, #8
	bl 0x0200b59c
	movs r1, #4
	movs r0, #11
	bl 0x0200b544
	movs r0, #10
	bl 0x0200b4cc
	movs r0, #11
	movs r1, #0
	movs r2, #10
	bl 0x0200b594
	movs r1, #160
	movs r2, #20
	movs r0, #13
	lsls r1, r1, #7
	bl 0x0200b59c
	movs r1, #2
	b .L_02000054_2
	.4byte 0x000010d7
	.4byte 0x00001001
	.4byte 0x000010d8
	.4byte 0x0000040c
	.4byte 0x00000121
	.4byte 0x0000e666
	.4byte 0x020090c5
	.4byte 0x0200935d
	.4byte 0x01db0000
	.4byte 0x01eb0000
	.4byte 0x01cb0000
	.4byte 0x01fb0000
	.4byte 0x01d70000
	.4byte 0x01df0000
	.4byte 0x00004005
	.4byte 0x00000105
	.4byte 0x00000103
	.4byte 0x000001d7
	.4byte 0x00004009
.L_02000054_2:
	movs r0, #13
	bl 0x0200b55c
	movs r0, #20
	bl 0x0200b4cc
	movs r2, #0
	movs r1, #11
	movs r0, #10
	bl 0x0200b564
	movs r0, #30
	bl 0x0200b4cc
	movs r0, #10
	movs r1, #3
	bl 0x0200b53c
	movs r0, #11
	movs r1, #3
	bl 0x0200b544
	movs r1, #208
	movs r0, #11
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200b59c
	movs r1, #176
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #10
	bl 0x0200b59c
	movs r0, #40
	bl 0x0200b4cc
	movs r1, #1
	movs r0, #13
	bl 0x0200b55c
	movs r0, #30
	bl 0x0200b4cc
	movs r1, #128
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #13
	bl 0x0200b59c
	movs r0, #40
	bl 0x0200b4cc
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #13
	bl 0x0200b5ac
	movs r0, #60
	bl 0x0200b4cc
	movs r1, #176
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #14
	bl 0x0200b59c
	movs r0, #30
	bl 0x0200b4cc
	movs r0, #14
	movs r1, #4
	bl 0x0200b544
	ldr r0, [pc, #1012]
	movs r1, #0
	movs r2, #30
	bl 0x0200b594
	movs r1, #176
	movs r0, #9
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200b59c
	movs r1, #176
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200b59c
	movs r1, #192
	movs r0, #13
	lsls r1, r1, #6
	movs r2, #20
	bl 0x0200b59c
	movs r1, #129
	movs r0, #5
	lsls r1, r1, #1
	movs r2, #40
	bl 0x0200b5ac
	ldr r0, [pc, #956]
	movs r1, #0
	movs r2, #40
	bl 0x0200b594
	movs r2, #60
	movs r0, #13
	ldr r1, [pc, #948]
	bl 0x0200b5ac
	movs r0, #13
	movs r1, #3
	bl 0x0200b544
	movs r0, #10
	ldr r1, [pc, #936]
	movs r2, #0
	bl 0x0200b5ac
	movs r2, #60
	movs r0, #11
	ldr r1, [pc, #924]
	bl 0x0200b5ac
	movs r0, #14
	movs r1, #2
	bl 0x0200b55c
	movs r2, #60
	movs r0, #14
	movs r1, #0
	bl 0x0200b594
	movs r1, #3
	movs r0, #13
	bl 0x0200b544
	movs r0, #40
	bl 0x0200b4cc
	movs r1, #129
	movs r0, #5
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200b5ac
	movs r1, #129
	movs r0, #9
	lsls r1, r1, #1
	movs r2, #40
	bl 0x0200b5ac
	movs r2, #20
	movs r0, #10
	movs r1, #11
	bl 0x0200b564
	movs r0, #10
	movs r1, #3
	bl 0x0200b53c
	movs r1, #3
	movs r0, #11
	bl 0x0200b544
	movs r0, #20
	bl 0x0200b4cc
	movs r0, #10
	ldr r1, [pc, #832]
	ldr r2, [pc, #832]
	bl 0x0200b4fc
	movs r0, #11
	ldr r1, [pc, #820]
	ldr r2, [pc, #824]
	bl 0x0200b4fc
	movs r2, #174
	movs r0, #11
	ldr r1, [pc, #816]
	lsls r2, r2, #1
	bl 0x0200b51c
	movs r2, #174
	lsls r2, r2, #1
	ldr r1, [pc, #808]
	movs r0, #10
	bl 0x0200b51c
	movs r0, #11
	bl 0x0200b52c
	movs r0, #10
	bl 0x0200b52c
	movs r0, #11
	movs r1, #1
	bl 0x0200b53c
	movs r0, #10
	movs r1, #1
	bl 0x0200b53c
	movs r1, #208
	movs r0, #11
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200b59c
	movs r1, #176
	movs r0, #10
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200b59c
	movs r1, #192
	movs r0, #13
	lsls r1, r1, #6
	movs r2, #10
	bl 0x0200b59c
	movs r2, #0
	movs r0, #11
	ldr r1, [pc, #736]
	bl 0x0200b5ac
	movs r0, #11
	movs r1, #2
	bl 0x0200b55c
	ldr r0, [pc, #728]
	movs r1, #0
	bl 0x0200b58c
	movs r0, #11
	ldr r1, [pc, #720]
	ldr r2, [pc, #724]
	bl 0x0200b4fc
	movs r0, #5
	ldr r1, [pc, #720]
	ldr r2, [pc, #684]
	bl 0x0200b4fc
	movs r2, #169
	ldr r1, [pc, #684]
	lsls r2, r2, #1
	movs r0, #11
	bl 0x0200b524
	movs r0, #11
	bl 0x0200b4ec
	adds r2, r0, #0
	adds r5, r2, #0
	adds r5, #90
	ldrb r2, [r5]
	movs r3, #254
	ands r3, r2
	movs r2, #174
	strb r3, [r5]
	lsls r2, r2, #1
	movs r0, #11
	ldr r1, [pc, #648]
	bl 0x0200b524
	movs r0, #5
	movs r1, #1
	bl 0x0200b55c
	movs r0, #5
	movs r1, #4
	movs r2, #0
	bl 0x0200b54c
	movs r2, #158
	movs r0, #5
	ldr r1, [pc, #648]
	lsls r2, r2, #1
	bl 0x0200b524
	movs r1, #128
	movs r2, #128
	movs r0, #5
	lsls r1, r1, #8
	lsls r2, r2, #7
	bl 0x0200b4fc
	movs r2, #0
	movs r0, #13
	ldr r1, [pc, #604]
	bl 0x0200b5ac
	movs r0, #13
	movs r1, #3
	bl 0x0200b554
	movs r2, #30
	movs r0, #13
	movs r1, #0
	bl 0x0200b594
	movs r1, #1
	movs r0, #11
	bl 0x0200b55c
	movs r0, #20
	bl 0x0200b4cc
	ldrb r2, [r5]
	movs r3, #1
	orrs r3, r2
	movs r1, #128
	movs r2, #128
	strb r3, [r5]
	movs r0, #11
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200b4fc
	movs r2, #166
	movs r0, #11
	ldr r1, [pc, #532]
	lsls r2, r2, #1
	bl 0x0200b524
	movs r1, #176
	movs r2, #20
	movs r0, #11
	lsls r1, r1, #8
	bl 0x0200b59c
	ldr r0, [pc, #524]
	movs r1, #0
	bl 0x0200b58c
	movs r1, #176
	movs r2, #0
	movs r0, #10
	lsls r1, r1, #8
	bl 0x0200b59c
	movs r0, #10
	movs r1, #2
	bl 0x0200b554
	movs r0, #10
	movs r1, #0
	movs r2, #20
	bl 0x0200b594
	movs r1, #129
	movs r0, #11
	lsls r1, r1, #1
	movs r2, #20
	bl 0x0200b5ac
	movs r1, #192
	movs r0, #5
	lsls r1, r1, #6
	movs r2, #0
	bl 0x0200b59c
	movs r1, #192
	movs r2, #20
	movs r0, #11
	lsls r1, r1, #6
	bl 0x0200b59c
	movs r0, #10
	movs r1, #4
	bl 0x0200b544
	movs r0, #11
	movs r1, #3
	bl 0x0200b544
	movs r1, #176
	movs r2, #20
	movs r0, #11
	lsls r1, r1, #8
	bl 0x0200b59c
	movs r0, #11
	movs r1, #3
	bl 0x0200b544
	movs r1, #3
	movs r0, #13
	bl 0x0200b544
	movs r0, #20
	bl 0x0200b4cc
	movs r1, #216
	movs r2, #158
	movs r0, #5
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl 0x0200b51c
	movs r1, #211
	ldr r2, [pc, #404]
	movs r0, #13
	lsls r1, r1, #1
	bl 0x0200b524
	movs r0, #5
	movs r1, #1
	bl 0x0200b53c
	movs r1, #192
	movs r0, #5
	lsls r1, r1, #6
	movs r2, #0
	bl 0x0200b59c
	movs r1, #192
	movs r2, #20
	movs r0, #13
	lsls r1, r1, #6
	bl 0x0200b59c
	movs r0, #10
	movs r1, #2
	bl 0x0200b55c
	movs r1, #208
	movs r0, #10
	lsls r1, r1, #8
	movs r2, #20
	bl 0x0200b59c
	movs r2, #40
	ldr r0, [pc, #344]
	movs r1, #0
	bl 0x0200b594
	movs r1, #2
	movs r0, #9
	bl 0x0200b55c
	movs r0, #20
	bl 0x0200b4cc
	movs r1, #176
	movs r0, #9
	lsls r1, r1, #8
	movs r2, #30
	bl 0x0200b59c
	movs r1, #128
	movs r2, #128
	movs r0, #9
	lsls r1, r1, #8
	lsls r2, r2, #7
	bl 0x0200b4fc
	movs r2, #148
	movs r0, #9
	ldr r1, [pc, #260]
	lsls r2, r2, #1
	bl 0x0200b524
	movs r1, #160
	movs r0, #9
	lsls r1, r1, #7
	movs r2, #40
	bl 0x0200b59c
	movs r1, #128
	movs r2, #128
	movs r0, #10
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200b4fc
	movs r1, #128
	movs r2, #128
	movs r0, #11
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200b4fc
	movs r1, #128
	movs r2, #128
	movs r0, #13
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200b4fc
	movs r1, #128
	movs r2, #128
	movs r0, #14
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200b4fc
	movs r2, #154
	movs r0, #10
	ldr r1, [pc, #216]
	lsls r2, r2, #1
	bl 0x0200b524
	movs r1, #160
	movs r0, #10
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200b59c
	movs r2, #154
	movs r0, #11
	ldr r1, [pc, #196]
	lsls r2, r2, #1
	bl 0x0200b524
	movs r1, #192
	movs r0, #11
	lsls r1, r1, #6
	movs r2, #0
	bl 0x0200b59c
	movs r2, #154
	movs r0, #14
	ldr r1, [pc, #176]
	lsls r2, r2, #1
	bl 0x0200b524
	movs r1, #192
	movs r0, #10
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200b59c
	movs r1, #208
	movs r0, #11
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200b59c
	movs r1, #176
	movs r0, #14
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200b59c
	movs r1, #208
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200b59c
	movs r1, #208
	movs r2, #0
	movs r0, #13
	lsls r1, r1, #8
	bl 0x0200b59c
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #8
	lsls r1, r1, #5
	bl 0x0200b5bc
	movs r0, #236
	movs r1, #1
	movs r2, #150
	movs r3, #1
	negs r1, r1
	lsls r2, r2, #17
	lsls r0, r0, #17
	bl 0x0200b5c4
	bl 0x0200b5cc
	movs r0, #40
	b .L_02000054_3
	.4byte 0x0000200e
	.4byte 0x00002005
	.4byte 0x00000105
	.4byte 0x00000101
	.4byte 0x00009999
	.4byte 0x00004ccc
	.4byte 0x000001db
	.4byte 0x000001eb
	.4byte 0x00000103
	.4byte 0x0000200b
	.4byte 0x00019999
	.4byte 0x0000cccc
	.4byte 0x00013333
	.4byte 0x000001cb
	.4byte 0x00000137
	.4byte 0x0000100a
	.4byte 0x000001d7
	.4byte 0x000001c7
	.4byte 0x000001e7
.L_02000054_3:
	bl 0x0200b4cc
	ldr r0, [pc, #232]
	bl 0x0200b4c4
	movs r1, #128
	movs r2, #128
	movs r0, #10
	lsls r1, r1, #8
	lsls r2, r2, #7
	bl 0x0200b4fc
	movs r1, #128
	movs r2, #128
	movs r0, #11
	lsls r1, r1, #8
	lsls r2, r2, #7
	bl 0x0200b4fc
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #7
	movs r0, #14
	lsls r1, r1, #8
	bl 0x0200b4fc
	movs r1, #3
	movs r0, #10
	bl 0x0200b544
	movs r0, #10
	bl 0x02008e30
	movs r1, #2
	movs r0, #9
	bl 0x0200b55c
	movs r0, #9
	bl 0x02008e30
	movs r1, #3
	movs r0, #11
	bl 0x0200b544
	movs r0, #11
	bl 0x02008e30
	movs r1, #144
	movs r2, #40
	movs r0, #5
	lsls r1, r1, #8
	bl 0x0200b59c
	movs r0, #5
	movs r1, #2
	bl 0x0200b554
	ldr r0, [pc, #128]
	movs r1, #0
	movs r2, #40
	bl 0x0200b594
	movs r1, #192
	movs r0, #13
	lsls r1, r1, #6
	movs r2, #0
	bl 0x0200b59c
	movs r1, #176
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #30
	bl 0x0200b59c
	movs r1, #224
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #13
	bl 0x0200b59c
	movs r0, #5
	bl 0x02008e30
	movs r0, #13
	bl 0x02008e30
	movs r1, #224
	movs r0, #14
	lsls r1, r1, #7
	movs r2, #40
	bl 0x0200b59c
	movs r2, #30
	movs r0, #14
	movs r1, #0
	bl 0x0200b594
	movs r1, #4
	movs r0, #14
	bl 0x0200b544
	movs r0, #20
	bl 0x0200b4cc
	movs r1, #0
	movs r2, #30
	movs r0, #14
	bl 0x0200b594
	movs r0, #14
	bl 0x02008e30
	bl 0x02009410
	movs r0, #5
	bl 0x0200b4dc
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.4byte 0x00000246
	.4byte 0x00002005
	.global Func_02000e30
	.thumb_func
Func_02000e30:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	mov r10, r0
	movs r0, #8
	bl 0x0200b4ec
	movs r3, #128
	lsls r3, r3, #9
	movs r2, #145
	str r3, [r0, #24]
	str r3, [r0, #28]
	ldr r1, [pc, #204]
	lsls r2, r2, #1
	mov r8, r0
	mov r0, r10
	bl 0x0200b524
	movs r1, #192
	lsls r1, r1, #8
	movs r2, #0
	mov r0, r10
	bl 0x0200b59c
	movs r0, #10
	bl 0x0200b4cc
	movs r2, #145
	lsls r2, r2, #17
	ldr r1, [pc, #172]
	movs r0, #8
	bl 0x0200b534
	mov r0, r10
	bl 0x0200b4ec
	adds r5, r0, #0
	mov r0, r10
	bl 0x0200b4ec
	movs r1, #0
	bl 0x0200b484
	movs r1, #128
	mov r0, r10
	lsls r1, r1, #1
	bl 0x0200b56c
	adds r2, r5, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	movs r0, #201
	bl 0x0200b624
	movs r6, #0
.L_02000e30_0:
	ldr r3, [r5, #12]
	movs r2, #128
	lsls r2, r2, #8
	adds r3, r3, r2
	str r3, [r5, #12]
	movs r0, #1
	bl 0x0200b4cc
	adds r3, r6, #1
	lsls r3, r3, #24
	lsrs r6, r3, #24
	cmp r6, #60
	bne .L_02000e30_0
	movs r0, #190
	bl 0x0200b624
	ldr r7, [pc, #92]
	movs r6, #0
.L_02000e30_1:
	ldr r3, [r5, #12]
	ldr r2, [pc, #88]
	adds r3, r3, r2
	str r3, [r5, #12]
	ldr r3, [r5, #24]
	adds r3, r3, r7
	str r3, [r5, #24]
	ldr r3, [r5, #28]
	adds r3, r3, r7
	str r3, [r5, #28]
	mov r2, r8
	ldr r3, [r2, #24]
	adds r3, r3, r7
	str r3, [r2, #24]
	ldr r3, [r2, #28]
	adds r3, r3, r7
	str r3, [r2, #28]
	movs r0, #1
	bl 0x0200b4cc
	adds r3, r6, #1
	lsls r3, r3, #24
	lsrs r6, r3, #24
	cmp r6, #90
	bne .L_02000e30_1
	mov r0, r10
	movs r1, #0
	movs r2, #0
	bl 0x0200b534
	movs r0, #8
	movs r1, #0
	movs r2, #0
	bl 0x0200b534
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.4byte 0x000001d7
	.4byte 0x01d70000
	.4byte 0xfffffd71
	.4byte 0x00001999
	.global Func_02000f28
	.thumb_func
Func_02000f28:
	bx lr
	.2byte 0x0000
	.global Func_02000f2c
	.thumb_func
Func_02000f2c:
	bx lr
	.2byte 0x0000
	.global Func_02000f30
	.thumb_func
Func_02000f30:
	push {lr}
	ldr r3, [pc, #28]
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #10
	bne .L_02000f30_0
	bl 0x0200b5e4
	bl 0x02008054
.L_02000f30_0:
	movs r0, #0
	pop {r1}
	bx r1
	.4byte 0x02000240
	.global Func_02000f54
	.thumb_func
Func_02000f54:
	ldrh r3, [r0, #6]
	movs r1, #128
	ldr r2, [r0, #80]
	lsls r1, r1, #7
	adds r3, r3, r1
	strh r3, [r2, #30]
	movs r0, #1
	bx lr
	.global Func_02000f64
	.thumb_func
Func_02000f64:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r7, #16
.L_02000f64_0:
	adds r0, r7, #0
	bl 0x0200b4ec
	adds r5, r0, #0
	adds r0, r7, #0
	bl 0x0200b504
	adds r0, r5, #0
	movs r1, #0
	bl 0x0200b484
	adds r0, r5, #0
	movs r1, #2
	bl 0x0200b444
	ldr r1, [r5, #80]
	movs r0, #13
	ldrb r3, [r1, #9]
	negs r0, r0
	adds r2, r0, #0
	ands r3, r2
	adds r2, r5, #0
	strb r3, [r1, #9]
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	movs r3, #128
	lsls r3, r3, #12
	str r3, [r5, #48]
	movs r3, #192
	lsls r3, r3, #8
	str r3, [r5, #52]
	ldr r3, [pc, #200]
	str r3, [r5, #24]
	str r3, [r5, #28]
	movs r3, #232
	lsls r3, r3, #16
	str r3, [r5, #8]
	movs r3, #160
	lsls r3, r3, #13
	str r3, [r5, #12]
	movs r3, #132
	lsls r3, r3, #16
	adds r7, #1
	str r3, [r5, #16]
	cmp r7, #31
	bls .L_02000f64_0
	movs r0, #145
	bl 0x0200b624
	ldr r1, [pc, #168]
	movs r7, #0
	mov r10, r1
	adds r0, r7, #0
	adds r0, #16
	bl 0x0200b4ec
	adds r6, r0, #0
	ldr r0, [pc, #156]
	ldr r2, [r6, #80]
	lsls r5, r7, #12
	adds r3, r5, r0
	strh r3, [r2, #30]
	adds r0, r5, #0
	bl 0x0200b40c
	movs r1, #128
	lsls r1, r1, #17
	movs r0, r0
	mov r12, pc
	bx r10
	.2byte 0x4680
	.2byte 0x1c28
	.2byte 0xf002
	.2byte 0xfa00
	.2byte 0x2180
	.2byte 0x0449
	.2byte 0x46fc
	.2byte 0x4750
	.2byte 0x68b1
	.2byte 0x6933
	.2byte 0x4441
	.2byte 0x181b
	.2byte 0x68f2
	.2byte 0x1c30
	.2byte 0x3701
	.2byte 0xf002
	.2byte 0xfa27
	.2byte 0x2f0f
	.2byte 0xd9da
	.2byte 0x2014
	.2byte 0xf002
	.2byte 0xfa52
	.2byte 0x2010
	.2byte 0xf002
	.2byte 0xfa7f
	.2byte 0x2180
	.2byte 0x2380
	.2byte 0x0249
	.2byte 0x061b
	.2byte 0x2710
	.2byte 0x468a
	.2byte 0x2600
	.2byte 0x4698
	.2byte 0x1c38
	.2byte 0xf002
	.2byte 0xfa54
	.2byte 0x1c05
	.2byte 0x1c38
	.2byte 0xf002
	.2byte 0xfa5c
	.2byte 0x3701
	.2byte 0x4641
	.2byte 0x4650
	.2byte 0x61a8
	.2byte 0x61e8
	.2byte 0x60ae
	.2byte 0x60ee
	.2byte 0x612e
	.2byte 0x626e
	.2byte 0x62ae
	.2byte 0x62ee
	.2byte 0x63a9
	.2byte 0x63e9
	.2byte 0x6429
	.2byte 0x2f1f
	.2byte 0xd9e8
	.2byte 0xbc28
	.2byte 0x4698
	.2byte 0x46aa
	.2byte 0xbce0
	.2byte 0xbc01
	.2byte 0x4700
	.4byte 0x0001cccc
	.4byte 0x03000118
	.4byte 0xffffc000
	.global Func_02001084
	.thumb_func
Func_02001084:
	push {lr}
	ldr r2, [pc, #40]
	movs r3, #63
	str r3, [r2]
	ldr r3, [pc, #36]
	movs r2, #0
	str r2, [r3]
	ldr r3, [pc, #36]
	str r2, [r3]
	ldr r2, [pc, #36]
	movs r3, #120
	str r3, [r2]
	ldr r2, [pc, #32]
	movs r3, #0
	movs r1, #0
.L_02001084_0:
	adds r3, #1
	stmia r2!, {r1}
	cmp r3, #15
	bls .L_02001084_0
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0200bb68
	.4byte 0x0200bb00
	.4byte 0x0200bb6c
	.4byte 0x0200bb70
	.4byte 0x0200bac0
	.global Func_020010c4
	.thumb_func
Func_020010c4:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r3, [pc, #596]
	ldr r5, [r3]
	ldr r3, [pc, #596]
	movs r1, #10
	ldr r0, [r3]
	sub sp, #4
	bl 0x0200b3d4
	cmp r0, #0
	beq .L_020010c4_0
	ldr r1, [pc, #584]
	movs r2, #0
	adds r3, r5, r1
	str r2, [r3]
	lsls r1, r0, #16
	movs r2, #128
	adds r0, r1, #0
	lsls r2, r2, #9
	bl 0x0200b494
	b .L_020010c4_1
.L_020010c4_0:
	ldr r3, [pc, #560]
	movs r0, #1
	adds r2, r5, r3
	movs r1, #1
	movs r3, #1
	str r3, [r2]
	negs r0, r0
	negs r1, r1
	ldr r2, [pc, #548]
	bl 0x0200b494
.L_020010c4_1:
	ldr r2, [pc, #536]
	ldr r3, [r2]
	cmp r3, #0
	beq .L_020010c4_2
	subs r3, #3
	str r3, [r2]
.L_020010c4_2:
	ldr r1, [pc, #536]
	movs r0, #0
	mov r8, r0
	mov r10, r1
	movs r2, #0
.L_020010c4_6:
	mov r3, r8
	lsls r6, r3, #2
	mov r0, r10
	ldr r3, [r0, r6]
	cmp r3, #0
	beq .L_020010c4_3
	mov r0, r8
	adds r0, #16
	str r2, [sp, #0]
	bl 0x0200b4ec
	adds r5, r0, #0
	movs r1, #128
	ldr r3, [r5, #56]
	lsls r1, r1, #24
	ldr r2, [sp, #0]
	cmp r3, r1
	bne .L_020010c4_3
	ldr r7, [r5, #64]
	cmp r7, r3
	bne .L_020010c4_3
	mov r0, r10
	ldr r3, [r0, r6]
	adds r3, #1
	str r3, [r0, r6]
	cmp r3, #2
	bne .L_020010c4_4
	adds r0, r5, #0
	movs r1, #3
	bl 0x0200b444
	ldr r2, [sp, #0]
.L_020010c4_4:
	mov r1, r10
	ldr r3, [r1, r6]
	cmp r3, #19
	bne .L_020010c4_5
	str r2, [r5, #8]
	str r2, [r5, #12]
	str r2, [r5, #16]
	str r2, [r5, #36]
	str r2, [r5, #40]
	str r2, [r5, #44]
	str r7, [r5, #56]
	str r7, [r5, #60]
	str r7, [r5, #64]
	adds r0, r5, #0
	movs r1, #15
	str r2, [sp, #0]
	bl 0x0200b574
	ldr r2, [sp, #0]
	b .L_020010c4_3
.L_020010c4_5:
	cmp r3, #20
	bne .L_020010c4_3
	mov r3, r10
	str r2, [r3, r6]
.L_020010c4_3:
	mov r3, r8
	adds r3, #1
	lsls r3, r3, #24
	lsrs r3, r3, #24
	mov r8, r3
	cmp r3, #15
	bls .L_020010c4_6
	ldr r3, [pc, #404]
	ldr r0, [pc, #404]
	ldr r2, [r3]
	cmp r2, r0
	bne .L_020010c4_7
	b .L_020010c4_8
.L_020010c4_7:
	ldr r3, [pc, #400]
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	beq .L_020010c4_9
	b .L_020010c4_8
.L_020010c4_9:
	ldr r2, [pc, #392]
	movs r1, #0
	ldr r6, [pc, #372]
	mov r8, r1
	mov r9, r2
.L_020010c4_13:
	bl 0x0200b3fc
	ldr r1, [pc, #380]
	bl 0x0200b3dc
	lsls r0, r0, #16
	asrs r0, r0, #16
	mov r11, r0
	mov r0, r8
	adds r0, #16
	bl 0x0200b4ec
	mov r3, r8
	lsls r5, r3, #2
	adds r7, r0, #0
	ldr r0, [r6, r5]
	mov r10, r0
	cmp r0, #0
	beq .L_020010c4_10
	b .L_020010c4_11
.L_020010c4_10:
	ldr r0, [pc, #348]
	bl 0x0200b4bc
	cmp r0, #0
	bne .L_020010c4_12
	movs r0, #246
	bl 0x0200b624
.L_020010c4_12:
	movs r3, #1
	str r3, [r6, r5]
	adds r3, r7, #0
	mov r1, r10
	adds r3, #85
	strb r1, [r3]
	movs r2, #128
	movs r3, #128
	lsls r2, r2, #12
	lsls r3, r3, #9
	str r2, [r7, #48]
	str r3, [r7, #52]
	adds r0, r7, #0
	movs r1, #0
	bl 0x0200b484
	ldr r1, [r7, #80]
	movs r0, #13
	ldrb r3, [r1, #9]
	negs r0, r0
	adds r2, r0, #0
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	strb r3, [r1, #9]
	adds r0, r7, #0
	movs r1, #2
	bl 0x0200b444
	adds r0, r7, #0
	ldr r1, [pc, #276]
	bl 0x0200b44c
	mov r1, r11
	lsls r6, r1, #16
	lsrs r6, r6, #16
	adds r0, r6, #0
	bl 0x0200b40c
	adds r5, r0, #0
	bl 0x0200b3fc
	adds r1, r0, #0
	lsls r1, r1, #8
	lsrs r1, r1, #16
	movs r2, #128
	lsls r2, r2, #17
	lsls r1, r1, #16
	adds r0, r5, #0
	adds r1, r1, r2
	mov r12, pc
	bx r9
	.2byte 0x4b3b
	.2byte 0x18c0
	.2byte 0x60b8
	.2byte 0x4650
	.2byte 0x60f8
	.2byte 0x1c30
	.2byte 0xf002
	.2byte 0xf8c6
	.2byte 0x1c05
	.2byte 0xf002
	.2byte 0xf8bf
	.2byte 0x1c01
	.2byte 0x0209
	.2byte 0x0c09
	.2byte 0x2280
	.2byte 0x0452
	.2byte 0x0409
	.2byte 0x1c28
	.2byte 0x1889
	.2byte 0x0000
	.2byte 0x46fc
	.2byte 0x4748
	.2byte 0x2397
	.2byte 0x045b
	.2byte 0x18c0
	.2byte 0x6138
	.2byte 0x1c30
	.2byte 0xf002
	.2byte 0xf8b5
	.2byte 0x1c05
	.2byte 0xf002
	.2byte 0xf8aa
	.2byte 0x1c01
	.2byte 0x203f
	.2byte 0x4001
	.2byte 0x2280
	.2byte 0x0312
	.2byte 0x0409
	.2byte 0x1c28
	.2byte 0x1889
	.2byte 0x46fc
	.2byte 0x4748
	.2byte 0x1c05
	.2byte 0x1c30
	.2byte 0xf002
	.2byte 0xf8a0
	.2byte 0x1c06
	.2byte 0xf002
	.2byte 0xf899
	.2byte 0x233f
	.2byte 0x1c01
	.2byte 0x4019
	.2byte 0x2280
	.2byte 0x0312
	.2byte 0x0409
	.2byte 0x1c30
	.2byte 0x1889
	.2byte 0x0000
	.2byte 0x46fc
	.2byte 0x4748
	.2byte 0x4b1d
	.2byte 0x218f
	.2byte 0x18ed
	.2byte 0x0449
	.2byte 0x1843
	.2byte 0x2200
	.2byte 0x1c38
	.2byte 0x1c29
	.2byte 0xf002
	.2byte 0xf8bc
	.2byte 0x1c38
	.2byte 0x2100
	.2byte 0xf002
	.2byte 0xf93c
	.2byte 0x4a0b
	.2byte 0x231e
	.2byte 0x6013
	.2byte 0xe007
.L_020010c4_11:
	mov r3, r8
	adds r3, #1
	lsls r3, r3, #24
	lsrs r3, r3, #24
	mov r8, r3
	cmp r3, #15
	bhi .L_020010c4_8
	b .L_020010c4_13
.L_020010c4_8:
	sub sp, #-4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x03001ec4
	.4byte 0x0200bb00
	.4byte 0x0000040c
	.4byte 0x0000e666
	.4byte 0x0200bac0
	.4byte 0x0200bb68
	.4byte 0x000003e7
	.4byte 0x03001e40
	.4byte 0x03000118
	.4byte 0x0000ffff
	.4byte 0x00000246
	.4byte 0x0200ba00
	.2byte 0x0000
	.2byte 0x0145
	.global Func_0200135c
	.thumb_func
Func_0200135c:
	push {r5, lr}
	ldr r2, [pc, #164]
	ldr r3, [r2]
	cmp r3, #0
	beq .L_0200135c_0
	subs r3, #1
	str r3, [r2]
	b .L_0200135c_1
.L_0200135c_0:
	ldr r5, [pc, #152]
	ldr r3, [r5]
	cmp r3, #0
	beq .L_0200135c_2
	subs r3, #1
	str r3, [r5]
	b .L_0200135c_3
.L_0200135c_2:
	bl 0x0200b3fc
	lsls r0, r0, #2
	lsrs r0, r0, #16
	str r0, [r5]
.L_0200135c_3:
	ldr r3, [pc, #128]
	ldr r2, [r3]
	cmp r2, #2
	beq .L_0200135c_4
	cmp r2, #2
	bhi .L_0200135c_5
	cmp r2, #1
	beq .L_0200135c_6
	b .L_0200135c_7
.L_0200135c_5:
	cmp r2, #3
	bne .L_0200135c_7
	ldr r3, [pc, #112]
	str r2, [r3]
	ldr r5, [pc, #100]
	bl 0x0200b3fc
	lsls r3, r0, #2
	adds r3, r3, r0
	lsls r3, r3, #2
	lsrs r3, r3, #16
	adds r3, #40
	b .L_0200135c_8
.L_0200135c_4:
	ldr r2, [pc, #88]
	movs r3, #15
	str r3, [r2]
	ldr r5, [pc, #76]
	bl 0x0200b3fc
	lsls r3, r0, #2
	adds r3, r3, r0
	lsls r3, r3, #3
	lsrs r3, r3, #16
	adds r3, #80
	b .L_0200135c_8
.L_0200135c_6:
	ldr r2, [pc, #64]
	movs r3, #63
	str r3, [r2]
	ldr r5, [pc, #52]
	bl 0x0200b3fc
	lsls r3, r0, #2
	adds r3, r3, r0
	lsls r3, r3, #4
	lsrs r3, r3, #16
	adds r3, #160
	b .L_0200135c_8
.L_0200135c_7:
	ldr r2, [pc, #40]
	movs r3, #127
	str r3, [r2]
	ldr r5, [pc, #28]
	bl 0x0200b3fc
	lsls r3, r0, #2
	adds r3, r3, r0
	lsls r3, r3, #5
	movs r2, #160
	lsrs r3, r3, #16
	lsls r2, r2, #1
	adds r3, r3, r2
.L_0200135c_8:
	str r3, [r5]
.L_0200135c_1:
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0200bb70
	.4byte 0x0200bb6c
	.4byte 0x0200bb68
	.global Func_02001410
	.thumb_func
Func_02001410:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	ldr r3, [pc, #892]
	ldr r3, [r3]
	movs r0, #15
	sub sp, #8
	mov r9, r3
	bl 0x0200b4ec
	mov r10, r0
	ldr r0, [pc, #880]
	bl 0x0200b3f4
	ldr r2, [pc, #876]
	movs r3, #3
	str r3, [r2]
	movs r0, #80
	bl 0x0200b4cc
	movs r0, #17
	bl 0x0200b624
	movs r1, #0
	ldr r0, [pc, #860]
	bl 0x0200b5ec
	movs r0, #40
	bl 0x0200b5f4
	movs r0, #40
	bl 0x0200b4cc
	ldr r0, [pc, #848]
	bl 0x0200b3f4
	movs r5, #0
.L_02001410_0:
	adds r0, r5, #0
	adds r0, #16
	movs r1, #0
	movs r2, #0
	bl 0x0200b534
	adds r3, r5, #1
	lsls r3, r3, #24
	lsrs r5, r3, #24
	cmp r5, #15
	bls .L_02001410_0
	movs r0, #1
	bl 0x0200b3e4
	movs r1, #0
	movs r2, #0
	movs r0, #15
	bl 0x0200b534
	movs r0, #0
	bl 0x0200a820
	movs r0, #1
	bl 0x0200a820
	ldr r3, [pc, #792]
	movs r5, #0
	add r3, r9
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #9
	lsls r1, r1, #9
	str r5, [r3]
	lsls r0, r0, #9
	bl 0x0200b494
	movs r0, #80
	bl 0x0200b4cc
	bl 0x0200ac9c
	bl 0x0200b5d4
	adds r3, r0, #0
	adds r3, #85
	strb r5, [r3]
	movs r3, #231
	lsls r3, r3, #16
	str r3, [r0, #8]
	movs r3, #144
	lsls r3, r3, #16
	str r3, [r0, #16]
	movs r3, #128
	lsls r3, r3, #24
	str r3, [r0, #56]
	str r3, [r0, #60]
	str r3, [r0, #64]
	str r5, [r0, #12]
	str r5, [r0, #36]
	str r5, [r0, #44]
	movs r0, #4
	bl 0x0200b3e4
	bl 0x0200b464
	movs r0, #4
	bl 0x0200b3e4
	movs r0, #0
	movs r1, #3
	bl 0x0200b5a4
	movs r0, #1
	movs r1, #3
	bl 0x0200b5a4
	movs r0, #128
	movs r1, #0
	lsls r0, r0, #9
	bl 0x0200b5ec
	movs r0, #40
	bl 0x0200b5f4
	movs r0, #40
	bl 0x0200b4cc
	movs r0, #0
	bl 0x0200b4ec
	mov r8, r0
	movs r0, #1
	bl 0x0200b4ec
	adds r6, r0, #0
	mov r0, r8
	movs r1, #192
	ldr r2, [r0, #80]
	ldr r7, [r6, #80]
	lsls r1, r1, #7
.L_02001410_1:
	ldrh r3, [r2, #30]
	movs r0, #128
	lsls r0, r0, #1
	adds r3, r3, r0
	strh r3, [r2, #30]
	ldrh r3, [r7, #30]
	ldr r0, [pc, #632]
	adds r3, r3, r0
	strh r3, [r7, #30]
	mov r0, r8
	ldr r3, [r0, #8]
	adds r3, r3, r1
	str r3, [r0, #8]
	ldr r3, [r6, #8]
	subs r3, r3, r1
	str r3, [r6, #8]
	movs r0, #1
	str r1, [sp, #4]
	str r2, [sp, #0]
	bl 0x0200b3e4
	adds r5, #1
	ldr r1, [sp, #4]
	ldr r2, [sp, #0]
	cmp r5, #19
	bls .L_02001410_1
	movs r0, #40
	bl 0x0200b4cc
	movs r1, #128
	movs r2, #128
	movs r0, #0
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl 0x0200b4fc
.L_02001570:
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #10
	lsls r2, r2, #9
	movs r0, #1
	bl 0x0200b4fc
	movs r0, #0
	bl 0x0200b4ec
	ldr r3, [r0, #80]
	movs r5, #0
	strh r5, [r3, #30]
	movs r0, #1
	bl 0x0200b4ec
	ldr r3, [r0, #80]
	movs r1, #6
	strh r5, [r3, #30]
	movs r0, #0
	movs r2, #0
	bl 0x0200b54c
	movs r0, #1
	movs r1, #6
	movs r2, #0
	bl 0x0200b54c
	movs r0, #0
	movs r1, #246
	movs r2, #150
	bl 0x0200b50c
	movs r2, #150
	movs r0, #1
	movs r1, #220
	bl 0x0200b514
	movs r0, #0
	movs r1, #2
	bl 0x0200b5a4
	movs r1, #2
	movs r0, #1
	bl 0x0200b5a4
	movs r0, #0
	bl 0x0200b4ec
	adds r0, #35
	ldrb r3, [r0]
	movs r5, #1
	orrs r3, r5
	strb r3, [r0]
	movs r0, #1
	bl 0x0200b4ec
	adds r0, #35
	ldrb r3, [r0]
	orrs r5, r3
	strb r5, [r0]
	movs r0, #0
	bl 0x0200b504
	movs r0, #1
	bl 0x0200b504
	movs r1, #128
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #0
	bl 0x0200b59c
	movs r0, #20
	bl 0x0200b4cc
	movs r1, #224
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #1
	bl 0x0200b59c
	movs r0, #40
	bl 0x0200b4cc
	movs r1, #144
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #0
	bl 0x0200b59c
	movs r0, #40
	bl 0x0200b4cc
	movs r1, #160
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #1
	bl 0x0200b59c
	movs r0, #80
	bl 0x0200b4cc
	movs r1, #128
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #0
	bl 0x0200b59c
	movs r0, #10
	bl 0x0200b4cc
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #5
	movs r0, #1
	bl 0x0200b59c
	movs r0, #60
	bl 0x0200b4cc
	movs r1, #2
	movs r0, #1
	bl 0x0200b55c
	ldr r5, [pc, #328]
	adds r0, r5, #0
	bl 0x0200b57c
	movs r1, #0
	movs r0, #1
	bl 0x0200b584
	movs r0, #0
	movs r1, #0
	bl 0x0200b4e4
	cmp r0, #0
	bne .L_02001570_0
	movs r0, #1
	movs r1, #3
	bl 0x0200b544
	adds r0, r5, #1
	bl 0x0200b57c
	b .L_02001570_1
.L_02001570_0:
	movs r0, #1
	movs r1, #1
	bl 0x0200b55c
	adds r0, r5, #2
	bl 0x0200b57c
.L_02001570_1:
	movs r0, #1
	movs r1, #0
	movs r2, #60
	bl 0x0200b594
	movs r1, #192
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #1
	bl 0x0200b59c
	movs r0, #40
	bl 0x0200b4cc
	movs r0, #1
	movs r1, #2
	movs r2, #0
	bl 0x0200b54c
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #1
	bl 0x0200b5ac
	movs r0, #40
	bl 0x0200b4cc
	ldr r0, [pc, #216]
	bl 0x0200b57c
	movs r0, #1
	movs r1, #0
	movs r2, #10
	bl 0x0200b594
	movs r1, #2
	movs r2, #0
	movs r0, #0
	bl 0x0200b54c
	movs r0, #10
	bl 0x0200b4cc
	movs r1, #128
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #0
	bl 0x0200b59c
	movs r0, #60
	bl 0x0200b4cc
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #0
	bl 0x0200b5ac
	movs r0, #80
	bl 0x0200b4cc
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #5
	movs r0, #1
	bl 0x0200b59c
	movs r0, #10
	bl 0x0200b4cc
	movs r0, #1
	movs r1, #2
	bl 0x0200b55c
	movs r1, #0
	movs r0, #1
	bl 0x0200b584
	movs r0, #0
	movs r1, #0
	bl 0x0200b4e4
	cmp r0, #0
	bne .L_02001570_2
	movs r0, #10
	bl 0x0200b4cc
	movs r1, #2
	movs r0, #1
	bl 0x0200b55c
	movs r0, #10
	bl 0x0200b4cc
	movs r1, #192
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #1
	bl 0x0200b59c
	movs r0, #60
	bl 0x0200b4cc
	movs r1, #128
	lsls r1, r1, #5
	movs r2, #0
	movs r0, #1
	bl 0x0200b59c
	movs r0, #10
	bl 0x0200b4cc
	movs r0, #1
	movs r1, #4
	bl 0x0200b53c
	ldr r0, [pc, #44]
	bl 0x0200b57c
	b .L_02001570_3
	.2byte 0x0000
	.2byte 0x1ec4
	.2byte 0x0300
	.2byte 0x935d
	.2byte 0x0200
	.2byte 0xbb68
	.2byte 0x0200
	.2byte 0x7fff
	.2byte 0x0000
	.2byte 0x90c5
	.2byte 0x0200
	.2byte 0x040c
	.2byte 0x0000
	.2byte 0xff00
	.2byte 0xffff
	.4byte 0x000010f8
	.4byte 0x000010fb
	.4byte 0x000010fd
.L_02001570_2:
	movs r0, #10
	bl 0x0200b4cc
	movs r1, #2
	movs r0, #1
	bl 0x0200b55c
	movs r0, #10
	bl 0x0200b4cc
	movs r1, #192
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #1
	bl 0x0200b59c
	movs r0, #60
	bl 0x0200b4cc
	movs r1, #128
	lsls r1, r1, #5
	movs r2, #0
	movs r0, #1
	bl 0x0200b59c
	movs r0, #10
	bl 0x0200b4cc
	movs r0, #1
	movs r1, #4
	bl 0x0200b53c
	ldr r0, [pc, #816]
	bl 0x0200b57c
.L_02001570_3:
	movs r2, #40
	movs r0, #1
	movs r1, #0
	bl 0x0200b594
	movs r0, #0
	movs r1, #3
	bl 0x0200b53c
	movs r1, #3
	movs r0, #1
	bl 0x0200b544
	movs r0, #40
	bl 0x0200b4cc
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200b59c
	movs r1, #224
	movs r2, #0
	movs r0, #1
	lsls r1, r1, #7
	bl 0x0200b59c
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #10
	lsls r1, r1, #7
	bl 0x0200b5bc
	movs r0, #142
	movs r1, #1
	movs r2, #184
	movs r3, #1
	negs r1, r1
	lsls r2, r2, #15
	lsls r0, r0, #17
	bl 0x0200b5c4
	bl 0x0200b5cc
	movs r0, #20
	bl 0x0200b4cc
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200b59c
	movs r1, #160
	movs r2, #0
	movs r0, #1
	lsls r1, r1, #7
	bl 0x0200b59c
	movs r0, #192
	movs r1, #192
	lsls r0, r0, #9
	lsls r1, r1, #6
	bl 0x0200b5bc
	movs r0, #254
	movs r1, #1
	movs r2, #162
	movs r3, #1
	negs r1, r1
	lsls r2, r2, #16
	lsls r0, r0, #15
	bl 0x0200b5c4
	bl 0x0200b5cc
	movs r0, #40
	bl 0x0200b4cc
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #6
	movs r2, #0
	bl 0x0200b59c
	movs r1, #192
	movs r2, #0
	movs r0, #1
	lsls r1, r1, #6
	bl 0x0200b59c
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #11
	lsls r1, r1, #8
	bl 0x0200b5bc
	movs r0, #152
	movs r1, #1
	movs r2, #147
	movs r3, #1
	negs r1, r1
	lsls r2, r2, #17
	lsls r0, r0, #17
	bl 0x0200b5c4
	bl 0x0200b5cc
	movs r0, #20
	bl 0x0200b4cc
	movs r0, #0
	movs r1, #0
	movs r2, #0
	bl 0x0200b59c
	movs r1, #128
	movs r2, #0
	movs r0, #1
	lsls r1, r1, #5
	bl 0x0200b59c
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #9
	lsls r1, r1, #6
	bl 0x0200b5bc
	movs r0, #200
	movs r1, #1
	movs r2, #215
	lsls r2, r2, #16
	movs r3, #1
	negs r1, r1
	lsls r0, r0, #17
	bl 0x0200b5c4
	bl 0x0200b5cc
	movs r0, #60
	bl 0x0200b4cc
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #11
	lsls r1, r1, #8
	bl 0x0200b5bc
	movs r1, #1
	movs r2, #145
	movs r3, #1
	lsls r2, r2, #16
	negs r1, r1
	ldr r0, [pc, #508]
	bl 0x0200b5c4
	bl 0x0200b5cc
	movs r0, #20
	bl 0x0200b4cc
	movs r1, #2
	movs r0, #1
	bl 0x0200b55c
	ldr r0, [pc, #488]
	bl 0x0200b57c
	movs r0, #1
	movs r1, #0
	movs r2, #20
	bl 0x0200b594
	movs r1, #192
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #1
	bl 0x0200b59c
	movs r0, #40
	bl 0x0200b4cc
	movs r1, #208
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #1
	bl 0x0200b59c
	movs r0, #20
	bl 0x0200b4cc
	movs r1, #240
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #1
	bl 0x0200b59c
	movs r0, #60
	bl 0x0200b4cc
	movs r1, #192
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #1
	bl 0x0200b59c
	movs r0, #40
	bl 0x0200b4cc
	movs r1, #240
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #1
	bl 0x0200b59c
	movs r0, #10
	bl 0x0200b4cc
	movs r2, #10
	movs r0, #1
	movs r1, #0
	bl 0x0200b594
	movs r1, #3
	movs r0, #0
	bl 0x0200b544
	movs r0, #10
	bl 0x0200b4cc
	ldr r5, [pc, #360]
	movs r0, #23
	bl 0x0200b624
	movs r0, #1
	movs r1, #4
	movs r2, #0
	bl 0x0200ad48
	add r5, r9
	movs r3, #0
	movs r0, #160
	movs r1, #160
	movs r2, #128
	str r3, [r5]
	lsls r1, r1, #11
	lsls r2, r2, #9
	lsls r0, r0, #11
	bl 0x0200b494
	movs r0, #10
	bl 0x0200b4cc
	movs r0, #0
	movs r1, #40
	movs r2, #0
	bl 0x0200ad48
	movs r1, #128
	movs r2, #128
	movs r0, #0
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl 0x0200b4fc
	movs r1, #128
	movs r2, #128
	movs r0, #1
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl 0x0200b4fc
	movs r0, #0
	movs r1, #6
	movs r2, #0
	bl 0x0200b54c
	movs r0, #1
	movs r1, #6
	movs r2, #0
	bl 0x0200b54c
	movs r0, #0
	movs r1, #243
	movs r2, #144
	bl 0x0200b50c
	movs r1, #202
	movs r2, #144
	movs r0, #1
	bl 0x0200b50c
	movs r0, #0
	bl 0x0200b52c
	movs r0, #20
	bl 0x0200b4cc
	movs r3, #1
	movs r0, #128
	movs r1, #128
	movs r2, #128
	str r3, [r5]
	lsls r1, r1, #9
	lsls r2, r2, #9
	lsls r0, r0, #9
	bl 0x0200b494
	movs r0, #60
	bl 0x0200b4cc
	movs r1, #129
	movs r0, #0
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200b5ac
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #1
	bl 0x0200b5ac
	movs r0, #80
	bl 0x0200b4cc
	movs r0, #0
	ldr r1, [pc, #172]
	ldr r2, [pc, #172]
	bl 0x0200b4fc
	movs r0, #1
	ldr r1, [pc, #160]
	ldr r2, [pc, #164]
	bl 0x0200b4fc
	movs r0, #1
	movs r1, #220
	movs r2, #150
	bl 0x0200b51c
	movs r2, #150
	movs r0, #0
	movs r1, #246
	bl 0x0200b524
	movs r0, #1
	movs r1, #1
	bl 0x0200b53c
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200b59c
	movs r1, #224
	movs r2, #0
	lsls r1, r1, #7
	movs r0, #1
	bl 0x0200b59c
	movs r0, #60
	bl 0x0200b4cc
	movs r1, #2
	movs r0, #1
	bl 0x0200b55c
	movs r0, #10
	bl 0x0200b4cc
	movs r1, #192
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #1
	bl 0x0200b59c
	movs r0, #40
	bl 0x0200b4cc
	movs r1, #0
	movs r0, #1
	bl 0x0200b584
	movs r0, #0
	movs r1, #0
	bl 0x0200b4e4
	cmp r0, #0
	bne .L_02001570_4
	movs r1, #128
	lsls r1, r1, #5
	movs r0, #1
	movs r2, #0
	bl 0x0200b59c
	movs r0, #10
	bl 0x0200b4cc
	movs r0, #1
	movs r1, #3
	bl 0x0200b544
	b .L_02001570_5
	.2byte 0x0000
	.4byte 0x000010fe
	.4byte 0x01110000
	.4byte 0x000010ff
	.4byte 0x0000040c
	.4byte 0x00006666
	.4byte 0x00003333
.L_02001570_4:
	movs r1, #128
	lsls r1, r1, #5
	movs r2, #0
	movs r0, #1
	bl 0x0200b59c
	movs r0, #10
	bl 0x0200b4cc
	movs r0, #1
	movs r1, #4
	bl 0x0200b53c
	ldr r0, [pc, #996]
	bl 0x0200b57c
.L_02001570_5:
	movs r0, #20
	bl 0x0200b4cc
	movs r0, #1
	movs r1, #0
	bl 0x0200b58c
	movs r1, #220
	movs r2, #152
	lsls r1, r1, #15
	lsls r2, r2, #16
	movs r0, #15
	bl 0x0200b534
	movs r0, #1
	bl 0x0200b3e4
	movs r0, #15
	ldr r1, [pc, #956]
	ldr r2, [pc, #960]
	bl 0x0200b4fc
	movs r0, #15
	movs r1, #171
	movs r2, #152
	bl 0x0200b50c
	movs r2, #0
	movs r1, #0
	movs r0, #15
.L_02001ba8:
	bl 0x0200b59c
	movs r0, #15
	bl 0x0200b4ec
	movs r1, #1
	bl 0x0200b484
	movs r1, #128
	movs r2, #128
	movs r0, #1
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200b4fc
	movs r0, #1
	movs r1, #217
	movs r2, #182
	bl 0x0200b51c
	movs r1, #224
	movs r2, #0
	lsls r1, r1, #7
	movs r0, #0
	bl 0x0200b59c
	movs r0, #10
	bl 0x0200b4cc
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #10
	lsls r1, r1, #7
	bl 0x0200b5bc
	movs r0, #217
	movs r1, #1
	movs r2, #176
	movs r3, #1
	lsls r2, r2, #16
	negs r1, r1
	lsls r0, r0, #16
	bl 0x0200b5c4
	bl 0x0200b5cc
	movs r0, #20
	bl 0x0200b4cc
	movs r1, #129
.L_02001c0c:
	lsls r1, r1, #1
	movs r0, #0
	bl 0x0200b5b4
	movs r0, #20
	bl 0x0200b4cc
	movs r1, #2
	movs r2, #0
	movs r0, #0
	bl 0x0200b54c
	movs r0, #10
	bl 0x0200b4cc
	movs r1, #4
	movs r2, #0
	movs r0, #0
	bl 0x0200b54c
	movs r0, #30
	bl 0x0200b4cc
	movs r1, #128
	movs r2, #0
	movs r0, #1
	lsls r1, r1, #1
	bl 0x0200b5ac
.L_02001c46:
	movs r1, #1
	movs r0, #1
	bl 0x0200b53c
	movs r0, #40
	bl 0x0200b4cc
	movs r1, #128
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #1
	bl 0x0200b59c
	movs r0, #4
	bl 0x0200b4cc
	movs r0, #1
	movs r1, #231
	movs r2, #175
	bl 0x0200b524
	movs r1, #224
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #1
	bl 0x0200b59c
	ldr r0, [pc, #728]
	bl 0x0200b57c
	movs r1, #0
	movs r0, #1
	bl 0x0200b58c
	movs r0, #40
	bl 0x0200b4cc
	ldr r1, [pc, #712]
	movs r2, #0
	movs r0, #1
	bl 0x0200b5ac
	movs r0, #40
	bl 0x0200b4cc
	movs r1, #160
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #1
	bl 0x0200b59c
	movs r0, #60
	bl 0x0200b4cc
	movs r0, #1
	movs r1, #1
	bl 0x0200b554
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #1
	bl 0x0200b5b4
	ldr r6, [pc, #664]
	movs r0, #40
	bl 0x0200b4cc
	movs r0, #1
	movs r1, #20
	movs r2, #0
	bl 0x0200ad48
	add r6, r9
	movs r3, #0
	movs r0, #128
	movs r1, #128
	movs r2, #128
	str r3, [r6]
	lsls r2, r2, #9
	lsls r1, r1, #9
	lsls r0, r0, #9
	mov r8, r3
	bl 0x0200b494
	movs r0, #40
	bl 0x0200b4cc
	movs r0, #0
	movs r1, #2
	bl 0x0200b554
	movs r1, #2
	movs r0, #1
	bl 0x0200b554
	ldr r0, [pc, #604]
	bl 0x0200b624
	movs r0, #128
	movs r1, #1
	lsls r0, r0, #9
	bl 0x0200b5ec
	movs r0, #40
	bl 0x0200b5f4
	movs r3, #1
	movs r0, #1
	movs r1, #1
	str r3, [r6]
	ldr r2, [pc, #580]
	negs r1, r1
	negs r0, r0
	bl 0x0200b494
	movs r0, #120
	bl 0x0200b4cc
	movs r1, #0
	movs r0, #15
	bl 0x0200b58c
	movs r0, #20
	bl 0x0200b4cc
	movs r1, #129
	movs r0, #0
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200b5ac
	movs r1, #129
	movs r2, #0
	lsls r1, r1, #1
	movs r0, #1
	bl 0x0200b5ac
	movs r0, #100
	bl 0x0200b4cc
	movs r1, #0
	movs r0, #15
	bl 0x0200b58c
	movs r0, #20
	bl 0x0200b4cc
	movs r0, #1
	movs r1, #10
	movs r2, #0
	bl 0x0200ad48
	mov r0, r8
	str r0, [r6]
	movs r1, #128
	movs r0, #128
	movs r2, #128
	lsls r1, r1, #10
	lsls r2, r2, #9
	lsls r0, r0, #10
	bl 0x0200b494
	movs r0, #20
	bl 0x0200b4cc
	movs r1, #128
	movs r2, #128
	movs r0, #0
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl 0x0200b4fc
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #10
	lsls r2, r2, #9
	movs r0, #1
	bl 0x0200b4fc
	movs r0, #0
	bl 0x0200b4ec
	adds r0, #90
	ldrb r2, [r0]
	movs r5, #254
	adds r3, r5, #0
	ands r3, r2
	strb r3, [r0]
	movs r0, #1
	bl 0x0200b4ec
	adds r0, #90
	ldrb r3, [r0]
	ands r5, r3
	strb r5, [r0]
	movs r1, #4
	movs r0, #0
	movs r2, #0
	bl 0x0200b54c
	movs r0, #1
	movs r1, #4
	movs r2, #0
	bl 0x0200b54c
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #1
	movs r2, #150
	bl 0x0200b50c
	movs r1, #231
	movs r2, #180
	movs r0, #1
	bl 0x0200b50c
	movs r0, #1
	bl 0x0200b52c
	movs r2, #0
	movs r1, #40
	movs r0, #0
	bl 0x0200ad48
	movs r0, #20
	bl 0x0200b4cc
	movs r0, #0
	bl 0x0200b4ec
	adds r0, #90
	ldrb r3, [r0]
	movs r5, #1
	orrs r3, r5
	strb r3, [r0]
	movs r0, #1
	bl 0x0200b4ec
	adds r0, #90
	ldrb r3, [r0]
	orrs r5, r3
	strb r5, [r0]
	movs r1, #2
	movs r0, #0
	bl 0x0200b554
	movs r1, #2
	movs r0, #1
	bl 0x0200b55c
	movs r0, #40
	bl 0x0200b4cc
	movs r2, #0
	ldr r1, [pc, #296]
	movs r0, #1
	bl 0x0200b5ac
	movs r0, #40
	bl 0x0200b4cc
	movs r1, #0
	movs r0, #1
	bl 0x0200b58c
	movs r0, #20
	bl 0x0200b4cc
	ldr r1, [pc, #252]
	movs r2, #0
	movs r0, #15
	bl 0x0200b5ac
	movs r0, #60
	bl 0x0200b4cc
	movs r1, #160
	movs r0, #0
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200b59c
	movs r1, #208
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #1
	bl 0x0200b59c
	movs r0, #20
	bl 0x0200b4cc
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200b59c
	movs r1, #176
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #1
	bl 0x0200b59c
	movs r0, #10
	bl 0x0200b4cc
	movs r0, #107
	bl 0x0200b624
	movs r1, #200
	lsls r1, r1, #4
	ldr r0, [pc, #184]
	bl 0x0200b3ec
	movs r0, #10
	bl 0x0200b4cc
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #1
	movs r0, #15
	bl 0x0200b5ac
	movs r0, #40
	bl 0x0200b4cc
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #9
	lsls r1, r1, #6
	bl 0x0200b5bc
	movs r0, #186
	movs r1, #1
	movs r2, #166
	movs r3, #1
	lsls r0, r0, #16
	negs r1, r1
	lsls r2, r2, #16
	bl 0x0200b5c4
	movs r1, #130
	movs r2, #113
	movs r0, #15
	bl 0x0200b50c
	movs r0, #15
	bl 0x0200b52c
	movs r1, #192
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #15
	bl 0x0200b59c
	movs r0, #20
	bl 0x0200b4cc
	mov r3, r8
	movs r0, #128
	movs r1, #128
	movs r2, #128
	str r3, [r6]
	lsls r2, r2, #9
	lsls r0, r0, #10
	lsls r1, r1, #10
	bl 0x0200b494
	movs r1, #1
	ldr r0, [pc, #72]
	bl 0x0200b5ec
	movs r0, #20
	bl 0x0200b5f4
	movs r0, #20
	bl 0x0200b4cc
	bl 0x0200aff0
	movs r0, #15
	movs r1, #2
	bl 0x0200b544
	movs r5, #0
	b .L_02001c46_0
	.2byte 0x0000
	.2byte 0x1103
	.2byte 0x0000
	.2byte 0x3333
	.2byte 0x0001
	.2byte 0x9999
	.2byte 0x0000
	.4byte 0x00001104
	.4byte 0x00000101
	.4byte 0x0000040c
	.4byte 0x00000121
	.4byte 0x0000e666
	.4byte 0x00000103
	.4byte 0x0200a93d
	.4byte 0x0020119e
.L_02001c46_0:
	mov r0, r10
	bl 0x0200ad94
	adds r5, #1
	movs r0, #1
	bl 0x0200b3e4
	cmp r5, #39
	bls .L_02001c46_0
	movs r1, #200
	lsls r1, r1, #4
	ldr r0, [pc, #448]
	bl 0x0200b3ec
	movs r0, #128
	movs r1, #1
	lsls r0, r0, #9
	bl 0x0200b5ec
	movs r0, #60
	bl 0x0200b5f4
	movs r0, #30
	bl 0x0200b4cc
	ldr r0, [pc, #424]
	bl 0x0200b624
	ldr r0, [pc, #420]
	bl 0x0200b3f4
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r0, r0, #9
	lsls r1, r1, #9
	lsls r2, r2, #9
	bl 0x0200b494
	movs r5, #15
.L_02001c46_1:
	movs r0, #0
	bl 0x0200a84c
	adds r0, r5, #0
	bl 0x0200b3e4
	movs r0, #1
	bl 0x0200a84c
	adds r0, r5, #0
	bl 0x0200b3e4
	movs r0, #1
	subs r5, #1
	negs r0, r0
	cmp r5, r0
	bne .L_02001c46_1
	movs r0, #0
	bl 0x0200a84c
	ldr r2, [pc, #360]
	movs r3, #1
	add r2, r9
	str r3, [r2]
	adds r0, r5, #0
	ldr r2, [pc, #356]
	adds r1, r5, #0
	bl 0x0200b494
	bl 0x0200b49c
	movs r1, #3
	movs r0, #15
	bl 0x0200b544
	ldr r0, [pc, #320]
	bl 0x0200b3f4
	movs r0, #1
	bl 0x0200b3e4
	movs r1, #0
	movs r0, #15
	bl 0x0200b56c
	movs r0, #60
	bl 0x0200b4cc
	movs r1, #0
	movs r0, #15
	bl 0x0200b58c
	movs r0, #40
	bl 0x0200b4cc
	movs r1, #129
	movs r0, #0
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200b5ac
	movs r1, #129
	movs r2, #0
	lsls r1, r1, #1
	movs r0, #1
	bl 0x0200b5ac
	movs r0, #60
	bl 0x0200b4cc
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #9
	lsls r1, r1, #6
	bl 0x0200b5bc
	movs r0, #218
	movs r2, #181
	movs r3, #1
	lsls r0, r0, #16
	adds r1, r5, #0
	lsls r2, r2, #16
	bl 0x0200b5c4
	movs r1, #128
	movs r2, #128
	movs r0, #15
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200b4fc
	movs r1, #169
	movs r2, #151
	movs r0, #15
	bl 0x0200b50c
	movs r0, #15
	bl 0x0200b52c
	movs r0, #10
	bl 0x0200b4cc
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #5
	movs r0, #15
	bl 0x0200b59c
	movs r0, #40
	bl 0x0200b4cc
	movs r1, #0
	movs r0, #15
	bl 0x0200b58c
	movs r0, #20
	bl 0x0200b4cc
	movs r1, #160
	movs r0, #0
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200b59c
	movs r1, #208
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #1
	bl 0x0200b59c
	movs r0, #40
	bl 0x0200b4cc
	movs r1, #240
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #15
	bl 0x0200b59c
	movs r0, #10
	bl 0x0200b4cc
	movs r1, #224
	movs r0, #0
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200b59c
	movs r1, #176
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200b59c
	movs r0, #224
	movs r2, #158
	lsls r2, r2, #16
	movs r3, #1
	adds r1, r5, #0
	lsls r0, r0, #16
	bl 0x0200b5c4
	bl 0x0200b5cc
	movs r0, #0
	bl 0x0200a9a4
	movs r1, #2
	movs r0, #15
	bl 0x0200b55c
	movs r0, #20
	bl 0x0200b4cc
	movs r1, #0
	movs r0, #15
	bl 0x0200b58c
	movs r0, #40
	bl 0x0200b4cc
	movs r1, #0
	movs r0, #15
	bl 0x0200b584
	movs r0, #0
	movs r1, #0
	bl 0x0200b4e4
	cmp r0, #0
	bne .L_02001c46_2
	movs r0, #40
	bl 0x0200b4cc
	b .L_02001c46_3
	.2byte 0x0000
	.4byte 0x0200b00d
	.4byte 0x00000121
	.4byte 0x0200a93d
	.4byte 0x0000040c
	.4byte 0x0000e666
.L_02001c46_2:
	ldr r0, [pc, #996]
	movs r1, #1
	bl 0x0200b4ac
	movs r0, #40
	bl 0x0200b4cc
.L_02001c46_3:
	movs r0, #0
	bl 0x0200b4ec
	movs r3, #144
	ldr r2, [r0, #12]
	lsls r3, r3, #14
	ldr r1, [r0, #8]
	adds r2, r2, r3
	ldr r3, [r0, #16]
	movs r0, #22
	bl 0x0200b454
	adds r7, r0, #0
	cmp r7, #0
	beq .L_02001c46_4
	movs r1, #193
	lsls r1, r1, #3
	movs r0, #17
	bl 0x0200b414
	ldr r6, [r7, #80]
	adds r2, r6, #0
	movs r3, #0
	adds r2, #38
	strb r3, [r2]
	adds r2, #1
	strb r3, [r2]
	ldrb r2, [r6, #5]
	subs r3, #33
	ands r3, r2
	ldrb r2, [r6, #9]
	strb r3, [r6, #5]
	movs r3, #15
	ands r3, r2
	movs r2, #13
	negs r2, r2
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	adds r5, r0, #0
	strb r3, [r6, #9]
	movs r0, #222
	bl 0x0200b4b4
	movs r3, #128
	lsls r3, r3, #3
	adds r5, r5, r3
	adds r2, r5, #0
	movs r1, #128
	ldrb r0, [r6, #28]
	bl 0x0200b434
	movs r0, #17
	bl 0x0200b424
	movs r0, #0
	movs r1, #28
	bl 0x0200b53c
	adds r0, r7, #0
	movs r1, #3
	bl 0x0200b60c
	movs r0, #0
	movs r1, #28
	bl 0x0200b53c
.L_02001c46_4:
	movs r0, #1
	movs r1, #20
	movs r2, #0
	bl 0x0200ad48
	ldr r0, [pc, #844]
	movs r6, #128
	movs r5, #0
	mov r8, r0
	lsls r6, r6, #9
	mov r0, r10
	bl 0x0200ad94
	movs r0, #1
	bl 0x0200b3e4
	mov r0, r10
	bl 0x0200ad94
	movs r0, #1
	bl 0x0200b3e4
	mov r3, r8
	str r3, [r7, #24]
	str r3, [r7, #28]
.L_0200222a:
	mov r0, r10
	bl 0x0200ad94
	movs r0, #1
	bl 0x0200b3e4
	mov r0, r10
	bl 0x0200ad94
	adds r5, #1
	movs r0, #1
	bl 0x0200b3e4
	str r6, [r7, #24]
	str r6, [r7, #28]
	cmp r5, #23
	bls 0x0200a20c
	movs r0, #15
	movs r1, #0
	bl 0x0200b56c
	movs r1, #20
	movs r2, #0
	movs r0, #0
	bl 0x0200ad48
	ldr r0, [pc, #756]
	bl 0x0200b57c
	movs r0, #15
	movs r1, #0
	movs r2, #20
	bl 0x0200b594
	cmp r7, #0
	beq .L_0200222a_0
	adds r0, r7, #0
	bl 0x0200b45c
.L_0200222a_0:
	movs r1, #1
	movs r0, #0
	bl 0x0200b53c
	movs r0, #20
	bl 0x0200b4cc
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200b59c
	movs r1, #160
	movs r2, #60
	movs r0, #1
	lsls r1, r1, #8
	bl 0x0200b59c
	movs r1, #0
	movs r0, #15
	bl 0x0200b58c
	movs r0, #40
	bl 0x0200b4cc
	movs r1, #128
	movs r2, #0
	movs r0, #15
	lsls r1, r1, #7
	bl 0x0200b59c
	movs r6, #232
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #11
	lsls r1, r1, #8
	lsls r6, r6, #1
	bl 0x0200b5bc
	adds r1, r6, #0
	movs r0, #232
	bl 0x0200ac1c
	ldr r5, [pc, #644]
	movs r0, #1
	bl 0x0200a9a4
	movs r0, #15
	movs r1, #0
	bl 0x0200b58c
	adds r0, r5, #0
	movs r1, #144
	bl 0x0200ac1c
	movs r0, #2
	bl 0x0200a9a4
	movs r0, #15
	movs r1, #0
	bl 0x0200b58c
	adds r0, r5, #0
	adds r1, r6, #0
	bl 0x0200ac1c
	movs r0, #3
	bl 0x0200a9a4
	movs r0, #15
	movs r1, #0
	bl 0x0200b58c
	movs r1, #128
	movs r0, #15
	lsls r1, r1, #5
	movs r2, #0
	bl 0x0200b59c
	ldr r2, [pc, #576]
	ldr r1, [pc, #580]
	movs r0, #1
	bl 0x0200b534
	movs r0, #20
	bl 0x0200b4cc
	movs r0, #1
	movs r1, #0
	bl 0x0200b58c
	movs r1, #231
	movs r2, #180
	movs r0, #1
	lsls r1, r1, #16
	lsls r2, r2, #16
	bl 0x0200b534
	movs r1, #176
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #1
	bl 0x0200b59c
	movs r0, #20
	bl 0x0200b3e4
	movs r0, #219
	movs r1, #171
	bl 0x0200ac1c
	movs r1, #0
	movs r0, #15
	bl 0x0200b58c
	movs r0, #10
	bl 0x0200b4cc
	movs r0, #0
	movs r1, #2
	bl 0x0200b554
	movs r0, #1
	movs r1, #2
	bl 0x0200b55c
	movs r1, #0
	movs r0, #15
	bl 0x0200b58c
	movs r0, #20
	bl 0x0200b4cc
	movs r1, #0
	movs r0, #15
	bl 0x0200b58c
	movs r0, #40
	bl 0x0200b4cc
	ldr r2, [pc, #464]
	movs r3, #0
	add r2, r9
	str r3, [r2]
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #9
	lsls r0, r0, #11
	lsls r1, r1, #11
	bl 0x0200b494
	movs r1, #1
	ldr r0, [pc, #440]
	bl 0x0200b5ec
	movs r0, #20
	bl 0x0200b5f4
	movs r0, #20
	bl 0x0200b4cc
	movs r0, #107
	bl 0x0200b624
	movs r1, #200
	lsls r1, r1, #4
	ldr r0, [pc, #416]
	bl 0x0200b3ec
	movs r0, #20
	bl 0x0200b4cc
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #9
	lsls r1, r1, #6
	bl 0x0200b5bc
	movs r0, #184
	movs r1, #1
	movs r2, #132
	movs r3, #1
	lsls r0, r0, #16
	negs r1, r1
	lsls r2, r2, #16
	bl 0x0200b5c4
	movs r0, #0
	movs r1, #6
	movs r2, #0
	bl 0x0200b54c
	movs r1, #6
	movs r2, #0
	movs r0, #1
	bl 0x0200b54c
	movs r0, #0
	bl 0x0200b4ec
	adds r0, #90
	ldrb r2, [r0]
	movs r5, #254
	adds r3, r5, #0
	ands r3, r2
	strb r3, [r0]
	movs r0, #1
	bl 0x0200b4ec
	adds r0, #90
	ldrb r3, [r0]
	ands r5, r3
	strb r5, [r0]
	movs r1, #245
	movs r0, #0
	movs r2, #145
	bl 0x0200b50c
	movs r1, #215
	movs r2, #168
	movs r0, #1
	bl 0x0200b50c
	movs r0, #1
	bl 0x0200b52c
	movs r0, #80
	bl 0x0200b4cc
	movs r0, #0
	bl 0x0200b4ec
	adds r0, #90
	ldrb r3, [r0]
	movs r5, #1
	orrs r3, r5
	strb r3, [r0]
	movs r0, #1
	bl 0x0200b4ec
	adds r0, #90
	ldrb r3, [r0]
	orrs r5, r3
	strb r5, [r0]
	movs r1, #184
	movs r2, #87
	movs r0, #15
	bl 0x0200b50c
	movs r0, #15
	bl 0x0200b52c
	movs r1, #128
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #15
	bl 0x0200b59c
	movs r0, #20
	bl 0x0200b4cc
	movs r0, #15
	movs r1, #2
	bl 0x0200b544
	movs r5, #0
.L_0200222a_1:
	mov r0, r10
	bl 0x0200ad94
	adds r5, #1
	movs r0, #1
	bl 0x0200b3e4
	cmp r5, #39
	bls .L_0200222a_1
	movs r1, #200
	lsls r1, r1, #4
	ldr r0, [pc, #200]
	bl 0x0200b3ec
	movs r0, #128
	movs r1, #1
	lsls r0, r0, #9
	bl 0x0200b5ec
	movs r0, #60
	bl 0x0200b5f4
	movs r0, #30
	bl 0x0200b4cc
	ldr r0, [pc, #176]
	bl 0x0200b624
	ldr r0, [pc, #160]
	bl 0x0200b3f4
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r0, r0, #10
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl 0x0200b494
	movs r5, #7
.L_0200222a_2:
	movs r0, #0
	bl 0x0200a8dc
	adds r0, r5, #0
	bl 0x0200b3e4
	movs r0, #1
	bl 0x0200a8dc
	adds r0, r5, #0
	bl 0x0200b3e4
	movs r0, #1
	subs r5, #1
	negs r0, r0
	cmp r5, r0
	bne .L_0200222a_2
	movs r0, #0
	bl 0x0200a8dc
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #9
	lsls r0, r0, #9
	lsls r1, r1, #9
	bl 0x0200b494
	movs r1, #3
	movs r0, #15
	bl 0x0200b544
	ldr r0, [pc, #76]
	bl 0x0200b3f4
	movs r0, #1
	bl 0x0200b3e4
	movs r1, #0
	movs r0, #15
	bl 0x0200b56c
	movs r0, #60
	bl 0x0200b4cc
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #9
	lsls r0, r0, #11
	lsls r1, r1, #11
	bl 0x0200b494
	b .L_0200222a_3
	.2byte 0x110c
	.2byte 0x0000
	.2byte 0x6666
	.2byte 0x0000
	.4byte 0x0000110d
	.4byte 0x000002c7
	.4byte 0x01590000
	.4byte 0x02460000
	.4byte 0x0000040c
	.4byte 0x0020119e
	.4byte 0x0200a971
	.4byte 0x0200b00d
	.4byte 0x00000121
.L_0200222a_3:
	movs r1, #1
	ldr r0, [pc, #636]
	bl 0x0200b5ec
	movs r0, #20
	bl 0x0200b5f4
	movs r0, #20
	bl 0x0200b4cc
	movs r0, #107
	bl 0x0200b624
	movs r1, #200
	lsls r1, r1, #4
	ldr r0, [pc, #612]
	bl 0x0200b3ec
	movs r0, #40
	bl 0x0200b4cc
	movs r0, #15
	movs r1, #127
	movs r2, #110
	bl 0x0200b514
	movs r1, #128
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #15
	bl 0x0200b59c
	movs r0, #20
	bl 0x0200b4cc
	movs r0, #15
	movs r1, #2
	bl 0x0200b544
	movs r5, #0
.L_0200222a_4:
	mov r0, r10
	bl 0x0200ad94
	adds r5, #1
	movs r0, #1
	bl 0x0200b3e4
	cmp r5, #39
	bls .L_0200222a_4
	movs r1, #200
	lsls r1, r1, #4
	ldr r0, [pc, #544]
	bl 0x0200b3ec
	movs r0, #128
	movs r1, #1
	lsls r0, r0, #9
	bl 0x0200b5ec
	movs r0, #60
	bl 0x0200b5f4
	ldr r0, [pc, #524]
	bl 0x0200b624
	movs r0, #30
	bl 0x0200b4cc
	ldr r0, [pc, #504]
	bl 0x0200b3f4
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r0, r0, #10
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl 0x0200b494
	movs r5, #7
.L_0200222a_5:
	movs r0, #0
	bl 0x0200a84c
	adds r0, r5, #0
	bl 0x0200b3e4
	movs r0, #1
	bl 0x0200a84c
	adds r0, r5, #0
	bl 0x0200b3e4
	movs r3, #1
	subs r5, #1
	negs r3, r3
	cmp r5, r3
	bne .L_0200222a_5
	movs r0, #0
	bl 0x0200a84c
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #9
	lsls r2, r2, #9
	lsls r0, r0, #9
	bl 0x0200b494
	movs r0, #107
	bl 0x0200b624
	movs r0, #63
	bl 0x0200b624
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #9
	lsls r0, r0, #11
	lsls r1, r1, #11
	bl 0x0200b494
	movs r1, #1
	ldr r0, [pc, #392]
	bl 0x0200b5ec
	movs r0, #20
	bl 0x0200b5f4
	movs r0, #20
	bl 0x0200b4cc
	movs r0, #107
	bl 0x0200b624
	movs r1, #200
	lsls r1, r1, #4
	ldr r0, [pc, #380]
	bl 0x0200b3ec
	movs r1, #3
	movs r0, #15
	bl 0x0200b544
	ldr r5, [pc, #356]
	adds r0, r5, #0
	bl 0x0200b3f4
	movs r0, #1
	bl 0x0200b3e4
	movs r1, #0
	movs r0, #15
	bl 0x0200b56c
	movs r0, #60
	bl 0x0200b4cc
	movs r0, #15
	movs r1, #184
	movs r2, #87
	bl 0x0200b514
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #7
	movs r0, #15
	bl 0x0200b59c
	movs r0, #10
	bl 0x0200b4cc
	movs r1, #3
	movs r0, #15
	bl 0x0200b544
	adds r0, r5, #0
	bl 0x0200b3f4
	movs r0, #1
	bl 0x0200b3e4
	movs r1, #0
	movs r0, #15
	bl 0x0200b56c
	movs r0, #141
	bl 0x0200b624
	movs r0, #100
	bl 0x0200b4cc
	movs r1, #129
	movs r0, #0
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200b5ac
	movs r1, #129
	movs r2, #0
	lsls r1, r1, #1
	movs r0, #1
	bl 0x0200b5ac
	movs r0, #60
	bl 0x0200b4cc
	movs r0, #15
	movs r1, #0
	bl 0x0200b58c
	movs r0, #1
	movs r1, #3
	bl 0x0200b55c
	movs r1, #0
	movs r0, #1
	bl 0x0200b58c
	movs r0, #20
	bl 0x0200b4cc
	ldr r0, [pc, #204]
	bl 0x0200b624
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #6
	movs r0, #15
	bl 0x0200b59c
	movs r0, #20
	bl 0x0200b4cc
	movs r0, #15
	movs r1, #0
	bl 0x0200b58c
	movs r0, #20
	bl 0x0200b4cc
	movs r5, #0
.L_0200222a_6:
	mov r0, r10
	bl 0x0200ad94
	adds r5, #1
	movs r0, #1
	bl 0x0200b3e4
	cmp r5, #39
	bls .L_0200222a_6
	ldr r5, [pc, #144]
	movs r1, #200
	lsls r1, r1, #4
	adds r0, r5, #0
	bl 0x0200b3ec
	movs r0, #20
	bl 0x0200b4cc
	movs r1, #2
	ldr r0, [pc, #136]
	bl 0x0200b5ec
	movs r0, #60
	bl 0x0200b5f4
	movs r0, #100
	bl 0x0200b3e4
	movs r1, #1
	ldr r0, [pc, #116]
	bl 0x0200b5ec
	movs r0, #60
	bl 0x0200b5f4
	movs r0, #60
	bl 0x0200b3e4
	adds r0, r5, #0
	bl 0x0200b3f4
	ldr r2, [pc, #96]
	movs r3, #1
	add r2, r9
	movs r0, #1
	movs r1, #1
	str r3, [r2]
	negs r1, r1
	ldr r2, [pc, #84]
	negs r0, r0
	bl 0x0200b494
	bl 0x0200b49c
	bl 0x0200b000
	ldr r0, [pc, #72]
	bl 0x0200b4c4
	ldr r0, [pc, #72]
	bl 0x0200b4c4
	movs r0, #5
	bl 0x0200b5dc
	movs r0, #128
	lsls r0, r0, #1
	bl 0x0200b4c4
	sub sp, #-8
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0020119e
	.4byte 0x0200a93d
	.4byte 0x0200b00d
	.4byte 0x00000121
	.4byte 0x0200a971
	.4byte 0x00007fff
	.4byte 0x0000040c
	.4byte 0x0000e666
	.4byte 0x00000814
	.4byte 0x0000083f
	.global Func_02002820
	.thumb_func
Func_02002820:
	push {r5, r6, lr}
	adds r5, r0, #0
	bl 0x0200b4ec
	movs r1, #232
	movs r2, #250
	adds r6, r0, #0
	lsls r1, r1, #16
	adds r0, r5, #0
	lsls r2, r2, #15
	bl 0x0200b534
	movs r3, #128
	lsls r3, r3, #7
	strh r3, [r6, #6]
	adds r0, r5, #0
	movs r1, #3
	bl 0x0200b5a4
	pop {r5, r6}
	pop {r0}
	bx r0
	.global Func_0200284c
	.thumb_func
Func_0200284c:
	push {r5, r6, lr}
	sub sp, #8
	cmp r0, #0
	beq .L_0200284c_0
	movs r5, #1
	movs r0, #8
	movs r1, #47
	movs r2, #64
	movs r3, #7
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl 0x0200b474
	movs r6, #2
	movs r0, #7
	movs r1, #48
	movs r2, #63
	movs r3, #8
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl 0x0200b474
	movs r0, #7
	movs r1, #49
	movs r2, #63
	movs r3, #9
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl 0x0200b474
	b .L_0200284c_1
.L_0200284c_0:
	movs r5, #1
	movs r0, #56
	movs r1, #0
	movs r2, #64
	movs r3, #7
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl 0x0200b474
	movs r0, #56
	movs r1, #0
	movs r2, #63
	movs r3, #8
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl 0x0200b474
	movs r3, #2
	str r3, [sp, #0]
	movs r0, #56
	movs r1, #0
	movs r2, #63
	movs r3, #9
	str r5, [sp, #4]
	bl 0x0200b474
	movs r0, #58
	movs r1, #25
	movs r2, #64
	movs r3, #8
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl 0x0200b474
.L_0200284c_1:
	bl 0x0200b464
	sub sp, #-8
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_020028dc
	.thumb_func
Func_020028dc:
	push {r5, lr}
	sub sp, #8
	cmp r0, #0
	beq .L_020028dc_0
	movs r5, #2
	movs r0, #9
	movs r1, #45
	movs r2, #65
	movs r3, #5
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl 0x0200b474
	movs r3, #1
	str r3, [sp, #0]
	movs r0, #11
	movs r1, #46
	movs r2, #67
	movs r3, #6
	str r5, [sp, #4]
	bl 0x0200b474
	b .L_020028dc_1
.L_020028dc_0:
	movs r5, #2
	movs r0, #89
	movs r1, #2
	movs r2, #65
	movs r3, #5
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl 0x0200b474
	movs r3, #1
	str r3, [sp, #0]
	movs r0, #102
	movs r1, #32
	movs r2, #67
	movs r3, #6
	str r5, [sp, #4]
	bl 0x0200b474
.L_020028dc_1:
	bl 0x0200b464
	sub sp, #-8
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_0200293c
	.thumb_func
Func_0200293c:
	push {lr}
	ldr r3, [pc, #44]
	ldr r3, [r3]
	movs r2, #1
	ands r3, r2
	cmp r3, #0
	bne .L_0200293c_0
	bl 0x0200b3fc
	movs r1, #100
	bl 0x0200b3dc
	cmp r0, #50
	bls .L_0200293c_1
	movs r0, #1
	bl 0x0200a84c
	b .L_0200293c_0
.L_0200293c_1:
	movs r0, #0
	bl 0x0200a84c
.L_0200293c_0:
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x03001e40
	.global Func_02002970
	.thumb_func
Func_02002970:
	push {lr}
	ldr r3, [pc, #44]
	ldr r3, [r3]
	movs r2, #1
	ands r3, r2
	cmp r3, #0
	bne .L_02002970_0
	bl 0x0200b3fc
	movs r1, #100
	bl 0x0200b3dc
	cmp r0, #50
	bls .L_02002970_1
	movs r0, #1
	bl 0x0200a8dc
	b .L_02002970_0
.L_02002970_1:
	movs r0, #0
	bl 0x0200a8dc
.L_02002970_0:
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x03001e40
	.global Func_020029a4
	.thumb_func
Func_020029a4:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	adds r7, r0, #0
	movs r6, #0
.L_020029a4_0:
	adds r0, r6, #0
	adds r0, #16
	adds r6, #1
	bl 0x0200b4f4
	cmp r6, #15
	bls .L_020029a4_0
	cmp r7, #1
	beq .L_020029a4_1
	cmp r7, #1
	bcc .L_020029a4_2
	cmp r7, #2
	beq .L_020029a4_3
	cmp r7, #3
	beq .L_020029a4_4
	b .L_020029a4_5
.L_020029a4_2:
	ldr r0, [pc, #424]
	b .L_020029a4_6
.L_020029a4_1:
	ldr r0, [pc, #424]
	b .L_020029a4_6
.L_020029a4_3:
	ldr r0, [pc, #424]
.L_020029a4_6:
	movs r1, #1
	bl 0x0200b5ec
	b .L_020029a4_5
.L_020029a4_4:
	ldr r0, [pc, #416]
	movs r1, #1
	bl 0x0200b5ec
.L_020029a4_5:
	movs r0, #60
	bl 0x0200b5f4
	movs r0, #214
	bl 0x0200b624
	ldr r0, [pc, #400]
	movs r6, #0
	mov r9, r6
	mov r10, r0
.L_020029a4_13:
	mov r2, r10
	ldr r1, [r2]
	movs r3, #0
	ldr r2, [r2, #4]
	cmp r7, #1
	beq .L_020029a4_7
	cmp r7, #1
	bcc .L_020029a4_8
	cmp r7, #2
	beq .L_020029a4_9
	cmp r7, #3
	beq .L_020029a4_10
	b .L_020029a4_11
.L_020029a4_8:
	movs r3, #232
	lsls r3, r3, #16
	adds r1, r1, r3
	movs r3, #144
	lsls r3, r3, #16
	b .L_020029a4_11
.L_020029a4_7:
	movs r4, #232
	lsls r4, r4, #16
	movs r3, #232
	adds r1, r1, r4
	b .L_020029a4_12
.L_020029a4_9:
	ldr r0, [pc, #348]
	movs r3, #144
	adds r1, r1, r0
	lsls r3, r3, #16
	b .L_020029a4_11
.L_020029a4_10:
	ldr r3, [pc, #340]
	adds r1, r1, r3
	movs r3, #232
.L_020029a4_12:
	lsls r3, r3, #17
.L_020029a4_11:
	ldr r4, [pc, #336]
	lsls r5, r6, #2
	mov r0, r9
	str r0, [r4, r5]
	movs r0, #142
	lsls r0, r0, #1
	mov r8, r4
	bl 0x0200b454
	ldr r3, [pc, #320]
	str r0, [r3, r5]
	adds r3, r0, #0
	mov r2, r9
	adds r3, #85
	strb r2, [r3]
	ldr r1, [r0, #80]
	adds r3, r1, #0
	adds r3, #38
	strb r2, [r3]
	movs r4, #13
	ldrb r2, [r1, #9]
	negs r4, r4
	adds r3, r4, #0
	ands r2, r3
	movs r3, #4
	orrs r2, r3
	strb r2, [r1, #9]
	movs r1, #6
	bl 0x0200b444
	movs r0, #6
	bl 0x0200b3e4
	adds r6, #1
	movs r0, #8
	add r10, r0
	cmp r6, #9
	bls .L_020029a4_13
	cmp r7, #0
	bne .L_020029a4_14
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200b5ac
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200b5ac
.L_020029a4_14:
	movs r0, #20
	bl 0x0200b3e4
	movs r1, #200
	lsls r1, r1, #4
	ldr r0, [pc, #228]
	bl 0x0200b3ec
	movs r0, #246
	movs r5, #1
	bl 0x0200b624
	mov r2, r8
	str r5, [r2]
	movs r0, #6
	bl 0x0200b3e4
	mov r3, r8
	str r5, [r3, #4]
	movs r0, #6
	bl 0x0200b3e4
	mov r4, r8
	str r5, [r4, #8]
	movs r0, #6
	bl 0x0200b3e4
	mov r0, r8
	str r5, [r0, #12]
	movs r0, #6
	bl 0x0200b3e4
	mov r2, r8
	str r5, [r2, #16]
	movs r0, #6
	bl 0x0200b3e4
	mov r3, r8
	str r5, [r3, #20]
	movs r0, #6
	bl 0x0200b3e4
	mov r4, r8
	str r5, [r4, #24]
	movs r0, #6
	bl 0x0200b3e4
	mov r0, r8
	str r5, [r0, #28]
	movs r0, #6
	bl 0x0200b3e4
	mov r2, r8
	str r5, [r2, #32]
	movs r0, #6
	bl 0x0200b3e4
	mov r3, r8
	str r5, [r3, #36]
	movs r0, #6
	bl 0x0200b3e4
	mov r5, r8
.L_020029a4_19:
	ldr r3, [r5]
	movs r6, #0
	b .L_020029a4_15
.L_020029a4_17:
	adds r6, #1
	cmp r6, #9
	bhi .L_020029a4_16
	lsls r3, r6, #2
	ldr r3, [r5, r3]
.L_020029a4_15:
	cmp r3, #0
	beq .L_020029a4_17
	movs r6, #222
	lsls r6, r6, #2
.L_020029a4_16:
	movs r4, #222
	lsls r4, r4, #2
	cmp r6, r4
	bne .L_020029a4_18
	movs r0, #1
	bl 0x0200b3e4
	b .L_020029a4_19
.L_020029a4_18:
	movs r0, #40
	bl 0x0200b3e4
	ldr r0, [pc, #68]
	bl 0x0200b3f4
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #1
	bl 0x0200b5ec
	movs r0, #40
	bl 0x0200b5f4
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x004039d2
	.4byte 0x004049d2
	.4byte 0x00404a4e
	.4byte 0x00403a52
	.4byte 0x0200b684
	.4byte 0x02c70000
	.4byte 0x0200bb40
	.4byte 0x0200bb10
	.4byte 0x0200aba1
	.global Func_02002ba0
	.thumb_func
Func_02002ba0:
	push {r5, r6, r7, lr}
	ldr r2, [pc, #104]
	ldr r3, [pc, #104]
	mov lr, r2
	movs r5, #160
	ldr r6, [pc, #104]
	movs r4, #0
	lsls r5, r5, #13
	mov r0, lr
	mov r12, r3
	movs r1, #0
.L_02002ba0_3:
	mov r7, lr
	ldr r3, [r1, r7]
	cmp r3, #0
	beq .L_02002ba0_0
	mov r7, r12
	ldr r2, [r7, r1]
	cmp r3, #8
	bhi .L_02002ba0_1
	ldr r3, [r2, #24]
	ldr r7, [pc, #76]
	adds r3, r3, r7
	str r3, [r2, #24]
	movs r7, #128
	ldr r3, [r2, #28]
	lsls r7, r7, #8
	adds r3, r3, r7
	str r3, [r2, #28]
	ldr r3, [r2, #12]
	adds r3, r3, r6
	str r3, [r2, #12]
	ldr r3, [r2, #60]
	adds r3, r3, r6
	b .L_02002ba0_2
.L_02002ba0_1:
	ldr r3, [r2, #12]
	adds r3, r3, r5
	str r3, [r2, #12]
	ldr r3, [r2, #60]
	adds r3, r3, r5
.L_02002ba0_2:
	str r3, [r2, #60]
	ldr r3, [r0, r1]
	adds r3, #1
	str r3, [r0, r1]
	cmp r3, #14
	bls .L_02002ba0_0
	movs r3, #0
	str r3, [r0, r1]
.L_02002ba0_0:
	adds r4, #1
	adds r1, #4
	cmp r4, #9
	bls .L_02002ba0_3
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.4byte 0x0200bb40
	.4byte 0x0200bb10
	.4byte 0x00004ccc
	.4byte 0xffffe334
	.global Func_02002c1c
	.thumb_func
Func_02002c1c:
	push {r5, r6, lr}
	mov r6, r8
	push {r6}
	mov r8, r1
	adds r6, r0, #0
	bl 0x0200b5d4
	mov r3, r8
	lsls r3, r3, #16
	mov r8, r3
	lsls r6, r6, #16
	movs r1, #1
	mov r2, r8
	adds r5, r0, #0
	movs r3, #1
	adds r0, r6, #0
	negs r1, r1
	bl 0x0200b5c4
	movs r1, #0
	movs r0, #0
	bl 0x0200b5ec
	movs r0, #20
	bl 0x0200b5f4
	movs r0, #40
	bl 0x0200b3e4
	mov r3, r8
	str r3, [r5, #16]
	movs r3, #128
	lsls r3, r3, #24
	str r3, [r5, #56]
	str r3, [r5, #64]
	movs r3, #0
	str r3, [r5, #36]
	str r3, [r5, #44]
	movs r0, #5
	str r6, [r5, #8]
	bl 0x0200b3e4
	bl 0x0200b464
	movs r0, #5
	bl 0x0200b3e4
	movs r0, #128
	movs r1, #0
	lsls r0, r0, #9
	bl 0x0200b5ec
	movs r0, #20
	bl 0x0200b5f4
	movs r0, #30
	bl 0x0200b3e4
	pop {r3}
	mov r8, r3
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02002c9c
	.thumb_func
Func_02002c9c:
	push {r5, r6, lr}
	mov r6, r8
	push {r6}
	sub sp, #8
	movs r3, #10
	str r3, [sp, #0]
	movs r5, #0
	movs r0, #14
	movs r1, #8
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl 0x0200b47c
	movs r3, #11
	str r3, [sp, #0]
	movs r0, #14
	movs r1, #28
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl 0x0200b47c
	movs r3, #12
	str r3, [sp, #0]
	movs r0, #44
	movs r1, #8
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl 0x0200b47c
	movs r3, #13
	str r3, [sp, #0]
	movs r0, #44
	movs r1, #28
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl 0x0200b47c
	movs r3, #8
	str r3, [sp, #4]
	movs r5, #14
	mov r8, r3
	movs r0, #13
	movs r1, #8
	movs r2, #1
	movs r3, #1
	str r5, [sp, #0]
	bl 0x0200b47c
	movs r6, #28
	movs r0, #13
	movs r1, #28
	movs r2, #1
	movs r3, #1
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl 0x0200b47c
	mov r3, r8
	str r3, [sp, #4]
	movs r5, #44
	movs r0, #43
	movs r1, #8
	movs r2, #1
	movs r3, #1
	str r5, [sp, #0]
	bl 0x0200b47c
	movs r0, #43
	movs r1, #28
	movs r2, #1
	movs r3, #1
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl 0x0200b47c
	sub sp, #-8
	pop {r3}
	mov r8, r3
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02002d48
	.thumb_func
Func_02002d48:
	push {r5, r6, lr}
	adds r6, r1, #0
	adds r5, r2, #0
	cmp r0, #1
	bne .L_02002d48_0
	movs r0, #154
	lsls r0, r0, #1
	bl 0x0200b624
	ldr r0, [pc, #48]
	movs r1, #1
	bl 0x0200b5ec
	b .L_02002d48_1
.L_02002d48_0:
	ldr r0, [pc, #40]
	bl 0x0200b624
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #1
	bl 0x0200b5ec
.L_02002d48_1:
	adds r0, r6, #0
	bl 0x0200b5f4
	cmp r5, #0
	beq .L_02002d48_2
	adds r0, r5, #0
	bl 0x0200b4cc
.L_02002d48_2:
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00203a52
	.4byte 0x00000121
	.global Func_02002d94
	.thumb_func
Func_02002d94:
	push {r5, lr}
	ldr r3, [pc, #52]
	ldr r3, [r3]
	movs r2, #2
	ands r3, r2
	adds r5, r0, #0
	cmp r3, #0
	beq .L_02002d94_0
	movs r1, #7
	bl 0x0200b4a4
	b .L_02002d94_1
.L_02002d94_0:
	adds r0, r5, #0
	movs r1, #0
	bl 0x0200b4a4
.L_02002d94_1:
	ldr r3, [pc, #20]
	ldr r3, [r3]
	movs r2, #15
	ands r3, r2
	cmp r3, #0
	bne .L_02002d94_2
	adds r0, r5, #0
	bl 0x0200aeb0
.L_02002d94_2:
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0x03001e40
	.global Func_02002dd0
	.thumb_func
Func_02002dd0:
	push {r5, r6, lr}
	ldr r5, [pc, #52]
	ldr r3, [r5]
	movs r2, #1
	ands r3, r2
	adds r6, r0, #0
	cmp r3, #0
	beq .L_02002dd0_0
	ldr r0, [r5]
	movs r1, #6
	lsrs r0, r0, #1
	bl 0x0200b3dc
	adds r1, r0, #0
	adds r0, r6, #0
	bl 0x0200b4a4
.L_02002dd0_0:
	ldr r3, [r5]
	movs r2, #15
	ands r3, r2
	cmp r3, #0
	bne .L_02002dd0_1
	adds r0, r6, #0
	bl 0x0200aeb0
.L_02002dd0_1:
	pop {r5, r6}
	pop {r0}
	bx r0
	.4byte 0x03001e40
	.global Func_02002e0c
	.thumb_func
Func_02002e0c:
	push {r5, r6, lr}
	adds r5, r0, #0
	adds r2, r5, #0
	adds r2, #100
	ldrh r3, [r2]
	adds r3, #1
	ldr r6, [r5, #104]
	strh r3, [r2]
	lsls r3, r3, #16
	asrs r0, r3, #16
	cmp r0, #31
	ble .L_02002e0c_0
	adds r0, r5, #0
	bl 0x0200b45c
	b .L_02002e0c_1
.L_02002e0c_0:
	lsls r0, r0, #10
	bl 0x0200b404
	str r0, [r5, #24]
	str r0, [r5, #28]
	ldr r3, [r6, #8]
	movs r1, #128
	str r3, [r5, #8]
	ldr r3, [r5, #12]
	lsls r1, r1, #9
	adds r3, r3, r1
	str r3, [r5, #12]
	subs r1, r1, r0
	ldr r3, [r6, #16]
	lsls r2, r1, #2
	adds r2, r2, r1
	adds r3, r3, r2
	movs r2, #128
	lsls r2, r2, #12
	adds r3, r3, r2
	str r3, [r5, #16]
.L_02002e0c_1:
	pop {r5, r6}
	pop {r0}
	bx r0
	.global Func_02002e5c
	.thumb_func
Func_02002e5c:
	push {r5, r6, lr}
	adds r5, r0, #0
	adds r2, r5, #0
	adds r2, #100
	ldrh r3, [r2]
	adds r3, #1
	ldr r6, [r5, #104]
	strh r3, [r2]
	lsls r3, r3, #16
	asrs r0, r3, #16
	cmp r0, #31
	ble .L_02002e5c_0
	adds r0, r5, #0
	bl 0x0200b45c
	b .L_02002e5c_1
.L_02002e5c_0:
	lsls r0, r0, #10
	bl 0x0200b404
	negs r3, r0
	str r0, [r5, #24]
	str r3, [r5, #28]
	ldr r3, [r6, #8]
	movs r1, #128
	str r3, [r5, #8]
	ldr r3, [r5, #12]
	lsls r1, r1, #9
	adds r3, r3, r1
	str r3, [r5, #12]
	subs r1, r1, r0
	ldr r3, [r6, #16]
	lsls r2, r1, #2
	adds r2, r2, r1
	subs r3, r3, r2
	movs r2, #128
	lsls r2, r2, #13
	adds r3, r3, r2
	str r3, [r5, #16]
.L_02002e5c_1:
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02002eb0
	.thumb_func
Func_02002eb0:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r3, [pc, #80]
	adds r6, r0, #0
	ldr r3, [r3]
	movs r0, #146
	lsls r0, r0, #1
	sub sp, #8
	mov r11, r3
	bl 0x0200b624
	movs r1, #63
	movs r7, #0
	mov r10, sp
	mov r9, r1
.L_02002eb0_2:
	ldr r2, [r6, #12]
	ldr r3, [r6, #16]
	ldr r1, [r6, #8]
	movs r0, #26
	bl 0x0200b454
	lsls r3, r7, #2
	mov r2, r10
	str r0, [r3, r2]
	cmp r0, #0
	beq .L_02002eb0_0
	ldr r3, [r6, #20]
	str r3, [r0, #20]
	adds r3, r0, #0
	ldr r5, [r0, #80]
	adds r3, #85
	movs r2, #0
	ldr r1, [pc, #16]
	strb r2, [r3]
	adds r3, #15
	strh r2, [r3]
	mov r8, r1
	str r6, [r0, #104]
	cmp r5, #0
	beq .L_02002eb0_0
	b .L_02002eb0_1
	.4byte 0x00000000
	.4byte 0x03001f30
.L_02002eb0_1:
	movs r1, #0
	adds r0, r5, #0
	bl 0x0200b43c
	adds r3, r5, #0
	adds r3, #38
	mov r2, r8
	strb r2, [r3]
	ldrb r0, [r5, #28]
	bl 0x0200b42c
	mov r3, r11
	adds r3, #70
	ldrh r3, [r3]
	strb r3, [r5, #28]
	ldrb r3, [r5, #29]
	movs r2, #1
	orrs r3, r2
	strb r3, [r5, #29]
	ldrb r3, [r5, #28]
	ldr r2, [pc, #64]
	lsls r3, r3, #2
	adds r3, r3, r2
	ldrh r1, [r3, #2]
	ldr r2, [pc, #52]
	ldrh r3, [r5, #8]
	lsls r1, r1, #17
	lsrs r1, r1, #22
	ands r3, r2
	orrs r3, r1
	movs r1, #33
	negs r1, r1
	strh r3, [r5, #8]
	ldrb r3, [r5, #5]
	adds r2, r1, #0
	ands r3, r2
	mov r2, r9
	ands r3, r2
	movs r2, #64
	orrs r3, r2
	ldrb r2, [r5, #7]
	strb r3, [r5, #5]
	mov r3, r9
	ands r3, r2
	movs r2, #128
	orrs r3, r2
	strb r3, [r5, #7]
	ldr r3, [r5, #40]
	mov r1, r8
	strb r1, [r3, #22]
	b .L_02002eb0_0
	.2byte 0x0000
	.4byte 0xfffffc00
	.4byte 0x03001b10
.L_02002eb0_0:
	adds r7, #1
	cmp r7, #1
	ble .L_02002eb0_2
	ldr r2, [sp, #0]
	ldr r3, [pc, #88]
	movs r0, #15
	str r3, [r2, #108]
	bl 0x0200b4ec
	ldr r3, [sp, #0]
	ldr r4, [r3, #80]
	ldr r3, [r0, #80]
	movs r5, #13
	ldrb r3, [r3, #9]
	ldrb r1, [r4, #9]
	negs r5, r5
	movs r2, #12
	ands r2, r3
	adds r3, r5, #0
	ands r3, r1
	orrs r3, r2
	strb r3, [r4, #9]
	movs r0, #15
	bl 0x0200b4ec
	mov r2, r10
	ldr r1, [r2, #4]
	ldr r3, [r0, #80]
	ldr r4, [r1, #80]
	ldrb r2, [r3, #9]
	movs r3, #12
	ands r3, r2
	ldrb r2, [r4, #9]
	ands r5, r2
	orrs r5, r3
	ldr r3, [pc, #32]
	str r3, [r1, #108]
	adds r1, #35
	movs r3, #2
	strb r5, [r4, #9]
	strb r3, [r1]
	sub sp, #-8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.4byte 0x0200ae5d
	.4byte 0x0200ae0d
	.global Func_02002ff0
	.thumb_func
Func_02002ff0:
	push {lr}
	movs r0, #140
	movs r1, #0
	bl 0x0200b614
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02003000
	.thumb_func
Func_02003000:
	push {lr}
	bl 0x0200b61c
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_0200300c
	.thumb_func
Func_0200300c:
	push {lr}
	movs r0, #15
	bl 0x0200b4ec
	bl 0x0200add0
	pop {r0}
	bx r0
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r1, #202
	lsls	r1, r1, #1
	movs	r0, #33
	sub	sp, #68
	bl 0x0200b41c
	str	r0, [sp, #64]
	str	r0, [sp, #60]
	ldr	r1, [sp, #64]
	movs	r0, #0
	movs	r2, #200
	str	r0, [sp, #56]
	lsls	r2, r2, #1
	adds	r3, r1, r2
	ldrh	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L_0200304e
	b.n	.L_020032e2
.L_0200304e:
	adds	r1, #8
	ldr	r3, [sp, #64]
	ldr	r4, [sp, #64]
	str	r0, [sp, #8]
	ldr	r0, [pc, #668]
	mov	sl, r1
	ldr	r1, [pc, #668]
	adds	r3, #36
	adds	r4, #37
	adds	r0, #1
	str	r3, [sp, #16]
	str	r4, [sp, #12]
	str	r0, [sp, #4]
	str	r1, [sp, #0]
.L_0200306a:
	mov	r3, sl
	ldr	r3, [r3, #8]
	ldr	r2, [sp, #60]
	ldr	r5, [r2, #0]
	str	r3, [sp, #52]
	mov	r4, sl
	ldr	r4, [r4, #12]
	str	r4, [sp, #48]
	mov	r0, sl
	ldr	r0, [r0, #16]
	str	r0, [sp, #44]
	mov	r1, sl
	ldr	r1, [r1, #20]
	str	r1, [sp, #40]
	mov	r2, sl
	ldr	r2, [r2, #24]
	ldr	r4, [sp, #60]
	str	r2, [sp, #36]
	ldr	r3, [sp, #12]
	ldr	r4, [r4, #4]
	ldrb	r3, [r3, #0]
	ldr	r0, [sp, #60]
	str	r4, [sp, #28]
	ldr	r0, [r0, #8]
	ldr	r2, [sp, #60]
	str	r0, [sp, #24]
	ldr	r2, [r2, #12]
	mov	fp, r3
	str	r2, [sp, #20]
	ldr	r3, [sp, #16]
	ldrb	r3, [r3, #0]
	str	r3, [sp, #32]
	adds	r3, #255
	lsls	r3, r3, #24
	lsrs	r3, r3, #24
	mov	r1, fp
	str	r3, [sp, #32]
	cmp	r3, #0
	beq.n	.L_020030ba
	b.n	.L_0200326e
.L_020030ba:
	movs	r4, #3
	str	r4, [sp, #32]
	cmp	r1, #0
	bne.n	.L_0200310e
	ldr	r0, [sp, #40]
	ldr	r2, [sp, #36]
	ldr	r4, [sp, #56]
	adds	r0, r0, r2
	str	r0, [sp, #40]
	ldr	r3, [pc, #556]
	lsls	r2, r4, #2
	ldr	r3, [r3, r2]
	cmp	r0, r3
	blt.n	.L_020030e0
	ldr	r3, [pc, #552]
	ldr	r3, [r3, r2]
	negs	r3, r3
	str	r3, [sp, #36]
	b.n	.L_02003108
.L_020030e0:
	ldr	r0, [sp, #40]
	ldr	r3, [pc, #544]
	cmp	r0, r3
	bgt.n	.L_02003108
	ldr	r3, [pc, #532]
	ldr	r4, [pc, #536]
	ldr	r3, [r3, r2]
	str	r4, [sp, #40]
	str	r3, [sp, #36]
	ldr	r2, [r5, #8]
	str	r2, [sp, #28]
	ldr	r3, [r5, #12]
	str	r3, [sp, #24]
	ldr	r4, [r5, #16]
	movs	r0, #24
	str	r4, [sp, #20]
	str	r1, [r5, #8]
	str	r1, [r5, #12]
	str	r1, [r5, #16]
	mov	fp, r0
.L_02003108:
	ldr	r0, [sp, #40]
	str	r0, [r5, #24]
	str	r0, [r5, #28]
.L_0200310e:
	bl 0x0200b3fc
	ldr	r2, [pc, #480]
	ldr	r1, [sp, #8]
	ldrb	r3, [r1, r2]
	muls	r3, r0
	lsrs	r6, r3, #16
	bl 0x0200b3fc
	ldr	r4, [sp, #4]
	ldrb	r3, [r4, #0]
	muls	r3, r0
	lsrs	r7, r3, #16
	bl 0x0200b3fc
	ldr	r1, [sp, #4]
	ldrb	r3, [r1, #1]
	muls	r3, r0
	lsrs	r3, r3, #16
	mov	r8, r3
	cmp	r6, #0
	beq.n	.L_02003148
	movs	r1, #250
	lsls	r0, r6, #16
	lsls	r1, r1, #2
	bl 0x0200b3d4
	adds	r6, r0, #0
	b.n	.L_0200314a
.L_02003148:
	movs	r6, #0
.L_0200314a:
	cmp	r7, #0
	beq.n	.L_0200315c
	movs	r1, #250
	lsls	r0, r7, #16
	lsls	r1, r1, #2
	bl 0x0200b3d4
	mov	r9, r0
	b.n	.L_02003160
.L_0200315c:
	movs	r2, #0
	mov	r9, r2
.L_02003160:
	mov	r3, r8
	cmp	r3, #0
	beq.n	.L_02003172
	movs	r1, #250
	lsls	r0, r3, #16
	lsls	r1, r1, #2
	bl 0x0200b3d4
	b.n	.L_02003174
.L_02003172:
	movs	r0, #0
.L_02003174:
	ldr	r2, [pc, #400]
	ldr	r4, [sp, #8]
	ldrsb	r3, [r2, r4]
	cmp	r3, #1
	bne.n	.L_02003186
	ldr	r1, [sp, #52]
	adds	r1, r1, r6
	str	r1, [sp, #52]
	b.n	.L_02003198
.L_02003186:
	ldr	r4, [sp, #52]
.L_02003188:
	movs	r1, #1
	subs	r4, r4, r6
	negs	r1, r1
	str	r4, [sp, #52]
	cmp	r3, r1
	beq.n	.L_02003198
	movs	r3, #0
	str	r3, [sp, #52]
.L_02003198:
	ldr	r3, [sp, #8]
	adds	r3, #1
	ldrsb	r3, [r2, r3]
	cmp	r3, #1
	bne.n	.L_020031aa
	ldr	r4, [sp, #48]
	add	r4, r9
	str	r4, [sp, #48]
	b.n	.L_020031be
.L_020031aa:
	ldr	r1, [sp, #48]
	mov	r4, r9
	subs	r1, r1, r4
	str	r1, [sp, #48]
	movs	r1, #1
	negs	r1, r1
	cmp	r3, r1
	beq.n	.L_020031be
	movs	r3, #0
	str	r3, [sp, #48]
.L_020031be:
	ldr	r3, [sp, #8]
	adds	r3, #2
	ldrsb	r3, [r2, r3]
	cmp	r3, #1
	bne.n	.L_020031d0
	ldr	r4, [sp, #44]
	adds	r4, r4, r0
	str	r4, [sp, #44]
	b.n	.L_020031e2
.L_020031d0:
	ldr	r1, [sp, #44]
.L_020031d2:
	movs	r2, #1
	subs	r1, r1, r0
	negs	r2, r2
	str	r1, [sp, #44]
	cmp	r3, r2
	beq.n	.L_020031e2
	movs	r3, #0
	str	r3, [sp, #44]
.L_020031e2:
	ldr	r4, [sp, #0]
	ldr	r1, [sp, #52]
	ldrb	r3, [r4, #0]
	adds	r0, r3, #0
	muls	r0, r1
	bl 0x0200b404
	ldr	r2, [sp, #0]
.L_020031f2:
	ldr	r4, [sp, #48]
	ldrb	r3, [r2, #1]
	lsls	r6, r0, #1
	adds	r0, r3, #0
	muls	r0, r4
.L_020031fc:
	bl 0x0200b404
	lsls	r7, r0, #1
	ldr	r0, [sp, #0]
	ldr	r1, [sp, #44]
	ldrb	r3, [r0, #2]
	adds	r0, r3, #0
	muls	r0, r1
	bl 0x0200b40c
	mov	r2, fp
	lsls	r0, r0, #1
	cmp	r2, #0
	beq.n	.L_02003250
	ldr	r3, [sp, #28]
	adds	r3, r3, r6
.L_0200321c:
	str	r3, [sp, #28]
	mov	r3, fp
	ldr	r4, [sp, #24]
	ldr	r1, [sp, #20]
	adds	r3, #255
.L_02003226:
	lsls	r3, r3, #24
	adds	r4, r4, r7
	adds	r1, r1, r0
	lsrs	r3, r3, #24
	str	r4, [sp, #24]
	str	r1, [sp, #20]
	mov	fp, r3
	cmp	r3, #0
	bne.n	.L_0200326e
	ldr	r2, [sp, #28]
	mov	r3, r9
	str	r2, [r5, #8]
	str	r2, [r5, #56]
	cmp	r3, #0
	beq.n	.L_02003248
	str	r4, [r5, #12]
	str	r4, [r5, #60]
.L_02003248:
	ldr	r4, [sp, #20]
	str	r4, [r5, #16]
	str	r4, [r5, #64]
	b.n	.L_0200326e
.L_02003250:
	ldr	r3, [r5, #8]
	mov	r1, r9
	adds	r3, r3, r6
	str	r3, [r5, #8]
	str	r3, [r5, #56]
	cmp	r1, #0
	beq.n	.L_02003266
	ldr	r3, [r5, #12]
	adds	r3, r3, r7
	str	r3, [r5, #12]
	str	r3, [r5, #60]
.L_02003266:
	ldr	r3, [r5, #16]
	adds	r3, r3, r0
	str	r3, [r5, #16]
	str	r3, [r5, #64]
.L_0200326e:
	ldr	r2, [sp, #52]
	mov	r3, sl
	str	r2, [r3, #8]
	ldr	r4, [sp, #48]
	str	r4, [r3, #12]
	ldr	r0, [sp, #44]
	str	r0, [r3, #16]
	ldr	r1, [sp, #40]
	str	r1, [r3, #20]
	ldr	r2, [sp, #36]
	str	r2, [r3, #24]
	ldr	r4, [sp, #12]
	mov	r3, fp
	strb	r3, [r4, #0]
	ldr	r0, [sp, #28]
	ldr	r1, [sp, #60]
	str	r0, [r1, #4]
	ldr	r2, [sp, #24]
	mov	r3, sl
	str	r2, [r3, #0]
	ldr	r4, [sp, #20]
	add	r0, sp, #32
	str	r4, [r1, #12]
	ldrb	r0, [r0, #0]
	ldr	r1, [sp, #16]
	strb	r0, [r1, #0]
	ldr	r1, [sp, #0]
	ldr	r2, [sp, #8]
	adds	r1, #3
	adds	r2, #3
	ldr	r3, [sp, #4]
	ldr	r4, [sp, #56]
	str	r1, [sp, #0]
	str	r2, [sp, #8]
	ldr	r1, [sp, #16]
	ldr	r2, [sp, #12]
	adds	r3, #3
	adds	r4, #1
	adds	r1, #40
	adds	r2, #40
	str	r3, [sp, #4]
	str	r4, [sp, #56]
	str	r1, [sp, #16]
	str	r2, [sp, #12]
	ldr	r3, [sp, #60]
	movs	r0, #40
	adds	r3, #40
	add	sl, r0
	ldr	r4, [sp, #64]
	movs	r0, #200
	str	r3, [sp, #60]
	lsls	r0, r0, #1
	adds	r3, r4, r0
	ldrh	r3, [r3, #0]
	ldr	r1, [sp, #56]
	cmp	r1, r3
	beq.n	.L_020032e2
	b.n	.L_0200306a
.L_020032e2:
	add	sp, #68
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x0200ba0c
	.4byte 0x0200ba2a
	.4byte 0x0200ba68
	.4byte 0x0200ba90
	.4byte 0x00001999
	.2byte 0xba48
	.2byte 0x0200
	.global Func_0200330c
	.thumb_func
Func_0200330c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	mov r8, r1
	movs r1, #202
	adds r6, r0, #0
	lsls r1, r1, #1
	movs r0, #33
	sub sp, #4
	bl 0x0200b41c
	movs r3, #0
	mov r9, r0
	mov r0, sp
	str r3, [r0]
	mov r5, r9
	ldr r3, [pc, #136]
	mov r1, r9
	ldr r2, [pc, #136]
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	mov r2, r8
	cmp r2, #10
	bls .L_0200330c_0
	movs r3, #10
	mov r8, r3
.L_0200330c_0:
	movs r2, #0
	mov r3, r8
	mov r10, r2
	cmp r3, #0
	beq .L_0200330c_1
	mov r11, r2
	movs r7, #0
.L_0200330c_2:
	adds r0, r6, #0
	bl 0x0200b4ec
	ldr r3, [r0, #80]
	mov r2, r11
	adds r3, #38
	str r0, [r5]
	adds r0, #85
	strb r2, [r3]
	strb r2, [r0]
	adds r0, r6, #0
	bl 0x0200b4ec
	movs r1, #1
	bl 0x0200b48c
	ldr r2, [pc, #80]
	ldr r3, [r7, r2]
	str r3, [r5, #28]
	ldr r3, [pc, #76]
	ldr r3, [r3, r7]
	adds r2, r5, #0
	negs r3, r3
	str r3, [r5, #32]
	adds r2, #36
	movs r3, #3
	strb r3, [r2]
	movs r3, #1
	add r10, r3
	adds r7, #4
	adds r5, #40
	adds r6, #1
	cmp r10, r8
	bne .L_0200330c_2
.L_0200330c_1:
	movs r3, #200
	lsls r3, r3, #1
	add r3, r9
	mov r2, r8
	movs r1, #200
	strh r2, [r3]
	lsls r1, r1, #4
	ldr r0, [pc, #36]
	bl 0x0200b3ec
	sub sp, #-4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.4byte 0x040000d4
	.4byte 0x85000065
	.4byte 0x0200ba68
	.4byte 0x0200ba90
	.4byte 0x0200b01d
	.include "games/THE BROKEN SEAL/SRC/FIELD/SORU_FUNKA/IMPORT.INC"
	.section .rodata,"a",%progbits
	.4byte 0x00000015
	.4byte 0x0000000d
	.4byte 0x00060000
	.4byte 0x80010000
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00000a3d
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x00000a3d
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x00000012
	.4byte 0xc0010000
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00010000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00010000
	.4byte 0x00000010
	.4byte 0xfffa0000
	.4byte 0x00000000
	.4byte 0x000a0000
	.4byte 0x000a0000
	.4byte 0x00080000
	.4byte 0x00190000
	.4byte 0x00040000
	.4byte 0x001e0000
	.4byte 0xfffb0000
	.4byte 0x00140000
	.4byte 0x00020000
	.4byte 0x00050000
	.4byte 0xfffa0000
	.4byte 0x00230000
	.4byte 0xfff80000
	.4byte 0x000f0000
	.4byte 0x00020000
	.4byte 0x00280000
	.4byte 0xfffe0000
	.4byte 0x000f0000
	.4byte 0xffff0000
	.4byte 0x000001d8
	.4byte 0x40000142
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000012
	.4byte 0x0050800b
	.4byte 0x000001ff
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff0005
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff0016
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff001e
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff0020
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff0021
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff0022
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff0023
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff002b
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff00fc
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff00d8
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00d8
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00d8
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00d8
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00d8
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00d8
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00d8
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00d8
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00d8
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00d8
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00d8
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00d8
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00d8
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00d8
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00d8
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00d8
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0028003b
	.4byte 0x00040003
	.4byte 0x003e0006
	.4byte 0x00030028
	.4byte 0x00060004
	.4byte 0x00280041
	.4byte 0x00040003
	.4byte 0x00440006
	.4byte 0x00030028
	.4byte 0x00060004
	.4byte 0x00280047
	.4byte 0x00040003
	.4byte 0x004a0006
	.4byte 0x00030028
	.4byte 0x00060004
	.4byte 0x0028004d
	.4byte 0x00040003
	.4byte 0x00500006
	.4byte 0x00030028
	.4byte 0x00060004
	.4byte 0x00280053
	.4byte 0x00040003
	.4byte 0xffff0000
	.4byte 0x00000022
	.4byte 0x02008f55
	.4byte 0x00000010
	.4byte 0x04040404
	.4byte 0x00040300
	.4byte 0x04000404
	.4byte 0x04040003
	.4byte 0x00060406
	.4byte 0x03000404
	.4byte 0x02010002
	.4byte 0x01010200
	.4byte 0x02000102
	.4byte 0x01020001
	.4byte 0x00010100
	.4byte 0x02010102
	.4byte 0x01020001
	.4byte 0x00010200
	.4byte 0x02000102
	.4byte 0x01010101
	.4byte 0x00010100
	.4byte 0x0100ff01
	.4byte 0x01ff00ff
	.4byte 0x00ff0101
	.4byte 0xff00ffff
	.4byte 0xffff00ff
	.4byte 0x0000ff00
	.4byte 0x00009999
	.4byte 0x0000cccc
	.4byte 0x0000b333
	.4byte 0x00009999
	.4byte 0x0000cccc
	.4byte 0x00009999
	.4byte 0x0000b333
	.4byte 0x00009999
	.4byte 0x00009999
	.4byte 0x0000b333
	.4byte 0x0000028f
	.4byte 0x000001ca
	.4byte 0x0000028f
	.4byte 0x000001ca
	.4byte 0x0000028f
	.4byte 0x000001ca
	.4byte 0x0000028f
	.4byte 0x0000028f
	.4byte 0x0000020c
	.4byte 0x0000028f
