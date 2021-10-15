# Blur CLI

Proof-of-concept multithreaded Gaussian blur of PPM images using POSIX threads. Built and tested with `GCC 9.3` on `Ubuntu 20.04` and `GCC 4.9` on `Ubuntu 14.04`.


## Usage

To try it from the terminal, setup the [`viu`](https://github.com/atanunq/viu) terminal image viewer via `cargo install viu`. If you don't have `cargo` installed, visit [rustup.rs](https://rustup.rs/) to install it on your system.

Run `make test` to test against the included image (in `images/small_orig.ppm`). You should see the blurred image in your terminal.

Issues and PRs welcome!
