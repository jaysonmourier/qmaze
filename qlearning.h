#ifndef QLEARNING_H
#define QLEARNING_H

struct QTable {
    int observations_space;
    int actions_space;
    float *data;
};

struct QTable *create_qtable(int observations_space, int actions_space);
void destroy_qtable(struct QTable *qtable);

void set_state_action_value(struct QTable *qtable, int state, int action, float value);
float get_state_action_value(struct QTable *qtable, int state, int action);
float get_max_state_value(struct QTable *qtable, int state);
int get_best_action(struct QTable *qtable, int state);

void display_qtable(struct QTable *qtable);

#endif