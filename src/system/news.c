#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <curl/curl.h>
#include "info.h"

#define NEWS_URL "https://archlinux.org/feeds/news/"
#define MAX_ENTRIES 3
#define WRAP_WIDTH 120
#define INITIAL_BUFFER_SIZE 8192

typedef struct {
    char *data;
    size_t size;
} Response;

size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    Response *resp = (Response *)userp;
    char *ptr = realloc(resp->data, resp->size + realsize + 1);
    if (!ptr) return 0;
    resp->data = ptr;
    memcpy(&(resp->data[resp->size]), contents, realsize);
    resp->size += realsize;
    resp->data[resp->size] = '\0';
    return realsize;
}

char* extract_tag(const char *xml, const char *tag, const char *start_pos) {
    char open_tag[64], close_tag[64];
    snprintf(open_tag, sizeof(open_tag), "<%s>", tag);
    snprintf(close_tag, sizeof(close_tag), "</%s>", tag);
    const char *start = strstr(start_pos, open_tag);
    if (!start) return NULL;
    start += strlen(open_tag);
    const char *end = strstr(start, close_tag);
    if (!end) return NULL;
    size_t len = end - start;
    char *result = malloc(len + 1);
    if (!result) return NULL;
    strncpy(result, start, len);
    result[len] = '\0';
    return result;
}

void decode_html(char *str) {
    if (!str) return;
    struct { const char *entity; char replacement; int len; } entities[] = {
        {"&lt;", '<', 4}, {"&gt;", '>', 4}, {"&amp;", '&', 5},
        {"&quot;", '"', 6}, {"&apos;", '\'', 6}
    };
    char *src = str, *dst = str;
    while (*src) {
        if (*src == '&') {
            int replaced = 0;
            for (int i = 0; i < 5; i++) {
                if (strncmp(src, entities[i].entity, entities[i].len) == 0) {
                    *dst++ = entities[i].replacement;
                    src += entities[i].len;
                    replaced = 1;
                    break;
                }
            }
            if (!replaced) *dst++ = *src++;
        } else {
            *dst++ = *src++;
        }
    }
    *dst = '\0';
}

void strip_html_tags(char *str) {
    if (!str) return;
    char *src = str, *dst = str;
    int in_tag = 0;
    int last_was_space = 0;
    while (*src) {
        if (*src == '<') {
            in_tag = 1;
            if (strncmp(src, "</p>", 4) == 0 || strncmp(src, "<br", 3) == 0) {
                if (dst > str && *(dst-1) != '\n') {
                    *dst++ = '\n';
                    *dst++ = '\n';
                    last_was_space = 1;
                }
            }
        } else if (*src == '>') {
            in_tag = 0;
            src++;
            continue;
        } else if (!in_tag) {
            if (isspace((unsigned char)*src)) {
                if (!last_was_space) {
                    *dst++ = ' ';
                    last_was_space = 1;
                }
            } else {
                *dst++ = *src;
                last_was_space = 0;
            }
        }
        src++;
    }
    *dst = '\0';
}

void normalize_whitespace(char *str) {
    if (!str) return;
    char *src = str, *dst = str;
    int space_count = 0;
    int newline_count = 0;
    while (*src) {
        if (*src == '\n') {
            newline_count++;
            if (newline_count <= 2) {
                *dst++ = '\n';
            }
            space_count = 0;
        } else if (isspace((unsigned char)*src)) {
            space_count++;
            if (space_count == 1 && newline_count == 0) {
                *dst++ = ' ';
            }
        } else {
            *dst++ = *src;
            space_count = 0;
            newline_count = 0;
        }
        src++;
    }
    *dst = '\0';
    src = str;
    while (isspace((unsigned char)*src)) src++;
    if (src != str) {
        memmove(str, src, strlen(src) + 1);
    }
    dst = str + strlen(str) - 1;
    while (dst >= str && isspace((unsigned char)*dst)) {
        *dst-- = '\0';
    }
}

char* wrap_text(const char *text, int width) {
    if (!text) return NULL;
    size_t len = strlen(text);
    char *wrapped = malloc(len * 2);
    if (!wrapped) return NULL;
    const char *src = text;
    char *dst = wrapped;
    int col = 0;
    while (*src) {
        if (*src == '\n') {
            *dst++ = '\n';
            col = 0;
            src++;
            if (*src == '\n') {
                *dst++ = '\n';
                src++;
            }
            continue;
        }
        if (col >= width) {
            char *space = dst - 1;
            int back = 0;
            while (space > wrapped && *space != ' ' && *space != '\n' && back < 25) {
                space--;
                back++;
            }
            if (*space == ' ' && back > 0 && back < 25) {
                *space = '\n';
                col = back - 1;
            } else if (*src == ' ') {
                *dst++ = '\n';
                col = 0;
                src++;
                while (*src == ' ') src++;
                continue;
            }
        }
        *dst++ = *src++;
        col++;
    }
    *dst = '\0';
    return wrapped;
}

int append_to_buffer(char **buffer, size_t *size, size_t *capacity, const char *str) {
    size_t len = strlen(str);
    while (*size + len >= *capacity) {
        *capacity *= 2;
        char *new_buf = realloc(*buffer, *capacity);
        if (!new_buf) return 0;
        *buffer = new_buf;
    }
    strcat(*buffer, str);
    *size += len;
    return 1;
}

char* get_news(void) {
    CURL *curl = curl_easy_init();
    if (!curl) {
        return strdup("Failed to initialize curl.");
    }
    Response response = {NULL, 0};
    curl_easy_setopt(curl, CURLOPT_URL, NEWS_URL);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30L);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36");
    CURLcode res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    if (res != CURLE_OK) {
        free(response.data);
        return strdup("Failed to fetch feed.");
    }
    size_t output_size = 0;
    size_t output_capacity = INITIAL_BUFFER_SIZE;
    char *output = malloc(output_capacity);
    if (!output) {
        free(response.data);
        return strdup("Memory allocation failed.");
    }
    output[0] = '\0';
    const char *pos = response.data;
    int item_count = 0;
    while ((pos = strstr(pos, "<item>")) != NULL && item_count < MAX_ENTRIES) {
        pos += 6;
        char *title = extract_tag(response.data, "title", pos);
        char *link = extract_tag(response.data, "link", pos);
        char *pubdate = extract_tag(response.data, "pubDate", pos);
        char *description = extract_tag(response.data, "description", pos);
        if (title && link && pubdate) {
            decode_html(title);
            normalize_whitespace(title);
            char buffer[8192];
            int written = 0;
            if (item_count > 0) {
                written = snprintf(buffer, sizeof(buffer), "\n\n================================================\n\n");
            }
            written += snprintf(buffer + written, sizeof(buffer) - written, "%s\n%s\n%s\n------------------------------------------------\n", title, pubdate, link);
            if (description) {
                decode_html(description);
                strip_html_tags(description);
                normalize_whitespace(description);
                char *wrapped = wrap_text(description, WRAP_WIDTH);
                if (wrapped) {
                    written += snprintf(buffer + written, sizeof(buffer) - written, "%s\n", wrapped);
                    free(wrapped);
                }
            }
            if (!append_to_buffer(&output, &output_size, &output_capacity, buffer)) {
                free(title);
                free(link);
                free(pubdate);
                free(description);
                free(response.data);
                free(output);
                return strdup("Memory allocation failed.");
            }
            item_count++;
        }
        free(title);
        free(link);
        free(pubdate);
        free(description);
    }
    free(response.data);
    if (item_count == 0) {
        strcat(output, "No news items found.\n");
    }
    return output;
}