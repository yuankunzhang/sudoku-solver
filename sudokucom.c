#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sudokucom.h"

#define BASEURL "https://sudoku.com"

struct resp_buf {
    char *data;
    size_t size;
};

size_t write_resp_data(void *ptr, size_t size, size_t nmemb,
                       struct resp_buf *buf) {
    size_t s = size * nmemb;

    buf->data = realloc(buf->data, buf->size + s + 1);
    memcpy(buf->data + buf->size, ptr, s);
    buf->size += s;
    buf->data[buf->size] = 0;

    return s;
}

bool get_puzzle(char *puzzle, const char *difficulty) {
    char url[30];
    char *str;
    CURL *curl;
    CURLcode res;
    struct curl_slist *chunk = NULL;
    struct resp_buf buf;

    if (strcmp(difficulty, "easy") != 0 && strcmp(difficulty, "medium") != 0 &&
        strcmp(difficulty, "hard") != 0 && strcmp(difficulty, "expert") != 0 &&
        strcmp(difficulty, "evil") != 0) {
        fprintf(stderr, "invalid difficulty string\n");
        return false;
    }

    buf.size = 0;
    buf.data = malloc(1);

    sprintf(url, "%s/api/level/%s", BASEURL, difficulty);
    chunk = curl_slist_append(chunk, "x-requested-with: XMLHttpRequest");

    curl = curl_easy_init();
    if (!curl) {
        fprintf(stderr, "can't initialize curl\n");
        return false;
    }

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_resp_data);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buf);
    res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform() failed: %s\n",
                curl_easy_strerror(res));
        goto errout;
    }

    if ((str = strstr(buf.data, "mission")) == NULL) {
        fprintf(stderr, "invalid response\n");
        return false;
    }
    memcpy(puzzle, str + 10, 81);
    puzzle[81] = 0;

    free(buf.data);
    curl_slist_free_all(chunk);
    curl_easy_cleanup(curl);
    return true;

errout:
    curl_slist_free_all(chunk);
    curl_easy_cleanup(curl);
    return false;
}
