//
// Created by dwbrite on 5/8/20.
//

#ifndef LIGHTBOARD_GRAPHS_H
#define LIGHTBOARD_GRAPHS_H

#include <vector>
#include <Adafruit_SSD1351.h>
#include <functions.h>

class Line {
public:
    int x0;
    int y0;

    int x1;
    int y1;

    Line() = default;
    Line(int x0, int y0, int x1, int y1) {
        this->x0 = x0;
        this->y0 = y0;
        this->x1 = x1;
        this->y1 = y1;
    }
};

class Graph {
public:
    int x0;
    int y0;
    uint16_t bg;
    std::vector<GraphingFunction *> functions;
    std::vector<std::vector<Line>> cache;

    Graph(int x0, int y0, int width, int height, uint16_t bg);
    void addFunction(GraphingFunction *func);
    void draw(Adafruit_SSD1351 tft);
    void update(double dt);
    int getWidth();
    int getHeight();

private:
    int width;
    int height;
};
#endif //LIGHTBOARD_GRAPHS_H
