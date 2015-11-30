fn = '../data/scan0.dat'

set output "1.eps"
set terminal postscript eps enhanced color size 8.5,11 "Helvetica" 16

unset colorbox
set border lw 0.1

set xrange [1.5:8.5]
set yrange [-1000:0]
set cbrange [0:1.0]

unset xlabel
set ylabel 'Voltage on Focusing Electrode [V]'
set cblabel 'Collection efficiency'

set multiplot layout 5,2 rowsfirst title "Hex edge=5mm, cell height=20mm, Vcathode=-1000V, #streamlines=271, dTM=1mm\nSquare shows collection within TM, circle shows collection within d=2mm" \
    margins screen 0.1, 0.9, 0.05, 0.93 spacing screen 0.05

do for [dTmFe = 1:10:1] {

    if (dTmFe > 1) {
        unset xlabel
        unset ylabel
        unset colorbox
    }
    if (dTmFe == 2) {
        set colorbox
    }
    if (dTmFe == 9) {
        set xlabel 'Hole diameter in Focusing Electrode [mm]'
    }
    set title sprintf('Distance from Topmetal to Focusing Electrode %.1fmm', dTmFe)

    plot fn u ($3==dTmFe?$2:1/0):4:($6/$5) w p pt 5 ps 3 lt palette t '', '' u ($3==dTmFe?$2:1/0):4:($7/$5) w p pt 7 ps 2 lt palette t ''

#    pause -1

}

unset multiplot

unset output
