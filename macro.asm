jROG start 50
using *,15
MACRO
&a0 test &a1,&a2
SR 4,4
l 4,&a2
S 4,&a1
MEND
	MACRO 
&some_other_loop sample &m
sr 1,1
l 1,1
MEND
st 4,RES
a 4,=F'4'
a 4,=F'4'
test loop,D2,D4
D1 dc F'8'
D2 dc F'4'
D3 dc F'16'
D4 dc F'8'
RES ds 2F
really sample argument

