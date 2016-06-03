#include<stdio.h>
#include<json.h>
#include<stdlib.h>
#include<regex.h>
#include<string.h>
#include<curl/curl.h>

// Needs to be checked for errors, it's not working
/*
int is_valid_url(char* url) {
  char* url_regx = "dadsa";
  regex_t regex;
  int ret = regcomp(&regex, "(^(([^:\/?#]+):)?(//([^\/?#]*))?([^?#]*)(\?([^#]*))?(#(.*))?)", REG_EXTENDED);
  ret = regexec(&regex, url, 0, NULL, 0);
  regfree(&regex);
  if (!ret) {
    return 1;
  } else if (ret == REG_NOMATCH) {
    return 0;
  } else {
    return -1;
  }
}
*/

struct string {
  char* res;
  size_t len;
};

void init_string(struct string *s) {
  s->len = 0;
  s->res = malloc(s->len+1);
  if (s->res == NULL) {
    fprintf(stderr, "malloc() has failed\n");
    exit(EXIT_FAILURE);
  }
  s->res[0]='\0';
}


size_t writeOutput(void *ptr, size_t size, size_t nmemb, struct string* s) {
  size_t new_len = s->len+size*nmemb;
  s->res = realloc(s->res, new_len+1);
  if (s->res == NULL) {
    fprintf(stderr, "realloc() failed\n");
    exit(EXIT_FAILURE);
  }
  memcpy(s->res+s->len, ptr, size*nmemb);
  s->res[new_len] = '\0';
  s->len = new_len;
  return size*nmemb;
}

int main(int argc, char *argv[]) {

  if (argv[1] == NULL) {
    printf("Please enter a URL too be shortened");
    return(1);
  }

  struct json_object *req_body,*res_body,*short_url;
  struct string res;
  init_string(&res);
  CURL* handle;
  CURLcode rcode;
  char *url = "https://www.googleapis.com/urlshortener/v1/url?key=YOUR_API_KEY";
  struct curl_slist *headers = NULL;

  handle = curl_easy_init();
  req_body = json_object_new_object();
  res_body = json_object_new_object();
  short_url = json_object_new_object();
  json_object_object_add(req_body, "longUrl", json_object_new_string(argv[1]));
  headers = curl_slist_append(headers, "Content-Type: application/json");
  headers = curl_slist_append(headers, "Accept: application/json");
  curl_easy_setopt(handle, CURLOPT_URL, url);
  curl_easy_setopt(handle, CURLOPT_SSL_VERIFYPEER, 0L);
  curl_easy_setopt(handle, CURLOPT_SSL_VERIFYHOST, 0L);
  curl_easy_setopt(handle, CURLOPT_CUSTOMREQUEST, "POST");
  curl_easy_setopt(handle, CURLOPT_HTTPHEADER, headers);
  curl_easy_setopt(handle, CURLOPT_POSTFIELDS, json_object_to_json_string(req_body));
  curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, writeOutput);
  curl_easy_setopt(handle, CURLOPT_WRITEDATA, &res);
  rcode = curl_easy_perform(handle);
  if (rcode != CURLE_OK) {
    fprintf(stderr, "curl_easy_perform() failed : %s .\n", curl_easy_strerror(rcode));
  }
  res_body = json_tokener_parse(res.res);
  json_object_object_get_ex(res_body, "id", &short_url);
  printf("%s\n", json_object_to_json_string_ext(short_url, JSON_C_TO_STRING_SPACED | JSON_C_TO_STRING_PRETTY));
  free(res.res);
  curl_easy_cleanup(handle);
  return 0;
}
