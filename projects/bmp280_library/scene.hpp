#include <array>

#include "hwlib.hpp"
/*#include "hwlib-graphics-color.hpp"*/
#include <algorithm>
#include <cmath>

#ifndef __SCENE_HPP__
#define __SCENE_HPP__
constexpr double pi() { return std::atan(1)*4; }

class GraphicsObject {
public:
    void draw() {};
    void setRotationInDegrees(const unsigned int degrees) {
        this->degrees = degrees;
    }
    void setScale(const float scale)    {
        this->scale = scale;
    }
private:
protected:
    GraphicsObject(hwlib::glcd_oled& window) : window(window) {}
    hwlib::glcd_oled& window;
    unsigned int degrees = 0;
    float scale = 1.0;
};

class VectorGraphicsObject {
public:

    void draw() {}
    void setRotationInDegrees(const unsigned int degrees) {}
private:

protected:
    VectorGraphicsObject() {}

};

/*
    \class GraphicsObjectSquare
    \brief This class represents a simple square object. It contains helper methods to rotate and scale the square.
    \author Ferdi Stoeltie
    \version 1.0
*/
class GraphicsObjectSquare : public GraphicsObject {
public:

    /*
        \brief Constructor that takes in a glc_oled window to draw to.
        \param[in] window The oled window to draw to.
    */
    GraphicsObjectSquare(hwlib::glcd_oled& window) : GraphicsObject(window) {}

    /*
        \brief Rotates the square to the desired angle in degrees.
        \paramp[in] degrees desired degrees to rotate the square from it's base.
    */
    void setRotationInDegrees(const unsigned int degrees);

    /*
        \brief Sets the scale for square object.
        \param[in] scale Floating point to scale to.
    */
    void setScale(const float scale);

    /*
        \brief Calling this method will draw the object to screen.
    */
    void draw();

    /*
        Sets the square data (x, y, width, height and angle).
        param[in] p The x,y positions of where to start drawing the square.
        param[in] width Width of the square.
        param[in] height Height of the square.
        param[in] degrees Angle of the square in degrees.
    */
    void setSquareData(hwlib::xy p, unsigned int width = 0, unsigned int height = 0, const unsigned int degrees = 0);
    

protected:
private:
    void rotatePoint(hwlib::xy& xyp, hwlib::xy pivot);
    std::array<hwlib::xy, 4> pArray = {hwlib::xy(0, 0), hwlib::xy(0,0)};
};

class BaseScene{
public:
    BaseScene() {}
protected:  
private:

};
/*
    \class GraphScene
    \brief This class creates a graph with several datapoints that are connected with lines.
    \author Ferdi Stoeltie
    \version 0.9
*/
class GraphScene : public GraphicsObject{
public:
    /*
        \brief Constructor for creating the Graph Scene.
        \param[in] window The hwlib glcd_oled window that it will draw to.
        \param[in] startPoint point of where to start the Graph coordinates.
        \param[in] width Width of the graph.
        \param[in] height Height of the graph.
    */
    GraphScene(hwlib::glcd_oled& window, hwlib::xy startPoint, unsigned int width, unsigned int height);
    /*
        \brief Draws the graph and it's datapoints to the display.
    */
    void draw();
    /*
        \brief Pushes a new value onto the stack. The oldest one is pushed out of the stack.
        \param[in] value The new value to push onto the stack.
    */
    void push_back(float value);
    /*
        \brief Scales the graph.
        \param[in] scale The scale that is used to scale the GraphScene with (multiplaction).
    */
    void setScale(const float scale);
    /*
        \brief Method that takes in the minimum and maximum value that the values of the datapoints can have.
                Useful to change distribution of datapoints on the y axis.
        \param[in] min The minimum value that a datapoint can have.
        \param[out] max The maximum value that a datapoint can have.
    */
    void set_range_scale(int min, int max);
protected:
      
private:
    const uint8_t points = 10;
    int range_scale = 1;
    int min = 20;
    hwlib::xy pArray[10] = {hwlib::xy(0, 0), hwlib::xy(0, 0), hwlib::xy(0, 0), hwlib::xy(0, 0), hwlib::xy(0, 0),
                            hwlib::xy(0, 0), hwlib::xy(0, 0), hwlib::xy(0, 0), hwlib::xy(0, 0), hwlib::xy(0, 0)};
    hwlib::xy startPoint;
    hwlib::line x_axis;
    hwlib::line y_axis;
    unsigned int width; 
    unsigned int height;
    unsigned int everyValue = 1;
};

#endif // __SCENE_HPP__