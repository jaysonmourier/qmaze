#include <stdio.h>
#include <stdlib.h>
#include "env.h"

static void _updatePlayerPosition(struct Env *env, int newX, int newY)
{
    if (env == NULL)
        return;

    if ((newX < 0) || (newX >= env->cols) ||
        (newY < 0) || (newY >= env->rows))
        return;

    env->data[env->playerY + env->playerX * env->cols] = 0;
    env->data[newY + newX * env->cols] = 1;
    env->playerX = newX;
    env->playerY = newY;
}

struct Env *create_env(int rows, int cols, int playerX, int playerY, int exitX, int exitY)
{
    if ((rows <= 0) ||
        (cols <= 0) ||
        (playerX < 0) ||
        (playerY < 0) ||
        (exitX < 0) ||
        (exitY < 0) ||
        (playerX >= cols) ||
        (playerY >= rows) ||
        (exitX >= cols) ||
        (exitY >= rows) ||
        ((playerX == exitX) && (playerY == exitY)))
    {
        return NULL;
    }

    struct Env *env = (struct Env *)malloc(sizeof(struct Env));
    if (env == NULL)
    {
        return NULL;
    }

    env->data = (int *)calloc(rows * cols, sizeof(int));
    if (env->data == NULL)
    {
        free(env);
        return NULL;
    }

    env->rows = rows;
    env->cols = cols;
    env->playerX = playerX;
    env->playerY = playerY;
    env->initialPlayerX = playerX;
    env->initialPlayerY = playerY;
    env->exitX = exitX;
    env->exitY = exitY;

    env->data[playerY + playerX * cols] = 1;
    env->data[exitY + exitX * cols] = 2;

    return env;
}

void destroy_env(struct Env *env)
{
    if (env != NULL)
    {
        if (env->data != NULL)
        {
            free(env->data);
            env->data = NULL;
        }
        free(env);
        env = NULL;
    }
}

struct Env *create_env_from_file(const char *path)
{
    int rows, cols, data;
    FILE *file = fopen(path, "r");
    if (file == NULL)
    {
        fprintf(stderr, "unable to open the file...\n");
        return NULL;
    }

    struct Env *env = (struct Env *)malloc(sizeof(struct Env));
    if (env == NULL)
    {
        fprintf(stderr, "unable to allocate memory for the env...\n");
        fclose(file);
        return NULL;
    }

    if (fscanf(file, "rows=%d\ncols=%d", &rows, &cols) != 2 || rows <= 0 || cols <= 0)
    {
        fprintf(stderr, "Incorrect or corrupt file format...\n");
        fclose(file);
        destroy_env(env);
        return NULL;
    }

    env->data = (int *)calloc(rows * cols, sizeof(int));
    if (env->data == NULL)
    {
        fprintf(stderr, "unable to allocate memory for the env data...\n");
        fclose(file);
        destroy_env(env);
        return NULL;
    }

    env->initialPlayerX = -1;
    env->initialPlayerY = -1;
    env->playerX = -1;
    env->playerY = -1;
    env->exitX = -1;
    env->exitY = -1;
    env->rows = rows;
    env->cols = cols;

    int i = 0;
    int j = 0;

    while (fscanf(file, "%d,", &data) == 1)
    {
        switch (data)
        {
        case 1:
            env->initialPlayerX = i;
            env->initialPlayerY = j;
            env->playerX = i;
            env->playerY = j;
            break;
        case 2:
            env->exitX = i;
            env->exitY = j;
            break;
        case 3:
            break;
        default:
            data = 0;
            break;
        }

        env->data[j * cols + i] = data;
        i++;
        if (i == cols)
        {
            i = 0;
            j++;
        }
    }

    fclose(file);

    if ((env->initialPlayerX < 0) ||
        (env->initialPlayerY < 0) ||
        (env->playerX < 0) ||
        (env->playerY < 0) ||
        (env->exitX < 0) ||
        (env->exitY < 0))
    {
        fprintf(stderr, "incorrect map file...\n");
        destroy_env(env);
        return NULL;
    }

    return env;
}

int reset_env(struct Env *env)
{
    if (env == NULL)
    {
        return -1;
    }
    _updatePlayerPosition(env, env->initialPlayerX, env->initialPlayerY);
    return env->playerY + env->playerX * env->cols;
}

void display_env(struct Env *env)
{
    if (env == NULL)
        return;

    for (int i = 0; i < env->rows; ++i)
    {
        for (int j = 0; j < env->cols; ++j)
        {
            fprintf(stdout, "%d ", env->data[j + i * env->cols]);
        }
        putchar('\n');
    }
}

static int _isValidAction(struct Env *env, int x, int y)
{
    if (env == NULL)
        return 0;

    if ((x >= env->cols) || (y >= env->rows) || (x < 0) || (y < 0))
        return 0;

    if (env->data[y + x * env->cols] == 3)
        return 0;

    return 1;
}

void step(struct Env *env, enum ACTION action, int *obs, int *terminated, int *reward)
{
    if (env == NULL)
        return;

    int x = env->playerX;
    int y = env->playerY;

    *reward = 0;

    switch (action)
    {
    case UP:
        if (_isValidAction(env, x - 1, y))
        {
            _updatePlayerPosition(env, x - 1, y);
            if ((env->playerX == env->exitX) && (env->playerY == env->exitY))
                *terminated = 1, *reward = 1;
        }
        break;
    case RIGHT:
        if (_isValidAction(env, x, y + 1))
        {
            _updatePlayerPosition(env, x, y + 1);
            if ((env->playerX == env->exitX) && (env->playerY == env->exitY))
                *terminated = 1, *reward = 1;
        }
        break;
    case DOWN:
        if (_isValidAction(env, x + 1, y))
        {
            _updatePlayerPosition(env, x + 1, y);
            if ((env->playerX == env->exitX) && (env->playerY == env->exitY))
                *terminated = 1, *reward = 1;
        }
        break;
    case LEFT:
        if (_isValidAction(env, x, y - 1))
        {
            _updatePlayerPosition(env, x, y - 1);
            if ((env->playerX == env->exitX) && (env->playerY == env->exitY))
                *terminated = 1, *reward = 1;
        }
        break;
    default:
        break;
    }

    *obs = env->playerY + env->playerX * env->cols;
}