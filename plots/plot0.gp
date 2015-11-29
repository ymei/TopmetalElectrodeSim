fn = '../data/scan0.dat'
diaFe = 3.0

set xrange [0:11]
set yrange [-1000:0]

set xlabel 'Distance from Topmetal to Focusing Electrode [mm]'
set ylabel 'Voltage on Focusing Electrode [V]'
set cblabel 'Collection efficiency'
set title sprintf('Hole diameter %.1fmm in Focusing Electrode', diaFe)

plot fn u ($2==diaFe?$3:1/0):4:($6/$5) w p pt 5 ps 8 lt palette t '', '' u ($2==3.0?$3:1/0):4:($7/$5) w p pt 7 ps 5 lt palette t ''
