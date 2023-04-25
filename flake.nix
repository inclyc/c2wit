{
  inputs.nixpkgs.url = "github:NixOS/nixpkgs/nixpkgs-unstable";
  inputs.flake-utils.url = "github:numtide/flake-utils";

  outputs = { nixpkgs, flake-utils, ... }@inputs:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = nixpkgs.legacyPackages.${system};
        llvmPackages = pkgs.llvmPackages_15;
        devInputs = with pkgs; [
          clang-tools
        ];
        nativeBuildInputs = with pkgs; [
          ninja
          meson
          lit
          cmake
          llvmPackages.clang
        ];
        buildInputs = with llvmPackages; [
          libclang
          llvm
        ] ++ (with pkgs; [
          fmt
        ]);
        selfPackage = pkgs.stdenv.mkDerivation rec {
          inherit nativeBuildInputs buildInputs;
          pname = "C2Wit";
          version = "0.0.0";
          src = ./.;
        };
      in
      {
        devShells.default = pkgs.mkShell {
          inherit buildInputs;
          nativeBuildInputs = devInputs ++ nativeBuildInputs;
        };
        packages.default = selfPackage;
      }
    );
}
