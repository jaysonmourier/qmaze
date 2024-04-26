#include <stdio.h>
#include <stdlib.h>
#include "qlearning.h"

struct QTable *create_qtable(int observations_space, int actions_space)
{
    if ((observations_space <= 0) || (actions_space <= 0))
        return NULL;

    struct QTable *qtable = (struct QTable *)malloc(sizeof(struct QTable));
    if (qtable == NULL)
    {
        return NULL;
    }

    qtable->data = (float *)calloc(observations_space * actions_space, sizeof(float));
    if (qtable->data == NULL)
    {
        free(qtable);
        return NULL;
    }

    qtable->observations_space = observations_space;
    qtable->actions_space = actions_space;

    return qtable;
}

void destroy_qtable(struct QTable *qtable)
{
    if (qtable != NULL)
    {
        if (qtable->data != NULL)
        {
            free(qtable->data);
            qtable->data = NULL;
        }
        free(qtable);
        qtable = NULL;
    }
}

void set_state_action_value(struct QTable *qtable, int state, int action, float value)
{
    qtable->data[action + state * qtable->actions_space] = value;
}

float get_state_action_value(struct QTable *qtable, int state, int action)
{
    if (qtable == NULL)
        return -999;

    return qtable->data[action + state * qtable->actions_space];
}

float get_max_state_value(struct QTable *qtable, int state)
{
    if (qtable == NULL)
        return -999;

    float best_value = qtable->data[0 + state * qtable->actions_space];

    for (int i = 1; i < qtable->actions_space; ++i)
    {
        float value = qtable->data[i + state * qtable->actions_space];

        if (value > best_value)
        {
            best_value = value;
        }
    }

    return best_value;
}

int get_best_action(struct QTable *qtable, int state)
{
    if (qtable == NULL)
        return -999;

    float best_value = qtable->data[0 + state * qtable->actions_space];
    int best_action = 0;

    for (int i = 1; i < qtable->actions_space; ++i)
    {
        float value = qtable->data[i + state * qtable->actions_space];

        if (value > best_value)
        {
            best_value = value;
            best_action = i;
        }
    }

    return best_action;
}

void display_qtable(struct QTable *qtable)
{
    if (qtable == NULL)
        return;
    
    for (int i = 0; i < qtable->observations_space; ++i)
    {
        for (int j = 0; j < qtable->actions_space; ++j)
        {
            fprintf(stdout, "%5f ", qtable->data[j + i * qtable->actions_space]);
        }
        putchar('\n');
    }
}