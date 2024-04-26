#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "env.h"
#include "qlearning.h"

#define ROWS 10
#define COLS 10
#define N_ACTIONS 4
#define PLAYERX 0
#define PLAYERY 0
#define EXITX 9
#define EXITY 9

void play(struct Env *env, struct QTable *qtable, int max_steps)
{
    int obs = reset_env(env);
    int terminated = 0;
    int reward = 0;

    int i = 0;

    while (terminated == 0)
    {
        display_env(env);
        putchar('\n');
        int action = get_best_action(qtable, obs);
        step(env, action, &obs, &terminated, &reward);

        if (i++ > max_steps)
            break;
    }

    display_env(env);
    putchar('\n');

    if (terminated == 1)
    {
        printf("YOU WON!\n");
    }
    else
    {
        printf("YOU LOST!\n");
    }
}

int main(void)
{
    srand(time(NULL));

    struct Env *env = create_env(ROWS, COLS, PLAYERX, PLAYERY, EXITX, EXITY);

    if (env == NULL)
    {
        fprintf(stderr, "unable to create env...\n");
        return 1;
    }

    struct QTable *qa = create_qtable(ROWS * COLS, N_ACTIONS);
    if (qa == NULL)
    {
        fprintf(stderr, "unable to create qtable...\n");
        destroy_env(env);
        return 1;
    }

    struct QTable *qb = create_qtable(ROWS * COLS, N_ACTIONS);
    if (qb == NULL)
    {
        fprintf(stderr, "unable to create qtable...\n");
        destroy_env(env);
        destroy_qtable(qa);
        return 1;
    }

    int oldObs = reset_env(env);
    int newObs = oldObs;
    int terminated = 0;
    int reward = 0;

    float alpha = 0.005;
    float gamma = 0.9;
    float epsilon = 0.95;

    int epochs = 500;

    printf(":::TRAINING\n");
    for (int epoch = 0; epoch < epochs; ++epoch)
    {
        terminated = 0;
        oldObs = reset_env(env);

        if ((epoch > 0) && (epoch % 50 == 0))
        {
            epsilon = epsilon * 0.95;
            printf("EPSILON => %f\n", epsilon);
        }

        if ((epoch > 0) && (epoch % 100 == 0))
        {
            printf("%5d / %5d\n", epoch, epochs);
        }

        while (terminated == 0)
        {
            int action;

            if ((float)rand() / RAND_MAX > epsilon)
            {
                action = get_best_action(qa, oldObs);
            }
            else
            {
                action = (int)rand() % N_ACTIONS;
            }

            step(env, action, &newObs, &terminated, &reward);

            int best_action_qa = get_best_action(qa, newObs);
            float value1 = get_state_action_value(qa, oldObs, action) + alpha * (reward + gamma * get_state_action_value(qb, newObs, best_action_qa) - get_state_action_value(qa, oldObs, action));

            int best_action_qb = get_best_action(qb, newObs);
            float value2 = get_state_action_value(qb, oldObs, action) + alpha * (reward + gamma * get_state_action_value(qa, newObs, best_action_qb) - get_state_action_value(qb, oldObs, action));

            set_state_action_value(qa, oldObs, action, value1);
            set_state_action_value(qb, oldObs, action, value2);

            oldObs = newObs;
        }
    }

    putchar('\n');
    printf(":::PLAYING\n");
    play(env, qa, 50);
    putchar('\n');

    printf(":::QTABLE\n");
    display_qtable(qa);

    destroy_env(env);
    destroy_qtable(qa);
    destroy_qtable(qb);
    return 0;
}