{
  description = "A flake to build binfetch.";

  inputs = {
    nixpkgs.url = github:nixos/nixpkgs/nixos-unstable;
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = {
    self,
    nixpkgs,
    flake-utils,
    ...
  } @ inputs:
    flake-utils.lib.eachDefaultSystem (system: let
      pkgs = nixpkgs.legacyPackages.${system};
      lib = nixpkgs.lib;
    in {
      # Executed by `nix build`
      packages = rec {
        binfetch = pkgs.stdenv.mkDerivation {
          name = "binfetch";
          src = lib.cleanSourceWith {
            filter = name: type: !(lib.hasSuffix ".nix" (baseNameOf (toString name)));
            src = lib.cleanSource ./.;
          };
          buildInputs = with pkgs; [zlib];
          # TODO make custom config possible
          # optimally by loading $XDG_CONFIG_HOME ($HOME/.config as fallback)
          # and $XDG_CONFIG_DIRS (/etc/xdg as fallback)
          # see https://specifications.freedesktop.org/basedir-spec/latest/ar01s03.html
          preBuild = "mkdir -p $out/bin $out/etc/binfetch";
          makeFlags = ["bootstrap" "CONFIG=$(out)/etc/binfetch" "PREFIX=$(out)"];
          postBuild = "cp cfg/binfetch.cfg $out/etc/binfetch";
        };
        default = binfetch;
      };
    });
}
