@ A far-call stub, 8 bytes: it loads target's address into r4 and jumps to
@ it. Code calls a routine of another bank or overlay through its stub. A
@ stub is usually named for its target with Far added:
@ Text_DrawNumberAtOffsetFar reaches Text_DrawNumberAtOffset.
.macro overlay_veneer target
    ldr r4, .Loverlay_target\@
    bx r4
    .balign 4
.Loverlay_target\@:
    .4byte \target
.endm

@ Publish a named entry in a bank's jump table. Overlay imports use the same
@ entry form, with their local name and the bank entry they call.
.macro export name, target
    .global \name
    .thumb_func
\name:
    overlay_veneer \target
.endm
