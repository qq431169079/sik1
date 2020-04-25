#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "err.h"

#define ISBLANK(x)  (int)((((unsigned char)x) == ' ') ||        \
                          (((unsigned char)x) == '\t'))
#define COOKIES_MAX_SIZE 4096
#define MAX_COOKIE_SIZE 4096

size_t get_cookie_len(char *cookie, size_t cookie_tab_len) {
    char *cookie_ptr = cookie;
    while(cookie_ptr - cookie < cookie_tab_len && cookie_ptr - cookie < MAX_COOKIE_SIZE && cookie_ptr && !ISBLANK(cookie_ptr))
        cookie_ptr++;
    return cookie_ptr - cookie;
}

size_t handle_cookie(char header[], size_t header_len) {
    char *cookie_ptr;
    if (cookie_ptr = strstr(header, "Set-Cookie:") == 0) {
        return 0;
    }
    cookie_ptr += 11;
    while (cookie_ptr - header < header_len && *cookie_ptr && ISBLANK(*cookie_ptr)) {
        cookie_ptr++;
    }
    printf("%s", get_cookie_len(cookie_ptr, header_len - (cookie_ptr - header)), cookie_ptr);
    return 1;
}



FILE *get_file_desc(char *file_name){
    FILE *cookies_file;
    if (!(cookies_file = fopen(file_name, "r"))) {
        fclose(cookies_file);
        fatal("Nie ma takiego pliku %s", file_name);
    }
    return cookies_file;
}

size_t read_cookies(FILE **cookies_file, char cookies[], size_t *cookies_string_len) {
    char * cookie_line = NULL;
    size_t cookies_len = 0;
    size_t cookie_line_len = 0;
    ssize_t read, len;
    int read_sth = 0;
    while ((read = getline(&cookie_line, &len, *cookies_file)) != -1) {
        read_sth = 1;
        cookie_line_len = strlen(cookie_line);
        if (cookie_line_len < COOKIES_MAX_SIZE - cookies_len) {
            strcpy(cookies + cookies_len, cookie_line);
            cookies[cookies_len + cookie_line_len - 1] = ';';
            cookies[cookies_len + cookie_line_len] = ' ';
            cookies_len += cookie_line_len + 1;
        }
        else{
            //tu trzba przesunac wskaznik pliku z powrotem
            break;
        }
    }
    printf("%s", cookies);
    *cookies_string_len = strlen(cookies);
    return read_sth && (cookie_line_len < COOKIES_MAX_SIZE - cookies_len);
}



//struct CookieInfo *Curl_cookie_init(struct Curl_easy *data,
//                                    const char *file,
//                                    struct CookieInfo *inc,
//                                    bool newsession) {
//    struct CookieInfo *c;
//    FILE *fp = NULL;
//    bool fromfile = TRUE;
//    char *line = NULL;
//
//    if (NULL == inc) {
//        /* we didn't get a struct, create one */
//        c = calloc(1, sizeof(struct CookieInfo));
//        if (!c)
//            return NULL; /* failed to get memory */
//        c->filename = strdup(file ? file : "none"); /* copy the name just in case */
//        if (!c->filename)
//            goto fail; /* failed to get memory */
//    } else {
//        /* we got an already existing one, use that */
//        c = inc;
//    }
//    c->running = FALSE; /* this is not running, this is init */
//
//    if (file && !strcmp(file, "-")) {
//        fp = stdin;
//        fromfile = FALSE;
//    } else if (file && !*file) {
//        /* points to a "" string */
//        fp = NULL;
//    } else
//        fp = file ? fopen(file, FOPEN_READTEXT) : NULL;
//
//    c->newsession = newsession; /* new session? */
//
//    if (fp) {
//        char *lineptr;
//        bool headerline;
//
//        line = malloc(MAX_COOKIE_LINE);
//        if (!line)
//            goto fail;
//        while (Curl_get_line(line, MAX_COOKIE_LINE, fp)) {
//            if (checkprefix("Set-Cookie:", line)) {
//                /* This is a cookie line, get it! */
//                lineptr = &line[11];
//                headerline = TRUE;
//            } else {
//                lineptr = line;
//                headerline = FALSE;
//            }
//            while (*lineptr && ISBLANK(*lineptr))
//                lineptr++;
//
//            Curl_cookie_add(data, c, headerline, TRUE, lineptr, NULL, NULL, TRUE);
//        }
//        free(line); /* free the line buffer */
//        remove_expired(c); /* run this once, not on every cookie */
//
//        if (fromfile)
//            fclose(fp);
//    }
//
//    c->running = TRUE;          /* now, we're running */
//    if (data)
//        data->state.cookie_engine = TRUE;
//
//    return c;
//
//    fail:
//    free(line);
//    if (!inc)
//        /* Only clean up if we allocated it here, as the original could still be in
//         * use by a share handle */
//        Curl_cookie_cleanup(c);
//    if (fromfile && fp)
//        fclose(fp);
//    return NULL; /* out of memory */
//}
