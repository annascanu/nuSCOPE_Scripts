# nuSCOPE_Scripts

Scripts I'm writing for my studies for the [nuSCOPE experiment](https://nuscope.web.cern.ch/).  
⚠️ Everything is in very early stages of development! A complete switch to Python and Jupyter notebooks might happen in the future.

---

## Repository Structure
```plaintext
nuSCOPE_Scripts/
├── src/
│   └── test.cpp   # Macro to plot neutrino interactions for DUNE and T2K (more efficient, should become "main" macro)
│   └── DUNE_T2K_plots.cpp   # Macro to plot neutrino interactions for DUNE and T2K (a bit slower)
│   └── nuSCOPE_EnergyBias.cpp   # Macro to perform studies on energy bias for nuSCOPE
├── Test_new_plots
│   └── plots.pdf # A series of plots (which are "final" for the initial tests)
└── README.md
```

---

## Build & Run

To compile one of the codes, run from the command line:

```bash
c++ src/Name_Of_Code.cpp `root-config --cflags --libs` -o Name_Of_Executable.out
```

Then execute with:

```bash
./Name_Of_Executable.out
```

---

## Requirements
- [ROOT](https://root.cern.ch/install/)
- C++ compiler (e.g. `g++`, `clang++`)
- (more will be added as the repo grows)

---

## Roadmap
WIP! Some ideas for next steps:
- [x] DUNE vs T2K neutrino interaction plots
- [ ] Add more macros
- [ ] Explore switch to Python
- [ ] Improve documentation and examples

---

## Contacts
I’m a very messy coder so I’d be happy to chat in case anything in here is awful, breaks, or whatever.  
📧 anna.scanu@cern.ch  
📧 anna.scanu@mib.infn.it