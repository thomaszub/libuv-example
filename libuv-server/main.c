#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <uv.h>

#define DEFAULT_BACKLOG 128

uv_loop_t *loop;
struct sockaddr_in addr;

typedef struct {
    uv_write_t req;
    uv_buf_t buf;
} write_req_t;

void free_write_req(uv_write_t *req) {
    write_req_t *wr = (write_req_t *) req;
    free(wr->buf.base);
    free(wr);
}

void alloc_buffer(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf) {
    buf->base = (char *) malloc(suggested_size);
    buf->len = suggested_size;
}

void on_close(uv_handle_t *handle) {
    printf("Verbindung geschlossen\n");
    free(handle);
}

void on_write_done(uv_write_t *req, int status) {
    if (status) {
        fprintf(stderr, "Fehler beim Schreiben: %s\n", uv_strerror(status));
    }
    free_write_req(req);
}

void on_read_started(uv_stream_t *client, ssize_t nread, const uv_buf_t *buf) {
    if (nread > 0) {
        char *message = malloc((nread + 1) * sizeof(char));
        const char sep[1] = "#";
        memcpy(message, buf->base, nread);
        message[nread] = '\0';
        printf("Message: %s", message);
        char *token = strtok(message, sep);
        while (token != NULL) {
            printf("%s\n", token);
            token = strtok(NULL, sep);
        }

        write_req_t *req = (write_req_t *) malloc(sizeof(write_req_t));
        req->buf = uv_buf_init(buf->base, nread);
        uv_write((uv_write_t *) req, client, &req->buf, 1, on_write_done);

        free(message);
        return;
    }
    if (nread < 0) {
        if (nread != UV_EOF)
            fprintf(stderr, "Fehler beim Lesen: %s\n", uv_err_name(nread));
        uv_close((uv_handle_t *) client, on_close);
    }

    free(buf->base);
}

void on_new_connection(uv_stream_t *server, int status) {
    if (status < 0) {
        fprintf(stderr, "Verbindungsfehler: %s\n", uv_strerror(status));
        return;
    }
    uv_tcp_t *client = (uv_tcp_t *) malloc(sizeof(uv_tcp_t));
    uv_tcp_init(loop, client);
    int error = uv_accept(server, (uv_stream_t *) client);
    if (error) {
        fprintf(stderr, "Neue Verbindung nicht akzeptiert: %s\n", uv_strerror(error));
        uv_close((uv_handle_t *) client, on_close);
    } else {
        printf("Neue Verbindung akzeptiert\n");
        uv_read_start((uv_stream_t *) client, alloc_buffer, on_read_started);
    }
}

int main(int argc, char **argv) {
    if (argc < 3) {
        fprintf(stderr,
                "Nicht genug Argumente. Erstes Argument muss die Addresse sein, zweites Argument der Port. Z.B. 127.0.0.1 8000\n");
        return 1;
    }
    const char *address = argv[1];
    int port;
    if (sscanf(argv[2], "%d", &port) == EOF) {
        fprintf(stderr, "Argument %s ist kein korrekter Port\n", argv[1]);
        return 1;
    }

    loop = uv_default_loop();
    uv_tcp_t server;
    uv_tcp_init(loop, &server);
    printf("Start TCP Server %s:%d\n", address, port);
    uv_ip4_addr(address, port, &addr);
    uv_tcp_bind(&server, (const struct sockaddr *) &addr, 0);
    int error = uv_listen((uv_stream_t *) &server, DEFAULT_BACKLOG, on_new_connection);
    if (error) {
        fprintf(stderr, "Fehler beim Hoeren auf Verbindungen: %s\n", uv_strerror(error));
        return 1;
    }
    return uv_run(loop, UV_RUN_DEFAULT);
}