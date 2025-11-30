{
  inputs.nixpkgs.url = "github:nixos/nixpkgs/nixpkgs-unstable";

  outputs = inputs: let
    system = "x86_64-linux";
    pkgs = import inputs.nixpkgs { inherit system; };
  in {
    # Based on https://github.com/NixOS/nixpkgs/blob/nixos-24.11/pkgs/applications/system/qjournalctl/default.nix
    packages.${system}.default = pkgs.stdenv.mkDerivation rec {
      pname = "qjournalctl";
      version = "0.6.4";
      src = ./.;

      nativeBuildInputs = [
        pkgs.libsForQt5.qt5.qmake
        pkgs.libsForQt5.qt5.wrapQtAppsHook
        pkgs.pkg-config
      ];

      buildInputs = [
        pkgs.libsForQt5.qt5.qtbase
        pkgs.libssh
      ];

      meta = with pkgs.lib; {
        description = "Qt-based graphical user interface for systemd's journalctl command";
        mainProgram = "qjournalctl";
        homepage = "https://github.com/pentix/qjournalctl";
        license = licenses.gpl3Only;
        platforms = platforms.all;
        maintainers = with maintainers; [ romildo ];
      };
    };
  };
}
