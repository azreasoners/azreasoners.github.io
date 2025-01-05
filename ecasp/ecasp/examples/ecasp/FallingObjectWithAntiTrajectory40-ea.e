; FallingObjectWithAntiTrajectory40-ea.e
load foundations/Root.e
load foundations/EC.e


sort object
sort agent
sort height: integer

agent Nathan
object Apple

fluent Falling(object)
fluent HasHeight(object,height)
;noninertial Height

event Drop(agent,object)
event HitGround(object)

; Sigma

[agent,object,time]
Initiates(Drop(agent,object),Falling(object),time).

[object,time]
Terminates(HitGround(object),Falling(object),time).

; Delta
[object,time]
(HoldsAt(Falling(object),time) &
HoldsAt(HasHeight(object,0),time) ->
Happens(HitGround(object),time)).

Happens(Drop(Nathan,Apple),0).

; Psi

[object,height1,height2,time]
(HoldsAt(HasHeight(object,height1),time) &
HoldsAt(HasHeight(object,height2),time) ->
height1=height2).

; Pi

[object,height1,height2,offset,time]
(HoldsAt(HasHeight(object,height1),time) &
height2 = (height1 - offset) ->
Trajectory(Falling(object),time,HasHeight(object,height2),offset)).

[object,height,offset,time]
(HoldsAt(HasHeight(object,height),time) ->
AntiTrajectory(Falling(object),time,HasHeight(object,height),offset)).

; Gamma

!HoldsAt(Falling(Apple),0).
HoldsAt(HasHeight(Apple,35),0).

[object]
!ReleasedAt(Falling(object),0).

;noninertial HasHeight
[object,height,time]
ReleasedAt(HasHeight(object,height),time).

completion Happens
;completion Delta Happens

range time 0 40
range height 0 35
range offset 1 35


; End of file.
