# nodelet

A tiny Node.js-inspired runtime built in C, powered by [QuickJS](https://github.com/quickjs-ng/quickjs).

I've always been curious about how Node.js works internally, so I thought: why not try to build a mini version of it myself?

The goal is to learn by building. I want to explore how a JS engine is embedded, how C and JavaScript interact, how the eval loop works, and how the event loop decides what runs when, especially when promises and I/O get involved.

This is basically my playground for stealing ideas from Node.js, understanding the trade-offs behind them, and implementing a tiny version myself.

I'm using QuickJS instead of V8 to keep this a manageable, C-only side project. The scope is intentionally small: **console and I/O, no networking or child processes** (at least for now).

This is an ongoing hobby project that I'll keep tinkering with whenever I have the time to explore more.

I'm not trying to rebuild all of Node.js. I just want to understand how the pieces fit together by building them myself.

## Quick start

Fetch the vendored QuickJS sources (pinned to quickjs-ng commit `270b8c9`):

    git clone https://github.com/quickjs-ng/quickjs.git vendor/quickjs
    git -C vendor/quickjs checkout 270b8c9

TODO: automate vendor bootstrap (make vendor target or setup script) in init.

Build and run:

    make build
    ./nodelet test.js
