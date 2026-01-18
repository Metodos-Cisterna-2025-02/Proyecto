#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "io.h"
#include "hash.h"

#define MAX_DIFF 2
#define MAX_DEPTH 1024

struct pos {
	int x;
	int y;
	int h;
	struct pos* next;
};

struct Queue {
	struct pos* first;
	struct pos* last;
};

static struct Queue* frontier;

int** searchMap;
static int visited[MAX_VERTICAL][MAX_HORIZONTAL];
static int parentX[MAX_VERTICAL][MAX_HORIZONTAL];
static int parentY[MAX_VERTICAL][MAX_HORIZONTAL];

/* Prototipos de funciones */
void search(int xStart, int yStart, int xEnd, int yEnd);
int distanceTo(int xStart, int yStart, int xEnd, int yEnd);
struct pos* getNext(int xStart, int yStart, int xEnd, int yEnd);
int posGetX(struct pos target);
int posGetY(struct pos target);
int posGetH(struct pos target);
static void frontierAddNeighbours(int posX, int posY);
static struct pos* getNeighbour(char* key);
static void posSet(struct pos* target, int xPos, int yPos, int height);
static void frontierInit();
static int frontierIsEmpty();
static void frontierPut(struct pos* node);
static struct pos* frontierPop();
static void setCameFrom(struct pos* node, struct pos* previous);
static struct pos* getCameFrom(struct pos* node);
static int inCameFrom(struct pos* node);


void search(int xStart, int yStart, int xEnd, int yEnd) {
	if (!searchMap) {
		searchMap = getMap();
	}

	memset(visited, 0, sizeof(visited));
	memset(parentX, -1, sizeof(parentX));
	memset(parentY, -1, sizeof(parentY));

	struct pos queue[MAX_VERTICAL * MAX_HORIZONTAL];
	int head = 0, tail = 0;

	struct pos start;
	int hStart = searchMap[yStart][xStart];
	posSet(&start, xStart, yStart, hStart);

	struct pos end;
	int hEnd = searchMap[yEnd][xEnd];
	posSet(&end, xEnd, yEnd, hEnd);

	queue[tail++] = start;
	visited[start.y][start.x] = 1;

	int dirs[4][2] = {{0,-1},{1,0},{0,1},{-1,0}};

	while (head < tail) {
		struct pos node = queue[head++];

		if (node.x == end.x && node.y == end.y)
			break;

		for (int i = 0; i < 4; i++) {
			int nx = node.x + dirs[i][0];
			int ny = node.y + dirs[i][1];
			if (nx < 0 || nx >= MAP_TEST_SIZE || ny < 0 || ny >= MAP_TEST_SIZE)
				continue;

			if (visited[ny][nx])
				continue;

			int nh = searchMap[ny][nx];
			if (abs(node.h - nh) > MAX_DIFF)
				continue;

			struct pos next;
			posSet(&next, nx, ny, nh);
			queue[tail++] = next;
			visited[ny][nx] = 1;
			parentX[ny][nx] = node.x;
			parentY[ny][nx] = node.y;
		}
	}

	return;
}

int distanceTo(int xStart, int yStart, int xEnd, int yEnd) {
	int d = 0;
	
	if (!visited[yEnd][xEnd])
		return -1; /* no alcanzado */

	int cx = xEnd;
	int cy = yEnd;
	while (!(cx == xStart && cy == yStart)) {
		int px = parentX[cy][cx];
		int py = parentY[cy][cx];
		if (px == -1 || py == -1)
			return -1;
		cx = px;
		cy = py;
		d++;
	}

	return d;
}

struct pos* getNext(int xStart, int yStart, int xEnd, int yEnd) {
	struct pos start;
	posSet(&start, xStart, yStart, searchMap[yStart][xStart]);

	struct pos node;
	posSet(&node, xEnd, yEnd, searchMap[yEnd][xEnd]);

	struct pos* current = getCameFrom(&node);
	if (current == NULL)
		return NULL;
	
	struct pos* next = getCameFrom(current);
	while (next != NULL && (next->x != start.x || next->y != start.y)) {
		current = next;
		next = getCameFrom(current);
	}

	return current;
}

int posGetX(struct pos target) {
	return target.x;
}

int posGetY(struct pos target) {
	return target.y;
}

int posGetH(struct pos target) {
	return target.h;
}

static void frontierAddNeighbours(int posX, int posY) {
	/* agregar nodo de arriba a hashtable */
	int x = posX;
	int y = posY-1;
	int h;
	if (y >= 0) {
		h = searchMap[y][x];
		struct pos* nodeUp = (struct pos*) malloc(sizeof(struct pos));
		posSet(nodeUp, x, y, h);
		install("up", (struct node*)nodeUp);
	} else {
		install("up", NULL);
	}

	/* agregar nodo de la derecha a hashtable */
	x = posX+1;
	y = posY;
	if (x < MAP_TEST_SIZE) {
		h = searchMap[y][x];
		struct pos* nodeRight = (struct pos*) malloc(sizeof(struct pos));
		posSet(nodeRight, x, y, h);
		install("right", (struct node*)nodeRight);
	} else {
		install("right", NULL);
	}

	/* agregar nodo de abajo a hashtable */
	x = posX;
	y = posY+1;
	if (y < MAP_TEST_SIZE) {
		h = searchMap[y][x];
		struct pos* nodeDown = (struct pos*) malloc(sizeof(struct pos));
		posSet(nodeDown, x, y, h);
		install("down", (struct node*)nodeDown);
	} else {
		install("down", NULL);
	}

	/* agregar nodo de la izquierda a hashtable */
	x = posX-1;
	y = posY;
	if (x >= 0) {
		h = searchMap[y][x];
		struct pos* nodeLeft = (struct pos*) malloc(sizeof(struct pos));
		posSet(nodeLeft, x, y, h);
		install("left", (struct node*)nodeLeft);
	} else {
		install("left", NULL);
	}

	return;
}

static struct pos* getNeighbour(char* key) {
	struct nlist* entry = lookup(key);
	if (entry == NULL)
		return NULL;

	return (struct pos*) entry->value;
}

static void posSet(struct pos* target, int xPos, int yPos, int height) {
	target->x = xPos;
	target->y = yPos;
	target->h = height;
	target->next = NULL;
	
	return;
}

static void frontierInit() {
	frontier = (struct Queue*) malloc(sizeof(struct Queue) * MAX_DEPTH); 
	frontier->first = frontier->last = NULL;

	return;
}

static int frontierIsEmpty() {
	return (frontier->first == NULL);
}

static void frontierPut(struct pos* node) {
	if (frontierIsEmpty())
		frontier->first = frontier->last = node;
	else {
		frontier->last->next = node;
		frontier->last = node;
	}

	return;
}

static struct pos* frontierPop() {
	if (frontierIsEmpty())
		return NULL;
	
	struct pos* node = frontier->first;
	frontier->first = frontier->first->next;

	return node;
}

static void setCameFrom(struct pos* node, struct pos* previous) {
	char key[20];
	snprintf(key, 20, "%d %d", node->x, node->y);
	install(key, (struct node*)previous);

	return;
}

static struct pos* getCameFrom(struct pos* node) {
	char key[20];
	snprintf(key, 20, "%d %d", node->x, node->y);
	struct nlist* entry = lookup(key);
	if (entry != NULL)
		return (struct pos*)entry->value;
	
	return NULL;
}

static int inCameFrom(struct pos* node) {
	if (node == NULL)
		return 1; /* already handled via visited */

	return visited[node->y][node->x];
}