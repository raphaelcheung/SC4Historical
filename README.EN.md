# SC4Historical

Simcity 4 DLL plug-in, used to set historical buildings with one click.

In the rapid development of cities, it is often necessary to set historical buildings to avoid the crowding of high-income buildings by low-income buildings, so as to control the healthy proportion of income population and jobs.

Setting these up manually takes a lot of time and effort, so this plugin was developed to hopefully help.



Plug-ins can be downloaded from the distribution page.

## System requirements

* SimCity 4 v641 and above
* Windows 10 or above

## Installation

1. Shut down SimCity 4.
2. Copy the `sc4history.dll` file to the plugin directory of SimCity 4.
3. Run SimCity 4.

## Use plugins

After entering the map, press Ctrl+X to open the command line and type:

`his < building type > < minimum capacity > [declared historic building]`


`< building type >` : Here can be r, o, s, i, corresponding to residential, commercial, service, industrial buildings;

`< Minimum capacity >` : An integer value. This command applies only to buildings with a capacity greater than or equal to this capacity;

`[Declare historic building]` : Fill in true, false.

### Examples
If you want to set up office buildings with a capacity of more than 300 as historic buildings, the order is as follows:
`his o 300 true`

Eliminate all residential buildings as historic buildings:
`his r 0 false`

# License

This project follows MIT protocol.

View [LICENSE.txt](LICENSE.txt) for details.

## Third party code

[gzcom-dll](https://github.com/nsgomez/gzcom-dll/tree/master) Located in the vendor folder, MIT License.

[EABase](https://github.com/electronicarts/EABase) Located in the vendor folder, BSD 3-Clause License.

[EASTL](https://github.com/electronicarts/EASTL) Located in the vendor folder, BSD 3-Clause License.

[Windows Implementation Library](https://github.com/microsoft/wil) - MIT License
[SC4Fix](https://github.com/nsgomez/sc4fix) - MIT License
[Boost.Algorithm](https://www.boost.org/doc/libs/1_84_0/libs/algorithm/doc/html/index.html) - Boost Software License,  Version 1.0.
[sc4-growify]()

# Source code

## Preconditions

* Visual Studio 2022
* [VCPkg] (https://github.com/microsoft/vcpkg), according to the official guide to install and initialize, remember to perform ` VCPkg both install ` with Visual Studio