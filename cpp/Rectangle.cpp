#include <iostream>
class Rectangle{
private:
        double width;
        double height;
public:
    //提供构造函数
    Rectangle(double w,double h){
        width = w;
        height = h;
    }
    //拷贝构造函数
    Rectangle(const Rectangle &other){
        width = other.width;
        height = other.height;
    }
    //赋值运算符重载
    Rectangle& operator=(const Rectangle& other) {
        if (this != &other) {
            width = other.width;
            height = other.height;
        }
        return *this;
    }
    //提供成员函数 area()，计算矩形的面积
    double area() const{
        return width*height;
    }
    //提供成员函数 resize(double scale)，按比例缩放矩形的宽度和高度
    void resize(double scale){
        width  *= scale;
        height *= scale;
    }
    //提供友元函数 bool isSquare(const Rectangle& rect)，判断矩形是否为正方形。
    friend bool isSquare(const Rectangle& rect);
};

bool isSquare (const Rectangle& rect){
    return rect.height == rect.width;
}