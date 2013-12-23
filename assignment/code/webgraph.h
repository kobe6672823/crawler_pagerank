#ifndef _WEBGRAPH_H_
#define _WEBGRAPH_H_


init_webg();// 初始化web graph
insert_vertex(char * url);// 加入一个顶点
has_vertex(char *url);// 判断给定顶点是否在graph中
insert_edge();// 插入一条边到graph中
vertex_size();// graph的顶点数
calc_ind_cdf();// 计算所有顶点的入度的累积分布
calc_pagerank();// 计算所有顶点的pagerank
find_ind(char *url);// 查找给定顶点的入度
gen_graphviz();// 生成描述图拓扑的graphviz脚本
destroy_webg();// 销毁web graph


#endif

