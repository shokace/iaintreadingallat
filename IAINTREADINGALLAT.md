# git iaintreadingallat

The "hold tight twin" button for Git.

This command:
- stages everything (even ignored files)
- commits with a fixed message
- force-pushes without hooks

Use at your own risk. It is intentionally chaotic.

## Usage

```sh
./git iaintreadingallat
```

You should see:

```
hold tight twin
```

## Build (once)

```sh
make configure
./configure
make -j4
```

## System-wide PATH (two options)

Pick one:

1. Install this Git build (recommended if you want it everywhere)
```sh
make install
```
This installs into `/usr/local` by default. Ensure `/usr/local/bin` is before `/usr/bin` in your PATH.

2. Symlink the built binary into a directory on PATH (quick and reversible)
```sh
ln -s /Users/ezkie/Repos/iaintreadinallat/git/git /usr/local/bin/git
```

To verify:
```sh
which git
git --version
```
