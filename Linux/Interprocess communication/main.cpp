#include <iostream>

#include <string>

#include <sys/ipc.h>

#include <sys/msg.h>

#include <sys/types.h>

#include <stdlib.h>

#include <time.h>

#include <unistd.h>

#include <semaphore.h>

#include <signal.h>

using namespace std;

typedef struct mbuf {

  int number;
  long mtype;

  string text;
}
msg;

sem_t sem1, sem2, sem3;

int main() {

  sem_init( & sem1, 0, 1);
  sem_init( & sem2, 0, 1);

  sem_init( & sem3, 0, 1);
  pid_t p1, p2, p3;

  msg msg1, buf1, msg2, buf2, msg3, buf3, ch1, ch2, ch3, par1, par2, par3;
  int length = sizeof(msg) - sizeof(long);

  int msgKey1 = ftok("1", 1);
  int msgKey2 = ftok("2", 2);

  int msgKey3 = ftok("3", 3);
  int q1 = msgget(msgKey1, IPC_CREAT | 0660);

  int q2 = msgget(msgKey2, IPC_CREAT | 0660);
  int q3 = msgget(msgKey3, IPC_CREAT | 0660);

  p1 = fork();
  if (p1 == 0)

  {
    srand(time(NULL));

    int n = rand() % 15;

    string t;
    cout << "Enter: ";

    msg1.info = t;
    msg1.number = n;

    sem_wait( & sem1);
    msgsnd(q1, & msg1, length, 0);

    sem_post( & sem1);
    usleep(100);

    sem_wait( & sem1);
    int received1 = msgrcv(q1, & ch1, length, 0, IPC_NOWAIT);
    if (received1 != -1)

    {
      cout << ch1.text << endl;
    }

    sem_post( & sem1);
  } else {

    p2 = fork();
    if (p2 == 0)

    {
      srand(time(NULL));

      int n = rand() % 15;
      string t;

      cout << "Enter: ";
      msg2.info = t;

      msg2.number = n;
      sem_wait( & sem2);

      msgsnd(q2, & msg2, length, 0);
      sem_post( & sem2);

      usleep(100);
      sem_wait( & sem2);

      int received2 = msgrcv(q2, & ch2, length, 0, IPC_NOWAIT);
      if (received2 != -1)

      {
        cout << ch2.text << endl;

      }
      sem_post( & sem2);

    } else {

      p3 = fork();

      if (p3 == 0)

      {
        srand(time(NULL));

        int n = rand() % 15;
        string t;

        cout << "Enter: ";
        msg3.info = t;

        msg3.number = n;
        sem_wait( & sem3);

        msgsnd(q3, & msg3, length, 0);
        sem_post( & sem3);

        usleep(100);

        sem_wait( & sem3);
        int received3 = msgrcv(q3, & ch3, length, 0, IPC_NOWAIT);

        if (received3 != -1) {
          cout << ch3.text << endl;
        }

        sem_post( & sem3);

      } else {
        string text[15] = {
          "Peya",
          "Egor",
          "Vasya",
          "Kolya",
          "Christina",
          "Ilya",
          "Denis",
          "Mitya",
          "Alina",
          "Anya",
          "Igor",
          "Vlad",
          "Katya",
          "Amina",
          "Sergey"
        };

        while (1) {
          usleep(100);

          sem_wait( & sem1);
          int received1 = msgrcv(q1, & par1, length, 0, IPC_NOWAIT);

          if (received1 != -1) {

            if (text[par1.number] != "") {
              msg1.text = text[par1.number];
              text[par1.number] = received1;
              msgsnd(q1, & msg1, length, 0);
            }
          }

          sem_post( & sem1);

          usleep(100);
          sem_wait( & sem2);

          int received2 = msgrcv(q2, & par2, length, 0, IPC_NOWAIT);
          if (received2 != -1)

          {
            if (text[par2.number] != "")

            {
              msg2.text = text[par2.number];

              text[par2.number] = received2;
              msgsnd(q2, & msg2, length, 0);

            }

          }
          sem_post( & sem2);

          usleep(100);
          sem_wait( & sem3);

          int received3 = msgrcv(q3, & par3, length, 0, IPC_NOWAIT);
          if (received3 != -1)

          {
            if (text[par3.number] != "")

            {
              msg3.text = text[par3.number];

              text[par3.number] = received3;
              msgsnd(q3, & msg3, length, 0);

            }

          }

        }

      }
    }

  }
  return 0;

}
