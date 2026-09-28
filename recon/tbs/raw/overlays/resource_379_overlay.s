.syntax unified
.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.global Overlay_02000000
Overlay_02000000:
	.include "games/THE BROKEN SEAL/SRC/FIELD/GOMA_IKE/ENTRY.INC"
	.global Func_02000030
	.thumb_func
Func_02000030:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x02008cf0
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
	.4byte 0x02008d38
	.global Func_02000044
	.thumb_func
Func_02000044:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x02008d44
	.global Func_0200004c
	.thumb_func
Func_0200004c:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x02008e94
	.global Func_02000054
	.thumb_func
Func_02000054:
	push {lr}
	ldr r3, [pc, #24]
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #15
	bne .L_02000054_0
	bl 0x02008074
.L_02000054_0:
	movs r0, #0
	pop {r1}
	bx r1
	.4byte 0x02000240
	.global Func_02000074
	.thumb_func
Func_02000074:
	push {r5, lr}
	bl 0x02008af8
	movs r0, #14
	movs r1, #0
	bl 0x02008b50
	movs r0, #15
	movs r1, #0
	bl 0x02008b50
	movs r0, #16
	movs r1, #0
	bl 0x02008b50
	movs r0, #17
	movs r1, #0
	bl 0x02008b50
	movs r0, #18
	movs r1, #0
	bl 0x02008b50
	movs r0, #19
	movs r1, #0
	bl 0x02008b50
	movs r0, #11
	ldr r1, [pc, #980]
	ldr r2, [pc, #984]
	bl 0x02008b40
	movs r1, #160
	movs r0, #11
	lsls r1, r1, #8
	movs r2, #0
	bl 0x02008b98
	movs r1, #128
	movs r2, #250
	movs r0, #12
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl 0x02008b40
	movs r1, #160
	movs r2, #0
	movs r0, #12
	lsls r1, r1, #8
	bl 0x02008b98
	ldr r1, [pc, #944]
.L_020000dc:
	ldr r0, [pc, #944]
	bl 0x02008be0
	bl 0x02008bd8
	movs r0, #60
	bl 0x02008ab8
	movs r0, #128
	movs r1, #1
	movs r2, #153
	lsls r0, r0, #17
	negs r1, r1
	lsls r2, r2, #18
	movs r3, #0
	bl 0x02008bb8
	bl 0x02008bc0
	bl 0x02008ac0
	ldr r3, [pc, #908]
	ldr r1, [r3]
	movs r3, #224
	lsls r3, r3, #1
	adds r2, r1, r3
	movs r3, #0
	str r3, [r2]
	movs r3, #228
	lsls r3, r3, #1
	adds r2, r1, r3
	movs r3, #32
	str r3, [r2]
	bl 0x02008be8
	ldr r0, [pc, #884]
	ldr r1, [pc, #884]
	bl 0x02008bb0
	movs r0, #128
	movs r1, #1
	movs r2, #250
	movs r3, #1
	negs r1, r1
	lsls r2, r2, #17
	lsls r0, r0, #17
	bl 0x02008bb8
	movs r0, #20
	bl 0x02008af0
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #10
	lsls r2, r2, #9
	lsls r0, r0, #9
	bl 0x02008ad0
	bl 0x02008bd8
	movs r0, #145
	bl 0x02008bf8
	movs r0, #30
	bl 0x02008af0
	bl 0x02008bd8
	movs r0, #145
	bl 0x02008bf8
	bl 0x02008bc0
	movs r0, #128
	movs r1, #192
	movs r2, #128
	lsls r1, r1, #10
	lsls r2, r2, #9
	lsls r0, r0, #10
	bl 0x02008ad0
	bl 0x02008bd8
	movs r0, #145
	bl 0x02008bf8
	movs r0, #1
	movs r1, #1
	negs r1, r1
	ldr r2, [pc, #780]
	negs r0, r0
	bl 0x02008ad0
	bl 0x02008ad8
	movs r0, #60
	bl 0x02008af0
	ldr r0, [pc, #768]
	bl 0x02008b80
	movs r1, #129
	movs r2, #0
	lsls r1, r1, #1
	movs r0, #8
	bl 0x02008ba0
	movs r0, #60
	bl 0x02008af0
	movs r0, #8
	movs r1, #0
	bl 0x02008b90
	movs r1, #160
	movs r2, #0
	lsls r1, r1, #7
	movs r0, #9
	bl 0x02008b98
	movs r0, #30
	bl 0x02008af0
	movs r1, #0
	movs r0, #9
	bl 0x02008b90
	movs r0, #30
	bl 0x02008af0
	movs r0, #11
	movs r1, #4
	bl 0x02008b58
	movs r0, #11
	movs r1, #0
	bl 0x02008b90
	movs r1, #192
	movs r0, #9
	lsls r1, r1, #6
	movs r2, #0
	bl 0x02008b98
	movs r2, #0
	movs r1, #11
	movs r0, #12
	bl 0x02008b70
	movs r0, #30
	bl 0x02008af0
	movs r0, #12
	movs r1, #4
	bl 0x02008b58
	movs r0, #12
	movs r1, #0
	bl 0x02008b90
	movs r0, #13
	movs r1, #1
	bl 0x02008b68
	movs r0, #13
	movs r1, #0
	bl 0x02008b90
	movs r2, #0
	movs r1, #13
	movs r0, #10
	bl 0x02008b70
	movs r0, #30
	bl 0x02008af0
	movs r0, #10
	movs r1, #1
	bl 0x02008b68
	movs r0, #10
	movs r1, #0
	bl 0x02008b90
	movs r2, #0
	movs r1, #10
	movs r0, #9
	bl 0x02008b70
	movs r0, #30
	bl 0x02008af0
	movs r0, #9
	movs r1, #1
	bl 0x02008b68
	movs r0, #9
	movs r1, #0
	bl 0x02008b90
	movs r2, #0
	movs r1, #9
	movs r0, #10
	bl 0x02008b70
	movs r0, #30
	bl 0x02008af0
	movs r0, #10
	movs r1, #4
	bl 0x02008b58
	movs r1, #0
	movs r0, #10
	bl 0x02008b90
	movs r0, #60
	bl 0x02008af0
	movs r0, #128
	movs r1, #192
	movs r2, #128
	lsls r1, r1, #10
	lsls r2, r2, #9
	lsls r0, r0, #10
	bl 0x02008ad0
	bl 0x02008bd8
	movs r0, #145
	bl 0x02008bf8
	movs r0, #60
	bl 0x02008af0
	movs r0, #8
	movs r1, #9
	movs r2, #0
	bl 0x02008b78
	movs r0, #10
	movs r1, #11
	movs r2, #0
	bl 0x02008b78
	movs r2, #0
	movs r0, #12
	movs r1, #13
	bl 0x02008b78
	movs r0, #8
	movs r1, #2
	bl 0x02008b60
	movs r0, #9
	movs r1, #2
	bl 0x02008b60
	movs r0, #10
	movs r1, #2
	bl 0x02008b60
	movs r0, #11
	movs r1, #2
	bl 0x02008b60
	movs r0, #12
	movs r1, #2
	bl 0x02008b60
	movs r0, #13
	movs r1, #2
	bl 0x02008b60
	movs r0, #1
	movs r1, #1
	negs r0, r0
	negs r1, r1
	ldr r2, [pc, #404]
	bl 0x02008ad0
	bl 0x02008ad8
	movs r1, #128
	movs r2, #128
	movs r0, #0
	lsls r1, r1, #8
	lsls r2, r2, #7
	bl 0x02008b10
	movs r1, #128
	movs r2, #128
	movs r0, #1
	lsls r1, r1, #8
	lsls r2, r2, #7
	bl 0x02008b10
	movs r1, #240
	movs r2, #129
	lsls r2, r2, #17
	movs r0, #0
	lsls r1, r1, #15
	bl 0x02008b48
	movs r0, #192
	movs r1, #192
	lsls r0, r0, #9
	lsls r1, r1, #6
	bl 0x02008bb0
	movs r0, #224
	movs r1, #1
	movs r2, #160
	movs r3, #1
	lsls r2, r2, #17
	negs r1, r1
	lsls r0, r0, #15
	bl 0x02008bb8
	movs r0, #40
	bl 0x02008af0
	movs r0, #0
	movs r1, #2
	bl 0x02008b50
	movs r0, #1
	movs r1, #2
	bl 0x02008b50
	movs r2, #160
	movs r0, #0
	movs r1, #120
	lsls r2, r2, #1
	bl 0x02008b28
	movs r2, #160
	lsls r2, r2, #1
	movs r0, #1
	movs r1, #104
	bl 0x02008b30
	movs r0, #0
	movs r1, #1
	bl 0x02008b50
	movs r1, #1
	movs r0, #1
	bl 0x02008b50
	bl 0x02008bc0
	movs r0, #30
	bl 0x02008af0
	movs r1, #192
	movs r0, #1
	lsls r1, r1, #6
	movs r2, #0
	bl 0x02008b98
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #1
	bl 0x02008ba0
	movs r0, #50
	bl 0x02008af0
	movs r1, #192
	movs r2, #192
	lsls r2, r2, #8
	movs r0, #1
	lsls r1, r1, #9
	bl 0x02008b10
	movs r0, #1
	movs r1, #2
	bl 0x02008b50
	movs r2, #171
	lsls r2, r2, #1
	movs r0, #1
	movs r1, #105
	bl 0x02008b30
	movs r0, #1
	movs r1, #1
	bl 0x02008b50
	movs r0, #1
	movs r1, #2
	bl 0x02008b68
	movs r1, #0
	movs r0, #1
	bl 0x02008b90
	movs r0, #10
	bl 0x02008af0
	movs r0, #0
	movs r1, #1
	bl 0x02008b60
	movs r1, #1
	movs r2, #0
	movs r0, #0
	bl 0x02008b78
	movs r0, #20
	bl 0x02008af0
	movs r1, #0
	movs r0, #1
	bl 0x02008b88
	movs r0, #0
	movs r1, #0
	bl 0x02008b08
	cmp r0, #0
	bne .L_020000dc_0
	movs r0, #60
	bl 0x02008af0
	movs r0, #0
	movs r1, #3
	bl 0x02008b50
	movs r1, #3
	movs r0, #1
	bl 0x02008b50
	movs r0, #50
	bl 0x02008af0
	movs r1, #192
	movs r2, #0
	movs r0, #0
	lsls r1, r1, #8
	bl 0x02008b98
	movs r0, #1
	movs r1, #2
	bl 0x02008b50
	movs r1, #128
	movs r2, #128
	movs r0, #1
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x02008b10
	movs r2, #160
	movs r0, #1
	movs r1, #103
	lsls r2, r2, #1
	bl 0x02008b30
	movs r0, #1
	movs r1, #1
	bl 0x02008b50
	b .L_020000dc_1
	.2byte 0x0000
	.2byte 0x0109
	.2byte 0x0000
	.2byte 0x01e7
	.2byte 0x0000
	.2byte 0x0006
	.2byte 0x0001
	.4byte 0x00010003
	.4byte 0x03001ebc
	.4byte 0x0000cccc
	.4byte 0x00001999
	.4byte 0x0000e666
	.4byte 0x00001122
.L_020000dc_0:
	movs r0, #60
	bl 0x02008af0
	movs r0, #0
	movs r1, #3
	bl 0x02008b50
	movs r1, #3
	movs r0, #1
	bl 0x02008b50
	movs r0, #50
	bl 0x02008af0
	movs r1, #192
	movs r2, #0
	movs r0, #1
	lsls r1, r1, #6
	bl 0x02008b98
	movs r0, #0
	movs r1, #2
	bl 0x02008b50
	movs r2, #170
	movs r0, #0
	movs r1, #120
	lsls r2, r2, #1
	bl 0x02008b30
	movs r0, #0
	movs r1, #1
	bl 0x02008b50
.L_020000dc_1:
	movs r0, #12
	movs r1, #0
	bl 0x02008b90
	movs r1, #129
	movs r0, #1
	lsls r1, r1, #1
	bl 0x02008ba8
	movs r1, #2
	movs r0, #1
	bl 0x02008b68
	movs r0, #40
	bl 0x02008af0
	movs r1, #160
	movs r0, #9
	lsls r1, r1, #8
	movs r2, #0
	bl 0x02008b98
	movs r1, #160
	movs r0, #11
	lsls r1, r1, #8
	movs r2, #0
	bl 0x02008b98
	movs r1, #160
	movs r0, #10
	lsls r1, r1, #8
	movs r2, #0
	bl 0x02008b98
	movs r1, #160
	movs r0, #12
	lsls r1, r1, #8
	movs r2, #0
	bl 0x02008b98
	movs r1, #160
	movs r2, #0
	movs r0, #13
	lsls r1, r1, #8
	bl 0x02008b98
	movs r0, #192
	movs r1, #192
	lsls r0, r0, #10
	lsls r1, r1, #7
	bl 0x02008bb0
	movs r1, #1
	movs r0, #10
	bl 0x02008bc8
	bl 0x02008bc0
	movs r0, #50
	bl 0x02008af0
	movs r0, #10
	movs r1, #2
	bl 0x02008b68
	movs r1, #0
	movs r0, #10
	bl 0x02008b90
	movs r0, #30
	bl 0x02008af0
	movs r0, #8
	movs r1, #1
	bl 0x02008b68
	movs r1, #0
	movs r0, #8
	bl 0x02008b90
	movs r0, #40
	bl 0x02008af0
	movs r0, #9
	movs r1, #1
	bl 0x02008b68
	movs r1, #0
	movs r0, #9
	bl 0x02008b90
	movs r0, #40
	bl 0x02008af0
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #6
	movs r2, #0
	bl 0x02008b98
	movs r1, #192
	movs r0, #1
	lsls r1, r1, #6
	movs r2, #0
	bl 0x02008b98
	movs r0, #224
	movs r1, #1
	movs r2, #160
	lsls r2, r2, #17
	movs r3, #1
	lsls r0, r0, #15
	negs r1, r1
	bl 0x02008bb8
	bl 0x02008bc0
	movs r0, #0
	movs r1, #2
	bl 0x02008b60
	movs r1, #2
	movs r0, #1
	bl 0x02008b60
	movs r0, #1
	bl 0x02008b20
	movs r0, #50
	bl 0x02008af0
	movs r0, #0
	movs r1, #3
	bl 0x02008b50
	movs r1, #3
	movs r0, #1
	bl 0x02008b50
	movs r0, #1
	bl 0x02008b20
	movs r0, #60
	bl 0x02008af0
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #9
	lsls r1, r1, #6
	bl 0x02008bb0
	movs r0, #214
	movs r1, #1
	movs r2, #236
	movs r3, #1
	lsls r0, r0, #16
	negs r1, r1
	lsls r2, r2, #17
	bl 0x02008bb8
	movs r1, #128
	movs r2, #128
	movs r0, #0
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x02008b10
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #8
	movs r0, #1
	lsls r1, r1, #9
	bl 0x02008b10
	ldr r1, [pc, #392]
	movs r0, #0
	bl 0x02008b18
	movs r0, #30
	bl 0x02008af0
	ldr r1, [pc, #384]
	movs r0, #1
	bl 0x02008b18
	movs r0, #1
	bl 0x02008b20
	movs r0, #0
	movs r1, #0
	movs r2, #0
	bl 0x02008b98
	movs r0, #1
	movs r1, #0
	movs r2, #0
	bl 0x02008b98
	bl 0x02008bc0
	movs r1, #128
	movs r0, #9
	lsls r1, r1, #8
	movs r2, #0
	bl 0x02008b98
	ldr r2, [pc, #336]
	movs r0, #8
	ldr r1, [pc, #336]
	bl 0x02008b10
	movs r0, #8
	movs r1, #2
	bl 0x02008b50
	movs r0, #8
	ldr r1, [pc, #324]
	ldr r2, [pc, #328]
	bl 0x02008b30
	ldr r2, [pc, #320]
	movs r0, #8
	movs r1, #246
	bl 0x02008b30
	movs r1, #1
	movs r0, #8
	bl 0x02008b50
	movs r0, #30
	bl 0x02008af0
	movs r0, #9
	movs r1, #1
	bl 0x02008b68
	movs r0, #9
	movs r1, #0
	bl 0x02008b90
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #0
	bl 0x02008ba0
	movs r0, #50
	bl 0x02008af0
	movs r1, #129
	movs r2, #0
	lsls r1, r1, #1
	movs r0, #1
	bl 0x02008ba0
	movs r0, #50
	bl 0x02008af0
	movs r0, #8
	movs r1, #1
	bl 0x02008b68
	movs r1, #0
	movs r0, #8
	bl 0x02008b90
	movs r0, #40
	bl 0x02008af0
	movs r1, #1
	movs r2, #0
	movs r0, #0
	bl 0x02008b78
	movs r0, #50
	bl 0x02008af0
	movs r0, #0
	movs r1, #0
	movs r2, #0
	bl 0x02008b98
	movs r2, #0
	movs r1, #0
	movs r0, #1
	bl 0x02008b98
	movs r0, #20
	bl 0x02008af0
	movs r0, #0
	movs r1, #4
	bl 0x02008b50
	movs r1, #4
	movs r0, #1
	bl 0x02008b58
	movs r0, #40
	bl 0x02008af0
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #10
	bl 0x02008ba0
	movs r0, #50
	bl 0x02008af0
	movs r1, #192
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #1
	bl 0x02008b98
	movs r0, #10
	bl 0x02008af0
	movs r1, #0
	movs r0, #10
	bl 0x02008b88
	movs r0, #0
	movs r1, #0
	bl 0x02008b08
	cmp r0, #0
	bne .L_020000dc_2
	movs r0, #40
	bl 0x02008af0
	movs r1, #9
	movs r2, #0
	movs r0, #8
	bl 0x02008b78
	movs r0, #50
	bl 0x02008af0
	movs r1, #128
	movs r0, #8
	lsls r1, r1, #8
	movs r2, #0
	bl 0x02008b98
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #9
	bl 0x02008b98
	movs r0, #40
	bl 0x02008af0
	movs r0, #9
	movs r1, #1
	bl 0x02008b68
	movs r0, #9
	movs r1, #0
	bl 0x02008b90
	ldr r3, [pc, #40]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_020000dc_3
	.4byte 0x02008c00
	.4byte 0x02008c64
	.4byte 0x00006666
	.4byte 0x0000cccc
	.4byte 0x00000109
	.4byte 0x000001c7
	.4byte 0x03001ebc
.L_020000dc_2:
	movs r0, #40
	bl 0x02008af0
	movs r1, #9
	movs r2, #0
	movs r0, #8
	bl 0x02008b78
	movs r0, #50
	bl 0x02008af0
	movs r1, #128
	movs r0, #8
	lsls r1, r1, #8
	movs r2, #0
	bl 0x02008b98
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #9
	bl 0x02008b98
	movs r0, #40
	bl 0x02008af0
	movs r0, #9
	movs r1, #1
	bl 0x02008b68
	ldr r3, [pc, #612]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	movs r0, #9
	movs r1, #0
	bl 0x02008b90
.L_020000dc_3:
	movs r0, #30
	bl 0x02008af0
	movs r2, #0
	movs r1, #0
	movs r0, #1
	bl 0x02008b98
	movs r0, #30
	bl 0x02008af0
	movs r0, #0
	movs r1, #2
	bl 0x02008b60
	movs r1, #2
	movs r0, #1
	bl 0x02008b68
	movs r0, #40
	bl 0x02008af0
	movs r0, #0
	movs r1, #4
	bl 0x02008b50
	movs r1, #4
	movs r0, #1
	bl 0x02008b58
	movs r0, #60
	bl 0x02008af0
	movs r0, #8
	movs r1, #1
	bl 0x02008b68
	movs r0, #8
	movs r1, #0
	bl 0x02008b90
	movs r2, #0
	movs r1, #9
	movs r0, #8
	bl 0x02008b70
	movs r0, #30
	bl 0x02008af0
	movs r1, #1
	movs r0, #8
	bl 0x02008b68
	movs r0, #30
	bl 0x02008af0
	movs r1, #3
	movs r0, #8
	bl 0x02008b58
	movs r0, #30
	bl 0x02008af0
	movs r1, #0
	movs r0, #8
	bl 0x02008b90
	movs r0, #20
	bl 0x02008af0
	movs r0, #9
	movs r1, #1
	bl 0x02008b68
	movs r1, #176
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #9
	bl 0x02008b98
	movs r0, #30
	bl 0x02008af0
	movs r1, #3
	movs r0, #9
	bl 0x02008b58
	movs r0, #50
	bl 0x02008af0
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #8
	bl 0x02008b98
	movs r0, #20
	bl 0x02008af0
	movs r0, #8
	movs r1, #1
	bl 0x02008b68
	movs r1, #0
	movs r0, #8
	bl 0x02008b90
	movs r0, #40
	bl 0x02008af0
	movs r2, #0
	movs r1, #9
	movs r0, #8
	bl 0x02008b78
	movs r0, #40
	bl 0x02008af0
	movs r0, #8
	movs r1, #3
	bl 0x02008b50
	movs r1, #3
	movs r0, #9
	bl 0x02008b58
	movs r0, #30
	bl 0x02008af0
	movs r1, #255
	ldr r2, [pc, #332]
	movs r0, #8
	bl 0x02008b40
	movs r0, #40
	bl 0x02008af0
	movs r1, #45
	movs r2, #11
	ldr r0, [pc, #320]
	bl 0x02008ac8
	movs r0, #188
	bl 0x02008bf8
	movs r0, #30
	bl 0x02008af0
	movs r2, #195
	movs r1, #255
	lsls r2, r2, #1
	movs r0, #8
	bl 0x02008b38
	movs r0, #20
	bl 0x02008af0
	movs r0, #9
	ldr r1, [pc, #284]
	ldr r2, [pc, #288]
	bl 0x02008b10
	movs r0, #10
	ldr r1, [pc, #276]
	ldr r2, [pc, #276]
	bl 0x02008b10
	movs r2, #195
	movs r0, #9
	movs r1, #255
	lsls r2, r2, #1
	bl 0x02008b38
	movs r2, #230
	movs r0, #10
	movs r1, #255
	lsls r2, r2, #1
	bl 0x02008b40
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #10
	bl 0x02008b98
	movs r0, #40
	bl 0x02008af0
	movs r1, #3
	movs r0, #10
	bl 0x02008b58
	movs r0, #30
	bl 0x02008af0
	movs r0, #0
	movs r1, #1
	bl 0x02008b60
	movs r1, #1
	movs r0, #1
	bl 0x02008b68
	movs r0, #40
	bl 0x02008af0
	movs r2, #195
	lsls r2, r2, #1
	movs r0, #10
	movs r1, #255
	bl 0x02008b38
	ldr r5, [pc, #184]
	movs r0, #0
	adds r1, r5, #0
	bl 0x02008b18
	movs r0, #40
	bl 0x02008af0
	adds r1, r5, #0
	movs r0, #1
	bl 0x02008b18
	movs r0, #1
	bl 0x02008b20
	movs r1, #129
	movs r0, #11
	lsls r1, r1, #1
	movs r2, #0
	bl 0x02008ba0
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #12
	bl 0x02008ba0
	movs r0, #40
	bl 0x02008af0
	movs r0, #128
	movs r1, #192
	movs r2, #128
	lsls r1, r1, #10
	lsls r2, r2, #9
	lsls r0, r0, #10
	bl 0x02008ad0
	bl 0x02008bd8
	movs r0, #145
	bl 0x02008bf8
	movs r0, #30
	bl 0x02008af0
	ldr r3, [pc, #72]
	ldr r1, [r3]
	movs r3, #224
	lsls r3, r3, #1
	adds r2, r1, r3
	movs r3, #0
	str r3, [r2]
	movs r3, #228
	lsls r3, r3, #1
	adds r2, r1, r3
	movs r3, #64
	str r3, [r2]
	bl 0x02008bf0
	movs r0, #1
	movs r1, #1
	negs r1, r1
	ldr r2, [pc, #64]
	negs r0, r0
	bl 0x02008ad0
	bl 0x02008ad8
	ldr r0, [pc, #56]
	bl 0x02008ae8
	ldr r0, [pc, #52]
	bl 0x02008ae0
	movs r0, #1
	bl 0x02008bd0
	bl 0x02008b00
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x03001ebc
	.4byte 0x000001bd
	.4byte 0x02008ea0
	.4byte 0x0000cccc
	.4byte 0x00006666
	.4byte 0x02008cb4
	.4byte 0x0000e666
	.4byte 0x0000012f
	.4byte 0x00000879
	.include "games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_FUNKA/IMPORT.INC"
	.section .rodata,"a",%progbits
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00800000
	.4byte 0x00000000
	.4byte 0x017a0000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00cb0000
	.4byte 0x00000000
	.4byte 0x017a0000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00d40000
	.4byte 0x00000000
	.4byte 0x01d80000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00d60000
	.4byte 0x00000000
	.4byte 0x01d80000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00800000
	.4byte 0x00000000
	.4byte 0x017a0000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00cb0000
	.4byte 0x00000000
	.4byte 0x017a0000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00d60000
	.4byte 0x00000000
	.4byte 0x01c70000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00ff0000
	.4byte 0x00000000
	.4byte 0x01c40000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00ff0000
	.4byte 0x00000000
	.4byte 0x01860000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.4byte 0xffff0000
	.4byte 0x0000006e
	.4byte 0x4000010e
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000f
	.4byte 0x00000067
	.4byte 0x4000011c
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00114009
	.4byte 0x000001ff
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x01020000
	.4byte 0x00004000
	.4byte 0xffff0079
	.4byte 0x00000001
	.4byte 0x01090000
	.4byte 0x00000000
	.4byte 0x01d40000
	.4byte 0x0000a000
	.4byte 0xffff0075
	.4byte 0x00000001
	.4byte 0x00f60000
	.4byte 0x00000000
	.4byte 0x01d40000
	.4byte 0x0000a000
	.4byte 0xffff001f
	.4byte 0x00000001
	.4byte 0x00f60000
	.4byte 0x00000000
	.4byte 0x01e70000
	.4byte 0x0000a000
	.4byte 0xffff002a
	.4byte 0x00000001
	.4byte 0x01090000
	.4byte 0x00000000
	.4byte 0x01dd0000
	.4byte 0x0000a000
	.4byte 0xffff0025
	.4byte 0x00000001
	.4byte 0x01000000
	.4byte 0x00000000
	.4byte 0x01ea0000
	.4byte 0x0000a000
	.4byte 0xffff007a
	.4byte 0x00000001
	.4byte 0x01090000
	.4byte 0x00000000
	.4byte 0x020b0000
	.4byte 0x0000a000
	.4byte 0xffff0065
	.4byte 0x00000001
	.4byte 0x00f50000
	.4byte 0x00000000
	.4byte 0x02140000
	.4byte 0x0000a000
	.4byte 0xffff0072
	.4byte 0x00000001
	.4byte 0x01000000
	.4byte 0x00000000
	.4byte 0x021f0000
	.4byte 0x0000a000
	.4byte 0xffff0073
	.4byte 0x00000001
	.4byte 0x011b0000
	.4byte 0x00000000
	.4byte 0x02180000
	.4byte 0x0000a000
	.4byte 0xffff0066
	.4byte 0x00000001
	.4byte 0x00eb0000
	.4byte 0x00000000
	.4byte 0x02280000
	.4byte 0x0000a000
	.4byte 0xffff006c
	.4byte 0x00000001
	.4byte 0x01090000
	.4byte 0x00000000
	.4byte 0x02310000
	.4byte 0x0000a000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x00f70000
	.4byte 0x00000000
	.4byte 0x023d0000
	.4byte 0x0000a000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00620000
	.4byte 0x00020002
	.4byte 0x00020002
	.4byte 0x00020062
	.4byte 0x00020002
	.2byte 0xffff
