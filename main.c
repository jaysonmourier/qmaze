#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "env.h"
#include "qlearning.h"

#define ROWS 15
#define COLS 15
#define N_ACTIONS 4
#define PLAYERX 0
#define PLAYERY 0
#define EXITX 8
#define EXITY 8

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

    if(terminated == 1)
    {
        printf("YOU WON!\n");
    } else
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

    struct QTable *qtable = create_qtable(ROWS * COLS, N_ACTIONS);
    if (qtable == NULL)
    {
        fprintf(stderr, "unable to create qtable...\n");
        destroy_env(env);
        return 1;
    }

    int oldObs = reset_env(env);
    int newObs = oldObs;
    int terminated = 0;
    int reward = 0;

    float alpha = 0.005;
    float gamma = 0.9;
    float epsilon = 0.95;

    int epochs = 20000;

    printf(":::TRAINING\n");
    for (int epoch = 0; epoch < epochs; ++epoch)
    {
        terminated = 0;
        oldObs = reset_env(env);

        if ((epoch > 0) && (epoch % 1000 == 0))
        {
            epsilon = epsilon * 0.95;
            printf("EPSILON => %f\n", epsilon);
        }

        if ((epoch > 0) && (epoch % 500 == 0))
        {
            printf("%5d / %5d\n", epoch, epochs);
        }

        while (terminated == 0)
        {
            int action;

            if (rand() / RAND_MAX > epsilon)
            {
                action = get_best_action(qtable, oldObs);
            }
            else
            {
                action = rand() % 4;
            }

            step(env, action, &newObs, &terminated, &reward);

            float value = (1 - alpha) * get_state_action_value(qtable, oldObs, action) + alpha * (reward + gamma * get_max_state_value(qtable, newObs));

            set_state_action_value(qtable, oldObs, action, value);

            oldObs = newObs;
        }
    }

    putchar('\n');
    printf(":::PLAYING\n");
    play(env, qtable, 50);
    putchar('\n');

    printf(":::QTABLE\n");
    display_qtable(qtable);

    destroy_env(env);
    destroy_qtable(qtable);
    return 0;
}