# qmaze
Simple Q-Learning algorithm written in C to solve a maze.

$Q(s, a) = (1 - \alpha)Q(s, a) + \alpha(r + \gamma\max_{a'}Q(s', a'))$
