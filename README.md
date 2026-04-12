# REVive Emulator

This is a Steam.dll emulator forked from 2008 code of RevEmu. In addition to existing RevEmu features such as GCF mounting and multiplayer support, it also has Steam001 interface implementation (allows 2004 builds of Source Engine to work) and a good number of bugfixes.

Get the latest release here: https://github.com/NicknineTheEagle/ReviveEmu/releases/latest

**Please note that this is purely a Steam.dll emulator - it's intended for very old Steam games that use Steam.dll for Steam integration (such as old versions of GoldSource Engine and Source Engine). Pretty much all Steam games since 2008 use steam_api.dll to communicate with Steam, you want to use [Goldberg](https://github.com/Detanup01/gbe_fork) or [SmartSteamEmu](https://cs.rin.ru/forum/viewtopic.php?f=29&t=62935) for those.**

## Building

With Windows SDK installed, open Steam.sln in Visual Studio 2022 and build it.

## Usage (loose files)

1. Extract Steam.dll and rev.ini next to game exe (unless you're running a Source Engine game, then extract Steam.dll into bin directory and rev.ini next to game exe).

2. Play.

## Usage (GCF mounting)

1. Extract Steam.dll and rev.ini next to game exe (unless you're running a Source Engine game, then extract Steam.dll into bin directory and rev.ini next to game exe).

2. In rev.ini, set CacheEnabled to True and CachePath to the path where your GCF files are.

3. Put period appropriate raw CDR blob next to game exe and name it cdr.bin OR put ClientRegistry.blob with period appropriate CDR next to game exe OR extract revApps.ini next to game exe and configure it to mount the right GCF files for your game's app ID.

4. Run the game with -appid \<appid\> OR write your game's app ID into steam_appid.txt OR set SteamAppId environment variable to your game's app ID.

5. Play.

## Additional notes
* Later engines use both Steam.dll and steam_api.dll - in this case, you will need to combine REVive with a steam_api.dll emulator.

* The loaders that come with Goldberg and SmartSteamEmu both automatically set SteamAppId env var so you don't need to bother with step 4 when using GCF mounting.

## Credits

shmelle, revCrew, vityan666, bir3yk - original 2008 version of RevEmu.
