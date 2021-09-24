prog start 50
using *,15
macro test &a1,&a2
sr 4,4
l 4,&a2
s 4,&a1
mend
macro sample &m
sr 1,1
l 1,1
mend
st 4,RES
a 4,=F'4'
a 4,=F'4'
test D2,D4
D1 dc F'8'
D2 dc F'4'
D3 dc F'16'
D4 dc F'8'
RES ds 2F
