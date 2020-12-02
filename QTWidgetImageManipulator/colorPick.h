#ifndef COLORPICK_H
#define COLORPICK_H



class ColorPick
{


public:
    ColorPick():m_RY(0),m_GU(0),m_BV(0){}
    void setR(int value){m_RY = value;};
    void setG(int value){m_GU = value;};
    void setB(int value){m_BV = value;};

    int Bound(int min, int value, int max){
        if (value < min) return min;
        if (value > max) return max;
        return value;}

    void setY(int value){m_RY = value;};
    void setU(int value){m_GU = value;};
    void setV(int value){m_BV = value;};

    void calculateYUV(){m_Y = Bound(0,(m_RY+1.402*m_BV),255);
                        m_U = Bound(0,(m_RY-(0.344*m_GU)-(0.714*m_BV)),255);
                        m_V = Bound(0,(m_RY+1.772*m_GU),255);
                       };

    int getR(){return m_RY;};
    int getG(){return m_GU;};
    int getB(){return m_BV;};

    int getY(){return m_RY;};
    int getU(){return m_GU;};
    int getV(){return m_BV;};

    int getCR(){return m_Y;};
    int getCG(){return m_U;};
    int getCB(){return m_V;};

private:
    int m_RY;
    int m_GU;
    int m_BV;

    int m_Y;
    int m_U;
    int m_V;
};

#endif // COLORPICK_H
