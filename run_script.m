logfid = fopen('log.txt','a');

ModelUtil.showProgress(true); % show progress in a separate window
clear model;
model = HexCellTMnFocusElectrode();
model.hist.disable(); % disable the history logging

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Vcathode = -1000;
Vfocus = -600;
dTmFe = 3.0;
diaFe = 3.0;

for diaFe = 3.0:1.0:8.0
    for dTmFe = 1.0:1.0:10.0
        for Vfocus = -900:100:-100

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

model.param.set('FocusElectrodeHole_diameter', num2str(diaFe, '%f[mm]'), 'hole diameter of the focusing electrode');
model.param.set('TM_Focus_Electrode_d', num2str(dTmFe, '%f[mm]'), 'distance between top surface of TM electrode and bottom surface of focusing electrode');
model.param.set('V_FocusElectrode', num2str(Vfocus, '%f[V]'), 'voltage on focusing electrode');
model.param.set('V_Cathode', num2str(Vcathode, '%f[V]'), 'voltage on cathode');
% model.result('pg2').feature('str1').set('zcoord', '10.9');

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

mphgeom(model, 'geom1', 'Facealpha', 0.3); % display the geometry
%mphmesh(model);
model.study('std1').run(); % solve
%model.result('pg1').run(); % plot results, slices
%model.result('pg2').run(); % plot results, streamline
model.result.export('plot1').run(); % export streamline

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

slt = sltrace('sl.dat',1,19.0,0.8);
[sltn c] = size(slt);
sltr = hypot(slt(:,4), slt(:,5));
[sltrn c] = size(find(sltr<0.5));
[sltrn1 c] = size(find(sltr<1.0));

fprintf(logfid, '%f %f %f %f %d %d %d\n', Vcathode, diaFe, dTmFe, Vfocus, sltn, sltrn, sltrn1);
fprintf('%f %f %f %f %d %d %d\n', Vcathode, diaFe, dTmFe, Vfocus, sltn, sltrn, sltrn1);

        end
    end
end

fclose(logfid);
