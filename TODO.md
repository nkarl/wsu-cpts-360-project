
### High Priority

1. [ ] Look for the `ext2fs` header equivalent in Rust crates.
2. [ ] Map out the project hierarchy and start using the cargo toolchain for unit testing right away.

#### 1. Research tech requirements for the Rust project

- [bindgen](https://crates.io/crates/bindgen): automatically generates Rust FFI bindings to C (and some C++)
	- [requirements](https://rust-lang.github.io/rust-bindgen/requirements.html)
		1. add bingen as build dependecy to Cargo.toml
		2. create a wrapper.h header so that it can detect and generate Rust FFI
		3. create a `build.rs` ([example](https://rust-lang.github.io/rust-bindgen/tutorial-3.html) for the `bzip2` lib)
		4. include the bindings in `src/lib.rs`
		5. write a sanity test ([example](https://rust-lang.github.io/rust-bindgen/tutorial-5.html))

### Lower Priority


## Done

