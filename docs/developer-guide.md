# Developer Guide

## Environment setup

This is a C++ project.
Managing dependencies in C++ world is not an easy stuff.
To achive reproducible builds and deployments, we have a nix flake in our repo,
which specified the develop environment (see `flake.nix`).

<details>
<summary><b>For nix-lover</b></summary>

To enter the environment:

```
nix develop
```

Also, it is suggested to use [direnv](https://nixos.wiki/wiki/Development_environment_with_nix-shell#direnv).
So that you can choose your preferred shell (not only `bash`).

</details>

<details>
<summary><b>For nix-hater</b></summary>

However, you may not want to install nix on your system (for whatever reason).
The dependencies specified in `flake.nix` should be installed from your preferred package manager,
for example, package names `llvm-*-dev`.
This is not tested, so use as your own risk.
I don't mind to improve compatibility on most Linux systems,
so if you work on {Debian,Arch,RH,Gentoo}-based systems,
please test this project!
PRs are welcome for non-nix environment builds.

</details>

## Build from source


After you setup the environment, build this repository using Meson:

```
meson setup build --buildtype=release
```

```
meson compile -C build
```

## Testing

Our regression tests could be run via `lit test`.
