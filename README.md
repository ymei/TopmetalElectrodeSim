# Electric field simulation for Topmetal (pixel) and electrodes

## Generic pixel array and charge focusing

Simulated with COMSOL v6.0

  - `PixelFieldSimFEM.mph` Seven-pixel array, fully simulated with FEM.

## Topmetal-S in hexagonal array

All simulated with COMSOL up to v5.6

  - `HexCellElectrode.mph` Single cell, with saved mesh and results.
  - `HexCellx7Electrode.mph` Seven-chip array but simulating only the middle one.  For visualization only.
  - `HexCellElectrodeSurfaceCharge.mph`  A trial with added surface charge boundary condition to the chip.
  - `HexCellTMnFocusElectrode.mph` Single focusing electrode, with both drift and weighting fields set up.  No mesh or saved results.  Prepared for MATLAB.
  - `HexCellTMn2FocusElectrode.mph` two focusing electrodes stacked.  No mesh or saved results.  Prepared for MATLAB.

## Launching the simulation with COMSOL+MATLAB
Run `comsol mphserver matlab` to launch MATLAB with COMSOL server;
    `comsol mphserver matlab -graphics` to display plots in a COMSOL graphics window

`run_script.m` is the main entry point in the MATLAB Command Window.

`HexCellTMnFocusElectrode.m` is a model file saved in COMSOL.  It is generated from the corresponding `.mph` file.  Use `File->Compact History` in COMSOL to clean up the code.

```
    model = HexCellTMnFocusElectrode();
    model.hist.disable(); % disable the history logging
    mphgeom(model, 'geom1', 'Facealpha', 0.3); % display the geometry
    mphmesh(model);
    ModelUtil.showProgress(true); % show progress in a separate window
    model.study('std1').run(); % solve
    model.result('pg1').run(); % plot results in COMSOL's interactive window
    pd = mphplot(model,'pg1'); % plot and get plot data
```

Compile C files to be called in MATLAB:
```
    mex -v -largeArrayDims
```
Check out `Makefile` that automates this.
