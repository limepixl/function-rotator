## What?

This is a general purpose compiler suite that includes GCC, GNU make and GDB. If you just need to "download a C/++ compiler and be done with it", this is the way to go. This doesn't require any installation or path config etc. It comes preconfigured with static builds of the [Intel&reg; Threading Building Blocks](https://www.threadingbuildingblocks.org/intel-tbb-tutorial) library, which is a sane extension of C++ for dead-simple threading and concurrency. Click the link to learn more.

This hosts GCC 5.

## Why?

I need the same GCC environment on all my Windows PCs without going crazy. I hate installers. That's basically it.

## How?

- Download as a zip
- Run one of the batch files to get a compiler prompt. 

Now you can use gcc, g++, gdb and make as ordinary commands. It launches in powershell, so you get at least some unix aliases (`ls` etc).

## TBB?

In short it's an awesome library for building parallel applications. It provides such amazing things as maps, concurrent hashtables and vectors (read: thread-safe), parallel forms of other loops, automatic task scheduling and much more. Use it if you can. The included build is static and you don't need to ship any DLLs or other runtime with it.

### Example

**`for`-loop - bad!**

```cpp
void SerialApplyFoo( float a[], size_t n ) {
    for( size_t i=0; i!=n; ++i )
        Foo(a[i]);
}
```

**Parallelized `for`-loop - good!**

```cpp
#include "tbb/tbb.h"

using namespace tbb;

void ParallelApplyFoo( float a[], size_t n ) {
    tbb::parallel_for( size_t(0), n, [&]( size_t i ) {
        Foo(a[i]);
    } );
}
```

## Why not include library libawesome?!11!

Because it's probably too big and/or requires an SDK to run. Future additions will include source-only libs like linenoise (to nuke readline), [NanoCL](https://github.com/turbo/NanoCL) and others.
