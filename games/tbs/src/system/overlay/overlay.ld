/* One overlay of The Broken Seal, loaded at 0x2008000. Its objects are its
   rows of games/tbs/objects.tsv, which the build writes for the overlay it
   links into extern.ld, text.ld, bss.ld and discard.ld; bss.ld is the whole
   .bss section, under the header games/tbs/overlays.tsv gives it. Its unit
   row names its C file and is three objects of text.ld: the entry table
   (entry.s) of that file's folder, the C file, and the folder's import
   table (import.s). */
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
