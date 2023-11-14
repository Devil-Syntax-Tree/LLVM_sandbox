# LLVM PASS EXAMPLES

## How to run

To run the pass you must use opt and target the pass on its CLI

> opt -load-pass-plugin <path-to-pass-shared-lib> -passes=<pass-name> [-disable-output] <path-to-ir-file>

## NOTE

On the devcontainer the LLVM components have the version as postfix.

As an example

- clang-17
- clang++-17
- opt-17
- And so on...
