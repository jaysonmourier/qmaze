# qmaze
Double Q-Learning algorithm written in C to solve a maze.

$Q_{t+1}^A(s_t, a_t) = Q_{t+1}^A(s_t, a_t) + \alpha (r_t + \gamma Q_t^B(s_{t+1}, \argmax_a Q_t^A(s_{t+1}, a) - Q_t^A(s_t, a_t)))$


$Q_{t+1}^B(s_t, a_t) = Q_{t+1}^B(s_t, a_t) + \alpha (r_t + \gamma Q_t^A(s_{t+1}, \argmax_a Q_t^B(s_{t+1}, a) - Q_t^B(s_t, a_t)))$
