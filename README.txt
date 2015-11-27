Electric field simulation for Topmetal and electrodes

run 'comsol mphserver matlab' to run MATLAB with COMSOL server.
     comsol mphserver matlab -graphics to display plots in a 
                                       COMSOL graphics window

run_script.m is the main entry point.

HexCellTMnFocusElectrode.m is a model file saved in COMSOL

    Generated from the corresponding .mph file.  Used File->Compact History to
    clean up the code.

    model = HexCellTMnFocusElectrode();
    model.hist.disable(); % disable the history logging
    mphgeom(model, 'geom1', 'Facealpha', 0.3); % display the geometry
    mphmesh(model);
    ModelUtil.showProgress(true); % show progress in a separate window
    model.study('std1').run(); % solve
    model.result('pg1').run(); % plot results
    pd = mphplot(model,'pg1'); % plot and get plot data, bug in v5.1
