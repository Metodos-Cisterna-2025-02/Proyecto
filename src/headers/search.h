#ifndef SEARCH_H
#define SEARCH_H

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

/* Cambiamos searchMap a int** para que coincida con io.h */
static int** searchMap = NULL; 
static int visited[MAX_VERTICAL][MAX_HORIZONTAL];
static int parentX[MAX_VERTICAL][MAX_HORIZONTAL];
static int parentY[MAX_VERTICAL][MAX_HORIZONTAL];

/* Prototipos de funciones */
void search(int xStart, int yStart, int xEnd, int yEnd, int **mapaActual);
int distanceTo(int xStart, int yStart, int xEnd, int yEnd);
struct pos* getNext(int xStart, int yStart, int xEnd, int yEnd);
int posGetX(struct pos target);
int posGetY(struct pos target);
int posGetH(struct pos target);
static void posSet(struct pos* target, int xPos, int yPos, int height);

/* --- FUNCIONES DE BUSQUEDA --- */

void search(int xStart, int yStart, int xEnd, int yEnd, int **mapaActual) {
    searchMap = getMap(); 

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

	queue[tail++] = start;
	visited[yStart][xStart] = 1;

	int dirs[4][2] = {{0,-1},{1,0},{0,1},{-1,0}};

	while (head < tail) {
		struct pos node = queue[head++];

		if (node.x == xEnd && node.y == yEnd)
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
}

struct pos* getNext(int xStart, int yStart, int xEnd, int yEnd) {
	if (!visited[yEnd][xEnd]) return NULL;

	int cx = xEnd;
	int cy = yEnd;
	int stepX = xEnd;
	int stepY = yEnd;

	// Reconstruimos el camino hacia atras
	while (!(parentX[cy][cx] == xStart && parentY[cy][cx] == yStart)) {
		stepX = cx;
		stepY = cy;
		int tx = parentX[cy][cx];
		int ty = parentY[cy][cx];
		if (tx == -1) return NULL;
		cx = tx;
		cy = ty;
	}

	struct pos* res = (struct pos*)malloc(sizeof(struct pos));
	posSet(res, cx, cy, searchMap[cy][cx]);
	return res;
}

int distanceTo(int xStart, int yStart, int xEnd, int yEnd) {
	int d = 0;
	if (!visited[yEnd][xEnd]) return -1;

	int cx = xEnd;
	int cy = yEnd;
	while (!(cx == xStart && cy == yStart)) {
		int px = parentX[cy][cx];
		int py = parentY[cy][cx];
		if (px == -1) return -1;
		cx = px; cy = py;
		d++;
	}
	return d;
}

static void posSet(struct pos* target, int xPos, int yPos, int height) {
	target->x = xPos;
	target->y = yPos;
	target->h = height;
	target->next = NULL;
}

int posGetX(struct pos target) { return target.x; }
int posGetY(struct pos target) { return target.y; }
int posGetH(struct pos target) { return target.h; }

#endif