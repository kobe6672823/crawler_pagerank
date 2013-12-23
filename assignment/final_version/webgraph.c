/*****
** webgraph.c
** - implements the methods declared in webgraph.h
** - ¹¹Ôì¡¢Î¬»€ºÍŽæŽ¢Õûžöweb graphµÄ¶¥µãºÍ±ßÊýŸÝ,·ÖÎöweb graphµÄÁŽœÓ¹ØÏµ,È·±£¶àÏß³Ì°²È«·ÃÎÊ
*/
#include "webgraph.h"
#include <stdio.h>
#include <stdlib.h>
#include "hash.h"
#include <string.h>
#include <math.h>

// ³õÊŒ»¯web graph
int init_webg(struct web_graph *webg)
{
	int i = 0, j = 0;
	
	webg->ind = (int *)malloc(sizeof (int ) * GRAPH_SIZE);
	if (webg->ind == NULL)
	{
		printf("žøÈë¶ÈÊý×é·ÖÅä¿ÕŒäÊ§°Ü£¡\n");
		return INIT_FAIL;
	}
	else
	{
		for (i = 0; i < GRAPH_SIZE; i++)
			webg->ind[i] = 0;
	}

	webg->existed_page = (int *)malloc(sizeof (int ) * GRAPH_SIZE);
	if (webg->existed_page == NULL)
	{
		printf("fail to malloc space for existed_page!\n");
		free(webg->ind);
		return INIT_FAIL;
	}
	else
	{
		for (i = 0; i < GRAPH_SIZE; i++)
			webg->existed_page[i] = 0;
	}

	webg->vertex_set = (struct node*)malloc(sizeof (struct node) * HASH_TABLE_SIZE);
	if (webg->vertex_set == NULL)
	{
		free(webg->ind);
		free(webg->existed_page);
		printf("žøµãŒ¯·ÖÅä¿ÕŒäÊ§°Ü£¡\n");
		return INIT_FAIL;
	}
	for (i = 0; i < HASH_TABLE_SIZE; i++)
		webg->vertex_set[i].next_ptr = NULL;

	webg->all_url_list = (char **)malloc(sizeof (char *) * GRAPH_SIZE);
	if (webg->all_url_list == NULL)
	{
		printf("žøurlÁÐ±í·ÖÅä¿ÕŒäÊ§°Ü!\n");
		free(webg->ind); 
		free(webg->existed_page); 
		free(webg->vertex_set);
		return INIT_FAIL;
	}

	for (i = 0; i < GRAPH_SIZE; i++)
	{
		webg->all_url_list[i] = (char *)malloc(sizeof (char) * LINK_LEN);
		if (webg->all_url_list[i] == NULL)
		{
			printf("žøurlÁÐ±í·ÖÅä¿ÕŒäÊ§°Ü£¡\n");
			free(webg->ind); 
			free(webg->existed_page); 
			free(webg->vertex_set);
			for (j = 0; j < i; j++)
				free(webg->all_url_list[j]);
			free(webg->all_url_list);
			return INIT_FAIL;
		}
	}
	webg->edge_set = (int **)malloc(sizeof (int *) * GRAPH_SIZE);
	if (webg->edge_set == NULL)
	{
		free(webg->ind); 
		free(webg->existed_page); 
		free(webg->vertex_set);
		for (i = 0; i < GRAPH_SIZE; i++)
			free(webg->all_url_list[i]);
		free(webg->all_url_list);
		return INIT_FAIL;
	}
	for (i = 0; i < GRAPH_SIZE; i++)
		webg->edge_set[i] = NULL;
	webg->all_url_list_size = 0;
	webg->edge_set_size = 0;
	webg->existed_page_size = 0;
	return INIT_SUCCEED;
}

//ŒÓÈëÒ»žö¶¥µã
int insert_vertex(struct web_graph *webg, char * url)
{
	unsigned int key = JSHash(url, HASH_TABLE_SIZE);
	int num;
	struct node *new_ptr = NULL;

	num = ++(webg->all_url_list_size);
	strcpy(webg->all_url_list[num], url);
	new_ptr = (struct node *)malloc(sizeof (struct node));
	if (new_ptr == NULL)
	{
		printf("·ÖÅäÄÚŽæÊ§°Ü£¡\n");
		return -1;
	}
	new_ptr->number = num;
	new_ptr->url = (char *)malloc(sizeof (char ) * LINK_LEN);
	strcpy(new_ptr->url, url);
	new_ptr->next_ptr = webg->vertex_set[key].next_ptr;
	webg->vertex_set[key].next_ptr = new_ptr;//ÕâÀïÓÃµÄÊÇÍ·²å·š

	return num;//·µ»ØÕâžö¶¥µãµÄ±àºÅ
}

// ÅÐ¶Ïžø¶š¶¥µãÊÇ·ñÔÚgraphÖÐ
int has_vertex(struct web_graph *webg, char *url)
{
	unsigned int key = JSHash(url, HASH_TABLE_SIZE);
	struct node *cur_ptr = webg->vertex_set[key].next_ptr;

	while (cur_ptr != NULL)
	{
		if (strcmp(cur_ptr->url, url) == 0)
			return cur_ptr->number;
		cur_ptr = cur_ptr->next_ptr;
	}
	return NOT_IN_SET;
}

//²åÈëÒ»Ìõ±ßµœgraphÖÐ
void insert_edge(struct web_graph *webg, int src_num, int dst_num, int pos)
{
	webg->edge_set[src_num][pos] = dst_num;
}

// graphµÄ¶¥µãÊý
int vertex_size(struct web_graph *webg)
{
	return webg->all_url_list_size;
}
//calc_ind_cdf();// ŒÆËãËùÓÐ¶¥µãµÄÈë¶ÈµÄÀÛ»ý·Ö²Œ
//calc_pagerank();// ŒÆËãËùÓÐ¶¥µãµÄpagerank
//find_ind(char *url);// ²éÕÒžø¶š¶¥µãµÄÈë¶È
//gen_graphviz();// Éú³ÉÃèÊöÍŒÍØÆËµÄgraphvizœÅ±Ÿ

// Ïú»Ùweb graph
void destroy_webg(struct web_graph *webg)
{
	int i = 0;

	free(webg->ind); 
	free(webg->existed_page); 
	for (i = 0; i < HASH_TABLE_SIZE; i++)
	{
		if (webg->vertex_set[i].next_ptr != NULL)
		{
			destroy_list(webg->vertex_set[i].next_ptr);//ÕâÀïÊôÓÚ¶ÔÁŽ±íµÄ»ØÊÕ
		}
	}
	free(webg->vertex_set);

	for (i = 0; i < GRAPH_SIZE; i++)
		free(webg->all_url_list[i]);
	free(webg->all_url_list);

	for (i = 1; i <= webg->all_url_list_size; i++)
		free(webg->edge_set[i]);
	free(webg->edge_set);
}

//¶ÔÁŽ±íÄÚŽæµÄÊÍ·Å
void destroy_list(struct node *head_ptr)
{
	struct node *n_ptr = head_ptr->next_ptr;
	while (n_ptr != NULL)
	{
		head_ptr->next_ptr = n_ptr->next_ptr;
		free(n_ptr);
		n_ptr = head_ptr->next_ptr;
	}
	free(head_ptr);
}

void print_webg_to_file(struct web_graph *webg) 
{
	FILE *fp;
	int i, j;

	fp = fopen("webg_all_url_list.txt", "w");
	if (fp == NULL)
	{
		printf("fail to open webg_all_url_list.txt\n");
		return;
	}
	fprintf(fp, "size: %d\n", webg->all_url_list_size);
	for (i = 1; i <= webg->all_url_list_size; i++)
		fprintf(fp, "%s\n", webg->all_url_list[i]);
	fclose(fp);

	fp = fopen("webg_ind.txt", "w");
	if (fp == NULL)
	{
		printf("fail to open webg_ind.txt\n");
		return;
	}
	fprintf(fp, "size: %d\n", webg->all_url_list_size);
	for (i = 1; i <= webg->all_url_list_size; i++)
		fprintf(fp, "%d\n", webg->ind[i]);
	fclose(fp);

	fp = fopen("webg_existed_page.txt", "w");
	if (fp == NULL)
	{
		printf("fail to open webg_existed_page.txt\n");
		return;
	}
	fprintf(fp, "size: %d\n", webg->all_url_list_size);
	fprintf(fp, "existed_page_size: %d\n", webg->existed_page_size);
	for (i = 1; i <= webg->all_url_list_size; i++)
		fprintf(fp, "%d\n", webg->existed_page[i]);
	fclose(fp);

	fp = fopen("webg_edge_set.txt", "w");
	if (fp == NULL)
	{
		printf("fail to open webg_edge_set.txt\n");
		return;
	}
	fprintf(fp, "size: %d\n", webg->all_url_list_size);
	fprintf(fp, "edge_set_size: %d\n", webg->edge_set_size);
	for (i = 1; i <= webg->all_url_list_size; i++)
	{
		if (webg->edge_set[i] == NULL)
			fprintf(fp, "0\n");
		else
		{
			fprintf(fp, "%d\n", webg->edge_set[i][0]);
			for (j = 1; j <= webg->edge_set[i][0]; j++)
				fprintf(fp, " %d", webg->edge_set[i][j]);
			fprintf(fp, "\n");
		}
	}
	fclose(fp);
}

void output_result_file(void)
{
	FILE *fp = NULL;
	int all_url_list_size;
	int edge_set_size;
	int existed_page_size;
	char tmp[256];//to scanf some no_sense string
	char **all_url_list = NULL;
	int i, j, tmp_num;
	int **edge_set = NULL;
	int *existed_page = NULL;
	int *ind = NULL;
	float *coeff = NULL;
	float alpha = 0.15;
	int real_outd;
	float *v_old = NULL, *v_new = NULL;//v_old相当于vk，v_new相当于vk+1
	int end_cal;
	int cal_times = 0;
	float accuracy = 0.0001;//精度
	int *ind_cdf = NULL;
	int max_ind;
	int left;
	int top10ind[15];
	int pos;
	float first_max, second_max;
	float x;
	float sum;

	//从webg_all_url_list.txt文件将所有的url读进二维数组：all_url_list，大小读到all_url_list_size
	fp = fopen("webg_all_url_list.txt", "r");
	if (fp == NULL)
	{
		printf("fail to open webg_all_url_list.txt! exit!\n");
		exit(1);
	}
	fscanf(fp, "%s%d", tmp, &all_url_list_size);
	all_url_list = (char **)malloc(sizeof (char *) * (all_url_list_size + 1));
	if (all_url_list == NULL)
	{
		printf("fail to malloc space for all_url_list! exit!\n");
		fclose(fp);
		exit(1);
	}
	for (i = 1; i <= all_url_list_size; i++)
	{
		all_url_list[i] = (char *)malloc(sizeof (char ) * 512);
		fscanf(fp, "%s", all_url_list[i]);
	}
	printf("succeed in reading in all_url_list!\n");
	fclose(fp);

	fp = fopen("webg_edge_set.txt", "r");
	if (fp == NULL)
	{
		printf("fail to open webg_edge_set.txt! exit!\n");
		exit(1);
	}
	fscanf(fp, "%s%d", tmp, &all_url_list_size);
	fscanf(fp, "%s%d", tmp, &edge_set_size);
	edge_set = (int **)malloc(sizeof (int *) * (all_url_list_size + 1));
	if (edge_set == NULL)
	{
		printf("fail to malloc space for edge_set! exit!\n");
		exit(1);
	}
	for (i = 1; i <= all_url_list_size; i++)
	{
		fscanf(fp, "%d", &tmp_num);
		edge_set[i] = (int *)malloc(sizeof (int ) * (tmp_num + 1));
		edge_set[i][0] = tmp_num;
		for (j = 1; j <= tmp_num; j++)
			fscanf(fp, "%d", &edge_set[i][j]);
	}
	printf("succeed in reading in edge_set!\n");
	fclose(fp);

	fp = fopen("webg_existed_page.txt", "r");
	if (fp == NULL)
	{
		printf("fail to open webg_existed_page.txt! exit!\n");
		exit(1);
	}
	fscanf(fp, "%s%d", tmp, &all_url_list_size);
	fscanf(fp, "%s%d", tmp, &existed_page_size);
	existed_page = (int *)malloc(sizeof (int ) * (all_url_list_size + 1));
	for (i = 1; i <= all_url_list_size; i++)
		fscanf(fp, "%d", &existed_page[i]);
	printf("succeed in reading in webg_existed_page.txt!\n");
	fclose(fp);

	fp = fopen("webg_ind.txt", "r");
	if (fp == NULL)
	{
		printf("fail to open webg_ind.txt! exit!\n");
		exit(1);
	}
	fscanf(fp, "%s%d", tmp, &all_url_list_size);
	ind = (int *)malloc(sizeof (int ) * (all_url_list_size + 1));
	for (i = 1; i <= all_url_list_size; i++)
		fscanf(fp, "%d", &ind[i]);
	printf("succeed in reading in webg_ind.txt!\n");
	fclose(fp);

	printf("read in all data!\n\n");
	printf("start to cal page_rank!\n");

	//开始计算page_rank
	//先得出系数矩阵（一个一维列向量），是(1-alpha)*(1/实际出度)得到的一个系数，因为原来的边集是含有不存在的点的，实际出度是剔除这些点后的出度
	coeff = (float *)malloc(sizeof (float ) * (all_url_list_size + 1));
	for (i = 1; i <= all_url_list_size; i++)
	{
		real_outd = 0;
		for (j = 1; j <= edge_set[i][0]; j++)
			if (existed_page[edge_set[i][j]] == 1)
				real_outd++;
		if (real_outd > 0)
			coeff[i] = (1.0 - alpha) * (1.0 / (float)real_outd);
		else
			coeff[i] = 0;
	}
	
	//初始化两个列向量，v_old相当于v0
	v_old = (float  *)malloc(sizeof (float ) * (all_url_list_size + 1));
	v_new = (float  *)malloc(sizeof (float  ) * (all_url_list_size + 1));
	for (i = 1; i <= all_url_list_size; i++)
	{
		if (existed_page[i] == 1)
			v_old[i] = 1.0;
		else 
			v_old[i] = 0.0;
		v_new[i] = 0.0;
	}

	end_cal = 0;
	while (end_cal == 0)
	{
		cal_times++;
		end_cal = 1;
		for (i = 1; i <= all_url_list_size; i++)
		{
			for (j = 1; j <= edge_set[i][0]; j++)
				if (existed_page[edge_set[i][j]] == 1)
					v_new[edge_set[i][j]] += coeff[i] * v_old[i];	
		}
		x = 0;
		for (i = 1; i <= all_url_list_size; i++)
		{
			x += v_old[i] * alpha / existed_page_size;
		}
		for (i = 1; i <= all_url_list_size; i++)
		{
			if (existed_page[i] == 1)
				v_new[i] += x;
			if (fabs(v_new[i] - v_old[i]) > accuracy)
			{
				end_cal = 0;
				break;
			}
		}
		sum = 0;
		for (i = 1; i <= all_url_list_size; i++)
		{
			v_old[i] = v_new[i];
			v_new[i] = 0;
			if (existed_page[i] == 1)
				sum += v_old[i];
		}
		printf("finish %dth cal\n", cal_times);
	}
	//归一化
	for (i = 1; i <= all_url_list_size; i++)
		v_old[i] = v_old[i] / sum;

	fp = fopen("checkedresult.dat", "w");
	for (i = 1; i <= all_url_list_size; i++)
		if (existed_page[i] == 1)
			fprintf(fp, "%s  |   %d   |   %f\n", all_url_list[i], ind[i], v_old[i]);
	fclose(fp);
	printf("end_cal!\n");

	//计算入度的cdf
	printf("cal indcdf.dat\n");
	max_ind = -1;
	for (i = 1; i <= all_url_list_size; i++)
		if (existed_page[i] == 1 && ind[i] > max_ind)
				max_ind = ind[i];
	ind_cdf = (int *)malloc(sizeof (int ) * (max_ind + 1));
	for (i = 0; i <= max_ind; i++)
		ind_cdf[i] = 0;
	for (i = 1; i <= all_url_list_size; i++)
		if (existed_page[i] == 1)
			ind_cdf[ind[i]]++;
	fp = fopen("indcdf.dat", "w");
	tmp_num = 0;
	for (i = 1; i <= max_ind; i++)
	{
		tmp_num += ind_cdf[i];
		fprintf(fp, "%-20d%f\n", i, (float)tmp_num / (float)existed_page_size);
	}
	fclose(fp);
	printf("end cal indcdf.dat\n");

	//计算top10
	fp = fopen("top10.dat", "w");
	//计算top10ind
	printf("cal top 10 ind\n");
	left = 10;
	tmp_num = 0;
	fprintf(fp, "-----------------top 10 ind-------------------\n");
	for (i = max_ind; i >= 1 && left > 0; i--)
	{
		if (ind_cdf[i] != 0)
		{
			left -= ind_cdf[i];
			tmp_num++;
			top10ind[tmp_num] = i;
		}
	}
	for (i = 1; i <= tmp_num; i++)
	{
		for (j = 1; j <= all_url_list_size; j++)
		{
			if (ind[j] == top10ind[i])
				fprintf(fp, "%s    |   %d\n", all_url_list[j], top10ind[i]);
		}
	}
	printf("end cal top 10 ind\n");
	fprintf(fp, "\n");

	//计算top10 page_rank
	printf("cal top 10 page_rank!\n");
	fprintf(fp, "-----------------top 10 page_rank-------------------\n");
	first_max = 1.0;
	second_max = -1.0;
	tmp_num = 10;
	while (tmp_num > 0)
	{
		tmp_num--;
		pos = -1;
		for (i = 1; i <= all_url_list_size; i++)
		{
			if (existed_page[i] == 1 && v_old[i] > second_max && v_old[i] < first_max)
			{
				pos = i;
				second_max = v_old[i];
			}
		}
		if (pos > 0)
			fprintf(fp, "%s    |   %f\n", all_url_list[pos], v_old[pos]);
		first_max = second_max;
		second_max = -1.0;
	}
	fclose(fp);

	//生成graphviz脚本
	printf("create sitemap.dot\n");
	fp = fopen("sitemap.dot", "w");
	fprintf(fp, "digraph G {\n");
	for (i = 1; i <= all_url_list_size; i++)
	{
		if (existed_page[i] == 1)
		{
			fprintf(fp, "%s", all_url_list[i]);
			for (j = 1; j <= edge_set[i][0]; j++)
			{
				if (existed_page[edge_set[i][j]] == 1)
					fprintf(fp, " -> %s", all_url_list[edge_set[i][j]]);
			}
			fprintf(fp, ";\n");
		}
	}
	fprintf(fp, "}");
	fclose(fp);
	printf("finish creating sitemap.dot\n");
}
