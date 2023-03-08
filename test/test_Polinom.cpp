
#include "../samples/Polinom.h"

#include <gtest.h>

TEST(Polinom, Add_Monom){
    Polinom t;
    t.add(3,3,3,2);
    double val = t.calc(2,2,2);
    EXPECT_EQ(val,2*512);
}

TEST(Polinom, Erase_Monom){
    Polinom t;
    t.add(3,3,3,2);
    t.add(1,-1,1,1);
    t.erase(3,3,3,2);
    double val = t.calc(2,2,2);
    EXPECT_EQ(val,2);
}

TEST(Polinom, Calc_Less_Than_One){
    Polinom t;
    t.add(-3,-3,-4,1);
    double val = t.calc(2,2,2);
    EXPECT_EQ(val,0.0009765625);
}

TEST(Polinom, Calc_Greater_Than_One){
    Polinom t;
    t.add(3,3,4,2);
    double val = t.calc(2,3,2);
    EXPECT_EQ(val,2*2*2* 3*3*3* 2*2*2*2 * 2);
}

TEST(Polinom, Calc_Empty_Polinom){
    Polinom t;
    double val = t.calc(2,3,2);
    EXPECT_EQ(val,0);
}

TEST(Polinom, Add_Two_Polinom){
    Polinom t1,t2,t3;
    vector<Monom>m;
    constexpr int n = 6;
    for(int i = 0;i<n;i++){
        m.push_back(Monom(i+3,2+i,i-5,1));
    }
    for(int i =0 ;i<n;i++){
        if(i < n/2)
            t1.add(m[i]);
        else
            t2.add(m[i]);
    }
    t3 = t1 + t2;
    for(int i =0;i<n;i++){
        EXPECT_EQ(t3.find(m[i]),true);
    }
}

TEST(Polinom, Subtracting_Two_Polinom){
    Polinom t1,t2,t3;
    vector<Monom>m;
    constexpr int n = 6;
    for(int i = 0;i<n;i++){
        m.push_back(Monom(i+3,2+i,i-5,1));
    }
    for(int i =0 ;i<n;i++){
        if(i < n/2)
            t1.add(m[i]);
        else
            t2.add(m[i]);
    }
    t3 = t1 + t2;
    t3 = t3 - t1;
    EXPECT_EQ(t3==t2,true);
}

TEST(Polinom, Multiply_Two_Polinom){
    //(x^2 + xy + z)*(x+x^2y^2) = x^3 + x^4y^2 + x^2y + x^3y^3 + xz + x^2y^2z
    Polinom t1,t2,t3;
    t1.add(2,0,0,1);
    t1.add(1,1,0,1);
    t1.add(0,0,1,1);
    t2.add(1,0,0,1);
    t2.add(2,2,0,1);
    t3 = t1*t2;
    double val = t3.calc(2,2,2);
    EXPECT_EQ(val,180);
}