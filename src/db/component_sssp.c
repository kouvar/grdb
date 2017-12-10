#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>	
#include <sys/stat.h>

#include <unistd.h>
#include "config.h"
#include "graph.h"
#include "tuple.h"
#include "enum.h"
#include "cli.h"
#include <fcntl.h>
#include <limits.h>
#include <errno.h>

#define INF 99999
/* Place the code for your Dijkstra implementation in this file */

// Structure for edges containing the source, destination, weight of the edge, pointers to the next node and the previous one. 
struct edges{
	vertexid_t start;
	vertexid_t end;
	int weight;
	struct edges* prev;
	struct edges* next;


};
// Structure for vertices containing the current vertex, previous_vertex, distance and pointers to teh previous and current vertices. 
struct vertex_e{
	vertexid_t current;
	vertexid_t previous_vertex;
	int distance;
	struct vertex_e* previous;
	struct vertex_e* next;

};



// Creating a structure with all the vertices from the edges 
struct  vertex_e* create_vertex(vertexid_t data, struct vertex_e* head_v){
	struct vertex_e* temp = (struct vertex_e*)malloc(sizeof(struct vertex_e));
	temp-> current = data;
	temp->distance = INF;
	temp->next = NULL;
	temp->previous = NULL;
	if(head_v == NULL){
		return temp;
	}
	struct vertex_e * temp2 = head_v;
	while(temp2 != NULL){
		if(temp2->current == data){
			return head_v;

	}
	temp2 = temp2->next;

	
}
	temp->next = head_v;
	head_v-> previous = temp;
	return temp;



	
}

// Creating an edge 
struct edges* create_edge(struct edges* head, vertexid_t start, vertexid_t end, int weight){
	struct edges* temp = (struct edges*)malloc(sizeof(struct edges));
	temp->start = start;
	temp->weight = weight;
	temp->end = end;
	temp->next = head;
	temp->prev = NULL;
	if(head != NULL){
		head->prev = temp;

	}
	return temp;


	
}
// Adding an edge into the structure as we traverse through
struct edges* add_edge(struct edges* head, vertexid_t start, vertexid_t end, int weight){
	struct edges* new_edge = create_edge(head, start, end, weight);
	head = new_edge;
	return head;

}
// Creating a new structure to order all the vertices to acess indeexes
struct vertex_e* order_vertex(struct edges* head){
	struct edges* temp = head;
	struct vertex_e *head_v = NULL;
	while(temp != NULL){
		head_v = create_vertex(temp->start, head_v);
		head_v = create_vertex(temp->end, head_v);
		temp = temp->next;

	}
	return head_v;
}
// Popping vertices from teh structure to order them 
struct vertex_e* pop(struct vertex_e* temp_v, struct vertex_e* temp){
	if(temp_v == NULL){
		temp->previous = NULL;
		temp->next = NULL;
		return temp;


	}
	if(temp_v != NULL){
		temp->next = temp_v;
		temp_v->previous = temp;
		temp->previous = NULL;

	}
	

	return temp;

}
// Order the vertices in ascending order of their weights.
struct vertex_e* update(struct vertex_e* head_v, int value, vertexid_t v1, vertexid_t prev_v){
	struct vertex_e* temp_head = NULL;
	struct vertex_e* tmp = NULL;
	//printf("value %llu\n", v1);

	temp_head = head_v;
	while(temp_head != NULL){
		if(temp_head->current == v1){
			if(temp_head->distance > value){
				temp_head->distance = value;
				temp_head->previous_vertex = prev_v;
				break;
			}

		}
		temp_head = temp_head->next;

	}
	

	if(temp_head == NULL){
		return head_v;

	}

	if(temp_head->previous == NULL){
		return temp_head;	
	}

	tmp = head_v;
	while(tmp != NULL){
		if(temp_head->current == tmp->current){
			return head_v;

		}

		if(tmp->distance > temp_head->distance){
			if(tmp-> previous == NULL){
				temp_head->previous->next = temp_head->next;
				if(temp_head->next != NULL){
					temp_head->next->previous = temp_head->previous;
				}
				temp_head->next = tmp;
				tmp->previous = temp_head;
				temp_head->previous = NULL;
				return temp_head;

			}	
			else{
				//printf("Reached");
				temp_head->previous->next = temp_head->next;
				if(temp_head->next != NULL){
					//printf("did not come here");
					temp_head->next->previous = temp_head->previous;
				}
				temp_head->next = tmp;
				temp_head->previous = tmp->previous;
				tmp->previous = temp_head;
				temp_head->previous->next = temp_head;
				return head_v;

			}

	}
		tmp = tmp->next;


	}


	return head_v;
	


}

// Executing Dijkstra's algorithm
void dijkstra(struct edges* head_e, struct vertex_e* head_v, vertexid_t v1, vertexid_t v2){
		int total_weight = 0;
		head_v = update(head_v, 0, v1, (vertexid_t) 0);
		struct vertex_e *temp = NULL,*tp = NULL;
		struct vertex_e* temp_v = NULL;
		struct vertex_e* temp_v1 = NULL;
		struct vertex_e* temp_v2 = NULL;
		struct vertex_e* temp_v3 = NULL;

		struct edges* temp_e = NULL;
		while(head_v->current != v2){
			temp = head_v;
			head_v = head_v->next;
			head_v->previous = NULL;
			temp_v = pop(temp_v, temp);
			temp_e = head_e;
			while(temp_e != NULL){
				if(temp->current == temp_e->start){
					head_v = update(head_v, (temp->distance+temp_e->weight), temp_e->end, temp_e->start);
					/*tp = head_v;
					while(tp != NULL){
						printf("%llu \n", tp->current);
						tp = tp->next;

						}
						printf("done");*/

				}
				temp_e = temp_e->next;

			}


		}
		temp_v = pop(temp_v, head_v);
		temp_v1 = temp_v;

		/*while(temp_v1 != NULL){
		printf("%llu \n", temp_v1->current);
		temp_v1 = temp_v1->next;

	}*/
		// Printing the graph
		temp_v2 = head_v;
		temp_v3 = head_v;
		printf("Graph printed the other way\n");
		printf("-------\n");
		printf("Weight = %d\n", temp_v2->distance);

		while(temp_v2->previous_vertex != 0){
			

			printf("%llu<---",temp_v2->current);
			while(temp_v3 != NULL){
				if(temp_v3->current == temp_v2->previous_vertex){
					temp_v2 = temp_v3;
					break;
				}
				temp_v3 = temp_v3->next;
			}

		}
		printf("%llu\n",temp_v2->current);
		printf("-------\n");
		




	

	
	


}

int
component_sssp(
        component_t c,
        vertexid_t v1,
        vertexid_t v2,
        int *n,
        int *total_weight,
        vertexid_t **path)
{
	attribute_t attr;
	int i, offset;
	off_t off;
	ssize_t len, size;
	vertexid_t id, id1, id2;
	struct tuple tuple;
	char *buf;
	int readlen;
	struct edges *head_e = NULL,*temp_e = NULL;
	struct vertex_e *head_v = NULL, *temp_v=NULL;
	{
		/* data */
	};
	char s[BUFSIZE];
	//printf("in here");
	
	/*
	 * Figure out which attribute in the component edges schema you will
	 * use for your weight function
	 */
	 	memset(s, 0, BUFSIZE);
	sprintf(s, "%s/%d/%d/e",
		grdbdir, gno, cno  );

	c->efd = open(s, O_RDONLY);


	 if (c->se == NULL)
		size = 0;
	else
		size = schema_size(c->se);

	readlen = (sizeof(vertexid_t) << 1) + size;
	//free(buf);
	buf = malloc(readlen);

	for (off = 0;; off += readlen) {
		lseek(c->efd, off, SEEK_SET);
		len = read(c->efd, buf, readlen);
		if (len <= 0)
			break;

		

		id1 = *((vertexid_t *) buf);
		id2 = *((vertexid_t *) (buf + sizeof(vertexid_t)));
		

		
		memset(&tuple, 0, sizeof(struct tuple));
		tuple.s = c->se;
		tuple.len = size;
		tuple.buf = buf + (sizeof(vertexid_t) << 1);
		for (attr = tuple.s->attrlist; attr != NULL; attr = attr->next) {
			if(attr->bt == INTEGER){
				offset = tuple_get_offset(&tuple, attr->name);
				i = tuple_get_int(tuple.buf + offset);
				//printf("%llu %llu %d\n", id1, id2, i);
				head_e= add_edge(head_e, id1, id2, i);
			}

		
	}
}
	temp_e = head_e;
	/*while(temp_e != NULL){
		printf("%llu %llu %d\n", temp_e->start,temp_e->end,temp_e->weight);
		temp_e = temp_e->next;

	}
*/
	head_v = order_vertex(head_e);
	temp_v = head_v;
	/*while(temp_v != NULL){
		printf("%llu \n", temp_v->current);
		temp_v = temp_v->next;

	}*/
	free(buf);
	/*
	 * Execute Dijkstra on the attribute you found for the specified
	 * component
	 */
	dijkstra(head_e, head_v, v1, v2);


	



	/* Change this as needed */
	return (-1);
}
