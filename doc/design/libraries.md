# Libraries

| Name                     | Description |
|--------------------------|-------------|
| *libundal_cli*         | RPC client functionality used by *undal-cli* executable |
| *libundal_common*      | Home for common functionality shared by different executables and libraries. Similar to *libundal_util*, but higher-level (see [Dependencies](#dependencies)). |
| *libundal_consensus*   | Stable, backwards-compatible consensus functionality used by *libundal_node* and *libundal_wallet* and also exposed as a [shared library](../shared-libraries.md). |
| *libundalconsensus*    | Shared library build of static *libundal_consensus* library |
| *libundal_kernel*      | Consensus engine and support library used for validation by *libundal_node* and also exposed as a [shared library](../shared-libraries.md). |
| *libundalqt*           | GUI functionality used by *undal-qt* and *undal-gui* executables |
| *libundal_ipc*         | IPC functionality used by *undal-node*, *undal-wallet*, *undal-gui* executables to communicate when [`--enable-multiprocess`](multiprocess.md) is used. |
| *libundal_node*        | P2P and RPC server functionality used by *undald* and *undal-qt* executables. |
| *libundal_util*        | Home for common functionality shared by different executables and libraries. Similar to *libundal_common*, but lower-level (see [Dependencies](#dependencies)). |
| *libundal_wallet*      | Wallet functionality used by *undald* and *undal-wallet* executables. |
| *libundal_wallet_tool* | Lower-level wallet functionality used by *undal-wallet* executable. |
| *libundal_zmq*         | [ZeroMQ](../zmq.md) functionality used by *undald* and *undal-qt* executables. |

## Conventions

- Most libraries are internal libraries and have APIs which are completely unstable! There are few or no restrictions on backwards compatibility or rules about external dependencies. Exceptions are *libundal_consensus* and *libundal_kernel* which have external interfaces documented at [../shared-libraries.md](../shared-libraries.md).

- Generally each library should have a corresponding source directory and namespace. Source code organization is a work in progress, so it is true that some namespaces are applied inconsistently, and if you look at [`libundal_*_SOURCES`](../../src/Makefile.am) lists you can see that many libraries pull in files from outside their source directory. But when working with libraries, it is good to follow a consistent pattern like:

  - *libundal_node* code lives in `src/node/` in the `node::` namespace
  - *libundal_wallet* code lives in `src/wallet/` in the `wallet::` namespace
  - *libundal_ipc* code lives in `src/ipc/` in the `ipc::` namespace
  - *libundal_util* code lives in `src/util/` in the `util::` namespace
  - *libundal_consensus* code lives in `src/consensus/` in the `Consensus::` namespace

## Dependencies

- Libraries should minimize what other libraries they depend on, and only reference symbols following the arrows shown in the dependency graph below:

<table><tr><td>

```mermaid

%%{ init : { "flowchart" : { "curve" : "basis" }}}%%

graph TD;

undal-cli[undal-cli]-->libundal_cli;

undald[undald]-->libundal_node;
undald[undald]-->libundal_wallet;

undal-qt[undal-qt]-->libundal_node;
undal-qt[undal-qt]-->libundalqt;
undal-qt[undal-qt]-->libundal_wallet;

undal-wallet[undal-wallet]-->libundal_wallet;
undal-wallet[undal-wallet]-->libundal_wallet_tool;

libundal_cli-->libundal_util;
libundal_cli-->libundal_common;

libundal_common-->libundal_consensus;
libundal_common-->libundal_util;

libundal_kernel-->libundal_consensus;
libundal_kernel-->libundal_util;

libundal_node-->libundal_consensus;
libundal_node-->libundal_kernel;
libundal_node-->libundal_common;
libundal_node-->libundal_util;

libundalqt-->libundal_common;
libundalqt-->libundal_util;

libundal_wallet-->libundal_common;
libundal_wallet-->libundal_util;

libundal_wallet_tool-->libundal_wallet;
libundal_wallet_tool-->libundal_util;

classDef bold stroke-width:2px, font-weight:bold, font-size: smaller;
class undal-qt,undald,undal-cli,undal-wallet bold
```
</td></tr><tr><td>

**Dependency graph**. Arrows show linker symbol dependencies. *Consensus* lib depends on nothing. *Util* lib is depended on by everything. *Kernel* lib depends only on consensus and util.

</td></tr></table>

- The graph shows what _linker symbols_ (functions and variables) from each library other libraries can call and reference directly, but it is not a call graph. For example, there is no arrow connecting *libundal_wallet* and *libundal_node* libraries, because these libraries are intended to be modular and not depend on each other's internal implementation details. But wallet code is still able to call node code indirectly through the `interfaces::Chain` abstract class in [`interfaces/chain.h`](../../src/interfaces/chain.h) and node code calls wallet code through the `interfaces::ChainClient` and `interfaces::Chain::Notifications` abstract classes in the same file. In general, defining abstract classes in [`src/interfaces/`](../../src/interfaces/) can be a convenient way of avoiding unwanted direct dependencies or circular dependencies between libraries.

- *libundal_consensus* should be a standalone dependency that any library can depend on, and it should not depend on any other libraries itself.

- *libundal_util* should also be a standalone dependency that any library can depend on, and it should not depend on other internal libraries.

- *libundal_common* should serve a similar function as *libundal_util* and be a place for miscellaneous code used by various daemon, GUI, and CLI applications and libraries to live. It should not depend on anything other than *libundal_util* and *libundal_consensus*. The boundary between _util_ and _common_ is a little fuzzy but historically _util_ has been used for more generic, lower-level things like parsing hex, and _common_ has been used for undal-specific, higher-level things like parsing base58. The difference between util and common is mostly important because *libundal_kernel* is not supposed to depend on *libundal_common*, only *libundal_util*. In general, if it is ever unclear whether it is better to add code to *util* or *common*, it is probably better to add it to *common* unless it is very generically useful or useful particularly to include in the kernel.


- *libundal_kernel* should only depend on *libundal_util* and *libundal_consensus*.

- The only thing that should depend on *libundal_kernel* internally should be *libundal_node*. GUI and wallet libraries *libundalqt* and *libundal_wallet* in particular should not depend on *libundal_kernel* and the unneeded functionality it would pull in, like block validation. To the extent that GUI and wallet code need scripting and signing functionality, they should be get able it from *libundal_consensus*, *libundal_common*, and *libundal_util*, instead of *libundal_kernel*.

- GUI, node, and wallet code internal implementations should all be independent of each other, and the *libundalqt*, *libundal_node*, *libundal_wallet* libraries should never reference each other's symbols. They should only call each other through [`src/interfaces/`](`../../src/interfaces/`) abstract interfaces.

## Work in progress

- Validation code is moving from *libundal_node* to *libundal_kernel* as part of [The libundalkernel Project #24303](https://github.com/undal/undal/issues/24303)
- Source code organization is discussed in general in [Library source code organization #15732](https://github.com/undal/undal/issues/15732)
