#set hidden3d
set parametric
r = 1.5
z = 2.21
splot 'HexGrid.dat' u 1:2:3 w l lt 1 t '',\
      r*cos(u),r*sin(u),z w l lt 1 t '',\
      r*cos(u)+7.5,r*sin(u)+4.33,z w l lt 1 t '',\
      r*cos(u),r*sin(u)+8.66,z w l lt 1 t '',\
      r*cos(u)-7.5,r*sin(u)+4.33,z w l lt 1 t '',\
      r*cos(u)-7.5,r*sin(u)-4.33,z w l lt 1 t '',\
      r*cos(u),r*sin(u)-8.66,z w l lt 1 t '',\
      r*cos(u)+7.5,r*sin(u)-4.33,z w l lt 1 t '',\
      'sl.dat' u 1:2:3 w l lt 3 t ''
