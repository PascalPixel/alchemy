/* One overlay of The Broken Seal, loaded at 0x2008000. Its objects are its
   rows of games/tbs/objects.tsv, which the build writes for the overlay it
   links into extern.ld, text.ld, bss.ld and discard.ld; bss.ld is the whole
   .bss section, under the header games/tbs/overlays.tsv gives it. */
INCLUDE "extern.ld"
SECTIONS
{
    .text 0x2008000 :
    {
        INCLUDE "text.ld"
    }
    INCLUDE "bss.ld"
    /DISCARD/ :
    {
        INCLUDE "discard.ld"
        *(.ARM.attributes) *(.comment) *(.note*)
    }
}
