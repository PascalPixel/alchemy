@ A stub, 8 bytes: it loads target's address into r4 and jumps to
@ it. Code calls a function of another module or overlay through its stub. A
@ stub is usually named for its target with Far added:
@ mnTextDrawNumberAtOffsetStub reaches mnTextDrawNumberAtOffset.
.macro unnamed_stub target
    ldr r4, .Lstub_target\@
    bx r4
    .balign 4
.Lstub_target\@:
    .4byte \target
.endm

@ Publish a named entry in a module's jump table. Overlay imports use the
@ same entry form, with their local name and the module entry they call.
.macro stub name, target
    .global \name
    .thumb_func
\name:
    unnamed_stub \target
.endm
