#include <stdio.h>

#define WIDTH 20
#define HEIGHT 10

typedef struct {
    int x, y;
} Point;

void drawTriangle(char canvas[HEIGHT][WIDTH], Point points[3]);
void drawLine(char canvas[HEIGHT][WIDTH], Point p1, Point p2);
int max(int a, int b);
int abs(int x);
void printCanvas(char canvas[HEIGHT][WIDTH]);

int main() {
    char canvas[HEIGHT][WIDTH] = {0};
    Point points[3] = {
        {1, 1},
        {WIDTH / 2, HEIGHT - 2},
        {WIDTH - 2, 1}
    };

    drawTriangle(canvas, points);
    printCanvas(canvas);

    return 0;
}

void drawTriangle(char canvas[HEIGHT][WIDTH], Point points[3]) {
    for (int i = 0; i < 3; i++) {
        Point p1 = points[i];
        Point p2 = points[(i + 1) % 3];
        drawLine(canvas, p1, p2);
    }
}

void drawLine(char canvas[HEIGHT][WIDTH], Point p1, Point p2) {
    int dx = p2.x - p1.x;
    int dy = p2.y - p1.y;
    int steps = max(abs(dx), abs(dy));
    if (steps == 0) {
        return;
    }
    float xIncrement = (float)dx / steps;
    float yIncrement = (float)dy / steps;
    float x = p1.x;
    float y = p1.y;
    for (int i = 0; i < steps; i++) {
        canvas[(int)y][(int)x] = '*';
        x += xIncrement;
        y += yIncrement;
    }
}

int max(int a, int b) {
    return a > b ? a : b;
}

int abs(int x) {
    return x < 0 ? -x : x;
}

void printCanvas(char canvas[HEIGHT][WIDTH]) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            printf("%c", canvas[i][j] ? canvas[i][j] : ' ');
        }
        printf("\n");
    }
}

