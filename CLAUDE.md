# CLAUDE.md

eugo-inc/libpq-standalone (default branch: master): the PostgreSQL source
tree (18devel, full upstream commit history retained) carved down to libpq
and its build dependencies. Builds the client library only -- no server.
Fork identity lives in meson.build ~L104-108: PACKAGE_NAME
`libpq-standalone`, PACKAGE_BUGREPORT `raptors@eugo.io`, pg_url -> this repo.

Kept vs upstream postgres:

- Kept: src/interfaces/libpq (static `libpq_st` + shared `libpq_so`),
  src/common, src/port, src/include, src/bin/pg_config, src/tools.
- Removed: backend, doc, contrib, ecpg, every other src/bin tool, src/test,
  and the whole autoconf build (configure/GNUmakefile). Meson is the ONLY
  build system.
- Fork-only files (operationally relevant): .devcontainer/, .mcp.json, the
  meson.build identity lines (plus tracked IDE cruft: .idea/, .DS_Store).
- There are NO `@EUGO_CHANGE` markers in this repo (grep finds zero outside
  this file) -> to locate eugo deltas, diff against upstream postgres via
  git history, not markers.

## Build

```bash
meson setup build    # pass: exit 0; summary lists gss/ldap/nls/openssl state
ninja -C build       # pass: exit 0; produces libpq shared+static and pg_config
```

- `meson: command not found` on the host -> build inside .devcontainer/
  (linux/arm64 eugo toolchain image from ECR account 411727469413; ships
  clang/cmake/meson). NEVER hand-install a substitute toolchain on the
  host -> use the devcontainer.
- TAP scripts remain at src/interfaces/libpq/t/*.pl, but upstream's harness
  (src/test/perl) was carved out and no meson `test()` is wired -> there is
  no runnable test suite; a green `ninja -C build` is the check.

## When X happens

- About to sync upstream postgres -> this checkout has only `origin`; add
  an upstream postgres remote first. Every prior sync is a merge commit
  (messages vary: "Merge branch 'postgres-master'", "Merged upstream",
  "merge"); NEVER rebase master onto upstream -> merge, matching that
  history.
- Merge reports modify/delete conflicts on paths outside the kept set
  (src/backend/, doc/, contrib/, configure, other src/bin or
  src/interfaces dirs) -> keep our deletion; the carve-out stays libpq-only.
- Merge touches meson.build -> after resolving, `grep -n eugo meson.build`
  must still show the pg_url and PACKAGE_BUGREPORT identity lines.
