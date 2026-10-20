# Alchemy CI and publication

`build.yml` tests the C tools, builds the pinned compiler sources, compares all
six Broken Seal editions without an input ROM, and compiles them with current
GCC. Pull requests stop there. The Lost Age still needs private cartridge
dumps, so twelve-edition verification happens locally.

For a landing, stage every source change and run `make land`. The receipt
records the staged source tree, measurement time and hashes of the TSV and
decomp.dev report; measurement refuses unstaged or untracked source. The
pre-push hook uploads that receipt and both reports, named for the pushed
commit, to the `decomp-reports` release. A changed measurement can never
overwrite an existing commit's report. No ROM, executable or map is uploaded.

After the main build succeeds, CI verifies the matching receipt and accepts
it as `<commit>.accepted.tsv`. History is reconstructed from the immutable
`progress-history-v1.tsv` baseline and accepted ancestors of that commit.
The original baseline retains its recorded dates; new measurement dates use
UTC. A rerun keeps the original measurement time. The release also holds an
immutable `<commit>.site.tar.gz` containing only the two PNG figures,
`history.tsv` and `source.tsv`. Releases preserve these beyond Actions'
artifact retention. `combined_report`, containing `report.pb`, remains a CI
artifact for decomp.dev.

`docs.yml` runs after successful CI on main, refuses a superseded revision and
starts the site build through the Vercel deploy hook in the `VERCEL_DEPLOY_HOOK`
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
To retry publication, rerun the failed job. The `Publish docs` workflow also
accepts the ID of a successful main CI run; it refuses a superseded revision.
If the build fails before reports are archived, fix the build first.

The old `progress` branch remains as a frozen, recoverable archive. Its
baseline is also preserved in the release, checked against a fixed digest
by alchemy-devtools. Do not remove the release or its report assets: they
are the durable publication history.
