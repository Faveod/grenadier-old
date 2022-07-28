# Grenadier

[![ci](https://github.com/Faveod/grenadier/actions/workflows/ci.yml/badge.svg)](https://github.com/stackmystack/grenadier/actions/workflows/ci.yml)

Ruby bindings for [tree-sitter](https://github.com/tree-sitter/tree-sitter).

The [official bindings](https://github.com/tree-sitter/ruby-tree-sitter) are
very old, and unmaintained; it doesn't work with modern `tree-sitter` APIs.

## About
The main philosophy behind these bindings is to do a 1:1 mapping between
tree-sitter's `C` API and `Ruby`.

It doesn't mean that we're going to yield the best perormance, but this design
allows us to better experiment, and easily port ideas from other projects.

This design also implies that you need to be extra-careful when playing with the
provided objects.  Some of them have their underlying `C` data-structure
automatically freed, so you might get yourself in undesirable situations if you
don't pay attention to what you're doing.

We're only talking about `Tree`, `TreeCursor`, `Query`, and `QueryCursor`.  Just
don't copy them left and right, and then expect them to work without
`SEGFAULT`ing and creating a black-hole in your living-room.  Assume that you
have to work locally with them.

That said, we do aim at providing an idiomatic `Ruby` interface.  It should also
provide a _safer_ interface, where you don't have to worry about when and how
resources are freed.

To See a full list of the ruby-specific APIs, see [here](lib/README.md).

## Grenadier?

A _grenadier_ is a pomegrenate tree.  Pomegrenate fruits are ruby.  And they sit
on the grenadier.

## Dependencies

This gem is a binding for `tree-sitter`.  It doesn't have a version of
`tree-sitter` baked in it.

You must install `tree-sitter` and make sure that their dynamic library is accessible
from `$PATH`, or build the gem with `--disable-sys-libs` (see [Install](#Install))

You can either install `tree-sitter` from source or through your go-to package manager.

### Linux

`ubuntu >= 22.04`

```console
sudo apt install libtree-sitter-dev
```

`arch`

```console
sudo pacman -S tree-sitter
```

### MacOS

```console
sudo port install tree-sitter
```

or

```console
brew install tree-sitter
```

### Windows

It's not supported for the time being.  If you care about it please provide a patch.

Or if you use `WSL`, then follow the [Linux](#Linux) section above.

## Install

We haven't released the gem on `Rubygems` as of yet, but wer'e planning on doing so.

Meanwhile, please build from source.

### Gemfile

```ruby
gem tree_sitter, git: 'https://github.com/stackmystack/grenadier'
```

### Build from source

```console
git clone https://github.com/stackmystack/grenadier
bundle install
bundle exec rake compile
```

If you chose not to install and bother with `tree-sitter` installation:

``` console
bundle exec rake compile -- --disable-sys-libs
```


You can now jump into a REPL and start experimenting with Grenadier:

```console
bundle exec rake console
```

Or you can build the gem then install it:

```console
gem build tree_sitter.gemspec
gem install tree_sitter-x.x.x.gem
```

## Examples

See `examples` directory.

## Development

If you want to hack on this gem, you only have to work with `rake compile` and
`rake clean`.

It's advised to run `rake clean && rake compile` everytime you modify `C` code.
I've run into trouble several times by not doing so.

### ASAN

You can enable `asan` by setting the `SANITIZE` environment variable before building:

```console
SANITIZE=1 bundle exec rake compile
```

On linux:

``` console
LD_PRELOAD==libasan.so.8 bundle exec rake test
```

This is still experimental, and I haven't had any true success yet, but it's a good
start.
