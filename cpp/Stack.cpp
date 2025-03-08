#include<iostream>

template<typename T>
class Stack{
private:
    T* data = nullptr;
    int Capacity;
    int currentSize;

    void resize(){
        int newCapacity = ( Capacity==0 )? 1 : Capacity*2;
        T* newdata = new T[newCapacity];

        for(int i = 0; i<currentSize; i++){
            newdata[i] = data[i];
        }
        delete[] data;
        this->Capacity = newCapacity;
        data = newdata;
    }

public:
    //默认构造函数
    Stack(){
        this->Capacity = 0;
        this->currentSize = 0;
    }

    //拷贝构造函数
    Stack(const Stack& other){
        this->Capacity = other.Capacity;
        this->currentSize = other.currentSize;
        this->data = new T[other.Capacity];
        for(int i=0;i<this->currentSize;i++){
            this->data[i] = other.data[i];
        }
    }

    //析构函数
    ~Stack(){
        if(this->data != nullptr){
            delete[] data;
            this->Capacity = 0;
            this->currentSize = 0;
            this->data = nullptr;
        }
    }

    //赋值运算符重载
    Stack& operator=(const Stack& other){
        if(this->data != nullptr){
            delete[] data;
            this->Capacity = 0;
            this->currentSize = 0;
            this->data = nullptr;
        }

        this->Capacity = other.Capacity;
        this->currentSize = other.currentSize;
        this->data = new T[other.Capacity];
        for(int i=0;i<this->currentSize;i++){
            this->data[i] = other.data[i];
        }

        return *this;
    }

    //push
    void push(const T& value){
        if(Capacity == currentSize) resize();
        data[currentSize++] = value;
    }
    
    //isEmpty
    bool isEmpty() const{
        return currentSize == 0;
    }

    //pop
    void pop(){
        if(isEmpty()){
            throw std::out_of_range("Stack is empty");
        }
        currentSize--;
    }

    //top
    T& top() {
        if (isEmpty()) {
            throw std::out_of_range("Stack is empty");
        }
        return data[currentSize - 1];
    }

    //size
    int size(){
        return currentSize;
    }
};


int main(){

    Stack<int> s;
    s.push(1);
    s.push(2);
    s.push(3);

    Stack<int> s2 = s; // 调用拷贝构造函数
    s2.push(4);

    Stack<int> s3;
    s3 = s2; // 调用赋值运算符

    while (!s3.isEmpty()) {
        std::cout << s3.top() << " ";
        s3.pop();
    }
    // 输出：4 3 2 1
    
    //异常处理
    try{
        Stack<int> EmptyStack;
        EmptyStack.pop();
    }catch (const std::out_of_range& e) {
        std::cout << "\nError: " << e.what();
    }

    return 0;
}