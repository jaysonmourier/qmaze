#ifndef ENV_H
#define ENV_H

struct Env
{
    int rows;
    int cols;
    int playerX;
    int playerY;
    int initialPlayerX;
    int initialPlayerY;
    int exitX;
    int exitY;
    int *data;
};

enum ACTION {UP, RIGHT, DOWN, LEFT};

struct Env *create_env(int rows, int cols, int playerX, int playerY, int exitX, int exitY);
void destroy_env(struct Env *env);
int reset_env(struct Env *env);
void display_env(struct Env *env);
void step(struct Env *env, enum ACTION action, int *obs, int *terminated, int *reward);

#endif