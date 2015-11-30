fn = '../data/scan0.dat'

set output "1.eps"
set terminal postscript eps enhanced color size 8.5,11 "Helvetica" 16

unset colorbox
set border lw 0.1

set xrange [0:11]
set yrange [-1000:0]
set cbrange [0:1.0]

unset xlabel
set ylabel 'Voltage on Focusing Electrode [V]'
set cblabel 'Collection efficiency'

set multiplot layout 3,2 rowsfirst title "Hex edge=5mm, cell height=20mm, Vcathode=-1000V, #streamlines=271, dTM=1mm\nSquare shows collection within TM, circle shows collection within d=2mm" \
    margins screen 0.1, 0.9, 0.1, 0.9 spacing screen 0.05

do for [diaFe = 3:8:1] {

    if (diaFe > 3) {
        unset xlabel
        unset ylabel
        unset colorbox
    }
    if (diaFe == 4) {
        set colorbox
    }
    if (diaFe == 7) {
        set xlabel 'Distance from Topmetal to Focusing Electrode [mm]'
    }
    set title sprintf('Hole diameter %.1fmm in Focusing Electrode', diaFe)

    plot fn u ($2==diaFe?$3:1/0):4:($6/$5) w p pt 5 ps 5 lt palette t '', '' u ($2==diaFe?$3:1/0):4:($7/$5) w p pt 7 ps 4 lt palette t ''

#    pause -1

}

unset multiplot

unset output
