PbF2Calo
========
A simple simulation of PbF2 calorimenter, based on TestEm17 from Geant4.

Dependencies
------------
- Geant4 10.01: USolids
- Geant4 10.00: multi-threading
- ROOT6: TTreeReader
- CMake 2.8.12: CMAKE_MACOSX_RPATH 

Getting started
---------------
    git clone
    git submodule init
    git submodule update
    mkdir build_dir
    cmake
    make

In place of documentation
-------------------------
Production cuts are passed to Geant4 `G4ProductionCuts::SetProductionCuts`.
The order is defined in `enum G4ProductionCutsIndex` (`G4ProductionCuts.hh`) as:
`[ gamma, electron, positron, proton ]`. The unit is `um`.


MIT License.

