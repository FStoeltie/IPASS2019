#include "scene.hpp"

void GraphicsObjectSquare::setRotationInDegrees(const unsigned int degrees) {
    if (this->degrees == degrees)
        return;
    else    {
        this->degrees = degrees;
        hwlib::xy pivot =  hwlib::xy(pArray.at(0).x + (pArray.at(1).x - pArray.at(0).x) / 2, pArray.at(0).y + (pArray.at(2).y - pArray.at(0).y) / 2);
        rotatePoint(pArray.at(0), pivot);
        rotatePoint(pArray.at(1), pivot);
        rotatePoint(pArray.at(2), pivot);
        rotatePoint(pArray.at(3), pivot);
    }

}

void GraphicsObjectSquare::setScale(const float scale)   {
    if (this->scale == scale)
        return;
    else    {
        /*pArray.at(0) = hwlib::xy(pArray.at(0).x * scale, hwlib.at(0).y * scale);
        pArray.at(1) = hwlib::xy(pArray.at(1).x * scale, hwlib.at(1).y * scale);
        pArray.at(2) = hwlib::xy(pArray.at(2).x * scale, hwlib.at(2).y * scale);
        pArray.at(3) = hwlib::xy(pArray.at(3).x * scale, hwlib.at(3).y * scale);*/
    }
}

void GraphicsObjectSquare::draw()  {
    window.clear();

    auto line1 = hwlib::line( 
        pArray.at(0),
        pArray.at(1), 
        window.foreground 
    );
    line1.draw( window );

    auto line2 = hwlib::line( 
        pArray.at(1),
        pArray.at(2), 
        window.foreground 
    );
    line2.draw( window );

    auto line3 = hwlib::line( 
        pArray.at(2),
        pArray.at(3), 
        window.foreground 
    );
    line3.draw( window );

    auto line4 = hwlib::line( 
        pArray.at(3),
        pArray.at(0), 
        window.foreground 
    );
    line4.draw( window );

    window.flush(); 
    hwlib::wait_ms( 50000 );
}

void GraphicsObjectSquare::setSquareData(hwlib::xy p, unsigned int width, unsigned int height, const unsigned int degrees)  { 
    pArray.at(0) = p;
    pArray.at(1) = hwlib::xy(pArray.at(0).x + width, pArray.at(0).y);
    pArray.at(2) = hwlib::xy(pArray.at(0).x + width, pArray.at(0).y + height);
    pArray.at(3) = hwlib::xy(pArray.at(0).x, pArray.at(0).y + height);
    setRotationInDegrees(degrees);
}

void GraphicsObjectSquare::rotatePoint(hwlib::xy& xyp, hwlib::xy pivot)    {
    float angleInRadians = degrees * pi()/180;
    
    xyp = xyp - pivot;
    unsigned int xi = pivot.x + (std::cos(angleInRadians) * xyp.x - std::sin(angleInRadians)  * xyp.y);
    unsigned int yi = pivot.y + (std::sin(angleInRadians) * xyp.x + std::cos(angleInRadians) * xyp.y);
    xyp = hwlib::xy(xi, yi);
}

GraphScene::GraphScene(hwlib::glcd_oled& window, hwlib::xy startPoint, unsigned int width, unsigned int height) : GraphicsObject(window), startPoint(startPoint), 
    x_axis(hwlib::xy(0, height - 2), hwlib::xy(width, height - 2)),
    y_axis(hwlib::xy(1, height), hwlib::xy(1, 0)),
    width(width),
    height(height){
        //setScale(2);
}

void GraphScene::draw() {
    window.clear();
    //hwlib::xy(0, 62);
    //hwlib::line(hwlib::xy(0, height - 2), hwlib::xy(width, height - 2)).draw(window);
    //hwlib::line(hwlib::xy(5, 64), hwlib::xy(5, 0)).draw(window);
    x_axis.draw( window );
/*    for(uint8_t i = 1; i < points; i++) {
         hwlib::line(hwlib::xy(pArray[i - 1].x * everyValue, pArray[i - 1].y * everyValue), 
            hwlib::xy(pArray[i].x * everyValue, pArray[i].y * everyValue)).draw(window);
    }*/
/*    for (uint8_t i = 1; i < points; i++)    {
        hwlib::line(hwlib::xy(pArray[i - 1]).x * everyValue)
    }*/
    // needs range
    for(uint8_t i = 1; i < 20; i++) {
        hwlib::line(hwlib::xy((i-1) * width / 20, height - (pArrayValues[i-1] - min) * (height / my_range)), 
                    hwlib::xy((i) * width / 20, height - (pArrayValues[i] - min) * (height / my_range))).draw(window);
    }
    y_axis.draw( window );
    window.flush(); 
}

void GraphScene::push_back(float value)  {
/*    for(uint8_t i = 0; i < points - 1; i++) {
        pArray[i] = hwlib::xy(60/points * i, pArray[i + 1].y);
    }*/
/*    for(uint8_t i = 0; i < points - 1; i++) {
        pArray[i] = hwlib::xy(60/points * i, pArray[i + 1].y);
    }
    pArray[points - 1] = hwlib::xy(60/points * (points -  1), (int)((value - 20) * range_scale));*/
    for(uint8_t i = 1; i < 20; i++) {
        pArrayValues[i - 1] = pArrayValues[i];
    } 
    pArrayValues[19] = value;
}

void GraphScene::setScale(const float scale)   {
    if (this->scale == scale)
        return;
    else    {
        startPoint = hwlib::xy(startPoint.x * scale, startPoint.y * scale);
        width *= scale;
        height *= scale;
        everyValue *= scale;
        x_axis = hwlib::line(startPoint, hwlib::xy(startPoint.x + width, startPoint.y), window.foreground);
        y_axis = hwlib::line(startPoint,hwlib::xy(startPoint.x, startPoint.y + height), window.foreground);

        for(auto& datapoint : pArray)   {
            datapoint = hwlib::xy(datapoint.x, datapoint.y);
        }
    }
}

void GraphScene::set_range_scale(int min, int max)  {
    range_scale = 30 / (max - min);
    my_range = max - min;
    this->min = min;
}