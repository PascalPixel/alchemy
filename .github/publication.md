# Alchemy CI and publication

`build.yml` proves in public what anyone can check: from a clean clone with no
ROM, it builds the pinned compilers and all six Broken Seal editions, checks
each against `rom.sha1` and measures them from the linker maps it just made
(`alchemy-devtools progress --game tbs`, the same count `make progress` runs).
ccache hands back a compile only when its preprocessed source, compiler and
options are identical, so a push that changes comments recompiles almost
nothing; every ROM is still linked and checked. Pull requests stop there. The
tool tests, the format check, `make modern` and The Lost Age's editions, which
need private cartridge dumps, run locally in `make land`.

For a landing, stage every source change and run `make land`. The receipt
records the staged source tree, measurement time and hashes of the TSV and
decomp.dev report; measurement refuses unstaged or untracked source. The
pre-push hook uploads that receipt and both reports, named for the pushed
commit, to the report archive. A changed measurement can never overwrite an
existing commit's report.

The archive is the `decomp-reports` release and its numbered successors,
`decomp-reports-2` and on: GitHub holds at most 1,000 assets in a release.
Every reader (CI, alchemy-devtools, the docs site) searches all of them, and
every writer adds to the newest, opening the next while the newest still has
room for a whole landing (`alchemy-devtools progress --volumes` and
`--archive`). No ROM, executable or map is uploaded.

After the main build succeeds, the `publish` job admits the commit as
`<commit>.accepted.tsv`, with or without a receipt:

- With a receipt, it verifies the receipt against the commit's tree and both
  reports, requires its Broken Seal row to equal CI's own measurement, and
  accepts the receipt as it stands.
- Without one, it accepts a CI record: the commit's tree and time, CI's Broken
  Seal figures, and The Lost Age's from the nearest ancestor with a receipt.
  Those are carried as this commit's only when the git tree entries of
  everything The Lost Age's measurement reads (`Makefile`, `rom.sha1`,
  `games/tla`, `recon/tla`, `tools`, and the `alchemy-gcc` and
  `alchemy-devtools` pins) are identical to the ancestor's. Otherwise the record
  names the ancestor and holds no Lost Age row, and the chart and `source.tsv`
  show The Lost Age as last measured at that commit. With no ancestor receipt,
  only The Broken Seal is published. Each later publication re-verifies these
  claims against Git, so no record can give figures to a tree they were not
  measured from.

History is reconstructed from the immutable `progress-history-v1.tsv` baseline
and accepted ancestors of that commit. The original baseline retains its
recorded dates; new measurement dates use UTC, and a CI record is dated by its
commit. A rerun keeps the original measurement time. The release also holds an
immutable `<commit>.site.tar.gz` containing only the two PNG figures,
`history.tsv` and `source.tsv`; `source.tsv` names the commit The Lost Age's
figures were measured at and whether its inputs were identical. Releases
preserve these beyond Actions' artifact retention. `combined_report`,
containing `report.pb`, remains a CI artifact for decomp.dev, for commits with
a receipt, since only a receipt holds all twelve editions.

The same job then refuses a superseded revision and starts the site build through the Vercel deploy hook in the `VERCEL_DEPLOY_HOOK`
secret. Vercel builds `alchemy-docs` at its main branch: it clones Alchemy's
main, generates the pages from the source and takes the accepted figures from
the release, then serves them at <https://alchemy.pascalpixel.com/>, where
README shows the figures. A push to `alchemy-docs` builds the same way. There
is no schedule and no generated Git branch; Vercel keeps the last successful
deployment on failure.

## Setup and recovery

The `VERCEL_DEPLOY_HOOK` secret is the URL of a deploy hook on the `alchemy`
Vercel project for branch `main`; anyone holding it can start a build, nothing
more. To replace it, create a new hook with `vercel deploy-hooks create`,
store its URL in the secret and remove the old hook.

A generator change publishes when it is pushed to `alchemy-docs`.
To retry publication, rerun the failed `publish` job; it refuses a superseded
revision.
If the build fails before reports are archived, fix the build first.

The old `progress` branch remains as a frozen, recoverable archive. Its
baseline is also preserved in the release, checked against a fixed digest
by alchemy-devtools. Do not remove the releases or their report assets: they
are the durable publication history.
