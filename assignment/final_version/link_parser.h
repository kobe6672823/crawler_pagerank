#ifndef _LINKPARSER_H
#define _LINKPARSER_H

//相当与：<[:blank:]*a[^>]>
#define PATTERN "<[[:blank:]]*a[^>]*href[[:blank:]]*=[[:blank:]]*['\"]([^'\"]*)['\"][^>]*>"
#define LINK_LEN 512
#define IGNORE_THIS_URL 0
#define SUCCESS 1

int is_rela_path_html(char *link);
int extract_link(char *htmltxt, char **url_list, char *cur_url, char *cur_path);
int get_real_path_link(char *cur_url, char *link);

#endif

