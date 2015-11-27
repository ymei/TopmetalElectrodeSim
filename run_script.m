ModelUtil.showProgress(true); % show progress in a separate window
clear model;
model = HexCellTMnFocusElectrode();
model.hist.disable(); % disable the history logging
mphgeom(model, 'geom1', 'Facealpha', 0.3); % display the geometry
% mphmesh(model);
model.study('std1').run(); % solve
% model.result('pg1').run(); % plot results, slices
% model.result('pg2').run(); % plot results, streamline
model.result.export('plot1').run(); % export streamline
