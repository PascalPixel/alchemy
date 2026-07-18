# Sound sources

The ROM-side engine checks the little-endian `Smsh` signature and drives a
relocated mixed Thumb/ARM sample renderer. This identifies an engine-native
sequence, tone, and signed-PCM pipeline; MIDI and rendered audio are useful
previews, but neither is an exact source format by itself.

The current local evidence divides the native data as follows:

| ROM range | Evidence-backed role | Current source state |
|---|---|---|
| `080fb7a0..080fba77` | command-dispatch and pitch/volume lookup tables | unclaimed |
| `080fba78..080fc4ff` | tone-bank area; song headers select roots at `080fba78` and `080fc138` | unclaimed |
| `080fc500..080fc623` | engine configuration and lookup data | unclaimed |
| `080fc624..080fc683` | eight music-player records | unclaimed |
| `080fc684..080fd043` | 312-entry sound-selection table | `song_table.json` |
| `080fd044..080fd047` | shared empty sound header | unclaimed |
| `080fd048..0815fb77` | tone-referenced wave arena, including 32 signed-PCM records | unclaimed |
| `0815fb78..08184697` | 264 non-empty headers and 618 referenced track streams | unclaimed |

`song_table.json` uses symbols for header references and records the player
selected by the ROM code. Its final halfword always duplicates that selector;
the source represents the proven duplication without inventing a meaning for
the field. Forty-eight empty entries share `sound_empty`, and repeated real
headers likewise reuse one symbol.

The next exact unit is the header and track-stream grammar. It must preserve
loops, pattern calls, running-status commands, priorities, reverb, tone-bank
references, and pointer targets in an engine-native document. A MIDI export
can then be generated from that document, but must never become the input used
to rebuild the ROM.
