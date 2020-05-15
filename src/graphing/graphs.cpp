//
// Created by dwbrite on 5/8/20.
//

#include "graphs.h"

Graph::Graph(int x0, int y0, int width, int height, uint16_t bg) {
    this->x0 = x0;
    this->y0 = y0;
    this->width = width;
    this->height = height;
    this->bg = bg;
}

void Graph::addFunction(GraphingFunction *func) {
    this->functions.push_back(func);
    this->cache.emplace_back(width);
}

void Graph::draw(Adafruit_SSD1351 tft) {
    auto pixel = true;
    int prevX = 0;
    for(int x = 0; x < width; x++) {
        // blank
        for(int i = 0; i < functions.size(); i++) {
            Line l = cache[i][x];
            if (pixel) {
                tft.drawPixel(l.x1+x0, l.y1+y0, 0);
            } else if(x > 0) {
                tft.drawLine(l.x0+x0, l.y0+y0, l.x1+x0, l.y1+y0, bg);
            }
        }

        // cache and draw
        for(int i = 0; i < functions.size(); i++) {
            int prevY = cache[i][prevX].y1;
            int y = -functions[i]->f(double(x) / width) * (double(height) / 2) + height / 2;

            Line l = Line(prevX, prevY, x, y);
            cache[i][x] = l;

            if(pixel) {
                tft.drawPixel(l.x1+x0, l.y1+y0, functions[i]->color);
            } else if(x>0) {
                tft.drawLine(l.x0 + x0, l.y0 + y0, l.x1 + x0, l.y1+y0, functions[i]->color);
            }
        }
        prevX = x;
    }
}

void Graph::update(double dt) {
    for(auto f : functions) {
        f->update(dt);
    }
}

int Graph::getWidth() { return width; }
int Graph::getHeight() { return height; }