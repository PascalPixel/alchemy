# Pre-rendered Backgrounds

This directory contains the 34 palette-correct 256×120 backgrounds stored as
resource IDs `1d` through `3e`. Each indexed PNG is a canonical source asset
and rebuilds its original delta7-compressed resource exactly.

The filenames describe visible environments so the collection is useful to
artists without requiring knowledge of the resource table. The resource ID is
retained as a stable prefix. `index.json` records both the descriptive scene
and an optional `location` field.

The location fields remain `null` until a particular in-game location is
independently established from this repository's approved evidence. A visual
resemblance alone is not treated as proof of a location name.
