#include <glib.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>

#include "msg_queue.h"

gboolean on_idle(gpointer data)
{
    DimeClient* c = (DimeClient*)data;
    if (dime_mq_client_send(c, DIME_MSG_FLAG_SYNC, MSG_INPUT, 'z', 0) < 0) {
        return TRUE;
    }
    dime_mq_client_send(c, 0, MSG_INPUT, 'g', 0);
    dime_mq_client_send(c, 0, MSG_INPUT, 'r', 0);
    dime_mq_release_token(c);
    return FALSE;
}

int main(int argc, char *argv[])
{
    GMainLoop *l = g_main_loop_new(NULL, TRUE);

    pid_t pid = fork();
    if (pid == 0) {
        DimeClient* c = dime_mq_acquire_token();
        g_idle_add(on_idle, c);

    } else if (pid > 0) {
        DimeServer* s = dime_mq_server_new();

    } else {
        return -1;
    }

    g_main_loop_run(l);
    return 0;
}
