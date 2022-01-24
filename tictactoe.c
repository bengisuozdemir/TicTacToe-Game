#include <stdio.h>
#include <pthread.h>
#include <assert.h>
#include <time.h>
#include <stdlib.h>

int mypri = 1;
int isAvailable = 1;
int num;
int so_far = 0;
int **myBoard;
pthread_mutex_t mutex;
char whoIsWinner = 'T';

int winner(int sign)
{
    int counter = 0;
    for (int i = 0; i < num; i++)
    {
        counter = 0;
        for (int j = 0; j < num; j++)
        {
            if (myBoard[i][j] == sign)
            {
            counter++;
            }
        }
        if (counter == num)
        {
        return 1;
        }
        else
        {
        counter = 0;
        }
    }
    for (int i = 0; i < num; i++)
    {
        counter = 0;
        for (int j = 0; j < num; j++)
        {
            if (myBoard[j][i]==sign)
            {
                counter++;
            }
        }
        if (counter == num)
            {
            return 1;
        }
        else
        {
            counter = 0;
        }
    }
    for (int i = 0; i < num; i++)
    {
        if (myBoard[i][i] == sign)
        {
            counter++;
        }
        else
        {
            counter = 0;
            break;
        }
    }
    if (counter == num)
    {
        return 1;
    }
    for (int i = 0; i < num; i++)
    {
        if (myBoard[i][num - i - 1] == sign)
        {
            counter++;
        }
        else
        {
            counter = 0;
            break;
        }
    }
    if (counter == num)
    {
        return 1;
    }
    return 0;
}


void *mythread(void* arg)
{
  char *args = (char *)arg;
  char p = *args;
	int sign;
	if (p == 'X')
	{
		sign = 1;
	}
	else
	{
		sign = 9;
	}
  while (isAvailable)
  {
	  pthread_mutex_lock(&mutex);
    if (whoIsWinner != 'T' || so_far == num * num)
		{
      pthread_mutex_unlock(&mutex);
			return NULL;
		}
    if ((sign == 1 && mypri == 9) || (sign == 9 && mypri == 1))
    {
      pthread_mutex_unlock(&mutex);
    }
    else
    {
		while (1)
		{
        int r = rand() % num;
        int c = rand() % num;
        if (so_far == num * num)
        {
            pthread_mutex_unlock(&mutex);
            return NULL;
        }
        if (myBoard[r][c] == 0)
        {
            if (p == 'X')
            {
                mypri = 9;
                myBoard[r][c] = 1;
                printf("Player %c played on: (%d,%d)\n", p, r, c);
                so_far++;
                if (winner(sign) == 1)
                {
                    whoIsWinner = 'X';
                    pthread_mutex_unlock(&mutex);
                    return NULL;
                }
                pthread_mutex_unlock(&mutex);
                break;
            }
            else if (p == 'O')
            {
                mypri = 1;
                myBoard[r][c] = 9;
                printf("Player %c played on: (%d,%d)\n", p, r, c);
                so_far++;
                if (winner(sign) == 1)
                {
                    whoIsWinner = 'O';
                    pthread_mutex_unlock(&mutex);
                    return NULL;
                }
                pthread_mutex_unlock(&mutex);
                break;
            }
  		}
      }
    }
  }
}

int main(int argc, char *argv[])
{
    srand(time(NULL));
    num = atoi(argv[1]);
    printf("Board Size: %dX%d\n", num, num);
    myBoard = (int **)malloc(num * sizeof(int *));
    for (int i = 0; i < num; i++)
    {
    myBoard[i] = (int *)malloc(num * sizeof(int));
    }
    for (int i = 0; i < num; i++)
    {
    for (int j = 0; j < num; j++)
    {
        myBoard[i][j] = 0;
    }
    }
    char X = 'X';
    char O = 'O';
    pthread_t p1, p2;
    pthread_create(&p1, NULL, mythread, &X);
    pthread_create(&p2, NULL, mythread, &O);
    pthread_join(p1, NULL);
    pthread_join(p2, NULL);
    printf("Game end\n");
    if (whoIsWinner == 'X')
    {
    printf("Winner is X\n");
    }
    else if (whoIsWinner == 'O')
    {
    printf("Winner is O\n");
    }
    else
    {
    printf("It is a tie\n");
    }
    for (int i = 0; i < num; i++)
    {
    for (int j = 0; j < num; j++)
    {
        if (myBoard[i][j] == 1)
        {
        printf("[X]");
        }
            else if (myBoard[i][j] == 9)
        {
        printf("[O]");
        }
            else if (myBoard[i][j] == 0)
        {
        printf("[ ]");
        }
    }
        printf("\n");
    }
    return 0;
}