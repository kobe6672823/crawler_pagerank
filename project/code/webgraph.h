#ifndef _WEBGRAPH_H_
#define _WEBGRAPH_H_

#define GRAPH_SIZE 400000
#define HASH_TABLE_SIZE 400000  //(the 13th prime int)
#define LINK_LEN 512
#define INIT_FAIL 0
#define INIT_SUCCEED 1
#define NOT_IN_SET -1

struct node
{
	char *url;
	int number;//url编号，由1开始
	struct node *next_ptr;
};//hash_table的节点

struct web_graph
{
	struct node *vertex_set;
	char **all_url_list;
	int **edge_set;
	int all_url_list_size;
	int *ind;//这个一维数组用来统计顶点的入度
	int *existed_page;//这个一维数组用来统计页面是否存在（若返回200就记为1）
	int existed_page_size;//统计一共有多少个page是存在的
	int edge_set_size;
};

/*************************************
下面是对web_grah主要的5个数据结构的说明：

1：vertex_set（点集）:
是一个hash_table，主要用了struct node去构造这个hash_table
node里面的url用于存储点的内容，即具体url
node里面的number用于存储点的编号（由1开始编号）
这个hash_table可以用于快速判断一个点是否在点集内

2：all_url_list(存储所有url的一个表，有了这个表就可以在知道编号的情况下快速得到url的具体内容了
**************************
|1|url1内容，如index.html|
|2|url2内容，如index.html|
.......
**************************
需要注意的是编号从1开始算

3：edge_set（边集），是一个稀疏矩阵
边集是一个二维数组，第一行存的是一号url的边集（去重后的），第一个数字是它的出度n，后n个数字是由这个url出去的目标url的编号
| 出度n | n个数字（出去的n个url）

4:all_url_list_size记录现在已经给多少个url做编号了（注意由1开始编号）

5:ind是一个一维数组，用于统计各个url的入度
*************************************/

int init_webg(struct web_graph *webg);// 初始化web graph
int insert_vertex(struct web_graph *webg, char * url);// 加入一个顶点,不成功返回-1，成功返回插入的顶点的编号
int has_vertex(struct web_graph *webg, char *url);// 判断给定顶点是否在graph中，若找到返回顶点编号，若没找到，返回-1
void insert_edge(struct web_graph *webg, int src_num, int dst_num, int pos);// 插入一条边到graph中
int vertex_size(struct web_graph *webg);// graph的顶点数
//calc_ind_cdf();// 计算所有顶点的入度的累积分布
//calc_pagerank();// 计算所有顶点的pagerank
//find_ind(char *url);// 查找给定顶点的入度
//gen_graphviz();// 生成描述图拓扑的graphviz脚本
void destroy_webg(struct web_graph *webg);// 销毁web graph
void destroy_list(struct node *head_ptr);//对链表内存的释放
void print_webg_to_file(struct web_graph *webg);
void output_result_file(int argc, char *argv[]);
#endif

