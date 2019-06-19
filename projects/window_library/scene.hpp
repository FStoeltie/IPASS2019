#include <array>

#include "hwlib.hpp"
/*#include "hwlib-graphics-color.hpp"*/
#include <cmath>
constexpr double pi() { return std::atan(1)*4; }

class Point{
public:
    Point(unsigned int x, unsigned int y) : x(x), y(y) {}
private:
    unsigned int x, y;
protected:
    
};

class GraphicsObject {
public:
    void draw() {};
    void setRotationInDegrees(const unsigned int degrees) {
        this->degrees = degrees;
    }
private:
protected:
    GraphicsObject(hwlib::glcd_oled& window) : window(window) {}
    hwlib::glcd_oled& window;
    unsigned int degrees = 0;
};

class VectorGraphicsObject {
public:

    void draw() {}
    void setRotationInDegrees(const unsigned int degrees) {}
private:

protected:
    VectorGraphicsObject() {}
    

};

class GraphicsObjectSquare : public GraphicsObject {
public:
    void setRotationInDegrees(const unsigned int degrees) {
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
    GraphicsObjectSquare(hwlib::glcd_oled& window) : GraphicsObject(window) {}
    void draw()  {
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
    void setSquareData(hwlib::xy p, unsigned int width = 0, unsigned int height = 0, const unsigned int degrees = 0)  { 
        pArray.at(0) = p;
        pArray.at(1) = hwlib::xy(pArray.at(0).x + width, pArray.at(0).y);
        pArray.at(2) = hwlib::xy(pArray.at(0).x + width, pArray.at(0).y + height);
        pArray.at(3) = hwlib::xy(pArray.at(0).x, pArray.at(0).y + height);
        setRotationInDegrees(degrees);
    }
    std::array<hwlib::xy, 4> pArray = {hwlib::xy(0, 0), hwlib::xy(0,0)};
    void rotatePoint(hwlib::xy& xyp, hwlib::xy pivot)    {
        float angleInRadians = degrees * pi()/180;
        
        xyp = xyp - pivot;
        unsigned int xi = pivot.x + (std::cos(angleInRadians) * xyp.x - std::sin(angleInRadians)  * xyp.y);
        unsigned int yi = pivot.y + (std::sin(angleInRadians) * xyp.x + std::cos(angleInRadians) * xyp.y);
        xyp = hwlib::xy(xi, yi);
    }

private:
protected:
};

class Scene {
public:
    Scene() {}
private:
protected:  

};